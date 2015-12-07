include_dependency(JsonCpp)
include_dependency(OpenSSL REQUIRED)

define_libsourcey_test(skedtests base uv net http json sked)
