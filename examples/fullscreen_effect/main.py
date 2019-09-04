import avango
import avango.script
import avango.gua
from examples_common.GuaVE import GuaVE


class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    def evaluate(self):
        self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value * 2.0,
                                                       0.0, 1.0, 0.0)


class UpdateShaderTime(avango.script.Script):
    TimeIn = avango.SFFloat()
    effect = None

    def evaluate(self):
        self.effect.set_uniform("time", self.TimeIn.value)


def start():
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    monkey1 = loader.create_geometry_from_file(
        "monkey", "data/objects/monkey.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    monkey2 = loader.create_geometry_from_file(
        "monkey", "data/objects/monkey.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE)

    monkey1.Material.value.set_uniform("Color",
                                       avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
    monkey1.Material.value.set_uniform("Roughness", 0.3)
    monkey1.Material.value.set_uniform("Metalness", 1.0)

    monkey2.Material.value.set_uniform("Color",
                                       avango.gua.Vec4(1.0, 0.266, 0.136, 1.0))
    monkey2.Material.value.set_uniform("Roughness", 0.6)
    monkey2.Material.value.set_uniform("Metalness", 0.0)

    transform1 = avango.gua.nodes.TransformNode(Children=[monkey1])
    transform2 = avango.gua.nodes.TransformNode(
        Transform=avango.gua.make_trans_mat(-0.5, 0.0, 0.0),
        Children=[monkey2])

    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=100.0,
        Transform=(avango.gua.make_trans_mat(1, 1, 5) *
                   avango.gua.make_scale_mat(30, 30, 30)))

    size = avango.gua.Vec2ui(1024, 768)

    window = avango.gua.nodes.GlfwWindow(Size=size, LeftResolution=size)

    avango.gua.register_window("window", window)

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath="/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5))

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = True
    res_pass.SSAOIntensity.value = 4.0
    res_pass.SSAOFalloff.value = 10.0
    res_pass.SSAORadius.value = 7.0

    #res_pass.EnableScreenSpaceShadow.value = True

    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)

    anti_aliasing = avango.gua.nodes.SSAAPassDescription()

    shader_source = ""
    with open('effect3.frag') as f:
      shader_source = f.read()

    effect = avango.gua.nodes.FullscreenPassDescription(Source=shader_source)

    effect_updater = UpdateShaderTime()
    effect_updater.effect = effect

    pipeline_description = avango.gua.nodes.PipelineDescription(Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        res_pass,
        anti_aliasing,
        effect
    ])

    cam.PipelineDescription.value = pipeline_description

    screen = avango.gua.nodes.ScreenNode(Name="screen",
                                         Width=2,
                                         Height=1.5,
                                         Children=[cam])

    graph.Root.value.Children.value = [transform1, transform2, light, screen]

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    monkey_updater = TimedRotate()

    timer = avango.nodes.TimeSensor()
    monkey_updater.TimeIn.connect_from(timer.Time)
    effect_updater.TimeIn.connect_from(timer.Time)

    transform1.Transform.connect_from(monkey_updater.MatrixOut)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start()
