#pragma once
#include<memory>

class Transition;
class Situation;

class TransitionGenerator {
public:
	TransitionGenerator();

	~TransitionGenerator();

	/**
	 * Takes two Situations, compares them and generates a Transition if they differ.
	 * @param oldSituation Pointer to the Situation where the Transition is supposed to start.
	 * @param newSituation Pointer to the Situation where the Transition is supposed to end.
	 * @return A Transition with oldSItuation as start and newSituation as end or nullptr if the Situations are equal.
	 */
	std::unique_ptr<Transition> generate(std::shared_ptr<Situation> const  &oldSituation, std::shared_ptr<Situation> const &newSituation);
};