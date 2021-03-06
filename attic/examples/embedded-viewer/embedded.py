# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################

import avango.osg
import avango.script
import avango.osg.simpleviewer
import avango.osg.viewer
import pygtk
pygtk.require('2.0')
import gtk
import gtk.gtkgl
from OpenGL.GL import *
from OpenGL.GLU import *

class OSGViewer(object):
    "Simple example viewer class using an embedded OSG viewer"

    def __init__(self, root):
        width = 600
        height = 400
        self.viewer = avango.osg.simpleviewer.EmbeddedViewer(root, 0, 0, width, height)

        window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        window.set_title("AVANGO View")
        container = gtk.VBox()

        self.draw = gtk.DrawingArea()
        self.draw.props.width_request = width
        self.draw.props.height_request = height
        config = gtk.gdkgl.Config(mode = gtk.gdkgl.MODE_RGB | gtk.gdkgl.MODE_DOUBLE | gtk.gdkgl.MODE_DEPTH )
        gtk.gtkgl.widget_set_gl_capability(self.draw, glconfig = config)
        self.draw.connect("expose_event", self.draw_event)

        container.add(self.draw)
        container.add(gtk.Button("Click Me"))
        window.add(container)
        window.show_all()

        window.connect("destroy", gtk.mainquit)

    def draw_event(self, widget, event):
        alloc = self.draw.get_allocation()
        self.viewer.resized(0, 0, alloc.width, alloc.height)

        drawable = gtk.gtkgl.widget_get_gl_drawable(widget)
        context = gtk.gtkgl.widget_get_gl_context(widget)
        if not drawable.gl_begin(context):
            return

        avango.evaluate()
        self.viewer.frame()

        drawable.swap_buffers()
        drawable.gl_end()
        return True


root = avango.osg.nodes.MatrixTransform(Name="Root")
sphere = avango.osg.nodes.Sphere(Name="Sphere")
root.Children.value = [ sphere ]
root.Matrix.value = avango.osg.make_trans_mat(0., 0., -5.)

viewer = OSGViewer(root)
gtk.main()
