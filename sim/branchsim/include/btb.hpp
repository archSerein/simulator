#ifndef __BTB_HPP__
#define __BTB_HPP__

#include <vector>
#include <cstdint>
const uint32_t BLOCK = 1;
const uint32_t ASSOCIATIVITY = 1;
const uint32_t SET = 16;

struct btb_entry {
  uint32_t tag;
  uint32_t target;
  bool valid;
};
struct btb_config {
  uint32_t block;
  uint32_t associativity;
  uint32_t set;
};

class btb {
  public:
    btb(void);
    btb(struct btb_config &config);
    void update_btb(uint32_t pc, uint32_t target);
    uint32_t get_target(uint32_t pc);
  private:
    uint32_t block;
    uint32_t associativity;
    uint32_t set;
    // valid tag target
    std::vector<std::vector<std::vector<struct btb_entry>>> btb_table;
};

#endif // !__BTB_HPP__
