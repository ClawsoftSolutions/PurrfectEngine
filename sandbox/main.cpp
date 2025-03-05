#include "PurrfectEngine/ecs.hpp"

#include <iostream>

struct A {
  int value = 0;

  A() = default;

  A(int v)
    : value(v)
  {}
};

struct B {
  float value = 0.0f;

  B() = default;

  B(float v)
    : value(v)
  {}
};

struct C {
  char value = 'C';

  C() = default;

  C(char v)
    : value(v)
  {}
};

int main(void) {
  PurrfectEngine::ECS ecs{};

  ecs.registerComponent<A>();
  ecs.registerComponent<B>();
  ecs.registerComponent<C>();

  const PurrfectEngine::Entity &entityA = ecs.create();
  ecs.emplace<A>(entityA, 69);
  ecs.emplace<C>(entityA, 'A');

  const PurrfectEngine::Entity &entityB = ecs.create();
  ecs.emplace<B>(entityB, 420.0f);
  ecs.emplace<C>(entityB, 'B');

  const PurrfectEngine::Entity &entityC = ecs.create();
  ecs.emplace<C>(entityC, 'C');
  ecs.emplace<B>(entityC, 1337.0f);

  ecs.view<C>().forEach([](const PurrfectEngine::Entity &entity, C &c) {
    std::cout << entity << ": " << c.value << std::endl;
  });

  ecs.view<C, B>().forEach([](const PurrfectEngine::Entity &entity, C &c, B &b) {
    std::cout << entity << " (" << c.value << "): " << b.value << std::endl;
  });

  return 0;
}