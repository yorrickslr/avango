// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <avango/sound/SampleBuffer.h>

#include <avango/Logger.h>
#include <functional>
#include <stdexcept>

namespace
{
av::Logger& logger(av::getLogger("av::sound::SampleBuffer"));
}

AV_FC_DEFINE(av::sound::SampleBuffer)

AV_FIELD_DEFINE(av::sound::SFSampleBuffer);
AV_FIELD_DEFINE(av::sound::MFSampleBuffer);

av::sound::SampleBuffer::SampleBuffer() : mBuffer(new SampleType[0]), mNumSamples(0), mSampleRate(0) { throw std::logic_error("Default Constructor of av::sound::SampleBuffer::SampleBuffer called"); }

av::sound::SampleBuffer::SampleBuffer(boost::shared_array<SampleType> buffer, unsigned int numberOfSamples, bool stereo)
    : mBuffer(buffer), mNumSamples(numberOfSamples), mStereo(stereo), mSampleRate(0)
{
    AV_FC_ADD_ADAPTOR_FIELD(
        NumSamples, std::bind(&av::sound::SampleBuffer::getNumSamplesCB, this, std::placeholders::_1), std::bind(&av::sound::SampleBuffer::setNumSamplesCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(IsStereo, std::bind(&av::sound::SampleBuffer::getIsStereoCB, this, std::placeholders::_1), std::bind(&av::sound::SampleBuffer::setIsStereoCB, this, std::placeholders::_1));
}

/* virtual */
av::sound::SampleBuffer::~SampleBuffer() {}

unsigned int av::sound::SampleBuffer::getNumSamples() const { return mNumSamples; }

boost::shared_array<av::sound::SampleBuffer::SampleType> av::sound::SampleBuffer::getSamples() const { return mBuffer; }

unsigned int av::sound::SampleBuffer::getSampleRate() const { return mSampleRate; }

void av::sound::SampleBuffer::setSampleRate(unsigned int sampleRate) { mSampleRate = sampleRate; }

/*static*/ void av::sound::SampleBuffer::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::sound::SampleBuffer, true);
        SFSampleBuffer::initClass("av::sound::SFSampleBuffer", "av::Field");
        MFSampleBuffer::initClass("av::sound::MFSampleBuffer", "av::Field");
    }
}

/* virtual */ void av::sound::SampleBuffer::getNumSamplesCB(const av::SFUInt::GetValueEvent& event)
{
    av::SFUInt::ValueType& value(*event.getValuePtr());
    value = mNumSamples;
}

/* virtual */ void av::sound::SampleBuffer::setNumSamplesCB(const av::SFUInt::SetValueEvent&) {}

/* virtual */ void av::sound::SampleBuffer::getIsStereoCB(const av::SFBool::GetValueEvent& event)
{
    av::SFBool::ValueType& value(*event.getValuePtr());
    value = mStereo;
}

/* virtual */ void av::sound::SampleBuffer::setIsStereoCB(const av::SFBool::SetValueEvent&) {}
