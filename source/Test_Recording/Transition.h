#pragma once
#include<memory>
#include <gvc.h>

class Situation;

/**
* A Transition describes the transition and the output between two situations
*/
class Transition {
public:
	/**
	* Constructs a new Transition with the given predecessor, successor, transition count and transition probability.
	* Used when retrieving Transitions from the data storage.
	* @param from Pointer to the predecessor Situation.
	* @param to Pointer to the successor Situation.
	* @param count Number of times this Transition was recorded.
	* @param probability Probability that this Transition occurs in the predecessor Situation.
	*/
	Transition(std::shared_ptr<Situation> from, std::shared_ptr<Situation> to, int count, double probability);

	/**
	* Constructs a new Transition with the given predecessor, successor and no transition probability.
	* Used when recording a new Transition.
	* @param from Pointer to the predecessor Situation.
	* @param to Pointer to the successor Situation.
	*/
	Transition(std::shared_ptr<Situation> from, std::shared_ptr<Situation> to);

	/**
	* Standard Deconstructor.
	*/
	~Transition();

	/**
	* Getter for predecessor situation
	* @returns Predecessor situation
	*/
	std::shared_ptr<Situation> getFromSituation() const;

	/**
	* Getter for successor situation
	* @returns Successor situation
	*/
	std::shared_ptr<Situation> getToSituation() const;

	/**
	* Getter for the proability of the occurrence of this transition.
	*  @returns Probability of the occurrence of this transition.
	*/
	double getProbability() const;

	/**
	* Override operator ==
	* @param other Transition this one is compared to.
	* @return true if both predecessor and successor Situations are equal. The probability is not considered.
	*/
	bool operator==(const Transition &other) const;

	/**
	* Override operator !=
	* @param other Transition this one is compared to.
	* @return true iff == returns false
	*/
	bool operator!=(const Transition &other) const;

	/**
	* Adds the edge representation of this Transition to the graph.
	* @param graph Pointer to the graph struct this Transition should be added to.
	* @param id Unique ID of the resulting edge struct in the graph.
	* @return Pointer to the resulting edge struct.
	*/
	Agedge_t * addAsEdge(Agraph_t *graph, const int id);

private:
	std::shared_ptr<Situation> fromSituation;
	std::shared_ptr<Situation> toSituation;
	double probability;
	int count;
	Agedge_t *edge;
};

