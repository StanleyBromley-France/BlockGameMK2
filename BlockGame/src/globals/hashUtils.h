#ifndef HASHUTILS_H
#define HASHUTILS_H

#include <algorithm>
#include <utility>

// Custom hash function for std::pair<int, int>
struct PairHash {
    // Combines a single value's hash with the current seed
    static void combine_hash(size_t& seed, int value) {
        std::hash<int> hasher;
        // Magic formula to mix hashes and reduce collisions:
        // 1. 0x9e3779b9 is a golden ratio prime
        // 2. Bit shifts (<< 6 and >> 2) spread bits around
        // 3. XOR ensures order of elements matters
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    // Generates hash for the entire pair
    size_t operator()(const std::pair<int, int>& p) const {
        size_t hash = 0;
        combine_hash(hash, p.first);  // Hash first element
        combine_hash(hash, p.second); // Combine with second
        return hash;
    }
};


// Custom hash function for std::tuple<int, int, int>
struct TupleHash {
    // Combines a single value's hash with the current seed
    static void combine_hash(size_t& seed, int value) {
        std::hash<int> hasher;
        // Magic formula to mix hashes and reduce collisions:
        // 1. 0x9e3779b9 is a golden ratio prime
        // 2. Bit shifts (<< 6 and >> 2) spread bits around
        // 3. XOR ensures order of elements matters
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    // Generates hash for the entire tuple
    size_t operator()(const std::tuple<int, int, int>& t) const {
        size_t hash = 0;
        combine_hash(hash, std::get<0>(t));  // Hash first element
        combine_hash(hash, std::get<1>(t));  // Combine with second
        combine_hash(hash, std::get<2>(t));  // Combine with third
        return hash;
    }
};


#endif