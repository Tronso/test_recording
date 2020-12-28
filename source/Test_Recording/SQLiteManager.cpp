#include "SQLiteManager.h"
#include "sqlite3.h"
#include<fstream>
#include<iostream>
#include<atlstr.h>
#include "Situation.h"
#include "Transition.h"
#include "PredicateVehicle.h"
#include<vector>
#include <conio.h>
#include <map>
#include <queue>
#include "SituationGraph.h"
#include <sstream>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::map;
using std::queue;

SQLiteManager::SQLiteManager(const string &databaseName) 
	: getSituationIDStr(loadSQLScript("GetSituationID.sql")) {
	string dbPath("./database/" + databaseName);
	int rc = sqlite3_open(dbPath.c_str(), &db);
	if(rc != SQLITE_OK) {
		cout << "Cannot open database: " << sqlite3_errmsg(db) << endl;
		throw 0;
	}
	char* err = 0;
	loadAndExecuteSQLScript("CreateSQLDatabase.sql", nullptr, err);
	if(err != nullptr) {
		std::cout << err << std::endl;
		throw 0;
	}
	prepareStatements();
}

SQLiteManager::~SQLiteManager() {
	finalizeStatements();
	sqlite3_close(db);
}

void SQLiteManager::loadAndExecuteSQLScript(std::string const & file, int(*callback)(void*, int, char**, char**), char *err) {
	string content = loadSQLScript(file);
	//execute script
	executeQuery(content, callback, err);
}

void SQLiteManager::executeQuery(std::string const & query, int(*callback)(void*, int, char**, char**), char *err) {
	int rc = sqlite3_exec(db, query.c_str(), callback, this, &err);
	if(rc != SQLITE_OK) {
		cout << "An error occurred: " << sqlite3_errmsg(db) << endl;
	}
}

std::string SQLiteManager::loadSQLScript(std::string const & file) {
	//load file
	std::ifstream ifs("./sql/" + file, std::ifstream::binary);
	string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	ifs.close();

	//check for BOM in UTF-8
	if(content.compare(0, 3, "\xEF\xBB\xBF") == 0) {
		content.erase(0, 3);
	}
	return content;
}

sqlite3_int64 SQLiteManager::getSituationID(Situation const & situation) {
	map<sqlite_int64, int> idCounts;
	sqlite_int64 vutID = getObjectID(situation.getVUT());
	if(vutID < 0) {
		return -1;
	}
	for(shared_ptr<PredicateVehicle> const &v : situation.getOtherVehicles()) {
		sqlite3_int64 objectID = getObjectID(*v);
		if(objectID < 0) {
			return -1;
		}
		if(idCounts.find(objectID) != idCounts.end()) {
			//id already in map
			idCounts[objectID] = idCounts[objectID] +1;
		} else {
			//new id inserted
			idCounts[objectID] = 1;
		}
	}
	std::stringstream object_count_list_s;
	//manually insert the object IDs into the query string since a variable number of parameters is not possible
	object_count_list_s << "SELECT " << vutID << " AS Object_ID, " << 10 << " AS Role_in_Situation, " << 1 << " AS Object_Count " << endl;
	for(std::pair<sqlite_int64, int> entry : idCounts) {
		object_count_list_s << "UNION ALL SELECT " << entry.first << " AS Object_ID, " << 11 << " AS Role_in_Situation, " << entry.second << " AS Object_Count " << endl;
	}
	string object_count_list = object_count_list_s.str();
	string find = "OBJECT_COUNT_LIST";
	string sql = getSituationIDStr;
	size_t replace = sql.find(find, 0);
	sql = sql.replace(replace, find.size(), object_count_list);
	sqlite3_stmt *statement;
	sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr);
	sqlite3_bind_int64(statement, sqlite3_bind_parameter_index(statement, "@VUTID"), vutID);
	sqlite3_bind_int64(statement, sqlite3_bind_parameter_index(statement, "@IsInitial"), situation.isInitial());
	sqlite3_bind_int64(statement, sqlite3_bind_parameter_index(statement, "@DistinctObjectCount"), idCounts.size() + 1);
	int res = -1;
	map<int, shared_ptr<PredicateVehicle>> retrievedVehicles;
	if(sqlite3_step(statement) == SQLITE_ROW) {
		res = sqlite3_column_int(statement, 0);
	}
	sqlite3_finalize(statement);
	return res;
}

sqlite3_int64 SQLiteManager::getObjectID(PredicateVehicle const & vehicle) {
	sqlite3_bind_int(getObjectIDStmt, sqlite3_bind_parameter_index(getObjectIDStmt, "@RelX"), static_cast<int>(vehicle.getRelX()));
	sqlite3_bind_int(getObjectIDStmt, sqlite3_bind_parameter_index(getObjectIDStmt, "@RelV"), static_cast<int>(vehicle.getRelV()));
	sqlite3_bind_int(getObjectIDStmt, sqlite3_bind_parameter_index(getObjectIDStmt, "@Lane"), static_cast<int>(vehicle.getLane()));
	sqlite3_bind_int(getObjectIDStmt, sqlite3_bind_parameter_index(getObjectIDStmt, "@Acc"), static_cast<int>(vehicle.getAcc()));
	sqlite3_bind_int(getObjectIDStmt, sqlite3_bind_parameter_index(getObjectIDStmt, "@LnChg"), static_cast<int>(vehicle.getLnChg()));
	sqlite3_bind_int(getObjectIDStmt, sqlite3_bind_parameter_index(getObjectIDStmt, "@IsStable"), vehicle.isStable());
	int r = sqlite3_step(getObjectIDStmt);
	int res = -1;
	if(r == SQLITE_ROW) {
		res = sqlite3_column_int(getObjectIDStmt, 0);
	}
	sqlite3_reset(getObjectIDStmt);
	sqlite3_clear_bindings(getObjectIDStmt);
	return res;
}

sqlite3_int64 SQLiteManager::insertVehicle(PredicateVehicle const & vehicle) {
	sqlite3_bind_int(insertVehicleStmt, sqlite3_bind_parameter_index(insertVehicleStmt, "@IsStable"), vehicle.isStable());
	sqlite3_step(insertVehicleStmt);
	sqlite3_reset(insertVehicleStmt);
	sqlite3_clear_bindings(insertVehicleStmt);

	sqlite3_int64 objectId = sqlite3_last_insert_rowid(db);
	sqlite3_bind_int64(insertObjectValueStmt, sqlite3_bind_parameter_index(insertObjectValueStmt, "@ObjectID"), objectId);
	sqlite3_bind_int(insertObjectValueStmt, sqlite3_bind_parameter_index(insertObjectValueStmt, "@RelX"), static_cast<int>(vehicle.getRelX()));
	sqlite3_bind_int(insertObjectValueStmt, sqlite3_bind_parameter_index(insertObjectValueStmt, "@RelV"), static_cast<int>(vehicle.getRelV()));
	sqlite3_bind_int(insertObjectValueStmt, sqlite3_bind_parameter_index(insertObjectValueStmt, "@Lane"), static_cast<int>(vehicle.getLane()));
	sqlite3_bind_int(insertObjectValueStmt, sqlite3_bind_parameter_index(insertObjectValueStmt, "@Acc"), static_cast<int>(vehicle.getAcc()));
	sqlite3_bind_int(insertObjectValueStmt, sqlite3_bind_parameter_index(insertObjectValueStmt, "@LnChg"), static_cast<int>(vehicle.getLnChg()));
	sqlite3_step(insertObjectValueStmt);
	sqlite3_reset(insertObjectValueStmt);
	sqlite3_clear_bindings(insertObjectValueStmt);
	return objectId;
}

sqlite3_int64 SQLiteManager::insertSituation(Situation const & situation) {
	sqlite3_bind_int(insertSituationStmt, sqlite3_bind_parameter_index(insertSituationStmt, "@IsInitial"), situation.isInitial());
	sqlite3_step(insertSituationStmt);
	sqlite3_reset(insertSituationStmt);
	sqlite3_clear_bindings(insertSituationStmt);
	sqlite3_int64 situationID = sqlite3_last_insert_rowid(db);

	//VUT
	PredicateVehicle vut = situation.getVUT();
	sqlite3_int64 vutID = getObjectID(vut);
	if(vutID < 0) {
		vutID = insertVehicle(vut);
	}
	sqlite3_bind_int64(insertSituationObjectStmt, sqlite3_bind_parameter_index(insertSituationObjectStmt, "@SituationID"), situationID);
	sqlite3_bind_int(insertSituationObjectStmt, sqlite3_bind_parameter_index(insertSituationObjectStmt, "@Role"), 10);
	sqlite3_bind_int(insertSituationObjectStmt, sqlite3_bind_parameter_index(insertSituationObjectStmt, "@ObjectID"), vutID);
	sqlite3_step(insertSituationObjectStmt);
	sqlite3_clear_bindings(insertSituationObjectStmt);
	sqlite3_reset(insertSituationObjectStmt);
	//otherVehicles
	for(shared_ptr<PredicateVehicle> const &v : situation.getOtherVehicles()) {
		sqlite3_int64 objectID = getObjectID(*v);
		if(objectID < 0) {
			objectID = insertVehicle(*v);
		}
		sqlite3_bind_int64(insertSituationObjectStmt, sqlite3_bind_parameter_index(insertSituationObjectStmt, "@SituationID"), situationID);
		sqlite3_bind_int(insertSituationObjectStmt, sqlite3_bind_parameter_index(insertSituationObjectStmt, "@Role"), 11);
		sqlite3_bind_int(insertSituationObjectStmt, sqlite3_bind_parameter_index(insertSituationObjectStmt, "@ObjectID"), objectID);
		sqlite3_step(insertSituationObjectStmt);
		sqlite3_clear_bindings(insertSituationObjectStmt);
		sqlite3_reset(insertSituationObjectStmt);
	}
	return situationID;
}

std::shared_ptr<PredicateVehicle> SQLiteManager::retrieveVehicle(int objectID) {
	sqlite3_bind_int(retrieveVehicleStmt, sqlite3_bind_parameter_index(retrieveVehicleStmt, "@ObjectID"), objectID);
	RelX relX;
	RelV relV;
	int lane;
	Acc acc;
	LnChg lnChg;
	bool stable;
	while(sqlite3_step(retrieveVehicleStmt) == SQLITE_ROW) {
		stable = sqlite3_column_int(retrieveVehicleStmt, 2);
		switch(sqlite3_column_int(retrieveVehicleStmt, 0)) {
		case 1020:
			relX = static_cast<RelX>(sqlite3_column_int(retrieveVehicleStmt, 1));
			break;
		case 1030:
			relV = static_cast<RelV>(sqlite3_column_int(retrieveVehicleStmt, 1));
			break;
		case 1040:
			lane = sqlite3_column_int(retrieveVehicleStmt, 1);
			break;
		case 1050:
			acc = static_cast<Acc>(sqlite3_column_int(retrieveVehicleStmt, 1));
			break;
		case 1060:
			lnChg = static_cast<LnChg>(sqlite3_column_int(retrieveVehicleStmt, 1));
			break;
		}
	}
	sqlite3_reset(retrieveVehicleStmt);
	sqlite3_clear_bindings(retrieveVehicleStmt);
	return std::make_shared<PredicateVehicle>(relX, relV, lane, acc, lnChg, stable);
}

std::shared_ptr<Situation> SQLiteManager::retrieveSituation(int situationID, map<int, shared_ptr<PredicateVehicle>>& retrievedVehicles) {
	sqlite3_bind_int(retrieveSituationObjectIDsStmt, sqlite3_bind_parameter_index(retrieveSituationObjectIDsStmt, "@SituationID"), situationID);
	shared_ptr<PredicateVehicle> vut;
	vector<shared_ptr<PredicateVehicle>> otherVehicles;
	bool isStable;
	while(sqlite3_step(retrieveSituationObjectIDsStmt) == SQLITE_ROW) {
		int roleInSituation = sqlite3_column_int(retrieveSituationObjectIDsStmt, 0);
		int objectID = sqlite3_column_int(retrieveSituationObjectIDsStmt, 1);
		isStable = sqlite3_column_int(retrieveSituationObjectIDsStmt, 2);
		shared_ptr<PredicateVehicle> veh;
		if(retrievedVehicles.find(objectID) != retrievedVehicles.end()) {
			veh = retrievedVehicles.at(objectID);
		} else {
			veh = retrieveVehicle(objectID);
			retrievedVehicles[objectID] = veh;
		}
		if(roleInSituation == 10) {
			vut = veh;
		} else {
			otherVehicles.push_back(veh);
		}
	}
	sqlite3_reset(retrieveSituationObjectIDsStmt);
	sqlite3_clear_bindings(retrieveSituationObjectIDsStmt);
	return std::make_shared<Situation>(vut, otherVehicles, isStable);
}

void SQLiteManager::prepareStatements() {
	string sql(loadSQLScript("GetObjectID.sql"));
	sqlite3_prepare_v2(db, sql.c_str(), -1, &getObjectIDStmt, nullptr);

	sql = loadSQLScript("InsertVehicle.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &insertVehicleStmt, nullptr);

	sql = loadSQLScript("InsertObjectValue.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &insertObjectValueStmt, nullptr);

	sql = loadSQLScript("InsertSituation.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &insertSituationStmt, nullptr);

	sql = loadSQLScript("InsertSituationObject.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &insertSituationObjectStmt, nullptr);

	sql = loadSQLScript("RetrieveVehicle.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &retrieveVehicleStmt, nullptr);

	sql = loadSQLScript("RetrieveSituationObjectIDs.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &retrieveSituationObjectIDsStmt, nullptr);

	sql = loadSQLScript("GetTransitionCount.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &getTransitionCountStmt, nullptr);

	sql = loadSQLScript("IncrementTransitionCount.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &incrementTransitionCountStmt, nullptr);

	sql = loadSQLScript("InsertTransition.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &insertTransitionStmt, nullptr);

	sql = loadSQLScript("GetTransitionsFromSituation.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &getTransitionsFromSituationStmt, nullptr);

	sql = loadSQLScript("RetrieveAllTransitions.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &retrieveAllTransitionsStmt, nullptr);

	sql = loadSQLScript("GetMaxObjects.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &getMaxObjectsStmt, nullptr);

	sql = loadSQLScript("GetNumberOfSituations.sql");
	sqlite3_prepare_v2(db, sql.c_str(), -1, &getNumberOfSituationsStmt, nullptr);
}

void SQLiteManager::finalizeStatements() {
	sqlite3_finalize(getObjectIDStmt);
	sqlite3_finalize(insertVehicleStmt);
	sqlite3_finalize(insertObjectValueStmt);
	sqlite3_finalize(insertSituationStmt);
	sqlite3_finalize(insertSituationObjectStmt);
	sqlite3_finalize(retrieveVehicleStmt);
	sqlite3_finalize(retrieveSituationObjectIDsStmt);
	sqlite3_finalize(getTransitionCountStmt);
	sqlite3_finalize(incrementTransitionCountStmt);
	sqlite3_finalize(insertTransitionStmt);
	sqlite3_finalize(getTransitionsFromSituationStmt);
	sqlite3_finalize(retrieveAllTransitionsStmt);
	sqlite3_finalize(getMaxObjectsStmt);
	sqlite3_finalize(getNumberOfSituationsStmt);
}

RECORDING_API void SQLiteManager::storeTransition(Transition const & transition) {
	sqlite3_int64 fromID = getSituationID(*(transition.getFromSituation()));

	//DEBUG
	boolean insertSituationToCalled = false;
	boolean insertSituationFromCalled = false;
	//if Situation was not found insert new Situation
	if(fromID < 0) {
		fromID = insertSituation(*transition.getFromSituation());
		insertSituationFromCalled = true;
	}

	sqlite3_int64 toID = getSituationID(*transition.getToSituation());
	if(toID < 0) {
		toID = insertSituation(*transition.getToSituation());
		insertSituationToCalled = true;
	}
	if (fromID == toID) {
		throw std::exception("There should be no case of transitions between two equal situations");
	}
	sqlite3_bind_int64(getTransitionCountStmt, sqlite3_bind_parameter_index(getTransitionCountStmt, "@FromSituation"), fromID);
	sqlite3_bind_int64(getTransitionCountStmt, sqlite3_bind_parameter_index(getTransitionCountStmt, "@ToSituation"), toID);
	int r = sqlite3_step(getTransitionCountStmt);
	sqlite3_reset(getTransitionCountStmt);
	sqlite3_clear_bindings(getTransitionCountStmt);
	if(r == SQLITE_ROW) {
		//Transition was found
		sqlite3_bind_int64(incrementTransitionCountStmt, sqlite3_bind_parameter_index(incrementTransitionCountStmt, "@FromSituation"), fromID);
		sqlite3_bind_int64(incrementTransitionCountStmt, sqlite3_bind_parameter_index(incrementTransitionCountStmt, "@ToSituation"), toID);
		int r = sqlite3_step(incrementTransitionCountStmt);
		sqlite3_reset(incrementTransitionCountStmt);
		sqlite3_clear_bindings(incrementTransitionCountStmt);
	} else {
		//Transition does not exist
		sqlite3_bind_int64(insertTransitionStmt, sqlite3_bind_parameter_index(insertTransitionStmt, "@FromSituation"), fromID);
		sqlite3_bind_int64(insertTransitionStmt, sqlite3_bind_parameter_index(insertTransitionStmt, "@ToSituation"), toID);
		int r = sqlite3_step(insertTransitionStmt);
		sqlite3_reset(insertTransitionStmt);
		sqlite3_clear_bindings(insertTransitionStmt);
	}
}

std::unique_ptr<SituationGraph> SQLiteManager::retrieveSubSituationGraph(shared_ptr<Situation> root) {
	if (!root->isInitial()) {
		return nullptr;
	}
	map<int, shared_ptr<Situation>> retrievedSituations;
	map<int, shared_ptr<PredicateVehicle>> retrievedVehicles;
	vector<shared_ptr<Transition>> transitions;
	queue<std::pair<int, shared_ptr<Situation>>> todo;
	int initialID;
	if((initialID = getSituationID(*root)) < 0) {
		return nullptr;
	}
	todo.push(std::pair<int, shared_ptr<Situation>>(initialID, root));
	retrievedSituations[initialID] = root;

	while(!todo.empty()) {
		std::pair<int, shared_ptr<Situation>> currentSituation = todo.front();
		todo.pop();

		sqlite3_bind_int(getTransitionsFromSituationStmt, sqlite3_bind_parameter_index(getTransitionsFromSituationStmt, "@FromSituation"), currentSituation.first);
		while(sqlite3_step(getTransitionsFromSituationStmt) == SQLITE_ROW) {
			//for each transition
			int toSituationID = sqlite3_column_int(getTransitionsFromSituationStmt, 0);
			int transitionCount = sqlite3_column_int(getTransitionsFromSituationStmt, 1);
			double prob = sqlite3_column_double(getTransitionsFromSituationStmt, 2);
			shared_ptr<Situation> toSituation;
			if(retrievedSituations.find(toSituationID) != retrievedSituations.end()) {
				toSituation = retrievedSituations.at(toSituationID);
			} else {
				shared_ptr<Situation> toSituation = retrieveSituation(toSituationID, retrievedVehicles);
				retrievedSituations[toSituationID] = toSituation;
				todo.push(std::pair<int, shared_ptr<Situation>>(toSituationID, toSituation));
			}
			shared_ptr<Transition> t = std::make_shared<Transition>(currentSituation.second, retrievedSituations[toSituationID], transitionCount, prob);
			currentSituation.second->addTransition(t);
			transitions.push_back(t);
		}
		sqlite3_reset(getTransitionsFromSituationStmt);
		sqlite3_clear_bindings(getTransitionsFromSituationStmt);
	}

	unique_ptr<SituationGraph> sg = std::make_unique<SituationGraph>(transitions);
	return move(sg);
}

unique_ptr<SituationGraph> SQLiteManager::retrieveSituationGraph() {
	map<int, shared_ptr<PredicateVehicle>> retrievedVehicles;
	map<int, shared_ptr<Situation>> retrievedSituations;
	vector<shared_ptr<Transition>> transitions;

	while(sqlite3_step(retrieveAllTransitionsStmt) == SQLITE_ROW) {
		int fromID = sqlite3_column_int(retrieveAllTransitionsStmt, 0);
		int toID = sqlite3_column_int(retrieveAllTransitionsStmt, 1);
		int transitionCount = sqlite3_column_int(retrieveAllTransitionsStmt, 2);
		float transitionProbability = sqlite3_column_double(retrieveAllTransitionsStmt, 3);
		shared_ptr<Situation> from;
		shared_ptr<Situation> to;
		if(retrievedSituations.find(fromID) != retrievedSituations.end()) {
			from = retrievedSituations.at(fromID);
		} else {
			from = retrieveSituation(fromID, retrievedVehicles);
			retrievedSituations[fromID] = from;
		}
		if(retrievedSituations.find(toID) != retrievedSituations.end()) {
			to = retrievedSituations.at(toID);
		} else {
			to = retrieveSituation(toID, retrievedVehicles);
			retrievedSituations[toID] = to;
		}
		shared_ptr<Transition> t = std::make_shared<Transition>(from, to, transitionCount, transitionProbability);
		from->addTransition(t);
		transitions.push_back(t);
	}
	sqlite3_reset(retrieveAllTransitionsStmt);
	unique_ptr<SituationGraph> sg = std::make_unique<SituationGraph>(transitions);
	return move(sg);
}

RECORDING_API int SQLiteManager::getMaxNumberOfObjects()
{
	int res = 0;
	if(sqlite3_step(getMaxObjectsStmt) == SQLITE_ROW) {
		res = sqlite3_column_int(getMaxObjectsStmt, 0);
	};
	sqlite3_reset(getMaxObjectsStmt);
	return res;
}

RECORDING_API int SQLiteManager::getNumberOfSituations()
{
	int res = 0;
	if (sqlite3_step(getNumberOfSituationsStmt) == SQLITE_ROW) {
		res = sqlite3_column_int(getNumberOfSituationsStmt, 0);
	}
	sqlite3_reset(getNumberOfSituationsStmt);
	return res;
}
