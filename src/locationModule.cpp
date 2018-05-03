//
// Created by green on 04/12/17.
//

#include "inc/locationModule.h"
#include <boost/lexical_cast.hpp>
#include "inc/INode.h"
#include <lapacke.h>
using boost::lexical_cast;

locationModule::locationModule(): m_dgelsLoc(std::make_shared<Location>()),m_dgetrsLoc(std::make_shared<Location>()),m_dgesvLoc(std::make_shared<Location>()){

}
void locationModule::CalculateLocations(std::shared_ptr<node::Node_Container> nodes){
    boost::mutex::scoped_lock lock(g_i_mutex);
    //GetNodeTargets(nodes);
    try {
        calculateDgels(GetNodeTargets(nodes));
        calculateDgesvDgetrs(GetNodeTargets(nodes));
    }catch(std::exception e){
        std::cout <<"Not Enough Nodes in Range." <<std::endl;
    }

}
bool comparePtrToNode(std::shared_ptr<INode> a, std::shared_ptr<INode> b) { return (a->getRSSI() < b->getRSSI()); }

    std::shared_ptr<node::Node_Container> locationModule::GetNodeTargets(std::shared_ptr<node::Node_Container> tempNodes){

    std::shared_ptr<node::Node_Container> targetNodes = std::make_shared<node::Node_Container>();
    std::sort(tempNodes->GetNodes().begin(),tempNodes->GetNodes().end(),[] (std::shared_ptr<INode>& lhs, std::shared_ptr<INode>&  rhs) {
        return lhs->getRSSI() > rhs->getRSSI();
    });
    for (auto &i : tempNodes->GetNodes()) {
        if (targetNodes->GetNodes().size()<4 &&i->getRSSI()!=0 && i->getRecentlyUpdated()){

            targetNodes->AddNode(i);

        }
    }
        for (auto &i : targetNodes->GetNodes()) {

            std::cout << "RSSI ORDERING" << i->getRSSI() << i->getMAC() << std::endl;
        }

        if(targetNodes->GetNodes().size()<4){
            std::exception e;
            throw(e);
        }

    return targetNodes;
}

web::json::value locationModule::ToJson(){
    boost::mutex::scoped_lock lock(g_i_mutex);
    web::json::value response = web::json::value::object();
    response["DGESV"] = m_dgesvLoc->ToJson();
    response["DGETRS"] = m_dgetrsLoc->ToJson();
    response["DGELS"] = m_dgelsLoc->ToJson();
    return response;
}


web::json::value locationModule::BasicJson(){
    boost::mutex::scoped_lock lock(g_i_mutex);
    web::json::value response = web::json::value::object();
    response= m_dgesvLoc->ToJson();
    return response;
}

void  locationModule::calculateDgesvDgetrs(std::shared_ptr<node::Node_Container> nodes){


    if(nodes->GetNodes().size()!=4){
        throw("Error::Too few Nodes!");
    }

    std::vector<std::shared_ptr<Target_Node>> Nodes;

    for (std::shared_ptr<INode> a: nodes->GetNodes()){
        std::cout <<a->getSSID();
        Nodes.push_back(std::static_pointer_cast<Target_Node>(a));

    }

    double A[3][3]={2*(Nodes[1]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[1]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[1]->getZCoord()-Nodes[0]->getZCoord()),
                    2*(Nodes[2]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[2]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[2]->getZCoord()-Nodes[0]->getZCoord()),
                    2*(Nodes[3]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[3]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[3]->getZCoord()-Nodes[0]->getZCoord())};

    double B[3][1]={(pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[1]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[1]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[1]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[1]->getZCoord(),2.0)),
                    (pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[2]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[2]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[2]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[2]->getZCoord(),2.0)),
                    (pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[3]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[3]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[3]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[3]->getZCoord(),2.0))};


    lapack_int info,m,n,lda,ldb,nrhs;
    lapack_int *ipiv;
    m=3;
    n=3;
    nrhs=1;
    lda=3;
    ldb=1;
    ipiv = (lapack_int *)malloc(n*sizeof(lapack_int)) ;
    boost::timer::auto_cpu_timer t;
    info = LAPACKE_dgesv(LAPACK_ROW_MAJOR,n,nrhs,*A,lda,ipiv,*B,ldb);
    t.stop();

    if( info > 0 ) {
        printf( "The diagonal element %i of the triangular factor ", info );
        printf( "of A is zero, so that A does not have full rank;\n" );
        printf( "the least squares solution could not be computed.\n" );
        return;
    }

    printf("Calcuated values.");
    fflush(stdout);
    m_dgesvLoc->updateCoords(B[0][0],B[1][0],B[2][0]);
    /*m_dgesvLoc->xCoord=(B[0][0]);
    m_dgesvLoc->yCoord=(B[1][0]);
    m_dgesvLoc->zCoord=(B[2][0]);*/
    m_dgesvLoc->time=t.elapsed().wall;

    printf("Calculated Location: ");
    std::cout << "X:" << lexical_cast<std::string>(B[0][0]) <<" Y:" << lexical_cast<std::string>(B[1][0]) << " Z:" << lexical_cast<std::string>(B[2][0])  << "Time:" << lexical_cast<std::string>(m_dgesvLoc->time) <<std::endl;


    double B2[3][1]={(pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[1]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[1]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[1]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[1]->getZCoord(),2.0)),
                    (pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[2]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[2]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[2]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[2]->getZCoord(),2.0)),
                    (pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[3]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[3]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[3]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[3]->getZCoord(),2.0))};

    t.resume();
    LAPACKE_dgetrs(LAPACK_ROW_MAJOR,'N', n,nrhs,*A,lda,ipiv,*B2,ldb);

    if( info > 0 ) {
        printf( "The diagonal element %i of the triangular factor ", info );
        printf( "of A is zero, so that A does not have full rank;\n" );
        printf( "the least squares solution could not be computed.\n" );
        return;
    }

    t.stop();
    m_dgetrsLoc->updateCoords(B2[0][0],B2[1][0],B2[2][0]);
    /*m_dgetrsLoc->xCoord=(B2[0][0]);
    m_dgetrsLoc->yCoord=(B2[1][0]);
    m_dgetrsLoc->zCoord=(B2[2][0]);*/

    m_dgetrsLoc->time=t.elapsed().wall-m_dgesvLoc->time;

    std::cout << "X:" << lexical_cast<std::string>(B2[0][0]) <<" Y:" << lexical_cast<std::string>(B2[1][0]) << " Z:" << lexical_cast<std::string>(B2[2][0])  << "Time:" << lexical_cast<std::string>(m_dgetrsLoc->time) <<std::endl;


    return;
}



void locationModule::calculateDgels(std::shared_ptr<node::Node_Container> nodes){

    if(nodes->GetNodes().size()!=4){
        throw("Error::Too few Nodes!");
    }

    std::vector<std::shared_ptr<Target_Node>> Nodes;

    for (std::shared_ptr<INode> a: nodes->GetNodes()){
        std::cout <<a->getSSID();
        Nodes.push_back(std::static_pointer_cast<Target_Node>(a));

    }

    double A[3][3]={2*(Nodes[1]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[1]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[1]->getZCoord()-Nodes[0]->getZCoord()),
                    2*(Nodes[2]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[2]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[2]->getZCoord()-Nodes[0]->getZCoord()),
                    2*(Nodes[3]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[3]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[3]->getZCoord()-Nodes[0]->getZCoord())};

    double B[3][1]={(pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[1]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[1]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[1]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[1]->getZCoord(),2.0)),
                    (pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[2]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[2]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[2]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[2]->getZCoord(),2.0)),
                    (pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[3]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[3]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[3]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[3]->getZCoord(),2.0))};

    lapack_int info,m,n,lda,ldb,nrhs;

    m=3;
    n=3;
    nrhs=1;
    lda=3;
    ldb=1;
    boost::timer::auto_cpu_timer t;
    info=LAPACKE_dgels(LAPACK_ROW_MAJOR,'N',m,n,nrhs,*A,lda,*B,ldb);
    t.stop();
    if( info > 0 ) {
        printf( "The diagonal element %i of the triangular factor ", info );
        printf( "of A is zero, so that A does not have full rank;\n" );
        printf( "the least squares solution could not be computed.\n" );
        return;
    }
    /*m_dgelsLoc->xCoord=(B[0][0]);
    m_dgelsLoc->yCoord=(B[1][0]);
    m_dgelsLoc->zCoord=(B[2][0]);*/

    m_dgelsLoc->updateCoords(B[0][0],B[1][0],B[2][0]);
    m_dgelsLoc->time=t.elapsed().wall;

    printf("Calculated Location Dgels: ");
    std::cout << "X:" << lexical_cast<std::string>(B[0][0]) <<" Y:" << lexical_cast<std::string>(B[1][0]) << " Z:" << lexical_cast<std::string>(B[2][0])  << "Time:" << lexical_cast<std::string>(m_dgelsLoc->time) <<std::endl;

    return ;



}
