#include <lcc/alloc/error_handler.h>

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <new>
#include <unistd.h>

namespace lcc {

static fatal_error_handler_t BadAllocErrorHandler = nullptr;
static void *BadAllocErrorHandlerUserData = nullptr;

void install_bad_alloc_error_handler(fatal_error_handler_t handler, void *user_data)
{
  assert(!ErrorHandler && "Bad alloc error handler already registered!\n");
  BadAllocErrorHandler = handler;
  BadAllocErrorHandlerUserData = user_data;
}

void remove_bad_alloc_error_handler()
{
  BadAllocErrorHandler = nullptr;
  BadAllocErrorHandlerUserData = nullptr;
}

void report_bad_alloc_error(const char *Reason, bool GenCrashDiag)
{
  fatal_error_handler_t Handler = nullptr;
  void *HandlerData = nullptr;
  {
    Handler = BadAllocErrorHandler;
    HandlerData = BadAllocErrorHandlerUserData;
  }

  if (Handler) {
    Handler(HandlerData, Reason, GenCrashDiag);
    __builtin_unreachable(); // bad alloc handler should not return
  }

#ifdef LLVM_ENABLE_EXCEPTIONS
  // If exceptions are enabled, make OOM in malloc look like OOM in new.
  throw std::bad_alloc();
#else
  // Don't call the normal error handler. It may allocate memory. Directly write
  // an OOM to stderr and abort.
  const char *OOMMessage = "LLVM ERROR: out of memory\n";
  const char *Newline = "\n";
  (void)!::write(2, OOMMessage, std::strlen(OOMMessage));
  (void)!::write(2, Reason, std::strlen(Reason));
  (void)!::write(2, Newline, std::strlen(Newline));
  std::abort();
#endif
}


#ifdef LLVM_ENABLE_EXCEPTIONS
// Do not set custom new handler if exceptions are enabled. In this case OOM
// errors are handled by throwing 'std::bad_alloc'.
void llvm::install_out_of_memory_new_handler()
{
}
#else
// Causes crash on allocation failure. It is called prior to the handler set by
// 'install_bad_alloc_error_handler'.
static void out_of_memory_new_handler()
{
  report_bad_alloc_error("Allocation failed");
}

// Installs new handler that causes crash on allocation failure. It is called by
// InitLLVM.
void install_out_of_memory_new_handler()
{
  std::new_handler old = std::set_new_handler(out_of_memory_new_handler);
  (void)old;
  assert((old == nullptr || old == out_of_memory_new_handler) &&
         "new-handler already installed");
}
#endif
}
