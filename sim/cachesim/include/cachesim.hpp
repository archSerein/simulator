#ifndef CACHESIM_HPP_
#define CACHESIM_HPP_

#include <mutex>

extern  std::mutex  out_lock;
void    inst_cache_sim(FILE*, int, int, int, std::string);
void    data_cache_sim(FILE*, int, int, int, std::string);

constexpr uint32_t FLASH_LATENCY = 1000;
constexpr uint32_t SDRAM_LATENCY = 20;
constexpr uint32_t SRAM_LATENCY  = 1;
constexpr uint32_t PSRAM_LATENCY = 400;
#endif
