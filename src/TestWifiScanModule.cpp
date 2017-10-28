//
// Created by green on 28/10/17.
//

#include "inc/Nodes.h"
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>


int main(int argc, wchar_t *argv[])
{

    Nodes* NodeList;
    NodeList = new Nodes();
    while(true)
    {
        NodeList->RefreshNodes();
        NodeList->PrintNodes();
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));

    }



}