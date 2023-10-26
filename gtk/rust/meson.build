ghb_rs_src = [
  'src/lib.rs',
  'src/server.rs',
  'src/worker.rs',
]

if meson.version().version_compare('>= 1.3.0')
  ghb_rs = static_library('handbrake_rs', ghb_rs_src,
    rust_abi: 'c')
else
  ghb_rs = static_library('handbrake_rs', ghb_rs_src,
    rust_crate_type: 'staticlib')
endif

ghb_rs_1 = shared_library('handbrake_rs', ghb_rs_src,
    rust_crate_type: 'dylib', install: true)

ghb_rs_headers = custom_target('ghb_rs_headers',
  input: ghb_rs_src,
  output: 'ghb_rs.h',
  command: [find_program('cbindgen'), '--lang', 'c', '-o', '@OUTPUT@', '@INPUT0@'],
)