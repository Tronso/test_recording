#pragma once

#include<vector>
#include "recording_api.h"
#include <memory>
#include<queue>
#include "PredicateConstructor.h"
#include "TransitionGenerator.h"
#include <condition_variable>
#include<mutex>

class DataManager;
class PredicateConstructor;
class SituationCondenser;
class VehicleData;
class TransitionGenerator;
class Situation;

/*
* A DataProcessor takes the data for each step and uses its subcomponents to process it.
*/
class DataProcessor {
public:
	/**
	* Creates a new DataProcessor accessing the data storage via the given DataManager.
	*/
	RECORDING_API DataProcessor(std::unique_ptr<DataManager> dm);

	/**
	* Deletes the DataProcessor and its components after all remaining datasets are processed.
	*/
	RECORDING_API ~DataProcessor();

	/**
	* Takes data for a step and uses its subcomponents to process them.
	*/
	virtual void RECORDING_API processData(std::vector<std::unique_ptr<VehicleData>> data);

	/**
	* Getter for the DataManager to be used in testing
	*/
	std::unique_ptr<DataManager> const& getDataManager();

	/**
	* Getter for the current situation to be used in testing
	*/
	std::shared_ptr<Situation> const& getCurrentSituation();

	/**
	* Prompts the DataProcessor to stop processing data after the workingQueue is empty. Only to be used to access currentSituation in testing
	*/
	void stop();

protected:
	/**
	* Default constructor to be used by the derived test class.
	*/
	DataProcessor();

private:
	std::unique_ptr<DataManager> dataManager;
	PredicateConstructor predicateCreator;
	TransitionGenerator transitionGenerator;
	std::shared_ptr<Situation> currentSituation;
	std::queue<std::vector<std::unique_ptr<VehicleData>>> workingQueue;
	void processingTask();
	std::condition_variable cv;
	bool stopped;
	std::mutex mtx;
	std::thread processingThread;
};

