//
// Created by green on 27/11/17.
//

#include <cmath>
#include "inc/Data/TargetNode.h"

using namespace std;

void TargetNode::Update(std::shared_ptr<INode> Node)
{
    boost::mutex::scoped_lock lock(g_i_mutex);
    m_Node = Node;
    updateKalmanRSSI();
}

TargetNode::TargetNode(web::json::value node) : m_Kalman_rssi(0.001, 2.36, -32)
{
    this->ParseJson(node);

}

TargetNode::TargetNode(web::json::value node, double calibrationParam, double Q, double R) : m_Kalman_rssi(Q, R, calibrationParam)
{
    this->ParseJson(node);
}

void TargetNode::ParseJson(web::json::value node)
{
    boost::mutex::scoped_lock lock(g_i_mutex);
    m_Node = std::make_shared<Node>(node);
    try
    {
        m_XCoord = node["x"].as_double();
    }
    catch (std::exception e)
    {
        printf("Error:: Cannot parse XCoord from JSON. Please check Syntax");
        std::exception ParseError;
        throw (ParseError);
    }
    try
    {
        m_YCoord = node["y"].as_double();
    }
    catch (std::exception e)
    {
        printf("Error:: Cannot parse YCoord from JSON. Please check Syntax");
        std::exception ParseError;
        throw (ParseError);
    }
    try
    {
        m_ZCoord = node["z"].as_double();
    }
    catch (std::exception e)
    {
        printf("Error:: Cannot parse ZCoord from JSON. Please check Syntax");
        std::exception ParseError;
        throw (ParseError);
    }
    try
    {
        m_rssi_cal = node["cal"].as_integer();
    }
    catch (std::exception e)
    {
        printf("Error:: Cannot parse RSSICalib from JSON. Please check Syntax");
        std::exception ParseError;
        throw (ParseError);
    }
}


web::json::value TargetNode::ToJson()
{
    using namespace web;
    json::value response = m_Node->ToJson();

    response["XCoord"] = json::value::number(m_XCoord);
    response["YCoord"] = json::value::number(m_YCoord);
    response["ZCoord"] = json::value::number(m_ZCoord);
    response["RSSICalib"] = json::value::number(m_rssi_cal);
    response["Distance"] = json::value::number(GetDistance());
    response["RSSIFiltered"] = json::value::number(m_Kalman_RSSI_Val);

    return response;


}

int TargetNode::getRSSI()
{

    return m_Node->getRSSI();
}


std::string TargetNode::getSSID()
{

    return m_Node->getSSID();
}

void TargetNode::updateKalmanRSSI()
{

    m_Kalman_RSSI_Val = m_Kalman_rssi.kalmanUpdate(this->getRSSI());
}

double TargetNode::GetDistance()
{

    try
    {
        double Power = (-(static_cast<double>(m_Kalman_RSSI_Val - (m_rssi_cal)) / (10.0 * 2.35)));

        // printf("Calculating Distance: %f\n", pow(10.0, Power));

        return pow(10.0, Power);
    } catch (std::exception X)
    {
        printf("Error:: No Calibration Data Available");
        return 0;
    }
}

double TargetNode::getXCoord()
{
    return m_XCoord;
}

double TargetNode::getYCoord()
{
    return m_YCoord;
}

double TargetNode::getZCoord()
{
    return m_ZCoord;
}

std::string TargetNode::getMAC()
{
    return m_Node->getMAC();
}

bool TargetNode::getRecentlyUpdated()
{
    m_Node->getRecentlyUpdated();
}

void TargetNode::setRecentlyUpdated(bool status)
{
    m_Node->setRecentlyUpdated(status);
}
