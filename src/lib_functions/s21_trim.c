#include "../s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *new_str = s21_NULL;
  char temp[1024] = {'0'};
  char *arg = temp;
  if (src != s21_NULL) {
    s21_size_t len_src = s21_strlen(src) + 1;
    if (trim_chars == s21_NULL || trim_chars[0] == '\0') {
      arg = "\n|\t|\v|\r| ";
    } else {
      s21_size_t len_trim = s21_strlen(trim_chars);
      s21_size_t i = 0;
      for (s21_size_t k = 0; k < len_trim; k++) {
        arg[i] = trim_chars[k];
        arg[i + 1] = '|';
        i += 2;
      }
      arg[i - 1] = '\0';
    }

    char arg2[1024] = {'\0'};
    s21_strcat(arg2, arg);
    regmatch_t start;
    regex_t reg;
    if (regcomp(&reg, arg2, REG_EXTENDED) == 0) {
      s21_size_t down = 0;
      s21_size_t up = 0;
      s21_size_t flag_up = 0;
      s21_size_t flag_down = 0;
      for (int i = len_src - 2; i >= 0; i--) {
        char for_search[2];
        for_search[0] = src[i];
        for_search[1] = '\0';
        int reg_check = 1;
        reg_check = regexec(&reg, for_search, 1, &start, 0);
        if (reg_check == 0 && flag_down == 0) {
          down++;
        } else
          flag_down = 1;
      }
      for (s21_size_t i = 0; i < len_src - down - 1; i++) {
        char for_search[2];
        for_search[0] = src[i];
        for_search[1] = '\0';
        if ((regexec(&reg, for_search, 1, &start, 0) == 0) && flag_up == 0) {
          up++;

        } else
          flag_up = 1;
      }
      new_str = (char *)calloc((len_src - up - down), sizeof(char));
      for (s21_size_t i = up; i < len_src - down - 1; i++) {
        new_str[i - up] = src[i];
      }
      regfree(&reg);
    }
  }
  return new_str;
}