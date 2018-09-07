//
// Created by green on 04/12/17.
//

#ifndef FOO_LOCATION_H
#define FOO_LOCATION_H

class Location
{

public:
    Location(double x = 0, double y = 0, double z = 0, double calculationTime = 0) : xCoord(x), yCoord(y), zCoord(z),
                                                                                     m_calculationTime(
                                                                                             calculationTime)
    {

    }


    web::json::value ToJson()
    {
        using namespace web;
        json::value response;
        response["X"] = json::value::number(xCoord);
        response["Y"] = json::value::number(yCoord);
        response["Z"] = json::value::number(zCoord);
        response["time(ns)"] = json::value::number(m_calculationTime);
        return response;
    }

    void updateCoords(double x, double y, double z)
    {
        xCoord = x;
        yCoord = y;
        zCoord = z;

    }

    double xCoord;
    double yCoord;
    double zCoord;
    double m_calculationTime;
};

#endif //FOO_LOCATION_H
