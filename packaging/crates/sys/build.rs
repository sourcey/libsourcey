use std::env;
use std::error::Error;
use std::path::PathBuf;

fn main() -> Result<(), Box<dyn Error>> {
    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR")?);
    let graft_include = env::var_os("ICEY_GRAFT_INCLUDE_DIR")
        .map(PathBuf::from)
        .or_else(|| {
            manifest_dir
                .ancestors()
                .nth(3)
                .map(|repo_root| repo_root.join("src/graft/include"))
                .filter(|path| path.join("icy/graft/host/pipeline.h").exists())
        })
        .unwrap_or_else(|| manifest_dir.join("include"));
    let pipeline_header = graft_include.join("icy/graft/host/pipeline.h");
    let abi_header = graft_include.join("icy/graft/abi.h");

    println!("cargo:rerun-if-changed={}", pipeline_header.display());
    println!("cargo:rerun-if-changed={}", abi_header.display());
    println!("cargo:rerun-if-env-changed=ICEY_GRAFT_INCLUDE_DIR");
    println!("cargo:rerun-if-env-changed=ICEY_LIB_DIR");
    println!("cargo:rerun-if-env-changed=ICEY_LINK_KIND");
    println!("cargo:rustc-check-cfg=cfg(icey_native_link)");

    let bindings = bindgen::Builder::default()
        .header(pipeline_header.display().to_string())
        .clang_arg(format!("-I{}", graft_include.display()))
        .allowlist_function("icy_.*")
        .allowlist_type("icy_.*")
        .allowlist_var("ICY_.*")
        .allowlist_var("icy_.*")
        .layout_tests(false)
        .generate()?;

    let out_dir = PathBuf::from(env::var("OUT_DIR")?);
    bindings.write_to_file(out_dir.join("bindings.rs"))?;

    if let Ok(lib_dir) = env::var("ICEY_LIB_DIR") {
        let link_kind = env::var("ICEY_LINK_KIND").unwrap_or_else(|_| "dylib".to_owned());
        println!("cargo:rustc-cfg=icey_native_link");
        println!("cargo:rustc-link-search=native={lib_dir}");
        println!("cargo:rustc-link-lib={link_kind}=icy_pipeline_capi");
    } else {
        println!(
            "cargo:warning=ICEY_LIB_DIR is not set; generated bindings will check, but final native links need libicy_pipeline_capi"
        );
    }

    Ok(())
}
