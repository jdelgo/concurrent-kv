#include "concurrent_kv/bench.hpp"
#include "concurrent_kv/global_lock_kv.hpp"
#include "concurrent_kv/shared_kv.hpp"
#include <iostream>

int main() {
    concurrent_kv::GlobalLockKV global_lock_store;
    concurrent_kv::demo(global_lock_store);

    std::cout << '\n';
    concurrent_kv::ShardedKV sharded_store(5);
    concurrent_kv::demo(sharded_store);

    return 0;
}
