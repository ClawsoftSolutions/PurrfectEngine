#ifndef   _PURRFECT_ENGINE_STRING_VIEW_HPP_
#define   _PURRFECT_ENGINE_STRING_VIEW_HPP_

#include <ostream>
#include <functional>

namespace PurrfectEngine {

  class StringBuilder;

  // If you've been looking at the repo's source code you might've asked yourself "Why does CHP implement every data structure".
  // The answer wasn't clear to me up until @CyberWordPL said that it's "bcz you are C not C++ tf". (Get it? Cuz CHP = C Hating Python... forget it)
  class StringView {
  public:
    constexpr StringView()
      : m_data(nullptr)
    {}

    constexpr StringView(const char *cstr)
      : m_data(cstr), m_count(cstrLength(cstr))
    {}

    constexpr StringView(const char *data, size_t count)
      : m_data(data), m_count(count)
    {}

    StringView(const StringView &other)
    {
      *this = other;
    }

    StringView(StringView &&other)
      : m_data(other.m_data), m_count(other.m_count)
    {}

    StringView &operator =(const StringView &other) {
      if (this == &other) return *this;

      m_data = other.m_data;
      m_count = other.m_count;

      return *this;
    }

    friend std::ostream &operator <<(std::ostream &os, const StringView &sv);

    char *cstr();

    StringView chop(const size_t &i);
    StringView chopByDelim(char delim, bool chopOffDelim = true);
    StringView chopWhile(std::function<bool(char)>&& func);

    bool startsWith(const StringView &sv) const;

    friend bool operator ==(const StringView &lhs, const StringView &rhs);
    friend bool operator ==(const StringView &lhs, const char *const &rhs);

    friend StringBuilder &operator <<(StringBuilder &sb, const StringView &sv);

    constexpr const char &operator [](const size_t &i) const { return m_data[i]; }

    constexpr size_t size() const { return m_count; }
    constexpr const char *data() const { return m_data; }
  private:
    constexpr size_t cstrLength(const char *cstr) {
      const char *ptr = cstr;
      for (; *ptr; ++ptr);
      return ptr-cstr;
    }
  private:
    const char *m_data;
    size_t m_count = 0;
  };

}

#endif // _PURRFECT_ENGINE_STRING_VIEW_HPP_