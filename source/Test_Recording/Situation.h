#pragma once
#include<vector>
#include<set>
#include<memory>
#include <map>
#include <cgraph.h>


class Transition;
class PredicateVehicle;

/**
* A Situation consists of Objects and all outgoing Transitions.
*/
class Situation {
public:
	/**
	* Constructs a Situation object with the given VUT and PredicateVehicles and an 'initial' flag.
	* @param vut Pointer to the PredicateVehicle representing the VUT.
	* @param otherVehicles Vector of pointers to PredicateVehicle representing all vehicles but the VUT.
	* @param initial. Flag signalling if this Situation is the initial situation of a sequence.
	*/
	Situation(std::shared_ptr<PredicateVehicle> vut, std::vector<std::shared_ptr<PredicateVehicle>> otherVehicles, bool initial);
	/**
	* Delete Copy constructor
	*/
	Situation(const Situation&) = delete;
	/**
	* Standard Deconstructor.
	*/
	~Situation();

	/**
	* Adds a Transition to the vector of transitions. Checks if this situation is actually the starting situation in the Transition.
	* @param transition Pointer to the new transition.
	*/
	void addTransition(std::shared_ptr<Transition> transition);

	/**
	* Getter for the vector of outgoing transitions.
	* @returns A vector of all outgoing transitions.
	*/
	std::set<std::shared_ptr<Transition>> const& getTransitions();

	/**
	 * Override operator ==
	 * @return true if both Situations have the same stable value, the same isInitial value, contain an equal VUT and equal other vehicles, else false
	 */
	bool operator==(const Situation  &other) const;

	/**
	* Override operator !=
	* @return true iff == returns false
	*/
	bool operator!=(const Situation  &other) const;

	/**
	* The Situation is stable iff the VUT is stable.
	* @return True iff the Situation is stable.
	*/
	bool isStable() const;

	/**
	* Getter for the other vehicles
	* @return vector of vehicles in the Situation excluding the VUT
	*/
	std::vector<std::shared_ptr<PredicateVehicle>> const& getOtherVehicles() const;

	/**
	* Getter for the VUT
	* @return const reference to the VUT.
	*/
	PredicateVehicle const& getVUT() const;

	/**
	* Getter for the initial flag.
	* @return value of the initial flag.
	*/
	bool isInitial() const;

	/**
	* Marks this Situation as initial.
	*/
	void markInitial();

	/**
	* transforms this Situation to a string representation.
	* @return string representation of the Situation.
	*/
	std::string toString() const;

	/**
	* transforms this Situation to a wstring representation.
	* @return wstring representation of the Situation.
	*/
	std::wstring toWString() const;

	/**
	* Adds a node representing this Situation to the Graph if it wasn't added already.
	* @param graph Pointer to the graph the node should be added to.
	* @param id Unique ID for the resulting node struct in the graph.
	* @return The node representation of this Situation.
	*/
	Agnode_t * addAsNode(Agraph_t *graph, int id);

private:
	std::set<std::shared_ptr<Transition>> transitions;
	std::shared_ptr<PredicateVehicle> vut;
	std::vector<std::shared_ptr<PredicateVehicle>> otherVehicles;
	bool initial;
	Agnode_t *node;

	std::string toHTML();
};

