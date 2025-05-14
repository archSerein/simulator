#include <cmath>
#include <cstdint>
#include <cstdlib>
#include "helper.hpp"
#include "branchsim.hpp"

saturat_status branchsim::next_saturate_state(saturat_status state, bool taken) {
  switch (state) {
    case STRONGLY_TAKEN:
      return taken ? STRONGLY_TAKEN : WEAKLY_TAKEN;
    case WEAKLY_TAKEN:
      return taken ? STRONGLY_TAKEN : WEAKLY_NOT_TAKEN;
    case WEAKLY_NOT_TAKEN:
      return taken ? WEAKLY_TAKEN : STRONGLY_NOT_TAKEN;
    case STRONGLY_NOT_TAKEN:
      return taken ? WEAKLY_NOT_TAKEN : STRONGLY_NOT_TAKEN;
    default:
      return state; // Invalid state
  }
}

branchsim::branchsim(void) {
  saturat_counter = STRONGLY_NOT_TAKEN;
  bhr = 0;
  this->pht_size = PHT_SIZE;
  this->max_ghr_size = HIST_LEN;
  this->ghr.resize(HIST_LEN+1);
  this->pht.resize(pht_size, STRONGLY_NOT_TAKEN);
  this->perceptron_table.resize(this->pht_size, std::vector<int32_t>(HIST_LEN+1, 0));
  // Initialize the branch target buffer (BTB)
  PHT_ADDR_MASK = (1 << (static_cast<uint32_t>(std::log2(PHT_SIZE)))) - 1;
  GHR_MASK = (1 << (static_cast<uint32_t>(std::log2(PHT_SIZE)))) - 1;
}


branchsim::branchsim(uint32_t pht_size) {
  saturat_counter = STRONGLY_NOT_TAKEN;
  bhr = 0;
  this->pht_size = pht_size;
  this->max_ghr_size = HIST_LEN;
  this->ghr.resize(HIST_LEN+1);
  this->pht.resize(pht_size, STRONGLY_NOT_TAKEN);
  this->perceptron_table.resize(this->pht_size, std::vector<int32_t>(HIST_LEN+1, 0));
  PHT_ADDR_MASK = (1 << (static_cast<uint32_t>(std::log2(pht_size)))) - 1;
  GHR_MASK = (1 << (static_cast<uint32_t>(std::log2(pht_size)))) - 1;
}

bool branchsim::saturate_branch_predictor() {
  bool prediction = branchsim::saturat_counter == STRONGLY_TAKEN ||
                    branchsim::saturat_counter == WEAKLY_TAKEN;
  return prediction;
}

void branchsim::update_saturate_branch_predictor(bool taken) {
  branchsim::saturat_counter = next_saturate_state(branchsim::saturat_counter, taken);
}

bool branchsim::saturate_global_history_predictor(uint32_t pc) {
  uint32_t index = ((pc >> 2) & this->PHT_ADDR_MASK) ^ get_ghr_value(this->ghr, std::log2(this->pht_size));
  saturat_status pht_value = this->pht[index];
  bool prediction = pht_value == STRONGLY_TAKEN ||
                    pht_value == WEAKLY_TAKEN;
  return prediction;
}

void branchsim::update_saturate_global_history_predictor(uint32_t pc, bool taken) {
  uint32_t index = ((pc >> 2) & this->PHT_ADDR_MASK) ^ get_ghr_value(this->ghr, std::log2(this->pht_size));
  saturat_status pht_value = this->pht[index];
  this->pht[index] = next_saturate_state(pht_value, taken);
  // Update the global history register (GHR)
  this->ghr.pop_front();
  this->ghr.push_back(taken);
}

void branchsim::update_saturate_local_history_predictor(bool taken) {
  // TODO()
}

bool branchsim::saturate_local_history_predictor(uint32_t pc) {
  // TODO()
  return false;
}

void branchsim::update_perceptron_predictor(uint32_t pc, bool resolveDir, bool preDir, uint32_t target) {
  uint32_t index = ((pc >> 2) & this->PHT_ADDR_MASK);
  if (resolveDir != preDir || this->perceptronSteps <= THRESHOLD) {
    // If the branch was taken, increment the bias value, Else, decrement it.
    if (resolveDir) {
      int32_t updateVal = this->perceptron_table[index][0] + 1;
      if (updateVal > THRESHOLD+1) {
        this->perceptron_table[index][0] = THRESHOLD+1;
      } else {
        this->perceptron_table[index][0]++;
      }
    } else {
      int32_t updateVal = this->perceptron_table[index][0] - 1;
      if (updateVal < -THRESHOLD-1) {
        this->perceptron_table[index][0] = -THRESHOLD-1;
      } else {
        this->perceptron_table[index][0]--;
      }
    }
    // update the weights
    for (uint32_t i = 1; i <= HIST_LEN; i++) {
      // if the branch outcome matches the history, increment the weight
      // else, decrement it.
      if ((resolveDir && this->ghr.at(i-1).to_ulong()) ||
                                    (!resolveDir && !this->ghr.at(i-1).to_ulong())) {
        int32_t updateVal = this->perceptron_table[index][i] + 1;
        if (updateVal > THRESHOLD) {
          this->perceptron_table[index][i] = THRESHOLD;
        } else {
          this->perceptron_table[index][i]++;
        }
      } else {
        int32_t updateVal = this->perceptron_table[index][i] - 1;
        if (updateVal < -THRESHOLD) {
          this->perceptron_table[index][i] = -THRESHOLD;
        } else {
          this->perceptron_table[index][i]--;
        }
      }
    }
  }
  this->ghr.pop_front();
  this->ghr.push_back(resolveDir);
}

bool branchsim::perceptron_predictor(uint32_t pc) {
  uint32_t index = ((pc >> 2) & this->PHT_ADDR_MASK);
  uint32_t prediction = 0;
  // Calculate prediction based on selected perceptron and global history.
  // First add the bias, then all other weights.
  prediction += this->perceptron_table[index][0];
  for (uint32_t i = 1; i <= HIST_LEN; i++) {
    // If history bit is taken, add the weight to the prediction.
    // Else, subtract the weight.

    if (this->ghr.at(i-1).to_ulong() == 1) {
      prediction += this->perceptron_table[index][i];
    } else {
      prediction -= this->perceptron_table[index][i];
    }
  }
  // Update perceptron steps to absolute value of the prediction.

  this->perceptronSteps = std::abs(static_cast<long>(prediction));
  // If the prediction is negative, predict not taken. If it is positive,
  // predict taken. Assume zero is positive.
 
  return prediction >= 0;
}

bool branchsim::backward_propagation_predictor(void) {
  // TODO()
  return false;
}

void branchsim::update_backward_propagation_predictor(bool taken) {
  // TODO()
}

void branchsim::update_saturate_bimodal_predictor(uint32_t pc, bool taken) {
  // TODO()
}

bool branchsim::saturate_bimodal_predictor(uint32_t pc) {
  // TODO()
  return false;
}

