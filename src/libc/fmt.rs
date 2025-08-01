//! this file contains _ultoa and _ltoa

use core::ffi::c_char;

macro_rules! radix_check {
    ($radix:expr, $ptr: expr) => {
        if !(2..=36).contains(&$radix) {
            unsafe { $ptr.write(0 as c_char) };
            return $ptr;
        }
    };
}

#[unsafe(no_mangle)]
unsafe extern "C" fn _ltoa(value: i32, str: *mut c_char, radix: i32) -> *mut c_char {
    radix_check!(radix, str);
    match value {
        ..0 => {
            unsafe { str.write(b'-' as c_char) };
            let unsigned = (!value).wrapping_add(1) as u32; // funny twos complement stuff
            unsafe { _ultoa(unsigned, str.add(1), radix) };
        }
        _ => {
            unsafe { _ultoa(value as u32, str, radix) };
        }
    }
    str
}

macro_rules! cc {
    ($char:expr) => {
        $char as c_char
    };
}

const DIGITS: [c_char; 36] = [
    cc!('0'),
    cc!('1'),
    cc!('2'),
    cc!('3'),
    cc!('4'),
    cc!('5'),
    cc!('6'),
    cc!('7'),
    cc!('8'),
    cc!('9'),
    cc!('a'),
    cc!('b'),
    cc!('c'),
    cc!('d'),
    cc!('e'),
    cc!('f'),
    cc!('g'),
    cc!('h'),
    cc!('i'),
    cc!('j'),
    cc!('k'),
    cc!('l'),
    cc!('m'),
    cc!('n'),
    cc!('o'),
    cc!('p'),
    cc!('q'),
    cc!('r'),
    cc!('s'),
    cc!('t'),
    cc!('u'),
    cc!('v'),
    cc!('w'),
    cc!('x'),
    cc!('y'),
    cc!('z'),
];

#[unsafe(no_mangle)]
unsafe extern "C" fn _ultoa(mut value: u32, str: *mut c_char, radix: i32) -> *mut c_char {
    radix_check!(radix, str);
    if value == 0 {
        unsafe { str.write(DIGITS[0]) };
        unsafe { str.add(1).write(0 as c_char) };
        return str;
    }
    let radix = radix as u32;
    let mut buf = [0 as c_char; 65]; // 64 bit + NULL
    let mut i = 0;
    while value != 0 {
        let rem = value % radix;
        buf[i] = DIGITS[rem as usize];
        i += 1;
        value /= radix;
    }
    let mut j = 0;
    while i != 0 {
        i -= 1;
        unsafe { str.add(j).write(buf[i]) };
        j += 1;
    }
    unsafe { str.add(j).write(0 as c_char) };
    str
}
