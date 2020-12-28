#pragma once
#include<vector>
#include<map>
#include<memory>
#include "PredicateValues.h"

class PredicateVehicle;
class VehicleData;
class Situation;

/**
* A PredicateConstructor uses hystereses to transform raw data to predicate data.
*/
class PredicateConstructor {
public:
	/**
	* Standard constructor.
	*/
	PredicateConstructor();
	/**
	* Standard destructor.
	*/
	~PredicateConstructor();

	/**
	* For each vehicle the data transformed to predicates using hystereses and old data.
	* @param data Vector of VehicleData that will be transformed to predicates.
	* @return Map of the ID of the vehicle in the situation to the transformed PredicateVehicles
	*/
	std::vector<std::shared_ptr<PredicateVehicle>> transform(std::vector<std::unique_ptr<VehicleData>> data);

private:
	//map of Simulation IDs to Predicates from the last step
	std::map<int, std::shared_ptr<PredicateVehicle>> lastPredicates;

	//uses old state and hystereses to find the new predicate
	RelX getXPosPredicate(PredicateVehicle const &oldData, VehicleData const &newData, VehicleData const &vut);
	//uses old state and hystereses to find the new predicate
	RelV getRelVPredicate(PredicateVehicle const &oldData, VehicleData const &newData, VehicleData const &vut);
	//creates a predicate without the old data or hystereses
	RelX getXPosPredicate(VehicleData const &newData, VehicleData const &vut);
	//creates a predicate without the old data or hystereses
	RelV getRelVPredicate(VehicleData const &newData, VehicleData const &vut);
	//uses old state and hystereses to find the new predicate
	Acc getAccPredicate(PredicateVehicle const &oldData, VehicleData const &vut);
	//creates a predicate without the old data or hystereses
	Acc getAccPredicate(VehicleData const &vut);
	//creates a predicate without the old data or hystereses
	LnChg getLnChgPredicate(VehicleData const &vut);
};

