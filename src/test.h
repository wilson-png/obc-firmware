/* This file contains defines and helper macros for use in test files */

#ifndef __TEST_H__
#define __TEST_H__

#ifndef TEST_VERBOSE
  #define TEST_VERBOSE	false
#endif

#define assert_run(t, s)                                        \
  if((t))                                                       \
  {                                                             \
    printf("\033[1;32m[ PASS ]\033[0;32m " s "\033[0;00m\n");   \
  }                                                             \
  else                                                          \
  {                                                             \
    printf("\033[1;31m[ FAIL ]\033[0;31m " s "\033[0;00m\n");   \
  }

#endif /*  __TEST_H__ */