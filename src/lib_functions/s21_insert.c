#include "../s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *new_str = s21_NULL;
  if (src != s21_NULL && str != s21_NULL) {
    s21_size_t len_src = s21_strlen(src) + 1;
    s21_size_t len_str = s21_strlen(str) + 1;
    s21_size_t pos = start_index;

    if (pos <= len_src - 1) {
      new_str = (char *)calloc((len_src + len_str + 1), sizeof(char));
      s21_size_t shift = 0;
      for (s21_size_t i = 0; i < len_src + len_str; i++) {
        if (i == pos && shift < len_str - 1) {
          new_str[i] = str[shift];
          pos++;
          shift++;
        } else
          new_str[i] = src[i - shift];
      }
    }
  }

  return new_str;
}