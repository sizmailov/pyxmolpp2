from __future__ import print_function

import inspect
import os
import re
import pyxmolpp2


class DirectoryWalkerGuard(object):

    def __init__(self, dirname):
        self.dirname = dirname

    def __enter__(self):
        if not os.path.exists(self.dirname):
            os.mkdir(self.dirname)

        assert os.path.isdir(self.dirname)

        os.chdir(self.dirname)

    def __exit__(self, exc_type, exc_val, exc_tb):
        os.chdir(os.path.pardir)


def get_full_class_name(klass):
    return klass.__module__ + "." + klass.__name__


def remove_signature(docstring):
    signature_regex = r"(\s*{name})?\s*\((?P<args>[^\(\)]*)\)\s*->\s*(?P<rtype>[^\(\)]+)\s*".format(name="\w+")
    return "\n".join(filter(lambda line: not re.match(signature_regex,line),docstring.split("\n")))

def get_function_signature(func, strip_module_name=True, module_name=None, klass_name=None):
    name = func.__name__
    try:
        signature_regex = r"(\s*(?P<overload>\d+).)?\s*{name}\s*\((?P<args>[^\(\)]*)\)\s*->\s*(?P<rtype>[^\(\)]+)\s*".format(name=name)
        doc_lines = func.__doc__
        for line in doc_lines.split("\n"):
            if strip_module_name and hasattr(func, "__module__") and func.__module__ is not None:
                    line = line.replace(func.__module__+".", "")
            m = re.match(signature_regex, line)
            if m:
                args = m.group("args")
                rtype = m.group("rtype")
                overload = m.group("overload")
                if overload:
                    full_name = func.__name__
                    print("WARNING:", "Using first found overload for", full_name)
                # print(args, rtype)
                return name, args.replace("::","."), rtype.replace("::",".")
        # print("Can't find pybind11 signature string in __doc__", func.__name__, func.__doc__)
        return None
    except AttributeError:
        return name, "*args, **kwargs", None


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


def write_free_function(func, out):

    name, args, rtype = get_function_signature(func)
    print('''

def {name}({args}) -> {rtype}:
    """
    {docstring}
    """
    pass

'''.format(name=name,args=args,rtype=rtype,docstring=remove_signature(func.__doc__)),
        file=out, end="",
    )


def write_class(klass, out):
    assert inspect.isclass(klass)


    docstrings = []
    fields = []
    properties = []
    methods = []

    for name, member in inspect.getmembers(klass):
        # print(name, "::", member)

        if inspect.ismemberdescriptor(member):
            print("Method desc",  name, "::", member)
        elif inspect.isroutine(member):
            methods.append(member)
        elif inspect.ismemberdescriptor(member):
            print("mem decs",  name, "::", member)

        elif isinstance(member,property):
            # print("property", name, "::", member)
            properties.append((name,member))
        elif name == "__doc__":
            if member is not None:
                docstrings.append(member)
        elif name in ["__class__","__module__"]:
            pass
        else:
            print("Unknown type", name, "::", member)

    out.write("""
class {class_name}({base_class}):    
""".format(class_name=klass.__name__, base_class="object"))

    # write properties
    for doc in docstrings:
        out.write('''
    """{doc}"""
'''.format(doc=doc))


    for field in fields:
        print("FIELD", field.__doc__)

    # write properties
    for prop_name, prop in properties:

        T1, T2 = get_property_type(prop, module_name=klass.__module__)

        out.write('''
    @property
    def {field_name}(self) -> {T1}:
        """
        {docstring}
        """
        pass
'''.format(field_name=prop_name, T1=T1,docstring=remove_signature(prop.fget.__doc__)))

        if T2 != "None":
            out.write('''
    @{field_name}.setter
    def {field_name}({T2}) -> None:
        """
        {docstring}
        """
        pass
'''.format(field_name=prop_name, T2=T2, docstring=remove_signature(prop.fset.__doc__)))


    # write bound methods
    for method in methods:
        signature = get_function_signature(method)
        if signature:
            name, args, rtype = signature
            static_method_kw = ""
            if args.strip()=="":
                static_method_kw = "\n    @staticmethod"
            print("""{static_method_kw}
    def {name}({args}) -> {rtype}:
        pass
""".format(name=name, args=args, rtype=rtype, static_method_kw=static_method_kw),
      file=out, end=""
      )

    if len(methods)+len(fields)+len(properties)==0:
        out.write("""
    pass
""")

def write_stubs_for_module(module, main_module=None):
    if main_module is None:
        main_module = module
    print("Processing", module)

    with DirectoryWalkerGuard(module.__name__.split(".")[-1]):
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
                print("Unknown type", name,"::",member)

        with open("__init__.pyi", "w") as init:
            # write docstring if exists
            for doc in docstrings:
                init.write('"""%s"""' % doc)

            init.write("\n\n")


            if main_module is not module:
                init.write("import %s\n"%main_module.__name__)
                init.write("\n\n")

            # support `from module import *` syntax
            init.write("__all__ = [\n    ")
            init.write(",\n    ".join(list(map(lambda submodule: "\"%s\""%submodule.__name__.split('.')[-1],submodules))
                                    + list(map(lambda klass: "\"%s\""%klass.__name__,classes))
                                    + list(map(lambda func: "\"%s\""%func.__name__,free_functions))
                                      ))
            init.write("\n    ]\n")


            init.write("\n\n")

            # write classes
            for klass in classes:
                write_class(klass, init)

            # write free functions
            for func in free_functions:
                write_free_function(func, init)

            # declare variables

        for submodule in submodules:
            write_stubs_for_module(submodule, main_module=main_module)


path="./stubs"

if not os.path.exists(path):
    os.mkdir(path)

with DirectoryWalkerGuard(path):
    write_stubs_for_module(module=pyxmolpp2)


