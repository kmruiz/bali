/**
 * @file
 * @brief Core bali utilities and types.
 **/
#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Favour bsize_t instead of size_t to avoid gotchas
 * on unsigned under/overflow.
 **/
typedef int64_t bsize_t;

#if __has_include(<stdfloat.h>)
#include <stdfloat.h>
#else
typedef float float32_t;
typedef double float64_t;

static_assert(sizeof(float32_t) == 4);
static_assert(sizeof(float64_t) == 8);
#endif

#define BALI_DCHECK_BSIZE_BOUNDS(x) BALI_DCHECK(x <= ((bsize_t) SIZE_MAX))
#if defined(_WIN32)
#define BALI_API __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#define BALI_API __attribute__((visibility("default")))
#else
#define BALI_API
#endif

#define BALI_LCSTR const char *

/**
 * @brief Debug-time assertion that checks a runtime condition.
 *
 * Evaluates @p cond and triggers a diagnostic failure when DCHECKs are
 * enabled (typically in debug builds). In release builds this macro
 * compiles to a no-op but preserves side-effect safety of its argument.
 *
 * @param cond The condition that must hold.
 */
#define BALI_DCHECK(cond) ((void)(cond))
#if defined(BALI_USE_DCHECK)
#undef BALI_DCHECK
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
#endif

static inline void bali_rcheck_fail(
				    const char *cond,
				    const char *file,
				    const int line
				    ) {
  fprintf(stderr, "[BALI_RCHECK] %s:%d: %s failed.\n", file, line, cond);
  abort();
}

#define BALI_RCHECK(cond)					\
  do {								\
    if (!(cond)) {						\
      bali_rcheck_fail(#cond, __FILE__, __LINE__);		\
    }								\
  } while (0);

#define BALI_RCHECK_NOT_NULL(ptr) BALI_RCHECK((ptr) != nullptr)
