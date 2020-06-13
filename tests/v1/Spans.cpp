#include <gtest/gtest.h>

#include "xmol/Frame.h"
#include "xmol/geom/affine/Transformation3d.h"
#include "xmol/proxy/smart/spans.h"

using ::testing::Test;
using namespace xmol;
using namespace xmol::proxy;
using namespace xmol::proxy::smart;

class SpanTests : public Test {

public:
  [[nodiscard]] static Frame make_polyglycines(const std::vector<std::pair<std::string, int>>& chain_sizes) {
    Frame frame;
    add_polyglycines(chain_sizes, frame);
    return frame;
  }

  void static add_polyglycines(const std::vector<std::pair<std::string, int>>& chain_sizes, Frame& frame) {
    int aid = 1;
    int rid = 1;
    for (auto& chN_S : chain_sizes) {
      auto c = frame.add_molecule().name(chN_S.first);
      for (int i = 0; i < chN_S.second; i++) {
        auto r = c.add_residue().name("GLY").id(rid++);
        r.add_atom().name("N").id(aid++);
        r.add_atom().name("H").id(aid++);
        r.add_atom().name("CA").id(aid++);
        r.add_atom().name("HA2").id(aid++);
        r.add_atom().name("HA3").id(aid++);
        r.add_atom().name("C").id(aid++);
        r.add_atom().name("O").id(aid++);
      }
    }
  }
};

TEST_F(SpanTests, ref_count) {
  auto frame = make_polyglycines({{"A", 10}, {"B", 20}});

  auto n_mol_refs = [&] { return frame.n_references<MoleculeSmartSpan>(); };
  auto n_atom_refs = [&] { return frame.n_references<MoleculeSmartSpan>(); };
  auto n_res_refs = [&] { return frame.n_references<MoleculeSmartSpan>(); };

  ASSERT_EQ(n_mol_refs(), 0);
  ASSERT_EQ(n_res_refs(), 0);
  ASSERT_EQ(n_atom_refs(), 0);
  {
    auto atoms = frame.atoms().smart();
    auto residues = frame.residues().smart();
    auto molecules = frame.molecules().smart();

    ASSERT_EQ(n_mol_refs(), 1);
    ASSERT_EQ(n_res_refs(), 1);
    ASSERT_EQ(n_atom_refs(), 1);
    {
      auto atoms2 = atoms;
      auto residues2 = residues;
      auto molecules2 = molecules;

      ASSERT_EQ(n_mol_refs(), 2);
      ASSERT_EQ(n_res_refs(), 2);
      ASSERT_EQ(n_atom_refs(), 2);

      {
        auto atoms3 = molecules.atoms().smart();
        auto residues3 = molecules.residues().smart();
        auto molecule3 = residues.molecules().smart();

        ASSERT_EQ(n_mol_refs(), 3);
        ASSERT_EQ(n_res_refs(), 3);
        ASSERT_EQ(n_atom_refs(), 3);
        {

          auto atoms4 = residues.atoms().smart();
          auto residues4 = atoms.residues().smart();
          auto molecule4 = atoms.molecules().smart();

          ASSERT_EQ(n_mol_refs(), 4);
          ASSERT_EQ(n_res_refs(), 4);
          ASSERT_EQ(n_atom_refs(), 4);
        }
        ASSERT_EQ(n_mol_refs(), 3);
        ASSERT_EQ(n_res_refs(), 3);
        ASSERT_EQ(n_atom_refs(), 3);
      }
      ASSERT_EQ(n_mol_refs(), 2);
      ASSERT_EQ(n_res_refs(), 2);
      ASSERT_EQ(n_atom_refs(), 2);
    }
    ASSERT_EQ(n_mol_refs(), 1);
    ASSERT_EQ(n_res_refs(), 1);
    ASSERT_EQ(n_atom_refs(), 1);
  }
  ASSERT_EQ(n_mol_refs(), 0);
  ASSERT_EQ(n_res_refs(), 0);
  ASSERT_EQ(n_atom_refs(), 0);
}

TEST_F(SpanTests, split_exceptinos) {
  auto frame = make_polyglycines({{"A", 2}, {"B", 3}});
  auto atoms = frame.atoms().smart();
  auto residues = frame.residues().smart();
  auto molecules = frame.molecules().smart();

  residues[residues.size() - 1].add_atom();
  molecules[molecules.size() - 1].add_residue();
  frame.add_molecule();
  EXPECT_NO_THROW(static_cast<void>(atoms.size()));
  EXPECT_NO_THROW(static_cast<void>(residues.size()));
  EXPECT_NO_THROW(static_cast<void>(molecules.size()));

  residues[0].add_atom();
  EXPECT_THROW(static_cast<void>(atoms.size()), SpanSplitError);
  EXPECT_NO_THROW(static_cast<void>(residues.size()));
  EXPECT_NO_THROW(static_cast<void>(molecules.size()));

  atoms = frame.atoms(); // restore to check that residues creation doesn't affect atoms

  molecules[0].add_residue();
  EXPECT_THROW(static_cast<void>(residues.size()), SpanSplitError);
  EXPECT_NO_THROW(static_cast<void>(atoms.size()));
  EXPECT_NO_THROW(static_cast<void>(molecules.size()));

  residues = frame.residues();

  EXPECT_NO_THROW(static_cast<void>(atoms.size()));
  EXPECT_NO_THROW(static_cast<void>(residues.size()));
  EXPECT_NO_THROW(static_cast<void>(molecules.size()));
}

TEST_F(SpanTests, span_transforms) {
  using namespace xmol::geom::affine;
  using namespace xmol::geom;
  auto frame = make_polyglycines({{"A", 1}});
  auto coords = frame.coords();
  EXPECT_DOUBLE_EQ(coords._eigen().norm(), 0);
  coords.apply(Translation3d(XYZ(1, 1, 1)));
  EXPECT_DOUBLE_EQ((coords._eigen().array() - 1).matrix().norm(), 0);
  coords.apply(Translation3d(XYZ(0, -1, -1)));
  coords.apply(Rotation3d(XYZ(0, 0, 1), Degrees(90)));
  EXPECT_LE((coords._eigen().rowwise() - XYZ(0, 1, 0)._eigen()).norm(), 1e-9);
  coords.apply(Rotation3d(XYZ(0, 0, 1), Degrees(90)));
  EXPECT_LE((coords._eigen().rowwise() - XYZ(-1, 0, 0)._eigen()).norm(), 1e-9);
  coords.apply(Rotation3d(XYZ(0, 0, 1), Degrees(90)));
  EXPECT_LE((coords._eigen().rowwise() - XYZ(0, -1, 0)._eigen()).norm(), 1e-9);
  coords.apply(Rotation3d(XYZ(1, 0, 0), Degrees(90)));
  EXPECT_LE((coords._eigen().rowwise() - XYZ(0, 0, -1)._eigen()).norm(), 1e-9);
  coords.apply(UniformScale3d(10));
  EXPECT_LE((coords._eigen().rowwise() - XYZ(0, 0, -10)._eigen()).norm(), 1e-9);
}

TEST_F(SpanTests, coordinate_span_to_eigen) {
  auto frame = make_polyglycines({{"A", 1}});
  auto coords = frame.coords();
  coords[0].set({1, 2, 3});
  size_t last = coords.size() - 1;
  coords[last].set({4, 5, 6});

  auto matrix_ref = coords._eigen();
  matrix_ref.col(1).setConstant(1.0); // set all y values to 1
  std::cout << matrix_ref << std::endl;
  EXPECT_DOUBLE_EQ(coords[0].distance({1, 1, 3}), 0);
  EXPECT_DOUBLE_EQ(coords[last].distance({4, 1, 6}), 0);
}

TEST_F(SpanTests, int_span) {
  std::vector<int> vec;
  int n = 10;
  vec.resize(n);
  future::Span<int> span(vec.data(), vec.size());
  ASSERT_EQ(vec.size(), span.size());
  for (int i = 0; i < n; i++) {
    ASSERT_EQ(&vec[i], &span[i]);
  }
  {
    auto it_v = vec.begin();
    auto it_span = span.begin();
    int i = 0;
    for (; it_span != span.end(); ++it_span, ++it_v, ++i) {
      ASSERT_EQ(&(*it_v), &(*it_span));
    }
    ASSERT_EQ(i, n);
  }
}

namespace {

struct FooStruct {
  char header[8];
  double payload[30];
};

struct FooStrutProxy {
  using value_type = FooStruct;
  FooStrutProxy(value_type* begin, value_type*) { ptr = begin; }
  FooStrutProxy(value_type& value) { ptr = &value; }
  bool operator!=(const FooStrutProxy& other) const { return ptr != other.ptr; }
  bool operator==(const FooStrutProxy& other) const { return ptr == other.ptr; }
  void advance() { ++ptr; }
  value_type* ptr;
};

} // namespace

TEST_F(SpanTests, foo_proxy_span) {
  for (int n = 0; n < 30; n += 7) {
    std::vector<FooStruct> vec;
    vec.resize(n);
    ProxySpan<FooStrutProxy, FooStruct> span(vec.data(), vec.size());
    ASSERT_EQ(vec.size(), span.size());
    ASSERT_EQ(span.size(), n);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(&vec[i], span[i].ptr);
    }
    {
      auto it_v = vec.begin();
      auto it_span = span.begin();
      int i = 0;
      for (; it_span != span.end(); ++it_span, ++it_v, ++i) {
        ASSERT_EQ(&(*it_v), it_span->ptr);
      }
      ASSERT_EQ(i, n);
    }
  }
}