# include_dependency(SSL REQUIRED)
# include_dependency(JsonCpp)

set(archotests_EXCLUDE_CTEST TRUE)
define_libsourcey_test(archotests base archo)
