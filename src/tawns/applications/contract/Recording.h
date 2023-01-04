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

#ifndef TAWNS_APPLICATIONS_CONTRACT_RECORDING_H_
#define TAWNS_APPLICATIONS_CONTRACT_RECORDING_H_

namespace tawns
{
    /**
     * Wrapper for a Recording that is produced by the microphone at a specific simulation time
     */
    class Recording
    {
        protected:
            const char *path;
            float recStart;
            float recEnd;

        public:
            Recording(const char * p, float rs, float re)
            {
                path = p;
                recStart = rs;
                recEnd = re;
            }

            const char* getPath()
            {
                return path;
            }

            std::pair<float,float> getRecTimes()
            {
                return std::pair<float,float>(recStart,recEnd);
            }

            bool valid()
            {
                if((recStart == 0) && (recEnd == 0))
                {
                    return false;
                }
                return true;
            }


            ~Recording() {}
    };
}



#endif /* TAWNS_APPLICATIONS_CONTRACT_RECORDING_H_ */
