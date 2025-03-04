#include "PurrfectEngine/array.hpp"

#include <iostream>

int main(void) {
  PurrfectEngine::Array<uint32_t> array = PurrfectEngine::Array<uint32_t>(10);
  for (size_t i = 0; i < 10; ++i)
    array.append(i);

  std::cout << "Size: " << array.size() << ", capacity: " << array.capacity() << std::endl;

  for (size_t i = 10; i;)
    if (!(--i % 2))
      array.fastRemove(i);

  std::cout << "Size: " << array.size() << ", capacity: " << array.capacity() << std::endl;

  return 0;
}