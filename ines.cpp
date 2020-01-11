#include "ines.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <map>
#include "KnxdObject/utils.h"
#include "JSON.h"

static inline std::string readline(int sock)
{
    std::string result;
    result.reserve(128);

    char cur = 0;
    while ( cur != '\n' && (read(sock, &cur, 1) > 0 ))
    {
        if(cur != '\r' && cur != '\n')
            result.push_back(cur);
    }
    return result;
}

static std::map<int, std::string> INES_STATUS = {
    {0, "Off"},
    {1, "Pause"},
    {2, "Vérification"},
    {3, "Préchauffage"},
    {4, "Chargement de pellets"},
    {5, "Test d'allumage"},
    {6, "En chauffe"},
    {9, "Refroidissement du fluide caloporteur"},
    {10, "Arrêt de la flamme"},
    {11, "Brûlage des derniers pellets"},
    {12, "Refroidissement"},
    {241, "CHIMNEY ALARM"},
    {243, "GRATE ERROR"},
    {244, "NTC2 ALARM"},
    {245, "NTC3 ALARM"},
    {247, "DOOR ALARM"},
    {248, "PRESS ALARM"},
    {249, "NTC1 ALARM"},
    {250, "TC1 ALARM"},
    {252, "GAS ALARM"},
    {253, "Plus de pellets"}
};

static std::map<int, std::string> INES_ROOMFAN = {
    {0, "OFF"},
    {1, "1"},
    {2, "2"},
    {3, "3"},
    {4, "4"},
    {5, "5"},
    {6, "Hi"},
    {7, "Auto"}
};

std::string Ines::query_json(const std::string &query)
{
    struct sockaddr_in client;
    int sock;
    std::string result;

    bzero(&client, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons( m_port );
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, m_hostname.data(), &client.sin_addr) <= 0)
    {
       std::cerr << "Invalid address / Address not supported" << std::endl;
       exit(2);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cout << "Error creating socket." << std::endl;
        exit(1);
    }

    if ( connect(sock, reinterpret_cast<struct sockaddr *>(&client), sizeof(client)) < 0 )
    {
        close(sock);
        std::cout << "Could not connect" << std::endl;
        exit(1);
    }

    std::stringstream ss;
    ss << "GET " << query << " HTTP/1.1\r\n"
       << "Accept: application/json\r\n"
       << "\r\n\r\n";
    std::string request = ss.str();

    if (send(sock, request.c_str(), request.length(), 0) != static_cast<int>(request.length()))
    {
        std::cout << "Error sending request." << std::endl;
        exit(1);
    }

    std::string line = " ";

    /* GET HEADERS */
    while(line.size() > 0)
    {
        line = readline(sock);
    }

    /* GET DATA */
    line = " ";
    while(line.size() > 0)
    {
        line = readline(sock);
        if(line[0] == '{')
        {
            result.append(line);
        }
    }
    close(sock);
    return result;
}

Ines::Ines()
{

}

void Ines::setUrl(const std::string &url)
{
    size_t sep = url.find(":");
    setHostname(url.substr(0, sep));
    setPort(static_cast<unsigned short>(std::stoul((url.substr(sep + 1)))));
}

void Ines::setHostname(const std::string &hostname)
{
    m_hostname = hostname;
}

void Ines::setPort(unsigned short port)
{
    m_port = port;
}

double Ines::get(std::string param)
{
    std::vector<std::string> p = split(param.substr(2), '/');
    std::string cmd = p[0];
    std::wstring path = std::wstring(p[1].begin(), p[1].end());
    std::wstring field = std::wstring(p[2].begin(), p[2].end());

    std::string url("/cgi-bin/sendmsg.lua?cmd=");
    url.append(cmd);
    std::string tjson = query_json(url);
    std::shared_ptr<JSONValue> jsv = std::shared_ptr<JSONValue>(JSON::Parse(tjson.c_str()));
    if(jsv == nullptr)
    {
        std::cerr << "Can't use simple json" << std::endl;
    }
    else
    {
        m_jsonv[param] = std::make_pair(std::time(nullptr), jsv);
    }

    auto v = m_jsonv[param].second->Child(path.c_str())->Child(field.c_str());
    if(v == nullptr)
    {
        std::wstring ws(m_jsonv[param].second->Stringify(true));
        std::string log( ws.begin(), ws.end() );
        std::cerr << "Fail to resolve " << param << " in " << log << std::endl;
        return 0;
    }
    return v->AsNumber();
}

