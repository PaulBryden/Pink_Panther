//
// Created by green on 27/11/17.
//

#ifndef FOO_TARGET_NODE_H
#define FOO_TARGET_NODE_H

#include <boost/shared_ptr.hpp>
#include "Node.h"
#include "INode.h"
#include "KamalFilterRSSI.h"

class Target_Node: public INode  {
private:
    std::shared_ptr<INode> m_Node;
    std::atomic<double> m_XCoord;
    std::atomic<double> m_YCoord;
    std::atomic< double> m_ZCoord;
    KamalFilterRSSI m_Kalman_distance;
    KamalFilterRSSI m_Kalman_rssi;
    double m_Kalman_RSSI_Val;
    int m_rssi_cal;
public:
    Target_Node(std::shared_ptr<INode> Node, double XCoord, double YCoord, double ZCoord, double RssiCalib);
    Target_Node(web::json::value node);
    double CalculateDistance();
    web::json::value ToJson();
    void Update(std::shared_ptr<INode> Node);
    int getRSSI() const;
    std::string getSSID();
    double getXCoord();
    double getYCoord();
    double getZCoord();
    void updateKalmanRSSI();
    double getKalmanRSSI();
    std::string getMAC();
    std::atomic<double> m_Kalman_Distance;

    inline bool operator< (const Target_Node & c2)
    {
        return (this->getRSSI() < c2.getRSSI());
    }
    inline bool operator> (const Target_Node & c2)
    {
        return (this->getRSSI() > c2.getRSSI());
    }
    inline bool operator<= (const Target_Node & c2)
    {
        return (this->getRSSI() <= c2.getRSSI());
    }
    inline bool operator>= (const Target_Node & c2)
    {
        return (this->getRSSI() >= c2.getRSSI());
    }

};


#endif //FOO_TARGET_NODE_H
