#include <stddef.h>

// dummy typeinfo since ldo.c does not use rtti
namespace std {

    using terminate_handler = void (*)();
    using unexpected_handler = void (*)();

    class type_info {
    public:
        virtual ~type_info() {}
        virtual bool operator==(const type_info& rhs) const { return this == &rhs; }
        virtual bool operator!=(const type_info& rhs) const { return this != &rhs; }
        virtual bool before(const type_info& rhs) const { return this < &rhs; }
        virtual const char* name() const { return "stub"; }
    };
}

// its empty since there is no DWARF or itanium
struct _Unwind_Exception { };

struct __cxa_exception {
    //  Manage the exception object itself.
    std::type_info *exceptionType;
    // In Wasm, a destructor returns its argument
    void *(*exceptionDestructor)(void *);

    std::unexpected_handler unexpectedHandler;
    std::terminate_handler  terminateHandler;

    __cxa_exception *nextException;

    int handlerCount;

    int handlerSwitchValue;
    const unsigned char *actionRecord;
    const unsigned char *languageSpecificData;
    void *catchTemp;
    void *adjustedPtr;

    // This is a new field to support C++11 exception_ptr.
    // For binary compatibility it is placed where the compiler
    // previously added padding to 64-bit align unwindHeader.
    size_t referenceCount;
    _Unwind_Exception unwindHeader;
};

struct __cxa_eh_globals {
    __cxa_exception *   caughtExceptions;
    unsigned int        uncaughtExceptions;
};

extern "C" {
    static __cxa_eh_globals eh_globals;
    __cxa_eh_globals *__cxa_get_globals() { return &eh_globals; }
    __cxa_eh_globals *__cxa_get_globals_fast() { return &eh_globals; }
}

__cxa_exception* __cxa_init_primary_exception(void* object, std::type_info* tinfo,
                                              void *(*dest)(void*)) throw() {
  __cxa_exception* exception_header = static_cast<__cxa_exception*>(object) - 1;
  exception_header->referenceCount = 0;
  exception_header->exceptionType = tinfo;
  exception_header->exceptionDestructor = dest;
  return exception_header;
}

/// Called by __cxa_throw.
void _Unwind_RaiseException(_Unwind_Exception *exception_object) {
  // Use Wasm EH's 'throw' instruction.
  __builtin_wasm_throw(0, exception_object);
}

extern "C" {
// In Wasm, a destructor returns its argument
__attribute__((used))
void __cxa_throw(void *thrown_object, std::type_info *tinfo, void *(*dest)(void *)) {
    __cxa_eh_globals* globals = __cxa_get_globals();
    globals->uncaughtExceptions += 1; // Not atomically, since globals are thread-local

    __cxa_exception* exception_header = __cxa_init_primary_exception(thrown_object, tinfo, dest);
    exception_header->referenceCount = 1; // This is a newly allocated exception, no need for thread safety.

    _Unwind_RaiseException(&exception_header->unwindHeader);
}
}