#include "stdafx.h"
#include "CppUnitTest.h"
#include "PredicateConstructor.h"
#include "VehicleData.h"
#include "PredicateVehicle.h"
#include "PredicateValues.h"
#include <iostream>
#include <memory>
#include <map>
#include <algorithm>

using std::unique_ptr;
using std::shared_ptr;
using std::move;
using std::make_unique;
using std::make_shared;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestPredicateConstructor {

	static void assertEqualVector(std::vector<shared_ptr<PredicateVehicle>> const& result, std::vector<unique_ptr<PredicateVehicle>> const& expected) {
		Assert::IsTrue(result.size() == expected.size());
		std::string expectedString("Expected: ");
		/*
		for(unique_ptr<PredicateVehicle> const &v : expected) {
		expectedString += v->toString() + ", ";
		}
		Logger::WriteMessage(expectedString.c_str());
		std::string resultString("Result: ");
		for(shared_ptr<PredicateVehicle> const &v : result) {
		resultString += v->toString() + ", ";
		}
		Logger::WriteMessage(resultString.c_str());
		*/
		for(shared_ptr<PredicateVehicle> entry : result) {
			Assert::IsTrue(std::find_if(expected.begin(), expected.end(), [&](const unique_ptr<PredicateVehicle> &v) { return *entry == *v; }) != expected.end());
		}
	}

	TEST_CLASS(ComponentTestsPredicateConstructor) {
public:

	TEST_METHOD(CT_PC_10) {
		unique_ptr<PredicateConstructor> pc = make_unique<PredicateConstructor>();
		std::vector<unique_ptr<VehicleData>> input;
		input.push_back(make_unique<VehicleData>(0, 100, 0, 1, 0, 0, 100, 1));

		std::vector<unique_ptr<PredicateVehicle>> expected;
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));

		std::vector<shared_ptr<PredicateVehicle>> result = pc->transform(move(input));
		assertEqualVector(result, expected);
	}

	TEST_METHOD(CT_PC_20) {
		unique_ptr<PredicateConstructor> pc = make_unique<PredicateConstructor>();
		std::vector<unique_ptr<VehicleData>> input;
		input.push_back(make_unique<VehicleData>(0, 100, 0, 1, 3, 0, 100, 1));

		std::vector<unique_ptr<PredicateVehicle>> expected;
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::ACCELERATE, LnChg::NONE, true));

		std::vector<shared_ptr<PredicateVehicle>> result = pc->transform(move(input));
		assertEqualVector(result, expected);
	}

	TEST_METHOD(CT_PC_30) {
		unique_ptr<PredicateConstructor> pc = make_unique<PredicateConstructor>();
		std::vector<unique_ptr<VehicleData>> input;
		input.push_back(make_unique<VehicleData>(0, 100, 200, 1, 0, 0, 100, 1));	//VUT
		input.push_back(make_unique<VehicleData>(1, 100, 70, 1, 0, 0, 100, 1));	//behind
		input.push_back(make_unique<VehicleData>(2, 100, 140, 1, 0, 0, 100, 1));	//close behind
		input.push_back(make_unique<VehicleData>(3, 100, 180, 1, 0, 0, 100, 1));	//same height
		input.push_back(make_unique<VehicleData>(4, 100, 260, 1, 0, 0, 100, 1));	//close in front
		input.push_back(make_unique<VehicleData>(5, 100, 330, 1, 0, 0, 100, 1));	//in front


		std::vector<unique_ptr<PredicateVehicle>> expected;
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::BEHIND, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::CLOSE_BEHIND, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::CLOSE_IN_FRONT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::IN_FRONT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));

		std::vector<shared_ptr<PredicateVehicle>> result = pc->transform(move(input));
		assertEqualVector(result, expected);
	}

	TEST_METHOD(CT_PC_40) {
		unique_ptr<PredicateConstructor> pc = make_unique<PredicateConstructor>();
		std::vector<unique_ptr<VehicleData>> input;
		input.push_back(make_unique<VehicleData>(0, 100, 0, 1, 0, 0, 100, 1));	//VUT
		input.push_back(make_unique<VehicleData>(1, 70, 0, 1, 0, 0, 70, 1));	//slower
		input.push_back(make_unique<VehicleData>(2, 100, 0, 1, 0, 0, 100, 1));	//same
		input.push_back(make_unique<VehicleData>(3, 130, 0, 1, 0, 0, 130, 1));	//faster

		std::vector<unique_ptr<PredicateVehicle>> expected;
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SLOWER, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::FASTER, 1, Acc::NONE, LnChg::NONE, true));

		std::vector<shared_ptr<PredicateVehicle>> result = pc->transform(move(input));
		assertEqualVector(result, expected);
	}

	TEST_METHOD(CT_PC_50) {
		unique_ptr<PredicateConstructor> pc = make_unique<PredicateConstructor>();
		std::vector<unique_ptr<VehicleData>> existing;
		//fill in current data. requires CT_PC_10, CT_PC_30 to be succesfull
		existing.push_back(make_unique<VehicleData>(0, 100, 200, 1, 0, 0, 100, 1));	//VUT
		existing.push_back(make_unique<VehicleData>(1, 100, 70, 1, 0, 0, 100, 1));	//behind
		existing.push_back(make_unique<VehicleData>(2, 100, 140, 1, 0, 0, 100, 1));	//close behind
		existing.push_back(make_unique<VehicleData>(3, 100, 140, 1, 0, 0, 100, 1));	//close behind
		existing.push_back(make_unique<VehicleData>(4, 100, 180, 1, 0, 0, 100, 1));	//same height
		existing.push_back(make_unique<VehicleData>(5, 100, 180, 1, 0, 0, 100, 1));	//same height
		existing.push_back(make_unique<VehicleData>(6, 100, 260, 1, 0, 0, 100, 1));	//close in front
		existing.push_back(make_unique<VehicleData>(7, 100, 260, 1, 0, 0, 100, 1));	//close in front
		existing.push_back(make_unique<VehicleData>(8, 100, 330, 1, 0, 0, 100, 1));	//in front
		pc->transform(move(existing));

		std::vector<unique_ptr<VehicleData>> input;
		input.push_back(make_unique<VehicleData>(0, 100, 200, 1, 0, 0, 100, 1));	//VUT
		input.push_back(make_unique<VehicleData>(1, 100, 90, 1, 0, 0, 100, 1));		//behind
		input.push_back(make_unique<VehicleData>(2, 100, 90, 1, 0, 0, 100, 1));	//close behind
		input.push_back(make_unique<VehicleData>(3, 100, 170, 1, 0, 0, 100, 1));	//close behind
		input.push_back(make_unique<VehicleData>(4, 100, 170, 1, 0, 0, 100, 1));	//same height
		input.push_back(make_unique<VehicleData>(5, 100, 230, 1, 0, 0, 100, 1));	//same height
		input.push_back(make_unique<VehicleData>(6, 100, 230, 1, 0, 0, 100, 1));	//close in front
		input.push_back(make_unique<VehicleData>(7, 100, 310, 1, 0, 0, 100, 1));	//close in front
		input.push_back(make_unique<VehicleData>(8, 100, 310, 1, 0, 0, 100, 1));	//in front

		std::vector<unique_ptr<PredicateVehicle>> expected;
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::BEHIND, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::CLOSE_BEHIND, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::CLOSE_BEHIND, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::CLOSE_IN_FRONT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::CLOSE_IN_FRONT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::IN_FRONT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));

		std::vector<shared_ptr<PredicateVehicle>> result = pc->transform(move(input));
		assertEqualVector(result, expected);
	}

	TEST_METHOD(CT_PC_60) {
		unique_ptr<PredicateConstructor> pc = make_unique<PredicateConstructor>();
		std::vector<unique_ptr<VehicleData>> existing;
		//fill in current data. requires CT_PC_10, CT_PC_40 to be succesfull
		existing.push_back(make_unique<VehicleData>(0, 100, 200, 1, 0, 0, 100, 1));	//VUT
		existing.push_back(make_unique<VehicleData>(1, 70, 200, 1, 0, 0, 70, 1));	//slower
		existing.push_back(make_unique<VehicleData>(2, 100, 200, 1, 0, 0, 100, 1));	//same
		existing.push_back(make_unique<VehicleData>(3, 100, 200, 1, 0, 0, 100, 1));	//same
		existing.push_back(make_unique<VehicleData>(4, 130, 200, 1, 0, 0, 130, 1));	//faster
		pc->transform(move(existing));

		std::vector<unique_ptr<VehicleData>> input;
		input.push_back(make_unique<VehicleData>(0, 100, 200, 1, 0, 0, 100, 1));	//VUT
		input.push_back(make_unique<VehicleData>(1, 85, 200, 1, 0, 0, 85, 1));	//slower
		input.push_back(make_unique<VehicleData>(2, 85, 200, 1, 0, 0, 85, 1));	//same
		input.push_back(make_unique<VehicleData>(3, 120, 200, 1, 0, 0, 120, 1));	//same
		input.push_back(make_unique<VehicleData>(4, 120, 200, 1, 0, 0, 120, 1));	//faster

		std::vector<unique_ptr<PredicateVehicle>> expected;
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SLOWER, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::FASTER, 1, Acc::NONE, LnChg::NONE, true));

		std::vector<shared_ptr<PredicateVehicle>> result = pc->transform(move(input));
		assertEqualVector(result, expected);
	}

	TEST_METHOD(CT_PC_70) {
		unique_ptr<PredicateConstructor> pc = make_unique<PredicateConstructor>();
		std::vector<unique_ptr<VehicleData>> existing;
		//fill in current data. requires CT_PC_10, CT_PC_30 to be succesfull
		existing.push_back(make_unique<VehicleData>(0, 100, 200, 1, 0, 0, 100, 1));	//VUT
		existing.push_back(make_unique<VehicleData>(1, 100, 70, 1, 0, 0, 100, 1));	//behind
		existing.push_back(make_unique<VehicleData>(2, 100, 140, 1, 0, 0, 100, 1));	//close behind
		existing.push_back(make_unique<VehicleData>(3, 100, 140, 1, 0, 0, 100, 1));	//close behind
		existing.push_back(make_unique<VehicleData>(4, 100, 180, 1, 0, 0, 100, 1));	//same height
		existing.push_back(make_unique<VehicleData>(5, 100, 180, 1, 0, 0, 100, 1));	//same height
		existing.push_back(make_unique<VehicleData>(6, 100, 220, 1, 0, 0, 100, 1));	//close in front
		existing.push_back(make_unique<VehicleData>(7, 100, 220, 1, 0, 0, 100, 1));	//close in front
		existing.push_back(make_unique<VehicleData>(8, 100, 330, 1, 0, 0, 100, 1));	//in front
		pc->transform(move(existing));

		std::vector<unique_ptr<VehicleData>> input;
		input.push_back(make_unique<VehicleData>(0, 100, 200, 1, 0, 0, 100, 1));	//VUT
		input.push_back(make_unique<VehicleData>(1, 100, 110, 1, 0, 0, 100, 1));	//close behind
		input.push_back(make_unique<VehicleData>(2, 100, 70, 1, 0, 0, 100, 1));		//behind
		input.push_back(make_unique<VehicleData>(3, 100, 180, 1, 0, 0, 100, 1));	//same height
		input.push_back(make_unique<VehicleData>(4, 100, 140, 1, 0, 0, 100, 1));	//close behind
		input.push_back(make_unique<VehicleData>(5, 100, 260, 1, 0, 0, 100, 1));	//close in front
		input.push_back(make_unique<VehicleData>(6, 100, 220, 1, 0, 0, 100, 1));	//same height
		input.push_back(make_unique<VehicleData>(7, 100, 330, 1, 0, 0, 100, 1));	//in front
		input.push_back(make_unique<VehicleData>(8, 100, 290, 1, 0, 0, 100, 1));	//close in front

		std::vector<unique_ptr<PredicateVehicle>> expected;
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::CLOSE_BEHIND, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::BEHIND, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::CLOSE_BEHIND, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::CLOSE_IN_FRONT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::IN_FRONT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::CLOSE_IN_FRONT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));

		std::vector<shared_ptr<PredicateVehicle>> result = pc->transform(move(input));
		assertEqualVector(result, expected);
	}

	TEST_METHOD(CT_PC_80) {
		unique_ptr<PredicateConstructor> pc = make_unique<PredicateConstructor>();
		std::vector<unique_ptr<VehicleData>> existing;
		//fill in current data. requires CT_PC_10, CT_PC_40 to be succesfull
		existing.push_back(make_unique<VehicleData>(0, 100, 200, 1, 0, 0, 100, 1));	//VUT
		existing.push_back(make_unique<VehicleData>(1, 70, 200, 1, 0, 0, 70, 1));	//slower
		existing.push_back(make_unique<VehicleData>(2, 100, 200, 1, 0, 0, 100, 1));	//same
		existing.push_back(make_unique<VehicleData>(3, 100, 200, 1, 0, 0, 100, 1));	//same
		existing.push_back(make_unique<VehicleData>(4, 130, 200, 1, 0, 0, 130, 1));	//faster
		pc->transform(move(existing));

		std::vector<unique_ptr<VehicleData>> input;
		input.push_back(make_unique<VehicleData>(0, 100, 200, 1, 0, 0, 100, 1));	//VUT
		input.push_back(make_unique<VehicleData>(1, 95, 200, 1, 0, 0, 95, 1));		//same
		input.push_back(make_unique<VehicleData>(2, 70, 200, 1, 0, 0, 70, 1));		//slower
		input.push_back(make_unique<VehicleData>(3, 130, 200, 1, 0, 0, 130, 1));	//faster
		input.push_back(make_unique<VehicleData>(4, 105, 200, 1, 0, 0, 105, 1));	//same

		std::vector<unique_ptr<PredicateVehicle>> expected;
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SLOWER, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::FASTER, 1, Acc::NONE, LnChg::NONE, true));
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));

		std::vector<shared_ptr<PredicateVehicle>> result = pc->transform(move(input));
		assertEqualVector(result, expected);
	}

	TEST_METHOD(CT_PC_90) {
		unique_ptr<PredicateConstructor> pc = make_unique<PredicateConstructor>();
		std::vector<unique_ptr<VehicleData>> input;
		input.push_back(make_unique<VehicleData>(0, 100, 0, 1, 0, 1, 100, 1));

		std::vector<unique_ptr<PredicateVehicle>> expected;
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::LEFT, true));

		std::vector<shared_ptr<PredicateVehicle>> result = pc->transform(move(input));
		assertEqualVector(result, expected);
	}

	TEST_METHOD(CT_PC_100) {
		unique_ptr<PredicateConstructor> pc = make_unique<PredicateConstructor>();
		std::vector<unique_ptr<VehicleData>> input;
		input.push_back(make_unique<VehicleData>(0, 100, 0, 1, 3, 1, 100, 1));

		std::vector<unique_ptr<PredicateVehicle>> expected;
		expected.push_back(make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::ACCELERATE, LnChg::LEFT, true));

		std::vector<shared_ptr<PredicateVehicle>> result = pc->transform(move(input));
		assertEqualVector(result, expected);
	}
	};
}