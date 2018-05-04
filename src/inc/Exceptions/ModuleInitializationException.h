//
// Created by green on 04/05/18.
//

#ifndef FOO_ModuleInitializationException_H
#define FOO_ModuleInitializationException_H

#include <exception>

class ModuleInitializationException : public std::exception
{

    virtual const char *what() const throw()
    {
        return "Module Initialization Failed";
    };
};

#endif //FOO_UNSUPPORTEDOPERATION_H
