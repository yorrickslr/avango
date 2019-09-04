#include <avango/python/register_field.h>
#include <boost/python.hpp>

#include <avango/gua/virtual_texturing/VTBackend.hpp>

#include "VTBackend.hpp"

#ifdef AVANGO_VIRTUAL_TEXTURING_SUPPORT

using namespace boost::python;
using namespace av::python;

namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    using type = T;
};
} // namespace python
} // namespace boost

void start_backend(av::gua::VTBackend const& vt_backend) { vt_backend.start_backend(); }

void stop_backend(av::gua::VTBackend const& vt_backend) { vt_backend.stop_backend(); }

void add_camera(av::gua::VTBackend const& vt_backend, av::gua::CameraNode const& camera) { vt_backend.add_camera(camera); }

void set_physical_texture_size(av::gua::VTBackend const& vt_backend, uint32_t size) { vt_backend.set_physical_texture_size(size); }
void set_update_throughput_size(av::gua::VTBackend const& vt_backend, uint32_t size) { vt_backend.set_update_throughput_size(size); }
void set_ram_cache_size(av::gua::VTBackend const& vt_backend, uint32_t size) { vt_backend.set_ram_cache_size(size); }

void init_VTBackend()
{
    register_ptr_to_python<av::Link<av::gua::VTBackend>>();

    class_<av::gua::VTBackend, av::Link<av::gua::VTBackend>, bases<av::FieldContainer>, boost::noncopyable>("VTBackend", "docstring")
        .def("set_physical_texture_size", &set_physical_texture_size)
        .def("set_update_throughput_size", &set_update_throughput_size)
        .def("set_ram_cache_size", &set_ram_cache_size)
        .def("start_backend", &start_backend)
        .def("stop_backend", &stop_backend)
        .def("add_camera", &add_camera);

    register_field<av::gua::SFVTBackend>("SFVTBackend");
    register_multifield<av::gua::MFVTBackend>("MFVTBackend");
}

#endif // AVANGO_VIRTUAL_TEXTURING_SUPPORT