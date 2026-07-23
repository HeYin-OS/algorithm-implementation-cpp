#pragma once

#include "tree.hpp"

#include <algorithm>
#include <utility>
#include <vector>

// ---------------------------------------------------------------
// Stable Sorting Algorithm
// ---------------------------------------------------------------

template<typename T>
void bubble_sort(std::vector<T>& nums) {
    const int n = nums.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // If find(j) < top(i)
            if (nums[j] < nums[i]) std::swap(nums[i], nums[j]);
        }
    }
}

template<typename T>
void insertion_sort(std::vector<T>& nums) {
    const int n = nums.size();
    T curr;
    // Start from second one (i = 1)
    for (int i = 1; i < n; ++i) {
        // curr = element to be inserted
        curr = nums[i];
        // Compare from front one (i - 1)
        int j = i - 1;
        for (; j >= 0; --j) {
            // If current one < compare one, move comp to later position
            if (curr < nums[j]) nums[j + 1] = nums[j];
            else {
                break;
            }
        }
        // j means the one that smaller than curr, so we place curr at j + 1.
        nums[j + 1] = curr;
    }
}

template<typename T>
void cocktail_sort(std::vector<T>& nums) {
    // index: [left, right] means sequence that hasn't been sorted.
    int left = 0, right = nums.size() - 1;
    while (left < right) {
        for (int i = left; i < right; ++i) {
            if (nums[i] > nums[i + 1]) std::swap(nums[i], nums[i + 1]);
        }
        // Biggest one moved to right end, right boundary--.
        right--;
        for (int i = right; i > left; --i) {
            if (nums[i] < nums[i - 1]) std::swap(nums[i], nums[i - 1]);
        }
        // Smallest one moved to left end, left boundary--.
        left++;
    }
}

template<typename T>
class Bucket{
public:
    void insert(T val) {
        isEmpty = false;
        data.push_back(val);
    }

    void sort_safe() {
        if (!isEmpty) insertion_sort(data);
    }

    const std::vector<T>& get_data() const {return data;};

private:
    bool isEmpty = true;
    std::vector<T> data;
};

template<typename T>
void bucket_sort(std::vector<T>& nums) {
    constexpr int bucket_size = 10;
    if (nums.empty()) return;
    // Calculate bucket size
    auto [min_itr, max_itr] = std::minmax_element(nums.begin(), nums.end());
    auto min = *min_itr, max = *max_itr;
    T bucket_n = (max - min) / bucket_size + 1;
    auto buckets = std::vector<Bucket<T>>(bucket_n);
    // Insert number into corresponding bucket
    for (auto num: nums) {
        T idx = (num - min) / bucket_size;
        buckets[idx].insert(num);
    }
    // Sort each bucket
    std::for_each(buckets.begin(), buckets.end(), [](Bucket<T>& b){
        b.sort_safe();
    });
    // Overwrite old vector
    nums.resize(0);
    for (auto& buk: buckets) {
        auto vec_ref = buk.get_data();
        for (auto num: vec_ref) nums.push_back(num);
    }
}

template<typename T>
void counting_sort(std::vector<T>& nums) {
    if (nums.empty()) return;
    // Calculate count vector size
    auto [min_itr, max_itr] = std::minmax_element(nums.begin(), nums.end());
    auto min = *min_itr, max = *max_itr;
    T n = max - min + 1;
    auto counts = std::vector<T>(n, 0);
    // Count number
    for (auto num: nums) {
        counts[num - min]++;
    }
    // Overwrite old vector
    nums.resize(0);
    for (int num = 0; num < counts.size(); ++num) {
        int repeat = counts[num];
        while (repeat--) nums.push_back(num + min);
    }
}

template<typename T>
void merge_sort(std::vector<T>& nums) {
    if (nums.size() < 2) return;
    int interval = 1;
    while (interval < nums.size()) {
        // Double the interval at each loop
        for (int i = 0; i < nums.size(); i += interval * 2) {
            // Use temp vector to store merged data
            auto temp_vec = std::vector<T>();
            // Merge two ordered sequences
            int p1 = i, p2 = i + interval, p1_end = p1 + interval, p2_end = p2 + interval;
            while (p1 < p1_end && p2 < p2_end && p1 < nums.size() && p2 < nums.size()) {
                if (nums[p1] < nums[p2]) {
                    temp_vec.push_back(nums[p1]);
                    p1++;
                } else {
                    temp_vec.push_back(nums[p2]);
                    p2++;
                }
            }
            // Residual process
            while (p1 != p1_end && p1 < nums.size()) {
                temp_vec.push_back(nums[p1]);
                p1++;
            }
            while (p2 != p2_end && p2 < nums.size()) {
                temp_vec.push_back(nums[p2]);
                p2++;
            }
            // Write data back to original vector
            for (int j = 0; j < temp_vec.size(); ++j) {
                nums[i + j] = temp_vec[j];
            }
        }
        interval *= 2;
    }
}

template<typename T>
void pigeonhole_sort(std::vector<T>& nums) {
    if (nums.size() < 2) return;
    // Calculate hole size
    auto [min_it, max_it] = std::minmax_element(nums.begin(), nums.end());
    T min = *min_it, max = *max_it;
    T hole_size = max - min + 1;
    auto holes = std::vector<std::vector<T>>(hole_size);
    // Push value into right hole
    for (T num: nums) {
        holes[num - min].push_back(num);
    }
    // Write back to nums using data in holes
    nums.resize(0);
    for (auto& hole: holes) {
        for (T num: hole) nums.push_back(num);
    }
}

template<typename T>
void binarytree_sort(std::vector<T>& nums) {
    auto avl_tree = AVL_Tree<T>();
    for (auto num: nums) {
        avl_tree.insert(num);
    }
    nums = std::move(avl_tree.get_sorted_data());
}

template<typename T>
void radix_sort(std::vector<T>& nums) {
    if (nums.size() < 2) return;
    auto max = *std::max_element(nums.begin(), nums.end());
    auto min = *std::min_element(nums.begin(), nums.end());
    // Calculate loop times
    int digits = 0;
    {
        auto diff = max - min;
        while(diff != 0) {
            digits++;
            diff /= 10;
        }
    }
    // divider: 1 -> 10 -> 100 -> ...
    T divider = 1;
    while(digits--) {
        // Analyze number in this digit (0 1 ... 9) and put them in right idx of radices
        auto radices = std::vector<std::vector<T>>(10, std::vector<T>());
        for (auto num: nums) {
            auto idx = ((num - min) / divider) % 10;
            radices[idx].push_back(num);
        }
        nums.resize(0);
        for (auto& vec: radices) {
            for (auto num: vec) {
                nums.push_back(num);
            }
        }
        divider *= 10;
    }
}

template<typename T>
void gnome_sort(std::vector<T>& nums) {
    if (nums.size() < 2) return;
    for (int idx = 1; idx < nums.size(); ++idx) {
        auto curr_idx = idx;
        // Swap until: lower idx < higher idx
        while (curr_idx > 0 && nums[curr_idx] < nums[curr_idx - 1]) {
            std::swap(nums[curr_idx], nums[curr_idx - 1]);
            curr_idx--;
        }
    }
}

template<typename T>
void tim_sort(std::vector<T>& nums) {
    
}

// ---------------------------------------------------------------
// Unstable Sorting Algorithm
// ---------------------------------------------------------------

template<typename T>
void selection_sort(std::vector<T>& nums) {
    
}

template<typename T>
void shell_sort(std::vector<T>& nums) {
    
}

template<typename T>
void heap_sort(std::vector<T>& nums) {
    
}

template<typename T>
void quick_sort(std::vector<T>& nums) {
    
}

template<typename T>
void three_way_quick_sort(std::vector<T>& nums) {
    
}

template<typename T>
void intro_sort(std::vector<T>& nums) {
    
}

template<typename T>
void pdq_sort(std::vector<T>& nums) {
    
}