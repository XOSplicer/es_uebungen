#ifndef PREALLOCSTRING_H
#define PREALLOCSTRING_H

#include <cstddef>

#include "debug.h"
#include "util.h"
#include "Printf.h"

#define CREATE(varName, size)\
  PreAllocString<size> varName;

template<size_t SIZE>
class PreAllocString {
  public:

    PreAllocString() {
      DEBUG("New PreAllocString of SIZE " << SIZE);
      Empty();
    }

    NO_COPY_INSTANCE(PreAllocString)

    /* Current number of characters in string */
    size_t GetLength() const {
      return m_next_writable - &m_content[0];
    }

    /* Maximum number of characters possible */
    constexpr size_t SizeOf() {
      return m_size;
    }

    /* Empty the string , set length field to zero */
    void Empty() {
      DEBUG("Empty");
      for (size_t pos = 0; pos < SizeOf(); pos++) {
        m_content[pos] = '\0';
      }
      m_next_writable = &m_content[0];
    }

    /* insert formated string */
    void AddFormat(const char * format, ...) {
      DEBUG("AddFormat: " << format);
      va_list args;
      va_start(args, format);
      m_next_writable = Printf(m_next_writable, (&(m_content[SIZE])), format, args);
      m_next_writable--; //directly at \0 not after
      va_end(args);
    }

    /* insert one whitespace character */
    void AddWhiteSpace() {
      if(m_next_writable >= m_content + SizeOf()) {
        return;
      }
      DEBUG("AddWhiteSpace");
      *m_next_writable = ' ';
      m_next_writable++;
      /* no need to insert \0, since it was already set at Empty */
    }

    /* operators */
    /* const char* cast */
    operator const char*() const {
      return raw();
    }

    /* const void* cast */
    operator const void*() const {
      return raw();
    }

    /* indexing to get char*/
    const char& operator[](const int idx) {
      return m_content[idx];
    }

    /* assignment operators */
    PreAllocString& operator=(char rhs) {
      Empty();
      AddFormat("%c", rhs);
      return *this;
    }

    PreAllocString& operator=(const char* rhs) {
        Empty();
        AddFormat("%s", rhs);
        return *this;
    }

    PreAllocString& operator=(char* const rhs) {
      Empty();
      AddFormat("%s", rhs);
      return *this;
    }

    PreAllocString& operator+=(char rhs) {
      AddFormat("%c", rhs);
      return *this;
    }

    PreAllocString& operator+=(char const* rhs) {
      AddFormat("%s", rhs);
      return *this;
    }

  private:
    const char* raw() const {
      return &m_content[0];
    }

  protected:

  private:
    const size_t m_size = SIZE;
    char m_content[SIZE];
    char* m_next_writable;
};

#endif
