#include "init.h"

#include <pybind11/stl.h>

#include "xmol/pdb/PdbRecord.h"
#include "xmol/utils/string.h"

#include "PdbRecord.h"

void pyxmolpp::pdb::init_PdbRecords(pybind11::module& pdb) {

  py::class_<xmol::pdb::RecordName>(pdb, "RecordName")
      .def(py::init<const std::string&>(), py::arg("name"))
      .def_property_readonly("str", &xmol::pdb::RecordName::str)
      .def_property_readonly("__hash__", &xmol::pdb::RecordName::value)
      .def("__str__", [](const xmol::pdb::RecordName& name) { return name.str(); })
      .def("__repr__", [](const xmol::pdb::RecordName& name) {
        return "<pyxmolpp2.pdb.RecordName\"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) +
               ">";
      });

  py::class_<xmol::pdb::FieldName>(pdb, "FieldName")
      .def(py::init<const std::string&>(), py::arg("name"))
      .def_property_readonly("str", &xmol::pdb::FieldName::str)
      .def_property_readonly("__hash__", &xmol::pdb::FieldName::value)
      .def("__str__", [](const xmol::pdb::FieldName& name) { return name.str(); })
      .def("__repr__", [](const xmol::pdb::FieldName& name) {
        return "<pyxmolpp2.pdb.FieldName\"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) +
               ">";
      });

  py::class_<xmol::pdb::PdbRecordType>(pdb, "PdbRecordType");

  py::class_<xmol::pdb::basic_PdbRecords, Pybasic_PdbRecords>(pdb, "basic_PdbRecords")
      .def("get_record", &xmol::pdb::basic_PdbRecords::get_record, py::arg("record_name"),
           py::return_value_policy::reference);

  py::class_<xmol::pdb::StandardPdbRecords, xmol::pdb::basic_PdbRecords>(pdb, "StandardPdbRecords")
      .def("get_record", &xmol::pdb::StandardPdbRecords::get_record, py::arg("record_name"),
           py::return_value_policy::reference)
      .def_static("instance", &xmol::pdb::StandardPdbRecords::instance, py::return_value_policy::reference);

  py::class_<xmol::pdb::AlteredPdbRecords, xmol::pdb::basic_PdbRecords>(
      pdb, "AlteredPdbRecords", "Provides flexibility for reading/writing non-standard PDB files")
      .def(py::init<const xmol::pdb::basic_PdbRecords&>(), py::arg("basic"))
      .def("get_record", &xmol::pdb::AlteredPdbRecords::get_record, py::arg("record_name"),
           py::return_value_policy::reference)
      .def("alter_record", &xmol::pdb::AlteredPdbRecords::alter_record, py::arg("record_name"), py::arg("field_name"),
           py::arg("colons"), "Alter/create PDB record field")

      ;
}
