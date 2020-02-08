#ifndef INES_H
#define INES_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <ctime>
#include "KnxdObject/knxdconnection.h"

class JSONValue;


class Ines
{
    std::string m_hostname;
    unsigned short m_port {0};
    std::string query_json(const std::string &query);
    std::pair<std::time_t, std::shared_ptr<JSONValue>> m_jsonv;
    std::map<uint16_t, uint16_t> m_gadDpt;
    std::map<uint16_t, std::string> m_gadInesCmd;
    std::map<uint16_t, double> m_gadInesVal;
    std::map<uint16_t, std::string> m_gadFlags;
    KnxdConnection &m_knxd;

public:
    Ines(KnxdConnection &knxd);

    void setUrl(const std::string &url);
    void setHostname(const std::string &hostname);
    void setPort(unsigned short port);
    double get(std::string param);
    void rx(uint16_t src, uint16_t gad, unsigned char *payload);
    void addConf(const std::string &var);
    void process();
};

#endif // INES_H
