// Copyright 2019 Minghao Yang and Weicheng Pei
#include "abc/vector.h"

#include <algorithm>
#include <chrono>  // NOLINT
#include <vector>

#include "abc/data/copyable.h"
#include "gtest/gtest.h"

class TestVector : public ::testing::Test {
 protected:
  // helper class
  using Kitten = abc::data::Copyable;
  // common data
  std::vector<int> std_vector_of_id{ 1, 2, 3, 4 };
  std::vector<Kitten> std_vector_of_kitten;
  abc::vector<Kitten> abc_vector_of_kitten;
  // common operations
  void ExpectEqual() const {
    EXPECT_EQ(abc_vector_of_kitten.empty(),
              std_vector_of_kitten.empty());
    EXPECT_EQ(abc_vector_of_kitten.size(),
              std_vector_of_kitten.size());
    EXPECT_EQ(abc_vector_of_kitten.capacity(),
              std_vector_of_kitten.capacity());
    for (int i = 0; i < abc_vector_of_kitten.size(); i++) {
      EXPECT_EQ(abc_vector_of_kitten[i],
                std_vector_of_kitten[i]) << i;
    }
  }
};
TEST_F(TestVector, ConstructorDefault) {
  // `TestVector`'s members will be re-constructed before each `TEST_F`.
  ExpectEqual();
}
TEST_F(TestVector, ConstructorWithSize) {
  auto size = 73;
  abc_vector_of_kitten = abc::vector<Kitten>(size);
  std_vector_of_kitten = std::vector<Kitten>(size);
  ExpectEqual();
}
TEST_F(TestVector, ConstructorWithSizeAndValue) {
  auto size = 73;
  auto value = Kitten(size);
  abc_vector_of_kitten = abc::vector<Kitten>(size, value);
  std_vector_of_kitten = std::vector<Kitten>(size, value);
  ExpectEqual();
}
TEST_F(TestVector, ConstructorWithInitializerList) {
  auto list = { Kitten{1}, Kitten{2}, Kitten{3} };
  abc_vector_of_kitten = abc::vector<Kitten>(list);
  std_vector_of_kitten = std::vector<Kitten>(list);
  ExpectEqual();
  abc_vector_of_kitten = abc::vector<Kitten>{ Kitten{1}, Kitten{2}, Kitten{3} };
  std_vector_of_kitten = std::vector<Kitten>{ Kitten{1}, Kitten{2}, Kitten{3} };
  ExpectEqual();
}
TEST_F(TestVector, EmplaceBack) {
  for (auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    abc_vector_of_kitten.emplace_back(i);
    ExpectEqual();
  }
}
TEST_F(TestVector, PushBack) {
  for (auto& i : std_vector_of_id) {
    std_vector_of_kitten.push_back(Kitten(i));
    abc_vector_of_kitten.push_back(Kitten(i));
    ExpectEqual();
  }
}
TEST_F(TestVector, FrontAndBack) {
  for (const auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    abc_vector_of_kitten.emplace_back(i);
    EXPECT_EQ(abc_vector_of_kitten.front(),
              std_vector_of_kitten.front());
    EXPECT_EQ(abc_vector_of_kitten.back(),
              std_vector_of_kitten.back());
  }
}
TEST_F(TestVector, Resize) {
  for (int i = 0; i != 37; ++i) {
    std_vector_of_kitten.emplace_back(i);
    abc_vector_of_kitten.emplace_back(i);
  }
  std_vector_of_kitten.resize(73, std_vector_of_kitten.back());
  abc_vector_of_kitten.resize(73, abc_vector_of_kitten.back());
  ExpectEqual();
  while (std_vector_of_kitten.size() < std_vector_of_kitten.capacity()) {
    std_vector_of_kitten.emplace_back();
    abc_vector_of_kitten.emplace_back();
  }
  ExpectEqual();
  std_vector_of_kitten.resize(37);
  abc_vector_of_kitten.resize(37);
  ExpectEqual();
  std_vector_of_kitten.resize(1);
  abc_vector_of_kitten.resize(1);
  ExpectEqual();
  std_vector_of_kitten.resize(0);
  abc_vector_of_kitten.resize(0);
  ExpectEqual();
}
TEST_F(TestVector, PopBack) {
  for (const auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    abc_vector_of_kitten.emplace_back(i);
  }
  while (!std_vector_of_kitten.empty()) {
    EXPECT_EQ(abc_vector_of_kitten.back(),
              std_vector_of_kitten.back());
    EXPECT_EQ(abc_vector_of_kitten.front(),
              std_vector_of_kitten.front());
    std_vector_of_kitten.pop_back();
    abc_vector_of_kitten.pop_back();
  }
}
TEST_F(TestVector, At) {
  int j = 0;
  for (const auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    abc_vector_of_kitten.emplace_back(i);
    EXPECT_EQ(abc_vector_of_kitten.at(j),
              std_vector_of_kitten.at(j));
    j++;
  }
}
TEST_F(TestVector, BeginAndEnd) {
  for (const auto& i : std_vector_of_id) {
    abc_vector_of_kitten.emplace_back(i);
    std_vector_of_kitten.emplace_back(i);
  }
  auto iter_abc = abc_vector_of_kitten.begin();
  auto iter_std = std_vector_of_kitten.begin();
  while (iter_abc != abc_vector_of_kitten.end()) {
    EXPECT_EQ(*iter_abc, *iter_std);
    EXPECT_EQ(iter_abc->Id(), iter_std->Id());
    ++iter_abc;
    ++iter_std;
  }
}
TEST_F(TestVector, RangeFor) {
  for (const auto& i : std_vector_of_id) {
    abc_vector_of_kitten.emplace_back(i);
    std_vector_of_kitten.emplace_back(i);
  }
  auto iter = std_vector_of_kitten.begin();
  for (auto& x : abc_vector_of_kitten) {
    EXPECT_EQ(x, *iter++);
  }
}
TEST_F(TestVector, Iterator) {
  for (const auto& i : std_vector_of_id) {
    abc_vector_of_kitten.emplace_back(i);
  }
  // look for an object in the vector
  auto iter = std::find(
      abc_vector_of_kitten.begin(),
      abc_vector_of_kitten.end(),
      Kitten(2));
  EXPECT_NE(iter, abc_vector_of_kitten.end());
  EXPECT_EQ(*iter, Kitten(2));
  // look for an object not in the vector
  iter = std::find(
      abc_vector_of_kitten.begin(),
      abc_vector_of_kitten.end(),
      Kitten(-2));
  EXPECT_EQ(iter, abc_vector_of_kitten.end());
}
TEST_F(TestVector, Equal) {
  auto new_vector_of_kitten = decltype(abc_vector_of_kitten)();
  for (const auto& i : std_vector_of_id) {
    abc_vector_of_kitten.emplace_back(i);
    new_vector_of_kitten.emplace_back(i);
  }
  EXPECT_TRUE(abc_vector_of_kitten == abc_vector_of_kitten);
  EXPECT_TRUE(new_vector_of_kitten == abc_vector_of_kitten);
  EXPECT_FALSE(abc_vector_of_kitten != abc_vector_of_kitten);
  EXPECT_FALSE(new_vector_of_kitten != abc_vector_of_kitten);
}
TEST_F(TestVector, Swap) {
  auto list_a = {1, 2, 3, 4};
  auto list_b = {5, 6};
  auto a = abc::vector<int>(list_a);
  auto b = abc::vector<int>(list_b);
  auto size_a = a.size();
  auto size_b = b.size();
  auto end_of_a = a.back();
  auto end_of_b = b.back();
  a.swap(b);
  EXPECT_EQ(a.size(), size_b);
  EXPECT_EQ(a.capacity(), size_b);
  EXPECT_EQ(a.back(), end_of_b);
  EXPECT_EQ(b.size(), size_a);
  EXPECT_EQ(b.capacity(), size_a);
  EXPECT_EQ(b.back(), end_of_a);
}
TEST_F(TestVector, Performance) {
  using clock = std::chrono::high_resolution_clock;
  auto ticks = [](auto& vector) {
    auto start = clock::now();
    for (int i = 0; i != 1000000; ++i) {
      vector.emplace_back(i);
    }
    vector.clear();
    std::chrono::duration<double> duration = clock::now() - start;
    return duration.count();
  };
  auto t_std = ticks(std_vector_of_kitten);
  auto t_abc = ticks(abc_vector_of_kitten);
  EXPECT_LT(t_abc, t_std);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
