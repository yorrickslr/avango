#include <avango/gua/renderer/PipelineDescription.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::PipelineDescription"));
}

AV_FC_DEFINE(av::gua::PipelineDescription);

AV_FIELD_DEFINE(av::gua::SFPipelineDescription);
AV_FIELD_DEFINE(av::gua::MFPipelineDescription);

av::gua::PipelineDescription::PipelineDescription(std::shared_ptr<::gua::PipelineDescription> const& guaPipelineDescription) : m_guaPipelineDescription(guaPipelineDescription)
{
    auto avGuaPasses(new av::MultiField<av::Link<av::gua::PipelinePassDescription>>::ContainerType());
    guaPipelineDescription->set_user_data(avGuaPasses);

    AV_FC_ADD_ADAPTOR_FIELD(
        EnableABuffer, std::bind(&PipelineDescription::getEnableABufferCB, this, std::placeholders::_1), std::bind(&PipelineDescription::setEnableABufferCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(
        ABufferSize, std::bind(&PipelineDescription::getABufferSizeCB, this, std::placeholders::_1), std::bind(&PipelineDescription::setABufferSizeCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(Passes, std::bind(&PipelineDescription::getPassesCB, this, std::placeholders::_1), std::bind(&PipelineDescription::setPassesCB, this, std::placeholders::_1));
}

av::gua::PipelineDescription::~PipelineDescription()
{
    if(m_guaPipelineDescription)
    {
        auto avGuaPasses(static_cast<av::MultiField<av::Link<PipelinePassDescription>>::ContainerType*>(m_guaPipelineDescription->get_user_data()));
        m_guaPipelineDescription->set_user_data(nullptr);
        if(avGuaPasses)
        {
            avGuaPasses->clear();
            delete avGuaPasses;
        }
    }
}

void av::gua::PipelineDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::PipelineDescription, true);

        SFPipelineDescription::initClass("av::gua::SFPipelineDescription", "av::Field");
        MFPipelineDescription::initClass("av::gua::MFPipelineDescription", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

void av::gua::PipelineDescription::getEnableABufferCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPipelineDescription->get_enable_abuffer(); }

void av::gua::PipelineDescription::setEnableABufferCB(const SFBool::SetValueEvent& event) { m_guaPipelineDescription->set_enable_abuffer(event.getValue()); }

void av::gua::PipelineDescription::getABufferSizeCB(const SFInt::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPipelineDescription->get_abuffer_size(); }

void av::gua::PipelineDescription::setABufferSizeCB(const SFInt::SetValueEvent& event) { m_guaPipelineDescription->set_abuffer_size(event.getValue()); }

std::shared_ptr<::gua::PipelineDescription> const& av::gua::PipelineDescription::getGuaPipelineDescription() const { return m_guaPipelineDescription; }

/* virtual */ void av::gua::PipelineDescription::getPassesCB(const av::gua::MFPipelinePassDescription::GetValueEvent& event)
{
    *(event.getValuePtr()) = *static_cast<av::MultiField<av::Link<PipelinePassDescription>>::ContainerType*>(m_guaPipelineDescription->get_user_data());
}

/* virtual */ void av::gua::PipelineDescription::setPassesCB(const av::gua::MFPipelinePassDescription::SetValueEvent& event)
{
    if(m_guaPipelineDescription)
    {
        m_guaPipelineDescription->clear();
        auto avGuaPasses(static_cast<av::MultiField<av::Link<PipelinePassDescription>>::ContainerType*>(m_guaPipelineDescription->get_user_data()));

        if(avGuaPasses)
            avGuaPasses->clear();

        const av::gua::MFPipelinePassDescription::ContainerType& passes(event.getValue());

        for(auto& pass : passes)
        {
            if(pass->getGuaPipelinePassDescription() != nullptr)
            {
                m_guaPipelineDescription->add_pass(pass->getGuaPipelinePassDescription());
                if(avGuaPasses)
                {
                    avGuaPasses->push_back(pass);
                }
            }
        }
    }
}
