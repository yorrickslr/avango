#include "SkyMapPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/SkyMapPassDescription.hpp>

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

void init_SkyMapPassDescription()
{
    register_ptr_to_python<av::Link<av::gua::SkyMapPassDescription>>();
    register_field<av::gua::SFSkyMapPassDescription>("SFSkyMapPassDescription");
    register_multifield<av::gua::MFSkyMapPassDescription>("MFSkyMapPassDescription");
    class_<av::gua::SkyMapPassDescription, av::Link<av::gua::SkyMapPassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>("SkyMapPassDescription", "docstring", no_init);
}
