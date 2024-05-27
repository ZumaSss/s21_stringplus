#include "../s21_string.h"

void *s21_to_lower(const char *str) {
  char *new_str = s21_NULL;
  if (str != s21_NULL) {
    s21_size_t len = s21_strlen(str) + 1;

    s21_size_t i = 0;
    new_str = (char *)calloc((len + 1), sizeof(char));
    for (i = 0; i < len; i++) {
      if (str[i] > 64 && str[i] < 91)
        new_str[i] = str[i] + 32;
      else
        new_str[i] = str[i];
    }
    new_str[i] = '\0';
  }
  return new_str;
}