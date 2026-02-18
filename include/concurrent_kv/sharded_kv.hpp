#pragma once

#include <cstdint>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace concurrent_kv {

class ShardedKV {
  public:
    ShardedKV(uint64_t shard_cnt, uint64_t expected_items)
        : shards_{shard_cnt == 0
                      ? throw std::invalid_argument("shard_cnt must be >= 1")
                      : shard_cnt},
          m_{shard_cnt} {
        for (auto &v : shards_)
            v.reserve((expected_items / shard_cnt) * 2);
    }
    void put(uint64_t key, uint64_t value) {
        uint64_t idx = get_shard_idx(key);
        std::scoped_lock lck{m_[idx].m};
        shards_[idx][key] = value;
    }
    std::optional<uint64_t> get(uint64_t key) const {
        uint64_t idx = get_shard_idx(key);
        std::scoped_lock lck{m_[idx].m};
        auto it = shards_[idx].find(key);
        if (it == shards_[idx].end())
            return std::nullopt;
        return it->second;
    }
    bool erase(uint64_t key) {
        uint64_t idx = get_shard_idx(key);
        std::scoped_lock lck{m_[idx].m};
        auto it = shards_[idx].find(key);
        if (it == shards_[idx].end())
            return false;
        shards_[idx].erase(it);
        return true;
    }

  private:
    struct alignas(64) PaddedMutex {
        std::mutex m;
    };
    std::vector<std::unordered_map<uint64_t, uint64_t>> shards_;
    mutable std::vector<PaddedMutex> m_;
    uint64_t get_shard_idx(uint64_t key) const { return key % shards_.size(); }
};
} // namespace concurrent_kv
