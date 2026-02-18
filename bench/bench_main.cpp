#include "concurrent_kv/bench.hpp"
#include "concurrent_kv/global_lock_kv.hpp"
#include "concurrent_kv/sharded_kv.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>

int main() {
    concurrent_kv::BenchConfig cfg;
    constexpr int runs = 5;

    std::vector<double> global_lock_ops(runs);
    std::vector<double> sharded_ops(runs);

    std::cout << "Global Lock Store" << '\n';
    for (int i = 0; i < runs; ++i) {
        concurrent_kv::GlobalLockKV global_lock_store{cfg.keys_range};
        auto global_lock_res =
            concurrent_kv::run_read_heavy_bench(global_lock_store, cfg);
        std::cout << "Total operations= " << global_lock_res.total_ops << ' '
                  << "Total gets= " << global_lock_res.total_gets << ' '
                  << "Total puts= " << global_lock_res.total_puts << ' '
                  << "Ops per sec= " << global_lock_res.ops_per_sec << '\n';
        global_lock_ops[i] = global_lock_res.ops_per_sec;
    }

    std::cout << '\n';

    std::cout << "Sharded Store" << '\n';
    for (int i = 0; i < runs; ++i) {
        concurrent_kv::ShardedKV sharded_store{16, cfg.keys_range};
        auto sharded_res =
            concurrent_kv::run_read_heavy_bench(sharded_store, cfg);
        std::cout << "Total operations= " << sharded_res.total_ops << ' '
                  << "Total gets= " << sharded_res.total_gets << ' '
                  << "Total puts= " << sharded_res.total_puts << ' '
                  << "Ops per sec= " << sharded_res.ops_per_sec << '\n';
        sharded_ops[i] = sharded_res.ops_per_sec;
    }

    auto avg = [](const std::vector<double> &v) {
        double sum = std::accumulate(v.begin(), v.end(), 0.0);
        return sum / v.size();
    };

    auto median = [](std::vector<double> v) {
        std::sort(v.begin(), v.end());
        return v[v.size() / 2];
    };

    std::cout << '\n';
    std::cout << "Average ops per sec for a global lock= "
              << avg(global_lock_ops) << '\n';
    std::cout << "Average ops per sec for sharding= " << avg(sharded_ops)
              << '\n';
    std::cout << "Percent difference between sharding and global lock= "
              << avg(sharded_ops) / avg(global_lock_ops) * 100 << "%\n\n";

    std::cout << "Median ops per sec for a global lock= "
              << median(global_lock_ops) << '\n';
    std::cout << "Median ops per sec for sharding= " << median(sharded_ops)
              << '\n';
    std::cout << "Percent difference between sharding and global locks= "
              << median(sharded_ops) / median(global_lock_ops) * 100 << "%\n";
    return 0;
}
