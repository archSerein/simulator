#ifndef CACHESIM_HPP_
#define CACHESIM_HPP_

#include <mutex>

extern  std::mutex  out_lock;
void    inst_cache_sim(FILE*, int, int, int, std::string);
void    data_cache_sim(FILE*, int, int, int, std::string);

#endif
