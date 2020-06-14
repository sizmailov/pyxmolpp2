import pytest

try:
    _ = unicode("123")
except NameError:
    unicode=str
    pass

def test_import_pyxmolpp2():
    import pyxmolpp2

def test_import_pyxmolpp2_v1():
    import pyxmolpp2