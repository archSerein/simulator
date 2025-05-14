#ifndef __HELPER_HPP__
#define __HELPER_HPP__

#include <bitset>
#include <deque>
#include <variant>
#include <string>
#include "branchsim.hpp"
#include "btb.hpp"

const uint32_t INVALID_NUM = 0xffffffff;

struct SimConfig {
  std::string simulator_name; // Name of the simulator
  uint32_t set = INVALID_NUM;               // Value for --set
  uint32_t associativity = INVALID_NUM; // Value for --associativity
  uint32_t block = INVALID_NUM;          // Value for --block
  std::string trace_file; // Trace file name
  uint32_t pht_size = INVALID_NUM; // Value for --pht
    // Add other config options as needed
};
using Simulator = std::variant<branchsim, btb>;

struct SimConfig parse_arguments(int argc, char* argv[]);
Simulator create_simulator(const SimConfig &config, const std::string &type);
uint32_t  get_ghr_value(std::deque<std::bitset<BIT_WIDTH>>, uint32_t);

#endif // !__HELPER_HPP__
