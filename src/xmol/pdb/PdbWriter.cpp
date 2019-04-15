#include "xmol/pdb/PdbWriter.h"
#include "xmol/pdb/PdbRecord.h"

using namespace xmol::pdb;
using namespace xmol::polymer;

void PdbWriter::write(const xmol::polymer::Atom& atom) {
  this->write(atom,StandardPdbRecords::instance());
}

void PdbWriter::write(const xmol::polymer::Residue& residue) {
  this->write(residue,StandardPdbRecords::instance());
}

void PdbWriter::write(const xmol::polymer::Chain& chain) {
  this->write(chain,StandardPdbRecords::instance());
}

void PdbWriter::write(const xmol::polymer::Frame& frame) {
  this->write(frame,StandardPdbRecords::instance());
}
namespace {
  const int pdb_line_width = 80;

  struct write_field_t{
    write_field_t(const PdbRecordType& record,std::string& line):record(record),line(line){};
    template<typename T>
    void operator()(const char* const fmt, const T& value, const FieldName& field){
      const std::vector<int>& colons = record.getFieldColons(field);
      const int first = colons[0]-1;
      const int last = colons[1]-1;
      assert(first<=last);
      assert(last<pdb_line_width);
      int bytes_to_write = last+1-first;
      char buff[bytes_to_write+2];
      int w = std::snprintf(buff,bytes_to_write+1,fmt, value);
      line.replace(last-w+1,w, buff);
    };
    const PdbRecordType& record;
    std::string& line;
  };
}
void PdbWriter::write(const xmol::polymer::Atom& atom, const xmol::pdb::basic_PdbRecords& db) {

  std::string line(pdb_line_width,' ');

  auto& atom_record = db.get_record(RecordName("ATOM"));

  auto write_field = write_field_t(atom_record,line);

  auto format_atom_name  = [](const AtomName& aname){
    auto s = aname.str();
    if (s.size()<4){
      return " "+s;
    }
    return s;
  };

  line.replace(0,6,"ATOM");
  write_field("%d", atom.id(), FieldName("serial"));
  write_field("%-4s", format_atom_name(atom.name()).c_str(), FieldName("name"));
//  write_field("%s", , FieldName("altLoc"));
  write_field("%-s", atom.residue().name().str().c_str(), FieldName("resName"));
  write_field("%-s", atom.residue().chain().name().str().c_str(), FieldName("chainID"));
  write_field("%-d", atom.residue().id().serial, FieldName("resSeq"));
  write_field("%s", atom.residue().id().iCode.str().c_str(), FieldName("iCode"));
  write_field("%8.3f", atom.r().x(), FieldName("x"));
  write_field("%8.3f", atom.r().y(), FieldName("y"));
  write_field("%8.3f", atom.r().z(), FieldName("z"));
//  write_field("%s", , FieldName("occupancy"));
//  write_field("%s", , FieldName("tempFactor"));
//  write_field("%s", , FieldName("element"));


  (*m_ostream) << line <<"\n";
}

void PdbWriter::write(const xmol::polymer::Residue& residue, const xmol::pdb::basic_PdbRecords& db) {
  for(auto&a: residue.asAtoms()){
    this->write(a,db);
  }
}
void PdbWriter::write(const xmol::polymer::Chain& chain, const xmol::pdb::basic_PdbRecords& db) {
  for(auto&r: chain.asResidues()){
    this->write(r,db);
  }
  (*m_ostream) << ("TER\n");
}

void PdbWriter::write(const xmol::polymer::Frame& frame, const xmol::pdb::basic_PdbRecords& db) {
  for(auto&c: frame.asChains()){
    this->write(c,db);
  }
}


void PdbWriter::write(const xmol::polymer::ConstAtomSelection& atomSelection, const xmol::pdb::basic_PdbRecords& db) {
  for(auto&a: atomSelection){
    this->write(a,db);
  }
}
void PdbWriter::write(const xmol::polymer::ConstResidueSelection& residueSelection, const xmol::pdb::basic_PdbRecords& db) {
  for(auto&r: residueSelection){
    this->write(r,db);
  }
  (*m_ostream) << ("TER\n");
}

void PdbWriter::write(const xmol::polymer::ConstChainSelection & chainSelection, const xmol::pdb::basic_PdbRecords& db) {
  for(auto&c: chainSelection){
    this->write(c,db);
  }
}


void PdbWriter::write(const xmol::polymer::AtomSelection& atomSelection, const xmol::pdb::basic_PdbRecords& db) {
  for(auto&a: atomSelection){
    this->write(a,db);
  }
}
void PdbWriter::write(const xmol::polymer::ResidueSelection& residueSelection, const xmol::pdb::basic_PdbRecords& db) {
  for(auto&r: residueSelection){
    this->write(r,db);
  }
  (*m_ostream) << ("TER\n");
}

void PdbWriter::write(const xmol::polymer::ChainSelection & chainSelection, const xmol::pdb::basic_PdbRecords& db) {
  for(auto&c: chainSelection){
    this->write(c,db);
  }
}

