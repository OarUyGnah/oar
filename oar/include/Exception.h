#ifndef __OAR_EXCEPTION_H__
#define __OAR_EXCEPTION_H__

#include "oar/ThisThread.h"

#include <exception>
#include <string>

namespace oar {

class Exception : public std::exception {
public:
    Exception(std::string what)
        : _message(std::move(what))
        , _stack(ThisThread::stackTrace(false))
    {
    }

    ~Exception() noexcept override = default;

    // default copy-ctor and operator= are okay.

    const char* what() const noexcept override
    {
        return _message.c_str();
    }

    const char* stackTrace() const noexcept
    {
        return _stack.c_str();
    }

private:
    std::string _message;
    std::string _stack;
};
}

#endif
