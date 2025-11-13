#pragma once

#include <assert.h>
#include <stdint.h>

/**
 * @brief Do not use size_t if possible because unsigned ints have unsafe
 * behaviour on under/overflow.
 **/
typedef int64_t bsize_t;

/**
 * @brief this will show
 *
 **/
typedef struct Test {

} Test;
  
#if defined(_WIN32)
#define BALI_API __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#define BALI_API __attribute__((visibility("default")))
#else
#define BALI_API
#endif

#if defined(BALI_USE_DCHECK)
#include <stdio.h>
#include <stdlib.h>
static inline void bali_dcheck_fail(
				    const char *cond,
				    const char *file,
				    const int line
				    ) {
  fprintf(stderr, "[BALI_DCHECK] %s:%d: %s failed.\n", file, line, cond);
  abort();
}

#define BALI_DCHECK(cond)					\
  do {								\
    if (!(cond)) {						\
      bali_dcheck_fail(#cond, __FILE__, __LINE__);		\
    }								\
  } while (0);					
#else
#define BALI_DCHECK(cond)
#endif
