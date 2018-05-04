//
// Created by green on 27/11/17.
//

#ifndef FOO_TARGET_NODE_H
#define FOO_TARGET_NODE_H

#include <boost/shared_ptr.hpp>
#include "Node.h"
#include "../Interfaces/INode.h"
#include "../KalmanFilterRSSI.h"

class TargetNode : public INode
{
private:

    std::shared_ptr<INode> m_Node;
    std::atomic<double> m_XCoord;
    std::atomic<double> m_YCoord;
    std::atomic<double> m_ZCoord;
    KalmanFilterRSSI m_Kalman_rssi;
    double m_Kalman_RSSI_Val;
    int m_rssi_cal;

    void updateKalmanRSSI();

    void ParseJson(web::json::value node);

    boost::mutex g_i_mutex;
public:
    TargetNode(std::shared_ptr<INode> Node, double XCoord, double YCoord, double ZCoord, double RssiCalib);

    TargetNode(web::json::value node);

    TargetNode(web::json::value node, double calibrationParam, double Q, double R);

    web::json::value ToJson();

    void Update(std::shared_ptr<INode> Node);

    int getRSSI();

    std::string getSSID();

    double getXCoord();

    double getYCoord();

    double getZCoord();

    double GetDistance();

    void setRecentlyUpdated(bool status);

    bool getRecentlyUpdated();

    std::string getMAC();

    inline bool operator<(TargetNode &c2)
    {
        return (this->getRSSI() < c2.getRSSI());
    }

    inline bool operator>(TargetNode &c2)
    {
        return (this->getRSSI() > c2.getRSSI());
    }

    inline bool operator<=(TargetNode &c2)
    {
        return (this->getRSSI() <= c2.getRSSI());
    }

    inline bool operator>=(TargetNode &c2)
    {
        return (this->getRSSI() >= c2.getRSSI());
    }

};


#endif //FOO_TARGET_NODE_H
