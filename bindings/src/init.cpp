#include "init.h"
#include "xmol/Frame.h"
#include "xmol/proxy/smart/selections.h"
#include "xmol/proxy/smart/spans.h"

#include "TorsionAngle.h"
#include "algo/algo.h"
#include "base.h"
#include "geom/AngleValue.h"
#include "geom/Transformation3d.h"
#include "geom/UnitCell.h"
#include "geom/XYZ.h"
#include "io/AmberNetCDF.h"
#include "io/GromacsXtcFile.h"
#include "io/PdbFile.h"
#include "io/TrjtoolDatFile.h"
#include "pipe/pipe.h"
#include "predicates/predicates.h"
#include "proxy/references.h"
#include "proxy/selections.h"
#include "proxy/spans.h"
#include "trajectory/trajectory.h"

namespace py = pybind11;
using namespace xmol;
using namespace xmol::geom;
using namespace xmol::geom::affine;
using namespace xmol::proxy;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::init(pybind11::module& v1) {

  auto&& pyXYZ = py::class_<XYZ>(v1, "XYZ", "3D Vector");
  auto&& pyAngleValue = py::class_<AngleValue>(v1, "AngleValue", "Angular value");
  auto&& pyUnitCell = py::class_<UnitCell>(v1, "UnitCell", "Unit cell");

  auto pyResidueId = py::class_<ResidueId>(v1, "ResidueId", "Residue id");

  auto pyAtom = py::class_<AtomSmartRef>(v1, "Atom", "Atom reference");
  auto pyResidue = py::class_<ResidueSmartRef>(v1, "Residue", "Residue reference");
  auto pyMolecule = py::class_<MoleculeSmartRef>(v1, "Molecule", "Molecule reference");
  auto pyFrame = py::class_<Frame>(v1, "Frame", "Molecular frame");

  init_predicates(v1);

  auto pyCoordSpan = py::class_<CoordSmartSpan>(v1, "CoordSpan", "Continuous span of atomic coordinate references");
  auto pyAtomSpan = py::class_<AtomSmartSpan>(v1, "AtomSpan", "Continuous span of atom references");
  auto pyResidueSpan = py::class_<ResidueSmartSpan>(v1, "ResidueSpan", "Continuous span of residue references");
  auto pyMoleculeSpan = py::class_<MoleculeSmartSpan>(v1, "MoleculeSpan", "Continuous span of molecule references");

  auto pyCoordSelection = py::class_<CoordSmartSelection>(v1, "CoordSelection", "Ordered set of atomic coordinate references");
  auto pyAtomSelection = py::class_<AtomSmartSelection>(v1, "AtomSelection", "Ordered set of atom references");
  auto pyResidueSelection = py::class_<ResidueSmartSelection>(v1, "ResidueSelection", "Ordered set of residue references");
  auto pyMoleculeSelection = py::class_<MoleculeSmartSelection>(v1, "MoleculeSelection", "Ordered set of molecule references");

  auto pyTranslation = py::class_<Translation3d>(v1, "Translation", "Translational transformation");
  auto pyRotation = py::class_<Rotation3d>(v1, "Rotation", "Rotational transformation");
  auto pyUniformScale = py::class_<UniformScale3d>(v1, "UniformScale", "Uniform scale transformation");
  auto pyTransformation = py::class_<Transformation3d>(v1, "Transformation", "Generic transformation");

  auto pyTrajectory = py::class_<trajectory::Trajectory>(v1, "Trajectory", "Trajectory of frames");
  auto pyTrajectoryInputFile = py::class_<trajectory::TrajectoryInputFile, PyTrajectoryInputFile>(v1, "TrajectoryInputFile", "Trajectory input file ABC");

  auto pyPdbInputFile = py::class_<io::PdbInputFile, trajectory::TrajectoryInputFile>(v1, "PdbFile", "PDB file");
  auto pyTrjtoolDatFile = py::class_<io::TrjtoolDatFile, trajectory::TrajectoryInputFile>(v1, "TrjtoolDatFile", "Trajtool trajectory file");
  auto pyAmberNetCDF = py::class_<io::AmberNetCDF, trajectory::TrajectoryInputFile>(v1, "AmberNetCDF", "Amber trajectory file");
  auto pyGromacsXtc = py::class_<io::GromacsXtcFile, trajectory::TrajectoryInputFile>(v1, "GromacsXtcFile", "Gromacs binary `.xtc` input file");
  auto pyXtcWriter = py::class_<io::xdr::XtcWriter>(v1, "XtcWriter", "Writes frames in `.xtc` binary format");

  py::implicitly_convertible<AtomSmartSpan,AtomSmartSelection>();
  py::implicitly_convertible<ResidueSmartSpan,ResidueSmartSelection>();
  py::implicitly_convertible<MoleculeSmartSpan,MoleculeSmartSelection>();

  populate(pyResidueId);

  populate(pyXYZ);
  populate(pyAngleValue);
  populate(pyUnitCell);

  define_angle_free_functions(v1);

  populate(pyAtom);
  populate(pyResidue);
  populate(pyMolecule);
  populate(pyFrame);

  populate(pyCoordSpan);
  populate(pyAtomSpan);
  populate(pyResidueSpan);
  populate(pyMoleculeSpan);

  populate(pyCoordSelection);
  populate(pyAtomSelection);
  populate(pyResidueSelection);
  populate(pyMoleculeSelection);

  populate(pyTransformation);
  populate(pyTranslation);
  populate(pyRotation);
  populate(pyUniformScale);

  populate(pyTrajectory);
  populate(pyTrajectoryInputFile);

  // underscore in `_pipe` help disambiguate from pure python pyxmolpp2.pipe
  auto pipe = v1.def_submodule("_pipe");
  populate_pipe(pipe);

  populate(pyPdbInputFile);
  populate(pyTrjtoolDatFile);
  populate(pyAmberNetCDF);
  populate(pyGromacsXtc);
  populate(pyXtcWriter);

  define_algo_functions(v1);
  init_TorsionAngle(v1);

  py::register_exception<DeadFrameAccessError>(v1, "DeadFrameAccessError");
  py::register_exception<SpanSplitError>(v1, "SpanSplitError");
  py::register_exception<MultipleFramesSelectionError>(v1, "MultipleFramesSelectionError");
  py::register_exception<CoordSelectionSizeMismatchError>(v1, "CoordSelectionSizeMismatchError");
  py::register_exception<xmol::trajectory::TrajectoryDoubleTraverseError>(v1, "TrajectoryDoubleTraverseError");
  py::register_exception<xmol::geom::GeomError>(v1, "GeomError");
  py::register_exception<xmol::io::XtcReadError>(v1, "XtcReadError");
  py::register_exception<xmol::io::XtcWriteError>(v1, "XtcWriteError");
  py::register_exception<xmol::utils::DeadObserverAccessError>(v1, "DeadObserverAccessError");
}