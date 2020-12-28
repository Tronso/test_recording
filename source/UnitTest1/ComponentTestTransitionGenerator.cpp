#include "stdafx.h"
#include "CppUnitTest.h"
#include "TransitionGenerator.h"
#include "Transition.h"
#include "Situation.h"
#include "PredicateVehicle.h"
#include "PredicateValues.h"
#include<memory>

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using std::move;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestTransitionGenerator {
	TEST_CLASS(ComponentTestTransitionGenerator) {

		BEGIN_TEST_METHOD_ATTRIBUTE(CT_TG_10)
			TEST_DESCRIPTION("Equal Situations")
			END_TEST_METHOD_ATTRIBUTE()

			TEST_METHOD(CT_TG_10) {
			shared_ptr<PredicateVehicle> fromVUT = make_shared<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true);;
			std::vector<shared_ptr<PredicateVehicle>> fromVehicles;
			shared_ptr<Situation> from = make_shared<Situation>(fromVUT, fromVehicles, true);
			shared_ptr<Situation> to = make_shared<Situation>(fromVUT, fromVehicles, true);

			unique_ptr<TransitionGenerator> tg = make_unique<TransitionGenerator>();
			unique_ptr<Transition> result = move(tg->generate(from, to));
			if(result == nullptr) {
				Logger::WriteMessage("result is NULL");
			} else {
				Logger::WriteMessage("result is not NULL");
			}
			Assert::IsTrue(result == nullptr);
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(CT_TG_20)
			TEST_DESCRIPTION("Different Situations")
			END_TEST_METHOD_ATTRIBUTE()

			TEST_METHOD(CT_TG_20) {
			std::vector<shared_ptr<PredicateVehicle>> fromVehicles;
			shared_ptr<PredicateVehicle> fromVUT = (make_shared<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 1, Acc::NONE, LnChg::NONE, true));
			shared_ptr<Situation> from = make_shared<Situation>(fromVUT, fromVehicles, true);
			std::vector<shared_ptr<PredicateVehicle>> toVehicles;
			shared_ptr<PredicateVehicle> toVUT = make_shared<PredicateVehicle>(RelX::SAME_HEIGHT, RelV::SAME, 2, Acc::NONE, LnChg::NONE, true);
			shared_ptr<Situation> to = make_shared<Situation>(toVUT, toVehicles, false);

			unique_ptr<Transition> expected = make_unique<Transition>(from, to);

			unique_ptr<TransitionGenerator> tg = make_unique<TransitionGenerator>();
			unique_ptr<Transition> result = tg->generate(from, to);
			Assert::IsTrue(result != nullptr);
			Assert::IsTrue(*expected == *result, L"Transitions are not equal", LINE_INFO());
		}
	};
}