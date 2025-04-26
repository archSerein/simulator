#include "btb.hpp"

btb::btb() {
  btb_size = BTB_SIZE;
  // Initialize the branch target buffer (BTB)
  btb_table.resize(BTB_SIZE);
}

btb::btb(uint32_t btb_size) {
  this->btb_size = btb_size;
  // Initialize the branch target buffer (BTB)
  btb_table.resize(btb_size);
}

uint32_t btb::get_target(uint32_t pc) {
  uint32_t index = (pc >> 2) & (this->btb_size - 1);
  uint32_t tag = pc >> (std::log(btb_size) + 2);
  struct btb_entry entry = this->btb_table[index];
  if (entry.valid && entry.tag == tag) {
    return entry.target; // Return the target address if found
  } else {
    // If not found, return the next instruction address
    // Assuming the next instruction is pc + 4
   return pc+4;
  }
}

void btb::update_btb(uint32_t pc, uint32_t target) {
  uint32_t index = (pc >> 2) & (this->btb_size - 1);
  uint32_t tag = pc >> (std::log(btb_size) + 2);
  struct btb_entry entry;
  entry.tag = tag;
  entry.target = target;
  entry.valid = true;
  this->btb_table[index] = entry; // Update the BTB entry
}
