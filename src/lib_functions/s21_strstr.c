#include "../s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  char *res = s21_NULL;
  regmatch_t start;
  regex_t reg;
  regcomp(&reg, needle, REG_EXTENDED);
  if (regexec(&reg, haystack, 1, &start, 0) == 0) {
    res = (char *)haystack + start.rm_so;
  }
  regfree(&reg);
  return res;
}