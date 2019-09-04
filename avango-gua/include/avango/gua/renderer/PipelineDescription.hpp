#ifndef AVANGO_GUA_PIPELINE_DESCRIPTION_HPP
#define AVANGO_GUA_PIPELINE_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/PipelineDescription.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#if defined(AVANGO_PBR_SUPPORT)
#include <avango/gua/renderer/PLODPassDescription.hpp>
#endif

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::PipelineDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL PipelineDescription : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::PipelineDescription is created.
     * Otherwise, the given ::gua::PipelineDescription is used.
     */
    PipelineDescription(std::shared_ptr<::gua::PipelineDescription> const& PipelineDescription = std::shared_ptr<::gua::PipelineDescription>(new ::gua::PipelineDescription()));

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~PipelineDescription();

  public:
    SFBool EnableABuffer;
    SFInt ABufferSize;
    MultiField<Link<PipelinePassDescription>> Passes;

    virtual void getEnableABufferCB(const SFBool::GetValueEvent& event);
    virtual void setEnableABufferCB(const SFBool::SetValueEvent& event);

    virtual void getABufferSizeCB(const SFInt::GetValueEvent& event);
    virtual void setABufferSizeCB(const SFInt::SetValueEvent& event);

    virtual void getPassesCB(const MultiField<Link<PipelinePassDescription>>::GetValueEvent& event);
    virtual void setPassesCB(const MultiField<Link<PipelinePassDescription>>::SetValueEvent& event);

    /**
     * Get the wrapped ::gua::PipelineDescription.
     */
    std::shared_ptr<::gua::PipelineDescription> const& getGuaPipelineDescription() const;

  private:
    std::shared_ptr<::gua::PipelineDescription> m_guaPipelineDescription;

    PipelineDescription(const PipelineDescription&);
    PipelineDescription& operator=(const PipelineDescription&);
};

using SFPipelineDescription = SingleField<Link<PipelineDescription>>;
using MFPipelineDescription = MultiField<Link<PipelineDescription>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::PipelineDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::PipelineDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_PIPELINE_DESCRIPTION_HPP
