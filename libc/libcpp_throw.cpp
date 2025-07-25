
static struct __cxa_exception {
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

static struct __cxa_eh_globals {
    __cxa_exception *   caughtExceptions;
    unsigned int        uncaughtExceptions;
};

extern "C" {
    static __cxa_eh_globals eh_globals;
    __cxa_eh_globals *__cxa_get_globals() { return &eh_globals; }
    __cxa_eh_globals *__cxa_get_globals_fast() { return &eh_globals; }
}

// In Wasm, a destructor returns its argument
void __cxa_throw(void *thrown_object, std::type_info *tinfo, void *(*dest)(void *)) {
    __cxa_eh_globals* globals = __cxa_get_globals();
    globals->uncaughtExceptions += 1; // Not atomically, since globals are thread-local

    __cxa_exception* exception_header = __cxa_init_primary_exception(thrown_object, tinfo, dest);
    exception_header->referenceCount = 1; // This is a newly allocated exception, no need for thread safety.

    _Unwind_RaiseException(&exception_header->unwindHeader);
}