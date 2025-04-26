#include <functional>
#include <iostream>
#include <unordered_map>

std::unordered_map<std::string, std::function<void(uint32_t, bool)>> update_map;
std::unordered_map<std::string, std::function<bool(uint32_t)>> predict_map;

// 

int main(int argc, char *argv[]) {
  if (argc < 2) {
      printf("Usage: %s <trace_file>\n", argv[0]);
      return 1;
  }

  const char *trace_file = argv[1];

}
