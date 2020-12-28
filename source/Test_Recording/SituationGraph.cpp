#include "SituationGraph.h"
#include "Transition.h"
#include "Situation.h"
#include <iostream>
#include <gvplugin.h>
#include <queue>

using std::shared_ptr;

SituationGraph::SituationGraph(std::vector<std::shared_ptr<Transition>> transitions) 
: transitions(move(transitions)) {
	gvc = gvContext();
	createGraph();
}

SituationGraph::~SituationGraph() {
	gvFreeContext(gvc);
	agclose(graph);
}

void SituationGraph::saveAsPDF(const std::string & filename) const {
	gvLayout(gvc, graph, "dot");
	gvRenderFilename(gvc, graph, "pdf", filename.c_str());
	gvFreeLayout(gvc, graph);
}

RECORDING_API int SituationGraph::getLongestSequence() const
{
	int longestPath = 0;
	for (shared_ptr<Situation> s : initialSituations) {
		int currLong = longestDFS(s);
		if (longestPath < currLong) {
			longestPath = currLong;
		}
	}
	return longestPath;
}

int SituationGraph::longestDFS(std::shared_ptr<Situation> root) const
{
	std::set<shared_ptr<Situation>> visited;
	std::queue<std::pair<shared_ptr<Situation>, int>> todo;
	int longest = 0;

	todo.push(std::pair<shared_ptr<Situation>, int>(root, 0));
	visited.insert(root);
	while (!todo.empty()) {
		shared_ptr<Situation> currSit = todo.front().first;
		int currLength = todo.front().second;
		todo.pop();
		if (currSit->isStable() && !currSit->isInitial()) {
			longest = longest < currLength + 1 ? currLength + 1 : longest;
		}
		else {
			for (shared_ptr<Transition> t : currSit->getTransitions()) {
				shared_ptr<Situation> to = t->getToSituation();
				if (visited.find(to) == visited.end()) {
					todo.push(std::pair<shared_ptr<Situation>, int>(to, currLength + 1));
					visited.insert(to);
				}
			}
		}
	}

	return longest;
}

void SituationGraph::createGraph() {
	graph = agopen("Situation Graph", Agdirected, &AgDefaultDisc);

	agattr(graph, AGNODE, "shape", "plaintext");
	agattr(graph, AGNODE, "fillcolor", "white");
	agattr(graph, AGEDGE, "label", "/N");

	int id = 0;
	for(std::shared_ptr<Transition> t : transitions) {
		shared_ptr<Situation> from = t->getFromSituation();
		shared_ptr<Situation> to = t->getToSituation();
		if(from->isInitial()) {
			initialSituations.insert(from);
		}
		if (!to->isInitial() && to->isStable()) {
			finalSituations.insert(to);
		}
		t->addAsEdge(graph, id);
		++id;
	}
}
