#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

// Round s up to next multiple of a.
static inline size_t aligned_allocation_size(size_t s, size_t a) {
    return (s + a - 1) & ~(a - 1);
}

static inline size_t cxa_exception_size_from_exception_thrown_size(size_t size) {
    return aligned_allocation_size(size + sizeof (__cxa_exception),
                                   alignof(__cxa_exception));
}

extern "C" {
// In Wasm, a destructor returns its argument
void __cxa_throw(void *thrown_object, std::type_info *tinfo, void *(*dest)(void *)) {
    __cxa_eh_globals* globals = __cxa_get_globals();
    globals->uncaughtExceptions += 1; // Not atomically, since globals are thread-local

    __cxa_exception* exception_header = __cxa_init_primary_exception(thrown_object, tinfo, dest);
    exception_header->referenceCount = 1; // This is a newly allocated exception, no need for thread safety.

    _Unwind_RaiseException(&exception_header->unwindHeader);
}

//  Allocate a __cxa_exception object, and zero-fill it.
//  Reserve "thrown_size" bytes on the end for the user's exception
//  object. Zero-fill the object. If memory can't be allocated, call
//  std::terminate. Return a pointer to the memory to be used for the
//  user's exception object.
void *__cxa_allocate_exception(size_t thrown_size) throw() {
    size_t actual_size = cxa_exception_size_from_exception_thrown_size(thrown_size);

    char *raw_buffer =
        (char *)malloc(actual_size);
    if (NULL == raw_buffer)
        abort();
    __cxa_exception *exception_header =
        static_cast<__cxa_exception *>((void *)(raw_buffer));
    memset(exception_header, 0, actual_size);
    return static_cast<void*>(exception_header + 1);;
}


//  Free a __cxa_exception object allocated with __cxa_allocate_exception.
void __cxa_free_exception(void *thrown_object) throw() {
    // Compute the size of the padding before the header.
    char *raw_buffer =
        ((char *)(static_cast<__cxa_exception*>(thrown_object) - 1));
    free((void *)raw_buffer);
}
}