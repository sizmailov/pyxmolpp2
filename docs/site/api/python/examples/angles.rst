AngleValue
^^^^^^^^^^

:ref-prefix:
    pyxmolpp2.AngleValue
    pyxmolpp2

Protect yourself from missing ``2π/180`` factor

.. py-exec::
    :context-id: angles

    from pyxmolpp2 import Degrees, Radians
    import numpy as np


To avoid accidental errors user is forced to use :ref:`AngleValue` instead of raw float numbers

:ref:`AngleValue` can be constructed via :ref:`Degrees` or :ref:`Radians`:

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

:ref:`AngleValue` also has :ref:`cos`, :ref:`sin`, :ref:`tan`, :ref:`fabs` methods for convenience:

.. py-exec::
    :context-id: angles

    print( (angle_value_1).cos(),
           (angle_value_1).sin(),
           (angle_value_1).tan(),
           (angle_value_1).fabs().degrees)

.. py-exec::
    :context-id: angles
    :discard-context:

    print("380 deg casted to range[0..2pi]:", Degrees(380).to_standard_range().degrees )