#include "../s21_string.h"

char *s21_strchr(const char *str, int c) {
  char *res = s21_NULL;
  int flag = 0;
  for (s21_size_t i = 0; i < (s21_size_t)s21_strlen(str) + 1; i++) {
    if (*((char *)str + i) == c && flag == 0) {
      res = (char *)str + i;
      flag = 1;
    }
  }

  return res;
}