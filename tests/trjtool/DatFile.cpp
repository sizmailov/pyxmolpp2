#include <gtest/gtest.h>

#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbRecord.h"
#include "xmol/trjtool/DatFile.h"

using ::testing::Test;
using namespace xmol::trjtool;

class TrjtoolDatFileTests : public Test {
public:
  TrjtoolDatFileTests()
      : permissibleRecords(xmol::pdb::StandardPdbRecords::instance()) {
    permissibleRecords.alter_record(xmol::pdb::RecordName("ATOM"),
        xmol::pdb::FieldName("serial"),
        {7, 12});
  }
  std::vector<int> get_vector_n(int n) const {
    std::vector<int> update_list;
    for (int k=0;k<n;k++){update_list.push_back(k);}
    return update_list;
  }

  xmol::pdb::AlteredPdbRecords permissibleRecords;
};


TEST_F(TrjtoolDatFileTests, basic_read) {
  std::ifstream ifs2("trjtool/GB1/run00001.pdb", std::ios::binary);
  auto frame = xmol::pdb::PdbReader(ifs2).read_frame(permissibleRecords);
  const auto frame999 = frame;

  DatFile datfile("trjtool/GB1/run00001.dat");

  auto atoms = frame.asAtoms();
  EXPECT_TRUE(datfile.match(atoms));
  EXPECT_EQ(datfile.n_atoms_per_frame(), frame.asAtoms().size());
  EXPECT_EQ(datfile.n_frames(), 1000);


  xmol::geometry::XYZ first_atom_coords (8.9505224227905273, -9.4666690826416016, -3.2724499702453613);
  EXPECT_GE((first_atom_coords-frame.asAtoms()[0].r()).len(), 1e-1);
  datfile.set_coordinates(0,frame.asAtoms(),get_vector_n(frame.asAtoms().size()));
  EXPECT_LE((first_atom_coords-frame.asAtoms()[0].r()).len(), 1e-3);

  datfile.set_coordinates(999,frame.asAtoms(),get_vector_n(frame.asAtoms().size()));
  first_atom_coords = frame999.asAtoms()[0].r();
  auto frame_r = frame.asAtoms()[0].r();
  EXPECT_LE(std::fabs(frame_r.x()-first_atom_coords.x()),1e-3);
  EXPECT_LE(std::fabs(frame_r.y()-first_atom_coords.y()),1e-3);
  EXPECT_LE(std::fabs(frame_r.z()-first_atom_coords.z()),1e-3);

}

TEST_F(TrjtoolDatFileTests, basic_read_copy) {
  std::ifstream ifs2("trjtool/GB1/run00001.pdb", std::ios::binary);
  auto frame = xmol::pdb::PdbReader(ifs2).read_frame(permissibleRecords);

  DatFile datfile0("trjtool/GB1/run00001.dat");
  DatFile datfile(datfile0);

  auto atoms = frame.asAtoms();
  EXPECT_TRUE(datfile.match(atoms));
  EXPECT_EQ(datfile.n_atoms_per_frame(), frame.asAtoms().size());
  EXPECT_EQ(datfile.n_frames(), 1000);


  xmol::geometry::XYZ first_atom_coords (8.9505224227905273, -9.4666690826416016, -3.2724499702453613);
  EXPECT_GE((first_atom_coords-frame.asAtoms()[0].r()).len(), 1e-1);
  datfile.set_coordinates(0,frame.asAtoms(),get_vector_n(frame.asAtoms().size()));
  EXPECT_LE((first_atom_coords-frame.asAtoms()[0].r()).len(), 1e-3);

}
