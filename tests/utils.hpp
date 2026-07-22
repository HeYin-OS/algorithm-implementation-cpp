#include <cstddef>
#include <random>
#include <chrono>
#include <vector>

void generate_random_n_vec_int(std::vector<int>& vec, int n){
    vec.resize(n);
    // Use time to srand random engine
    static auto gen = std::mt19937(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));
    // Expansion factor for numeric max in uniform distribution
    constexpr double factor = 1.3f;
    using n_type = decltype(n);
    // Use uniform distribution
    auto distrib = std::uniform_int_distribution<n_type>(0, static_cast<n_type>(n * factor));
    for (auto& num: vec) {
        num = distrib(gen);
    }
} 