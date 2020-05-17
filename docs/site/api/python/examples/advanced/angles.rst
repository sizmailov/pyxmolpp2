AngleValue
^^^^^^^^^^

Protect yourself from missing 2pi/180.0

.. py-exec::
    :context-id: angles

    from pyxmolpp2.geometry import Degrees, Radians, cos, sin, tan, fabs
    import numpy as np


To avoid accidental errors user is forced to use `pyxmolpp2.geometry.AngleValue` instead of raw float numbers

reorder_atoms`pyxmolpp2.geometry.AngleValue` can be constructed via reorder_atoms`pyxmolpp2.geometry.Degrees`
or reorder_atoms`pyxmolpp2.geometry.Radians`:

.. py-exec::
    :context-id: angles

    angle_value_1 = Degrees(45)
    angle_value_2 = Radians(np.pi)


It can be casted back to float as degrees or radians:

.. py-exec::
    :context-id: angles

    print(angle_value_1.degrees, angle_value_1.radians)
    print(angle_value_2.degrees, angle_value_2.radians)


AngleValue supports all basic arithmetic operations:


.. py-exec::
    :context-id: angles

    print((angle_value_1*2 + angle_value_2/3).degrees)

`pyxmolpp2.geometry` also defines `pyxmolpp2.geometry.cos`, `pyxmolpp2.geometry.sin`, `pyxmolpp2.geometry.tan`, `~pyxmolpp2.geometry.fabs` for convenience:

.. py-exec::
    :context-id: angles

    print( cos(angle_value_1),
           sin(angle_value_1),
           tan(angle_value_1),
           fabs(angle_value_1).degrees)

.. py-exec::
    :context-id: angles
    :discard-context:

    print("380 deg casted to range[0..2pi]:", Degrees(380).to_standard_range().degrees )