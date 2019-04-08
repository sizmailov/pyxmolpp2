#include <gtest/gtest.h>

#include "xmol/selection/ContainerSelection.h"

using ::testing::Test;
using namespace xmol::selection;

namespace {
struct int_with_parent {
  int_with_parent(int value, Container<int_with_parent>& container)
      : value(value), ptr(&container) {}
  int value;
  Container<int_with_parent>* ptr;
  bool is_now_deleted = false;
  bool is_deleted() const { return is_now_deleted; }
  void set_deleted() { is_now_deleted = true; }
  const Container<int_with_parent>* parent() const { return ptr; };
  Container<int_with_parent>* parent() { return ptr; };
};
bool operator==(const int_with_parent& lhs,
    const int_with_parent& rhs) noexcept {
  return lhs.value == rhs.value && lhs.ptr == rhs.ptr;
}
}

class ConSelTests : public Test {
public:
  using V = int_with_parent;
  using C = Container<int_with_parent>;
  using S = Selection<int_with_parent>;
  using cS = Selection<const int_with_parent>;
};

template <typename T>
using is_int_with_parent = typename std::enable_if<
    std::is_same<typename std::remove_const<T>::type, int_with_parent>::value>::type;

namespace xmol {
namespace selection {
template <>
bool SelectionPointerComparator<int_with_parent>::
operator()(const int_with_parent* lhs, const int_with_parent* rhs) const {
  return std::make_tuple(lhs->parent(), lhs->value) <
         std::make_tuple(rhs->parent(), rhs->value);
}
}
}


TEST_F(ConSelTests, invalid_selection_throws_on_use) {

  S s;
  {
    C c;
    c.emplace(1, c);
    s = c.all();
    ASSERT_TRUE(s.is_valid());
  }
  ASSERT_FALSE(s.is_valid());
  C c2;
  auto value = int_with_parent(1, c2);

  EXPECT_THROW(s[0], xmol::selection::dead_selection_access<int_with_parent>);
  EXPECT_THROW(s.count(value), xmol::selection::dead_selection_access<int_with_parent>);
  EXPECT_THROW(s.begin(), xmol::selection::dead_selection_access<int_with_parent>);
  EXPECT_THROW(s.end(), xmol::selection::dead_selection_access<int_with_parent>);
  EXPECT_NO_THROW(s.empty());
  EXPECT_NO_THROW(s.size());
}

TEST_F(ConSelTests, set_operations_and_comparisons) {

  C c;
  c.emplace(1, c);
  c.emplace(2, c);
  c.emplace(3, c);

  C c2;
  c2.emplace(1, c2);
  c2.emplace(2, c2);
  c2.emplace(3, c2);
  c2.emplace(4, c2);

  S s = c.all();
  cS s2 = c2.all();

  ASSERT_EQ(s.size(), 3);
  ASSERT_EQ(s2.size(), 4);

  EXPECT_EQ(3, (s + s).size());
  EXPECT_EQ(7, (s + s2).size());
  EXPECT_EQ(7, (s2 + s).size());
  EXPECT_EQ(4, (s2 + s2).size());

  EXPECT_EQ(0, (s - s).size());
  EXPECT_EQ(3, (s - s2).size());
  EXPECT_EQ(4, (s2 - s).size());
  EXPECT_EQ(0, (s2 - s2).size());

  EXPECT_EQ(3, (s * s).size());
  EXPECT_EQ(0, (s * s2).size());
  EXPECT_EQ(0, (s2 * s).size());
  EXPECT_EQ(4, (s2 * s2).size());

  EXPECT_TRUE(s == s);
  EXPECT_TRUE(s2 == s2);
  EXPECT_FALSE(s2 == s);
  EXPECT_FALSE(s == s2);

  EXPECT_FALSE(s != s);
  EXPECT_FALSE(s2 != s2);
  EXPECT_TRUE(s2 != s);
  EXPECT_TRUE(s != s2);
}

TEST_F(ConSelTests, selection_indexing) {

  C c;
  c.emplace(1, c);
  c.emplace(2, c);
  c.emplace(3, c);

  C c2;
  c2.emplace(1, c2);
  c2.emplace(2, c2);
  c2.emplace(3, c2);
  c2.emplace(4, c2);

  S s = c.all();
  cS s2 = c2.all();

  ASSERT_EQ(s.size(), 3);
  ASSERT_EQ(s2.size(), 4);

  EXPECT_NO_THROW(s[-1]);
  EXPECT_NO_THROW(s[-2]);
  EXPECT_NO_THROW(s[-3]);
  EXPECT_THROW(s[-4], xmol::selection::selection_out_of_range<int_with_parent>);
  EXPECT_NO_THROW(s[0]);
  EXPECT_NO_THROW(s[1]);
  EXPECT_NO_THROW(s[2]);
  EXPECT_THROW(s[3], xmol::selection::selection_out_of_range<int_with_parent>);

  EXPECT_NO_THROW(s2[-1]);
  EXPECT_NO_THROW(s2[-2]);
  EXPECT_NO_THROW(s2[-3]);
  EXPECT_NO_THROW(s2[-4]);
  EXPECT_THROW(s2[-5], xmol::selection::selection_out_of_range<int_with_parent>);
  EXPECT_NO_THROW(s2[0]);
  EXPECT_NO_THROW(s2[1]);
  EXPECT_NO_THROW(s2[2]);
  EXPECT_NO_THROW(s2[3]);
  EXPECT_THROW(s2[4], xmol::selection::selection_out_of_range<int_with_parent>);
}


TEST_F(ConSelTests, selection_array_indicator) {

  C c;
  c.emplace(1, c);
  c.emplace(2, c);
  c.emplace(3, c);
  c.emplace(2, c);
  c.emplace(1, c);

  auto s = c.all();
  {
    auto index = s.indicator([](const V&x)->bool{return x.value%2==0;});
    EXPECT_EQ(index.size(),s.size());
    auto subset = s.filter(index.begin(),index.end());
    EXPECT_EQ(subset.size(),2);
  }

  {
    auto index = s.indicator([](const V&x)->bool{return x.value%2!=0;});
    EXPECT_EQ(index.size(),s.size());
    auto subset = s.filter(index.begin(),index.end());
    EXPECT_EQ(subset.size(),3);
  }

  {
    auto index = s.indicator([](const V&x)->bool{return x.value>1;});
    EXPECT_EQ(index.size(),s.size());
    auto subset = s.filter(index.begin(),index.end());
    EXPECT_EQ(subset.size(),3);
  }

  {
    auto index = std::vector<char>{1};
    EXPECT_THROW(s.filter(index.begin(),index.end()),SelectionException<V>);
  }


  {
    auto index = std::vector<char>{1,2,3,4,5,6};
    EXPECT_THROW(s.filter(index.begin(),index.end()),SelectionException<V>);
  }

  {
    auto index = std::vector<char>{1,2,3,4,5};
    EXPECT_NO_THROW(s.filter(index.begin(),index.end()));
  }

}


TEST_F(ConSelTests, selection_array_indexing) {

  C c;
  c.emplace(1, c);
  c.emplace(2, c);
  c.emplace(3, c);
  c.emplace(2, c);
  c.emplace(1, c);

  auto s = c.all();
  {
    auto index = s.index([](const V&x)->bool{return x.value%2==0;});
    EXPECT_EQ(index.size(),2);
    auto subset = s.at_index(index.begin(),index.end());
    EXPECT_EQ(subset.size(),2);
  }

  {
    auto index = s.index([](const V&x)->bool{return x.value%2!=0;});
    EXPECT_EQ(index.size(),3);
    auto subset = s.at_index(index.begin(),index.end());
    EXPECT_EQ(subset.size(),3);
  }

  {
    auto index = s.index([](const V&x)->bool{return x.value>1;});
    EXPECT_EQ(index.size(),3);
    auto subset = s.at_index(index.begin(),index.end());
    EXPECT_EQ(subset.size(),3);
  }

  {
    auto index = std::vector<int>{0};
    EXPECT_THROW(s.filter(index.begin(),index.end()),SelectionException<V>);
  }


  {
    auto index = std::vector<int>{0,0,0,0,0,0};
    EXPECT_THROW(s.filter(index.begin(),index.end()),SelectionException<V>);
  }

  {
    auto index = std::vector<int>{0,0,0,0,0};
    EXPECT_NO_THROW(s.filter(index.begin(),index.end()));
  }

}


TEST_F(ConSelTests, selection_count) {

  C c;
  c.emplace(1, c);
  c.emplace(2, c);
  c.emplace(3, c);

  C c2;
  c2.emplace(1, c2);
  c2.emplace(2, c2);
  c2.emplace(3, c2);
  c2.emplace(4, c2);

  S s = c.all();
  cS s2 = c2.all();

  ASSERT_EQ(s.size(), 3);
  ASSERT_EQ(s2.size(), 4);

  EXPECT_EQ(s.count(s[0]), 1);
  EXPECT_EQ(s2.count(s2[0]), 1);
  EXPECT_EQ(s2.count(s[0]), 0);
  EXPECT_EQ(s.count(s2[0]), 0);
}

TEST_F(ConSelTests, selection_grows) {

  C c;
  C c2;
  S s;
  cS s2;
  const int limit = 1025;
  for (int i = 0; i < limit; i++) {
    c.emplace(i, c);
    c2.emplace(i, c2);
    if (i % 100 == 0) {
      s = c.all();
      s2 = c2.all();
    }
  }

  s = c.all();
  s2 = c2.all();

  ASSERT_EQ(c.size(), limit);
  ASSERT_EQ(c2.size(), limit);

  auto it1 = s.begin();
  auto it2 = s2.begin();
  for (; it1 != s.end(); ++it1, ++it2) {
    ASSERT_TRUE(it1->value == it2->value);
  }
}

TEST_F(ConSelTests, selection_copy_move) {
  C c;
  C c2;

  S s3;
  cS s4;

  const int limit = 10;
  {
    S s;
    cS s2;
    for (int i = 0; i < limit; i++) {
      c.emplace(i, c);
      c2.emplace(i, c2);
    }
    s = c.all();
    const C& cc2 = c2;
    s2 = cc2.all();
    cS s5 = s2;
    s3 = std::move(s);
    s4 = std::move(s2);
  }
  EXPECT_EQ(s3.size(), limit);
  EXPECT_EQ(s4.size(), limit);
}

TEST_F(ConSelTests, access_dead_selection_throws) {

  C c;
  S s;
  const int n = 100;
  for (int i = 0; i < n; i++) {
    c.emplace(i, c);
  }

  s = c.all();

  EXPECT_NO_THROW(s[0]);
  EXPECT_NO_THROW(*s.begin());
  EXPECT_NO_THROW([&]{auto x = s.begin()->value;static_cast<void>(x);}());

  c.erase(c.all());

  ASSERT_EQ(c.size(), 0);
  ASSERT_EQ(s.size(), n);
  EXPECT_THROW(s[0], xmol::selection::deleted_element_access<int_with_parent>);
  EXPECT_THROW(*s.begin(), xmol::selection::deleted_element_access<int_with_parent>);
  EXPECT_THROW([&]{auto x = s.begin()->value;static_cast<void>(x);}(), xmol::selection::deleted_element_access<int_with_parent>);
}

TEST_F(ConSelTests, selection_tracks_container_move) {

  C c;
  S s;
  const int n = 100;
  for (int i = 0; i < n; i++) {
    c.emplace(i, c);
  }

  s = c.all();

  C c2 = std::move(c);

  ASSERT_EQ(c.size(), 0);
  ASSERT_EQ(c2.size(), n);
  ASSERT_TRUE(s.is_valid());

  EXPECT_NO_THROW(s[0]);
  EXPECT_NO_THROW(*s.begin());
  EXPECT_NO_THROW([&]{auto x = s.begin()->value;static_cast<void>(x);}());

  c.erase(c.all());

  ASSERT_EQ(c.size(), 0);
  ASSERT_EQ(s.size(), n);

  EXPECT_NO_THROW(s[0]);
  EXPECT_NO_THROW(*s.begin());
  EXPECT_NO_THROW([&]{auto x = s.begin()->value;static_cast<void>(x);}());
  int k = 0;
  EXPECT_NO_THROW(for (auto& x
                       : s) {
    x.value += 1;
    ++k;
  });
  EXPECT_EQ(k, n);
}

TEST_F(ConSelTests, selection_filter) {
  C c;

  const int n = 100;
  for (int i = 0; i < n; i++) {
    c.emplace(i, c);
  }

  S div_5 = c.all().filter([](const V& val) { return val.value % 5 == 0; });
  S not_div_5 =
      c.all().remove_if([](const V& val) { return val.value % 5 == 0; });

  EXPECT_EQ(div_5.size(), 20);
  EXPECT_EQ(not_div_5.size(), 80);
}

TEST_F(ConSelTests, selection_composition) {
  C c;

  const int n = 100;
  for (int i = 0; i < n; i++) {
    c.emplace(i, c);
  }

  S div_10 = c.all()
                 .filter([](const V& val) { return val.value % 5 == 0; })
                 .filter([](const V& val) { return val.value % 2 == 0; });
  EXPECT_EQ(div_10.size(), 10);
}

TEST_F(ConSelTests, selection_range_1) {
  C c;

  const int n = 100;
  for (int i = 0; i < n; i++) {
    c.emplace(i, c);
  }

  cS s = c.all();

  int sum = 0;
  for (auto it = s.begin(); it != s.end(); ++it) {
    sum += it->value;
  }
  EXPECT_EQ(sum, 50 * 99);

  int sum2 = 0;
  s.for_each([&sum2](const V& val) { sum2 += val.value; });

  EXPECT_EQ(sum2, 50 * 99);
}
