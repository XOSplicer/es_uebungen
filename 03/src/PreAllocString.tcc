/**
 * @author Felix Stegmaier
 */

template<size_t SIZE>
void PreAllocString<SIZE>::Empty() {
  DEBUG("empty");
  for (size_t pos = 0; pos < SizeOf(); pos++) {
    m_content[pos] = '\0';
  }
  m_length = 0;
  m_next_writable = &m_content[0];
}

template<size_t SIZE>
void PreAllocString<SIZE>::AddFormat(const char * format, ...) {
  va_list args;
  va_start(args, format);
  m_next_writable = Printf(m_next_writable, (&(m_content[SIZE])), format, args);
  int r = m_next_writable? 1 : 0;
  DEBUG("Add Format Printf return: " << r);
  va_end(args);
}

template<size_t SIZE>
void PreAllocString<SIZE>::AddWhiteSpace() {
  if(m_next_writable >= m_content + SizeOf()) {
    return;
  }
  *m_next_writable = 's';
  m_next_writable++;
  m_length++;
  /* no need to insert \0, since it was already set at Empty */
  DEBUG("content: " << m_content);
}
