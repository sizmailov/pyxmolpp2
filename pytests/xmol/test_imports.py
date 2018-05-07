import pytest

try:
    _ = unicode("123")
except NameError:
    unicode=str
    pass

def test_import_pyxmolpp2():
    import pyxmolpp2

def test_import_polymer():
    import pyxmolpp2.polymer

def test_import_geometry():
    import pyxmolpp2.geometry

def test_print_version():
    import pyxmolpp2.version

    assert isinstance(pyxmolpp2.version.version,(str,unicode))
    assert isinstance(pyxmolpp2.version.git_revision,(str,unicode))
    assert isinstance(pyxmolpp2.version.build_datetime,(str,unicode))
    assert isinstance(pyxmolpp2.version.build_hostname,(str,unicode))
