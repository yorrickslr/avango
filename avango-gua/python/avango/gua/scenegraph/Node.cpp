#include "Node.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/Node.hpp>

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

void init_Node()
{
    register_ptr_to_python<av::Link<av::gua::Node>>();

    register_field<av::gua::SFNode>("SFNode");
    register_multifield<av::gua::MFNode>("MFNode");
    class_<av::gua::Node, av::Link<av::gua::Node>, bases<av::FieldContainer>, boost::noncopyable>("Node", "docstring", no_init);
}
