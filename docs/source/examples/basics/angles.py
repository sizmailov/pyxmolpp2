"""

AngleValue
----------

Protect yourself from missing 2pi/180.0


"""

from pyxmolpp2.geometry import Degrees, Radians, cos, sin, tan, fabs
import numpy as np

##############################################################################
# To avoid accidental errors user is forced to use :py:class:`~pyxmolpp2.geometry.AngleValue` instead of raw float numbers
#
# :py:class:`~pyxmolpp2.geometry.AngleValue` can be constructed via :py:class:`~pyxmolpp2.geometry.Degrees`
# or :py:class:`~pyxmolpp2.geometry.Radians`:

angle_value_1 = Degrees(45)
angle_value_2 = Radians(np.pi)

##############################################################################
# It can be casted back to float as degrees or radians:

print(angle_value_1.degrees, angle_value_1.radians)
print(angle_value_2.degrees, angle_value_2.radians)

##############################################################################
# AngleValue supports all basic arithmetic operations:

print((angle_value_1*2 + angle_value_2/3).degrees)

##############################################################################
# :py:mod:`pyxmolpp2.geometry` also defines :py:func:`~pyxmolpp2.geometry.cos`, :py:func:`~pyxmolpp2.geometry.sin`, :py:func:`~pyxmolpp2.geometry.tan`, :py:func:`~pyxmolpp2.geometry.fabs` for convenience:

print( cos(angle_value_1),
       sin(angle_value_1),
       tan(angle_value_1),
       fabs(angle_value_1).degrees)

print("380 deg casted to range[0..2pi]:", Degrees(380).to_standard_range().degrees )