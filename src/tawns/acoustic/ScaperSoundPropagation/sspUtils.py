#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU  General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU  General Public License for more details.
# 
# You should have received a copy of the GNU  General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
# 

from enum import Enum

from scipy.stats import uniform, truncnorm
import numpy as np  # scipy.stats uses numpy.random in background



class Coordinate:
    def __init__(self, x: float, y: float, z: float):
        """
        Coordinate in the same kind of coordinate Omnetpp uses.

        Parameters
        ----------
        x: float
            the x coordinate
        y: float
            the y coordinate
        z: float
            the z coordinate

        """
        self.x : float = x
        self.y : float = y
        self.z : float = z

    def __repr__(self):
        return ' | '.join(map(repr, [self.x, self.y, self.z]))

    def __add__(self, new):
        return Coordinate(self.x+new.x, self.y + new.y, self.z + new.z)


class Event:

    def __init__(self, time_seconds: float):
        """
        Refers to to an general event.

        Parameters
        ----------
        time_seconds: float
            the time at which an event occurs, in seconds
        """
        self.time = time_seconds

    def __lt__(self, other):
        return self.time < other.time


class Sound_Event(Event):

    def __init__(self, time_seconds):
        """
        Class of a sound event, merely used for distinction when using __class__ in the Sound_Environment.

        Parameters
        ----------
        time_seconds: float
            the time at which an event occurs, in seconds
        """

        """Merely used for distinction by __class__"""
        super().__init__(time_seconds)

    def __repr__(self):
        return ''.join(map(repr, [self.time]))


class Mobility_Event(Event):

    def __init__(self, time_seconds: float, coord: Coordinate):
        """
        Refers to events in which a position of an entitiy changes.

        Parameters
        ----------
        time_seconds: float
            time in which the event takes place
        coord: Coordinate
            the new coordinate which is reached at a specific time
        """
        super().__init__(time_seconds)
        self.coord: Coordinate = coord


def random_Sequence_With_Range_And_Minimum_Distance_Between_Samples(max:float, max_samples:int,
                                                                    mingap:float=None, mingap_offset=1.0,
                                                                    random_state=None)->[float]:
    """
    Generate a sequence of times with an minimal gap between each other.

    Parameters
    ----------
    max: float
        the latest time where an number might be generated
    max_samples: float
        the maximal number of samples generated
    mingap:
        the minimal distance between each chunk
    mingap_offset:
        mingap offset to prevent start *almost directly* after the end of a chunk
    random_state: int
        The seed for the random number generator

    Returns: [float]
        random sequence with minimum distance between values
    -------

    """

    if random_state is not None:
        np.random.seed(random_state)

    slack = np.max([1, int(max - (mingap+mingap_offset) * (max_samples - 1))])

    steps = np.random.randint(0, slack)
    increments = np.hstack([np.ones((steps,)), np.zeros((max_samples,))])
    np.random.shuffle(increments)
    locs = np.argwhere(increments == 0).flatten()
    return np.cumsum(increments)[locs] + (mingap+mingap_offset) * np.arange(1, max_samples+1)