use std::{env, path::PathBuf};

use bindgen::MacroTypeVariation;

fn main() {
    let lua_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap()).join("lua");
    let out = PathBuf::from(env::var_os("OUT_DIR").unwrap());

    let target_os = env::var("CARGO_CFG_TARGET_OS");
    let target_family = env::var("CARGO_CFG_TARGET_FAMILY");

    let mut cc_config = cc::Build::new();
    cc_config.warnings(false);
    
    if target_os == Ok("linux".to_string()) {
        cc_config.define("LUA_USE_LINUX", None);
    } else if target_os == Ok("macos".to_string()) {
        cc_config.define("LUA_USE_MACOSX", None);
    } else if target_os == Ok("ios".to_string()) {
        cc_config.define("LUA_USE_IOS", None);
    } else if target_family == Ok("unix".to_string()) {
        cc_config.define("LUA_USE_POSIX", None);
    } else if target_family == Ok("windows".to_string()) {
        cc_config.define("LUA_USE_WINDOWS", None);
    }

    let mut cc_config_build = cc_config.include(&lua_dir);

    cc_config_build = cc_config_build
        .file(lua_dir.join("lapi.c"))
        .file(lua_dir.join("lauxlib.c"))
        .file(lua_dir.join("lbaselib.c"))
        .file(lua_dir.join("lcode.c"))
        .file(lua_dir.join("lcorolib.c"))
        .file(lua_dir.join("lctype.c"))
        .file(lua_dir.join("ldebug.c"))
        .file(lua_dir.join("ldo.c"))
        .file(lua_dir.join("ldump.c"))
        .file(lua_dir.join("lfunc.c"))
        .file(lua_dir.join("lgc.c"))
        .file(lua_dir.join("llex.c"))
        .file(lua_dir.join("lmathlib.c"))
        .file(lua_dir.join("lmem.c"))
        .file(lua_dir.join("lobject.c"))
        .file(lua_dir.join("lopcodes.c"))
        .file(lua_dir.join("lparser.c"))
        .file(lua_dir.join("lstate.c"))
        .file(lua_dir.join("lstring.c"))
        .file(lua_dir.join("lstrlib.c"))
        .file(lua_dir.join("ltable.c"))
        .file(lua_dir.join("ltablib.c"))
        .file(lua_dir.join("ltm.c"))
        .file(lua_dir.join("lundump.c"))
        .file(lua_dir.join("lutf8lib.c"))
        .file(lua_dir.join("lvm.c"))
        .file(lua_dir.join("lzio.c"));
    
    let libc = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap()).join("libc");

    if cfg!(feature = "baremetal") {
        cc_config_build
            .cpp(true)
            .cpp_link_stdlib(None)
            .include(&libc)
            .file(libc.join("libc_utils.cpp"))
            .flag("-fexceptions");
    } else {
        cc_config_build
            .file(lua_dir.join("ldblib.c"))
            .file(lua_dir.join("liolib.c"))
            .file(lua_dir.join("loadlib.c"));
    }

    cc_config_build
        .out_dir(out.join("lib"))
        .compile("lua53");

    let mut bindings = bindgen::builder()
        .header("lua/lua.h")
        .header("lua/lualib.h")
        .header("lua/lauxlib.h")
        .default_macro_constant_type(MacroTypeVariation::Signed)
        .clang_arg("-fvisibility=default");

    if cfg!(feature = "baremetal") {
        bindings = bindings
            .clang_arg(format!("-I{}", libc.display()));
    }
    bindings.generate().unwrap().write_to_file(out.join("bindings.rs")).unwrap();
}
