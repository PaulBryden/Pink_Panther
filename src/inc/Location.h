//
// Created by green on 04/12/17.
//

#ifndef FOO_LOCATION_H
#define FOO_LOCATION_H

class Location{

public:
    Location(){
        xDeque.resize(12);
        yDeque.resize(12);
        zDeque.resize(12);

    }


    web::json::value ToJson(){
        using namespace web;
        //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_Rssi)+" }"));
        json::value response = json::value::object();
        response["X"] = json::value::number(getMedian(xDeque));
        response["Y"] = json::value::number(getMedian(yDeque));
        response["Z"] = json::value::number(getMedian(zDeque));
        response["time(ns)"] = json::value::number(time);
        return response;
    }
    double getMedian(std::deque<double>& deque){
        std::deque<double> y(deque);
        std::sort(y.begin(),y.end());
        return y[((deque.size()/2))];

    }
    void pushX(double val){
        if (xDeque.size()==12){
            xDeque.pop_front();
        }
        xDeque.push_back(val);
    }

    void pushY(double val){
        if (yDeque.size()==12){
            yDeque.pop_front();
        }
        yDeque.push_back(val);
    }

    void pushZ(double val){
        if (zDeque.size()==12){
            zDeque.pop_front();
        }
        zDeque.push_back(val);
    }

    std::deque<double> xDeque;
    std::deque<double> yDeque;
    std::deque<double> zDeque;
    double time;
};
#endif //FOO_LOCATION_H
