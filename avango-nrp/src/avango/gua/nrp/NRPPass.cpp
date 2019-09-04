#include <avango/Base.h>
#include <avango/gua/nrp/NRPPass.hpp>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::nrp::NRPPassDescription);

AV_FIELD_DEFINE(av::gua::nrp::SFNRPPassDescription);
AV_FIELD_DEFINE(av::gua::nrp::MFNRPPassDescription);

av::gua::nrp::NRPPassDescription::NRPPassDescription(std::shared_ptr<::gua::nrp::NRPPassDescription> const &guaNRPPassDescription)
    : PipelinePassDescription(guaNRPPassDescription), m_guaPipelinePassDescription(guaNRPPassDescription)
{
}

void av::gua::nrp::NRPPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::nrp::NRPPassDescription, true);

        SFNRPPassDescription::initClass("av::gua::nrp::SFNRPPassDescription", "av::Field");
        MFNRPPassDescription::initClass("av::gua::nrp::MFNRPPassDescription", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}
std::shared_ptr<::gua::nrp::NRPPassDescription> const &av::gua::nrp::NRPPassDescription::getGuaPipelinePassDescription() const { return m_guaPipelinePassDescription; }
