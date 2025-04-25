#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <cstring> // for strerror
#include <vector>
#include "cachesim.hpp"

#ifndef CACHETYPE
  #define CACHETYPE "inst"
#endif

static const char *cache = CACHETYPE;

int offset_bits = 0;
std::mutex  out_lock;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <trace_file>\n", argv[0]);
        return 1;
    }

    const char *trace_file = argv[1];

    std::vector<int> blocks = {4, 8, 16, 64};
    std::vector<int> associativities = {1, 2, 4, 8, 16};
    std::vector<int> block_size = {1, 2,4, 8};

    std::vector<std::thread> threads;

    char command[256];
    snprintf(command, sizeof(command), "bzcat %s", trace_file);

    for (int b : blocks) {
        for (int a : associativities) {
            for (int c : block_size) {
                FILE* fp = popen(command, "r");
                if (fp == nullptr) {
                    printf("Failed to open file %s: %s\n", trace_file, strerror(errno));
                    return 1;
                }
                std::string label = std::to_string(b) + "-" + std::to_string(a);
                if (strcmp(cache, "inst") == 0) {
                    threads.emplace_back(inst_cache_sim, fp, b, a, c, label);
                } else if (strcmp(cache, "data") == 0) {
                    threads.emplace_back(data_cache_sim, fp, b, a, c, label);
                } else {
                    printf("Invalid cache type: %s\n", cache);
                    pclose(fp);
                    return 1;
                }
            }
        }
    }

    // 等待所有线程完成
    for (auto &t : threads) {
        t.join();
    }

    return 0;
}
