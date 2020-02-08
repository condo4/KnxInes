#ifndef INES_H
#define INES_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <ctime>
#include "KnxdObject/knxdconnection.h"
#include "KnxdObject/knxdevice.h"

class JSONValue;


class Ines: public KnxDevice
{
    std::string query_json(const std::string &query);
    std::shared_ptr<JSONValue> m_jsonv;

public:
    Ines();
    void process();
    double getFromDevice(std::string param);
    double sendToDevice(uint16_t gad, unsigned char *payload) override;
};

#endif // INES_H
