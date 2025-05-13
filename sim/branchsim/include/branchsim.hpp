#ifndef __BRANCHSIM_HPP__
#define __BRANCHSIM_HPP__

#include <cstdint>
#include <vector>

const uint32_t PHT_SIZE = 1024;

enum saturat_status {
  STRONGLY_TAKEN = 3,
  WEAKLY_TAKEN = 2,
  WEAKLY_NOT_TAKEN = 1,
  STRONGLY_NOT_TAKEN = 0
};

class branchsim {
  public:
    branchsim(void);
    branchsim(uint32_t pht_size);
    void update_saturate_branch_predictor(bool);
    bool saturate_branch_predictor(void);
    void update_saturate_bimodal_predictor(bool);
    bool saturate_bimodal_predictor(uint32_t);
    void update_saturate_global_history_predictor(uint32_t, bool);
    bool saturate_global_history_predictor(uint32_t);
    void update_saturate_local_history_predictor(bool);
    bool saturate_local_history_predictor(uint32_t);
    void update_preceptron_predictor(bool);
    bool preceptron_predictor(void);
    void update_backward_propagation_predictor(bool);
    bool backward_propagation_predictor(void);
  private:
    // Two-digit saturation counter
    saturat_status saturat_counter;
    // global history register
    uint32_t ghr = 0;
    // local history register
    uint32_t bhr = 0;

    std::vector<saturat_status> pht;
    // bi-mode predictor
    std::vector<saturat_status> pht_bi_1;
    std::vector<saturat_status> pht_bi_2;

    uint32_t pht_size;
    uint32_t PHT_ADDR_MASK;
    uint32_t GHR_MASK;

    saturat_status next_saturate_state(saturat_status, bool);
};

#endif // !__BRANCHSIM_HPP__
