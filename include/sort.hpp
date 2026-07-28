#pragma once

#include "tree.hpp"

#include <algorithm>
#include <random>
#include <utility>
#include <vcruntime_typeinfo.h>
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
    if (nums.size() < 2) return;
    for (int i = 0; i < nums.size(); ++i) {
        T prefer_idx = i;
        // Find the preferring idx in latter indices
        for (int j = i + 1; j < nums.size(); ++j) {
            if (nums[j] < nums[prefer_idx]) prefer_idx = j;
        }
        // Swap the value
        if (prefer_idx != i) std::swap(nums[prefer_idx], nums[i]);
    }
}

// Decrease increment sorting
template<typename T>
void shell_sort(std::vector<T>& nums) {
    const int n = nums.size();
    if (n < 2) return;
    auto step = n / 2;
    // Decrease step towards 1
    while (step > 0) {
        // Initial index for each handled group
        for (int idx_group_initial = 0; idx_group_initial < step; ++idx_group_initial) {
            // Insertion sort with step, start from second one
            for (int idx = idx_group_initial + step; idx < n; idx += step) {
                // Use temp to store minimal element
                auto temp = nums[idx];
                // Traverse from the one before idx
                int j = idx - step;
                // Until j == 0
                while (j >= 0) {
                    // If temp should take this position
                    if (temp < nums[j]) {
                        // Move idx=j to idx=j+step
                        nums[j + step] = nums[j];
                    } else break;
                    // Move backward
                    j -= step;
                }
                // The element at idx=j means that
                // the one bigger than temp or idx=-1(no one bigger than it)
                // so that use idx=j+step
                nums[j + step] = temp;
            }
        }
        step /= 2;
    }
}

//                                   0
//                                 /   \
// Exmaple Heap Structure:        1     2   
//                               / \   / \
//                              3   4 5   6
int inline _heap_l_child_idx(int root_idx, int offset) {return 2 * (root_idx - offset) + offset + 1;} // 0 -> 1 ; 1 -> 3
int inline _heap_r_child_idx(int root_idx, int offset) {return 2 * (root_idx - offset) + offset + 2;} // 0 -> 2 ; 1 -> 4
int inline _heap_father_idx(int child_idx, int offset) {return (child_idx - offset - 1) / 2 + offset;} // 1 or 2 -> 0 ; 3 or 4 -> 1

template<typename T>
void inline _make_heap_impl(std::vector<T>& nums, int start_idx, int end_idx) {
    const int length = end_idx - start_idx;
    int heap_size = 1;
    while (heap_size < length) {
        auto insert_idx = heap_size + start_idx;
        while (true) {
            auto father_idx = _heap_father_idx(insert_idx, start_idx);
            // Swap root and child
            if (nums[insert_idx] > nums[father_idx]) std::swap(nums[insert_idx], nums[father_idx]);
            else break;
            // End of heap (strict-order procedure)
            if (father_idx == start_idx) break;
            // Continue search along father node
            insert_idx = father_idx;
        }
        heap_size++;
    }
}

template<typename T>
void inline _heap_sort_impl(std::vector<T>& nums, int start_idx, int end_idx) {
    const int length = end_idx - start_idx;
    int heap_size = end_idx - start_idx;
    while (heap_size > 0) {
        // Move top element to tail
        auto tail_idx = heap_size + start_idx - 1;
        std::swap(nums[tail_idx], nums[start_idx]);
        heap_size--;
        // Maintain heap from idx=start_idx
        auto idx = start_idx;
        auto heap_end_idx = start_idx + heap_size;
        while (idx < heap_end_idx) {
            auto l_child_idx = _heap_l_child_idx(idx, start_idx);
            auto r_child_idx = _heap_r_child_idx(idx, start_idx);
            // Left child index outside heap means that
            // we do not need to compare two childs
            // since left one always comes smaller idx.
            if (l_child_idx >= heap_end_idx) break;
            // Left child is in range but right child is out of range.
            else if (r_child_idx >= heap_end_idx) {
                if (nums[idx] > nums[l_child_idx]) break;
                std::swap(nums[idx], nums[l_child_idx]);
                idx = l_child_idx;
            // Two childs is in range.
            } else {
                auto swap_child_idx = nums[l_child_idx] > nums[r_child_idx] ? l_child_idx : r_child_idx;
                if (nums[idx] > nums[swap_child_idx]) break;
                std::swap(nums[idx], nums[swap_child_idx]);
                idx = swap_child_idx;
            }
        }
    }
}

template<typename T>
void heap_sort(std::vector<T>& nums) {
    const int n = nums.size();
    if (n < 2) return;
    _make_heap_impl(nums, 0, n);
    _heap_sort_impl(nums, 0, n);
}

inline auto&& _get_random_engine() {
    static std::random_device rd;
    static std::mt19937 g(rd());
    return g;
}

inline int _random_pick_idx(int start_idx, int end_idx) {
    return std::uniform_int_distribution<int>(start_idx, end_idx)(_get_random_engine());
}

template<typename T>
inline int _quick_middle_pick_idx(std::vector<T>& nums, int start_idx, int end_idx) {
    auto mid_idx = (start_idx + end_idx) / 2;
    if ((nums[start_idx] < nums[mid_idx]) != (nums[start_idx] < nums[end_idx - 1])) {
        return start_idx;
    }

    if ((nums[mid_idx] < nums[start_idx]) != (nums[mid_idx] < nums[end_idx - 1])) {
        return mid_idx;
    }

    return end_idx - 1;
}

template<typename T>
int _pivot_partition_2_way(std::vector<T>& nums, int start_idx, int end_idx, bool use_random_pick) {
    // Randomly pick pivot
    if (use_random_pick) std::swap(nums[_quick_middle_pick_idx(nums, start_idx, end_idx)], nums[end_idx - 1]);
    // Use left to mark initial idx of unhandled area,
    // right to mark initial idx of larger-than-pivot area
    auto left = start_idx, right = end_idx - 1;
    while (left < right) {
        if (nums[left] > nums[end_idx - 1]) {
            if (left == right) break;
            std::swap(nums[left], nums[--right]);
        } else {
            left++;
        }
    }
    std::swap(nums[right], nums[end_idx - 1]);
    return right;
}

template<typename T>
void _quick_sort_impl(std::vector<T>& nums, int start_idx, int end_idx) {
    if (end_idx - start_idx < 2) return;
    // Perform pivot-based division (Result: left indices <= pivot idx <= right indices)
    auto pivot = _pivot_partition_2_way(nums, start_idx, end_idx, true);
    // Recursively sort left area
    _quick_sort_impl(nums, start_idx, pivot);
    // Recursively sort right area
    _quick_sort_impl(nums, pivot + 1, end_idx);
}

template<typename T>
void quick_sort(std::vector<T>& nums) {
    const int n = nums.size();
    if (n < 2) return;
    _quick_sort_impl(nums, 0, n);
}

template<typename T>
std::pair<int, int> _pivot_partition_3_way(std::vector<T>& nums, int start_idx, int end_idx, bool use_random_pick) {
    // Randomly pick pivot
    if (use_random_pick) std::swap(nums[_quick_middle_pick_idx(nums, start_idx, end_idx)], nums[end_idx - 1]);
    // [start_idx, less_end) -> less area
    // [less_end, current_start) -> equal area
    // [greater_start, end_idx) -> greater area
    auto less_end = start_idx, current_start = start_idx, greater_start = end_idx - 1;
    while (current_start < greater_start) {
        if (nums[current_start] < nums[end_idx - 1]) {
            std::swap(nums[less_end++], nums[current_start++]);
        } else if (nums[current_start] == nums[end_idx - 1]) {
            current_start++;
        } else {
            std::swap(nums[current_start], nums[--greater_start]);
        }
    }
    std::swap(nums[greater_start], nums[end_idx - 1]);
    return {less_end, greater_start};
}

template<typename T>
void _three_way_quick_sort_impl(std::vector<T>& nums, int start_idx, int end_idx) {
    if (end_idx - start_idx < 2) return;
    // Perform pivot-based division (Result: left indices < pivot indices (equal) < right indices)
    auto [pivot_start, pivot_end] = _pivot_partition_3_way(nums, start_idx, end_idx, true);
    // Recursively sort left area
    _three_way_quick_sort_impl(nums, start_idx, pivot_start);
    // Recursively sort right area
    _three_way_quick_sort_impl(nums, pivot_end, end_idx);
}

template<typename T>
void three_way_quick_sort(std::vector<T>& nums) {
    const int n = nums.size();
    if (n < 2) return;
    _three_way_quick_sort_impl(nums, 0, n);
}

template<typename T>
void intro_sort(std::vector<T>& nums) {
    
}

template<typename T>
void pdq_sort(std::vector<T>& nums) {
    
}