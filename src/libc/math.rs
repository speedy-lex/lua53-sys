use core::ffi::c_int;

#[unsafe(no_mangle)]
extern "C" fn fabs(x: f64) -> f64 {
    x.abs()
}

#[unsafe(no_mangle)]
extern "C" fn fmod(x: f64, y: f64) -> f64 {
    x % y
}

#[unsafe(no_mangle)]
extern "C" fn floor(x: f64) -> f64 {
    x.floor()
}

#[unsafe(no_mangle)]
extern "C" fn ceil(x: f64) -> f64 {
    x.ceil()
}

#[unsafe(no_mangle)]
extern "C" fn sqrt(x: f64) -> f64 {
    x.sqrt()
}

#[unsafe(no_mangle)]
extern "C" fn log(x: f64) -> f64 {
    x.ln()
}

#[unsafe(no_mangle)]
extern "C" fn log2(x: f64) -> f64 {
    x.log2()
}

#[unsafe(no_mangle)]
extern "C" fn log10(x: f64) -> f64 {
    x.log10()
}

#[unsafe(no_mangle)]
extern "C" fn exp(x: f64) -> f64 {
    x.exp()
}

#[unsafe(no_mangle)]
extern "C" fn frexp(x: f64, out: *mut c_int) -> f64 {
    let int = x as c_int;
    unsafe { *out = int };
    x.fract()
}

#[unsafe(no_mangle)]
extern "C" fn pow(x: f64, y: f64) -> f64 {
    x.powf(y)
}

#[unsafe(no_mangle)]
extern "C" fn sin(x: f64) -> f64 {
    x.sin()
}

#[unsafe(no_mangle)]
extern "C" fn cos(x: f64) -> f64 {
    x.cos()
}

#[unsafe(no_mangle)]
extern "C" fn tan(x: f64) -> f64 {
    x.tan()
}

#[unsafe(no_mangle)]
extern "C" fn asin(x: f64) -> f64 {
    x.asin()
}

#[unsafe(no_mangle)]
extern "C" fn acos(x: f64) -> f64 {
    x.acos()
}

#[unsafe(no_mangle)]
extern "C" fn atan2(x: f64, y: f64) -> f64 {
    x.atan2(y)
}
