
use std::ffi::*;

const NULL_BYTE: c_char = '\0' as c_char;

enum PrintfSequence {
    JustString(String),
    FormatSpecifier {
        opts: String,
        last: char,
    },
}

// panic on OOM!!!!!!!!!!!!!
fn parse_formats_and_such(s: &CStr) -> Vec<PrintfSequence> {
    let mut v = vec![];
    let mut is_format = false;
    let mut buf = String::new();
    for c in s.to_bytes() {
        if *c == b'%' {
            // fuck, format specifier
            // tiny bit of analysis
            if is_format {
                // this is a %%
                buf.push(*c as char);
                v.push(PrintfSequence::JustString(buf.clone()));
                buf.clear();
                is_format = false;
            } else {
                is_format = true;
            }
        } else if is_format {
            // gosh darn it we have to actually process this
            if c.is_ascii_whitespace() {
                continue; // we don't care
            } else if c.is_ascii_alphabetic() {
                // end of it, fuck, we're cooked
                v.push(PrintfSequence::FormatSpecifier {
                    opts: buf.clone(),
                    last: *c as char,
                });
                buf.clear();
            } else {
                buf.push(*c as char);
            }
        } else {
            buf.push(*c as char);
        }
    }
    if !buf.is_empty() {
        // if it is, then its an incomplete format specifier, and thus should be discarded
        if !is_format {
            v.push(PrintfSequence::JustString(buf));
        }
    }
    return v;
}

#[unsafe(no_mangle)]
unsafe extern "C" fn snprintf(buf: *mut c_char, buflen: usize, s: *const c_char, mut arg: ...) -> c_int {
    // do be ranted about again later
    unsafe {
        // Parsing, processing, and killing myself
        let v = parse_formats_and_such(CStr::from_ptr(s));
        let mut processed = String::new();

        if !buf.is_null() {
            let str_slice: &str = &processed[0..(buflen-1)];
            for (i, c) in str_slice.bytes().enumerate() {
                *buf.add(i) = c as c_char;
            }
            *buf.add(str_slice.len()) = NULL_BYTE;
            return str_slice.len() as c_int;
        }
        return processed.len() as c_int;
    }
}

#[test]
fn parsing() {
    let cstr = CString::new("Hi there! This is %s, reporting for %.s at %d AM").unwrap();
    let v = parse_formats_and_such(cstr.as_c_str());
}
