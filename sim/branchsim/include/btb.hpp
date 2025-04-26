#ifndef __BTB_HPP__
#define __BTB_HPP__

#include <vector>
constexpr uint32_t BTB_SIZE = 1024;

struct btb_entry {
  uint32_t tag;
  uint32_t target;
  bool valid;
};

class btb {
  public:
    btb();
    btb(uint32_t btb_size);
    void update_btb(uint32_t pc, uint32_t target);
    uint32_t get_target(uint32_t pc);
  private:
    uint32_t btb_size;
    // valid tag target
    std::vector<struct btb_entry> btb_table;
};

#endif // !__BTB_HPP__
