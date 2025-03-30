#include "List.hpp"
#include "unit_test_framework.hpp"

using namespace std;


TEST(test_list_default_ctor) {
    List<int> empty_list;
    ASSERT_TRUE(empty_list.empty());
}

TEST(test_forward_traversal) {
    List<int> list;
    for (int i = 0; i < 5; ++i) {
        list.push_back(i);
    }
    int j = 0;
    for (List<int>::Iterator it = list.begin(); it != list.end(); ++it, ++j) {
        ASSERT_EQUAL(*it, j);
    }
    ASSERT_EQUAL(j, list.size());
}

TEST(test_backward_traversal) {
    List<int> list;
    for (int i = 0; i < 5; ++i) {
        list.push_back(i);
    }
    int j = list.size();
    List<int>::Iterator it = list.end();
    do {
        --it;
        --j;
        ASSERT_EQUAL(*it, j);
    }
    while(it != list.begin());
    ASSERT_EQUAL(j, 0);
}

TEST_MAIN()
