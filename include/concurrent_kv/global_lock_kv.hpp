#pragma once

#include <cstdint>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace concurrent_kv {

class GlobalLockKV {

  public:
    void put(uint64_t key, uint64_t value) {
        std::scoped_lock lck{m_};
        map_[key] = value;
    }
    std::optional<uint64_t> get(uint64_t key) {
        std::scoped_lock lck{m_};
        auto it = map_.find(key);
        if (it == map_.end())
            return std::nullopt;
        return it->second;
    }
    bool erase(uint64_t key) {
        std::scoped_lock lck{m_};
        if (!map_.count(key))
            return false;
        map_.erase(key);
        return true;
    }

  private:
    std::unordered_map<uint64_t, uint64_t> map_;
    std::mutex m_;
};

} // namespace concurrent_kv
