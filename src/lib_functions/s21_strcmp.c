#include "../s21_string.h"

int s21_strcmp(const char *str1, const char *str2) {
  int res = 0;
  char *temp1 = (char *)str1;
  char *temp2 = (char *)str2;
  s21_size_t len1 = s21_strlen(temp1);
  s21_size_t len2 = s21_strlen(temp2);
  if (len1 > len2)
    res = 1;
  else if (len2 > len1)
    res = -1;
  else {
    s21_size_t i = 0;
    int flag = 0;
    while (i < len1 && flag == 0) {
      if (temp1[i] == temp2[i])
        i++;
      else {
        flag = 1;
      }
    }
    if ((i == len1))
      res = 0;
    else {
      if (temp1[i] > temp2[i])
        res = 1;
      else
        res = -1;
    }
  }
  return res;
}