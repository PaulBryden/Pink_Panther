//
// Created by green on 27/11/17.
//

#ifndef FOO_TARGET_NODE_H
#define FOO_TARGET_NODE_H

#include <boost/shared_ptr.hpp>
#include "Node.h"
#include "INode.h"
class Target_Node: public INode  {
private:
    std::shared_ptr<INode> m_Node;
    std::atomic<double> m_XCoord;
    std::atomic<double> m_YCoord;
    std::atomic< double> m_ZCoord;
    std::atomic<double> m_RssiCalib;
    std::deque<int> m_RssiVec;
public:
    Target_Node(std::shared_ptr<INode> Node, double XCoord, double YCoord, double ZCoord, double RssiCalib);
    Target_Node(web::json::value node);
    double CalculateDistance();
    web::json::value ToJson();
    void Update(std::shared_ptr<INode> Node);
    int getRSSI();
    double getChannel();
    std::string getSSID();
    double getXCoord();
    double getYCoord();
    double getZCoord();

};


#endif //FOO_TARGET_NODE_H
