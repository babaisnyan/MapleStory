#pragma once
#include <random>

namespace utils {
  class Randomizer {
  public:
    static Randomizer& GetInstance() {
      static Randomizer instance;
      return instance;
    }

  public:
    int32_t GetRandomInt32(const int32_t min, const int32_t max) {
      std::uniform_int_distribution distribution(min, max);
      return distribution(_random_generator);
    }

  private:
    Randomizer() {
      _random_generator = std::mt19937(_random_device());
    }

  private:
    std::random_device _random_device;
    std::mt19937 _random_generator;
  };
}
