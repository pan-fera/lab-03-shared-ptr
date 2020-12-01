// Copyright 2020 hacker
#include <SharedPtr.hpp>
#include <gtest/gtest.h>

TEST(Example, EmptyTest) {
    EXPECT_TRUE(true);
}

TEST(Constructor, EmptyConstructor) {
  SharedPtr<int> ptr1;
  EXPECT_FALSE(ptr1);
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_THROW(*ptr1, std::runtime_error);
  EXPECT_TRUE(ptr1.get()== nullptr);
}

TEST(Constructor, PointerConstructor) {
  int val = 5;
  int *pointer = new int (val);
  *pointer = val;
  SharedPtr<int> ptr1(pointer);
  EXPECT_TRUE(ptr1);
  EXPECT_EQ(ptr1.use_count(), 1);
  EXPECT_EQ(*ptr1, *pointer);
  EXPECT_TRUE(ptr1.get() == pointer);
}

TEST(Constructor, CopyConstructor) {
  int val = 5;
  int *pointer = new int (val);
  *pointer = val;
  SharedPtr<int> ptr1(pointer);
  SharedPtr<int> ptr2(ptr1);
  SharedPtr<int> ptr3(ptr1);

  EXPECT_TRUE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr1.use_count(), 3);
  EXPECT_EQ(ptr2.use_count(), 3);
  EXPECT_EQ(*ptr1, *pointer);
  EXPECT_EQ(*ptr2, *pointer);
  EXPECT_TRUE(ptr1.get() == pointer);
  EXPECT_TRUE(ptr2.get() == pointer);
}

TEST(Constructor, MoveConstructor) {
  int val = 5;
  int *pointer = new int (val);
  *pointer = val;
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

TEST(Operator, CopyOperator) {
  int val = 5;
  int *pointer = new int (val);
  *pointer = val;
  SharedPtr<int> ptr1(pointer);
  SharedPtr<int> ptr2;
  ptr2 = ptr1;

  EXPECT_TRUE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(*ptr1, *pointer);
  EXPECT_EQ(*ptr2, *pointer);
  EXPECT_TRUE(ptr1.get() == pointer);
  EXPECT_TRUE(ptr2.get() == pointer);

  SharedPtr<int> ptr3;
  ptr2 = ptr3;

  EXPECT_TRUE(ptr1);
  EXPECT_FALSE(ptr2);
  EXPECT_FALSE(ptr3);
  EXPECT_EQ(ptr1.use_count(), 1);
  EXPECT_EQ(ptr2.use_count(), 0);
  EXPECT_EQ(ptr3.use_count(), 0);
  EXPECT_EQ(*ptr1, *pointer);
  EXPECT_THROW(*ptr2, std::runtime_error);
  EXPECT_THROW(*ptr3, std::runtime_error);
  EXPECT_TRUE(ptr1.get() == pointer);
  EXPECT_TRUE(ptr2.get() == nullptr);
  EXPECT_TRUE(ptr3.get() == nullptr);

  EXPECT_THROW(ptr3 = ptr3, std::runtime_error);
}

TEST(Operator, MoveOperator) {
  int val = 5;
  int *pointer = new int (val);
  *pointer = val;
  SharedPtr<int> ptr1(pointer);
  SharedPtr<int> ptr2(ptr1);
  SharedPtr<int> ptr3;
  ptr3 = std::move(ptr1);

  EXPECT_FALSE(ptr1);
  EXPECT_TRUE(ptr3);
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_EQ(ptr3.use_count(), 2);
  EXPECT_THROW(*ptr1, std::runtime_error);
  EXPECT_EQ(*ptr3, *pointer);
  EXPECT_TRUE(ptr1.get()== nullptr);
  EXPECT_TRUE(ptr3.get() == pointer);
}

TEST(Methods, Reset) {
  SharedPtr<std::string> ptr1(new std::string{"Ptr1"});
  SharedPtr<std::string> ptr2(ptr1);
  ptr1.reset();
  EXPECT_EQ(ptr2.use_count(), 1);
  EXPECT_FALSE(ptr1);
}
TEST(Methods, ResetPtr) {
  SharedPtr<std::string> ptr1(new std::string{"Ptr1"});
  EXPECT_EQ(*ptr1, "Ptr1");
  EXPECT_EQ(ptr1.use_count(), 1);
  std::string* str = new std::string{"Ptr2"};
  ptr1.reset(str);
  EXPECT_EQ(*ptr1, "Ptr2");
  EXPECT_EQ(ptr1.use_count(), 1);
}
TEST(Methods, Swap) {
  SharedPtr<std::string> ptr1(new std::string{"Ptr1"});
  EXPECT_EQ(*ptr1, "Ptr1");
  EXPECT_EQ(ptr1.use_count(), 1);
  SharedPtr<std::string> ptr2(new std::string{"Ptr2"});
  SharedPtr<std::string> ptr3(ptr2);
  EXPECT_EQ(*ptr2, "Ptr2");
  EXPECT_EQ(ptr2.use_count(), 2);
  ptr1.swap(ptr2);
  EXPECT_EQ(*ptr1, "Ptr2");
  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(*ptr2, "Ptr1");
  EXPECT_EQ(ptr2.use_count(), 1);
}

struct Tests {
 public:
  int _x, _y;
  Tests(){
    _x = 0;
    _y = 0;
  }
  Tests(int x, int y){
    _x = x;
    _y = y;
  }
  ~Tests(){}
};

TEST(Struct, Constructor) {
  SharedPtr<Tests> ptr1(new Tests (4, 3));
  SharedPtr<Tests> ptr2(ptr1);
  EXPECT_TRUE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);

  SharedPtr<Tests> ptr3(std::move(ptr1));
  EXPECT_FALSE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_TRUE(ptr3);
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr3.use_count(), 2);
}

TEST(Struct, Operator) {
  SharedPtr<Tests> ptr1(new Tests (4, 3));
  SharedPtr<Tests> ptr2;
  ptr2 = ptr1;
  EXPECT_TRUE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);

  SharedPtr<Tests> ptr3;
  ptr3 = std::move(ptr1);
  EXPECT_FALSE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_TRUE(ptr3);
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr3.use_count(), 2);

  std::stringstream str1;
  EXPECT_THROW(str1 << ptr1->_x, std::runtime_error);
}

TEST(StructMethods, AppealOperator) {
  SharedPtr<Tests> ptr1(new Tests (4, 3));
  SharedPtr<Tests> ptr2(new Tests (5, -4));
  SharedPtr<Tests> ptr3(ptr1);

  std::stringstream str1;
  std::stringstream str2;
  std::stringstream str3;

  str1 << ptr1->_x << " " << ptr1->_y;
  str2 << ptr2->_x << " " << ptr2->_y;
  str3 << ptr3->_x << " " << ptr3->_y;

  std::string ref1 = "4 3";
  std::string ref2 = "5 -4";

  EXPECT_EQ(str1.str(), ref1);
  EXPECT_EQ(str2.str(), ref2);
  EXPECT_EQ(str3.str(), ref1);
}

TEST(StructMethods, Reset) {
  SharedPtr<Tests> ptr1(new Tests (4, 3));
  SharedPtr<Tests> ptr2 = ptr1;
  SharedPtr<Tests> ptr3(ptr1);

  ptr1.reset();
  EXPECT_FALSE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_TRUE(ptr3);
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr3.use_count(), 2);
}

TEST(StructMethods, ResetPtr) {
  Tests* _test1 = new Tests (4, 3);
  Tests* _test2 = new Tests (5, -4);
  SharedPtr<Tests> ptr1(_test1);
  SharedPtr<Tests> ptr2 = ptr1;
  SharedPtr<Tests> ptr3(ptr1);

  ptr1.reset(_test2);
  EXPECT_TRUE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_TRUE(ptr3);
  EXPECT_EQ(ptr1.use_count(), 1);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr3.use_count(), 2);
}
