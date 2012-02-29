#ifndef __LIBSTROPHE_TEST_H__
#define __LIBSTROPHE_TEST_H__

#define TEST_MAIN \
    int main(int argc, char **argv) {\
    int num_failed;\
    Suite *s = parser_suite();\
    SRunner *sr = srunner_create(s);\
    srunner_run_all(sr, CK_NORMAL);\
    num_failed = srunner_ntests_failed(sr);\
    srunner_free(sr);\
    return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;\
    }\
      

#endif /* __LIBSTROPHE_TEST_H__ */
