// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/osg/Node.h>
#include "OSGNode.h"

#include <osg/NodeVisitor>
#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/MatrixTransform>
#include <osg/Billboard>
#include <osg/ComputeBoundsVisitor>

#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>

using namespace boost::python;
using namespace av::python;

namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    typedef T type;
};
} // namespace python
} // namespace boost

namespace
{
osg::BoundingBox CalcBoundingBox(av::osg::Node* node)
{
    ::osg::ComputeBoundsVisitor cbbv(::osg::NodeVisitor::TRAVERSE_ACTIVE_CHILDREN);
    node->getOsgNode()->accept(cbbv);
    return cbbv.getBoundingBox();
}

void WriteNodeFile(av::osg::Node* node, std::string filename) { osgDB::writeNodeFile(*(node->getOsgNode()), filename); }

void OptimizeSceneGraph(av::osg::Node* node)
{
    osgUtil::Optimizer optimizer;
    optimizer.optimize(node->getOsgNode());
}

void RecalculateSmoothNormals(av::osg::Node* node)
{
    osgUtil::SmoothingVisitor sv;
    node->getOsgNode()->accept(sv);
}

} // namespace

void init_OSGNode(void)
{
    // wrapping osg::Node functionality
    register_field<av::osg::SFNode>("SFNode");
    register_multifield<av::osg::MFNode>("MFNode");
    class_<av::osg::Node, av::Link<av::osg::Node>, bases<av::osg::Object>, boost::noncopyable>("Node", "docstring", no_init)
        .def("get_bounding_sphere", &av::osg::Node::getBoundingSphere)
        .def("get_absolute_transform", &av::osg::Node::getAbsoluteTransform)
        .def("get_absolute_transform_halt_at_node", &av::osg::Node::getAbsoluteTransformHaltAtNode);

    def("calc_bounding_box", CalcBoundingBox);
    def("write_node_file", WriteNodeFile);
    def("optimize_scene_graph", OptimizeSceneGraph);
    def("recalculate_smooth_normals", RecalculateSmoothNormals);
}
