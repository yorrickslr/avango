#ifndef AVANGO_GUA_GEOMETRY_NODE_HPP
#define AVANGO_GUA_GEOMETRY_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/GeometryNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::GeometryNode
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL GeometryNode : public av::gua::Node
{
    AV_FC_DECLARE_ABSTRACT();

  public:
    enum ShadowModeEnum
    {
        OFF = static_cast<unsigned>(::gua::ShadowMode::OFF),
        LOW_QUALITY = static_cast<unsigned>(::gua::ShadowMode::LOW_QUALITY),
        HIGH_QUALITY = static_cast<unsigned>(::gua::ShadowMode::HIGH_QUALITY)
    };

    /**
     * Constructor. When called without arguments, a new ::gua::GeometryNode
     * is created. Otherwise, the given ::gua::GeometryNode is used.
     */
    GeometryNode(std::shared_ptr<::gua::node::GeometryNode> guanode);

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~GeometryNode() {}

  public:
    SFUInt ShadowMode;

    /**
     * Get the wrapped ::gua::GeometryNode.
     */
    std::shared_ptr<::gua::node::GeometryNode> getGuaNode() const;

  public:
    virtual void getShadowModeCB(const SFUInt::GetValueEvent& event);
    virtual void setShadowModeCB(const SFUInt::SetValueEvent& event);

  protected:
    std::shared_ptr<::gua::node::GeometryNode> m_guaNode;

  private:
    GeometryNode(const GeometryNode&);
    GeometryNode& operator=(const GeometryNode&);
};

using SFGeometryNode = SingleField<Link<GeometryNode>>;
using MFGeometryNode = MultiField<Link<GeometryNode>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::GeometryNode>>;
template class AV_GUA_DLL MultiField<Link<gua::GeometryNode>>;
#endif

} // namespace av

#endif // AVANGO_GUA_GEOMETRY_NODE_HPP
