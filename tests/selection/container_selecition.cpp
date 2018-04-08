#include <gtest/gtest.h>

#include "xmol/selection/ContainerSelection.h"


using ::testing::Test;
using namespace xmol::selection;


class ConSelTests : public Test{

};

namespace {
struct int_with_parent{
  int_with_parent(int value,Container<int_with_parent>& container): value(value), ptr(&container){}
  int value;
  Container<int_with_parent>* ptr;
};
}

template<>
Container<int_with_parent>* ElementWithFlags<int_with_parent>::parent() noexcept {
  return value.ptr;
}

template<>
const Container<int_with_parent>* ElementWithFlags<int_with_parent>::parent() const noexcept {
  return value.ptr;
}

template<>
bool ElementWithFlags<int_with_parent>::operator<(const ElementWithFlags<int_with_parent>& rhs) const noexcept {
  return std::tuple(this->parent(),this->value.value) < std::tuple(rhs.parent(),rhs.value.value);
}

bool operator==(const int_with_parent& lhs, const int_with_parent& rhs) noexcept {
  return lhs.value == rhs.value && lhs.ptr==rhs.ptr;
}





TEST_F(ConSelTests, invalid_selection_throws_on_use){

  Selection<int_with_parent> s;
  {
    Container<int_with_parent> c;
    c.emplace(1,c);
    s = c.all();
    ASSERT_TRUE(s.is_valid());
  }
  ASSERT_FALSE(s.is_valid());
  Container<int_with_parent> c2;
  auto value = int_with_parent(1,c2);


  EXPECT_THROW(s[0],xmol::selection::dead_selection_access);
  EXPECT_THROW(s.count(value),xmol::selection::dead_selection_access);
  EXPECT_THROW(s.begin(),xmol::selection::dead_selection_access);
  EXPECT_THROW(s.end(),xmol::selection::dead_selection_access);
  EXPECT_NO_THROW(s.empty());
  EXPECT_NO_THROW(s.size());
}

TEST_F(ConSelTests, set_operations_and_comparisons){

  using V = int_with_parent;
  using C = Container<int_with_parent>;
  using S = Selection<int_with_parent>;
  using cS = Selection<const int_with_parent>;

  C c;
  c.emplace(1,c);
  c.emplace(2,c);
  c.emplace(3,c);


  C c2;
  c2.emplace(1,c2);
  c2.emplace(2,c2);
  c2.emplace(3,c2);
  c2.emplace(4,c2);


  S s = c.all();
  cS s2 = c2.all();


  ASSERT_EQ(s.size(),3);
  ASSERT_EQ(s2.size(),4);


  EXPECT_EQ(3,(s+s).size());
  EXPECT_EQ(7,(s+s2).size());
  EXPECT_EQ(7,(s2+s).size());
  EXPECT_EQ(4,(s2+s2).size());

  EXPECT_EQ(0,(s-s).size());
  EXPECT_EQ(3,(s-s2).size());
  EXPECT_EQ(4,(s2-s).size());
  EXPECT_EQ(0,(s2-s2).size());

  EXPECT_EQ(3,(s*s).size());
  EXPECT_EQ(0,(s*s2).size());
  EXPECT_EQ(0,(s2*s).size());
  EXPECT_EQ(4,(s2*s2).size());

  EXPECT_TRUE(s==s);
  EXPECT_TRUE(s2==s2);
  EXPECT_FALSE(s2==s);
  EXPECT_FALSE(s==s2);

  EXPECT_FALSE(s!=s);
  EXPECT_FALSE(s2!=s2);
  EXPECT_TRUE(s2!=s);
  EXPECT_TRUE(s!=s2);

}


TEST_F(ConSelTests, selection_indexing){

  using V = int_with_parent;
  using C = Container<int_with_parent>;
  using S = Selection<int_with_parent>;
  using cS = Selection<const int_with_parent>;

  C c;
  c.emplace(1,c);
  c.emplace(2,c);
  c.emplace(3,c);


  C c2;
  c2.emplace(1,c2);
  c2.emplace(2,c2);
  c2.emplace(3,c2);
  c2.emplace(4,c2);


  S s = c.all();
  cS s2 = c2.all();


  ASSERT_EQ(s.size(),3);
  ASSERT_EQ(s2.size(),4);


  EXPECT_NO_THROW(s[-1]);
  EXPECT_NO_THROW(s[-2]);
  EXPECT_NO_THROW(s[-3]);
  EXPECT_THROW(s[-4],std::out_of_range);
  EXPECT_NO_THROW(s[0]);
  EXPECT_NO_THROW(s[1]);
  EXPECT_NO_THROW(s[2]);
  EXPECT_THROW(s[3],std::out_of_range);


  EXPECT_NO_THROW(s2[-1]);
  EXPECT_NO_THROW(s2[-2]);
  EXPECT_NO_THROW(s2[-3]);
  EXPECT_NO_THROW(s2[-4]);
  EXPECT_THROW(s2[-5],std::out_of_range);
  EXPECT_NO_THROW(s2[0]);
  EXPECT_NO_THROW(s2[1]);
  EXPECT_NO_THROW(s2[2]);
  EXPECT_NO_THROW(s2[3]);
  EXPECT_THROW(s2[4],std::out_of_range);

}


TEST_F(ConSelTests, selection_count){

  using V = int_with_parent;
  using C = Container<int_with_parent>;
  using S = Selection<int_with_parent>;
  using cS = Selection<const int_with_parent>;

  C c;
  c.emplace(1,c);
  c.emplace(2,c);
  c.emplace(3,c);


  C c2;
  c2.emplace(1,c2);
  c2.emplace(2,c2);
  c2.emplace(3,c2);
  c2.emplace(4,c2);


  S s = c.all();
  cS s2 = c2.all();


  ASSERT_EQ(s.size(),3);
  ASSERT_EQ(s2.size(),4);


  EXPECT_EQ(s.count(s[0]),1);
  EXPECT_EQ(s2.count(s2[0]),1);
  EXPECT_EQ(s2.count(s[0]),0);
  EXPECT_EQ(s.count(s2[0]),0);

}

TEST_F(ConSelTests, selection_grows){

  using V = int_with_parent;
  using C = Container<int_with_parent>;
  using S = Selection<int_with_parent>;
  using cS = Selection<const int_with_parent>;

  C c;
  C c2;
  S s;
  cS s2;
  const int limit = 1025;
  for (int i=0;i<limit;i++){
    c.emplace(i,c);
    c2.emplace(i,c2);
    s = c.all();
    s2 = c2.all();
  }

  ASSERT_EQ(c.size(),limit);
  ASSERT_EQ(c2.size(),limit);

  auto it1 = s.begin();
  auto it2 = s2.begin();
  for (;it1!=s.end();++it1,++it2){
    ASSERT_TRUE(it1->value==it2->value);
  }
}


TEST_F(ConSelTests, selection_copy_move){
  xmol::utils::Logger::init("log.log",xmol::utils::Logger::VERBOSE);
  LOG_DEBUG_SCOPE("global");
  using V = int_with_parent;
  using C = Container<int_with_parent>;
  using S = Selection<int_with_parent>;
  using cS = Selection<const int_with_parent>;

  C c;
  C c2;

  S s3;
  cS s4;

  const int limit = 10;
  {
    LOG_DEBUG_SCOPE("local");
    S s;
    cS s2;
    for (int i=0;i<limit;i++){
      c.emplace(i,c);
      c2.emplace(i,c2);
    }
    s = c.all();
    s2 = c2.all();
    s3 = std::move(s);
    LOG_TRACE("HERE!");
    s4 = std::move(s2);
  }
  EXPECT_EQ(s3.size(),limit);
  EXPECT_EQ(s4.size(),limit);

}

