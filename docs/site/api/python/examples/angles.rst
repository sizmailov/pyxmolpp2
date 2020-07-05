AngleValue
^^^^^^^^^^

:ref-prefix:
    pyxmolpp2.AngleValue
    pyxmolpp2

This class protects from sudden loss of ``2π/180`` factor

.. py-exec::
    :context-id: angles

    from pyxmolpp2 import Degrees, Radians
    import numpy as np


To avoid accidental errors use of :ref:`AngleValue` is enforced by API.

:ref:`AngleValue` can be constructed via :ref:`Degrees` or :ref:`Radians`:

.. py-exec::
    :context-id: angles

    a = Degrees(45)
    b = Radians(np.pi)


It can be casted back to float as degrees or radians:

.. py-exec::
    :context-id: angles

    print(a.degrees, b.radians)
    print(b.degrees, b.radians)


All basic arithmetic operations are supported:


.. py-exec::
    :context-id: angles

    print((a * 2 + b / 3).degrees)

:ref:`AngleValue` also has :ref:`cos`, :ref:`sin`, :ref:`tan`, :ref:`abs` methods for convenience:

.. py-exec::
    :context-id: angles

    print(a.cos(), a.sin(), a.tan(), a.abs().degrees)

Sometimes we don't need extra periodic cycles and want our angle in :math:`[-\pi .. \pi)` range.
:ref:`to_standard_range` functions would help you.

.. py-exec::
    :context-id: angles
    :discard-context:

    print("380 to [-180..180) range:",
          Degrees(380).to_standard_range().degrees)