#pragma once
#include "DataManager.h"
#include "Transition.h"
#include <vector>
#include<memory>

class StubDataManager : public DataManager {
public:
	StubDataManager();
	~StubDataManager();

	// Inherited via DataManager
	virtual void storeTransition(Transition const & transition) override;
	virtual std::unique_ptr<SituationGraph> retrieveSubSituationGraph(std::shared_ptr<Situation> root);
	virtual std::unique_ptr<SituationGraph> retrieveSituationGraph();
	virtual int getMaxNumberOfObjects();
	virtual int getNumberOfSituations();

	std::vector<std::shared_ptr<Transition>> const& getInputs();

private:
	std::vector<std::shared_ptr<Transition>> inputs;
};

