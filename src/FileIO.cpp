//
// Created by green on 24/11/17.
//

#include "inc/FileIO.h"
#include <fstream>
#include <sstream>
namespace FileIO {
    FileIO::FileIO(std::shared_ptr<node::Node_Container> nodeList):m_NodeList(nodeList) {

    }

    void FileIO::WriteOut() {

    }

    void FileIO::ReadIn() {
        try{
            std::string filename("Settings.json");
            std::ifstream f(filename);
            std::stringstream s;
            std::stringstream json;
            web::json::value v;
            if (f) {
                s << f.rdbuf();                                         // stream results of reading from file stream into string stream
                f.close();                                              // close the filestream
                v=web::json::value::parse(s.str());
                std::cout <<s.str();// parse the resultant string stream.
                std::shared_ptr<Target_Node> newNode(std::make_shared<Target_Node>(v.as_array()[0]));
                std::cout << "AFTER PARSING:" << v.serialize();
                /*for (auto iter = v.as_array().cbegin(); iter != v.as_array().cend(); ++iter)
                {
                    auto k = iter->first;
                    auto n = iter->second;

                    auto key = k.as_string();
                    auto value = n.to_string();

                    std::cout << key << L" : " << value << " (" << n << ")" << std::endl;
                }*/
                std::cout << v.as_array()[0];
                m_NodeList->AddNode(newNode);
            }else{
                std::cout << "Settings.json Does Not Exist";
            }
        }catch(web::json::json_exception excep) {
            std::cout << "ERROR Parsing JSON: ";
            std::cout << excep.what();

        }
    }
}