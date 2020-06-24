#pragma once

#include "xmol/io/pdb/PdbWriter.h"
#include <fstream>

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <xmol/io/pdb/PdbRecord.h>

namespace pyxmolpp::v1 {

template <typename Element> void to_pdb_file(Element& element, std::string& path) {
  std::ofstream out(path);
  if (out.fail()) {
    throw std::runtime_error("Can't open file `" + path + "` for writing"); // toto: replace with IOError
  }
  xmol::io::pdb::PdbWriter writer(out);
  writer.write(element, xmol::io::pdb::StandardPdbRecords::instance());
}

template <typename Element> void to_pdb_stream(Element& element, pybind11::object& fileHandle) {

  if (!(pybind11::hasattr(fileHandle, "write") && pybind11::hasattr(fileHandle, "flush"))) {
    throw pybind11::type_error(
        "Frame.to_pdb(file): incompatible function argument: `file` must be a file-like object, but `" +
        (std::string)(pybind11::repr(fileHandle)) + "` provided");
  }
  pybind11::detail::pythonbuf buf(fileHandle);
  std::ostream stream(&buf);
  xmol::io::pdb::PdbWriter writer(stream);
  writer.write(element, xmol::io::pdb::StandardPdbRecords::instance());
}

} // namespace pyxmolpp::v1