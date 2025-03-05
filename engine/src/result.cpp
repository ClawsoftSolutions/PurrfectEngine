#include "PurrfectEngine/result.hpp"

#include "PurrfectEngine/core.hpp"

namespace PurrfectEngine {

  std::ostream &operator <<(std::ostream &os, const Code &code) {
    return os << code.toCstr();
  }

  const char *Code::toCstr() const {
    switch (m_value) {
    case Success: return "Success";
    case BuyMoreRAM: return "Out of memory";
    case OutOfBounds: return "Out of bounds";
    case DeadEntity: return "Dead entity";
    }
    
    throw CodeException(Unreachable);
  }

  const char *CodeException::what() const noexcept {
    return m_code;
  }

}