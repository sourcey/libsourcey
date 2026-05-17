use std::env;

fn main() {
    println!("cargo:rerun-if-env-changed=ICEY_LIB_DIR");
    println!("cargo:rustc-check-cfg=cfg(icey_native_link)");

    if env::var_os("ICEY_LIB_DIR").is_some() {
        println!("cargo:rustc-cfg=icey_native_link");
    }
}
