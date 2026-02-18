#pragma once

#include <iostream>

namespace concurrent_kv {

template <typename Store> void demo(Store &store) {
    store.put(1, 111);

    store.put(2, 222);

    auto v1 = store.get(1);
    auto v2 = store.get(2);

    std::cout << "key=1 | val=" << (v1 ? *v1 : -1) << '\n';
    std::cout << "key=2 | val=" << (v2 ? *v2 : -1) << '\n';
}

} // namespace concurrent_kv
