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
  m_next_writable = m_content;
}

template<size_t SIZE>
void PreAllocString<SIZE>::AddFormat(const char * format, ...) {

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
