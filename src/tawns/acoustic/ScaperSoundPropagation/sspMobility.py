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

from enum import Enum

from sspUtils import Coordinate, Mobility_Event


class MobilityModels(Enum):
    StationaryMobility = 1
    BonnMotionMobility = 2

    @classmethod
    def has_value(cls, value):
        return value in cls.__members__


class I_Mobility:
    def __init__(self, modelName : str):
        """
        Interface for all mobility models that are supported
        Parameters
        ----------
        modelName: str
            The name of the mobility model
        """
        if  MobilityModels.has_value(modelName):
            self.__modelName = modelName
        else:
            raise AttributeError('modelName does not match any model in enum MobilityModels!')

    def updateCurrentLocation(self, simulationTime: float, offset: Coordinate = Coordinate(0,0,0)):
        """
        Must be overwritten by derived classes.

        Parameters
        ----------
        simulationTime: float
            the time the event occurs
        offset: Coordinate
            the optional coordinate offset

        Returns
        -------

        """
        pass


class StationaryMobility(I_Mobility):
    def __init__(self, initialX: float, initialY: float, initialZ: float):
        """
        Implementation of a stationary mobility model at which the entity always stays at its initial location.
        Parameters
        ----------
        initialX: float
            initial x- coordinated
        initialY: float
            initial y coordination
        initialZ: float
            initial z coordination
        """
        super().__init__("StationaryMobility")
        self.__initialX = initialX
        self.__initialY = initialY
        self.__initialZ = initialZ
        self.mobilityEvents : [Mobility_Event] = [Mobility_Event(0.0, Coordinate(self.__initialX, self.__initialY, self.__initialZ))]
        self.currentLocation : Coordinate = self.mobilityEvents[0].coord

    def generateMobilityEvents(self)->[Mobility_Event]:
        return self.mobilityEvents

    def updateCurrentLocation(self, simulationTime: float, offset: Coordinate = Coordinate(0,0,0))-> None:
        """
        Update the nodes position. Use the offset to refer to a microphones position.

        Parameters
        ----------
        simulationTime: float
            the time the event occurs
        offset: Coordinate
             the optional coordinate offset of a microphone
        -------

        """
        absolute_difference_function = lambda listVal: abs(listVal.time - simulationTime)
        self.currentLocation = min(self.mobilityEvents, key=absolute_difference_function).coord + offset

    def __repr__(self):
        return "%s(%s | %r)" % (self.__class__, self.currentLocation, self.mobilityEvents)


class BonnMotionMobility(I_Mobility):
    def __init__(self, traceFile: str, nodeId : int):
        """
        Implementation of Omnetpp BonnMotionModel in which an entity moves according to a tracefile.

        Parameters
        ----------
        traceFile: str
            path to file
        nodeId:
            line in which the trace is specified
        """
        super().__init__("BonnMotionMobility")
        self.__trace = traceFile
        self.__nodeId = nodeId
        self.mobilityEvents : [Mobility_Event] = self.__generateMobilityEvents()
        self.currentLocation : Coordinate = self.mobilityEvents[0].coord


    def __generateMobilityEvents(self) -> [Mobility_Event] :
        """
        Generate the Mobility Events

        Returns: [Mobility_Event]
            List of Mobility_Events
        -------

        """
        events : [Mobility_Event] = []
        with open(self.__trace, 'r') as fd:
            lines = fd.readlines()
            trace = lines[self.__nodeId].strip()
            data = trace.split(" ")
            if (len(data) % 4) != 0:
                raise ValueError(f"Illigal trace {self.__trace}, might miss time or 3d coordinates!")
            for i in range(0, int(len(data) / 4)):
                time = float(data[i])
                if i == 0 and time != 0.0:
                    raise ValueError(f"Illigal trace {self.__trace}, first timestamp must be 0.0!")
                x = float(data[i+1])
                y = float(data[i+2])
                z = float(data[i+3])
                events.append(Mobility_Event(time, Coordinate(x,y,z)))
        return events

    def updateCurrentLocation(self, simulationTime: float, offset: Coordinate = Coordinate(0,0,0)):
        """
        Update current location according to simulation time and a offset


        Parameters
        ----------
        simulationTime: float
            the time the event occurs
        offset: Coordinate
             the optional coordinate offset of a microphone

        -------

        """
        absolute_difference_function = lambda listVal: abs(listVal.time - simulationTime)
        self.currentLocation = min(self.mobilityEvents, key=absolute_difference_function).coord + offset


    def getLocationAtTime(self, time: float):
        """
        Get the location at a specific time


        Parameters
        ----------
        time: float
            the simulation time in seconds

        Returns
        -------

        """
        absolute_difference_function = lambda listVal: abs(listVal.time - time)
        closest_value = min(self.mobilityEvents, key=absolute_difference_function)
        return closest_value

    def __repr__(self):
        return "%s(%s | %r)" % (self.__class__, self.currentLocation, self.mobilityEvents)