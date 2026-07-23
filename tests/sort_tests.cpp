#include <sort.hpp>

#include "sort_test_suite.hpp"

REGISTER_INTEGER_SORT_TESTS(BubbleSortTest, bubble_sort)

REGISTER_INTEGER_SORT_TESTS(InsertionSortTest, insertion_sort)

REGISTER_INTEGER_SORT_TESTS(CockTailSortTest, cocktail_sort)

REGISTER_INTEGER_SORT_TESTS(BucketSortTest, bucket_sort)

REGISTER_INTEGER_SORT_TESTS(CountingSortTest, counting_sort)

REGISTER_INTEGER_SORT_TESTS(MergeSortTest, merge_sort)

REGISTER_INTEGER_SORT_TESTS(PigeonHoleSortTest, pigeonhole_sort)

REGISTER_INTEGER_SORT_TESTS(BinaryTreeSortTest, binarytree_sort)