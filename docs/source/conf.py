# -*- coding: utf-8 -*-
#
# Configuration file for the Sphinx documentation builder.
#
# This file does only contain a selection of the most common options. For a
# full list see the documentation:
# http://www.sphinx-doc.org/en/master/config

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = u'pyxmolpp2'
copyright = u'2018, Sergei Izmailov'
author = u'Sergei Izmailov'

# The short X.Y version
version = u''
# The full version, including alpha/beta/rc tags
release = u'0.0.1'


# -- General configuration ---------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx_autodoc_annotation',
]

autosummary_generate = True


# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

# The master toctree document.
master_doc = 'index'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path .
exclude_patterns = []

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
# html_theme_options = {}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# Custom sidebar templates, must be a dictionary that maps document names
# to template names.
#
# The default sidebars (for documents that don't match any pattern) are
# defined by theme itself.  Builtin themes are using these templates by
# default: ``['localtoc.html', 'relations.html', 'sourcelink.html',
# 'searchbox.html']``.
#
# html_sidebars = {}


# -- Options for HTMLHelp output ---------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = 'pyxmolpp2doc'


# -- Options for LaTeX output ------------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    #
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    #
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    #
    # 'preamble': '',

    # Latex figure (float) alignment
    #
    # 'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    (master_doc, 'pyxmolpp2.tex', u'pyxmolpp2 Documentation',
     u'Sergei Izmailov', 'manual'),
]


# -- Options for manual page output ------------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, 'pyxmolpp2', u'pyxmolpp2 Documentation',
     [author], 1)
]


# -- Options for Texinfo output ----------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, 'pyxmolpp2', u'pyxmolpp2 Documentation',
     author, 'pyxmolpp2', 'One line description of project.',
     'Miscellaneous'),
]


# -- Extension configuration -------------------------------------------------

autodoc_default_flags = ['members','undoc-members', 'show-inheritance','special-members']
autoclass_content = "both"


def strip_argumet_types(app=None, what=None, name=None, obj=None, options=None, arg=None, ret=None):
    import re
    # print(arg,ret)
    if what in ["method","function"]:

        arg_old = arg
        # print(arg_old)
        arg_new = re.sub(r"\[[^\]\[]*\]","",arg_old)
        while arg_new!=arg_old:
            # print(arg_new)
            arg_old = arg_new
            arg_new = re.sub(r"\[[^\]\[]*\]","",arg_old)

        # print(arg_new)

        arg = re.sub(r"([\w_]+):\s*[^,\)\(=]*",r"\g<1>", arg_new)

    if ret=="None":
        ret = None

    # if ret is not None:
    #     ret = ret.split["."][-1]

    return arg,ret

def add_types_to_function(objtype, sig, docstringlines):
    import re

    args_with_decs = set(("self",))
    args_with_type = set()
    for l in docstringlines:
        m = re.match("\s*:param\s+(?P<arg>\w+)\s*:.*",l)
        if m:
            args_with_type.add(m.group("arg"))
        m = re.match("\s*:type\s+(?P<arg>\w+)\s*:.*",l)
        if m:
            args_with_decs.add(m.group("arg"))
    m = re.match(r".*\((?P<args>[^\(\)]*)\)"
                 r"(\s*->\s*(?P<ret>\w[\[\],\w\.\s]+))?\s*",sig)
    if m:
        args, ret = m.group("args"), m.group("ret")

        braket_counter = 0
        aargs = []
        begin = 0
        for i,a in enumerate(args):
            if a == "[": braket_counter+=1
            if a == "]": braket_counter-=1
            if a == "," and braket_counter==0:
                aargs.append(args[begin:i])
                begin=i+1
        aargs.append(args[begin:])
        for arg in aargs:
            if ":" in arg:
                arg_name, arg_type = arg.split(":")

                arg_name = arg_name.strip()
                arg_type = arg_type.strip()

                if arg_name in args_with_type:
                    continue

                if arg_name not in args_with_decs:
                    line = ":param {}: {}".format(arg_name,"not documented yet")
                    docstringlines.append(line)

                if "[" not in arg_type:
                    arg_type = "~"+arg_type
                line = ":type {}: :py:class:`{}`".format(arg_name,arg_type)
                docstringlines.append(line)

        if ret is not None and ret.strip() !="None":
            ret = ret.strip()
            if "[" not in ret:
                ret = "~"+ret
            docstringlines.append(":rtype: :py:class:`{}`".format(ret))


def add_types_to_overloaded_function(objtype, f,docstringlines):
    import re

    overloads = []

    if objtype=="class":
        lines = f.__init__.__doc__.split("\n")
    else:
        lines = f.__doc__.split("\n")
    end = len(lines)
    fname = None
    for i, l in reversed(list(enumerate(lines))):
        m = re.match(r"\d+\.\s*(?P<fname>\w+)\s*\((?P<args>[^\(\)]*)\)"
                 r"(\s*->\s*(?P<ret>\w[\[\],\w\.\s]+))?\s*",l)
        if m:
            args, ret = m.group("args"), m.group("ret")
            args, ret = strip_argumet_types(what="method", arg=args,ret=ret)
            overloads = [(l, args, ret, lines[i+1:end])] + overloads
            fname = m.group("fname")
            end = i

    new_docs = [".. note:: There are several overloads for :py:meth:`{}`".format(fname),""]

    docstringlines[:] = new_docs

    for sig, args, ret, ll in overloads:
        add_types_to_function(objtype,sig,ll)
        docstringlines.append(".. py:method:: {}({args})".format(fname,args=args))
        docstringlines.append('')
        docstringlines.extend(ll)
        docstringlines.append('')

    # print("\n".join(docstringlines))


    # print(overloads)


    pass

def process_docs(app, objtype, fullname, object, options, docstringlines):
    import re
    sig = None

    try:
        sig = object.__doc__.split("\n")[0]
    except (KeyError,AttributeError,IndexError):
        pass

    if sig:

        is_overloaded = False

        for l in docstringlines:
            if "Overloaded function." in l:
                is_overloaded = True

        if is_overloaded:
            add_types_to_overloaded_function(objtype,object, docstringlines)
        else:
            add_types_to_function(objtype, sig, docstringlines)

    return None

def setup(app):
    app.connect('autodoc-process-signature', strip_argumet_types)
    app.connect('autodoc-process-docstring', process_docs)
