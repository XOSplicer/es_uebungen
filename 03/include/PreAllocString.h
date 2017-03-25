#ifndef PREALLOCSTRING_H
#define PREALLOCSTRING_H

#define CREATE(varName, size)
//TODO fill marco

class PreAllocString {
  public:

    //TODO add constructor

    /* Current number of characters in string */
    size_t GetLength() const;

    /* Maximum number of characters possible */
    constexpr size_t SizeOf();

    /* Empty the string , set length field to zero */
    void Empty () ;

    /* insert formated string */
    void AddFormat(const char * format, ...);

    /* insert one whitespace character */
    void AddWhiteSpace();

    /* operators */
    operator const char *() const;
    operator const void *() const;
    const char & operator [](const int idx);

    PreAllocString & operator =(char rhs);
    PreAllocString & operator =(const char* rhs);
    PreAllocString & operator =(char* const rhs);

    PreAllocString & operator +=(char rhs);
    PreAllocString & operator +=(char const* rhs);

  protected:

  private:
    size_t length;
    const size_t size;

}

#endif
