#pragma once
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include<string>
#include<vector>
#include "recording_api.h"
#include<memory>
#include <map>

class Transition;
class Situation;
class SituationGraph;

/**
* A DataManager offers methods to store and retrieve data from a data storage.
*/
class DataManager {
public:

	/**
	* Stores the given transition in the data storage.
	* If an equal transition already exists, the count is incremented.
	* @param transition The transition that should be stored.
	*/
	RECORDING_API virtual void storeTransition(Transition const &transition) = 0;

	/**
	* Retrieves all stored situations reachable from the given root as a graph from the data storage.
	* @param root Initial Situation of the subgraph.
	* @returns SituationGraph containing the Subgraph reachable from root, or nullptr if the Situation is not initial or not contained in the data storage.
	*/
	RECORDING_API virtual std::unique_ptr<SituationGraph> retrieveSubSituationGraph(std::shared_ptr<Situation> root) = 0;

	/**
	* Retrieves a SituationGraph representing all entries in the database.
	* @return SituationGraph represented by the entries in the database.
	*/
	RECORDING_API virtual std::unique_ptr<SituationGraph> retrieveSituationGraph() = 0;

	/**
	* Returns the maximum number of objects in a single Situation in the database.
	* @return Maximum number of objects in a single situation in the database.
	*/
	RECORDING_API virtual int getMaxNumberOfObjects() = 0;

	/**
	* Getter for the number of Situations in the database.
	* @Return Number of Situations in the database.
	*/
	RECORDING_API virtual int getNumberOfSituations() = 0;

	virtual ~DataManager() {
	};
};

#endif // !DATA_MANAGER_H
