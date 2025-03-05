#ifndef   _PURRFECT_ENGINE_STRING_BUILDER_HPP_
#define   _PURRFECT_ENGINE_STRING_BUILDER_HPP_

#include "result.hpp"
#include "stringView.hpp"

#include <stdlib.h>

namespace PurrfectEngine {

  class StringView;
  class StringBuilder {
  public:
    constexpr StringBuilder()
    {}
    
    constexpr StringBuilder(size_t initialCapacity)
      : m_capacity(initialCapacity)
    {
      if (!m_capacity) return;
      m_data = (char*)malloc(m_capacity);
      if (!m_data) throw CodeException(Code::BuyMoreRAM);
    }

    StringBuilder &write(const char *const &string, size_t strLen);
    StringBuilder &operator <<(const bool &value);
    StringBuilder &operator <<(const char *const &value);

    void reserve(const size_t &count);

    constexpr operator StringView() const { return StringView(m_data, m_count); }
  private:
    char *m_data = nullptr;
    size_t m_capacity = 0;
    size_t m_count = 0;
  };

}

#endif // _PURRFECT_ENGINE_STRING_BUILDER_HPP_