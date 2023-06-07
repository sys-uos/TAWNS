//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU  General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU  General Public License for more details.
//
// You should have received a copy of the GNU  General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef TAWNS_APPLICATIONS_BASE_IACOUSTICAPP_H_
#define TAWNS_APPLICATIONS_BASE_IACOUSTICAPP_H_

#include "../../node/base/Disc.h"
#include "inet/common/INETDefs.h"
#include "tawns/applications/contract/Recording.h"


namespace tawns {


class IAcousticApp
{
    public:
    /*
     * Pop last recording from the list
     */
        virtual Recording popRecording(Disc *disc) = 0;
};

}

#endif /* TAWNS_APPLICATIONS_BASE_IACOUSTICAPP_H_ */
