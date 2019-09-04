#ifndef AVANGO_GUA_VIEWER_HPP
#define AVANGO_GUA_VIEWER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/events.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/Renderer.hpp>
#include <avango/gua/renderer/WindowBase.hpp>
#include <avango/gua/scenegraph/CameraNode.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#if defined(AVANGO_PHYSICS_SUPPORT)
#include <avango/gua/physics/Physics.hpp>
#endif
#include <avango/FieldContainer.h>
#include <avango/gua/windows_specific_gua.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::Viewer
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL Viewer : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    Viewer();

    MFSceneGraph SceneGraphs;
    MFWindowBase Windows;
#if defined(AVANGO_PHYSICS_SUPPORT)
    SFPhysics Physics;
#endif
    SFFloat DesiredFPS;

    SFFloat ApplicationFPS;

    void run();
    void frame();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Viewer();

  private:
    mutable av::gua::Renderer* m_renderer;

    mutable ::gua::events::MainLoop m_loop;
    mutable ::gua::events::Ticker m_ticker;

    Viewer(const Viewer&);
    Viewer& operator=(const Viewer&);
};

using SFViewer = SingleField<Link<Viewer>>;
using MFViewer = MultiField<Link<Viewer>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::Viewer>>;
template class AV_GUA_DLL MultiField<Link<gua::Viewer>>;
#endif

} // namespace av

#endif // AVANGO_GUA_VIEWER_HPP
