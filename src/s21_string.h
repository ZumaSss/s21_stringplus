#include <float.h>
#include <limits.h>
#include <math.h>
#include <regex.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define s21_size_t unsigned long long
#define s21_NULL (void *)0

#define UNKNOWN "Unknown error: "
#define is_flag(ch)                                                   \
  ((ch) == '-') || ((ch) == '+') || ((ch) == ' ') || ((ch) == '#') || \
      ((ch) == '0')

#define is_digit(ch) ((ch) >= '0' && (ch) <= '9')

#define is_length(ch) ((ch) == 'h' || (ch) == 'l' || (ch) == 'L')

#define is_specifier(ch)                                                      \
  ((ch) == 'c' || (ch) == 'd' || (ch) == 'i' || (ch) == 'e' || (ch) == 'E' || \
   (ch) == 'f' || (ch) == 'g' || (ch) == 'G' || (ch) == 'o' || (ch) == 's' || \
   (ch) == 'u' || (ch) == 'x' || (ch) == 'X' || (ch) == 'p' || (ch) == 'n' || \
   (ch) == '%')

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strcat(char *dest, const char *src);
char *s21_strchr(const char *str, int c);
int s21_strcmp(const char *str1, const char *str2);
char *s21_strcpy(char *dest, const char *src);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
s21_size_t s21_strspn(const char *str1, const char *str2);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);

void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_to_lower(const char *str);
void *s21_to_upper(const char *str);
void *s21_trim(const char *src, const char *trim_chars);

//-----------------------------sscanf---------------------------//

typedef struct {
  char asterisk;
  s21_size_t width;
  char length;
  char specifier;
} FORMAT;

int s21_sscanf(const char *str, const char *format, ...);

void parsing_format_string(FORMAT *parametrs, const char **f_string);
int parsing_input_string(FORMAT parametrs, va_list *args, const char **str,
                         const char **input_string, int *count);
int processing_for_cs(FORMAT parametrs, va_list *args,
                      const char **input_string);
int processing_for_di(FORMAT parametrs, va_list *args, int base,
                      const char **input_string);
int processing_for_ouxp(FORMAT parametrs, va_list *args, int base,
                        const char **input_string);
void processing_for_n(va_list *args, int position);
int processing_for_fgGeE(FORMAT parametrs, va_list *args,
                         const char **input_string);
int handling_sign_and_base(s21_size_t *count, int *sign, s21_size_t width,
                           int *base, const char **input_sring);
int s21_atoi(char c, int base);
int s21_isspace(int c);

//-----------------------------sprintf---------------------------//

typedef struct s21_FORMAT {
  int flag_minus;  // Выравнивание по левому краю
  int flag_plus;  // Заставляет предварять результат знаком + или -
  int flag_space;  // Если знак не записан, перед значением
                   // вставляется пробел.
  int flag_sharp;  //При o, x или X значению предшествуют 0, 0x или
                   // 0X соответственно, если не нуль. При с e, E и
                   // f заставляет записанный вывод содержать
                   //десятичную точку. По умолчанию, если цифры не
                   //следуют, десятичная точка не записывается.
  int flag_zero;  // Дополняет число слева нулями (0) вместо
                  // пробелов
  int width;  //Минимальное количество печатаемых цифр.
  int precision;  //Минимальное количество записываемых цифр.
  char length;     // Тип переменной
  char specifier;  // Спецификатор

  int sign;   // Знак + или -
  int point;  // ФЛАГ НА ТООООЧКУУ!
} s21_FORMAT;

int s21_sprintf(char *str, const char *format, ...);
void s21_default_precision(s21_FORMAT *parameters);
void s21_check_flag(s21_FORMAT *parameters, const char *format);
void s21_sharp_flag(s21_FORMAT *parameters, char *str);

char *s21_str_to_int(int *number, const char *format, va_list *input);
void s21_int_to_str(s21_FORMAT *parameters, char *str, long double number);
void s21_simple_str_to_int(int *number, char *str);
void s21_ul_to_str(char *str, unsigned long long number,
                   s21_FORMAT *parameters);
void s21_ol_to_str(char *str, unsigned long long number, int prefix);
void s21_hl_to_str(char *str, unsigned long long number,
                   s21_FORMAT *parameters);
void s21_float_to_str(char *str, long double number, s21_FORMAT *parameters);
void s21_wch_to_str(char *str, wchar_t *wstr, s21_size_t length);
void s21_eE_to_str(char *str, long double number, s21_FORMAT *parameters);

char *s21_check_specifier(char *str, s21_size_t *str_len, va_list *input,
                          s21_FORMAT *parameters);
int s21_compare_round(s21_FORMAT *parameters, long double number);
void s21_delete_zero(s21_FORMAT *parameters, char *str);

void s21_move_string(char *str);
void s21_make_string_flags(s21_FORMAT *parameters, char *str);
void s21_make_string_width(s21_FORMAT *parameters, char *str);
void s21_make_string_precision(s21_FORMAT *parameters, char *str);
void s21_make_mantissa(s21_FORMAT *parameters, char *mantice, int notation);
void s21_reverse_writing(char *str);

void s21_spec_c(char *str, va_list *input, s21_FORMAT *parameters);  //Символ
void s21_spec_di(char *str, va_list *input,
                 s21_FORMAT *parameters);  // Знаковое десятичное целое число
void s21_spec_eE(
    char *str, va_list *input,
    s21_FORMAT *parameters);  //Научная нотация (мантисса/экспонента) с
                              //использованием символа e/E (вывод чисел должен
                              //совпадать с точностью до
                              // e-6)
void s21_spec_f(char *str, va_list *input,
                s21_FORMAT *parameters);  // Десятичное число с плавающей точкой
void s21_spec_gG(char *str, va_list *input,
                 s21_FORMAT *parameters);  // Использует кратчайший из
                                           // представлений десятичного числа
void s21_spec_o(char *str, va_list *input,
                s21_FORMAT *parameters);  // Беззнаковое восьмеричное число
void s21_spec_s(char *str, va_list *input,
                s21_FORMAT *parameters);  // Строка символов
void s21_spec_u(char *str, va_list *input,
                s21_FORMAT *parameters);  // Беззнаковое десятичное целое число
void s21_spec_xX(char *str, va_list *input,
                 s21_FORMAT *parameters);  // Беззнаковое шестнадцатеричное
                                           // целое число(X - заглавные буквы)
void s21_spec_p(char *str, va_list *input,
                s21_FORMAT *parameters);  // Адрес указателя
void s21_spec_n(
    s21_size_t const *str_len,
    va_list *input);  // Количество символов, напечатанных до появления %n
void s21_spec_percentage(char *str, s21_FORMAT *parameters);  // Символ %