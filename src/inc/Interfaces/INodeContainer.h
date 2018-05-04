//
// Created by green on 04/05/18.
//

#ifndef FOO_INODECONTAINER_H
#define FOO_INODECONTAINER_H
class INodeContainer{


    web::json::value ToJson();

    void PrintNodes();

    void ClearNodes();

    std::vector<std::shared_ptr<INode>>& GetNodes();

    void AddNode(std::shared_ptr<INode> node);

    void UpdateNodes(std::shared_ptr<INodeContainer> nodes);

};
#endif //FOO_INODECONTAINER_H_H
