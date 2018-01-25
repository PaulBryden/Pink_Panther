//
// Created by green on 04/12/17.
//

#ifndef FOO_LOCATIONMODULE_H
#define FOO_LOCATIONMODULE_H
#include "Node_Container.h"
#include "Target_Node.h"
#include "Location.h"
#include <boost/chrono/chrono.hpp>
#include <boost/timer/timer.hpp>
class locationModule {
public:
    locationModule();
    void CalculateLocations(std::shared_ptr<node::Node_Container> nodes);
    web::json::value ToJson();
private:
    boost::mutex g_i_mutex;
    std::shared_ptr<Location> m_dgelsLoc;
    std::shared_ptr<Location> m_dgesvLoc;
    std::shared_ptr<Location> m_dgetrsLoc;
    void  calculateDgels(std::shared_ptr<node::Node_Container> nodes);
    void  calculateDgesvDgetrs(std::shared_ptr<node::Node_Container> nodes);


};


#endif //FOO_LOCATIONMODULE_H
