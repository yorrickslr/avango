// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 2007 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <shade/shaders/Checkboard.h>

using namespace shade::shaders;

SHADE_CLASS_INIT(Checkboard, "Checkboard.glsl", SHADE_ENV_DEFS(shade::fragment_shader, (get_pattern)), SHADE_DEFS((position)(scale)(color1)(color2)))

shade::vec4<shade::Type> Checkboard::get_pattern(void) { return invoke<vec4<Type>>("Checkboard_get_pattern_impl"); }

void Checkboard::get_inline(formatter::Generator& generator)
{
    generator.handle_environment_begin(fragment_shader);
    generator.handle_return_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier("Checkboard_get_pattern");
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("self");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_return_end();
    generator.handle_environment_end();
    generator.handle_return_begin();
    generator.handle_literal_vec4(0, 0, 0, 0);
    generator.handle_return_end();
}

shade::vec4<> Checkboard::get(void) { return invoke_inline<vec4<>>(boost::bind(&Checkboard::get_inline, boost::ref(*this), _1)); }
