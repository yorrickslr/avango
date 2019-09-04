import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE

#STEREO_MODE = avango.gua.StereoMode.ANAGLYPH_RED_CYAN
STEREO_MODE = avango.gua.StereoMode.SEPARATE_WINDOWS
# STEREO_MODE = avango.gua.StereoMode.ANAGLYPH_RED_GREEN
# STEREO_MODE = avango.gua.StereoMode.SIDE_BY_SIDE
# STEREO_MODE = avango.gua.StereoMode.CHECKERBOARD
# STEREO_MODE = avango.gua.StereoMode.NVIDIA_3D_VISION
# STEREO_MODE = avango.gua.StereoMode.QUAD_BUFFERED

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*2.0, 0.0, 1.0, 0.0)

def start():

    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

    loader = avango.gua.nodes.TriMeshLoader()
    monkey = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", avango.gua.LoaderFlags.DEFAULTS)

    light = avango.gua.nodes.LightNode(
      Type=avango.gua.LightType.POINT,
      Name = "light",
      Color = avango.gua.Color(1.0, 1.0, 1.0),
      Brightness = 10
    )
    light.Transform.value = avango.gua.make_trans_mat(1, 1, 2) * avango.gua.make_scale_mat(15, 15, 15)

    # setup viewing
    width = 3840 #1024
    height = 2160 #768
    eye_size = avango.gua.Vec2ui(width, height)
    window_size = avango.gua.Vec2ui(width, height)
    left_pos = avango.gua.Vec2ui(0, 0)
    right_pos = avango.gua.Vec2ui(0, 0)

    if STEREO_MODE == avango.gua.StereoMode.SIDE_BY_SIDE:
        right_pos.x = width + 1
        window_size.x *= 2

    window_left = avango.gua.nodes.Window(
        Size = window_size,
        Display = ":0.0",
        #SwapGroup = 1,
        #SwapBarrier = 1,
        LeftPosition = left_pos,
        LeftResolution = eye_size,
        RightPosition = right_pos,
        RightResolution = eye_size,
        StereoMode = STEREO_MODE)

    avango.gua.register_window("window_left", window_left)

    window_right = avango.gua.nodes.Window(
        Size = window_size,
        Display = ":0.0",  # ":0.1",
        #SwapGroup = 1,
        #SwapBarrier = 1,
        LeftPosition = left_pos,
        LeftResolution = eye_size,
        RightPosition = right_pos,
        RightResolution = eye_size,
        StereoMode = STEREO_MODE)

    avango.gua.register_window("window_right", window_right)

    cam = avango.gua.nodes.CameraNode(
        Name = "cam",
        LeftScreenPath = "/screen",
        RightScreenPath = "/screen",
        SceneGraph = "scenegraph",
        Resolution = eye_size,
        EyeDistance = 0.06,
        EnableStereo = True,
        LeftOutputWindow = "window_left",
        RightOutputWindow = "window_right",
        Transform = avango.gua.make_trans_mat(0.0, 0.0, 0.5)
        # NearClip =
    )

    screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 0.5, Height = 0.5 * 0.3 / 0.4)
    screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 2.5)
    screen.Children.value = [cam]

    graph.Root.value.Children.value = [monkey, light, screen]

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window_left, window_right]
    viewer.DesiredFPS.value = 500.0

    monkey_updater = TimedRotate()

    timer = avango.nodes.TimeSensor()
    monkey_updater.TimeIn.connect_from(timer.Time)

    monkey.Transform.connect_from(monkey_updater.MatrixOut)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
  start()

