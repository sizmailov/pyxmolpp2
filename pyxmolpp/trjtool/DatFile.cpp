#include "init.h"

#include "xmol/trjtool/DatFile.h"
#include "xmol/trjtool/DATReader.h"

#include <fstream>

using namespace xmol::trjtool;
using namespace xmol::polymer;

void pyxmolpp::trjtool::init_DatFile(py::module& trjtool) {

  py::class_<DatFile,xmol::trajectory::TrajectoryPortion>(trjtool,"DatFile")
      .def(py::init<const std::string&>())
      .def("close",&DatFile::close)
      .def("match",&DatFile::match)
      .def("n_frames",&DatFile::n_frames)
      .def("n_atoms_per_frame",&DatFile::n_atoms_per_frame)
      .def("set_coordinates",(void (DatFile::*)(frameIndex_t, const AtomSelection&))(&DatFile::set_coordinates))
      ;
}
