//
// Created by green on 15/04/18.
//

#include "inc/GETNodes.h"
GETNodes::GETNodes(std::string url):m_Url(url){
}


web::json::value GETNodes::GetRequest(){
    using namespace web::http::client;
    using namespace web::http;
    http_client client(U(m_Url));
    // Build request URI and start the request.
 //   std::cout << client.request(methods::GET, "").get().extract_json().get().as_string() << std::endl;
    return client.request(methods::GET, "").get().extract_json().get();
}

std::shared_ptr<node::Node_Container> GETNodes::get()
{
    std::shared_ptr<node::Node_Container> p_NodeList;
    p_NodeList = std::make_shared<node::Node_Container>();
    web::json::value v = GetRequest()["ap"];
    //std::cout <<v.String// parse the resultant string stream.
    for(int i=0;i<v.as_array().size();i++){
        try {
            std::shared_ptr<Target_Node> newNode(std::make_shared<Target_Node>(v.as_array().at(i)));
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
