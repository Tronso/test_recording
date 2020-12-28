#pragma once

#include "recording_api.h"

/**
* This class represents the raw data of a simulated vehicle.
*/
class VehicleData {
public:
	/**
	* Constructor for VehicleData. Initializes the members with the given values.
	*/
	RECORDING_API VehicleData(int id, double velocity, double xPosition, int lane, double acceleration, int lnChg, double desiredVelocity, int desiredLane);

	/**
	* Defualt Deconstructor
	*/
	RECORDING_API ~VehicleData();

	/**
	* Getter for the ID of the vehicle
	* @return ID of the vehicle.
	*/
	int getID() const;

	/**
	* Getter for the velocity of the vehicle in km/h
	* @return Velocity of the vehicle in km/h
	*/
	double getVelocity() const;

	/**
	* Getter for the x position on the road in meters
	* @return X position of the vehicle in m
	*/
	double getXPosition() const;

	/**
	* Getter for the lane of the vehicle. Starts with 1 on the furthest right lane.
	* @return lane of the vehicle.
	*/
	int getLane() const;

	/**
	 * Getter for the acceleration of the vehicle in m/s^2.
	 * @return Acceleration of the vehicle in m/s^2.
	 */
	double getAcceleration() const;

	/**
	* Getter for the desired lane.
	* @return The number of the desired lane the vehicle wants to change to.
	*/
	int getLnChg() const;

	/**
	* Determines if the vehicle is stable.
	* The vehicle is considered stable if it drives on its desired lane and the velocity does not differ from the desired velocity more than 5%.
	* @return true if the vehicle is stable, false else.
	*/
	bool isStable();

private:
	const int id;
	const double velocity;
	const double xPosition;
	const int lane;
	const double acceleration;
	const int lnChg;
	const bool stable;
};
