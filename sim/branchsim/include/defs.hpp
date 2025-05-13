#ifndef __DEFS_HPP__
#include <cstdio>

void      saturat_simulator(FILE *, struct SimConfig);
void      global_history_simulator(FILE *, struct SimConfig);
void      local_history_simulator(FILE *, struct SimConfig);
void      bimodal_simulator(FILE *, struct SimConfig);
void      perceptron_simulator(FILE *, struct SimConfig);
void      bp_predictor_simulator(FILE *, struct SimConfig);

#endif // __DEFS_HPP__
