#include "init.h"
#include "xmol/Frame.h"
#include "xmol/proxy/smart/selections.h"
#include "xmol/proxy/smart/spans.h"

#include "v1/proxy/references.h"
#include "v1/proxy/spans.h"
#include "v1/proxy/selections.h"
#include "v1/geom/XYZ.h"
#include "v1/geom/Transformation3d.h"

namespace py = pybind11;
using namespace xmol;
using namespace xmol::geom;
using namespace xmol::geom::affine;
using namespace xmol::proxy;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::init(pybind11::module& v1) {

  auto&& pyXYZ = py::class_<XYZ>(v1, "XYZ");

  auto pyAtomName = py::class_<AtomName>(v1, "AtomName");
  auto pyResidueName = py::class_<ResidueName>(v1, "ResidueName");
  auto pyResidueId = py::class_<ResidueId>(v1, "ResidueId");
  auto pyResidueInsertionCode = py::class_<ResidueInsertionCode>(v1, "ResidueInsertionCode");
  auto pyMoleculeName = py::class_<MoleculeName>(v1, "MoleculeName");

  auto pyAtom = py::class_<AtomSmartRef>(v1, "Atom");
  auto pyResidue = py::class_<ResidueSmartRef>(v1, "Residue");
  auto pyMolecule = py::class_<MoleculeSmartRef>(v1, "Molecule");
  auto pyFrame = py::class_<Frame>(v1, "Frame");

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

  populate(pyXYZ);

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

}