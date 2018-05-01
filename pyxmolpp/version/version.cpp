#include "init.h"
#include "xmol_version_info.h"

void pyxmolpp::version::init_version(pybind11::module& version) {
  using namespace xmol::version;
  version.attr("version") =  std::string(version_number)
      + "-" + std::string(git_revision).substr(0,7)
      + "@" + git_branch;
  version.attr("git_revision") = git_revision;
  version.attr("build_datetime") = build_datetime;
  version.attr("build_hostname") = build_hostname;

}