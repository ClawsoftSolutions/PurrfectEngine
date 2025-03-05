#include "PurrfectEngine/stringBuilder.hpp"

#include "PurrfectEngine/stringView.hpp"

#include <string.h>

namespace PurrfectEngine {

  StringBuilder &StringBuilder::write(const char *const &string, size_t strLen) {
    if (m_count+strLen >= m_capacity) reserve(m_count+strLen-m_capacity);
    memcpy(&m_data[m_count], string, strLen);
    m_count += strLen;
    return *this;
  }

  StringBuilder &StringBuilder::operator <<(const bool &value) {
    return value?write("true", 4):write("false", 5);
  }

  StringBuilder &StringBuilder::operator <<(const char *const &value) {
    return write(value, strlen(value));
  }

  void StringBuilder::reserve(const size_t &count) {
    if (m_count+count <= m_capacity) return;
    m_capacity += m_count+count-m_capacity;
    m_data = (char*)realloc(m_data, m_capacity);
    if (!m_data) throw CodeException(Code::BuyMoreRAM);
  }

}