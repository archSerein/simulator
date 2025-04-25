#include <vector>
#include "branchsim.hpp"


branchsim::branchsim() {
  saturat_counter = STRONGLY_NOT_TAKEN;
  ghr = 0;
  bhr = 0;
  pht_size = PHT_SIZE;
  // Initialize the pattern history table (PHT)
  pht.resize(PHT_SIZE, INIT_PHT_VALUE);
  // Initialize the bimodal predictor
  pht_bi_1.resize(PHT_SIZE, INIT_PHT_VALUE);
  pht_bi_2.resize(PHT_SIZE, INIT_PHT_VALUE);
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

void branchsim::update_saturate_bimodal_predictor(bool taken) {
  // TODO()
}

bool branchsim::saturate_bimodal_predictor(void) {
  // TODO()
  return false;
}

bool branchsim::saturate_global_history_predictor(uint32_t pc) {
  uint32_t index = (pc & this->PHT_ADDR_MASK) ^ this->ghr;
  saturat_status pht_value = this->pht[index];
  bool prediction = pht_value == STRONGLY_TAKEN ||
                    pht_value == WEAKLY_TAKEN;
  return prediction;
}

void branchsim::update_saturate_global_history_predictor(bool taken) {
  // TODO
  // how to update pht
}
