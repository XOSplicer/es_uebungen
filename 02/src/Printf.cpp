/**
 * @author Felix Stegmaier
 */

#include "Printf.h"

/*
Insert functions:
Insert value into dst for given format.
Return next writable char fmt_position.
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
 * Minimal printf implementation
 * Support of
 *  (0) Varbatim text
 *  (1) %d for signed int
 *  (2) %u for unsigned int
 *  (3) %c for a single char
 *  (4) %s for a null terminated string
 *  (5) %x for hexadecimal (0xaf) TODO signed ? raw ?
 *  (6) %b for binary (0b10101) TODO signed ? raw ?
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
char* Printf(char* dst, const void* end, const char* fmt, ...) {
  if (!dst
      || !end
      || !fmt
      || end <= dst) {
    return nullptr;
    }

  char* next_writable = dst;

  va_list args;
  va_start(args, fmt);

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
  va_end(args);
  next_writable = insert_end(next_writable, end);
  return next_writable;
}

char* insert_verbatim(char* dst, const void* end, const char value) {
  DEBUG("insert verbatim: " << value);
  if (!dst
      || !end
      /* value may be 0 for '\0' */
      || end <= dst) {
    return nullptr;
  }
  *dst = value;
  return dst+1;
}

char* insert_d(char* dst, const void* end, const int /*value*/) {
  if (!dst
      || !end
      || end <= dst) {
    return nullptr;
  }
  //TODO
  /*test*/ return nullptr;
}

char* insert_u(char* dst, const void* end, const unsigned int /*value*/) {
  if (!dst
      || !end
      || end <= dst) {
    return nullptr;
  }
  //TODO
  /*test*/ return nullptr;
}

char* insert_c(char* dst, const void* end, const char value) {
  /* checks done in insert_verbatim */
  DEBUG("insert char");
  return insert_verbatim(dst, end, value);
}

char* insert_s(char* dst, const void* end, const char* value) {
  DEBUG("insert string: " << value);
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

char* insert_x(char* dst, const void* end, const unsigned int /*value*/) {
  if (!dst
      || !end
      || end <= dst) {
    return nullptr;
  }
  //TODO
  /*test*/ return nullptr;
}

char* insert_b(char* dst, const void* end, const unsigned int /*value*/) {
  if (!dst
      || !end
      || end <= dst) {
    return nullptr;
  }
  //TODO
  /*test*/ return nullptr;
}

char* insert_percent(char* dst, const void* end) {
  /* checks done in insert_verbatim */
  DEBUG("insert percent");
  return insert_verbatim(dst, end, '%');
}

char* insert_end(char* dst, const void* end) {
  /* checks done in insert_verbatim */
  DEBUG("insert end");
  return insert_verbatim(dst, end, '\0');
}
