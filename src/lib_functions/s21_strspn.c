#include "../s21_string.h"

s21_size_t s21_strspn(const char *str1, const char *str2) {
  size_t i, j;
  for (i = 0; str1[i] != '\0'; i++) {
    for (j = 0; str2[j] != '\0'; j++) {
      if (str1[i] == str2[j]) {
        break;
      }
    }
    if (str2[j] == '\0') {
      return i;
    }
  }

  return i;
}
