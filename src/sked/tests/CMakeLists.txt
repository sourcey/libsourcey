# include_dependency(JsonCpp)

set(skedtests_EXCLUDE_CTEST TRUE)
define_libsourcey_test(skedtests uv base net json sked)
