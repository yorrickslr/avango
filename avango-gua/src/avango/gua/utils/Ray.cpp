#include <avango/gua/utils/Ray.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::Ray);

AV_FIELD_DEFINE(av::gua::SFRay);
AV_FIELD_DEFINE(av::gua::MFRay);

av::gua::Ray::Ray(std::shared_ptr<::gua::Ray> guaRay) : m_guaRay(guaRay)
{
    AV_FC_ADD_ADAPTOR_FIELD(Origin, std::bind(&Ray::getOriginCB, this, std::placeholders::_1), std::bind(&Ray::setOriginCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(Direction, std::bind(&Ray::getDirectionCB, this, std::placeholders::_1), std::bind(&Ray::setDirectionCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(TMax, std::bind(&Ray::getTMaxCB, this, std::placeholders::_1), std::bind(&Ray::setTMaxCB, this, std::placeholders::_1));
}

void av::gua::Ray::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Ray, true);

        SFRay::initClass("av::gua::SFRay", "av::Field");
        MFRay::initClass("av::gua::MFRay", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr<::gua::Ray> av::gua::Ray::getGuaRay() const { return m_guaRay; }

void av::gua::Ray::getOriginCB(const SFVec3::GetValueEvent& event) { *(event.getValuePtr()) = m_guaRay->origin_; }

void av::gua::Ray::setOriginCB(const SFVec3::SetValueEvent& event) { m_guaRay->origin_ = event.getValue(); }

void av::gua::Ray::getDirectionCB(const SFVec3::GetValueEvent& event) { *(event.getValuePtr()) = m_guaRay->direction_; }

void av::gua::Ray::setDirectionCB(const SFVec3::SetValueEvent& event) { m_guaRay->direction_ = event.getValue(); }

void av::gua::Ray::getTMaxCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaRay->t_max_; }

void av::gua::Ray::setTMaxCB(const SFFloat::SetValueEvent& event) { m_guaRay->t_max_ = event.getValue(); }
