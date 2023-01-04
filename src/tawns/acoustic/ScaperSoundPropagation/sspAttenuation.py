#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
# 

from dataclasses import dataclass
from typing import Callable

import numpy as np


@dataclass
class IAttenuation_Model:
    """
    ("Pythonic") Interface  for the sound attenuation model.
    """
    id : str
    params : list
    ref_db: float
    function : Callable = None


class Attenuation_Model_Log(IAttenuation_Model):

    def __init__(self, id:str, ref_db:float, params:[]):
        """
        Logarithmic model for the attenuation.
        Formular f(x) = a * log(x+b) + c for x > 1 and a * log(x+b) + c for x <= 1 or (x+b) <= 1

        Parameters
        ----------
        id: str
            The name of the model.
        ref_db: float
            The reference dB value .
        params: []
            The parameters of the model
        """
        IAttenuation_Model.__init__(self, id, params, ref_db, self.model)

    def model(self, distance):
        if distance >=1 and (self.params[1] + distance) >=1:
            ret = self.params[0] * np.log(self.params[1] + distance) + self.params[2]
        else:
            ret =  self.params[0] * np.log(1) + self.params[2]

        # snr ("how much louder the foreground event is") is the difference between the ref_db and the model
        ret = self.ref_db - ret
        return (-1) *  ret # ret indicates how much louder the FG event should be, so it must first be positive then negative