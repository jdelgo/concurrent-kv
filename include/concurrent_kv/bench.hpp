#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <sys/types.h>
#include <thread>
#include <vector>

namespace concurrent_kv {

struct BenchConfig {
    int threads = 8;
    int seconds = 3;
    uint64_t keys_range = 1'000'000;
    uint64_t keys_preloaded = 200'000;
    int read_pct = 95;
};

struct BenchResult {
    double elapsed = 0.0;
    uint64_t total_ops = 0;
    uint64_t total_gets = 0;
    uint64_t total_puts = 0;
    uint64_t ops_per_sec = 0.0;
};

template <typename Store> void demo(Store &store) {
    store.put(1, 111);

    store.put(2, 222);

    auto v1 = store.get(1);
    auto v2 = store.get(2);

    std::cout << "key=1 | val=" << (v1 ? *v1 : -1) << '\n';
    std::cout << "key=2 | val=" << (v2 ? *v2 : -1) << '\n';
}

template <typename Store>
BenchResult run_read_heavy_bench(Store &store, const BenchConfig &cfg) {
    using Clock = std::chrono::steady_clock;

    for (uint64_t i = 0; i < cfg.keys_preloaded; ++i) {
        store.put(i, i);
    }

    std::atomic<bool> start{false};
    std::atomic<bool> finish{false};

    struct ThreadStats {
        uint64_t ops = 0;
        uint64_t gets = 0;
        uint64_t puts = 0;
    };
    std::vector<ThreadStats> stats(cfg.threads);

    std::vector<std::jthread> workers;
    workers.reserve(cfg.threads);

    for (int t = 0; t < cfg.threads; ++t) {
        workers.emplace_back([&, t](std::stop_token) {
            std::mt19937_64 rng(0xC0FFEEULL + (uint64_t)t * 1337ULL);
            std::uniform_int_distribution<uint64_t> key_dist(0, cfg.keys_range -
                                                                    1);
            std::uniform_int_distribution<int> pct(1, 100);

            while (!start.load(std::memory_order_acquire)) {
            }

            ThreadStats local_stats{};
            while (!finish.load(std::memory_order_relaxed)) {
                const uint64_t key = key_dist(rng);
                const int r = pct(rng);

                if (r <= cfg.read_pct) {
                    (void)store.get(key);
                    local_stats.gets += 1;
                } else {
                    store.put(key, key ^ 0x9e3779b97f4a7c15ULL);
                    local_stats.puts += 1;
                }
                local_stats.ops += 1;
            }
            stats[t] = local_stats;
        });
    }

    const auto t0 = Clock::now();
    start.store(true, std::memory_order_release);
    std::this_thread::sleep_for(std::chrono::seconds(cfg.seconds));
    finish.store(true, std::memory_order_release);
    workers.clear();

    const auto t1 = Clock::now();

    BenchResult res;

    double elapsed = std::chrono::duration<double>(t1 - t0).count();

    for (const auto &s : stats) {
        res.total_ops += s.ops;
        res.total_gets += s.gets;
        res.total_puts += s.puts;
    }
    res.elapsed = elapsed;
    res.ops_per_sec = static_cast<double>(res.total_ops) / res.elapsed;
    return res;
}

} // namespace concurrent_kv
