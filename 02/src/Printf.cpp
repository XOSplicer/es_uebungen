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
 */
char* Printf(char* dst, const void* end, const char* fmt, ...) {
  if (!dst
      || !end
      || !fmt
      || end <= dst) {
    return nullptr;
    }

  char* next_writable = dst;

  for (const char* fmt_position = fmt; '\0' != *fmt_position ; ++fmt_position) {
    if ('%' == *fmt_position) { /* insert argument */
      /* TODO code format here */

    } else { /* insert verbatim */
      next_writable = insert_verbatim(next_writable, end, *fmt_position);
    }
    if (!next_writable) { /* check if previous insert was successfull */
      return nullptr;
    }
  }
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
