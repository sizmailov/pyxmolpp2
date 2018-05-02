#include "init.h"
#include "xmol_version_info.h"

void pyxmolpp::version::init_version(pybind11::module& version) {
  using namespace xmol::version;

  std::string v = version_number;
  if (!std::string(git_revision).empty()){
    v += "-" + std::string(git_revision).substr(0,7)
       + "@" + git_branch;
  }

  version.attr("version") = v;
  version.attr("git_revision") = git_revision;
  version.attr("build_datetime") = build_datetime;
  version.attr("build_hostname") = build_hostname;

}