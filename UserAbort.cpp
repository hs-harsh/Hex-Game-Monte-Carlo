#include "UserAbort.h" 

const char* UserAbort::what() const throw () {
    return "User triggerd abort";
}