#include "stdafx.h"
#include "CppUnitTest.h"
#include "SQLiteManager.h"
#include "Transition.h"
#include "Situation.h"
#include <vector>
#include "PredicateVehicle.h"
#include "PredicateValues.h"
#include "SituationGraph.h"
#include <memory>
#include <map>

using std::unique_ptr;
using std::shared_ptr;
using std::move;
using std::make_unique;
using std::make_shared;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DataManagerTest {

	TEST_CLASS(ComponentTestDataManager) {

		TEST_METHOD(CT_DM_10) {
			//database file should not exists at the beginning
			remove("CT_DM_10.db");
			unique_ptr<SQLiteManager> dm = make_unique<SQLiteManager>("CT_DM_10.db");
			std::vector<shared_ptr<PredicateVehicle>> fromVehicles;
			shared_ptr<PredicateVehicle> fromVUT = make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true);
			std::vector<shared_ptr<PredicateVehicle>> toVehicles;
			shared_ptr<PredicateVehicle> toVUT = make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 2, Acc::NONE, LnChg::NONE, true);
			shared_ptr<Situation> from = make_shared<Situation>(fromVUT, fromVehicles, true);
			shared_ptr<Situation> to = make_shared<Situation>(toVUT, toVehicles, false);
			unique_ptr<Transition> t = make_unique<Transition>(from, to);
			dm->storeTransition(*t);
			//check with an administration tool if the transition is inserted correctly
		}

		TEST_METHOD(CT_DM_20) {
			remove("CT_DM_20.db");
			//database file should not exists at the beginning
			unique_ptr<SQLiteManager> dm = make_unique<SQLiteManager>("CT_DM_20.db");
			std::vector<shared_ptr<PredicateVehicle>> fromVehicles;
			shared_ptr<PredicateVehicle> fromVUT = make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true);
			std::vector<shared_ptr<PredicateVehicle>> toVehicles;
			shared_ptr<PredicateVehicle> toVUT = make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 2, Acc::NONE, LnChg::NONE, true);
			shared_ptr<Situation> from = make_shared<Situation>(fromVUT, fromVehicles, true);
			shared_ptr<Situation> to = make_shared<Situation>(toVUT, toVehicles, false);
			unique_ptr<Transition> t = make_unique<Transition>(from, to);
			dm->storeTransition(*t);
			std::vector<shared_ptr<PredicateVehicle>> toVehicles2;
			shared_ptr<PredicateVehicle> toVUT2 = make_shared<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 3, Acc::NONE, LnChg::NONE, true);
			shared_ptr<Situation> to2 = make_unique<Situation>(toVUT2, toVehicles2, false);
			t = make_unique<Transition>(from, to2);
			dm->storeTransition(*t);
			//check with an administration tool if the transition is inserted correctly
		}

		TEST_METHOD(CT_DM_30) {
			remove("CT_DM_30.db");
			//database file should not exists at the beginning
			unique_ptr<SQLiteManager> dm = make_unique<SQLiteManager>("CT_DM_30.db");
			std::vector<shared_ptr<PredicateVehicle>> fromVehicles;
			shared_ptr<PredicateVehicle> fromVUT = make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true);
			std::vector<shared_ptr<PredicateVehicle>> toVehicles;
			shared_ptr<PredicateVehicle> toVUT = make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 2, Acc::NONE, LnChg::NONE, true);
			shared_ptr<Situation> from = make_shared<Situation>(fromVUT, fromVehicles, true);
			shared_ptr<Situation> to = make_shared<Situation>(toVUT, toVehicles, false);
			unique_ptr<Transition> t = make_unique<Transition>(from, to);
			dm->storeTransition(*t);
			dm->storeTransition(*t);
			std::vector<shared_ptr<PredicateVehicle>> toVehicles2;
			shared_ptr<PredicateVehicle> toVUT2 = make_shared<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 3, Acc::NONE, LnChg::NONE, true);
			shared_ptr<Situation> to2 = make_unique<Situation>(toVUT2, toVehicles2, true);
			t = make_unique<Transition>(from, to2);
			dm->storeTransition(*t);
			//check with an administration tool if the transition is inserted correctly
		}

		TEST_METHOD(CT_DM_40) {
			unique_ptr<SQLiteManager> dm = make_unique<SQLiteManager>("CT_DM_40.db");
			unique_ptr<SituationGraph> sg = move(dm->retrieveSituationGraph());
			sg->saveAsPDF("CT_DM_40.pdf");
			//look at PDF to evaluate Test
		}

		TEST_METHOD(CT_DM_50) {
			unique_ptr<SQLiteManager> dm = make_unique<SQLiteManager>("CT_DM_40.db");
			std::vector<shared_ptr<PredicateVehicle>> fromVehicles;
			shared_ptr<PredicateVehicle> fromVUT = make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true);
			shared_ptr<Situation> root = make_shared<Situation>(fromVUT, fromVehicles, true);
			unique_ptr<SituationGraph> sg = move(dm->retrieveSubSituationGraph(root));
			sg->saveAsPDF("CT_DM_50.pdf");
			//look at PDF to evaluate Test
		}

		TEST_METHOD(CT_DM_60) {
			unique_ptr<SQLiteManager> dm = make_unique<SQLiteManager>("CT_DM_60.db");
			std::vector<shared_ptr<PredicateVehicle>> fromVehicles;
			shared_ptr<PredicateVehicle> fromVUT = make_unique<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true);
			shared_ptr<Situation> root = make_shared<Situation>(fromVUT, fromVehicles, true);
			unique_ptr<SituationGraph> sg = move(dm->retrieveSubSituationGraph(root));
			Assert::IsTrue(sg == nullptr, L"SituationGraph is null", LINE_INFO());
		}
	};
}