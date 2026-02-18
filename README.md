# concurrent-kv
A concurrent in-memory key-value store in C++ built to to explore locking strategies and scalability tradeoffs.

## Overview
`concurrent-kv` is a small systems project focused on learning about:
 - Thread safety in data structures that are shared
 - Lock contention and how it affects throughput
 - Benchmarking within C++

The project begins with a `std::unordered_map` protected by a global mutex and will evolve toward sharding and other forms of synchronization strategies. The goal is to start with correctness and iterate toward more scalable and realistic designs.

## Current Implementation

### GlobalLockKV
- Single `std::unordered_map<uint64_t, uint64_t>`
- One `std::mutex`
- Thread-safe `put`, `get`, and `erase`

## Getting Started

### Build
`make`

### Run Demo
`make run`

### Run Benchmark
`make bench`
