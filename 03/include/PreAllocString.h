#ifndef PREALLOCSTRING_H
#define PREALLOCSTRING_H

#include <cstddef>

#include "debug.h"
#include "Printf.h"



/*
TODO fix copy constructor
TODO implement things
TODO import the Printf
 */





#define CREATE(varName, size)\
  PreAllocString<size> varName;

template<size_t SIZE>
class PreAllocString {
  public:

    PreAllocString()
    /*:*/ {
      DEBUG("New PreAllocString of SIZE " << SIZE);
      Empty();
    }

    //PreAllocString(const PreAllocString&) = 0;

    /* Current number of characters in string */
    size_t GetLength() const {
      return m_next_writable - &m_content[0];
    }

    /* Maximum number of characters possible */
    constexpr size_t SizeOf() {
      return m_size;
    }

    /* Empty the string , set length field to zero */
    void Empty();

    /* insert formated string */
    void AddFormat(const char * format, ...);

    /* insert one whitespace character */
    void AddWhiteSpace();

    /* operators */
    operator const char *() const;
    operator const void *() const;
    const char& operator[](const int idx);

    PreAllocString& operator=(char rhs);
    PreAllocString& operator=(const char* rhs);
    PreAllocString& operator=(char* const rhs);

    PreAllocString& operator+=(char rhs);
    PreAllocString& operator+=(char const* rhs);

    const char* raw() {
      return &m_content[0];
    }

  protected:

  private:
    const size_t m_size = SIZE;
    char m_content[SIZE];
    char* m_next_writable;
};

#include "../src/PreAllocString.tcc"

#endif
