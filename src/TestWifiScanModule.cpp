//
// Created by green on 28/10/17.
//

#include "inc/Node_Container.h"
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>


int main(int argc, wchar_t *argv[])
{

    node::Node_Container* NodeList;
    NodeList = new node::Node_Container();
    while(true)
    {
        NodeList->RefreshNodes();
        NodeList->PrintNodes();
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));

    }



}