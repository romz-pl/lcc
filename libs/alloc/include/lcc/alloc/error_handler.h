#pragma once

namespace lcc {

/// An error handler callback.
typedef void (*fatal_error_handler_t)(void *user_data,
                                      const char *reason,
                                      bool gen_crash_diag);

/// Installs a new bad alloc error handler that should be used whenever a
/// bad alloc error, e.g. failing malloc/calloc, is encountered by LLVM.
///
/// The user can install a bad alloc handler, in order to define the behavior
/// in case of failing allocations, e.g. throwing an exception. Note that this
/// handler must not trigger any additional allocations itself.
///
/// If no error handler is installed the default is to print the error message
/// to stderr, and call exit(1).  If an error handler is installed then it is
/// the handler's responsibility to log the message, it will no longer be
/// printed to stderr.  If the error handler returns, then exit(1) will be
/// called.
///
///
/// \param user_data - An argument which will be passed to the installed error
/// handler.
void install_bad_alloc_error_handler(fatal_error_handler_t handler,
                                     void *user_data = nullptr);

/// Restores default bad alloc error handling behavior.
void remove_bad_alloc_error_handler();

/// Reports a bad alloc error, calling any user defined bad alloc
/// error handler. In contrast to the generic 'report_fatal_error'
/// functions, this function might not terminate, e.g. the user
/// defined error handler throws an exception, but it won't return.
///
/// Note: When throwing an exception in the bad alloc handler, make sure that
/// the following unwind succeeds, e.g. do not trigger additional allocations
/// in the unwind chain.
///
/// If no error handler is installed (default), throws a bad_alloc exception
/// if LLVM is compiled with exception support. Otherwise prints the error
/// to standard error and calls abort().
[[noreturn]] void report_bad_alloc_error(const char *Reason,
                                         bool GenCrashDiag = true);

void install_out_of_memory_new_handler();

}
