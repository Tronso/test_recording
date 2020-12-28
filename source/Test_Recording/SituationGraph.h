#pragma once
extern "C" {
#include <gvc.h>
}
#include <vector>
#include <memory>
#include <set>
#include "recording_api.h"

class Situation;
class Transition;

/**
* This class represents a graph of Situations and Transitions and can create a visual representation of it.
*/
class SituationGraph {
public:
	/**
	* Builds a graph from the given Situations.
	*/
	RECORDING_API SituationGraph(std::vector<std::shared_ptr<Transition>> transitions);
	/**
	* Cleans up memory of the graph representation.
	*/
	RECORDING_API ~SituationGraph();

	/**
	* Exports the image of the graph as PDF with the given filename in the subfolder 'Graphs'
	* @param filename Name of the file to be created.
	*/
	RECORDING_API void saveAsPDF(const std::string &filename) const;

	/**
	* Computes the longest simple path from an initial situation to a final situation.
	* @Returns longest simple path in the graph
	*/
	RECORDING_API int getLongestSequence() const;

private:
	Agraph_t *graph;
	GVC_t *gvc;
	std::vector<std::shared_ptr<Transition>> transitions;
	std::set<std::shared_ptr<Situation>> initialSituations;
	std::set<std::shared_ptr<Situation>> finalSituations;

	int longestDFS(std::shared_ptr<Situation> root) const;

	/**
	* Adds nodes for all Situations and edges for all Transitions.
	*/
	void createGraph();
};

