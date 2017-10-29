//
// Created by green on 25/10/17.
//

#include "inc/Nodes.h"
#include "../../build_panther/usr/local/include/cpprest/json.h"
#include <iostream>

using namespace node;
using namespace std;
using namespace web;
Nodes::Nodes() : m_Scanner(m_Nodes){

}
void Nodes::RefreshNodes(){
    m_Nodes.clear();
    m_Scanner.Scan();
}

void Nodes::PrintNodes(){
    for (auto const& i : m_Nodes) {
        std::cout << i->m_name;
        std::cout << i->m_Rssi;
        std::cout << "/n";

    }
}

web::json::value Nodes::ToJson() {
    web::json::value yourJson;
    yourJson[U("Nodes")] = web::json::value::array(m_Nodes.size());
    int i = 0;
    for (i = 0; i < m_Nodes.size(); i++) {

    yourJson[U("Nodes")].as_array()[i] = web::json::value(m_Nodes[i]->ToJson());
}
    auto aValue = yourJson.at(U("Nodes"))[0].at(U("RSSI"));
    cout << aValue;
    return yourJson;
}

