#![feature(c_variadic)]

#[allow(dead_code, non_camel_case_types, non_snake_case, non_upper_case_globals)]
mod bindings {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}
//#[cfg(feature = "baremetal")]
mod libc;

#[cfg(not(feature = "baremetal"))]
#[unsafe(no_mangle)]
unsafe extern "C" fn get_unix_time_s() -> i64 {
    use std::time::{SystemTime, UNIX_EPOCH};
    return SystemTime::now().duration_since(UNIX_EPOCH).unwrap().as_secs() as i64;
}

pub use bindings::*;
