#include "xmol/pdb/PDBReader.h"
#include "xmol/pdb/PDBRecord.h"
#include "xmol/pdb/PDBLine.h"
#include "xmol/utils/string.h"

#include "range/v3/all.hpp"

using namespace xmol::pdb;
using namespace xmol::polymer;

namespace {

template<typename Iterator>
Atom& readAtom(Residue& res, Iterator& it) {
  assert( it != ranges::default_sentinel{});
  assert( it->getRecordName()==RecordTypeName("ATOM")   ||
      it->getRecordName()==RecordTypeName("HETATM") ||
      it->getRecordName()==RecordTypeName("ANISOU") );
  using xmol::utils::string::trim;
  Atom& atom = res.emplace(
      AtomName(trim(it->getString(RecordFieldName("name")))),
      it->getInt(RecordFieldName("serial")),
      XYZ{
          it->getDouble(RecordFieldName("x")),
          it->getDouble(RecordFieldName("y")),
          it->getDouble(RecordFieldName("z"))
      }
  );
  ++it;

  // skip "ANISOU" records
  while (it!=ranges::default_sentinel{} &&
      (it->getRecordName()==RecordTypeName("ANISOU") ||
          it->getRecordName()==RecordTypeName("SIGATM") ||
          it->getRecordName()==RecordTypeName("SIGUIJ") ) ){
    ++it;
  }

  return atom;
}

template<typename Iterator>
Residue& readResidue(Chain& c, Iterator& it) {
  assert( it != ranges::default_sentinel{});
  assert( it->getRecordName()==RecordTypeName("ATOM")   ||
      it->getRecordName()==RecordTypeName("HETATM") ||
      it->getRecordName()==RecordTypeName("ANISOU") );

  using xmol::utils::string::trim;

  residueId_t residueId = it->getInt(RecordFieldName("resSeq"));
  chainIndex_t chainName = it->getChar(RecordFieldName("chainID"));

  Residue& r = c.emplace(ResidueName(trim(it->getString(RecordFieldName("resName")))),residueId);

  while (it!=ranges::default_sentinel{} &&
      (it->getRecordName()==RecordTypeName("ATOM")   ||
          it->getRecordName()==RecordTypeName("HETATM") ||
          it->getRecordName()==RecordTypeName("ANISOU")) &&
      it->getChar(RecordFieldName("chainID"))==chainName &&
      it->getInt(RecordFieldName("resSeq"))==residueId ) {
      readAtom(r, it);
  }

  return r;
}

template<typename Iterator>
Chain& readChain(Frame& frame, Iterator& it)  {
  assert(it->getRecordName()==RecordTypeName("ATOM")
      || it->getRecordName()==RecordTypeName("HETATM"));
  std::string stringChainId = it->getString(RecordFieldName("chainID"));

  Chain& c = frame.emplace(ChainName(stringChainId));

  while ( it != ranges::default_sentinel{} &&
      it->getRecordName()!=RecordTypeName("TER") &&
      ( it->getRecordName()==RecordTypeName("ATOM") ||
          it->getRecordName()==RecordTypeName("HETATM")
      ) &&
      it->getChar(RecordFieldName("chainID"))==stringChainId[0]
      ) {
      readResidue(c,it);
  }

  if (it != ranges::default_sentinel{} && it->getRecordName()=="TER"){
    ++it;
  }
  return c;
}

template<typename Iterator>
Frame readFrame(Iterator& it) {
  frameIndex_t id{0};
  bool has_model = false;
  if (it->getRecordName()==RecordTypeName("MODEL")){
    id = it->getInt(RecordFieldName("serial"));
    has_model = true;
    ++it;
  }
  Frame frame(id);
  assert(it->getRecordName()==RecordTypeName("ATOM") ||
      it->getRecordName()==RecordTypeName("HETATM"));
  chainIndex_t chainIndex=0;
  while ( it!=ranges::default_sentinel{} &&
      ((has_model && it->getRecordName() != RecordTypeName("ENDMDL")) ||
          it->getRecordName()==RecordTypeName("ATOM") ||
          it->getRecordName()==RecordTypeName("HETATM"))
      ) {
    readChain(frame,it);
  }

  if (it != ranges::default_sentinel{}){
    ++it;
  }
  return frame;
}


using namespace ranges;
struct getPDBLines_range
    : view_facade<getPDBLines_range, ranges::unknown>
{
private:
  friend range_access;
  std::istream *sin_;
  std::string str_;
  PDBLine pdbLine;
  const PDBRecordTypesBase& db;
  struct cursor
  {
  private:
    getPDBLines_range *rng_;
  public:
    cursor() = default;
    explicit cursor(getPDBLines_range &rng)
        : rng_(&rng)
    {}
    void next()
    {
      rng_->next();
    }
    const PDBLine& read() const noexcept {
      return rng_->pdbLine;
    }

    bool equal(default_sentinel) const
    {
      return !*rng_->sin_;
    }
  };
  void next()
  {
    std::getline(*sin_, str_, '\n');
    if (str_.size()>0){
      pdbLine = PDBLine(str_,db);
    }else{
      pdbLine = PDBLine();
    }
  }
  cursor begin_cursor()
  {
    return cursor{*this};
  }
public:
  getPDBLines_range() = default;
  getPDBLines_range(std::istream &sin, const PDBRecordTypesBase& db)
      : sin_(&sin), str_{}, db(db)
  {
    this->next(); // prime the pump
  }
//  std::string & cached() noexcept
//  {
//    return str_;
//  }
};

struct getPDBLines_fn
{
  getPDBLines_range operator()(std::istream & sin, const PDBRecordTypesBase& db=DefaultPDBRecordTypesBase::instance()) const
  {
    return getPDBLines_range{sin, db};
  }
};
inline namespace function_objects
{
inline constexpr getPDBLines_fn getPDBLines{};
}

}

Frame PDBReader::read_frame() {
  Frame result(0);

  auto pdbLines = getPDBLines(*is);

  std::vector<Frame> frames;

  auto it = ranges::begin(pdbLines);

  while (it!=ranges::default_sentinel{}){
    if (it->getRecordName()==RecordTypeName("MODEL") ||
        it->getRecordName()==RecordTypeName("ATOM") ||
        it->getRecordName()==RecordTypeName("HETATM")){
      return readFrame(it);
    }else{
      ++it;
    }
  }
  return result;
}


std::vector<Frame> PDBReader::read_frames() {

  auto pdbLines = getPDBLines(*is);
  std::vector<Frame> frames;

  auto it = ranges::begin(pdbLines);

  while (it!=ranges::default_sentinel{}){
    if (it->getRecordName()==RecordTypeName("MODEL") ||
        it->getRecordName()==RecordTypeName("ATOM") ||
        it->getRecordName()==RecordTypeName("HETATM")){
      frames.push_back(readFrame(it));
    }else{
      ++it;
    }
  }
  return frames;
}
