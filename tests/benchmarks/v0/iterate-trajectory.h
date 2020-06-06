#pragma once

#include "common.h"
#include "xmol/trajectory/Trajectory.h"
#include "xmol/trjtool/DatFile.h"
#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbRecord.h"

#include <cassert>
#include <cstdlib>


class BM_TrajectoryTrjtool : public benchmark::Fixture {
public:
  void SetUp(const ::benchmark::State& state) {
    char* data_path = std::getenv("TEST_DATA_PATH");
    assert(data_path);
    const std::string filename = std::string(data_path) + "/trjtool/GB1/run00001.dat";

    xmol::pdb::AlteredPdbRecords permissibleRecords(xmol::pdb::StandardPdbRecords::instance());
    permissibleRecords.alter_record(xmol::pdb::RecordName("ATOM"),
                                    xmol::pdb::FieldName("serial"),
                                    {7, 12});

    std::ifstream ifs2(std::string(data_path) + "/trjtool/GB1/run00001.pdb", std::ios::binary);
    auto ref = xmol::pdb::PdbReader(ifs2).read_frame(permissibleRecords);

    trj_ptr = std::make_unique<xmol::trajectory::Trajectory>(ref);
    trj_ptr->push_trajectory_portion(xmol::trjtool::DatFile(std::string(data_path) + "/trjtool/GB1/run00001.dat"));
    trj_ptr->push_trajectory_portion(xmol::trjtool::DatFile(std::string(data_path) + "/trjtool/GB1/run00001.dat"));
  }

  void TearDown(const ::benchmark::State& state) { trj_ptr = {}; }
  std::unique_ptr<xmol::trajectory::Trajectory> trj_ptr;
};