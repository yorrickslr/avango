#include "Material.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Material.hpp>

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

list list_uniforms(av::gua::Material& mat)
{
    list result;
    auto const& uniforms = mat.getGuaMaterial()->get_uniforms();

    for(auto& uniform : uniforms)
    {
        result.append(uniform.first);
    }
    return result;
}

void init_Material()
{
    register_ptr_to_python<av::Link<av::gua::Material>>();

    register_field<av::gua::SFMaterial>("SFMaterial");
    register_multifield<av::gua::MFMaterial>("MFMaterial");
    class_<av::gua::Material, av::Link<av::gua::Material>, bases<av::FieldContainer>, boost::noncopyable>("Material", "docstring", no_init)
        .def("list_uniforms", list_uniforms)
        .def("reset_view_uniform", &av::gua::Material::reset_view_uniform)
        .def("set_uniform", &av::gua::Material::set_uniform<float>)
        .def("set_uniform", &av::gua::Material::set_uniform<int>)
        // .def("set_uniform", &av::gua::Material::set_uniform<unsigned>)
        .def("set_uniform", &av::gua::Material::set_uniform<bool>)
        .def("set_uniform", &av::gua::Material::set_uniform<std::string>)
        .def("set_uniform", &av::gua::Material::set_uniform<::gua::math::vec2>)
        .def("set_uniform", &av::gua::Material::set_uniform<::gua::math::vec3>)
        .def("set_uniform", &av::gua::Material::set_uniform<::gua::math::vec4>)
        .def("set_uniform", &av::gua::Material::set_uniform<::gua::math::mat4>)
        .def("set_view_uniform", &av::gua::Material::set_view_uniform<float>)
        .def("set_view_uniform", &av::gua::Material::set_view_uniform<int>)
        // .def("set_view_uniform", &av::gua::Material::set_view_uniform<unsigned>)
        .def("set_view_uniform", &av::gua::Material::set_view_uniform<bool>)
        .def("set_view_uniform", &av::gua::Material::set_view_uniform<std::string>)
        .def("set_view_uniform", &av::gua::Material::set_view_uniform<::gua::math::vec2>)
        .def("set_view_uniform", &av::gua::Material::set_view_uniform<::gua::math::vec3>)
        .def("set_view_uniform", &av::gua::Material::set_view_uniform<::gua::math::vec4>)
        .def("set_view_uniform", &av::gua::Material::set_view_uniform<::gua::math::mat4>)

        .def("get_mat4", &av::gua::Material::get_mat4);
}
