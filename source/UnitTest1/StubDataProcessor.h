#pragma once

#include "DataProcessor.h"
#include<memory>
#include "recording_api.h"

class StubDataProcessor : public DataProcessor {
public:
	StubDataProcessor();
	~StubDataProcessor();

	virtual void processData(std::vector<std::unique_ptr<VehicleData>> data) override;

	bool isProcessDataCalled();

	std::vector<std::vector<std::unique_ptr<VehicleData>>> const& getInputs();

private:
	bool processDataCalled;
	std::vector<std::vector<std::unique_ptr<VehicleData>>> inputs;
};

