#include "FullscreenPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/FullscreenPassDescription.hpp>

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

void init_FullscreenPassDescription()
{
    register_ptr_to_python<av::Link<av::gua::FullscreenPassDescription>>();

    register_field<av::gua::SFFullscreenPassDescription>("SFFullscreenPassDescription");
    register_multifield<av::gua::MFFullscreenPassDescription>("MFFullscreenPassDescription");
    class_<av::gua::FullscreenPassDescription, av::Link<av::gua::FullscreenPassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>(
        "FullscreenPassDescription", "docstring", no_init)
        .def("set_uniform", &av::gua::FullscreenPassDescription::set_uniform<float>)
        .def("set_uniform", &av::gua::FullscreenPassDescription::set_uniform<int>)
        // .def("set_uniform", &av::gua::FullscreenPassDescription::set_uniform<unsigned>)
        .def("set_uniform", &av::gua::FullscreenPassDescription::set_uniform<bool>)
        .def("set_uniform", &av::gua::FullscreenPassDescription::set_uniform<std::string>)
        .def("set_uniform", &av::gua::FullscreenPassDescription::set_uniform<::gua::math::vec2>)
        .def("set_uniform", &av::gua::FullscreenPassDescription::set_uniform<::gua::math::vec3>)
        .def("set_uniform", &av::gua::FullscreenPassDescription::set_uniform<::gua::math::vec4>)
        .def("set_uniform", &av::gua::FullscreenPassDescription::set_uniform<::gua::math::mat4>);
}
