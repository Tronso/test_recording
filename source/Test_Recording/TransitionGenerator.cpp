#include "TransitionGenerator.h"
#include "Situation.h"
#include "Transition.h"
#include <iostream>

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;

TransitionGenerator::TransitionGenerator() {
}

TransitionGenerator::~TransitionGenerator() {
}

unique_ptr<Transition> TransitionGenerator::generate(shared_ptr<Situation> const &oldSituation, shared_ptr<Situation> const &newSituation) {
	if((*oldSituation) == (*newSituation)) {
		return nullptr;
	} else {
		return move(make_unique<Transition>(oldSituation, newSituation));
	}
}
