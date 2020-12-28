#include "stdafx.h"
#include "CppUnitTest.h"
#include "Simulation.h"
#include "DataProcessor.h"
#include "StubDataProcessor.h"
#include<memory>

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestSimulation {
	TEST_CLASS(ComponentTestsSimulation) {
public:

	TEST_METHOD(CT_S_10) {
		Simulation sim("", "ShortCircle");
		unique_ptr<StubDataProcessor> dp = make_unique<StubDataProcessor>();
		sim.initialize();
		sim.runAndRecord(*dp, nullptr, 10);
		for(std::vector<unique_ptr<VehicleData>> const &v : dp->getInputs()) {
			Assert::IsTrue(v.size() == 0);
		}
		sim.stop();
	}

	TEST_METHOD(CT_S_20) {
		Simulation sim("", "ShortCircle");
		unique_ptr<StubDataProcessor> dp = make_unique<StubDataProcessor>();
		sim.initialize();
		shared_ptr<TestVehicle> vut = sim.add_vehicle(100, 1, 1, 600, 100);
		sim.runAndRecord(*dp, move(vut), 10);
		for(std::vector<unique_ptr<VehicleData>> const &v : dp->getInputs()) {
			Assert::IsTrue(v.size() == 1);
		}
		sim.stop();
	}

	TEST_METHOD(CT_S_30) {
		Simulation sim("", "ShortCircle");
		unique_ptr<StubDataProcessor> dp = make_unique<StubDataProcessor>();
		sim.initialize();
		shared_ptr<TestVehicle> vut = sim.add_vehicle(100, 1, 1, 600, 100);
		sim.add_vehicle(100, 1, 1, 300, 100);
		sim.add_vehicle(100, 1, 1, 900, 100);
		sim.add_vehicle(100, 1, 1, 500, 100);
		sim.add_vehicle(100, 1, 1, 700, 100);
		sim.runAndRecord(*dp, move(vut), 10);
		for(std::vector<unique_ptr<VehicleData>> const &v : dp->getInputs()) {
			Assert::IsTrue(v.size() == 5);
		}
		sim.stop();
	}

	TEST_METHOD(CT_S_40) {
		Simulation sim("", "ShortCircle");
		unique_ptr<StubDataProcessor> dp = make_unique<StubDataProcessor>();
		sim.initialize();
		shared_ptr<TestVehicle> vut = sim.add_vehicle(100, 1, 1, 600, 100);
		sim.add_vehicle(100, 1, 1, 50, 100);
		sim.add_vehicle(100, 1, 1, 1150, 100);
		sim.runAndRecord(*dp, move(vut), 10);
		for(std::vector<unique_ptr<VehicleData>> const &v : dp->getInputs()) {
			Assert::IsTrue(v.size() == 1);
		}
		sim.stop();
	}

	TEST_METHOD(CT_S_50) {
		Simulation sim("", "ShortCircle");
		unique_ptr<StubDataProcessor> dp = make_unique<StubDataProcessor>();
		sim.initialize();
		shared_ptr<TestVehicle> vut = make_shared<TestVehicle>(*sim.add_vehicle(100, 1, 1, 600, 100));
		sim.add_vehicle(100, 1, 1, 50, 100);
		sim.add_vehicle(100, 1, 1, 1150, 100);
		sim.add_vehicle(100, 1, 1, 500, 100);
		sim.add_vehicle(100, 1, 1, 700, 100);
		sim.runAndRecord(*dp, move(vut), 10);
		for(std::vector<unique_ptr<VehicleData>> const &v : dp->getInputs()) {
			Assert::IsTrue(v.size() == 3);
		}
		sim.stop();
	}
	};

}