#include <iostream>
#include "UnixConfig/config.h"
#include "KnxdObject/knxdconnection.h"
#include "KnxdObject/utils.h"
#include "KnxdObject/dpt.h"
#include <ctime>
#include "ines.h"

using namespace std;

static bool running = true;


int main()
{
    fd_set readfds;
    Config conf("KnxInes");
    KnxdConnection knxd(strToPhy(conf["physical_address"]), conf["knxd_url"]);
    Ines ines(knxd);

    for(const std::string &var: conf.array("knxines_vars"))
    {
        ines.addConf(var);
    }

    knxd.connect();
    ines.setUrl(conf["knxines_hostname"]);

    int polling = std::stoi(conf["knxines_polling"]);

    std::cout << "Start main loop, polling: " << polling << "s" <<  std::endl;
    std::time_t last_time = 0;
    std::time_t current = std::time(nullptr);
    /* EVENT LOOP */
    while(running)
    {
        //clear the socket set
        struct timeval tv = {polling, 0};
        if(current - last_time < polling)
            tv.tv_sec = polling - (current - last_time);
        int knxdfd = knxd.fd();
        FD_ZERO(&readfds);
        FD_SET(knxdfd, &readfds);
        int max_fd = knxd.fd();

        int activity = select( max_fd + 1 , &readfds, nullptr, nullptr, &tv);

        if ((activity < 0) && (errno != EINTR))
        {
            std::cerr << "select error" << std::endl;
        }

        current = std::time(nullptr);
        if((current - last_time) > polling || activity == 0)
        {
            std::cout << "POLL:" << std::asctime(std::localtime(&current));
            last_time = current;

            knxd.keepalive();
            ines.process();
        }

        if (FD_ISSET(knxdfd, &readfds))
        {
            knxd.process();
        }
    }
    return 0;
}
