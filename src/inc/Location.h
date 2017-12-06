//
// Created by green on 04/12/17.
//

#ifndef FOO_LOCATION_H
#define FOO_LOCATION_H

class Location{

public:
    web::json::value ToJson(){
        using namespace web;
        //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_Rssi)+" }"));
        json::value response = json::value::object();
        response["X"] = json::value::number(x);
        response["Y"] = json::value::number(y);
        response["Z"] = json::value::number(z);
        response["time(ns)"] = json::value::number(time);
        return response;
    }
    double x;
    double y;
    double z;
    double time;
};
#endif //FOO_LOCATION_H
