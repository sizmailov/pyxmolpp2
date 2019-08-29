#include "xmol/amber/NetCDFTrajectoryFile.h"
#include <netcdf.h>

namespace {
inline void check_netcdf_call(int retval, int expected, const char* const nc_funciton_name,
    const char* const caller_function_name) {
    if (retval!=expected) {
        throw std::runtime_error(
            std::string(caller_function_name)+": "+nc_funciton_name+" failed ("+std::to_string(retval)+")");
    }
}
}

xmol::amber::NetCDFTrajectoryFile::NetCDFTrajectoryFile(const std::string& filename)
    :m_filename(filename) {
    this->open();
    this->check();
    this->close();
}

void xmol::amber::NetCDFTrajectoryFile::open() const {
    if (m_is_open) {
        return;
    }
    check_netcdf_call(
        nc_open(m_filename.c_str(), NC_NOWRITE, &ncid), NC_NOERR,
        "nc_open()",
        __FUNCTION_SIGNATURE__
    );
    m_is_open = true;
}

void xmol::amber::NetCDFTrajectoryFile::check() const {
    Ensures(m_is_open);
    if (read_global_string_attr("Conventions").find("AMBER")==std::string::npos) {
        throw std::runtime_error("xmol::amber::NetCDFTrajectoryFile::check(): `"+m_filename+"` does't follow AMBER conventions");
    };
}
xmol::polymer::frameIndex_t xmol::amber::NetCDFTrajectoryFile::n_frames() const {
    this->open();

    int frames_id;
    check_netcdf_call(
        nc_inq_unlimdim(ncid, &frames_id), NC_NOERR,
        "nc_inq_unlimdim()",
        __FUNCTION_SIGNATURE__
    );
    size_t nframes;
    check_netcdf_call(
        nc_inq_dimlen(ncid, frames_id, &nframes), NC_NOERR,
        "nc_inq_dimlen()",
        __FUNCTION_SIGNATURE__
    );
    return nframes;
}
xmol::polymer::atomIndex_t xmol::amber::NetCDFTrajectoryFile::n_atoms_per_frame() const {
    this->open();

    int atomid;
    check_netcdf_call(
        nc_inq_dimid(ncid, "atom", &atomid), NC_NOERR,
        "nc_inq_dimid()",
        __FUNCTION_SIGNATURE__
    );
    size_t natoms;
    check_netcdf_call(
        nc_inq_dimlen(ncid, atomid, &natoms), NC_NOERR,
        "nc_inq_dimlen()",
        __FUNCTION_SIGNATURE__
    );
    return natoms;
}
xmol::amber::NetCDFTrajectoryFile::~NetCDFTrajectoryFile() {
    if (m_is_open) {
        close();
    }
}
void xmol::amber::NetCDFTrajectoryFile::close() {
    if (m_is_open) {
        check_netcdf_call(
            nc_close(ncid), NC_NOERR,
            "nc_close()",
            __FUNCTION_SIGNATURE__
        );
    }
    this->m_is_open = false;
}
std::unique_ptr<xmol::trajectory::TrajectoryPortion> xmol::amber::NetCDFTrajectoryFile::get_copy() const {
    return std::unique_ptr<NetCDFTrajectoryFile>(new NetCDFTrajectoryFile(this->m_filename));
}
bool xmol::amber::NetCDFTrajectoryFile::match(const xmol::polymer::AtomSelection& atoms) const {
    return atoms.size()==this->n_atoms_per_frame();
}
void xmol::amber::NetCDFTrajectoryFile::set_coordinates(xmol::polymer::frameIndex_t frameIndex,
    const xmol::polymer::AtomSelection& atoms, const std::vector<int>& update_list) {
    this->open();
    std::vector<float> tmp;
    tmp.resize(atoms.size()*3);

    size_t start[] = {static_cast<size_t>(frameIndex), 0, 0};
    size_t count[] = {1, static_cast<size_t>(atoms.size()), 3};

    int coords_id;

    nc_inq_varid(ncid, "coordinates", &coords_id);

    check_netcdf_call(
        nc_get_vara_float(ncid, coords_id, start, count, tmp.data()), NC_NOERR,
        "nc_get_vara_float",
        __FUNCTION_SIGNATURE__
    );
    for (auto i: update_list) {
        atoms[i].set_r(xmol::geometry::XYZ(tmp[i*3], tmp[i*3+1], tmp[i*3+2]));
    }
}
void xmol::amber::NetCDFTrajectoryFile::print_info() {
    for (auto attr: {
        "Conventions",
        "ConventionVersion",
        "application",
        "program",
        "programVersion",
        "title",
    }) {
        std::cout << attr << ": `" << read_global_string_attr(attr) << "`" << std::endl;
    }
}
std::string xmol::amber::NetCDFTrajectoryFile::read_global_string_attr(const char* name) const {
    this->open();
    nc_type attr_info;
    size_t attr_len;
    if (nc_inq_att(ncid, NC_GLOBAL, name, &attr_info, &attr_len)!=NC_NOERR) {
        return std::string("<MISSING>");
    }
    char buffer[attr_len];
    check_netcdf_call(
        nc_get_att_text(ncid, NC_GLOBAL, name, buffer), NC_NOERR,
        "nc_get_att_text",
        __FUNCTION_SIGNATURE__
    );
    return std::string(buffer, attr_len);
}

