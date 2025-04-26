#include "branchsim.hpp"


branchsim::branchsim() {
  saturat_counter = STRONGLY_NOT_TAKEN;
  ghr = 0;
  bhr = 0;
  pht_size = PHT_SIZE;
  // Initialize the pattern history table (PHT)
  pht.resize(PHT_SIZE, STRONGLY_NOT_TAKEN);
  // Initialize the bimodal predictor
  pht_bi_1.resize(PHT_SIZE, STRONGLY_NOT_TAKEN);
  pht_bi_2.resize(PHT_SIZE, STRONGLY_NOT_TAKEN);
  // Initialize the branch target buffer (BTB)
}


branchsim::branchsim(uint32_t pht_size) {
  saturat_counter = STRONGLY_NOT_TAKEN;
  ghr = 0;
  bhr = 0;
  this->pht_size = pht_size;
  // Initialize the pattern history table (PHT)
  pht.resize(pht_size, STRONGLY_NOT_TAKEN);
  // Initialize the bimodal predictor
  pht_bi_1.resize(pht_size, STRONGLY_NOT_TAKEN);
  pht_bi_2.resize(pht_size, STRONGLY_NOT_TAKEN);
  // Initialize the branch target buffer (BTB)
}

bool branchsim::saturate_branch_predictor(bool taken) {
  bool prediction = branchsim::saturat_counter == STRONGLY_TAKEN ||
                    branchsim::saturat_counter == WEAKLY_TAKEN;
  return prediction;
}

void branchsim::update_saturate_branch_predictor(bool taken) {
  if (taken) {
    if (branchsim::saturat_counter < STRONGLY_TAKEN) {
      branchsim::saturat_counter++;
    }
  } else {
    if (branchsim::saturat_counter > STRONGLY_NOT_TAKEN) {
      branchsim::saturat_counter--;
    }
  }
}

bool branchsim::saturate_global_history_predictor(uint32_t pc) {
  uint32_t index = ((pc >> 2) & this->PHT_ADDR_MASK) ^ this->ghr;
  saturat_status pht_value = this->pht[index];
  bool prediction = pht_value == STRONGLY_TAKEN ||
                    pht_value == WEAKLY_TAKEN;
  return prediction;
}

void branchsim::update_saturate_global_history_predictor(uint32_t pc, bool taken) {
  uint32_t index = ((pc >> 2) & this->PHT_ADDR_MASK) ^ this->ghr;
  saturat_status pht_value = this->pht[index];
  if (taken) {
    if (pht_value < STRONGLY_TAKEN) {
      this->pht[index]++;
    }
  } else {
    if (pht_value > STRONGLY_NOT_TAKEN) {
      this->pht[index]--;
    }
  }
  // Update the global history register (GHR)
  this->ghr = (this->ghr << 1) | (taken ? 1 : 0);
  this->ghr &= this->GHR_MASK;
}

void branchsim::update_saturate_local_history_predictor(bool taken) {
  // TODO()
}

bool branchsim::saturate_local_history_predictor(uint32_t pc) {
  // TODO()
  return false;
}

void branchsim::update_preceptron_predictor(bool taken) {
  // TODO()
}

bool branchsim::preceptron_predictor(void) {
  // TODO()
  return false;
}

bool branchsim::backward_propagation_predictor(void) {
  // TODO()
  return false;
}

bool branchsim::update_backward_propagation_predictor(bool taken) {
  // TODO()
  return false;
}

void branchsim::update_saturate_bimodal_predictor(bool taken) {
  // TODO()
}

bool branchsim::saturate_bimodal_predictor(void) {
  // TODO()
  return false;
}

