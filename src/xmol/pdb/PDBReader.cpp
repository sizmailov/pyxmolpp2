#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbLine.h"
#include "xmol/pdb/PdbRecord.h"
#include "xmol/utils/string.h"

#include "range/v3/all.hpp"

using namespace xmol::pdb;
using namespace xmol::polymer;

namespace {

struct AtomStub {
  explicit AtomStub(AtomName name, atomId_t serial, XYZ xyz)
      : name(name), serial(serial), xyz(xyz){};
  AtomName name;
  atomId_t serial;
  XYZ xyz;
};

struct ResidueStub {
  explicit ResidueStub(ResidueName name, residueId_t serial)
      : name(name), serial(serial){};
  ResidueName name;
  residueId_t serial;
  std::vector<AtomStub> atoms;
};

struct ChainStub {
  explicit ChainStub(ChainName name) : name(name){};
  ChainName name;
  std::vector<ResidueStub> residues;
};

struct FrameStub {
  frameIndex_t id;
  std::vector<ChainStub> chains;
};

template <typename Iterator>
AtomStub& readAtom(ResidueStub& res, Iterator& it) {
  assert(it != ranges::default_sentinel{});
  assert(it->getRecordName() == RecordName("ATOM") ||
         it->getRecordName() == RecordName("HETATM") ||
         it->getRecordName() == RecordName("ANISOU"));
  using xmol::utils::string::trim;
  AtomStub& atom = res.atoms.emplace_back(
      AtomName(trim(it->getString(FieldName("name")))),
      it->getInt(FieldName("serial")),
      XYZ{it->getDouble(FieldName("x")),
          it->getDouble(FieldName("y")),
          it->getDouble(FieldName("z"))});
  ++it;

  // skip "ANISOU" records
  while (it != ranges::default_sentinel{} &&
         (it->getRecordName() == RecordName("ANISOU") ||
          it->getRecordName() == RecordName("SIGATM") ||
          it->getRecordName() == RecordName("SIGUIJ"))) {
    ++it;
  }

  return atom;
}

template <typename Iterator>
ResidueStub& readResidue(ChainStub& c, Iterator& it) {
  assert(it != ranges::default_sentinel{});
  assert(it->getRecordName() == RecordName("ATOM") ||
         it->getRecordName() == RecordName("HETATM") ||
         it->getRecordName() == RecordName("ANISOU"));

  using xmol::utils::string::trim;

  residueId_t residueId = it->getInt(FieldName("resSeq"));
  chainIndex_t chainName = it->getChar(FieldName("chainID"));

  ResidueStub& r = c.residues.emplace_back(
      ResidueName(trim(it->getString(FieldName("resName")))), residueId);

  while (it != ranges::default_sentinel{} &&
         (it->getRecordName() == RecordName("ATOM") ||
          it->getRecordName() == RecordName("HETATM") ||
          it->getRecordName() == RecordName("ANISOU")) &&
         it->getChar(FieldName("chainID")) == chainName &&
         it->getInt(FieldName("resSeq")) == residueId) {
    readAtom(r, it);
  }

  return r;
}

template <typename Iterator>
ChainStub& readChain(FrameStub& frame, Iterator& it) {
  assert(it->getRecordName() == RecordName("ATOM") ||
         it->getRecordName() == RecordName("HETATM"));
  std::string stringChainId = it->getString(FieldName("chainID"));

  ChainStub& c = frame.chains.emplace_back(ChainName(stringChainId));

  while (it != ranges::default_sentinel{} &&
         it->getRecordName() != RecordName("TER") &&
         (it->getRecordName() == RecordName("ATOM") ||
          it->getRecordName() == RecordName("HETATM")) &&
         it->getChar(FieldName("chainID")) == stringChainId[0]) {
    readResidue(c, it);
  }

  if (it != ranges::default_sentinel{} && it->getRecordName() == "TER") {
    ++it;
  }
  return c;
}

template <typename Iterator> Frame readFrame(Iterator& it) {
  frameIndex_t id{0};
  bool has_model = false;
  if (it->getRecordName() == RecordName("MODEL")) {
    id = it->getInt(FieldName("serial"));
    has_model = true;
    ++it;
  }
  FrameStub frame{id};
  assert(it->getRecordName() == RecordName("ATOM") ||
         it->getRecordName() == RecordName("HETATM"));
  chainIndex_t chainIndex = 0;
  while (it != ranges::default_sentinel{} &&
         ((has_model && it->getRecordName() != RecordName("ENDMDL")) ||
          it->getRecordName() == RecordName("ATOM") ||
          it->getRecordName() == RecordName("HETATM"))) {
    readChain(frame, it);
  }

  if (it != ranges::default_sentinel{}) {
    ++it;
  }

  Frame result(frame.id, frame.chains.size());
  for (auto& chain_stub : frame.chains) {
    auto& c = result.emplace(chain_stub.name, chain_stub.residues.size());
    for (auto& residue_stub : chain_stub.residues) {
      auto& r = c.emplace(residue_stub.name, residue_stub.serial,
                          residue_stub.atoms.size());
      for (auto& atom_stub : residue_stub.atoms) {
        r.emplace(atom_stub.name, atom_stub.serial, atom_stub.xyz);
      }
    }
  }

  return result;
}

using namespace ranges;
struct getPDBLines_range : view_facade<getPDBLines_range, ranges::unknown> {
private:
  friend range_access;
  std::istream* sin_;
  std::string str_;
  PdbLine pdbLine;
  const basic_PdbRecords& db;
  struct cursor {
  private:
    getPDBLines_range* rng_;

  public:
    cursor() = default;
    explicit cursor(getPDBLines_range& rng) : rng_(&rng) {}
    void next() { rng_->next(); }
    const PdbLine& read() const noexcept { return rng_->pdbLine; }

    bool equal(default_sentinel) const { return !*rng_->sin_; }
  };
  void next() {
    std::getline(*sin_, str_, '\n');
    if (str_.size() > 0) {
      pdbLine = PdbLine(str_, db);
    } else {
      pdbLine = PdbLine();
    }
  }
  cursor begin_cursor() { return cursor{*this}; }

public:
  getPDBLines_range() = default;
  getPDBLines_range(std::istream& sin, const basic_PdbRecords& db)
      : sin_(&sin), str_{}, db(db) {
    this->next(); // prime the pump
  }
  //  std::string & cached() noexcept
  //  {
  //    return str_;
  //  }
};

struct getPDBLines_fn {
  getPDBLines_range operator()(
      std::istream& sin,
      const basic_PdbRecords& db = StandardPdbRecords::instance()) const {
    return getPDBLines_range{sin, db};
  }
};
inline namespace function_objects {
inline constexpr getPDBLines_fn getPDBLines{};
}
}

Frame PdbReader::read_frame() {
  return read_frame(StandardPdbRecords::instance());
}

Frame PdbReader::read_frame(const basic_PdbRecords& db) {
  Frame result(0);

  auto pdbLines = getPDBLines(*is, db);

  std::vector<Frame> frames;

  auto it = ranges::begin(pdbLines);

  while (it != ranges::default_sentinel{}) {
    if (it->getRecordName() == RecordName("MODEL") ||
        it->getRecordName() == RecordName("ATOM") ||
        it->getRecordName() == RecordName("HETATM")) {
      return readFrame(it);
    } else {
      ++it;
    }
  }
  return result;
}

std::vector<Frame> PdbReader::read_frames() {

  auto pdbLines = getPDBLines(*is);
  std::vector<Frame> frames;

  auto it = ranges::begin(pdbLines);

  while (it != ranges::default_sentinel{}) {
    if (it->getRecordName() == RecordName("MODEL") ||
        it->getRecordName() == RecordName("ATOM") ||
        it->getRecordName() == RecordName("HETATM")) {
      frames.push_back(readFrame(it));
    } else {
      ++it;
    }
  }
  return frames;
}
