//
// Created by green on 28/11/17.
//

#ifndef FOO_INODE_H
#define FOO_INODE_H

#include <string>
#include <cpprest/json.h>
#include <atomic>
#include <memory>

class INode {
public:
    virtual web::json::value ToJson() = 0;

    virtual void Update(std::shared_ptr<INode> Node) = 0;

    virtual int getRSSI() const = 0;

    virtual std::string getSSID() = 0;

    virtual std::string getMAC() = 0;

    virtual void setRecentlyUpdated(bool status) = 0;

    virtual bool getRecentlyUpdated() = 0;


};

#endif //FOO_INODE_H
