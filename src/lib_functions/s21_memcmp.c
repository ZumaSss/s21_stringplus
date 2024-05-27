#include "../s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int res = 0;

  if (n != 0) {
    char *temp1 = (char *)str1;
    char *temp2 = (char *)str2;
    s21_size_t i = 0;
    while (i < n - 1 && *(char *)temp1 == *(char *)temp2) {
      (char *)temp1++;
      (char *)temp2++;
      i++;
    }
    res = *(char *)temp1 - *(char *)temp2;
  }

  return res;
}