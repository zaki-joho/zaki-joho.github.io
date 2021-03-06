#include <stdint.h>
#include <time.h>
#include <chrono>

struct xor_shift {
  using result_type = uint32_t;
  result_type w = 123456789, x = 362436069, y = 521288629, z = 88675123;
  xor_shift(result_type seed = time(nullptr)) {
    w = seed;
    x = w << 13;
    y = (w >> 9) ^ (x << 6);
    z = y >> 7;
  }
  static constexpr result_type min() { return 0; }
  static constexpr result_type max() { return 0x7FFFFFFF; }
  result_type operator()() {
    result_type t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = (w ^ (w >> 19) ^ (t ^ (t >> 8)));
  }
  result_type rand() {
    result_type t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = (w ^ (w >> 19) ^ (t ^ (t >> 8)));
  }
  // [min,max] の整数値乱数
  result_type rand_int(result_type min = 0, result_type max = 0x7FFFFFFF) {
    return rand() % (max - min + 1) + min;
  }
  // [min,max] の浮動小数点乱数
  double rand_double(double min = 0, double max = 1) {
    return (double)(rand() % 0xFFFF) / 0xFFFF * (max - min) + min;
  }
};

struct stop_watch {
  void start() { clock_begin = std::chrono::high_resolution_clock::now(); }
  long long stop() {
    auto clock_end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       clock_end - clock_begin)
                       .count();
    return elapsed;
  }

 private:
  std::chrono::high_resolution_clock::time_point clock_begin;
};
