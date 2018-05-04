//
// Created by green on 24/11/17.
//

#include "inc/Modules/FileNodeReaderModule.h"
#include <fstream>
#include <sstream>
    FileNodeReaderModule::FileNodeReaderModule(std::string filepath) {
        m_filepath=filepath;
    }

    std::shared_ptr<node::NodeContainer> FileNodeReaderModule::readNodes(){
        std::shared_ptr<node::NodeContainer> m_NodeList = std::make_shared<node::NodeContainer>();
        try{
            std::ifstream f(m_filepath);
            std::stringstream s;
            std::stringstream json;
            web::json::value v;
            if (f) {
                s << f.rdbuf();                                         // stream results of reading from file stream into string stream
                f.close();                                              // close the filestream
                v=web::json::value::parse(s.str());
                std::cout <<s.str();// parse the resultant string stream.
                for(int i=0;i<v.as_array().size();i++){
                    std::shared_ptr<TargetNode> newNode(std::make_shared<TargetNode>(v.as_array()[i]));
                    std::cout << "AFTER PARSING " << i <<": " << v.serialize();
                    std::cout << v.as_array()[0];
                    m_NodeList->AddNode(newNode);

                }
            }else{
                std::cout << m_filepath << "Does Not Exist";
            }
        }catch(web::json::json_exception excep) {
            std::cout << "ERROR Parsing JSON: ";
            std::cout << excep.what();

        }
    }
