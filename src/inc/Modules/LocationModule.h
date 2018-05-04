//
// Created by green on 04/12/17.
//

#ifndef FOO_LOCATIONMODULE_H
#define FOO_LOCATIONMODULE_H

#include "../Data/NodeContainer.h"
#include "../Data/TargetNode.h"
#include "../Data/Location.h"
#include "Interfaces/ILocationModule.h"
#include <boost/chrono/chrono.hpp>
#include <boost/timer/timer.hpp>

class LocationModule : public ILocationModule
{
public:
    LocationModule(std::shared_ptr<node::NodeContainer> &nodes);

    web::json::value GetJson();

    web::json::value BasicJson();

    std::shared_ptr<Location> GetLocation();

    void initialize();

    void deInitialize();

    bool isRunning();

private:
    bool m_isRunning;

    void CalculateLocations();

    boost::mutex g_i_mutex;
    std::shared_ptr<Location> m_dgelsLoc;
    std::shared_ptr<Location> m_dgesvLoc;
    std::shared_ptr<Location> m_dgetrsLoc;

    void calculateDgels(std::shared_ptr<node::NodeContainer> nodes);

    void calculateDgesvDgetrs(std::shared_ptr<node::NodeContainer> nodes);

    std::shared_ptr<node::NodeContainer> GetNodeTargets(std::shared_ptr<node::NodeContainer> tempNodes);

    std::shared_ptr<node::NodeContainer> m_Nodes;


};


#endif //FOO_LOCATIONMODULE_H
