#ifndef __DEFS_HPP__
#include <cstdio>
#include <mutex>

void      saturat_simulator(FILE *, struct SimConfig);
void      global_history_simulator(FILE *, struct SimConfig);
void      local_history_simulator(FILE *, struct SimConfig);
void      bimode_simulator(FILE *, struct SimConfig);
void      perceptron_simulator(FILE *, struct SimConfig);
void      bp_predictor_simulator(FILE *, struct SimConfig);

extern    std::mutex info;
#endif // __DEFS_HPP__
