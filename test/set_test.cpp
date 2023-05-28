#include <gtest/gtest.h>

#include "set.h"

TEST(SET, set_create) {
    set_handle_t set = z_set_create();

    EXPECT_NE(set, nullptr);
}

TEST(SET, set_insert_contains) {
    set_handle_t set = z_set_create();

    EXPECT_NE(set, nullptr);

    EXPECT_EQ(z_set_contains(set, 1), 0);
    z_set_insert(set, 1);
    EXPECT_EQ(z_set_contains(set, 1), 1);

    EXPECT_EQ(z_set_contains(set, 1), 1);
    z_set_insert(set, 1);
    EXPECT_EQ(z_set_contains(set, 1), 1);

    EXPECT_EQ(z_set_contains(set, 2), 0);
    z_set_insert(set, 2);
    EXPECT_EQ(z_set_contains(set, 2), 1);

    EXPECT_EQ(z_set_contains(set, 3), 0);
    EXPECT_EQ(z_set_contains(set, 4), 0);
}

TEST(SET, set_insert_contains_erase) {
    set_handle_t set = z_set_create();

    EXPECT_NE(set, nullptr);

    EXPECT_EQ(z_set_contains(set, 1), 0);
    z_set_insert(set, 1);
    EXPECT_EQ(z_set_contains(set, 1), 1);
    z_set_erase(set, 1);
    EXPECT_EQ(z_set_contains(set, 1), 0);

    z_set_insert(set, 2);
    z_set_insert(set, 3);
    z_set_insert(set, 4);
    z_set_insert(set, 5);
    z_set_insert(set, 6);
    z_set_insert(set, 65536);

    int ret = 0;
    ret = z_set_contains(set, 65536);
    EXPECT_EQ(ret, 1);
    z_set_erase(set, 65536);
    ret = z_set_contains(set, 65536);
    EXPECT_NE(ret, 1);

    ret = z_set_contains(set, 2);
    EXPECT_EQ(ret, 1);
    z_set_erase(set, 2);
    EXPECT_NE(z_set_contains(set, 2), 1);
    ret = z_set_contains(set, 3);
    EXPECT_EQ(ret, 1);
    z_set_erase(set, 3);
    EXPECT_NE(z_set_contains(set, 3), 1);
    ret = z_set_contains(set, 4);
    EXPECT_EQ(ret, 1);
    z_set_erase(set, 4);
    EXPECT_NE(z_set_contains(set, 4), 1);
    ret = z_set_contains(set, 5);
    EXPECT_EQ(ret, 1);
    z_set_erase(set, 5);
    EXPECT_NE(z_set_contains(set, 5), 1);
    ret = z_set_contains(set, 6);
    EXPECT_EQ(ret, 1);
    z_set_erase(set, 6);
    EXPECT_NE(z_set_contains(set, 6), 1);
}