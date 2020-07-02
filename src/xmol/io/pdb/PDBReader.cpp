#include "xmol/io/pdb/PdbReader.h"
#include "xmol/io/pdb/PdbLine.h"
#include "xmol/io/pdb/PdbRecord.h"
#include "xmol/io/pdb/exceptions.h"
#include "xmol/utils/string.h"

using namespace xmol::io::pdb;
using namespace xmol;

namespace {

struct PdbLineSentinel {};

struct PdbLineInputIterator {
private:
  std::istream* sin_;
  std::string str_;
  int m_line_number = 0;
  PdbLine pdbLine;
  const basic_PdbRecords& db;

public:
  PdbLineInputIterator(std::istream& sin, const basic_PdbRecords& db) : sin_(&sin), str_{}, db(db) {}

  std::string& cached() noexcept { return str_; }
  int line_number() noexcept { return m_line_number; }

  PdbLineInputIterator& operator++() {
    std::getline(*sin_, str_, '\n');
    m_line_number++;
    if (str_.size() > 0) {
      pdbLine = PdbLine(str_, db);
    } else {
      pdbLine = PdbLine();
    }
    return *this;
  }

  const PdbLine* operator->() const { return &(this->pdbLine); }

  const PdbLine& operator*() const { return this->pdbLine; }

  bool operator!=(const PdbLineSentinel&) const { return !!(*sin_); }
};

template <typename Iterator> ResidueId to_resid(const Iterator& it) {
  using xmol::utils::trim;
  return ResidueId(it->getInt(FieldName("resSeq")), ResidueInsertionCode(trim(it->getString(FieldName("iCode")))));
}

struct AtomStub {
  explicit AtomStub(AtomName name, AtomId serial, XYZ xyz) : name(name), serial(serial), xyz(xyz){};
  AtomName name;
  AtomId serial;
  XYZ xyz;
};

struct ResidueStub {
  explicit ResidueStub(ResidueName name, ResidueId serial) : name(name), serial(serial){};
  ResidueName name;
  ResidueId serial;
  std::vector<AtomStub> atoms;
};

struct ChainStub {
  explicit ChainStub(MoleculeName name) : name(name){};
  MoleculeName name;
  std::vector<ResidueStub> residues;
};

struct FrameStub {
  int id;
  std::vector<ChainStub> chains;
};

template <typename Iterator> AtomStub& readAtom(ResidueStub& res, Iterator& it) {
  assert(it != PdbLineSentinel{});
  assert(it->getRecordName() == RecordName("ATOM") || it->getRecordName() == RecordName("HETATM") ||
         it->getRecordName() == RecordName("ANISOU"));
  using xmol::utils::trim;
  res.atoms.emplace_back(
      AtomName(trim(it->getString(FieldName("name")))), it->getInt(FieldName("serial")),
      XYZ{it->getDouble(FieldName("x")), it->getDouble(FieldName("y")), it->getDouble(FieldName("z"))});
  AtomStub& atom = res.atoms.back();
  ++it;

  // skip "ANISOU" records
  while (it != PdbLineSentinel{} &&
         (it->getRecordName() == RecordName("ANISOU") || it->getRecordName() == RecordName("SIGATM") ||
          it->getRecordName() == RecordName("SIGUIJ"))) {
    ++it;
  }

  return atom;
}

template <typename Iterator> ResidueStub& readResidue(ChainStub& c, Iterator& it) {
  assert(it != PdbLineSentinel{});
  assert(it->getRecordName() == RecordName("ATOM") || it->getRecordName() == RecordName("HETATM") ||
         it->getRecordName() == RecordName("ANISOU"));

  using xmol::utils::trim;

  auto residueId = to_resid(it);
  int chainName = it->getChar(FieldName("chainID"));

  c.residues.emplace_back(ResidueName(trim(it->getString(FieldName("resName")))), residueId);
  ResidueStub& r = c.residues.back();

  while (it != PdbLineSentinel{} &&
         (it->getRecordName() == RecordName("ATOM") || it->getRecordName() == RecordName("HETATM") ||
          it->getRecordName() == RecordName("ANISOU")) &&
         it->getChar(FieldName("chainID")) == chainName && to_resid(it) == residueId) {
    readAtom(r, it);
  }

  return r;
}

template <typename Iterator> ChainStub& readChain(FrameStub& frame, Iterator& it) {
  assert(it->getRecordName() == RecordName("ATOM") || it->getRecordName() == RecordName("HETATM"));
  std::string stringChainId = it->getString(FieldName("chainID"));

  frame.chains.emplace_back(MoleculeName(stringChainId));
  ChainStub& c = frame.chains.back();

  while (it != PdbLineSentinel{} && it->getRecordName() != RecordName("TER") &&
         (it->getRecordName() == RecordName("ATOM") || it->getRecordName() == RecordName("HETATM")) &&
         it->getChar(FieldName("chainID")) == stringChainId[0]) {
    readResidue(c, it);
  }

  if (it != PdbLineSentinel{} && it->getRecordName() == "TER") {
    ++it;
  }
  return c;
}

template <typename Iterator> Frame readFrame(Iterator& it) {
  int id{0};
  bool has_model = false;
  if (it->getRecordName() == RecordName("MODEL")) {
    id = it->getInt(FieldName("serial"));
    has_model = true;
    ++it;
  }
  FrameStub frame_stub{id};
  assert(it->getRecordName() == RecordName("ATOM") || it->getRecordName() == RecordName("HETATM"));

  while (it != PdbLineSentinel{} &&
         ((has_model && it->getRecordName() != RecordName("ENDMDL")) || it->getRecordName() == RecordName("ATOM") ||
          it->getRecordName() == RecordName("HETATM"))) {
    readChain(frame_stub, it);
  }

  if (it != PdbLineSentinel{}) {
    ++it;
  }

  Frame result;
  for (auto& chain_stub : frame_stub.chains) {
    auto c = result.add_molecule().name(chain_stub.name);
    for (auto& residue_stub : chain_stub.residues) {
      auto r = c.add_residue().name(residue_stub.name).id(residue_stub.serial);
      for (auto& atom_stub : residue_stub.atoms) {
        r.add_atom().name(atom_stub.name).id(atom_stub.serial).r(atom_stub.xyz);
      }
    }
  }

  return result;
}

geom::UnitCell read_cell_from_cryst1_record(const PdbLine& line){
  return geom::UnitCell(
      line.getDouble(FieldName("a")),
      line.getDouble(FieldName("b")),
      line.getDouble(FieldName("c")),
      geom::Degrees(line.getDouble(FieldName("alpha"))),
      geom::Degrees(line.getDouble(FieldName("beta"))),
      geom::Degrees(line.getDouble(FieldName("gamma")))
  );
}

} // namespace

Frame PdbReader::read_frame() { return read_frame(StandardPdbRecords::instance()); }

Frame PdbReader::read_frame(const basic_PdbRecords& db) {
  Frame result;

  std::vector<Frame> frames;

  auto it = PdbLineInputIterator(*is, db);
  try {
    while (it != PdbLineSentinel{}) {
      if (it->getRecordName() == RecordName("MODEL") || it->getRecordName() == RecordName("ATOM") ||
          it->getRecordName() == RecordName("HETATM")) {
        return readFrame(it);
      } else {
        ++it;
      }
    }
  } catch (PdbFieldReadError& e) {
    std::string filler(std::min(std::max(e.colon_l, 0), 80), '~');
    std::string underline(std::min(e.colon_r - e.colon_l + 1, 80), '^');
    throw PdbException(std::string(e.what()) + "\n" + "at line " + std::to_string(it.line_number()) + ":" +
                       std::to_string(e.colon_l) + "-" + std::to_string(e.colon_r) + "\n" + it.cached() + "\n" +
                       filler + underline);
  }
  return result;
}

std::vector<Frame> PdbReader::read_frames() { return read_frames(StandardPdbRecords::instance()); }

std::vector<Frame> PdbReader::read_frames(const basic_PdbRecords& db) {

  std::vector<Frame> frames;
  auto cell = geom::UnitCell::unit_cubic_cell(); // create dummy cell

  auto it = PdbLineInputIterator(*is, db);
  try {
    while (it != PdbLineSentinel{}) {
      if (it->getRecordName()== RecordName("CRYST1")) {
        cell = read_cell_from_cryst1_record(*it);
      } else if (it->getRecordName() == RecordName("MODEL") || it->getRecordName() == RecordName("ATOM") ||
          it->getRecordName() == RecordName("HETATM")) {
        frames.push_back(readFrame(it));
        frames.back().cell = cell;
        continue;
      }
      ++it;
    }
  } catch (PdbFieldReadError& e) {
    std::string filler(std::min(std::max(e.colon_l, 0), 80), '~');
    std::string underline(std::min(e.colon_r - e.colon_l + 1, 80), '^');
    throw PdbException(std::string(e.what()) + "\n" + "at line " + std::to_string(it.line_number()) + ":" +
                       std::to_string(e.colon_l) + "-" + std::to_string(e.colon_r) + "\n" + it.cached() + "\n" +
                       filler + underline);
  }
  return frames;
}
