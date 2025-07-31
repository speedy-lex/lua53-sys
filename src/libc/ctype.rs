use core::ffi::c_int;

macro_rules! ret_if_oob {
    ($x:expr) => {
        if $x > u8::MAX as c_int {
            return 0;
        }
    };
}

#[unsafe(no_mangle)]
extern "C" fn isalpha(x: c_int) -> c_int {
    ret_if_oob!(x);
    match x as u8 {
        b'a'..=b'z' | b'A'..=b'Z' => 1,
        _ => 0,
    }
}
#[unsafe(no_mangle)]
extern "C" fn isalnum(x: c_int) -> c_int {
    ret_if_oob!(x);
    match x as u8 {
        b'a'..=b'z' | b'A'..=b'Z' | b'0'..=b'9' => 1,
        _ => 0,
    }
}
#[unsafe(no_mangle)]
extern "C" fn iscntrl(x: c_int) -> c_int {
    ret_if_oob!(x);
    match x as u8 {
        0..=0x1F | 0x7F => 1,
        _ => 0,
    }
}
#[unsafe(no_mangle)]
extern "C" fn isdigit(x: c_int) -> c_int {
    ret_if_oob!(x);
    match x as u8 {
        b'0'..=b'9' => 1,
        _ => 0,
    }
}
#[unsafe(no_mangle)]
extern "C" fn isgraph(x: c_int) -> c_int {
    ret_if_oob!(x);
    match x as u8 {
        b'!'..=b'~' => 1,
        _ => 0,
    }
}
#[unsafe(no_mangle)]
extern "C" fn islower(x: c_int) -> c_int {
    ret_if_oob!(x);
    match x as u8 {
        b'a'..=b'z' => 1,
        _ => 0,
    }
}
#[unsafe(no_mangle)]
extern "C" fn ispunct(x: c_int) -> c_int {
    ret_if_oob!(x);
    isgraph(x) & (1 - isalnum(x)) // funny bitwise because these arent bools
}
#[unsafe(no_mangle)]
extern "C" fn isspace(x: c_int) -> c_int {
    ret_if_oob!(x);
    match x as u8 {
        b' ' | b'\t'..=b'\r' => 1,
        _ => 0,
    }
}
#[unsafe(no_mangle)]
extern "C" fn isupper(x: c_int) -> c_int {
    ret_if_oob!(x);
    match x as u8 {
        b'A'..=b'Z' => 1,
        _ => 0,
    }
}
#[unsafe(no_mangle)]
extern "C" fn isxdigit(x: c_int) -> c_int {
    ret_if_oob!(x);
    match x as u8 {
        b'0'..=b'9' | b'a'..=b'f' | b'A'..=b'F' => 1,
        _ => 0,
    }
}
#[unsafe(no_mangle)]
extern "C" fn tolower(x: c_int) -> c_int {
    if isupper(x) != 0 {
        return x | 32;
    }
    x
}
#[unsafe(no_mangle)]
extern "C" fn toupper(x: c_int) -> c_int {
    if islower(x) != 0 {
        return x & 0x5f;
    }
    x
}
