#include <iostream>
#include <cstdlib>
#include "defs.hpp"
#include "debug.hpp"
#include "helper.hpp"

void saturat_simulator(FILE *fp, struct SimConfig config) {
  auto bpu_simulator = create_simulator(config, "branchsim");
  auto bpu_sim = std::get_if<branchsim>(&bpu_simulator);
  auto btb_simulator = create_simulator(config, "btb");
  auto btb_sim = std::get_if<btb>(&btb_simulator);
  uint32_t pc;
  uint32_t snpc, dnpc;
  uint32_t mispredictions = 0;
  uint32_t pc_cnt = 1;
  if (fread(&pc, sizeof(uint32_t), 1, fp) != 1) {
    std::cerr << "Error reading PC from trace file." << std::endl;
    pclose(fp);
    EXIT_FAILURE;
  }
  while (fread(&dnpc, sizeof(uint32_t), 1, fp) == 1) {
    bool taken = bpu_sim->saturate_branch_predictor();
    if (taken) {
      snpc = btb_sim->get_target(pc);
    } else {
      snpc = pc + 4;
    }
    if (snpc != dnpc) {
      mispredictions++;
      bpu_sim->update_saturate_branch_predictor(dnpc != pc + 4);
      btb_sim->update_btb(pc, dnpc);
    }
    pc_cnt++;
    pc = dnpc;
  }
  pclose(fp);
  std::lock_guard<std::mutex> lock(info);
  std::cout << "Mispredictions: " << mispredictions << std::endl;
  std::cout << "Total predictions: " << pc_cnt << std::endl;
  std::cout << "Misprediction rate: " << (static_cast<float>(mispredictions) / pc_cnt) * 100 << "%" << std::endl;
}

void global_history_simulator(FILE *fp, struct SimConfig config) {
  auto bpu_simulator = create_simulator(config, "branchsim");
  auto bpu_sim = std::get_if<branchsim>(&bpu_simulator);
  auto btb_simulator = create_simulator(config, "btb");
  auto btb_sim = std::get_if<btb>(&btb_simulator);
  uint32_t pc;
  uint32_t snpc, dnpc;
  uint32_t mispredictions = 0;
  uint32_t pc_cnt = 1;
  if (fread(&pc, sizeof(uint32_t), 1, fp) != 1) {
    std::cerr << "Error reading PC from trace file." << std::endl;
    pclose(fp);
    EXIT_FAILURE;
  }
  while (fread(&dnpc, sizeof(uint32_t), 1, fp) == 1) {
    bool taken = bpu_sim->saturate_global_history_predictor(pc);
    if (taken) {
      snpc = btb_sim->get_target(pc);
    } else {
      snpc = pc + 4;
    }
    if (snpc != dnpc) {
      mispredictions++;
      bpu_sim->update_saturate_global_history_predictor(pc, dnpc != pc + 4);
      btb_sim->update_btb(pc, dnpc);
    }
    pc_cnt++;
    pc = dnpc;
  }
  pclose(fp);
  std::lock_guard<std::mutex> lock(info);
  std::cout << "Mispredictions: " << mispredictions << std::endl;
  std::cout << "Total predictions: " << pc_cnt << std::endl;
  std::cout << "Misprediction rate: " << (static_cast<float>(mispredictions) / pc_cnt) * 100 << "%" << std::endl;
}

void bimodal_simulator(FILE *fp, struct SimConfig config) {
  auto bpu_simulator = create_simulator(config, "branchsim");
  auto bpu_sim = std::get_if<branchsim>(&bpu_simulator);
  auto btb_simulator = create_simulator(config, "btb");
  auto btb_sim = std::get_if<btb>(&btb_simulator);
  uint32_t pc;
  uint32_t snpc, dnpc;
  uint32_t mispredictions = 0;
  uint32_t pc_cnt = 1;
  if (fread(&pc, sizeof(uint32_t), 1, fp) != 1) {
    std::cerr << "Error reading PC from trace file." << std::endl;
    pclose(fp);
    EXIT_FAILURE;
  }
  while (fread(&dnpc, sizeof(uint32_t), 1, fp) == 1) {
    bool taken = bpu_sim->saturate_bimodal_predictor(pc);
    if (taken) {
      snpc = btb_sim->get_target(pc);
    } else {
      snpc = pc + 4;
    }
    if (snpc != dnpc) {
      mispredictions++;
      bpu_sim->update_saturate_bimodal_predictor(pc, dnpc != pc + 4);
      btb_sim->update_btb(pc, dnpc);
    }
    Log("pc: %x, snpc: %x, dnpc: %x", pc, snpc, dnpc);
    pc_cnt++;
    pc = dnpc;
  }
  pclose(fp);
  std::lock_guard<std::mutex> lock(info);
  std::cout << "Mispredictions: " << mispredictions << std::endl;
  std::cout << "Total predictions: " << pc_cnt << std::endl;
  std::cout << "Misprediction rate: " << (static_cast<float>(mispredictions) / pc_cnt) * 100 << "%" << std::endl;
}

void perceptron_simulator(FILE *fp, struct SimConfig config) {
  auto bpu_simulator = create_simulator(config, "branchsim");
  auto bpu_sim = std::get_if<branchsim>(&bpu_simulator);
  auto btb_simulator = create_simulator(config, "btb");
  auto btb_sim = std::get_if<btb>(&btb_simulator);
  uint32_t pc;
  uint32_t snpc, dnpc;
  uint32_t mispredictions = 0;
  uint32_t pc_cnt = 1;
  if (fread(&pc, sizeof(uint32_t), 1, fp) != 1) {
    std::cerr << "Error reading PC from trace file." << std::endl;
    pclose(fp);
    EXIT_FAILURE;
  }
  while (fread(&dnpc, sizeof(uint32_t), 1, fp) == 1) {
    bool taken = bpu_sim->perceptron_predictor(pc);
    if (taken) {
      snpc = btb_sim->get_target(pc);
    } else {
      snpc = pc + 4;
    }
    if (snpc != dnpc) {
      mispredictions++;
      btb_sim->update_btb(pc, dnpc);
    }
    bpu_sim->update_perceptron_predictor(pc, dnpc != pc + 4, taken, dnpc);
    pc_cnt++;
    pc = dnpc;
  }
  pclose(fp);
  std::lock_guard<std::mutex> lock(info);
  std::cout << "Mispredictions: " << mispredictions << std::endl;
  std::cout << "Total predictions: " << pc_cnt << std::endl;
  std::cout << "Misprediction rate: " << (static_cast<float>(mispredictions) / pc_cnt) * 100 << "%" << std::endl;
}

void bp_predictor_simulator(FILE *fp, struct SimConfig config) {
  auto bpu_simulator = create_simulator(config, "branchsim");
  auto bpu_sim = std::get_if<branchsim>(&bpu_simulator);
  auto btb_simulator = create_simulator(config, "btb");
  auto btb_sim = std::get_if<btb>(&btb_simulator);
  uint32_t pc;
  uint32_t snpc, dnpc;
  uint32_t mispredictions = 0;
  uint32_t pc_cnt = 1;
  if (fread(&pc, sizeof(uint32_t), 1, fp) != 1) {
    std::cerr << "Error reading PC from trace file." << std::endl;
    pclose(fp);
    EXIT_FAILURE;
  }
  while (fread(&dnpc, sizeof(uint32_t), 1, fp) == 1) {
    bool taken = bpu_sim->backward_propagation_predictor();
    if (taken) {
      snpc = btb_sim->get_target(pc);
    } else {
      snpc = pc + 4;
    }
    if (snpc != dnpc) {
      mispredictions++;
      bpu_sim->update_backward_propagation_predictor(dnpc != pc + 4);
      btb_sim->update_btb(pc, dnpc);
    }
    Log("pc: %x, snpc: %x, dnpc: %x", pc, snpc, dnpc);
    pc_cnt++;
    pc = dnpc;
  }
  pclose(fp);
  std::lock_guard<std::mutex> lock(info);
  std::cout << "Mispredictions: " << mispredictions << std::endl;
  std::cout << "Total predictions: " << pc_cnt << std::endl;
  std::cout << "Misprediction rate: " << (static_cast<float>(mispredictions) / pc_cnt) * 100 << "%" << std::endl;
}

void local_history_simulator(FILE *fp, struct SimConfig config) {
  auto bpu_simulator = create_simulator(config, "branchsim");
  auto bpu_sim = std::get_if<branchsim>(&bpu_simulator);
  auto btb_simulator = create_simulator(config, "btb");
  auto btb_sim = std::get_if<btb>(&btb_simulator);
  uint32_t pc;
  uint32_t snpc, dnpc;
  uint32_t mispredictions = 0;
  uint32_t pc_cnt = 1;
  if (fread(&pc, sizeof(uint32_t), 1, fp) != 1) {
    std::cerr << "Error reading PC from trace file." << std::endl;
    pclose(fp);
    EXIT_FAILURE;
  }
  while (fread(&dnpc, sizeof(uint32_t), 1, fp) == 1) {
    bool taken = bpu_sim->saturate_local_history_predictor(pc);
    if (taken) {
      snpc = btb_sim->get_target(pc);
    } else {
      snpc = pc + 4;
    }
    if (snpc != dnpc) {
      mispredictions++;
      bpu_sim->update_saturate_local_history_predictor(dnpc != pc + 4);
      btb_sim->update_btb(pc, dnpc);
    }
    Log("pc: %x, snpc: %x, dnpc: %x", pc, snpc, dnpc);
    pc_cnt++;
    pc = dnpc;
  }
  pclose(fp);
  std::lock_guard<std::mutex> lock(info);
  std::cout << "Mispredictions: " << mispredictions << std::endl;
  std::cout << "Total predictions: " << pc_cnt << std::endl;
  std::cout << "Misprediction rate: " << (static_cast<float>(mispredictions) / pc_cnt) * 100 << "%" << std::endl;
}
