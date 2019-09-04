#ifndef AVANGO_GUA_CAMERA_HPP
#define AVANGO_GUA_CAMERA_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/CameraNode.hpp>

#include <avango/gua/scenegraph/Node.hpp>
#include <avango/gua/renderer/PipelineDescription.hpp>
#include <avango/gua/math/Frustum.hpp>

namespace av
{
namespace gua
{
class SceneGraph;

/**
 * Wrapper for ::gua::CameraNode
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL CameraNode : public av::gua::Node
{
    AV_FC_DECLARE();

  public:
    enum ProjectionMode
    {
        PERSPECTIVE = static_cast<unsigned int>(::gua::node::CameraNode::ProjectionMode::PERSPECTIVE),
        ORTHOGRAPHIC = static_cast<unsigned int>(::gua::node::CameraNode::ProjectionMode::ORTHOGRAPHIC)
    };

    enum CameraMode
    {
        CENTER = static_cast<unsigned int>(::gua::CameraMode::CENTER),
        LEFT = static_cast<unsigned int>(::gua::CameraMode::LEFT),
        RIGHT = static_cast<unsigned int>(::gua::CameraMode::RIGHT)
    };

    /**
     * Constructor. When called without arguments, a new ::gua::CameraNode is created.
     * Otherwise, the given ::gua::CameraNode is used.
     */
    CameraNode(std::shared_ptr<::gua::node::CameraNode> guaCameraNode = std::shared_ptr<::gua::node::CameraNode>(new ::gua::node::CameraNode("")));

    av::Link<Frustum> get_frustum(SceneGraph const& graph, CameraMode mode);
#if defined(AVANGO_DISTRIBUTION_SUPPORT)
    virtual void on_distribute(av::gua::NetTransform& netNode);
    virtual void on_undistribute(av::gua::NetTransform& netNode);
#endif
  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    // virtual ~CameraNode() {}

  public:
    SFBool Enabled;

    SFPipelineDescription PipelineDescription;

    SFString SceneGraph;

    SFInt ViewID;

    SFUInt Mode;

    SFBool EnableStereo;
    SFFloat EyeDistance;
    SFFloat EyeOffset;
    SFString LeftScreenPath;
    SFString RightScreenPath;
    SFString AlternativeFrustumCullingScreenPath;
    SFUInt MonoMode;

    SFFloat FarClip;
    SFFloat NearClip;

    SFVec2ui Resolution;

    SFString OutputTextureName;
    SFString OutputWindowName;
    SFString LeftOutputWindow;
    SFString RightOutputWindow;

    SFBool EnableFrustumCulling;

    MFString WhiteList;
    MFString BlackList;

    MultiField<Link<CameraNode>> PreRenderCameras;

    virtual void getEnabledCB(const SFBool::GetValueEvent& event);
    virtual void setEnabledCB(const SFBool::SetValueEvent& event);

    virtual void getPipelineDescriptionCB(const SFPipelineDescription::GetValueEvent& event);
    virtual void setPipelineDescriptionCB(const SFPipelineDescription::SetValueEvent& event);

    virtual void getSceneGraphCB(const SFString::GetValueEvent& event);
    virtual void setSceneGraphCB(const SFString::SetValueEvent& event);

    virtual void getViewIDCB(const SFInt::GetValueEvent& event);
    virtual void setViewIDCB(const SFInt::SetValueEvent& event);

    virtual void getModeCB(const SFUInt::GetValueEvent& event);
    virtual void setModeCB(const SFUInt::SetValueEvent& event);

    virtual void getEnableStereoCB(const SFBool::GetValueEvent& event);
    virtual void setEnableStereoCB(const SFBool::SetValueEvent& event);

    virtual void getEyeDistanceCB(const SFFloat::GetValueEvent& event);
    virtual void setEyeDistanceCB(const SFFloat::SetValueEvent& event);

    virtual void getEyeOffsetCB(const SFFloat::GetValueEvent& event);
    virtual void setEyeOffsetCB(const SFFloat::SetValueEvent& event);

    virtual void getLeftScreenPathCB(const SFString::GetValueEvent& event);
    virtual void setLeftScreenPathCB(const SFString::SetValueEvent& event);

    virtual void getRightScreenPathCB(const SFString::GetValueEvent& event);
    virtual void setRightScreenPathCB(const SFString::SetValueEvent& event);

    virtual void getAlternativeFrustumCullingScreenPathCB(const SFString::GetValueEvent& event);
    virtual void setAlternativeFrustumCullingScreenPathCB(const SFString::SetValueEvent& event);

    virtual void getMonoModeCB(const SFUInt::GetValueEvent& event);
    virtual void setMonoModeCB(const SFUInt::SetValueEvent& event);

    virtual void getFarClipCB(const SFFloat::GetValueEvent& event);
    virtual void setFarClipCB(const SFFloat::SetValueEvent& event);

    virtual void getNearClipCB(const SFFloat::GetValueEvent& event);
    virtual void setNearClipCB(const SFFloat::SetValueEvent& event);

    virtual void getResolutionCB(const SFVec2ui::GetValueEvent& event);
    virtual void setResolutionCB(const SFVec2ui::SetValueEvent& event);

    virtual void getOutputTextureNameCB(const SFString::GetValueEvent& event);
    virtual void setOutputTextureNameCB(const SFString::SetValueEvent& event);

    virtual void getOutputWindowNameCB(const SFString::GetValueEvent& event);
    virtual void setOutputWindowNameCB(const SFString::SetValueEvent& event);

    virtual void getLeftOutputWindowCB(const SFString::GetValueEvent& event);
    virtual void setLeftOutputWindowCB(const SFString::SetValueEvent& event);

    virtual void getRightOutputWindowCB(const SFString::GetValueEvent& event);
    virtual void setRightOutputWindowCB(const SFString::SetValueEvent& event);

    virtual void getEnableFrustumCullingCB(const SFBool::GetValueEvent& event);
    virtual void setEnableFrustumCullingCB(const SFBool::SetValueEvent& event);

    virtual void getWhiteListCB(const MFString::GetValueEvent& event);
    virtual void setWhiteListCB(const MFString::SetValueEvent& event);

    virtual void getBlackListCB(const MFString::GetValueEvent& event);
    virtual void setBlackListCB(const MFString::SetValueEvent& event);

    virtual void getPreRenderCamerasCB(const MultiField<Link<CameraNode>>::GetValueEvent& event);
    virtual void setPreRenderCamerasCB(const MultiField<Link<CameraNode>>::SetValueEvent& event);

    /**
     * Get the wrapped ::gua::CameraNode.
     */
    std::shared_ptr<::gua::node::CameraNode> getGuaNode() const;

  public:
  private:
    std::shared_ptr<::gua::node::CameraNode> m_guaNode;

    MultiField<Link<CameraNode>>::ContainerType m_preRenderCameraNodes;
    av::Link<av::gua::PipelineDescription> m_PipelineDescription;

    CameraNode(const CameraNode&);
    CameraNode& operator=(const CameraNode&);
};

typedef SingleField<Link<CameraNode>> SFCameraNode;
typedef MultiField<Link<CameraNode>> MFCameraNode;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::CameraNode>>;
template class AV_GUA_DLL MultiField<Link<gua::CameraNode>>;
#endif

} // namespace av

#endif // AVANGO_GUA_CAMERA_HPP
