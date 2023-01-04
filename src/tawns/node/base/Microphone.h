//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef DEVICES_BASE_MICROPHONE_H_
#define DEVICES_BASE_MICROPHONE_H_

#include "inet/common/INETDefs.h"
#include "MicrophoneControlMsg_m.h"

namespace tawns {

class Microphone : public inet::cSimpleModule
{
    private:
        float recStart = 0.0;
        float recEnd = 0.0;

        long recordingSignal;
        long recordingsCtr;

    protected:
        const char *id;
        const char *filename;

        const char* audioFileType;
        double audioSampleFrequency;
        int audioSampleSize;
        int numChannels;
        const char* audioEncoding;
        double audioDuration;

        inet::cMessage *timeoutMsg = nullptr;

        virtual void initialize(int stage) override;
        virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }

        virtual void handleMessage(inet::cMessage *msg) override;
        virtual MicrophoneControlMsg* generateMsg();


    public:
        virtual void setMicId(const char* newId);
        virtual const char* getMicId();

        virtual const char* getAudioFileType();
        virtual double getAudioSampleFrequency();
        virtual int getAudioSampleSize();
        virtual int getNumChannels();
        virtual const char* getAudioEncoding();
        virtual double getAudioDuration();
};


} //namespace tawns



#endif /* DEVICES_BASE_MICROPHONE_H_ */
