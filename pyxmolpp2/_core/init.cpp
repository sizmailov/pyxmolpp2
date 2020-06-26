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
#include "io/PdbFile.h"
#include "io/TrjtoolDatFile.h"
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

  auto&& pyXYZ = py::class_<XYZ>(v1, "XYZ");
  auto&& pyAngleValue = py::class_<AngleValue>(v1, "AngleValue");
  auto&& pyUnitCell = py::class_<UnitCell>(v1, "UnitCell");

  auto pyResidueId = py::class_<ResidueId>(v1, "ResidueId");

  auto pyAtom = py::class_<AtomSmartRef>(v1, "Atom");
  auto pyResidue = py::class_<ResidueSmartRef>(v1, "Residue");
  auto pyMolecule = py::class_<MoleculeSmartRef>(v1, "Molecule");
  auto pyFrame = py::class_<Frame>(v1, "Frame");

  init_predicates(v1);

  auto pyCoordSpan = py::class_<CoordSmartSpan>(v1, "CoordSpan");
  auto pyAtomSpan = py::class_<AtomSmartSpan>(v1, "AtomSpan");
  auto pyResidueSpan = py::class_<ResidueSmartSpan>(v1, "ResidueSpan");
  auto pyMoleculeSpan = py::class_<MoleculeSmartSpan>(v1, "MoleculeSpan");

  auto pyCoordSelection = py::class_<CoordSmartSelection>(v1, "CoordSelection");
  auto pyAtomSelection = py::class_<AtomSmartSelection>(v1, "AtomSelection");
  auto pyResidueSelection = py::class_<ResidueSmartSelection>(v1, "ResidueSelection");
  auto pyMoleculeSelection = py::class_<MoleculeSmartSelection>(v1, "MoleculeSelection");

  auto pyTranslation = py::class_<Translation3d>(v1, "Translation");
  auto pyRotation = py::class_<Rotation3d>(v1, "Rotation");
  auto pyUniformScale = py::class_<UniformScale3d>(v1, "UniformScale");
  auto pyTransformation = py::class_<Transformation3d>(v1, "Transformation");

  auto pyTrajectory = py::class_<trajectory::Trajectory>(v1, "Trajectory");
  auto pyTrajectoryInputFile = py::class_<trajectory::TrajectoryInputFile, PyTrajectoryInputFile>(v1, "TrajectoryInputFile");

  auto pyPdbInputFile = py::class_<io::PdbInputFile, trajectory::TrajectoryInputFile>(v1, "PdbFile");
  auto pyTrjtoolDatFile = py::class_<io::TrjtoolDatFile, trajectory::TrajectoryInputFile>(v1, "TrjtoolDatFile");
  auto pyAmberNetCDF = py::class_<io::AmberNetCDF, trajectory::TrajectoryInputFile>(v1, "AmberNetCDF");

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

  populate(pyPdbInputFile);
  populate(pyTrjtoolDatFile);
  populate(pyAmberNetCDF);

  define_algo_functions(v1);
  init_TorsionAngle(v1);

  py::register_exception<DeadFrameAccessError>(v1, "DeadFrameAccessError");
  py::register_exception<SpanSplitError>(v1, "SpanSplitError");
  py::register_exception<MultipleFramesSelectionError>(v1, "MultipleFramesSelectionError");
  py::register_exception<CoordSelectionSizeMismatchError>(v1, "CoordSelectionSizeMismatchError");
  py::register_exception<xmol::trajectory::TrajectoryDoubleTraverseError>(v1, "TrajectoryDoubleTraverseError");
  py::register_exception<xmol::geom::GeomError>(v1, "GeomError");
  py::register_exception<xmol::utils::DeadObserverAccessError>(v1, "DeadObserverAccessError");
}