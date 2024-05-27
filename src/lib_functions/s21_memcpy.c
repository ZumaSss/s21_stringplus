#include "../s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *dest_p = dest;
  const char *src_p = src;
  for (s21_size_t i = 0; i < n; i++) {
    dest_p[i] = src_p[i];
  }

  return dest;
}