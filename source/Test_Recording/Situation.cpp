#include "Situation.h"
#include "PredicateVehicle.h"
#include <string>
#include "Transition.h"
#include <iostream>
#include <algorithm>
#include <sstream>

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using std::move;

Situation::Situation(std::shared_ptr<PredicateVehicle> vut, std::vector<std::shared_ptr<PredicateVehicle>> otherVehicles, bool initial)
	: vut(vut)
	, otherVehicles(otherVehicles)
	, initial(initial)
	, node(nullptr) {
}

Situation::~Situation() {
}

void Situation::addTransition(std::shared_ptr<Transition> transition) {
	transitions.insert(transition);
}

std::set<shared_ptr<Transition>> const& Situation::getTransitions() {
	return transitions;
}

bool Situation::operator==(const Situation &other) const {
	if(isStable() != other.isStable()) {
		return false;
	}
	if(initial != other.initial) {
		return false;
	}
	//check for number of cars for optimization
	if(otherVehicles.size() != other.otherVehicles.size()) {
		return false;
	}
	//check VUT
	if(*vut != *other.vut) {
		return false;
	}
	for(shared_ptr<PredicateVehicle> const vehicle : otherVehicles) {
		if(std::find_if(other.otherVehicles.begin(), other.otherVehicles.end(), [&](const shared_ptr<PredicateVehicle> v) { return *vehicle == *v; }) == other.otherVehicles.end()) {
			return false;
		}
	}
	return true;
}

bool Situation::operator!=(const Situation & other) const {
	return !(*this == other);
}

bool Situation::isStable() const {
	return vut->isStable();
}

std::vector<shared_ptr<PredicateVehicle>> const& Situation::getOtherVehicles() const {
	return otherVehicles;
}

PredicateVehicle const & Situation::getVUT() const {
	return *vut;
}

bool Situation::isInitial() const {
	return initial;
}

void Situation::markInitial() {
	initial = true;
}

std::string Situation::toString() const {
	std::string result("{");
	result += "IsInitial: " + std::to_string(initial) + ", IsStable: " + std::to_string(isStable()) + "\n";
	result += "VUT: " + vut->toString() + ",\n";
	for(shared_ptr<PredicateVehicle> vehicle : otherVehicles) {
		result += vehicle->toString() + ",\n";
	}
	result = result.substr(0, result.size() - 2);
	result += "}";
	return result;
}

std::wstring Situation::toWString() const {
	std::wstring result(L"{");
	result += L"IsInitial: " + std::to_wstring(initial) + L", IsStable: " + std::to_wstring(isStable()) + L"\n";
	result += L"VUT: " + vut->toWString() + L",\n";
	result += vut->toWString() + L",\n";
	for(shared_ptr<PredicateVehicle> vehicle : otherVehicles) {
		result += vehicle->toWString() + L",\n";
	}
	result = result.substr(0, result.size() - 2);
	result += L"}";
	return result;
}

Agnode_t * Situation::addAsNode(Agraph_t * graph, const int id) {
	if(!node) {
		//TODO create content and layout of node
		std::string name("Situation " + std::to_string(id));
		node = agnode(graph, &name[0u], true);
		agset(node, "label", agstrdup_html(graph, &toHTML()[0u]));
	}
	return node;
}

std::string Situation::toHTML() {
	std::stringstream html;
	std::string bgcolor("\"white\"");
	if(initial && isStable()) {
		bgcolor = "\"green\"";
	} else if(initial && !isStable()) {
		bgcolor = "\"yellow\"";
	} else if(!initial && isStable()) {
		bgcolor = "\"blue\"";
	}
	 html << "<TABLE BGCOLOR=" << bgcolor << " BORDER = \"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">" << std::endl;
	 html << "<TR>" << std::endl;
	 html << "<TD>Role</TD>" << "<TD>RelX</TD>" << "<TD>RelV</TD>" << "<TD>Lane</TD>" << "<TD>Acc</TD>" << "<TD>LnChg</TD>" << std::endl;
	 html << "</TR>" << std::endl;
	 html << "<TR>" << std::endl;
	 html << "<TD>VUT</TD>" << "<TD>" << to_string(vut->getRelX()) << "</TD>" << "<TD>" << to_string(vut->getRelV()) << "</TD>" << "<TD>" << std::to_string(vut->getLane()) << "</TD>" << "<TD>" << to_string(vut->getAcc()) << "</TD>" << "<TD>" << to_string(vut->getLnChg()) << "</TD>" << std::endl;
	 html << "</TR>" << std::endl;
	 for(shared_ptr<PredicateVehicle> v : otherVehicles) {
		 html << "<TR>" << std::endl;
		 html << "<TD>OtherVeh</TD>" << "<TD>" << to_string(v->getRelX()) << "</TD>" << "<TD>" << to_string(v->getRelV()) << "</TD>" << "<TD>" << std::to_string(v->getLane()) << "</TD>" << "<TD>" << to_string(v->getAcc()) << "</TD>" << "<TD>" << to_string(v->getLnChg()) << "</TD>" << std::endl;
		 html << "</TR>" << std::endl;
	 }
	 html << "</TABLE>";
	return html.str();
}
