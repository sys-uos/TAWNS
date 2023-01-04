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

#ifndef TAWNS_ACOUSTIC_BASE_ATTENUATIONMODEL_H_
#define TAWNS_ACOUSTIC_BASE_ATTENUATIONMODEL_H_

#include "inet/common/INETDefs.h"

namespace tawns {

class IAttenuationModel : public inet::cSimpleModule
{
    protected:
        virtual void initialize(int stage) override = 0;
        virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }

    public:
        virtual const char* getModelId() = 0;
        virtual std::vector<double> getParams() = 0;
};

class AttenuationModel_Log : public tawns::IAttenuationModel
{
    protected:
        const char* modulId = "AttenuationModel_Log";
        double a;
        double b;
        double c;
        virtual void initialize(int stage) override;
        virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }


    public:
        virtual const char* getModelId() override;
        virtual std::vector<double> getParams() override;
};

} // tawns namespace

#endif /* TAWNS_ACOUSTIC_BASE_ATTENUATIONMODEL_H_ */
