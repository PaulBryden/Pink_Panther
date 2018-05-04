//
// Created by green on 15/04/18.
//

#ifndef FOO_GETNODES_H
#define FOO_GETNODES_H

#include <string>
#include "../Data/NodeContainer.h"
#include "../Data/TargetNode.h"
#include "Interfaces/INodeReaderModule.h"
#include <cpprest/http_client.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

class HttpGetNodeReaderModule : public INodeReaderModule
{

public:
    HttpGetNodeReaderModule(std::string url);

    std::shared_ptr<node::NodeContainer> readNodes();

    bool isRunning();

    void deInitialize();

    void initialize();

private:
    bool m_isRunning;

    web::json::value GetRequest();

    const std::string m_Url;
    boost::mutex g_i_mutex;

};


#endif //FOO_GETNODES_H
