#pragma once

#include <gtest/gtest.h>

#include "utils.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

#define REGISTER_INTEGER_SORT_TESTS(SUITE_NAME, SORT_FUNCTION)                 \
                                                                              \
    TEST(SUITE_NAME, HandlesEmptyInput) {                                      \
        std::vector<int> values;                                               \
        SORT_FUNCTION(values);                                                 \
        EXPECT_TRUE(values.empty());                                           \
    }                                                                          \
                                                                              \
    TEST(SUITE_NAME, HandlesSingleElement) {                                   \
        std::vector<int> values{42};                                           \
        SORT_FUNCTION(values);                                                 \
        EXPECT_EQ(values, std::vector<int>{42});                               \
    }                                                                          \
                                                                              \
    TEST(SUITE_NAME, HandlesAlreadySortedInput) {                              \
        std::vector<int> values{1, 2, 3, 4, 5};                                \
        const auto expected = values;                                          \
        SORT_FUNCTION(values);                                                 \
        EXPECT_EQ(values, expected);                                           \
    }                                                                          \
                                                                              \
    TEST(SUITE_NAME, HandlesReverseInput) {                                    \
        std::vector<int> values{5, 4, 3, 2, 1};                                \
        SORT_FUNCTION(values);                                                 \
        EXPECT_EQ(values, (std::vector<int>{1, 2, 3, 4, 5}));                  \
    }                                                                          \
                                                                              \
    TEST(SUITE_NAME, HandlesDuplicateElements) {                               \
        std::vector<int> values{4, 2, 4, 1, 2, 1};                             \
        SORT_FUNCTION(values);                                                 \
        EXPECT_EQ(values, (std::vector<int>{1, 1, 2, 2, 4, 4}));              \
    }                                                                          \
                                                                              \
    TEST(SUITE_NAME, MatchesStandardSortForRandomInputs) {                     \
        constexpr int start_n = 50;                                            \
        constexpr int increment_n = 50;                                        \
        constexpr int max_n = 5000;                                            \
                                                                              \
        std::vector<int> actual;                                               \
                                                                              \
        for (int n = start_n; n <= max_n; n += increment_n) {                  \
            generate_random_n_vec_int(actual, n);                              \
                                                                              \
            auto expected = actual;                                            \
            std::ranges::sort(expected);                                       \
                                                                              \
            SORT_FUNCTION(actual);                                             \
                                                                              \
            EXPECT_EQ(actual, expected)                                        \
                << "Input size: " << n;                                        \
        }                                                                      \
    }