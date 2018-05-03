//
// Created by green on 04/12/17.
//

#ifndef FOO_LOCATION_H
#define FOO_LOCATION_H

class Location{

public:
    Location(double x=0, double y=0, double z=0, double time=0):xCoord(x),yCoord(y),zCoord(z),time(time),xFilter(0.01,2,35),yFilter(0.01,2,20),zFilter(0.01,2,2){
        xCoordKal=xFilter.kalmanUpdate(35);
        yCoordKal=xFilter.kalmanUpdate(20);
        zCoordKal=xFilter.kalmanUpdate(2);
    }


    web::json::value ToJson(){
        using namespace web;
        //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_rssi)+" }"));
        json::value response = json::value::object();
        response["X"] = json::value::number(xCoord);
        response["Y"] = json::value::number(yCoord);
        response["Z"] = json::value::number(zCoord);
        response["time(ns)"] = json::value::number(time);
        return response;
    }
    void updateCoords(double x, double y, double z){
        xCoord=x;
        yCoord=y;
        zCoord=z;
        xCoordKal=xFilter.kalmanUpdate(35);
        yCoordKal=yFilter.kalmanUpdate(20);
        zCoordKal=zFilter.kalmanUpdate(2);

    }

    std::atomic<double> xCoord;
    std::atomic<double> yCoord;
    std::atomic<double> zCoord;
    std::atomic<double> time;
    std::atomic<double> xCoordKal;
    std::atomic<double> yCoordKal;
    std::atomic<double> zCoordKal;
    KamalFilterRSSI xFilter;
    KamalFilterRSSI yFilter;
    KamalFilterRSSI zFilter;
};
#endif //FOO_LOCATION_H
