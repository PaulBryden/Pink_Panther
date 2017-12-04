//
// Created by green on 04/12/17.
//

#include "inc/locationModule.h"
#include "inc/Target_Node.h"
#include <vector>
#include <memory>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
std::shared_ptr<Location> locationModule::calculateDgels(std::shared_ptr<node::Node_Container> nodes){
    if(nodes->GetNodes().size()!=4){
        throw("Size of nodes list is not 4");
    }

    std::vector<std::shared_ptr<Target_Node>> Nodes;

    for (std::shared_ptr<INode> a: nodes->GetNodes()){
        std::cout <<a->getSSID();
        Nodes.push_back(std::static_pointer_cast<Target_Node>(a));

    }

    double A[3][3]={2*(Nodes[1]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[1]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[1]->getZCoord()-Nodes[0]->getZCoord()),
                    2*(Nodes[2]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[2]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[2]->getZCoord()-Nodes[0]->getZCoord()),
                    2*(Nodes[3]->getXCoord()-Nodes[0]->getXCoord()),2*(Nodes[3]->getYCoord()-Nodes[0]->getYCoord()),2*(Nodes[3]->getZCoord()-Nodes[0]->getZCoord())};
    double B[3][1]={(pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[1]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[1]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[1]->getZCoord(),2.0)),
                    (pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[2]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[2]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[2]->getZCoord(),2.0)),
                    (pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[0]->CalculateDistance(),2.0))-(pow(Nodes[0]->getXCoord(),2.0))+(pow(Nodes[3]->getXCoord(),2.0))-(pow(Nodes[0]->getYCoord(),2.0))+(pow(Nodes[3]->getYCoord(),2.0))-(pow(Nodes[0]->getZCoord(),2.0))+(pow(Nodes[3]->getZCoord(),2.0))};
    lapack_int info,m,n,lda,ldb,nrhs;

    m=3;
    n=3;
    nrhs=1;
    lda=3;
    ldb=1;

    info=LAPACKE_dgels(LAPACK_ROW_MAJOR,'N',m,n,nrhs,*A,lda,*B,ldb);

    if( info > 0 ) {
        printf( "The diagonal element %i of the triangular factor ", info );
        printf( "of A is zero, so that A does not have full rank;\n" );
        printf( "the least squares solution could not be computed.\n" );
        
    }
    std::shared_ptr<Location> newLocation = std::make_shared<Location>();
    newLocation->x=B[0][0];
    newLocation->y=B[1][0];
    newLocation->z=B[2][0];


    printf("Calculated Location: ");
    std::cout << "X:" << lexical_cast<std::string>(B[0][0]) <<" Y:" << lexical_cast<std::string>(B[1][0]) << " Z:" << lexical_cast<std::string>(B[2][0])  <<std::endl;

    return newLocation;



}
