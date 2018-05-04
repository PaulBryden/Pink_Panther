//
// Created by green on 04/12/17.
//

#include "inc/Modules/LocationModule.h"
#include <boost/lexical_cast.hpp>
#include "inc/Interfaces/INode.h"
#include <lapacke.h>
#include "inc/Data/Location.h"
#include "inc/Exceptions/UnsupportedOperationException.h"
using boost::lexical_cast;

LocationModule::LocationModule(std::shared_ptr<node::NodeContainer>& nodes): m_Nodes(nodes),m_dgelsLoc(std::make_shared<Location>()),m_dgetrsLoc(std::make_shared<Location>()),m_dgesvLoc(std::make_shared<Location>()){

}
void LocationModule::CalculateLocations(){
    boost::mutex::scoped_lock lock(g_i_mutex);
    try {
        calculateDgels(GetNodeTargets(m_Nodes));
        calculateDgesvDgetrs(GetNodeTargets(m_Nodes));
    }catch(std::exception e){
        std::cout <<"Not Enough Nodes in Range." <<std::endl;
        UnsupportedOperationException ex;
        throw(ex);
    }

}

std::shared_ptr<Location> LocationModule::GetLocation(){
    boost::mutex::scoped_lock lock(g_i_mutex);
    CalculateLocations();
    return m_dgelsLoc;
}

    std::shared_ptr<node::NodeContainer> LocationModule::GetNodeTargets(std::shared_ptr<node::NodeContainer> tempNodes){

    std::shared_ptr<node::NodeContainer> targetNodes = std::make_shared<node::NodeContainer>();
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

web::json::value LocationModule::GetJson(){
    boost::mutex::scoped_lock lock(g_i_mutex);
    CalculateLocations();
    web::json::value response = web::json::value::object();
    response["DGESV"] = m_dgesvLoc->ToJson();
    response["DGETRS"] = m_dgetrsLoc->ToJson();
    response["DGELS"] = m_dgelsLoc->ToJson();
    return response;
}


web::json::value LocationModule::BasicJson(){
    boost::mutex::scoped_lock lock(g_i_mutex);
    web::json::value response = web::json::value::object();
    response= m_dgesvLoc->ToJson();
    return response;
}

void  LocationModule::calculateDgesvDgetrs(std::shared_ptr<node::NodeContainer> nodes){


    if(nodes->GetNodes().size()!=4){
        throw("Error::Too few Nodes!");
    }

    std::vector<std::shared_ptr<TargetNode>> Nodes;

    for (std::shared_ptr<INode> a: nodes->GetNodes()){
        std::cout <<a->getSSID();
        Nodes.push_back(std::static_pointer_cast<TargetNode>(a));

    }

    double A[3][3]={2*(Nodes[1]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[1]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[1]->getZCoord()-Nodes[0]->getZCoord()),
                    2*(Nodes[2]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[2]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[2]->getZCoord()-Nodes[0]->getZCoord()),
                    2*(Nodes[3]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[3]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[3]->getZCoord()-Nodes[0]->getZCoord())};

    double B[3][1]={(pow(Nodes[0]->GetDistance(),2.0))-(pow(Nodes[1]->GetDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[1]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[1]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[1]->getZCoord(),2.0)),
                    (pow(Nodes[0]->GetDistance(),2.0))-(pow(Nodes[2]->GetDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[2]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[2]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[2]->getZCoord(),2.0)),
                    (pow(Nodes[0]->GetDistance(),2.0))-(pow(Nodes[3]->GetDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[3]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[3]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[3]->getZCoord(),2.0))};


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
    m_dgesvLoc->m_calculationTime=t.elapsed().wall;

    printf("Calculated Location: ");
    std::cout << "X:" << lexical_cast<std::string>(B[0][0]) <<" Y:" << lexical_cast<std::string>(B[1][0]) << " Z:" << lexical_cast<std::string>(B[2][0])  << "Time:" << lexical_cast<std::string>(m_dgesvLoc->m_calculationTime) <<std::endl;


    double B2[3][1]={(pow(Nodes[0]->GetDistance(),2.0))-(pow(Nodes[1]->GetDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[1]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[1]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[1]->getZCoord(),2.0)),
                    (pow(Nodes[0]->GetDistance(),2.0))-(pow(Nodes[2]->GetDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[2]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[2]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[2]->getZCoord(),2.0)),
                    (pow(Nodes[0]->GetDistance(),2.0))-(pow(Nodes[3]->GetDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[3]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[3]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[3]->getZCoord(),2.0))};

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

    m_dgetrsLoc->m_calculationTime=t.elapsed().wall-m_dgesvLoc->m_calculationTime;

    std::cout << "X:" << lexical_cast<std::string>(B2[0][0]) <<" Y:" << lexical_cast<std::string>(B2[1][0]) << " Z:" << lexical_cast<std::string>(B2[2][0])  << "Time:" << lexical_cast<std::string>(m_dgetrsLoc->m_calculationTime) <<std::endl;


    return;
}



void LocationModule::calculateDgels(std::shared_ptr<node::NodeContainer> nodes){

    if(nodes->GetNodes().size()!=4){
        throw("Error::Too few Nodes!");
    }

    std::vector<std::shared_ptr<TargetNode>> Nodes;

    for (std::shared_ptr<INode> a: nodes->GetNodes()){
        std::cout <<a->getSSID();
        Nodes.push_back(std::static_pointer_cast<TargetNode>(a));

    }

    double A[3][3]={2*(Nodes[1]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[1]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[1]->getZCoord()-Nodes[0]->getZCoord()),
                    2*(Nodes[2]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[2]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[2]->getZCoord()-Nodes[0]->getZCoord()),
                    2*(Nodes[3]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[3]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[3]->getZCoord()-Nodes[0]->getZCoord())};

    double B[3][1]={(pow(Nodes[0]->GetDistance(),2.0))-(pow(Nodes[1]->GetDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[1]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[1]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[1]->getZCoord(),2.0)),
                    (pow(Nodes[0]->GetDistance(),2.0))-(pow(Nodes[2]->GetDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[2]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[2]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[2]->getZCoord(),2.0)),
                    (pow(Nodes[0]->GetDistance(),2.0))-(pow(Nodes[3]->GetDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[3]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[3]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[3]->getZCoord(),2.0))};

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

    m_dgelsLoc->updateCoords(B[0][0],B[1][0],B[2][0]);
    m_dgelsLoc->m_calculationTime=t.elapsed().wall;

    printf("Calculated Location Dgels: ");
    std::cout << "X:" << lexical_cast<std::string>(B[0][0]) <<" Y:" << lexical_cast<std::string>(B[1][0]) << " Z:" << lexical_cast<std::string>(B[2][0])  << "Time:" << lexical_cast<std::string>(m_dgelsLoc->m_calculationTime) <<std::endl;

    return ;



}