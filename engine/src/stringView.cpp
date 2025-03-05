#include "PurrfectEngine/stringView.hpp"

#include "PurrfectEngine/result.hpp"

#include <stdlib.h>
#include <string.h>

namespace PurrfectEngine {

  std::ostream &operator <<(std::ostream &os, const StringView &sv) {
    return os.write(sv.m_data, sv.m_count);
  }

  char *StringView::cstr() {
    if (!m_count) return nullptr;
    char *cstr = (char*)malloc(m_count+1);
    memcpy(cstr, m_data, m_count);
    cstr[m_count] = '\0';
    return cstr;
  }

  StringView StringView::chop(const size_t &i) {
    if (m_count < i) throw CodeException(Code::OutOfBounds);
    StringView result = StringView(m_data, i);
    m_data += i;
    m_count -= i;
    return result;
  }

  StringView StringView::chopByDelim(char delim, bool chopOffDelim) {
    size_t i = 0;
    for (; i < m_count && m_data[i] != delim; ++i);
    StringView result = StringView(m_data, i);
    if (i < m_count) ++i;
    m_data += i;
    m_count -= i;
    return result;
  }

  StringView StringView::chopWhile(std::function<bool(char)>&& func) {
    size_t i = 0;
    for (; i < m_count && func(m_data[i]); ++i);
    StringView result = StringView(m_data, i);
    m_data += i;
    m_count -= i;
    return result;
  }

  bool StringView::startsWith(const StringView &sv) const {
    if (sv.m_count > m_count) return false;
    for (size_t i = 0; i < sv.m_count; ++i)
      if (m_data[i] != sv[i]) return false;
    return true;
  }

  bool operator ==(const StringView &lhs, const StringView &rhs) {
    return lhs.m_count == rhs.m_count && !memcmp(lhs.m_data, rhs.m_data, lhs.m_count);
  }

  bool operator ==(const StringView &lhs, const char *const &rhs) {
    return lhs.m_count == strlen(rhs) && !strncmp(lhs.m_data, rhs, lhs.m_count);
  }

}