#include "PredicateVehicle.h"
#include "PredicateValues.h"

PredicateVehicle::PredicateVehicle(RelX relX, RelV relV, int lane, Acc acc, LnChg lnChg, bool stable)
	: relX(relX)
	, relV(relV)
	, lane(lane)
	, acc(acc)
	, lnChg(lnChg)
	, stable(stable && acc == Acc::NONE && lnChg == LnChg::NONE){
}

PredicateVehicle::~PredicateVehicle() {
}

bool PredicateVehicle::operator==(const PredicateVehicle &other) const {
	return relX == other.relX &&
		relV == other.relV &&
		lane == other.lane && 
		acc == other.acc &&
		lnChg == other.lnChg &&
		stable == other.stable;
}

bool PredicateVehicle::operator!=(const PredicateVehicle &other) const {
	return !(*this == other);
}

RelX PredicateVehicle::getRelX() const {
	return relX;
}

RelV PredicateVehicle::getRelV() const {
	return relV;
}

int PredicateVehicle::getLane() const {
	return lane;
}

Acc PredicateVehicle::getAcc() const {
	return acc;
}

LnChg PredicateVehicle::getLnChg() const {
	return lnChg;
}

bool PredicateVehicle::isStable() const {
	return stable;
}

std::wstring PredicateVehicle::toWString() const {
	std::wstring res(L"(" + std::to_wstring(static_cast<int>(relX)) + L", " + std::to_wstring(static_cast<int>(relV)) + L", " + std::to_wstring(lane) + L", " + std::to_wstring(static_cast<int>(acc)) + L", " + std::to_wstring(static_cast<int>(lnChg)) + L")");
	return res;
}

std::string PredicateVehicle::toString() const {
	std::string res("(" + std::to_string(static_cast<int>(relX)) + ", " + std::to_string(static_cast<int>(relV)) + ", " + std::to_string(lane) + ", " + std::to_string(static_cast<int>(acc)) + ", " + std::to_string(static_cast<int>(lnChg)) + ")");
	return res;
}
