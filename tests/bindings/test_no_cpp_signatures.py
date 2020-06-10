
import inspect
import os
import re
import pyxmolpp2


def get_full_class_name(klass):
    return klass.__module__ + "." + klass.__name__


def has_no_cpp_type_in_signature(docstring):
    if docstring is None: return True
    signature_regex = r"(\s*\d+\.)?(\s*{name})?\s*\((?P<args>[^\(\)]*)\)\s*->\s*(?P<rtype>[^\(\)]+)\s*".format(name="\w+")
    signatures = "\n".join(filter(lambda line: re.match(signature_regex,line),docstring.split("\n")))
    result = signatures.find("::") == -1
    return result


def get_function_signature(func, strip_module_name=True, module_name=None, klass_name=None):
    name = func.__name__
    try:
        signature_regex = r"(\s*(?P<overload>\d+).)?\s*{name}\s*\((?P<args>[^\(\)]*)\)\s*->\s*(?P<rtype>[^\(\)]+)\s*".format(name=name)
        doc_lines = func.__doc__
        signatures = []
        for line in doc_lines.split("\n"):
            if strip_module_name and hasattr(func, "__module__") and func.__module__ is not None:
                line = line.replace(func.__module__+".", "")
            m = re.match(signature_regex, line)
            if m:
                args = m.group("args")
                rtype = m.group("rtype")
                overload = m.group("overload")
                signatures.append((name, args, rtype))
                # print(args, rtype)

        return signatures
    except AttributeError:
        return [(name, "*args, **kwargs", None)]


def get_property_type(prop, module_name):

    getter_rtype = "None"
    setter_args = "None"

    strip_module_name = module_name is not None

    for line in prop.fget.__doc__.split("\n"):
        if strip_module_name:
            line = line.replace(module_name + ".", "")
        m = re.match(r"\s*\((?P<args>[^\(\)]*)\)\s*->\s*(?P<rtype>[^\(\)]+)\s*", line)
        if m:
            getter_rtype = m.group("rtype")
            break

    if prop.fset is not None:
        for line in prop.fset.__doc__.split("\n"):
            if strip_module_name:
                line = line.replace(module_name + ".", "")
            m = re.match(r"\s*\((?P<args>[^\(\)]*)\)\s*->\s*(?P<rtype>[^\(\)]+)\s*", line)
            if m:
                args = m.group("args")
                # replace first argument with self
                setter_args = ",".join(["self"]+args.split(",")[1:])
                break

    return getter_rtype, setter_args


def check_free_function(func):
    assert has_no_cpp_type_in_signature(func.__doc__)


def check_class(klass):
    assert inspect.isclass(klass)


    docstrings = []
    fields = []
    properties = []
    methods = []

    for name, member in inspect.getmembers(klass):
        # print(name, "::", member)

        if inspect.ismemberdescriptor(member):
            pass  # print("Method desc",  name, "::", member)
        elif inspect.isroutine(member):
            methods.append(member)
        elif inspect.ismemberdescriptor(member):
            pass  # print("mem decs",  name, "::", member)

        elif isinstance(member,property):
            # print("property", name, "::", member)
            properties.append((name,member))
        elif name == "__doc__":
            if member is not None:
                docstrings.append(member)
        elif name in ["__class__","__module__"]:
            pass
        else:
            pass # print("Unknown type", name, "::", member)


    for field in fields:
        #print("FIELD", field.__doc__)
        pass

    # write properties
    for prop_name, prop in properties:

        T1, T2 = get_property_type(prop, module_name=klass.__module__)

        assert has_no_cpp_type_in_signature(prop.fget.__doc__)

        if T2 != "None":
            assert has_no_cpp_type_in_signature(prop.fset.__doc__)


    for method in methods:
        signatures = get_function_signature(method)
        for sign in signatures:
            assert has_no_cpp_type_in_signature(method.__doc__)


def check_module(module, main_module=None):
    if main_module is None:
        main_module = module
    #print("Processing", module)


    docstrings = []
    free_functions = []
    classes = []
    submodules = []

    for name, member in inspect.getmembers(module):
        # print(name, "::", member)
        if inspect.ismodule(member):
            submodules.append(member)
        elif inspect.isbuiltin(member) or inspect.isfunction(member):
            free_functions.append(member)
        elif inspect.isclass(member) or inspect.isclass(member):
            classes.append(member)
        elif name == "__doc__":
            docstrings.append(member)
        elif name in ["__file__", "__loader__", "__name__", "__package__", "__spec__"]:
            pass
        else:
            pass # print("Unknown type", name,"::",member)

    for klass in classes:
        check_class(klass)

    for func in free_functions:
        check_free_function(func)

    # declare variables

    for submodule in submodules:
        check_module(submodule, main_module=main_module)

def test_no_cpp_types_in_signatures():
    check_module(module=pyxmolpp2)


