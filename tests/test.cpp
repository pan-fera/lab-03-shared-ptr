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

struct Test1 {
 public:
  int _x, _y;
  Test1(){
    _x = 0;
    _y = 0;
  }
  Test1(int x, int y){
    _x = x;
    _y = y;
  }
  ~Test1(){}
};

TEST(Struct, Constructor) {
  SharedPtr<Test1> ptr1(new Test1 (4, 3));
  SharedPtr<Test1> ptr2(ptr1);
  EXPECT_TRUE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);

  SharedPtr<Test1> ptr3(std::move(ptr1));
  EXPECT_FALSE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_TRUE(ptr3);
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr3.use_count(), 2);
}

TEST(Struct, Operator) {
  SharedPtr<Test1> ptr1(new Test1 (4, 3));
  SharedPtr<Test1> ptr2;
  ptr2 = ptr1;
  EXPECT_TRUE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);

  SharedPtr<Test1> ptr3;
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
  SharedPtr<Test1> ptr1(new Test1 (4, 3));
  SharedPtr<Test1> ptr2(new Test1 (5, -4));
  SharedPtr<Test1> ptr3(ptr1);

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
  SharedPtr<Test1> ptr1(new Test1 (4, 3));
  SharedPtr<Test1> ptr2 = ptr1;
  SharedPtr<Test1> ptr3(ptr1);

  ptr1.reset();
  EXPECT_FALSE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_TRUE(ptr3);
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr3.use_count(), 2);
}

TEST(StructMethods, ResetPtr) {
  Test1* _test1 = new Test1 (4, 3);
  Test1* _test2 = new Test1 (5, -4);
  SharedPtr<Test1> ptr1(_test1);
  SharedPtr<Test1> ptr2 = ptr1;
  SharedPtr<Test1> ptr3(ptr1);

  ptr1.reset(_test2);
  EXPECT_TRUE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_TRUE(ptr3);
  EXPECT_EQ(ptr1.use_count(), 1);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr3.use_count(), 2);
}

struct Test2 { Test2& operator= (Test2&&) = delete; };

TEST(MoveTest, AssignConstruct) {
  Test2* _test1 = new Test2;
  SharedPtr<Test2> ptr1(_test1);
  EXPECT_THROW(SharedPtr<Test2> ptr2(ptr1),
               std::runtime_error);
  EXPECT_THROW(SharedPtr<Test2> ptr3(std::move(ptr1)),
               std::runtime_error);

  SharedPtr<Test2> ptr4;
  SharedPtr<Test2> ptr5;
  EXPECT_THROW(ptr4 = ptr1,
               std::runtime_error);
  EXPECT_THROW(ptr5 = std::move(ptr1),
               std::runtime_error);

  EXPECT_EQ(std::is_move_assignable<SharedPtr<std::string>>::value,
            true);
  EXPECT_EQ(std::is_move_constructible<SharedPtr<std::string>>::value,
            true);
}
