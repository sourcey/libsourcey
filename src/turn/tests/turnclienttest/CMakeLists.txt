# include_dependency(OpenSSL REQUIRED)
# include_dependency(LibUV)

set(turnclienttest_EXCLUDE_CTEST TRUE)
define_libsourcey_test(turnclienttest base net stun turn uv util)
