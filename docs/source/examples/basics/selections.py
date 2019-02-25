"""

.. |Atom| replace:: :py:class:`~pyxmolpp2.polymer.Atom`
.. |Residue| replace:: :py:class:`~pyxmolpp2.polymer.Residue`
.. |Chain| replace:: :py:class:`~pyxmolpp2.polymer.Chain`
.. |Frame| replace:: :py:class:`~pyxmolpp2.polymer.Frame`

.. |AtomSelection| replace:: :py:class:`~pyxmolpp2.polymer.AtomSelection`
.. |ResidueSelection| replace:: :py:class:`~pyxmolpp2.polymer.ResidueSelection`
.. |ChainSelection| replace:: :py:class:`~pyxmolpp2.polymer.ChainSelection`

.. |AtomPredicate| replace:: :py:class:`~pyxmolpp2.polymer.AtomPredicate`
.. |ResiduePredicate| replace:: :py:class:`~pyxmolpp2.polymer.ResiduePredicate`
.. |ChainPredicate| replace:: :py:class:`~pyxmolpp2.polymer.ChainPredicate`



Selections
----------

*Selection* is ordered set of elements in ``pyxmolpp2``. Order is defined as follows

1. if two elements belongs to same parent object, the order match their construction order
2. otherwise, they ordered as their parents
3. |Frame| references are ordered by :py:attr:`~pyxmolpp2.polymer.Frame.index`

"""

import pyxmolpp2
import os

##############################################################################
# Let's create a frame to work with

pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

frame = pdb_file.get_frame()

##############################################################################
# Library has three types of `selections`: |AtomSelection|, |ResidueSelection| and |ChainSelection|
#
# Construction
# ^^^^^^^^^^^^
#
# Any selections might be created from |Frame| instance:

print(frame.asChains)
print(frame.asResidues)
print(frame.asAtoms)

##############################################################################
# |AtomSelection| and |ResidueSelection| can be created from |Chain|:

chain = frame.asChains[0]
print(chain.asResidues)
print(chain.asAtoms)

##############################################################################
# |AtomSelection| can be created from a |Residue|:

residue = frame.asResidues[0]
print(residue.asAtoms)

##############################################################################
# Conversions
# ^^^^^^^^^^^
#
# Selections might be converted up and down through hierarchy:

print(chain.asAtoms.asResidues)  # selects non-empty residues
print(frame.asResidues.asChains)  # selects chains with at least 1 residue
print(frame.asChains.asResidues.asAtoms.asResidues.asChains)  # select chains with at least 1 non-empty residue

##############################################################################
#
# Filter
# ^^^^^^
# ``filter`` method return new selection with elements that match predicate:
#

from pyxmolpp2.polymer import AtomName, ResidueName

chain.asAtoms.filter(lambda a: a.r.x < 0)  # select atoms with negative x coord
chain.asAtoms.filter(lambda a: a.name == AtomName("CA"))  # select CA atoms
chain.asResidues.filter(lambda r: r.name == ResidueName("LYS"))  # select LYS residues

##############################################################################
#
# ``pyxmolpp2`` defines predicate-generators which return predicate when compared to something:
#
from pyxmolpp2.polymer import aName, rName, aId, rId, cName, cIndex

frame.asAtoms.filter(aName == "CA")  # select CA atoms
frame.asResidues.filter(rName == "LYS")  # select LYS residues
frame.asChains.filter(cName == "A")  # select chain(s) A

##############################################################################
# |ChainPredicate| and |ResiduePredicate| can be naturally applied to |AtomSelection|,
# while |AtomPredicate| can be applied only to |AtomSelection|.

frame.asAtoms.filter(aName == "CA")  # select CA atoms
frame.asAtoms.filter(rName == "LYS")  # select all atoms of LYS residues

##############################################################################
# Application of |AtomPredicate| to |ResidueSelection| or |ChainSelection| leads to :py:class:`TypeError` exception.
# Same stands for |ResiduePredicate| and |ChainSelection|:

try:
    print(frame.asResidues.filter(aName == "CA"))
except TypeError as e:
    print(e)
##############################################################################

try:
    print(frame.asChains.filter(aName == "CA"))
except TypeError as e:
    print(e)
##############################################################################

try:
    print(frame.asChains.filter(rName == "LYS"))
except TypeError as e:
    print(e)
##############################################################################


##############################################################################
# Predicates can be combined using ``&``, ``|``, ``~`` operators and reused:

from pyxmolpp2.polymer import AtomPredicate

criteria = (aName == "CA") & rId.is_in({1, 2, 3, 4}) & AtomPredicate(lambda a: a.r.x < 0)  # type:AtomPredicate

print(frame.asAtoms.filter(criteria | cName.is_in({"X", "Y", "Z"})))

##############################################################################
# Set operations
# ^^^^^^^^^^^^^^
#
# Selections support set operations:
#
# - `union` (operators ``+``, ``+=``)
# - `set intersection` (operators ``*``, ``*=``)
# - `difference` (operators ``-``, ``-=``)
#
#
A = frame.asAtoms.filter(lambda a: a.r.x > 5)
B = frame.asAtoms.filter(lambda a: a.r.x <= 5)

print(A)
print(B)
print(A+B)
print(A-B)
print(A*B)

##############################################################################
# Invalidation of selection
# ^^^^^^^^^^^^^^^^^^^^^^^^^
#
# In execution of the program selection may be marked as `invalid`, i.e. further access
# to it's elements raises an exception.
#
# Selection gets invalidated on:
#
# - destruction of any of it's elements parent (Primary this happens on deletion of a whole |Frame|.
#
#   .. note::
#       The exception will be raised so you will know that you are doing something wrong.
#
# - on :py:attr:`~pyxmolpp2.polymer.Frame.index` change if selection had elements from two frames or more.
#
#   .. danger::
#       Currently there is no runtime checks against this type of errors for sake of performance.
#       Please make sure you don't do that

