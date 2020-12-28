#include "stdafx.h"
#include "StubDataProcessor.h"
#include <iostream>
#include "VehicleData.h"
#include "CppUnitTest.h"

using std::unique_ptr;
using std::shared_ptr;
using std::move;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

StubDataProcessor::StubDataProcessor() {
}


StubDataProcessor::~StubDataProcessor() {
}

void StubDataProcessor::processData(std::vector<std::unique_ptr<VehicleData>> data) {
	processDataCalled = true;
	Logger::WriteMessage("processDataCalled on IDs: ");
	for(unique_ptr<VehicleData> &d : data) {
		std::string output = std::to_string(d->getID()) + ", ";
		Logger::WriteMessage(output.c_str());
	}
	inputs.push_back(move(data));
}

bool StubDataProcessor::isProcessDataCalled() {
	return processDataCalled;
}

std::vector<std::vector<unique_ptr<VehicleData>>> const& StubDataProcessor::getInputs() {
	return inputs;
}
