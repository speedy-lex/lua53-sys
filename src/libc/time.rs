#[link(wasm_import_module = "libc")]
unsafe extern "C" {
    fn get_unix_time_s() -> i64;
}

#[unsafe(no_mangle)]
extern "C" fn time(out: *mut i64) -> i64 {
    let t = unsafe { get_unix_time_s() };
    if !out.is_null() {
        unsafe { *out = t };
    }
    t
}
