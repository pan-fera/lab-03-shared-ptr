// Copyright 2020 Your Name <your_email>
#include <SharedPtr.hpp>
#include <gtest/gtest.h>

TEST(Example, EmptyTest) {
    EXPECT_TRUE(true);
}

TEST(Methods, EmptyConstructor) {
  SharedPtr<int> ptr1;
  EXPECT_FALSE(ptr1);
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_THROW(*ptr1, std::runtime_error);
  EXPECT_TRUE(ptr1.get()== nullptr);
}

TEST(Methods, PointerConstructor) {
  int val = 5;
  int *pointer = (int*) malloc(sizeof(int));
  *pointer=val;
  SharedPtr<int> ptr1 (pointer);
  EXPECT_TRUE(ptr1);
  EXPECT_EQ(ptr1.use_count(), 1);
  EXPECT_EQ(*ptr1, *pointer);
  EXPECT_TRUE(ptr1.get() == pointer);
}

TEST(Methods, CopyConstructor) {
  int val = 5;
  int *pointer = (int*) malloc(sizeof(int));
  *pointer=val;
  SharedPtr<int> ptr1(pointer);

  SharedPtr<int> ptr2(ptr1);
  EXPECT_TRUE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(*ptr1, *pointer);
  EXPECT_EQ(*ptr2, *pointer);
  EXPECT_TRUE(ptr1.get() == pointer);
  EXPECT_TRUE(ptr2.get() == pointer);
}

TEST(Methods, MoveConstructor) {
  int val = 5;
  int *pointer = (int*) malloc(sizeof(int));
  *pointer=val;
  SharedPtr<int> ptr1(pointer);

  SharedPtr<int> ptr2(std::move(ptr1));
  EXPECT_FALSE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_EQ(ptr2.use_count(), 1);
  EXPECT_THROW(*ptr1, std::runtime_error);
  EXPECT_EQ(*ptr2, *pointer);
  EXPECT_TRUE(ptr1.get()== nullptr);
  EXPECT_TRUE(ptr2.get() == pointer);
}