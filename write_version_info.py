import subprocess
import time
import sys
import socket

def get_version_info():


    try:
        git_revision = subprocess.check_output(["git", "rev-parse", "HEAD"]).split("\n")[0]
        git_branch = subprocess.check_output(["git", "rev-parse","--abbrev-ref", "HEAD"]).split("\n")[0]
    except (subprocess.CalledProcessError, OSError):
        git_revision = ""
        git_branch = "non-git"

    build_datetime = time.strftime("%a, %d %b %Y %H:%M:%S +0000", time.gmtime())
    version_number = open("VERSION").readline()

    hostname = socket.gethostname()

    return git_revision, git_branch, build_datetime, version_number, hostname

if __name__ =="__main__":

    output_file = sys.argv[1]
    with open(output_file, "w") as fout:
        fout.write("""#pragma once 

namespace xmol{{
namespace version{{

auto constexpr git_revision = "{0}";
auto constexpr git_branch = "{1}";
auto constexpr build_datetime = "{2}";
auto constexpr version_number = "{3}";
auto constexpr build_hostname = "{4}";


}}
}}
        
""".format(*get_version_info()))

