#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// dummy typeinfo since ldo.c does not use rtti
namespace std {
    [[noreturn]] void terminate() {
        abort();
        while (1) {}
    }

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

/// Called by __cxa_end_catch.
void _Unwind_DeleteException(_Unwind_Exception *exception_object) {
    // if (exception_object->exception_cleanup != NULL)
    // (*exception_object->exception_cleanup)(_URC_FOREIGN_EXCEPTION_CAUGHT,
    //                                        exception_object);
}

// Round s up to next multiple of a.
static inline size_t aligned_allocation_size(size_t s, size_t a) {
    return (s + a - 1) & ~(a - 1);
}

static inline size_t cxa_exception_size_from_exception_thrown_size(size_t size) {
    return aligned_allocation_size(size + sizeof (__cxa_exception),
                                   alignof(__cxa_exception));
}

//  This does not need to be atomic
static inline int incrementHandlerCount(__cxa_exception *exception) {
    return ++exception->handlerCount;
}

//  This does not need to be atomic
static inline  int decrementHandlerCount(__cxa_exception *exception) {
    return --exception->handlerCount;
}

static bool isDependentException(_Unwind_Exception* unwind_exception) {
    return false; // uhh we cant track this and lua dont have so trust me bro
}

extern "C" {
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
        std::terminate();
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

void __cxa_decrement_exception_refcount(void *thrown_object) throw() {
    if (thrown_object != NULL )
    {
        __cxa_exception* exception_header = static_cast<__cxa_exception*>(thrown_object) - 1;
        
        if (--exception_header->referenceCount == 0)
        {
            if (NULL != exception_header->exceptionDestructor)
                exception_header->exceptionDestructor(thrown_object);
            __cxa_free_exception(thrown_object);
        }
    }
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

void*
__cxa_begin_catch(void* unwind_arg) throw()
{
    _Unwind_Exception* unwind_exception = static_cast<_Unwind_Exception*>(unwind_arg);
    bool native_exception = true;//__isOurExceptionClass(unwind_exception);
    __cxa_eh_globals* globals = __cxa_get_globals();
    // exception_header is a hackish offset from a foreign exception, but it
    //   works as long as we're careful not to try to access any __cxa_exception
    //   parts.
    __cxa_exception* exception_header =
            reinterpret_cast<__cxa_exception*>(
                static_cast<_Unwind_Exception*>(unwind_exception) + 1
            ) - 1;

    if (native_exception)
    {
        // Increment the handler count, removing the flag about being rethrown
        exception_header->handlerCount = exception_header->handlerCount < 0 ?
            -exception_header->handlerCount + 1 : exception_header->handlerCount + 1;
        //  place the exception on the top of the stack if it's not already
        //    there by a previous rethrow
        if (exception_header != globals->caughtExceptions)
        {
            exception_header->nextException = globals->caughtExceptions;
            globals->caughtExceptions = exception_header;
        }
        globals->uncaughtExceptions -= 1;   // Not atomically, since globals are thread-local
        return exception_header->adjustedPtr;
    }
    // Else this is a foreign exception
    // If the caughtExceptions stack is not empty, terminate
    if (globals->caughtExceptions != 0)
        std::terminate();
    // Push the foreign exception on to the stack
    globals->caughtExceptions = exception_header;
    return unwind_exception + 1;
}

void __cxa_end_catch() {
    __cxa_eh_globals* globals = __cxa_get_globals_fast(); // __cxa_get_globals called in __cxa_begin_catch
    __cxa_exception* exception_header = globals->caughtExceptions;
    // If we've rethrown a foreign exception, then globals->caughtExceptions
    //    will have been made an empty stack by __cxa_rethrow() and there is
    //    nothing more to be done.  Do nothing!
    if (NULL != exception_header)
    {
        bool native_exception = true;//__isOurExceptionClass(&exception_header->unwindHeader);
        if (native_exception)
        {
            // This is a native exception
            if (exception_header->handlerCount < 0)
            {
                //  The exception has been rethrown by __cxa_rethrow, so don't delete it
                if (0 == incrementHandlerCount(exception_header))
                {
                    //  Remove from the chain of uncaught exceptions
                    globals->caughtExceptions = exception_header->nextException;
                    // but don't destroy
                }
                // Keep handlerCount negative in case there are nested catch's
                //   that need to be told that this exception is rethrown.  Don't
                //   erase this rethrow flag until the exception is recaught.
            }
            else
            {
                // The native exception has not been rethrown
                if (0 == decrementHandlerCount(exception_header))
                {
                    //  Remove from the chain of uncaught exceptions
                    globals->caughtExceptions = exception_header->nextException;
                    // Destroy this exception, being careful to distinguish
                    //    between dependent and primary exceptions
                    if (isDependentException(&exception_header->unwindHeader))
                    {
                        // ts shouldnt be possible with lua's code
                        std::terminate();
                        // // Reset exception_header to primaryException and deallocate the dependent exception
                        // __cxa_dependent_exception* dep_exception_header =
                        //     reinterpret_cast<__cxa_dependent_exception*>(exception_header);
                        // exception_header =
                        //     cxa_exception_from_thrown_object(dep_exception_header->primaryException);
                        // __cxa_free_dependent_exception(dep_exception_header);
                    }
                    // Destroy the primary exception only if its referenceCount goes to 0
                    //    (this decrement must be atomic)
                    __cxa_decrement_exception_refcount(static_cast<void*>(exception_header + 1));
                }
            }
        }
        else
        {
            // The foreign exception has not been rethrown.  Pop the stack
            //    and delete it.  If there are nested catch's and they try
            //    to touch a foreign exception in any way, that is undefined
            //     behavior.  They likely can't since the only way to catch
            //     a foreign exception is with catch (...)!
            _Unwind_DeleteException(&globals->caughtExceptions->unwindHeader);
            globals->caughtExceptions = 0;
        }
    }
}
}