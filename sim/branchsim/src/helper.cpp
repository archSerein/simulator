#include <iostream>
#include <vector>
#include "helper.hpp"

struct SimConfig parse_arguments(int argc, char* argv[]) {
    SimConfig config;
    std::vector<std::string> args(argv + 1, argv + argc); // Skip argv[0] (program name)
    for (size_t i = 0; i < args.size(); ++i) {
      std::string arg = args[i];
      // handle set, associativity, block
      if (arg == "set")
        config.set = std::stoi(args[++i]);
      else if (arg == "associativity")
        config.associativity = std::stoi(args[++i]);
      else if (arg == "block")
        config.block = std::stoi(args[++i]);
      else if (arg == "trace_file")
        config.trace_file = args[++i];
      else if (arg == "pht")
        config.pht_size = std::stoi(args[++i]);
      else {
        config.simulator_name = arg;
        ++i;
      }
    }
    return config;
}

Simulator create_simulator(const SimConfig &config, const std::string &type) {
    if (type == "branchsim") {
      if (config.pht_size == INVALID_NUM) {
        return branchsim();
      } else {
        return branchsim(config.pht_size);
      }
    } else if (type == "btb") {
      if (config.set == INVALID_NUM) {
        return btb();
      } else {
        if (config.associativity == INVALID_NUM || config.block == INVALID_NUM) {
          std::cerr << "Associativity and block size must be specified for BTB" << std::endl;
          throw std::invalid_argument("Associativity and block size must be specified for BTB");
        }
        struct btb_config btb_config {config.block, config.associativity, config.set};
        return btb(btb_config);
      }
    } else {
        std::cerr << "Unknown simulator type: " << type << std::endl;
        throw std::invalid_argument("Unknown simulator type");
    }
}
