# #include_dependency(OpenSSL REQUIRED)
# include_dependency(JsonCpp)

set(archotests_EXCLUDE_CTEST TRUE)
define_libsourcey_test(archotests uv base archo)
