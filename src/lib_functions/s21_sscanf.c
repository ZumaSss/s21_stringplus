#include "../s21_string.h"

/*sscanf считывает форматированный ввод из строки. Спецификатор формата,
 прототип: %[*][ширина][длина]спецификатор.

  Для передачи переменного числа параметров, используются макросы va_arg(),
  va_start() и va_end(). Тип va_list. Самый правый известный параметр называется
  last_parm(format). Доступ осуществляется по указателю argptr (args)*/

int s21_sscanf(const char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);  //инициализируем указатель args
  const char *input_string = str;  // указатель на начало строки данных
  const char *f_string = format;
  //указатель на начало строки спецификаторов формата
  int count = 0;  //будем считать число считанных элементов
  int result = 1;

  //идем по форматной строке
  for (; *f_string != '\0' && result; ++f_string) {
    // ининциализируем структуру параметров прототипа формата
    FORMAT parametrs = {0};
    if (*f_string == '%') {
      //парсинг форматной строки
      parsing_format_string(&parametrs, &f_string);
      //обработка спецификатора + вводной строки
      result =
          parsing_input_string(parametrs, &args, &str, &input_string, &count);
    } else if (s21_isspace(*f_string)) {  //если не %, а пробел, то пропускаем
                                          //все пробелы
      while (s21_isspace(*input_string)) {
        input_string++;
      }
    } else {
      //если не % и не пробел, проверяем совпадают ли символы строк, т.к.
      //совпадающие реальный сканф пропускает и идет дальше, а если нет -
      //выходим из цикла и прекращаем считывать
      if (*f_string != *input_string) {
        result = 0;
      }
      input_string++;
    }
  }
  va_end(args);  // Завершаем работу с переменными аргументами
  return count;
}

void parsing_format_string(FORMAT *parametrs, const char **f_string) {
  // парсим префиксы
  /*1. астериск - в sscanf знак *, помещенный после % и перед спецификатором
  формата, считывает данные указанного типа, но подавляет их присваивание.*/
  (*f_string)++;
  if (**f_string == '*') {
    parametrs->asterisk = 1;
    (*f_string)++;
  }
  /* Ширина - минимальное количество печатаемых символов. Если выводимое
  значение короче этого числа, результат дополняется пробелами*/

  while (is_digit(**f_string)) {
    parametrs->width = parametrs->width * 10 + (**f_string - '0');
    (*f_string)++;
  }
  /* длина
  h - Аргумент интерпретируется как короткое int или короткое int без знака
  (применяется только к целочисленным спецификаторам: i, d, o, u, x и X).
  l - Аргумент интерпретируется как длинное int или длинное int без знака для
  целочисленных спецификаторов (i, d, o, u, x и X) и как широкий символ или
  строка широких символов для спецификаторов c и s. L- Аргумент интерпретируется
  как длинный double (применяется только к спецификаторам с плавающей точкой −
  e, E, f, g и G).
  */
  if (**f_string == 'h' || **f_string == 'l' || **f_string == 'L') {
    parametrs->length = **f_string;
    (*f_string)++;
  }
  // Парсим спецификатор
  switch (**f_string) {
    case 'c':  //Символ
    case 'd':  //Знаковое десятичное целое число
    case 'i':  //Знаковое целое число (Шестнадцатеричное число, если входная
               //строка начинается с "0x" или "0X", восьмеричное число, если
               //строка начинается с "0"; в противном случае — десятичное число)
    case 'o':  //Беззнаковое восьмеричное число
    case 'p':  //Адрес указателя в шестнадцатеричном виде. Макс число
               //считываемых цифр зависит от размера указателя (32 бита или 64
               //бита), который зависит от архитектуры компьютера. "0x" и "0X"
               //принимаются как префиксы.
    case 'u':  //Беззнаковое десятичное целое число
    case 'x':  //Беззнаковое шестнадцатеричное целое число (любые буквы)
    case 'X':  //то же
    case 'e':  //Десятичное число с плавающей точкой или научная нотация
               //(мантисса/экспонента)
    case 'E':  //то же
    case 'f':  //то же
    case 'g':  //то же
    case 'G':  //то же
    case 'n':  //Количество символов, считанных до появления %n
    case 's':  //Строка, до первого символа-разделителя (пробела, знака
               //табуляции или новой строки).
      //Размер включает место для завершающего нуль-символа

    case '%':  //Символ %
      parametrs->specifier = **f_string;
      break;
  }
}

int parsing_input_string(FORMAT parametrs, va_list *args, const char **str,
                         const char **input_string, int *count) {
  int result = 1;

  if (**input_string == '\0' && parametrs.specifier != 'n')
    result = 0;  //если закончилась строка и спецификатор не n
  else {
    switch (parametrs.specifier) {
      case 'c':
      case 's':
        result = processing_for_cs(parametrs, args, input_string);
        break;
      case 'n':
        int position = *input_string - *str;
        //не входит в число счит. символов, не добавляется к результату сканф
        processing_for_n(args, position);
        break;
      case 'd':
        result = processing_for_di(parametrs, args, 10, input_string);
        break;
      case 'i':
        result = processing_for_di(parametrs, args, 0, input_string);
        break;
      case 'o':
        result = processing_for_ouxp(parametrs, args, 8, input_string);
        break;
      case 'u':
        result = processing_for_ouxp(parametrs, args, 10, input_string);
        break;
      case 'x':
      case 'X':
        result = processing_for_ouxp(parametrs, args, 16, input_string);
        break;
      case 'p':
        result = processing_for_ouxp(parametrs, args, 16, input_string);
        break;
      case 'f':
      case 'g':
      case 'G':
      case 'e':
      case 'E':
        result = processing_for_fgGeE(parametrs, args, input_string);
        break;
      case '%':
        (*input_string)++;
        break;
      default:
        result = 0;
        break;
    }
    *count += (result && !parametrs.asterisk && parametrs.specifier != 'n' &&
               parametrs.specifier != '%');
  }

  if (*count == 0 && **input_string == '\0' && !result) {
    *count = -1;
    //Функция scanf() возвращает число введенных полей. Она возвращает
    // EOF,.если обнаруживается преждевременный конец файла.
  }
  return result;
}

int processing_for_cs(FORMAT parametrs, va_list *args,
                      const char **input_string) {
  if (parametrs.specifier == 's') {  // пропускаем пробелы
    while (s21_isspace(**input_string)) {
      (*input_string)++;
    }
  }
  if (parametrs.asterisk) {
    do {
      (*input_string)++;  //если случай с, пропускаем один символ, а если
                          //строчка - то пока она не кончится
    } while ((parametrs.specifier == 's') && **input_string != '\0' &&
             !s21_isspace(**input_string));
  } else {
    s21_size_t width = parametrs.width;
    //если не указана ширина для с считываем один символ
    if (parametrs.specifier == 'c' && width == 0) width = 1;
    //достаем адрес переменной из аргс и определяем желаемый тип данных для
    //записываемой переменной
    char *c = va_arg(*args, char *);
    for (s21_size_t i = 0;
         (i < width || width == 0) && **input_string != '\0' &&
         ((parametrs.specifier == 's') ? !s21_isspace(**input_string) : 1);
         i++) {
      //если s, то считываем до первого пробела, несмотря на ширину, если с, то
      //четко по указанной ширине, вместе с пробелами
      *c = **input_string;
      c++;
      (*input_string)++;
    }
    if (parametrs.specifier == 's') *c = '\0';
  }
  return 1;
}

void processing_for_n(va_list *args, int position) {
  int *p;
  p = va_arg(*args, int *);
  *p = position;  //записываем текущую позицию (сколько символов считали от
                  //начала строки) по указателю из аргс
}

int processing_for_di(FORMAT parametrs, va_list *args, int base,
                      const char **input_string) {
  long long number = 0;
  int sign = 1;  //предполагаем знак +
  long long result = 0;
  int overflow = 0;
  s21_size_t count = 0;
  //распознаем число, путем преобразования строки в число, проверяем знаковое
  //или беззнаковое, проверяем какая база для спецификатора i. Это может быть
  // 8меричная система -начинается с 0, либо 16тиричная, начинается с 0X или 0x

  int res_strtoi = handling_sign_and_base(&count, &sign, parametrs.width, &base,
                                          input_string);
  // рес = 0 если считали только знак
  while (count++ < parametrs.width || parametrs.width == 0) {
    int digit = s21_atoi(**input_string, base);  //преобразуем символы в цифры
    //если возвращает -1, цикл не отрабатываем

    if (digit >= 0 && digit < base) {
      res_strtoi = 1;  //присвоили 1, т.к. счтитали хотя бы один диджит
      if (!overflow) {
        //проверяем переполнение размера лонг лонг
        if (result > (LONG_MAX - digit) / base) {
          overflow = 1;
          //присваем максимально возможное значение при переполнении
          result = (sign == 1) ? LONG_MAX : LONG_MIN;
        } else {
          result = result * base + digit;  //считываем число
        }
      }
      (*input_string)++;
    } else {
      break;
    }
  }
  if (overflow == 1)
    number = result;
  else
    number = sign * result;

  //проверяем какую часть считанного числа нужно взять в соответствии с
  //префиксом длины и записать по указателю соответствующего типа

  if (res_strtoi && !parametrs.asterisk) {
    //создаем указатели `*х_number`, которые получают значение с помощью
    //`va_arg` из списка аргументов `args`. Затем значение `number` приводится к
    //типу соответствующему типу
    // и присваиваются `*х_number`.
    if (parametrs.length == 'l') {
      long long *l_number;
      l_number = va_arg(*args, long long *);
      *l_number = (long long)number;
    } else if (parametrs.length == 'h') {
      short *h_number;
      h_number = va_arg(*args, short *);
      *h_number = (short)number;
    } else {
      int *p_number;
      p_number = va_arg(*args, int *);
      *p_number = (int)number;
    }
  }
  return res_strtoi;  //возвращаем ноль если ничего не считали
}

int processing_for_ouxp(FORMAT parametrs, va_list *args, int base,
                        const char **input_string) {
  s21_size_t number = 0;
  int sign = 1;  //предполагаем знак +
  s21_size_t result = 0;
  int overflow = 0;
  s21_size_t count = 0;

  int res_strtoui = handling_sign_and_base(&count, &sign, parametrs.width,
                                           &base, input_string);
  // рес = 0 если считали только знак
  while (count++ < parametrs.width || parametrs.width == 0) {
    int digit = s21_atoi(**input_string, base);  //преобразуем символы в цифры
    //если возвращает -1, цикл не отрабатываем

    if (digit >= 0 && digit < base) {
      res_strtoui = 1;  //показываем, что считали символ
      if (!overflow) {
        if (result > (ULONG_MAX - digit) / base) {
          //проверяем переполнение размера ансайнгд лонг лонг
          overflow = 1;
          result = ULONG_MAX;  //присваем максимально возможное значение при
                               //переполнении
        } else {
          result = result * base + digit;  //считываем число
        }
      }
      (*input_string)++;
    } else {
      break;
    }
  }

  if (overflow)
    number = result;
  else
    number = sign * result;

  if (res_strtoui && !parametrs.asterisk) {
    //проверяем является ли считанное число указателем
    if (parametrs.specifier == 'p') {
      void **p_number = va_arg(*args, void **);
      *p_number = (void *)number;
    }
    //проверяем какую часть считанного числа нужно взять в соответствии с
    //префиксом длины и записать по указателю соответствующего типа
    else if (parametrs.length == 'l') {
      unsigned long *l_number;
      l_number = va_arg(*args, unsigned long *);
      *l_number = (unsigned long)number;
    } else if (parametrs.length == 'h') {
      unsigned short *h_number;
      h_number = va_arg(*args, unsigned short *);
      *h_number = (unsigned short)number;
    } else {
      unsigned int *p_number;
      p_number = va_arg(*args, unsigned int *);
      *p_number = (unsigned int)number;
    }
  }
  return res_strtoui;  //возвращаем ноль если ничего не считали
}

int processing_for_fgGeE(FORMAT parametrs, va_list *args,
                         const char **input_string) {
  long double number = 0;

  int sign = 1, base = 10, sign_e = 1;
  s21_size_t count = 0, decimal_point = 0;
  long double integer_part = 0, decimal_part = 0;
  long long exponent = 0;

  int overflow_int = 0;

  int res_strtofl = handling_sign_and_base(&count, &sign, parametrs.width,
                                           &base, input_string);

  //считываем целую часть до точки
  while (count < parametrs.width || parametrs.width == 0) {
    //преобразуем символы в цифры если возвращает -1, цикл не отрабатываем
    int digit = s21_atoi(**input_string, base);
    if (digit >= 0 && digit < base) {
      res_strtofl = 1;  //показываем, что считали символ
      (count)++;
      if (!overflow_int) {
        if (integer_part > (LDBL_MAX - digit) / base) {
          overflow_int = 1;
          integer_part = (sign == 1) ? LDBL_MAX : LDBL_MIN;
          //макс и мин значение long double
          sign = 1;
        } else {
          integer_part = integer_part * base + digit;
        }
      }
      (*input_string)++;
    } else {
      break;
    }
  }

  if (**input_string == '.' &&
      (count < parametrs.width || parametrs.width == 0)) {
    (*input_string)++;
    count++;
    decimal_point = count;
    int overflow_dec = 0;
    while (count < parametrs.width || parametrs.width == 0) {
      //преобразуем символы в цифры если возвращает -1, цикл не отрабатываем
      int digit = s21_atoi(**input_string, base);
      if (digit >= 0 && digit < base) {
        res_strtofl = 1;  //показываем, что считали символ
        (count)++;
        if (!overflow_dec) {
          if (decimal_part > (LDBL_MAX - digit) / base) {
            overflow_dec = 1;
            decimal_part = (sign == 1) ? LDBL_MAX : LDBL_MIN;
            sign = 1;
          } else {
            decimal_part = decimal_part * base + digit;
          }
        }
        (*input_string)++;
      } else {
        break;
      }
    }
    decimal_point = count - decimal_point;
  }

  //ищем экспоненту
  if ((**input_string == 'e' || **input_string == 'E') &&
      (count++ < parametrs.width || parametrs.width == 0)) {
    (*input_string)++;

    // sign = 1;
    res_strtofl = handling_sign_and_base(&count, &sign_e, parametrs.width,
                                         &base, input_string);

    long long result_e = 0;
    int overflow_e = 0;
    while (count++ < parametrs.width || parametrs.width == 0) {
      int digit = s21_atoi(**input_string, base);
      if (digit >= 0 && digit < base) {
        res_strtofl = 1;
        if (!overflow_e) {
          if (result_e > (LONG_MAX - digit) / base) {
            overflow_e = 1;
            result_e = (sign_e == 1) ? LONG_MAX : LONG_MIN;
            sign_e = 1;
          } else {
            result_e = result_e * base + digit;
          }
        }
        (*input_string)++;
      } else {
        break;
      }
    }
    exponent = sign_e * result_e;
  }
  number = sign * ((integer_part + decimal_part / pow(10, decimal_point)) *
                   pow(10, exponent));

  if (res_strtofl && !parametrs.asterisk) {
    if (parametrs.length == 'L') {
      long double *ld_number = va_arg(*args, long double *);
      *ld_number = number;
    } else if (parametrs.length == 'l') {
      double *d_number = va_arg(*args, double *);
      *d_number = (double)number;
    } else {
      float *p_number = va_arg(*args, float *);
      *p_number = (float)number;
    }
  }

  return res_strtofl;
}

int handling_sign_and_base(s21_size_t *count, int *sign, s21_size_t width,
                           int *base, const char **input_sring) {
  int result = 0;
  while (s21_isspace(**input_sring)) {  // пропускаем пробелы
    (*input_sring)++;
  }
  if (**input_sring == '-' || **input_sring == '+') {  // проверяем есть ли знак
    if (**input_sring == '-') *sign = -1;
    (*input_sring)++;
    (*count)++;  //считаем символы(чтобы сравнить с шириной - сколько еще
                 //выводить знаков)
  }
  //определяем базу, если она неизвестна. Если база 16 - мы должны пропустить
  // 0x, чтобы считать число
  if (*base == 0 || *base == 16) {
    int new_base = 10;
    if (**input_sring == '0' &&
        ((*count)++ < width || width == 0)) {  //проверка на базу 8 или 16
      result = 1;  //обозначаем, что считали символ 0
      new_base = 8;
      (*input_sring)++;
      if ((**input_sring == 'x' || **input_sring == 'X') &&
          ((*count)++ < width || width == 0)) {  //проверка на базу 16
        new_base = 16;
        (*input_sring)++;
      }
    }
    if (*base == 0) {  //записываем определенную базу для спецификатора i
      *base = new_base;
    }
  }
  return result;
}

int s21_atoi(char c, int base) {
  int digit = -1;
  if (is_digit(c)) {
    digit = c - '0';
  } else if (base == 16) {
    if (c >= 'a' && c <= 'f') {
      digit = c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
      digit = c - 'A' + 10;
    }
  }
  return digit;
}

int s21_isspace(int c) { return (c == ' ' || c == '\t' || c == '\n'); }