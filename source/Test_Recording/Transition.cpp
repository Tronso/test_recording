#include "Transition.h"
#include "Situation.h"
#include <iomanip>
#include <sstream>

using std::unique_ptr;
using std::shared_ptr;
using std::move;

Transition::Transition(std::shared_ptr<Situation> from, std::shared_ptr<Situation> to, int count, double probability)
	: fromSituation(from), toSituation(to), count(count), probability(probability), edge(nullptr) {
}

Transition::Transition(std::shared_ptr<Situation> from, std::shared_ptr<Situation> to)
	: fromSituation(from), toSituation(to), probability(0.0), edge(nullptr) {
}

Transition::~Transition() {
}

shared_ptr<Situation> Transition::getFromSituation() const {
	return fromSituation;
}

shared_ptr<Situation> Transition::getToSituation() const {
	return toSituation;
}

double Transition::getProbability() const {
	return probability;
}

bool Transition::operator==(const Transition & other) const {
	return *(this->fromSituation) == *(other.fromSituation) && *(this->toSituation) == *(other.toSituation);
}

bool Transition::operator!=(const Transition & other) const {
	return !(*this == other);
}

Agedge_t * Transition::addAsEdge(Agraph_t * graph, const int id) {
	if(!edge) {
		std::stringstream label;
		label << std::fixed << std::setprecision(2) << "Count: " << count << std::endl << "Probability: " << probability;
		std::string name("Transition " + std::to_string(id));
		edge = agedge(graph
			, fromSituation->addAsNode(graph, id * 2)
			, toSituation->addAsNode(graph, id * 2 + 1)
			, &name[0u]
			, true
		);
		agset(edge, "label", agstrdup(graph, &label.str()[0u]));
	}
	return edge;
}
