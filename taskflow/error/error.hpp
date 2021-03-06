#pragma once

#include <iostream>
#include <sstream>
#include <exception>
#include <system_error>

namespace tf {

// The error category that describes the task informaction. 
struct Error : public std::error_category {

  enum Code : int {
    SUCCESS = 0,
    EXECUTOR
  };

  inline const char* name() const noexcept override final;
  inline static const std::error_category& get();
  
  inline std::string message(int) const override final;
};

// Function: name
inline const char* Error::name() const noexcept {
  return "Taskflow error";
}

// Function: get 
inline const std::error_category& Error::get() {
  static Error instance;
  return instance;
}

// Function: message
inline std::string Error::message(int code) const {
  switch(auto ec = static_cast<Error::Code>(code); ec) {
    case SUCCESS:
      return "success";
    break;

    case EXECUTOR:
      return "executor error";
    break;

    default:
      return "unknown";
    break;
  };
}

// Function: make_error_code
// Argument dependent lookup.
inline std::error_code make_error_code(Error::Code e) {
  return std::error_code(static_cast<int>(e), Error::get());
}

};  // end of namespace tf ----------------------------------------------------

// Register for implicit conversion  
namespace std {
  template <>
  struct is_error_code_enum<tf::Error::Code> : true_type {};
};

// ----------------------------------------------------------------------------

namespace tf {

// Procedure: throw_se
// Throws the system error under a given error code.
template <typename... ArgsT>
void throw_se(const char* fname, const size_t line, Error::Code c, ArgsT&&... args) {
  std::ostringstream oss;
  oss << "ERROR [" << fname << ":" << line << "] ";
  (oss << ... << args);
  abort();
  // throw std::system_error(c, oss.str());
}

};  // ------------------------------------------------------------------------

#define TF_THROW(...) tf::throw_se(__FILE__, __LINE__, __VA_ARGS__);

