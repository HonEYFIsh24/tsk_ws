// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file  dt_error.hpp
 * @brief This file defines the Error class, which describes abnormal errors within the SDK.
 *        Detailed information about the exception can be obtained through this class.
 */

#ifndef DEPTRUM_HPP_DT_ERROR_HPP_
#define DEPTRUM_HPP_DT_ERROR_HPP_

#include <iostream>
#include <memory>
#include "deptrum/h/dt_error.h"
#include "dt_types.hpp"

namespace dt {
class Error : public std::exception {
 public:
  /**
   * @brief A static function to handle the dt_error and throw an exception if needed.
   *
   * @param[in] error The dt_error pointer to be handled.
   * @param[in] throw_exception A boolean value to indicate whether to throw an exception or not,
   * the default value is true.
   */
  static void Handle(dt_error** error, bool throw_exception = true) {
    if (!error || !*error) {  // no error
      return;
    }

    if (throw_exception) {
      throw Error(*error);
    } else {
      dt_delete_error(*error);
      *error = nullptr;
    }
  }

  /**
   * @brief Destroy the Error object.
   */
  ~Error() override {
    if (impl_) {
      dt_delete_error(impl_);
      impl_ = nullptr;
    }
  }

  /**
   * @brief Returns the error message of the exception.
   *
   * @return const char* The error message.
   */
  const char* what() const noexcept override { return impl_->message; }

  /**
   * @brief Returns the error message of the exception.
   *
   * @return const char* The error message.
   */
  const char* What() const noexcept { return this->what(); }

  /**
   * @brief Returns the exception type of the exception.
   * @brief Read the comments of the DtExceptionType enum in the deptrum/h/dt_types.h file for more
   * information.
   *
   * @return DtExceptionType The exception type.
   */
  DtExceptionType GetExceptionType() const noexcept { return impl_->exception_type; }

  /**
   * @brief Returns the name of the function where the exception occurred.
   *
   * @return const char* The function name.
   */
  const char* GetFunction() const noexcept { return impl_->function; }

  /**
   * @brief Returns the arguments of the function where the exception occurred.
   *
   * @return const char*  The arguments.
   */
  const char* GetArgs() const noexcept { return impl_->args; }

  /**
   * @brief Returns the error message of the exception.
   * @brief It is recommended to use the what() function instead.
   *
   * @return const char* The error message.
   */
  const char* GetMessage() const noexcept { return impl_->message; }

  /**
   * @brief Output the exception information to the stream.
   *
   * @param[in] os The output stream.
   * @param[in] e The exception object.
   * @return std::ostream& The output stream.
   */
  friend std::ostream& operator<<(std::ostream& os, const Error& e) {
    return os << "function:" << e.GetFunction() << "\nmessage:" << e.GetMessage()
              << "\nargs:" << e.GetArgs() << "\ntype:" << e.GetExceptionType() << std::endl;
  }

 private:
  dt_error* impl_ = nullptr;

  /**
   * @brief Construct a new Error object.
   *
   * @attention This constructor should not be called directly, use the handle() function instead.
   *
   * @param[in] error The dt_error object.
   */
  explicit Error(dt_error* error) : impl_(error){};

  Error& operator=(const Error&) = default;
};

}  // namespace dt

#endif  // DEPTRUM_HPP_DT_ERROR_HPP_
