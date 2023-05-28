#include <gtest/gtest.h>

#include "list.h"

int expected = 1;

int compare(void *a, void *b) {
    int *x = (int *)a;
    int *y = (int *)b;

    return *x - *y;
}

struct COMPLEX_DATA {
    int a;
    float b;
    double c;

    char* str;
};

TEST(LIST, list_create) {
  list_handle_t list = z_list_create(sizeof(int));

  EXPECT_NE(list, nullptr);
  EXPECT_EQ(z_list_size(list), 0);

  z_list_destroy(list);
}

TEST(LIST, lsit_push_back) {
    list_handle_t list = z_list_create(sizeof(int));

    int a;

    a = 1;
    z_list_push_back(list, &a);
    EXPECT_EQ(z_list_size(list), 1);

    a = 2;
    z_list_push_back(list, &a);
    EXPECT_EQ(z_list_size(list), 2);

    a = 3;
    z_list_push_back(list, &a);
    EXPECT_EQ(z_list_size(list), 3);

    EXPECT_EQ(z_list_size(list), 3);

    auto print = [](void *data, int* /*stop*/) {
        int *a = (int *)data;
        EXPECT_EQ(*a, expected++);
    };

    z_list_iterate(list, print);

    z_list_destroy(list);
}

TEST(LIST, z_list_erase) {
    list_handle_t list = z_list_create(sizeof(int));

    int a;
    int b;

    a = 1;
    z_list_push_back(list, &a);

    a = 2;
    z_list_push_back(list, &a);

    a = 3;
    z_list_push_back(list, &a);

    EXPECT_EQ(z_list_size(list), 3);
    b = 2;
    z_list_erase(list, &b, compare);
    EXPECT_EQ(z_list_size(list), 2);

    z_list_iterate(list, [](void *data, int* /*stop*/) {
        int *a = (int *)data;
        EXPECT_NE(*a, 2);
    });

    z_list_destroy(list);
}

TEST(LIST, z_list_erase_1) {
    list_handle_t list = z_list_create(sizeof(int));

    int a;
    int b;

    a = 1;
    z_list_push_back(list, &a);

    EXPECT_EQ(z_list_size(list), 1);
    b = 1;
    z_list_erase(list, &b, compare);
    EXPECT_EQ(z_list_size(list), 0);

    // invoke erase on empty list
    b = 1;
    z_list_erase(list, &b, compare);
    EXPECT_EQ(z_list_size(list), 0);

    z_list_iterate(list, [](void *data, int* /*stop*/) {
        int *a = (int *)data;
        EXPECT_NE(*a, 2);
    });

    z_list_destroy(list);
}

TEST(LIST, z_list_push_back_complex) {
    list_handle_t list = z_list_create(sizeof(struct COMPLEX_DATA));
    EXPECT_NE(list, nullptr);

    struct COMPLEX_DATA* val;
    val = (struct COMPLEX_DATA*)malloc(sizeof(struct COMPLEX_DATA));
    
    val->a = 1;
    val->b = 2.0;
    val->c = 3.0;
    val->str = const_cast<char *>("hello");

    z_list_push_back(list, val);
    EXPECT_EQ(z_list_size(list), 1);
    free(val);

    z_list_iterate(list, [](void *data, int* /*stop*/) {
        struct COMPLEX_DATA* val = (struct COMPLEX_DATA*)data;
        EXPECT_EQ(val->a, 1);
        EXPECT_EQ(val->b, 2.0);
        EXPECT_EQ(val->c, 3.0);
        EXPECT_STREQ(val->str, "hello");
    });

    z_list_destroy(list);
}
