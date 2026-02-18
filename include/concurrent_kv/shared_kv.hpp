#pragma once

#include <cstdint>
#include <mutex>
#include <optional>
#include <unordered_map>
#include <vector>

namespace concurrent_kv {

class ShardedKV {
  public:
    void put(uint64_t key, uint64_t value) {
        uint64_t idx = get_shard_idx(key);
        std::scoped_lock lck{m_[idx]};
        shards_[idx][key] = value;
    }
    std::optional<uint64_t> get(uint64_t key) {
        uint64_t idx = get_shard_idx(key);
        std::scoped_lock lck{m_[idx]};
        auto it = shards_[idx].find(key);
        if (it == shards_[idx].end())
            return std::nullopt;
        return it->second;
    }
    bool erase(uint64_t key) {
        uint64_t idx = get_shard_idx(key);
        std::scoped_lock lck{m_[idx]};
        auto it = shards_[idx].find(key);
        if (it == shards_[idx].end())
            return false;
        shards_[idx].erase(it);
        return true;
    }

  private:
    ShardedKV(uint64_t sz) : shards_{sz}, m_{sz} {}
    std::vector<std::unordered_map<uint64_t, uint64_t>> shards_;
    std::vector<std::mutex> m_;
    uint64_t get_shard_idx(uint64_t key) {
        return std::hash<uint64_t>{}(key) % shards_.size();
    }
};
} // namespace concurrent_kv
