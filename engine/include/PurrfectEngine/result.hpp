#ifndef   _PURRFECT_ENGINE_RESULT_HPP_
#define   _PURRFECT_ENGINE_RESULT_HPP_

#include <stdint.h>

#include <ostream>
#include <exception>

namespace PurrfectEngine {

  class Code { // https://stackoverflow.com/a/53284026
  public:
    enum Value : int32_t {
      Success = 0,
      BuyMoreRAM = -1, // Tsoding reference
      OutOfBounds = -2,
      Unreachable = -3,
      DeadEntity = -4,
    };
  public:
    constexpr Code(const Value &value)
      : m_value(value)
    {}

    constexpr Code(const Code &other)
      : m_value(other)
    {}

    constexpr operator Value() const { return m_value; }
    constexpr operator bool() const { return m_value >= Success; }

    operator const char *() const { return toCstr(); }

    constexpr bool operator !() const { return m_value < Success; }

    constexpr bool operator ==(const Code &other) const { return m_value == other.m_value; }
    constexpr bool operator ==(const Value &other) const { return m_value == other; }

    constexpr bool operator !=(const Code &other) const { return m_value != other.m_value; }
    constexpr bool operator !=(const Value &other) const { return m_value != other; }

    friend std::ostream &operator <<(std::ostream &os, const Code &code);
  private:
    const char *toCstr() const;
  private:
    Value m_value;
  };

  template <typename T>
  class Result {
  public:
    constexpr Result(Code code)
      : m_code(code)
    {}
  private:
    T m_value;
    Code m_code;
  };

  class CodeException : public std::exception {
  public:
    CodeException(const Code &code)
      : m_code(code)
    {}

    virtual const char *what() const noexcept override;
  private:
    Code m_code;
  };

}

#endif // _PURRFECT_ENGINE_RESULT_HPP_