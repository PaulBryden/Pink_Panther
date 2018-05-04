//
// Created by green on 04/05/18.
//

#ifndef FOO_UNSUPPORTEDOPERATION_H
#define FOO_UNSUPPORTEDOPERATION_H

#include <stdexcept>

class UnsupportedOperationException : public std::exception{
    /** Constructor (C strings).
 *  @param message C-style string error message.
 *                 The string contents are copied upon construction.
 *                 Hence, responsibility for deleting the char* lies
 *                 with the caller.
 */
    explicit UnsupportedOperationException(const char* message):
            m_msg(message)
    {
    }

    /** Constructor (C++ STL strings).
     *  @param message The error message.
     */
    explicit UnsupportedOperationException(const std::string& message):
            m_msg(message)
    {}

    /** Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~UnsupportedOperationException() throw (){}

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *          is in posession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char* what() const throw (){
        return m_msg.c_str();
    }

protected:
    /** Error message.
     */
    std::string m_msg;
};

#endif //FOO_UNSUPPORTEDOPERATION_H
