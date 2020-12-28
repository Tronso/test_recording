#include "VehicleData.h"
#include<cmath>
#include <iostream>

using std::cout;
using std::endl;

VehicleData::VehicleData(int id, double velocity, double xPosition, int lane, double acceleration, int lnChg, double desiredVelocity, int desiredLane)
	: id(id)
	, velocity(velocity)
	, xPosition(xPosition)
	, lane(lane)
	, acceleration(acceleration)
	, lnChg(lnChg)
	, stable((lane == desiredLane || desiredLane == 0) && abs(velocity - desiredVelocity) / velocity <= 0.05) {
}

VehicleData::~VehicleData() {
}

int VehicleData::getID() const {
	return id;
}

double VehicleData::getVelocity() const {
	return velocity;
}

double VehicleData::getXPosition() const {
	return xPosition;
}

int VehicleData::getLane() const {
	return lane;
}

double VehicleData::getAcceleration() const {
	return acceleration;
}

int VehicleData::getLnChg() const {
	return lnChg;
}

bool VehicleData::isStable() {
	return stable;
}
