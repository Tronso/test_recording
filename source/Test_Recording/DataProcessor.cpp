#include "DataProcessor.h"
#include "SQLiteManager.h"
#include "PredicateConstructor.h"
#include "TransitionGenerator.h"
#include "Situation.h"
#include "Transition.h"
#include "PredicateVehicle.h"
#include "VehicleData.h"
#include <iostream>
#include <map>
#include <thread>

using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::move;
using std::cout;
using std::endl;
using std::map;
using std::thread;
using std::mutex;

DataProcessor::DataProcessor(unique_ptr<DataManager> dm)
	: currentSituation(nullptr)
	, dataManager(move(dm))
	, predicateCreator()
	, transitionGenerator()
	, stopped(false) {

}


DataProcessor::~DataProcessor() {
	stop();
}

void DataProcessor::processData(std::vector<unique_ptr<VehicleData>> data) {
	std::unique_lock<mutex> lck(mtx);
	workingQueue.push(move(data));
	if(processingThread.get_id() == thread::id()) {
		processingThread = std::thread([this] { processingTask(); });

	}
	cv.notify_all();

}

DataProcessor::DataProcessor() {
}

void DataProcessor::processingTask() {
	while(!stopped || !workingQueue.empty()) {
		std::unique_lock<mutex> lck(mtx);
		while(!stopped && workingQueue.empty()) {
			cv.wait(lck);
		}
		if(stopped && workingQueue.empty()) {
			return;
		}
		std::vector<unique_ptr<VehicleData>> data = move(workingQueue.front());
		workingQueue.pop();
		lck.unlock();
		std::vector<shared_ptr<PredicateVehicle>> pVehicles = predicateCreator.transform(move(data));
		shared_ptr<PredicateVehicle> vut  = pVehicles[0];
		pVehicles.erase(pVehicles.begin());
		shared_ptr<Situation> newSituation = make_shared<Situation>(vut, pVehicles, false);
		//Initial Situation of the recording session
		if(currentSituation == nullptr) {
			currentSituation = move(newSituation);
		}
		//do not record transitions between stable situations
		else if(newSituation->isStable() && currentSituation->isStable()) {
			currentSituation = move(newSituation);
		}
		//Initial situation of the next sequence
		else if(currentSituation->isStable() && !newSituation->isStable()) {
			currentSituation->markInitial();
			unique_ptr<Transition> trans = move(transitionGenerator.generate(currentSituation, newSituation));
			if(trans) {
				currentSituation = newSituation;
				dataManager->storeTransition(*trans);
			}
		}
		//transition between two unstable Situation or from an unstable to a stable Situation
		else {
			unique_ptr<Transition> trans = move(transitionGenerator.generate(currentSituation, newSituation));
			if(trans) {
				currentSituation = newSituation;
				dataManager->storeTransition(*trans);
			}
		}
	}
}

std::unique_ptr<DataManager> const & DataProcessor::getDataManager() {
	return dataManager;
}

shared_ptr<Situation> const & DataProcessor::getCurrentSituation() {
	std::unique_lock<mutex>(mtx);
	return currentSituation;
}

void DataProcessor::stop() {
	cout << "Stopping Simulation, processing queued data points: " << workingQueue.size() << endl;
	stopped = true;
	cv.notify_all();
	if(processingThread.joinable()) {
		processingThread.join();
	}
}
