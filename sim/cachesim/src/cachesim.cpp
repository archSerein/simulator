#include <cassert>
#include <cmath>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <cstdint>
#include <string>
#include <iostream>
#include "cachesim.hpp"

void inst_cache_sim(FILE *fp, int block, int associativity, int block_size, std::string) {
  int hit = 0;
  int miss = 0;

  int offset_bits = 2 + std::log2(block_size);
  int index_bits = std::log2(block);
 
  std::vector<std::vector<bool>> valid_array(block, std::vector<bool>(associativity, false));
  std::vector<std::vector<uint32_t>> tag_array(block, std::vector<uint32_t>(associativity, 0));
 
  uint32_t pc;
  while (fread(&pc, sizeof(uint32_t), 1, fp) == 1) {
    uint32_t index = (pc >> offset_bits) & ((1 << index_bits) - 1);
    uint32_t tag = pc >> (index_bits + offset_bits);
 
    bool is_hit = false;
    int empty_way = -1;
 
    for (int way = 0; way < associativity; ++way) {
        if (valid_array[index][way] && tag_array[index][way] == tag) {
            is_hit = true;
            break;
        }
        if (!valid_array[index][way] && (empty_way == -1)) {
            empty_way = way;
        }
    }
    
    if (is_hit) {
        hit++;
    } else {
        miss++;
        if (pc >= 0x0f000000 && pc < 0x0f002000)
          continue;
        if (empty_way != -1) {
            valid_array[index][empty_way] = true;
            tag_array[index][empty_way] = tag;
        } else {
          for (int way = associativity-1; way > 0; way--) {
            valid_array[index][way] = valid_array[index][way-1];
            tag_array[index][way] = tag_array[index][way-1];
          }
          valid_array[index][0] = true;
          tag_array[index][0] = tag;
        }
    }
  }

  pclose(fp);

  std::lock_guard<std::mutex> lock(out_lock);
  std::cout << "block: " << block << ", associativity: " << associativity << ", block_size: " << block_size << "\t";
  std::cout << "hit: " << hit << ", miss: " << miss << ", hit rate: " << (double)hit / (hit + miss) << std::endl;
}

void data_cache_sim(FILE *fp, int block, int associativity, int block_size, std::string) {
  int hit = 0;
  int miss = 0;

  const int OFFSET = std::log2(block_size);
  const int INDEX = std::log2(block);
  const int TAG = 32 - INDEX - OFFSET;

  std::vector<std::vector<bool>>  valid_array(block, std::vector<bool>(associativity, false));
  std::vector<std::vector<uint32_t>>  tag_array(block, std::vector<uint32_t>(associativity, 0));

  uint32_t pc;
  while (fread(&pc, sizeof(uint32_t), 1, fp) == 1) {
    uint32_t index = (pc >> (OFFSET + 2)) & ((1 << INDEX) - 1);
    uint32_t tag = pc >> TAG;
    // uint32_t offset = (pc >> 2) & ((1 << OFFSET) - 1);

    bool is_hit = false;
    for(int way = 0; way < associativity; ++way) {
      if (valid_array[index][way] && tag_array[index][way] == tag) {
          is_hit = true;
          break;
      }
    }
    if (is_hit) {
      hit++;
    } else {
      miss++;
      // uncache addr
      // if (pc >= 0x0f000000 && pc < 0x0f002000)
      // continue;
      int empty_way = -1;
      for (int way = 0; way < associativity; ++way) {
        if (!valid_array[index][way] && empty_way == -1) {
          empty_way = way;
        }
      }
      if (empty_way != -1) {
        valid_array[index][empty_way] = true;
        tag_array[index][empty_way] = tag;
      } else {
        for (int way = associativity-1; way > 0; way--) {
          valid_array[index][way] = valid_array[index][way-1];
          tag_array[index][way] = tag_array[index][way-1];
        }
        valid_array[index][0] = true;
        tag_array[index][0] = tag;
      }
    }
  }
  pclose(fp);

  std::lock_guard<std::mutex> lock(out_lock);
  std::cout << "block: " << block << ", associativity: " << associativity << ", block_size: " << block_size << "\t";
  std::cout << "hit: " << hit << ", miss: " << miss << ", hit rate: " << (double)hit / (hit + miss) << std::endl;
}
