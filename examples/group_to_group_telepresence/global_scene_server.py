# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
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
'''
A distributed viewer setup: This Python scripts starts an
avango.osg.simpleviewer to load a given geometry. Any client connected to the
group "testgroup" should receive this model. (see also simpleviewer-clnt.py)
'''

import avango
import avango.script
import avango.gua
import avango.gua.lod
from avango.script import field_has_changed

import examples_common.navigator
import examples_common.default_views

from examples_common.GuaVE import GuaVE

#avango.enable_logging(4, "server.log")


nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                          Groupname="AVSERVER|141.54.147.52|7432") # server -> pan
                                         #Groupname="AVSERVER|141.54.147.54|7432")


class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    @field_has_changed(TimeIn)
    def update(self):
        self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value * 30.0,
                                                       0.0, 1.0, 0.0) * avango.gua.make_scale_mat(1.0)


class TimedKeyframePathAnimation(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    walking_speed_units_per_second = 1.666667

    indexed_keyframe_positions = [(0, avango.gua.Vec3(0, 0, -20)),
                                  (2000, avango.gua.Vec3(0, 0, -20)), 
                                  (4000, avango.gua.Vec3(0, 0.0, 5) ),
                                  (6000, avango.gua.Vec3(0, 0.0, 5) ),
                                  (8000, avango.gua.Vec3(0, 0.0, -1.6) ),
                                  (9000, avango.gua.Vec3(0, 0.0, -1.6) ),
                                  (10000, avango.gua.Vec3(-1.5, 0, -1.6) ),
                                  (12000, avango.gua.Vec3(-1.5, 0, -1.6) )
                                ]
    

    animation_length_in_ms = indexed_keyframe_positions[-1][0]


    @field_has_changed(TimeIn)
    def update(self):
        current_time_point = int(self.TimeIn.value*1000) % self.animation_length_in_ms
        print("Time:" + str(current_time_point) )

        keyframe_tuple_x = 0
        keyframe_tuple_x_plus_1 = 0

        found_first_time_point = False

        current_keyframe_id = 0
        for ikfp in self.indexed_keyframe_positions:
            if not found_first_time_point:
                if current_time_point == ikfp[0]:
                    found_first_time_point = True
                    keyframe_tuple_x = keyframe_tuple_x_plus_1 = ikfp
                elif current_time_point > ikfp[0]:
                    if (len(self.indexed_keyframe_positions) == current_keyframe_id + 1) or (self.indexed_keyframe_positions[current_keyframe_id + 1][0] > current_time_point) :
                        found_first_time_point = True
                        keyframe_tuple_x = ikfp
            else:
                if current_time_point < ikfp[0]:
                    keyframe_tuple_x_plus_1 = ikfp
                    break
            current_keyframe_id = current_keyframe_id + 1

        interpolation_weight = ((current_time_point) - keyframe_tuple_x[0]) / ((keyframe_tuple_x_plus_1[0]) - keyframe_tuple_x[0])
        print( str(keyframe_tuple_x[0]) + "; " + str(current_time_point) + "; " + str(keyframe_tuple_x_plus_1[0]) )
        print("interpolation_weight" + str(interpolation_weight))

        interpolated_position = keyframe_tuple_x[1] * (1.0 - interpolation_weight) + keyframe_tuple_x_plus_1[1] * (interpolation_weight)

        curr_pos = interpolated_position
        self.MatrixOut.value = avango.gua.make_trans_mat(curr_pos[0], curr_pos[1], curr_pos[2])


def make_node_distributable(node):
    for child in node.Children.value:
        make_node_distributable(child)
    nettrans.distribute_object(node)


def make_material_distributable(mat):
    nettrans.distribute_object(mat)

# setup scenegraph
graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
loader = avango.gua.nodes.TriMeshLoader()


kaisersaal = loader.create_geometry_from_file("kaisersaal", "/mnt/data_internal/geometry_data/confidential/Kaisersaal/Ktris_7500/Bam_Kai_o_L_12_ct_0750.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)

kaisersaal.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 2.0) * avango.gua.make_scale_mat(1.0, 1.0, 4.0) * avango.gua.make_rot_mat(90.0, 0.0, -1.0, 0.0) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)


lion = loader.create_geometry_from_file("loewe", "/home/wabi7015/Desktop/250k_hq_texture_loewe_quickfix.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
lion.Transform.value = avango.gua.make_trans_mat(-2.5, 0.0, -1.0) * avango.gua.make_rot_mat(-90.0, 0.0, 1.0, 0.0) * avango.gua.make_scale_mat(1.0, 1.0, 1.0) 



mat_desc = avango.gua.nodes.MaterialShaderDescription()
mat_desc.load_from_file("data/materials/SimpleMaterial.gmd")
avango.gua.register_material_shader(mat_desc, "mat")

mat = avango.gua.nodes.Material(ShaderName="mat")

#teapot.Material.value = mat
nettrans.distribute_object(mat)

spointsloader = avango.gua.nodes.SPointsLoader()
avatar_geode = spointsloader.load("kinect", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/spoints_resource_hekate_for_argos.sr")

light = avango.gua.nodes.LightNode(Type=avango.gua.LightType.POINT,
                                   Name="light",
                                   Color=avango.gua.Color(1.0, 1.0, 1.0),
                                   Brightness=200.0,
                                   Transform=avango.gua.make_trans_mat(1, 1, 5)
                                   * avango.gua.make_scale_mat(30, 30, 30))

scene_transform = avango.gua.nodes.TransformNode(Name="scene_transform")
scene_transform.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.0)

#+++++++++++

avatar_transform = avango.gua.nodes.TransformNode(Name="avatar_transform")
avatar_transform.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.0)
avatar_transform.Children.value = [avatar_geode]

non_avatar_scene_transform = avango.gua.nodes.TransformNode(Name="non_avatar_scene_transform")
non_avatar_scene_transform.Transform.value = avango.gua.make_trans_mat(1.0, 0.0, 0.0)
non_avatar_scene_transform.Children.value = [kaisersaal, lion, light]
scene_transform.Children.value = [non_avatar_scene_transform, avatar_transform]

#screen = avango.gua.nodes.ScreenNode(Name="screen", Width=4, Height=3)
screen = avango.gua.nodes.ScreenNode(Name="screen", Width=0.292, Height=0.22)

size = avango.gua.Vec2ui(1600, 1200)

tri_pass = avango.gua.nodes.TriMeshPassDescription()
tquad_pass = avango.gua.nodes.TexturedQuadPassDescription()
lvis_pass = avango.gua.nodes.LightVisibilityPassDescription()
res_pass = avango.gua.nodes.ResolvePassDescription()
res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
tscreenspace_pass = avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()
spoints_pass_description = avango.gua.nodes.SPointsPassDescription()
occlusion_slave_res_pass = avango.gua.nodes.OcclusionSlaveResolvePassDescription()

pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        avango.gua.nodes.SPointsPassDescription(),
        res_pass
    ])

occlusion_slave_pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        avango.gua.nodes.SPointsPassDescription(),
        occlusion_slave_res_pass
    ])


eye_height = 1.6

camera_translations_center = avango.gua.Vec3(0.0, 0.0, 0.7)
camera_translations_left = avango.gua.Vec3(-0.05, 0.0, 0.7)

server_cam = avango.gua.nodes.CameraNode(
    ViewID=1,
    LeftScreenPath="/net/grouped_view_setups_and_scene/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="server_window",
    Transform=avango.gua.make_trans_mat(camera_translations_center),
    PipelineDescription=pipeline_description,
    BlackList = ["invisible_osaka_avatar"]
    )


client_cam_center = avango.gua.nodes.CameraNode(
    ViewID=3,
    Name="viewer_0_weimar_center",
    LeftScreenPath="/net/grouped_view_setups_and_scene/screen",
    RightScreenPath="/net/grouped_view_setups_and_scene/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="client_window_weimar_center",
    Transform=avango.gua.make_trans_mat(camera_translations_center),
    PipelineDescription=pipeline_description,

    EyeDistance = 0.06,
    EnableStereo = True,
    )

occlusion_slave_client_cam_center = avango.gua.nodes.CameraNode(
    ViewID=3,
    Name="os_weimar_v0_osaka_center",
    LeftScreenPath="/net/grouped_view_setups_and_scene/screen",
    RightScreenPath="/net/grouped_view_setups_and_scene/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="slave_weimar_v0_osaka_center",
    Transform=avango.gua.make_trans_mat(camera_translations_center),
    PipelineDescription=occlusion_slave_pipeline_description,
    #PipelineDescription=pipeline_description,
    EyeDistance = 0.06,
    EnableStereo = True,
    BlackList = ["invisible_osaka_avatar"], 
    #needs to be invisible as soon as the real render-client comes into play
    )


client_cam_left = avango.gua.nodes.CameraNode(
    ViewID=4,
    Name="viewer_0_weimar_left",
    LeftScreenPath="/net/grouped_view_setups_and_scene/screen",
    RightScreenPath="/net/grouped_view_setups_and_scene/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="client_window_weimar_left",
    Transform=avango.gua.make_trans_mat(camera_translations_left),
    PipelineDescription=pipeline_description,

    EyeDistance = 0.06,
    EnableStereo = True,
    )

occlusion_slave_client_cam_left = avango.gua.nodes.CameraNode(
    ViewID=4,
    Name="os_weimar_v0_osaka_left",
    LeftScreenPath="/net/grouped_view_setups_and_scene/screen",
    RightScreenPath="/net/grouped_view_setups_and_scene/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="slave_weimar_v0_osaka_left",
    Transform=avango.gua.make_trans_mat(camera_translations_left),
    PipelineDescription=occlusion_slave_pipeline_description,
    #PipelineDescription=pipeline_description,
    EyeDistance = 0.06,
    EnableStereo = True,
    BlackList = ["invisible_osaka_avatar"], 
    #needs to be invisible as soon as the real render-client comes into play
    )


scene_view_transform = avango.gua.nodes.TransformNode(Name="scene_view_transform")
#scene_view_transform.Transform.value = avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0)
scene_view_transform.Children.value = [scene_transform]

grouped_view_setups_and_scene = avango.gua.nodes.TransformNode(Name="grouped_view_setups_and_scene")
grouped_view_setups_and_scene.Children.value = [screen, scene_view_transform]

screen.Children.value = [occlusion_slave_client_cam_center, client_cam_center, occlusion_slave_client_cam_left, client_cam_left, server_cam]
screen.Transform.value = avango.gua.make_trans_mat(0.0, 1.6, 8.5)
nettrans.Children.value = [grouped_view_setups_and_scene]

#grouped_view_setups_and_scene.Transform.value = avango.gua.make_trans_mat(0.0, 1.0, 2.5)


keyboard_based_default_viewer = examples_common.default_views.DefaultViews()
scene_view_transform.Transform.connect_from(keyboard_based_default_viewer.OutTransform)
#navigator = examples_common.navigator.Navigator()
#navigator.StartLocation.value = screen.Transform.value.get_translate()
#navigator.OutTransform.connect_from(screen.Transform)

#navigator.RotationSpeed.value = 0.12/30.0
#navigator.MotionSpeed.value = 0.07/10.0

avatar_geode.Tags.value = ["invisible_osaka_avatar"]


#screen.Transform.connect_from(navigator.OutTransform)

graph.Root.value.Children.value = [nettrans]

make_node_distributable(grouped_view_setups_and_scene)
make_node_distributable(non_avatar_scene_transform)
make_node_distributable(avatar_transform)
make_node_distributable(scene_view_transform)
make_node_distributable(scene_transform)
make_node_distributable(screen)

make_node_distributable(server_cam)

make_node_distributable(client_cam_center)
make_node_distributable(occlusion_slave_client_cam_center)
make_node_distributable(client_cam_left)
make_node_distributable(occlusion_slave_client_cam_left)

nettrans.distribute_object(tri_pass)
nettrans.distribute_object(tquad_pass)
nettrans.distribute_object(lvis_pass)
nettrans.distribute_object(res_pass)
nettrans.distribute_object(tscreenspace_pass)
nettrans.distribute_object(spoints_pass_description)
nettrans.distribute_object(occlusion_slave_res_pass)


for p in pipeline_description.Passes.value:
    nettrans.distribute_object(p)
nettrans.distribute_object(pipeline_description)

for p in occlusion_slave_pipeline_description.Passes.value:
    nettrans.distribute_object(p)
nettrans.distribute_object(occlusion_slave_pipeline_description)

# setup viewing
window = avango.gua.nodes.GlfwWindow(Size=size,
                                     LeftResolution=size,
                                     Title="server_window")

window.EnableVsync.value = False

avango.gua.register_window("server_window", window)

#setup viewer
viewer = avango.gua.nodes.Viewer()
viewer.SceneGraphs.value = [graph]
viewer.Windows.value = [window]



timer = avango.nodes.TimeSensor()

avatar_path_animator = TimedKeyframePathAnimation()
avatar_path_animator.TimeIn.connect_from(timer.Time)
avatar_transform.Transform.connect_from(avatar_path_animator.MatrixOut)


guaVE = GuaVE()
guaVE.start(locals(), globals())

viewer.DesiredFPS.value = 1000.0
viewer.run()