//! this file contains _ultoa and _ltoa

use core::ffi::c_char;

#[unsafe(no_mangle)]
unsafe extern "C" fn _ltoa(value: i32, str: *mut c_char, radix: i32) -> *mut c_char {
    todo!();
}
#[unsafe(no_mangle)]
unsafe extern "C" fn _ultoa(value: u32, str: *mut c_char, radix: i32) -> *mut c_char {
    todo!();
}
