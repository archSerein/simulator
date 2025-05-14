#include <functional>
#include <unordered_map>
#include <cstring>
#include <iostream>
#include "defs.hpp"
#include "helper.hpp"

std::mutex info;

static const std::unordered_map<std::string, std::function<void(FILE *, struct SimConfig)>> simulator_map = {
  {"saturat_simulator", saturat_simulator},
  {"global_history", global_history_simulator},
  {"local_history", local_history_simulator},
  {"bimodal", bimodal_simulator},
  {"perceptron", perceptron_simulator},
  {"bp_predictor", bp_predictor_simulator},
  // Add other simulator functions here
};

int main(int argc, char *argv[]) {
  struct SimConfig config = parse_arguments(argc, argv);
  char command[256];
  snprintf(command, sizeof(command), "bzcat %s", config.trace_file.c_str());

  // Check if the simulator name is valid
  if (config.simulator_name.empty()) {
    for (const auto &entry : simulator_map) {
      const std::string &simulator_name = entry.first;
      const auto &simulator_func = entry.second;
      FILE *fp = popen(command, "r");
      if (fp == nullptr) {
        std::cerr << "Failed to open file %s: %s\n" << config.trace_file << strerror(errno);
        return 1;
      }
      std::cout << "Running simulator: " << simulator_name << std::endl;
      simulator_func(fp, config);
    }
  } else {
      FILE *fp = popen(command, "r");
      if (fp == nullptr) {
        std::cerr << "Failed to open file %s: %s\n" << config.trace_file << strerror(errno);
        return 1;
      }
      std::cout << "Running simulator: " << config.simulator_name << std::endl;
      simulator_map.find(config.simulator_name)->second(fp, config);
  }
  return 0;
}
