#include "PredicateConstructor.h"
#include "VehicleData.h"
#include "PredicateVehicle.h"
#include "PredicateValues.h"
#include <iostream>

using std::shared_ptr;
using std::unique_ptr;
using std::make_unique;
using std::make_shared;
using std::move;
using std::cout;
using std::endl;

PredicateConstructor::PredicateConstructor()
	: lastPredicates() {
}


PredicateConstructor::~PredicateConstructor() {
}

std::vector<shared_ptr<PredicateVehicle>> PredicateConstructor::transform(std::vector<std::unique_ptr<VehicleData>> data) {
	std::vector<shared_ptr<PredicateVehicle>> res;
	std::map<int, shared_ptr<PredicateVehicle>> currentPredicates;
	//take vut out of the data and process it
	unique_ptr<VehicleData> vut = move(data[0]);
	data.erase(data.begin());
	shared_ptr<PredicateVehicle> oldVUT = nullptr;
	Acc acc;
	if(lastPredicates.find(vut->getID()) != lastPredicates.end()) {
		oldVUT = lastPredicates.at(vut->getID());
		acc = getAccPredicate(*oldVUT, *vut);
	} else {
		acc = getAccPredicate(*vut);
	}
	shared_ptr<PredicateVehicle> newVUT = make_shared<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, (*vut).getLane(), acc, getLnChgPredicate(*vut), vut->isStable());
	currentPredicates[vut->getID()] = newVUT;
	res.push_back(newVUT);

	//generate predicates for the othe vehicles
	for(unique_ptr<VehicleData> &v : data) {
		RelX xPosPred;
		RelV vRelPred;
		Acc acc;
		if(lastPredicates.find(v->getID()) != lastPredicates.end()) {
			//vehicle exists
			shared_ptr<PredicateVehicle> oldData = lastPredicates.at(v->getID());
			xPosPred = getXPosPredicate(*oldData, *v, *vut);
			vRelPred = getRelVPredicate(*oldData, *v, *vut);
			acc = getAccPredicate(*oldData, *v);
		} else {
			//vehicle doesn't exist in oldData
			xPosPred = getXPosPredicate(*v, *vut);
			vRelPred = getRelVPredicate(*v, *vut);
			acc = getAccPredicate(*v);
		}
		shared_ptr<PredicateVehicle> p = make_shared<PredicateVehicle>(xPosPred, vRelPred, v->getLane(), acc, getLnChgPredicate(*v), v->isStable());
		res.push_back(p);
		currentPredicates[v->getID()] = p;
	}
	lastPredicates.clear();
	lastPredicates = currentPredicates;
	return res;
}

RelX PredicateConstructor::getXPosPredicate(PredicateVehicle const &oldData, VehicleData const &newData, VehicleData const &vut) {
	double dist = newData.getXPosition() - vut.getXPosition();
	double ratio = dist / vut.getVelocity();
	switch(oldData.getRelX()) {
	case RelX::BEHIND:
		if(ratio < -1.0) return RelX::BEHIND; //behind
		if(ratio < -0.25) return RelX::CLOSE_BEHIND; //close behind
		if(ratio < 0.5) return RelX::SAME_HEIGHT; //same height
		if(ratio < 1.25) return RelX::CLOSE_IN_FRONT; //close in front
		return RelX::IN_FRONT; //in front
	case RelX::CLOSE_BEHIND:
		if(ratio < -1.25) return RelX::BEHIND; //behind
		if(ratio < -0.25) return RelX::CLOSE_BEHIND; //close behind
		if(ratio < 0.5) return RelX::SAME_HEIGHT; //same height
		if(ratio < 1.25) return RelX::CLOSE_IN_FRONT; //close in front
		return RelX::IN_FRONT; //in front
	case RelX::SAME_HEIGHT:
		if(ratio < -1.0) return RelX::BEHIND; //behind
		if(ratio < -0.5) return RelX::CLOSE_BEHIND; //close behind
		if(ratio < 0.5) return RelX::SAME_HEIGHT; //same height
		if(ratio < 1.25) return RelX::CLOSE_IN_FRONT; //close in front
		return RelX::IN_FRONT; //in front
	case RelX::CLOSE_IN_FRONT:
		if(ratio < -1.0) return RelX::BEHIND; //behind
		if(ratio < -0.25) return RelX::CLOSE_BEHIND; //close behind
		if(ratio < 0.25) return RelX::SAME_HEIGHT; //same height
		if(ratio < 1.25) return RelX::CLOSE_IN_FRONT; //close in front
		return RelX::IN_FRONT; //in front
	case RelX::IN_FRONT:
		if(ratio < -1.0) return RelX::BEHIND; //behind
		if(ratio < -0.25) return RelX::CLOSE_BEHIND; //close behind
		if(ratio < 0.5) return RelX::SAME_HEIGHT; //same height
		if(ratio < 1.0) return RelX::CLOSE_IN_FRONT; //close in front
		return RelX::IN_FRONT; //in front
	default:
		//predicate id is unknown
		return getXPosPredicate(newData, vut);
	}
}

RelV PredicateConstructor::getRelVPredicate(PredicateVehicle const &oldData, VehicleData const &newData, VehicleData const &vut) {
	double vRel = newData.getVelocity() / vut.getVelocity();
	switch(oldData.getRelV()) {
	case RelV::SLOWER:
		if(vRel < 0.9) return RelV::SLOWER; //slower
		if(vRel < 1.25) return RelV::SAME; //same velocity
		return RelV::FASTER; //faster
	case RelV::SAME:
		if(vRel < 0.75) return RelV::SLOWER; //slower
		if(vRel < 1.25) return RelV::SAME; //same velocity
		return RelV::FASTER; //faster
	case RelV::FASTER:
		if(vRel < 0.75) return RelV::SLOWER; //slower
		if(vRel < 1.1) return RelV::SAME; //same velocity
		return RelV::FASTER; //faster
	default:
		return getRelVPredicate(newData, vut);
	}
}

RelX PredicateConstructor::getXPosPredicate(VehicleData const &newData, VehicleData const &vut) {
	double dist = newData.getXPosition() - vut.getXPosition();
	double ratio = dist / vut.getVelocity();
	if(ratio < -1.25) return RelX::BEHIND; //behind
	if(ratio < -0.5) return RelX::CLOSE_BEHIND; //close behind
	if(ratio < 0.5) return RelX::SAME_HEIGHT; //same height
	if(ratio < 1.25) return RelX::CLOSE_IN_FRONT; //close in front
	return RelX::IN_FRONT; //in front
}

RelV PredicateConstructor::getRelVPredicate(VehicleData const &newData, VehicleData const &vut) {
	double vRel = newData.getVelocity() / vut.getVelocity();
	if(vRel < 0.75) return RelV::SLOWER; //slower
	if(vRel < 1.25) return RelV::SAME; // same velocity
	return RelV::FASTER; //faster
}

Acc PredicateConstructor::getAccPredicate(PredicateVehicle const &oldData, VehicleData const &vut) {
	double acc = vut.getAcceleration();
	switch(oldData.getAcc()) {
	case Acc::BRAKE:
		if(acc < -1) return Acc::BRAKE; //braking
		if(acc < 2) return Acc::NONE; //no acceleration
		return Acc::ACCELERATE; //accelerating
	case Acc::NONE:
		if(acc < -2) return Acc::BRAKE; //braking
		if(acc < 2) return Acc::NONE; //no acceleration
		return Acc::ACCELERATE; //accelerating
	case Acc::ACCELERATE:
		if(acc < -2) return Acc::BRAKE; //braking
		if(acc < 1) return Acc::NONE; //no acceleration
		return Acc::ACCELERATE; //accelerating
	default:
		return getAccPredicate(vut);
	}
}

Acc PredicateConstructor::getAccPredicate(VehicleData const &vut) {
	double acc = vut.getAcceleration();
	if(acc < -2) return Acc::BRAKE; //braking
	if(acc < 2) return Acc::NONE; //no acceleration
	return Acc::ACCELERATE; //accelerating
}

LnChg PredicateConstructor::getLnChgPredicate(VehicleData const & vut) {
	int dif = vut.getLnChg();
	if(dif < 0) return LnChg::RIGHT;
	if(dif > 0) return LnChg::LEFT;
	return LnChg::NONE;
}