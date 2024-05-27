#include "../s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *res = s21_NULL;
  char arg[s21_strlen(str2) * 2 - 1];
  s21_size_t i = 0;
  for (s21_size_t k = 0; k < s21_strlen(str2); k++) {
    arg[i] = str2[k];
    arg[i + 1] = '|';
    i += 2;
  }
  arg[i - 1] = '\0';
  regmatch_t start;
  regex_t reg;
  regcomp(&reg, arg, REG_EXTENDED);
  if (regexec(&reg, str1, 1, &start, 0) == 0) {
    res = (char *)str1 + start.rm_so;
  }

  regfree(&reg);
  return res;
}