//
// Created by green on 15/04/18.
//

#include "inc/Modules/HttpGetNodeReaderModule.h"
HttpGetNodeReaderModule::HttpGetNodeReaderModule(std::string url):m_Url(url){
}


web::json::value HttpGetNodeReaderModule::GetRequest(){
    using namespace web::http::client;
    using namespace web::http;
    http_client client(U(m_Url));
    // Build request URI and start the request.
 //   std::cout << client.request(methods::GET, "").get().extract_json().get().as_string() << std::endl;
    return client.request(methods::GET, "").get().extract_json().get();
}

std::shared_ptr<node::NodeContainer> HttpGetNodeReaderModule::readNodes()
{
    std::shared_ptr<node::NodeContainer> p_NodeList;
    p_NodeList = std::make_shared<node::NodeContainer>();
    web::json::value v = GetRequest()["ap"];
    //std::cout <<v.String// parse the resultant string stream.
    for(int i=0;i<v.as_array().size();i++){
        try {
            std::shared_ptr<TargetNode> newNode(std::make_shared<TargetNode>(v.as_array().at(i)));
            p_NodeList->AddNode(newNode);
            p_NodeList->PrintNodes();
        }catch(std::exception e){
            std::cout << "Error Node: " << i << " Invalid. Discarding..." <<std::endl;
            }
        //std::cout << "AFTER PARSING " << i <<": " << v.as_array()[i].serialize();
    }
    ;
    return p_NodeList;
}
