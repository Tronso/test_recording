#include "stdafx.h"
#include "CppUnitTest.h"
#include "DataProcessor.h"
#include <memory>
#include "StubDataManager.h"
#include <vector>
#include "VehicleData.h"
#include "Situation.h"
#include "PredicateValues.h"
#include "PredicateVehicle.h"
#include <map>

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::map;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestDataProcessor {

	TEST_CLASS(IntegrationTestDataManager) {

		TEST_METHOD(IT_DP_10) {
			unique_ptr<DataManager> dm = make_unique<StubDataManager>();
			DataProcessor dp(move(dm));
			vector<unique_ptr<VehicleData>> vehicles;
			vehicles.push_back(move(make_unique<VehicleData>(1, 100, 0, 1, 0, 0, 100, 1)));

			shared_ptr<PredicateVehicle> expectedVUT = make_shared<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true);
			vector<shared_ptr<PredicateVehicle>> expectedVehicles;

			Situation expectedCurrentSituation(expectedVUT, expectedVehicles, false);
			dp.processData(move(vehicles));
			dp.stop();
			vector<shared_ptr<Transition>> result = (dynamic_cast<StubDataManager&>(*dp.getDataManager())).getInputs();
			shared_ptr<Situation> curr = dp.getCurrentSituation();
			Logger::WriteMessage(curr->toString().c_str());
			Logger::WriteMessage(expectedCurrentSituation.toString().c_str());
			Assert::IsTrue(*dp.getCurrentSituation() == expectedCurrentSituation, L"currentSituation does not equal expected value", LINE_INFO());
			Assert::IsTrue(result.size() == 0);
		}

		TEST_METHOD(IT_DP_20) {
			unique_ptr<DataManager> dm = make_unique<StubDataManager>();
			DataProcessor dp(move(dm));

			vector<unique_ptr<VehicleData>> input;
			input.push_back(make_unique<VehicleData>(1, 100, 0, 1, 0, 0, 100, 1));
			vector<unique_ptr<VehicleData>> input2;
			input2.push_back(move(make_unique<VehicleData>(1, 100, 0, 1, 0, 0, 100, 1)));

			vector<shared_ptr<PredicateVehicle>> expectedVehicles;
			shared_ptr<PredicateVehicle> expectedVUT = make_shared<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true);
			Situation expectedCurrentSituation(expectedVUT, expectedVehicles, false);

			dp.processData(move(input));
			dp.processData(move(input2));
			dp.stop();

			vector<shared_ptr<Transition>> result = (dynamic_cast<StubDataManager&>(*dp.getDataManager())).getInputs();
			Assert::IsTrue(*dp.getCurrentSituation() == expectedCurrentSituation);
			Assert::IsTrue(result.size() == 0);
		}

		TEST_METHOD(IT_DP_30) {
			unique_ptr<DataManager> dm = make_unique<StubDataManager>();
			DataProcessor dp(move(dm));

			vector<unique_ptr<VehicleData>> input;
			input.push_back(move(make_unique<VehicleData>(1, 100, 0, 1, 0, 0, 100, 1)));
			vector<unique_ptr<VehicleData>> input2;
			input2.push_back(make_unique<VehicleData>(1, 100, 0, 2, 0, 0, 100, 1));

			vector<shared_ptr<PredicateVehicle>> expectedVehicles;
			shared_ptr<PredicateVehicle> expectedVUT = make_shared<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 2, Acc::NONE, LnChg::NONE, false);
			Situation expectedCurrentSituation(expectedVUT, expectedVehicles, false);

			dp.processData(move(input));
			dp.processData(move(input2));
			dp.stop();
			Logger::WriteMessage(dp.getCurrentSituation()->toString().c_str());
			Logger::WriteMessage(expectedCurrentSituation.toString().c_str());
			vector<shared_ptr<Transition>> result = (dynamic_cast<StubDataManager&>(*dp.getDataManager())).getInputs();
			Assert::IsTrue(*dp.getCurrentSituation() == expectedCurrentSituation, L"currentSituation does not equal expected value", LINE_INFO());
			Assert::IsTrue(result.size() == 1, L"result Size was not 1", LINE_INFO());
		}
	};
}