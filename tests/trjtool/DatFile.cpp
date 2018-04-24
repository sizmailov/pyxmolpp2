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

  xmol::pdb::AlteredPdbRecords permissibleRecords;
};


TEST_F(TrjtoolDatFileTests, basic_read) {
  std::ifstream ifs2("trjtool/GB1/run00001.pdb", std::ios::binary);
  auto frame = xmol::pdb::PdbReader(ifs2).read_frame(permissibleRecords);

  DatFile datfile("trjtool/GB1/run00001.dat");

  auto atoms = frame.asAtoms();
  EXPECT_TRUE(datfile.match(atoms));
  EXPECT_EQ(datfile.n_atoms_per_frame(), frame.asAtoms().size());
  EXPECT_EQ(datfile.n_frames(), 1000);


  xmol::geometry::XYZ first_atom_coords (-3.2724499702453613, -9.4666690826416016, 8.9505224227905273 );
  EXPECT_GE((first_atom_coords-frame.asAtoms()[0].r()).len(), 1e-1);
  datfile.set_coordinates(0,frame.asAtoms());
  EXPECT_LE((first_atom_coords-frame.asAtoms()[0].r()).len(), 1e-3);

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


  xmol::geometry::XYZ first_atom_coords (-3.2724499702453613, -9.4666690826416016, 8.9505224227905273 );
  EXPECT_GE((first_atom_coords-frame.asAtoms()[0].r()).len(), 1e-1);
  datfile.set_coordinates(0,frame.asAtoms());
  EXPECT_LE((first_atom_coords-frame.asAtoms()[0].r()).len(), 1e-3);

}
