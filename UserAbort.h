#ifndef USER_ABORT_H_
#define USER_ABORT_H_

#include <exception>

class UserAbort: public std::exception {
  const char* what() const throw();
};

#endif  // USER_ABORT_H_