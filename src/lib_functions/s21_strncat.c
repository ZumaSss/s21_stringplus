#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *dest_p = dest;
  const char *src_p = src;
  while (*dest_p != '\0') {
    dest_p++;
  }
  s21_size_t i = 0;
  while (src_p[i] != '\0' && i < n) {
    dest_p[i] = src_p[i];
    i++;
  }
  dest_p[i] = '\0';
  return dest;
}