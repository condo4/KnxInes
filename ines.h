#ifndef INES_H
#define INES_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <ctime>

class JSONValue;

class Ines
{
    std::string m_hostname;
    unsigned short m_port {0};
    std::string query_json(const std::string &query);
    std::map<std::string, std::pair<std::time_t, std::shared_ptr<JSONValue>>> m_jsonv;

public:
    Ines();

    void setUrl(const std::string &url);
    void setHostname(const std::string &hostname);
    void setPort(unsigned short port);
    double get(std::string param);
};

#endif // INES_H
