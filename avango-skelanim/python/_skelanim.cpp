// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
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
#include "SkeletalAnimationLoader.hpp"
#include "SkeletalAnimationPassDescription.hpp"
#include "SkeletalAnimationNode.hpp"
#include <avango/gua/skelanim/renderer/SkeletalAnimationLoader.hpp>
#include <avango/gua/skelanim/renderer/SkeletalAnimationPassDescription.hpp>
#include <avango/gua/skelanim/scenegraph/SkeletalAnimationNode.hpp>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

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

BOOST_PYTHON_MODULE(_skelanim)
{
    av::gua::skelanim::SkeletalAnimationLoader::initClass();
    av::gua::skelanim::SkeletalAnimationPassDescription::initClass();
    av::gua::skelanim::SkeletalAnimationNode::initClass();
    init_SkeletalAnimationNode();
    init_SkeletalAnimationPassDescription();
    init_SkeletalAnimationLoader();
}
