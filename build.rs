use std::{env, path::PathBuf, process::Command};

fn main() {
    let lua_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap()).join("lua");
    let out = PathBuf::from(env::var_os("OUT_DIR").unwrap());
    let emscripten_include = PathBuf::from(env::var_os("EMSDK").unwrap()).join("upstream").join("emscripten").join("cache").join("sysroot").join("include");

    let mut cmd = Command::new("make");
    cmd.current_dir(&lua_dir);
    cmd.arg("liblua.a");
    cmd.arg("CC=emcc");

    assert!(cmd.status().unwrap().success());

    let bindings = bindgen::builder()
        .header("lua/lua.h")
        .header("lua/lualib.h")
        .header("lua/lauxlib.h")
        .clang_arg("-fvisibility=default")
        .clang_arg(format!("-I{}", emscripten_include.display()))
        .blocklist_type("max_align_t")
        .use_core()
        .generate()
        .unwrap();

    bindings.write_to_file(out.join("bindings.rs")).unwrap();

    println!("cargo:rustc-link-search=native={}", lua_dir.display());
    println!("cargo:rustc-link-lib=static=lua");
}