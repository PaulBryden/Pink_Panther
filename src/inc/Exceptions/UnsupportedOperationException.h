//
// Created by green on 04/05/18.
//

#ifndef FOO_UNSUPPORTEDOPERATION_H
#define FOO_UNSUPPORTEDOPERATION_H

#include <stdexcept>

class UnsupportedOperationException : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Unsupported Operation";
    };
};

#endif //FOO_UNSUPPORTEDOPERATION_H
