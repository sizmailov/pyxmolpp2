#include "xmol/io/pdb/PdbWriter.h"
#include "xmol/Frame.h"
#include "xmol/io/pdb/PdbRecord.h"
#include "xmol/proxy/selections.h"

using namespace xmol::io::pdb;
using namespace xmol::io;
using namespace xmol;
using namespace xmol::proxy;

void PdbWriter::write(AtomRef& atom) { this->write(atom, StandardPdbRecords::instance()); }

void PdbWriter::write(ResidueRef& residue) { this->write(residue, StandardPdbRecords::instance()); }

void PdbWriter::write(MoleculeRef& chain) { this->write(chain, StandardPdbRecords::instance()); }

void PdbWriter::write(Frame& frame) { this->write(frame, StandardPdbRecords::instance()); }
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
void PdbWriter::write(AtomRef& atom, const basic_PdbRecords& db) {

  std::string line(pdb_line_width, ' ');

  auto& atom_record = db.get_record(RecordName("ATOM"));

  auto write_field = write_field_t(atom_record, line);

  auto format_atom_name = [](const AtomName& aname) {
    auto s = aname.str();
    if (s.size() < 4) {
      return " "+s;
    }
    return s;
  };

  line.replace(0,6,"ATOM");
  write_field("%d", atom.id(), FieldName("serial"));
  write_field("%-4s", format_atom_name(atom.name()).c_str(), FieldName("name"));
//  write_field("%s", , FieldName("altLoc"));
  write_field("%-s", atom.residue().name().str().c_str(), FieldName("resName"));
  write_field("%-s", atom.residue().molecule().name().str().c_str(), FieldName("chainID"));
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

void PdbWriter::write(ResidueRef& residue, const basic_PdbRecords& db) {
  for (auto& a : residue.atoms()) {
    this->write(a, db);
  }
}
void PdbWriter::write(MoleculeRef& chain, const basic_PdbRecords& db) {
  for (auto& r : chain.residues()) {
    this->write(r, db);
  }
  (*m_ostream) << ("TER\n");
}

void PdbWriter::write(Frame& frame, const basic_PdbRecords& db) {
  if (frame.cell.volume() != 1.0) {
    std::string line(pdb_line_width, ' ');
    line.replace(0, 6, "CRYST1");

    auto& cryst_record = db.get_record(RecordName("CRYST1"));
    auto write_field = write_field_t(cryst_record, line);

    write_field("%9.3f", frame.cell.a(), FieldName("a"));
    write_field("%9.3f", frame.cell.b(), FieldName("b"));
    write_field("%9.3f", frame.cell.c(), FieldName("c"));
    write_field("%7.2f", frame.cell.alpha().degrees(), FieldName("alpha"));
    write_field("%7.2f", frame.cell.beta().degrees(), FieldName("beta"));
    write_field("%7.2f", frame.cell.gamma().degrees(), FieldName("gamma"));

    (*m_ostream) << line << "\n";
  }
  for (auto& c : frame.molecules()) {
    this->write(c, db);
  }
}

void PdbWriter::write(AtomSelection& atomSelection, const basic_PdbRecords& db) {
  for (auto& a : atomSelection) {
    this->write(a, db);
  }
}
void PdbWriter::write(ResidueSelection& residueSelection, const basic_PdbRecords& db) {
  for (auto& r : residueSelection) {
    this->write(r, db);
  }
  (*m_ostream) << ("TER\n");
}

void PdbWriter::write(MoleculeSelection& chainSelection, const basic_PdbRecords& db) {
  for (auto& c : chainSelection) {
    this->write(c, db);
  }
}

void PdbWriter::write(AtomSpan& atomSpan, const basic_PdbRecords& db) {
  for (auto& a : atomSpan) {
    this->write(a, db);
  }
}
void PdbWriter::write(ResidueSpan& residueSpan, const basic_PdbRecords& db) {
  for (auto& r : residueSpan) {
    this->write(r, db);
  }
  (*m_ostream) << ("TER\n");
}

void PdbWriter::write(MoleculeSpan& chainSpan, const basic_PdbRecords& db) {
  for (auto& c : chainSpan) {
    this->write(c, db);
  }
}

