// Copyright 2019 Weicheng Pei

// #define ABC_USE_SMART_POINTER_

#include "abc/forward_list.h"

#include <algorithm>
#include <chrono>  // NOLINT
#include <forward_list>

#include "abc/data/copyable.h"
#include "gtest/gtest.h"

class TestForwardList : public ::testing::Test {
 protected:
  // helper class
  using Kitten = abc::data::Copyable;
  // common data
  std::forward_list<int> std_list_of_id{ 4, 3, 2, 1 };
  std::forward_list<Kitten> std_list_of_kitten;
  abc::forward_list<Kitten> abc_list_of_kitten;
};
TEST_F(TestForwardList, Empty) {
  EXPECT_EQ(abc_list_of_kitten.empty(), std_list_of_kitten.empty());
}
TEST_F(TestForwardList, EmplaceFront) {
  for (const auto& i : std_list_of_id) {
    std_list_of_kitten.emplace_front(i);
    abc_list_of_kitten.emplace_front(i);
  }
  EXPECT_EQ(abc_list_of_kitten.empty(), std_list_of_kitten.empty());
}
TEST_F(TestForwardList, Front) {
  for (const auto& i : std_list_of_id) {
    std_list_of_kitten.emplace_front(i);
    abc_list_of_kitten.emplace_front(i);
    EXPECT_EQ(abc_list_of_kitten.front(), std_list_of_kitten.front());
  }
}
TEST_F(TestForwardList, PopFront) {
  for (const auto& i : std_list_of_id) {
    std_list_of_kitten.emplace_front(i);
    abc_list_of_kitten.emplace_front(i);
  }
  while (!std_list_of_kitten.empty()) {
    std_list_of_kitten.pop_front();
    abc_list_of_kitten.pop_front();
    EXPECT_EQ(abc_list_of_kitten.empty(), std_list_of_kitten.empty());
  }
}
TEST_F(TestForwardList, Iterator) {
  for (const auto& i : std_list_of_id) {
    abc_list_of_kitten.emplace_front(i);
    std_list_of_kitten.emplace_front(i);
  }
  // construct
  auto iter = abc_list_of_kitten.begin();
  EXPECT_EQ(*iter, *std_list_of_kitten.begin());
  // copy
  auto iter_copy = iter;
  EXPECT_EQ(iter, iter_copy);
  EXPECT_EQ(*iter, *iter_copy);
  // move
  auto iter_move = std::move(iter_copy);
  EXPECT_EQ(iter, iter_move);
  EXPECT_EQ(*iter, *iter_move);
  // look for an object in the list
  iter = std::find(
      abc_list_of_kitten.begin(),
      abc_list_of_kitten.end(),
      Kitten(2));
  EXPECT_NE(iter, abc_list_of_kitten.end());
  EXPECT_EQ(*iter, Kitten(2));
  // look for an object not in the list
  iter = std::find(
      abc_list_of_kitten.begin(),
      abc_list_of_kitten.end(),
      Kitten(-2));
  EXPECT_EQ(iter, abc_list_of_kitten.end());
}
TEST_F(TestForwardList, RangeFor) {
  for (const auto& i : std_list_of_id) {
    abc_list_of_kitten.emplace_front(i);
    std_list_of_kitten.emplace_front(i);
  }
  auto iter = std_list_of_kitten.begin();
  for (auto& x : abc_list_of_kitten) {
    EXPECT_EQ(x, *iter++);
  }
}
TEST_F(TestForwardList, EmplaceAfter) {
  for (const auto& i : std_list_of_id) {
    std_list_of_kitten.emplace_front(i);
    abc_list_of_kitten.emplace_front(i);
  }
  const auto& target = Kitten(2);
  auto std_iter = std::find(
      std_list_of_kitten.begin(),
      std_list_of_kitten.end(),
      target);
  std_iter = std_list_of_kitten.emplace_after(std_iter, 0);
  auto abc_iter = std::find(
      abc_list_of_kitten.begin(),
      abc_list_of_kitten.end(),
      target);
  abc_iter = abc_list_of_kitten.emplace_after(abc_iter, 0);
  EXPECT_EQ(*abc_iter, *std_iter);
  while (!abc_list_of_kitten.empty()) {
    EXPECT_EQ(abc_list_of_kitten.front(), std_list_of_kitten.front());
    abc_list_of_kitten.pop_front();
    std_list_of_kitten.pop_front();
  }
}
TEST_F(TestForwardList, Equal) {
  auto new_list_of_kitten = decltype(abc_list_of_kitten)();
  for (const auto& i : std_list_of_id) {
    abc_list_of_kitten.emplace_front(i);
    new_list_of_kitten.emplace_front(i);
  }
  EXPECT_TRUE(abc_list_of_kitten == abc_list_of_kitten);
  EXPECT_TRUE(new_list_of_kitten == abc_list_of_kitten);
  EXPECT_FALSE(abc_list_of_kitten != abc_list_of_kitten);
  EXPECT_FALSE(new_list_of_kitten != abc_list_of_kitten);
}
TEST_F(TestForwardList, Copy) {
  for (const auto& i : std_list_of_id) {
    abc_list_of_kitten.emplace_front(i);
  }
  // test copy constructor:
  auto new_list_of_kitten = abc_list_of_kitten;
  EXPECT_EQ(new_list_of_kitten, abc_list_of_kitten);
  // test copy assignment operator:
  new_list_of_kitten = abc_list_of_kitten;
  EXPECT_EQ(new_list_of_kitten, abc_list_of_kitten);
  // test self assignment:
  new_list_of_kitten = new_list_of_kitten;
  EXPECT_EQ(new_list_of_kitten, abc_list_of_kitten);
}
TEST_F(TestForwardList, Move) {
  for (const auto& i : std_list_of_id) {
    abc_list_of_kitten.emplace_front(i);
  }
  // test move constructor:
  auto copied_list_of_kitten = abc_list_of_kitten;
  auto moved_list_of_kitten = abc::move(abc_list_of_kitten);
  EXPECT_EQ(moved_list_of_kitten, copied_list_of_kitten);
  // test move assignment operator:
  abc_list_of_kitten = abc::move(copied_list_of_kitten);
  EXPECT_EQ(moved_list_of_kitten, abc_list_of_kitten);
  // test self assignment:
  moved_list_of_kitten = abc::move(moved_list_of_kitten);
  EXPECT_EQ(moved_list_of_kitten, abc_list_of_kitten);
}
TEST_F(TestForwardList, Performance) {
  using clock = std::chrono::high_resolution_clock;
  auto ticks = [](auto& list) {
    auto start = clock::now();
    for (int i = 0; i != 1000000; ++i) {
      list.emplace_front(i);
    }
    list.clear();
    std::chrono::duration<double> duration = clock::now() - start;
    return duration.count();
  };
  auto t_std = ticks(std_list_of_kitten);
  auto t_abc = ticks(abc_list_of_kitten);
  EXPECT_LT(t_abc, t_std);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
