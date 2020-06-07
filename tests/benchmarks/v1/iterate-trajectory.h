#pragma once

#include "common.h"
#include "xmol/v1/trajectory/Trajectory.h"
#include "xmol/v1/trajectory/TrjtoolDatFile.h"
#include "xmol/v1/io/PdbFile.h"

#include <cassert>
#include <cstdlib>

using namespace trajectory;

class BM_TrajectoryTrjtool : public benchmark::Fixture {
public:
  void SetUp(const ::benchmark::State& state) {
    using xmol::v1::io::PdbFile;
    char* data_path = std::getenv("TEST_DATA_PATH");
    assert(data_path);
    const std::string filename = std::string(data_path) + "/trjtool/GB1/run00001.dat";
    const std::string pdb_filename = std::string(data_path) + "/trjtool/GB1/run00001.pdb";

    Frame frame = PdbFile(pdb_filename).read(PdbFile::Dialect::AMBER_99).frames()[0];

    trj_ptr = std::make_unique<Trajectory>(std::move(frame));
    trj_ptr->extend(TrjtoolDatFile(filename));
    trj_ptr->extend(TrjtoolDatFile(filename));
  }

  void TearDown(const ::benchmark::State& state) { trj_ptr = {}; }
  std::unique_ptr<Trajectory> trj_ptr;
};