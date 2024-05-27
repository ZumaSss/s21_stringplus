#include "../s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int res = 0;
  char *temp1 = (char *)str1;
  char *temp2 = (char *)str2;
  s21_size_t len = 0;
  if (s21_strlen(temp1) > s21_strlen(temp2))
    len = s21_strlen(temp1);
  else
    len = s21_strlen(temp2);
  s21_size_t i = 0;
  int flag = 0;
  while (i < n && i < len && flag == 0) {
    if (temp1[i] == temp2[i])
      i++;
    else {
      flag = 1;
    }
  }
  if ((i == n && n < len) || i == len)
    res = 0;
  else {
    if (temp1[i] > temp2[i])
      res = 1;
    else
      res = -1;
  }

  return res;
}