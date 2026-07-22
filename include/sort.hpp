#include <algorithm>
#include <vector>

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
    auto in_it = nums.begin();
    for (auto& buk: buckets) {
        auto vec_ref = buk.get_data();
        for (auto num: vec_ref) {
            *in_it = num;
            in_it++;
        }
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
    auto in_it = nums.begin();
    while (in_it != nums.end()) {
        for (int num = 0; num < counts.size(); ++num) {
            int repeat = counts[num];
            while (repeat--) {
                *in_it = num + min;
                in_it++;
            }
        }
    }
}

template<typename T>
void merge_sort(std::vector<T>& nums) {
    if (nums.size() < 2) return;
    int interval = 1;
    while (interval < nums.size()) {
        for (int i = 0; i < nums.size(); i += interval * 2) {
            auto temp_vec = std::vector<T>();
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
            while (p1 != p1_end && p1 < nums.size()) {
                temp_vec.push_back(nums[p1]);
                p1++;
            }
            while (p2 != p2_end && p2 < nums.size()) {
                temp_vec.push_back(nums[p2]);
                p2++;
            }
            for (int j = 0; j < temp_vec.size(); ++j) {
                nums[i + j] = temp_vec[j];
            }
        }
        interval *= 2;
    }
}