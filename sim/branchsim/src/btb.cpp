#include <cmath>
#include <vector>
#include "btb.hpp"
#include "debug.hpp"

btb::btb(void) {
  this->block = BLOCK;
  this->associativity = ASSOCIATIVITY;
  this->set = SET;
  // Initialize the branch target buffer (BTB)
  this->btb_table.resize(SET, std::vector<std::vector<struct btb_entry>>(ASSOCIATIVITY,
                          std::vector<struct btb_entry>(BLOCK)));
  Log("block: %d, associativity: %d, set: %d", this->block, this->associativity, this->set);
}

btb::btb(struct btb_config &config) {
  this->block = config.block;
  this->associativity = config.associativity;
  this->set = config.set;
  // Initialize the branch target buffer (BTB)
  this->btb_table.resize(config.set, std::vector<std::vector<struct btb_entry>>(config.associativity,
                          std::vector<struct btb_entry>(config.block)));
  Log("block: %d, associativity: %d, set: %d", this->block, this->associativity, this->set);
}

uint32_t btb::get_target(uint32_t pc) {
  uint32_t offset = pc & ((1 << static_cast<uint32_t>(std::log2(this->block))) - 1);
  uint32_t index = (pc >> 2) & ((1 << static_cast<uint32_t>(std::log2(this->set))) - 1);
  uint32_t tag = pc >> (static_cast<uint32_t>(std::log2(this->block + this->set)) + 2);
  for (auto &entry : this->btb_table[index]) {
    if (entry[offset].tag == tag && entry[offset].valid) {
      return entry[offset].target; // Return the target address if found
    }
  }
  // If not found, return the next instruction address
  // Assuming the next instruction is pc + 4
  return pc+4;
}

void btb::update_btb(uint32_t pc, uint32_t target) {
  uint32_t offset = pc & ((1 << static_cast<uint32_t>(std::log2(this->block))) - 1);
  uint32_t index = (pc >> 2) & ((1 << static_cast<uint32_t>(std::log2(this->set))) - 1);
  uint32_t tag = pc >> (static_cast<uint32_t>(std::log2(this->block + this->set)) + 2);
  auto &set = this->btb_table.at(index);
  std::vector<struct btb_entry> *empty_entry = nullptr;
  for (auto &it : set) {
    bool full = false;
    for (auto &entry : it) {
      if (entry.tag == tag && entry.valid) {
        // Entry already exists, update it
        entry.target = target; // Update the target address
        entry.valid = true; // Mark it as valid
        entry.tag = tag; // Update the tag
        return;
      }
      full |= entry.valid;
    }
    if (!full) {
      empty_entry = &it; // Store the empty entry
    }
  }
  if (empty_entry != nullptr) {
    // If an empty entry is found, use it
    (*empty_entry)[offset].tag = tag;
    (*empty_entry)[offset].target = target;
    (*empty_entry)[offset].valid = true; // Mark it as valid
  } else {
    // If no empty entry is found, replace the first one
    set.erase(set.begin());
    std::vector<struct btb_entry> new_entry;
    new_entry.resize(this->block);
    new_entry[offset] = {tag, target, true}; // Create a new entry
    set.push_back(new_entry); // Add the new entry to the set
  }
}

void btb::info(void) {
  Log("block: %ld, associativity: %ld, set: %ld", this->btb_table.at(0).at(0).size(),
      this->btb_table.at(0).size(), this->btb_table.size());
}
