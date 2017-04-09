/**
 * @author Felix Stegmaier
 */

#include "Printf.h"

/*
Insert functions:
Insert value into dst for given format.
Return next writable char in buffer.
 */

char* insert_verbatim(char* dst, const void* end, const char value);
char* insert_d(char* dst, const void* end, const int value);
char* insert_u(char* dst, const void* end, const unsigned int value);
char* insert_c(char* dst, const void* end, const char value);
char* insert_s(char* dst, const void* end, const char* value);
char* insert_x(char* dst, const void* end, const unsigned int value);
char* insert_b(char* dst, const void* end, const unsigned int value);
char* insert_percent(char* dst, const void* end);
char* insert_end(char* dst, const void* end);

/**
 * insert the given value as string representaion to given base
 * @param  dst   buffer to write to
 * @param  end   first non-writable position after buffer
 * @param  value the value to be inserted
 * @param  base  the base of the string representaion
 * @return       next writable char in buffer
 */
char* insert_uint_with_base(char* dst, const void* end, const unsigned int value, const unsigned int base);

static const char digits[] = "0123456789abcdef";


/**
 * Minimal printf implementation
 * Support of
 *  (0) Varbatim text
 *  (1) %d for signed int
 *  (2) %u for unsigned int
 *  (3) %c for a single char
 *  (4) %s for a null terminated string
 *  (5) %x for hexadecimal (0xaf) without leading 0s
 *  (6) %b for binary (0b10101) without leading 0s
 *  (7) %% for char '%'
 *
 * @param  dst     pointer to the buffer to write the formated string to
 * @param  end     pointer to the first byte after the buffer
 * @param  fmt     format string to be used
 * @param  VARARGS format value arguments
 * @return         pointer to the first writable char (after '\0')
 *                 nullptr if any argument is not valid or fmt is not correct
 *                 the content of buffer dst may be wrong in this case
 */
char* Printf(char* dst, const void* end, const char* fmt, va_list args) {
  if (!dst
      || !end
      || !fmt
      || end <= dst) {
    DEBUG("printf illegal");
    return nullptr;
    }
  DEBUG("printf");

  char* next_writable = dst;
  //va_list args;
  //va_start(args, fmt);

  for (const char* fmt_position = fmt; '\0' != *fmt_position ; ++fmt_position) {
    if ('%' == *fmt_position) { /* insert argument */
      switch (*(fmt_position+1)) { /* format specifier at next position */
        case 'd':
          next_writable = insert_d(next_writable, end,
                                   va_arg(args, signed int));
          break;
        case 'u':
          next_writable = insert_u(next_writable, end,
                                   va_arg(args, unsigned int));
          break;
        case 'c':
          next_writable = insert_c(next_writable, end,
                                   static_cast<char>(va_arg(args, int)));
          break;
        case 's':
          next_writable = insert_s(next_writable, end,
                                   va_arg(args,const char*));
          break;
        case 'x':
          next_writable = insert_x(next_writable, end,
                                   va_arg(args, unsigned int));
          break;
        case 'b':
          next_writable = insert_b(next_writable, end,
                                   va_arg(args, unsigned int));
          break;
        case '%':
          next_writable = insert_percent(next_writable, end);
          break;
        case '\0': /* % not allowed on last position in fmt */
          return nullptr;
          break;
        default: /* unrecognized format specifier */
          return nullptr;
      }
      /* skip one char, since it was the format specifier */
      ++fmt_position;
    } else { /* insert verbatim */
      next_writable = insert_verbatim(next_writable, end, *fmt_position);
    }
    if (!next_writable) { /* check if previous insert was successfull */
      return nullptr;
    }
  }

  //va_end(args);
  next_writable = insert_end(next_writable, end);
  return next_writable;
}

char* insert_verbatim(char* dst, const void* end, const char value) {
  if (!dst
      || !end
      /* value may be 0 for '\0' */
      || end <= dst) {
    return nullptr;
  }
  *dst = value;
  return dst+1;
}

char* insert_d(char* dst, const void* end, const int value) {
  /* checks done in subfunctions */
  char* next = dst;
  int value_adjusted = value;
  if (value < 0) {
    next = insert_verbatim(next, end, '-');
    value_adjusted = (-1) * value;
  }
  return insert_uint_with_base(next, end, static_cast<unsigned int>(value_adjusted), 10);
}

char* insert_u(char* dst, const void* end, const unsigned int value) {
  /* checks done in subfunctions */
  return insert_uint_with_base(dst, end, value, 10);
}

char* insert_c(char* dst, const void* end, const char value) {
  /* checks done in insert_verbatim */
  return insert_verbatim(dst, end, value);
}

char* insert_s(char* dst, const void* end, const char* value) {
  if (!dst
      || !end
      || !value
      || end <= dst) {
    return nullptr;
  }
  for (const char* s = value; '\0' != *s; ++s) {
    if(dst < end) {
      *dst = *s;
    } else {
      return nullptr;
    }
    ++dst;
  }
  return dst;
}

char* insert_x(char* dst, const void* end, const unsigned int value) {
  /* checks done in subfunctions */
  char* next = insert_verbatim(dst, end, '0');
  next = insert_verbatim(next, end, 'x');
  return insert_uint_with_base(next, end, value, 16);
}

char* insert_b(char* dst, const void* end, const unsigned int value) {
  /* checks done in subfunctions */
  char* next = insert_verbatim(dst, end, '0');
  next = insert_verbatim(next, end, 'b');
  return insert_uint_with_base(next, end, value, 2);
}

char* insert_percent(char* dst, const void* end) {
  /* checks done in insert_verbatim */
  return insert_verbatim(dst, end, '%');
}

char* insert_end(char* dst, const void* end) {
  /* checks done in insert_verbatim */
  return insert_verbatim(dst, end, '\0');
}

char* insert_uint_with_base(char* dst, const void* end,
                            const unsigned int value, const unsigned int base) {
  if(!dst
     || !end
     || end <= dst
     || 0 == base
     || base > 16) {
       return nullptr;
     }
  /*
    this funcion works recursively from the last digit to the first
   */
  char digit =  digits[value%base];          /* get the current digit */
  unsigned int shifted_value = value / base; /* shift value right in base */
  char* write_to = dst;
  if (shifted_value) { /* there are digits left on the left */
    /* recursion to write to the left first */
    write_to = insert_uint_with_base(dst, end, shifted_value, base);
  }
  return insert_verbatim(write_to, end, digit);
}
