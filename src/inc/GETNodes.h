//
// Created by green on 15/04/18.
//

#ifndef FOO_GETNODES_H
#define FOO_GETNODES_H
#include <string>
#include "Node_Container.h"
#include "Target_Node.h"
#include <cpprest/http_client.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

class GETNodes {

public:
    GETNodes(std::string url);
    std::shared_ptr<node::Node_Container> get();
private:
    web::json::value GetRequest();
    const std::string m_Url;

};


#endif //FOO_GETNODES_H
