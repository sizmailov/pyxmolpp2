"""


.. |Atom| replace:: :py:class:`~pyxmolpp2.polymer.Atom`
.. |Residue| replace:: :py:class:`~pyxmolpp2.polymer.Residue`
.. |Chain| replace:: :py:class:`~pyxmolpp2.polymer.Chain`
.. |Frame| replace:: :py:class:`~pyxmolpp2.polymer.Frame`

.. |AtomSelection| replace:: :py:class:`~pyxmolpp2.polymer.AtomSelection`
.. |ResidueSelection| replace:: :py:class:`~pyxmolpp2.polymer.ResidueSelection`
.. |ChainSelection| replace:: :py:class:`~pyxmolpp2.polymer.ChainSelection`



Frame/Chain/Residue/Atom hierarchy
----------------------------------


The `pyxmolpp2` library implements |Frame|/|Chain|/|Residue|/|Atom| hierarchy to represent a molecular system.

Every |Atom| exists as a part of some |Residue|. |Residue| is always a part of |Chain|. |Chain| is always a part of |Frame|.

For instance this implies that you can not create an |Atom| without pre-existed |Residue|.
Why not allow ``Atom.residue`` to be :py:class:`None` if Atom exists by it's own?
The choice was between flexibility of construction code versus complexity of it's further usage.
The rationale behind that design decision is fact that expression ``atom.residue.chain.frame.index``
is always correct, and no `not-None` checks are required. Such checks across the library and user code would
increases it's complexity and make it more prone to errors.



.. danger::
    |Atom|/|Residue|/|Chain| is alive until it's |Frame| exists. |Frame| exists until python holds a reference to it.
    No frame - no game.


"""


import pyxmolpp2
import os

pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

frame = pdb_file.get_frame()

##############################################################################

# print chain names of 1UBQ entry
print([ chain.name.str for chain in frame.asChains ])

##############################################################################

# print residue names of 1UBQ entry
print([ res.name.str for res in frame.asResidues])

##############################################################################
# print info about first atom:
a = frame.asAtoms[0]
from tabulate import tabulate

print(tabulate([
    ("name", a.name),
    ("id", a.id),
    ("[x,y,z]", a.r),
    ("rId", a.rId),
    ("rName", a.rName),
    ("cName", a.cName),
]))

##############################################################################
# we can find frame by atom
assert a.frame == frame

##############################################################################
# If you destroy all references to frame it will be eliminated with all it's content

asel = frame.asAtoms    # valid as long reference to frame exists
rsel = frame.asResidues # valid as long reference to frame exists
csel = frame.asChains   # valid as long reference to frame exists
a = frame.asAtoms[0]    # valid as long reference to frame exists

##############################################################################
# Let's `accidentally` destroy frame, by dropping only reference:
frame = None


##############################################################################
try:
    print(asel[0])
except pyxmolpp2.polymer.DeadAtomSelectionAccess as e:
    print("AtomSelection error:")
    print(e)
    pass

##############################################################################
try:
    print(rsel[0])
except pyxmolpp2.polymer.DeadResidueSelectionAccess as e:
    print("ResidueSelection error:")
    print(e)
    pass

##############################################################################
try:
    print(csel[0])
except pyxmolpp2.polymer.DeadChainSelectionAccess as e:
    print("ChainSelection error:")
    print(e)
    pass

##############################################################################



##############################################################################
try:
    print(a.name)
except RuntimeError as e:
    print("Atom access error:")
    print(e)
    pass