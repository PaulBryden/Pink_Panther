//
// Created by green on 25/10/17.
//

#include "inc/Nodes.h"
#include <iostream>
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