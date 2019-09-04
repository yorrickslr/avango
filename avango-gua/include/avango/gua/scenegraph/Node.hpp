#ifndef AVANGO_GUA_NODE_HPP
#define AVANGO_GUA_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/Node.hpp>
#include <gua/math/math.hpp>
#include <avango/gua/math/BoundingBox.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
class NetTransform;
/**
 * Wrapper for ::gua::Node
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL Node : public av::FieldContainer
{
    AV_FC_DECLARE_ABSTRACT();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::Node is created.
     * Otherwise, the given ::gua::Node is used.
     */
    Node(std::shared_ptr<::gua::node::Node> guanode); // use defined type to circumvent compiler bug in VS8

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
    /**
     * Override these to distribute embedded field containers.
     */
    virtual void on_distribute(av::gua::NetTransform& netNode);
    virtual void on_undistribute(av::gua::NetTransform& netNode);
#endif

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Node();

  public:
    SingleField<Link<Node>> Parent;
    MultiField<Link<Node>> Children;
    SFString Name;
    SFMatrix Transform;
    SFMatrix WorldTransform;
    SFBoundingBox BoundingBox;
    SFBool DisplayBoundingBox;
    SFInt Depth;
    SFString Path;
    MFString Tags;

    /**
     * Get the wrapped ::gua::Node.
     */
    std::shared_ptr<::gua::node::Node> getGuaNode() const;
    unsigned getUserDataHandle() const;

  public:
    virtual void getParentCB(const SingleField<Link<Node>>::GetValueEvent& event);
    virtual void setParentCB(const SingleField<Link<Node>>::SetValueEvent& event);

    virtual void getChildrenCB(const MultiField<Link<Node>>::GetValueEvent& event);
    virtual void setChildrenCB(const MultiField<Link<Node>>::SetValueEvent& event);

    virtual void getNameCB(const SFString::GetValueEvent& event);
    virtual void setNameCB(const SFString::SetValueEvent& event);

    virtual void getTransformCB(const SFMatrix::GetValueEvent& event);
    virtual void setTransformCB(const SFMatrix::SetValueEvent& event);

    virtual void getWorldTransformCB(const SFMatrix::GetValueEvent& event);
    virtual void setWorldTransformCB(const SFMatrix::SetValueEvent& event);

    virtual void getBoundingBoxCB(const SFBoundingBox::GetValueEvent& event);
    virtual void setBoundingBoxCB(const SFBoundingBox::SetValueEvent& event);

    virtual void getDisplayBoundingBoxCB(const SFBool::GetValueEvent& event);
    virtual void setDisplayBoundingBoxCB(const SFBool::SetValueEvent& event);

    virtual void getDepthCB(const SFInt::GetValueEvent& event);
    virtual void setDepthCB(const SFInt::SetValueEvent& event);

    virtual void getPathCB(const SFString::GetValueEvent& event);
    virtual void setPathCB(const SFString::SetValueEvent& event);

    virtual void getTagsCB(const MFString::GetValueEvent& event);
    virtual void setTagsCB(const MFString::SetValueEvent& event);

    void addToParentChildren();

  private:
    std::shared_ptr<::gua::node::Node> m_guaNode;
    ::gua::math::BoundingBox<::gua::math::vec3> m_guaBbox;
    unsigned m_selfUserDataHandle;
    unsigned m_childrenUserDataHandle;

    Node(const Node&);
    Node& operator=(const Node&);
};

using SFNode = SingleField<Link<Node>>;
using MFNode = MultiField<Link<Node>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::Node>>;
template class AV_GUA_DLL MultiField<Link<gua::Node>>;
#endif

} // namespace av

#endif // AVANGO_GUA_NODE_HPP
