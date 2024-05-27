#include "../s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  char *res = s21_NULL;
  int flag = 0;
  for (s21_size_t i = 0; i < n && i < (s21_size_t)s21_strlen(str) + 1; i++) {
    if (*((char *)str + i) == c && flag == 0) {
      res = (char *)str + i;
      flag = 1;
    }
  }

  return res;
}