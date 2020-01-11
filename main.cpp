#include <iostream>
#include "UnixConfig/config.h"
#include "KnxdObject/knxdconnection.h"
#include "KnxdObject/utils.h"
#include "KnxdObject/dpt.h"
#include <ctime>
#include "ines.h"

using namespace std;

static bool running = true;
static std::map<uint16_t, uint16_t> gadDpt;
static std::map<uint16_t, std::string> gadInesCmd;
static std::map<uint16_t, double> gadInesVal;
static Ines ines;

void gadRx(uint16_t src, uint16_t dest, unsigned char *payload)
{
    switch(getCmd(payload))
    {
    case CMD::READ:
        std::cout << "READ:";
        break;

    case CMD::WRITE:
        std::cout << "WRITE:";
        break;

    case CMD::RESPONSE:
        std::cout << "RESPONSE:";
        break;

    case CMD::MEMWRITE:
        std::cout << "MEMWRITE:";
        break;
    }
    std::cout << phyToStr(src) << "->" << gadToStr(dest);

    uint16_t dpt = gadDpt[dest];

    if((dpt >> 8) == 9)
    {
        float val;
        payload_to_dpt9(payload, &val);
        std::cout << " " << val << "°C";
    }
    else
    {
        std::cout << " (" << (dpt >> 8) << " " << dpt << ")";
    }

    std::cout << std::endl;
}

int main()
{
    fd_set readfds;
    Config conf("KnxInes");
    KnxdConnection knxd(strToPhy(conf["physical_address"]) ,conf["knxd_url"]);

    knxd.registerCallback(gadRx);
    for(const std::string &var: conf.array("knxines_vars"))
    {
        auto s = split(var, '|');
        if(s.size() == 3)
        {
            uint16_t gad = strToGad(s[1]);
            knxd.subscribe(gad);
            gadDpt[gad] = strToDpt(s[0]);
            gadInesCmd[gad] = s[2];
        }
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
            std::cout << "POLL:" << std::asctime(std::localtime(&current)) << std::endl;
            last_time = current;

            knxd.keepalive();
            for (auto const& x : gadInesCmd)
            {
                uint16_t gad = x.first;
                double val = ines.get(x.second);
                if(gadInesVal.count(gad) == 0) gadInesVal[gad] = 0;
                if(std::abs(gadInesVal[gad] - val) > 0.1)
                {
                    gadInesVal[gad] = val;
                    knxd.write(gad, gadDpt[gad], val);
                }
            }


            //static std::map<uint16_t, std::string> gadInesCmd;
        }

        if (FD_ISSET(knxdfd, &readfds))
        {
            knxd.process();
        }
    }
    return 0;
}
