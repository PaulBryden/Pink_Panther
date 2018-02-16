//
// Created by green on 04/12/17.
//

#ifndef FOO_LOCATION_H
#define FOO_LOCATION_H

class Location{

public:
    Location(){

    }


    web::json::value ToJson(){
        using namespace web;
        //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_Rssi)+" }"));
        json::value response = json::value::object();
        response["X"] = json::value::number(xCoord);
        response["Y"] = json::value::number(yCoord);
        response["Z"] = json::value::number(zCoord);
        response["time(ns)"] = json::value::number(time);
        return response;
    }


    std::atomic<double> xCoord;
    std::atomic<double> yCoord;
    std::atomic<double> zCoord;
    std::atomic<double> time;
};
#endif //FOO_LOCATION_H
