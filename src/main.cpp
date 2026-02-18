#include "concurrent_kv/bench.hpp"
#include "concurrent_kv/global_lock_kv.hpp"

int main() {
    concurrent_kv::GlobalLockKV global_lock_store;
    concurrent_kv::demo(global_lock_store);
    return 0;
}
