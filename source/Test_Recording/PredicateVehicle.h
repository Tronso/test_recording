#pragma once
#include <string>
#include "PredicateValues.h"

/**
* A PredicateVehicle is the representation of a vehicle object with predicates which are represented by IDs.
*/
class PredicateVehicle {
public:

	/**
	* Instantiates a predicate vehicle with the given values
	* @param id The ID of the vehicle.
	* @param relX The RelX predicate of the vehicle
	* @param relV The RelV predicate of the vehicle
	* @param lane The lane of the vehicle. Starting with 1 on the right
	* @param acc The Acc predicate of the vehicle
	* @param lnChg The LnChg predicate of the vehicle
	* @param stable Flag for the stable attribute of the VehicleData this PredicateVehicle is based on
	*/
	PredicateVehicle(RelX relX, RelV relV, int lane, Acc acc, LnChg lnChg, bool stable);

	/**
	* Standard deconstructor.
	*/
	~PredicateVehicle();

	/**
	 * Override operator ==
	 * True iff all attributes are equal.
	 */
	bool operator==(const PredicateVehicle &other) const;

	/**
	* Override operator !=
	* True iff == returns false.
	*/
	bool operator!=(const PredicateVehicle &other) const;

	/**
	* Getter for the relative x position predicate of the vehicle
	* @return ID of the predicate for the relative x position
	*/
	RelX getRelX() const;

	/**
	* Getter for the relatie velocity predicate of the vehicle
	* @return ID of the predicate for the relative velocity.
	*/
	RelV getRelV() const;

	/**
	* Getter for the lane of the vehicle
	* @return Lane of the vehicle starting with 0 oon the furthest right lane.
	*/
	int getLane() const;

	/**
	* Getter for the acc predicate of the vehicle
	* @return ID of the predicate for the acc of the vut
	*/
	Acc getAcc() const;

	/**
	* Getter for the lnChg predicate of the vehicle
	* @return ID of the predicate for the lnChg of the vut
	*/
	LnChg getLnChg() const;

	/**
	* Getter for the stable attribute
	* @returns True, if the desired lane is achieved, the desired velocity is achieved, no lane change is taking place and no acceleration is taking place. Else fale.
	*/
	bool isStable() const;

	std::wstring toWString() const;

	std::string toString() const;

private:
	RelX relX;
	RelV relV;
	int lane;
	Acc acc;
	LnChg lnChg;
	bool stable;
};

