#include "stdafx.h"
#include "StubDataManager.h"
#include "Situation.h"
#include "Transition.h"
#include <memory>
#include "CppUnitTest.h"
#include "SituationGraph.h"

using std::make_shared;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

StubDataManager::StubDataManager() : inputs() {
}


StubDataManager::~StubDataManager() {
}

void StubDataManager::storeTransition(Transition const & transition) {
	Logger::WriteMessage(L"storeTransition was called");
	inputs.push_back(make_shared<Transition>(transition));
}

std::unique_ptr<SituationGraph> StubDataManager::retrieveSubSituationGraph(std::shared_ptr<Situation> root)
{
	return nullptr;
}

std::unique_ptr<SituationGraph> StubDataManager::retrieveSituationGraph() {
	std::vector<std::shared_ptr<Transition>> t;
	SituationGraph sg(t);
	return move(std::make_unique<SituationGraph>(t));
}

int StubDataManager::getMaxNumberOfObjects()
{
	return 0;
}

int StubDataManager::getNumberOfSituations()
{
	return 0;
}

std::vector<std::shared_ptr<Transition>> const & StubDataManager::getInputs() {
	return inputs;
}
