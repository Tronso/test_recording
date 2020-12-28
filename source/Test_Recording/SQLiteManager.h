#pragma once
#include "DataManager.h"
#include <map>
#include "sqlite3.h"

class PredicateVehicle;

class SQLiteManager :
	public DataManager {
public:
	/**
	* The constructor for an SQLiteManager tries to open the given database file in the database subdirectory.
	* If the file doesn't exist it will be created with all necessary tables.
	* After the file is opened all needed statements are prepared.
	* @param databaseName Name of the database file as a string
	* @throws Exception if the database couldn't be opened.
	*/
	RECORDING_API SQLiteManager(const std::string &databaseName);

	/**
	* closes the opened database file.
	*/
	RECORDING_API ~SQLiteManager();

	// Inherited via DataManager
	virtual RECORDING_API void storeTransition(Transition const & transition) override;
	virtual std::unique_ptr<SituationGraph> retrieveSubSituationGraph(std::shared_ptr<Situation> root) override;
	RECORDING_API virtual std::unique_ptr<SituationGraph> retrieveSituationGraph() override;
	RECORDING_API virtual int getMaxNumberOfObjects() override;
	RECORDING_API virtual int getNumberOfSituations() override;

private:
	sqlite3 * db;
	/**
	* tries to load the given file and execute the contained sql script
	* @parameter file The name of the file that conatins the script. Base path used is the "./sql" directory
	* @parameter callback callback function to be used on the results
	* @parameter err Buffer for the error message
	*/
	void loadAndExecuteSQLScript(std::string const & file, int(*callback)(void*, int, char**, char**), char *err);

	/**
	* tries to execute the given sql script
	* @parameter query the query in a string format
	* @parameter callback callback function to be used on the results
	* @parameter err Buffer for the error message
	*/
	void executeQuery(std::string const & query, int(*callback)(void*, int, char**, char**), char *err);

	/**
	* tries to load the given file
	* @parameter file The name of the file that conatins the script. Base path used is the "./sql" directory
	* @returns the content of the file
	*/
	std::string loadSQLScript(std::string const & file);

	/**
	* looks up the situation in the database and tries to find its ID
	* @parameter situation Reference to the Situation that will be looked up
	* @return the ID of the Situation in the database if it was found, -1 else
	*/
	sqlite3_int64 getSituationID(Situation const & situation);

	/**
	* looks up the vehicle in the database and tries to find its ID
	* @parameter vehicle Pointer to the Vehicle that will be looked up
	* @return the ID of the Vehicle in the database if it was found, -1 else
	*/
	sqlite3_int64 getObjectID(PredicateVehicle const & vehicle);

	/**
	* Inserts a new Object with the Type '10' and then inserts the values to the given vehicle
	* @param vehicle Pointer to the PredicateVehicle to be inserted into the database
	* @return the ID of the inserted Object
	*/
	sqlite3_int64 insertVehicle(PredicateVehicle const & vehicle);

	/**
	* Inserts a new Situation into the database and returns the new ID
	* @param situation Pointer to the Situation that should be inserted
	* @param ids map of the id in the situation to the objectID
	* @return ID of the inserted Situation
	*/
	sqlite3_int64 insertSituation(Situation const & situation);

	/**
	* Builds a PredicateVehicle object from the database entry with the given ID.
	* @param objectID ID of the entry in the database.
	* @return Pointer to the constructed PredicateVehicle.
	*/
	std::shared_ptr<PredicateVehicle> retrieveVehicle(int objectID);

	/**
	* Builds a Situation object from the database entry with the given ID.
	* @param situationID ID of the entry in the database.
	* @param retrievedVehicles Reference to a map of objectIDs to retrievedVehicles. Is updated by this method.
	* @return Pointer to the constructed Situation.
	*/
	std::shared_ptr<Situation> retrieveSituation(int situationID, std::map<int, std::shared_ptr<PredicateVehicle>> &retrievedVehicles);

	/**
	* Helper function that prepares all statements the data manager needs.
	*/
	void prepareStatements();

	/**
	* Helper function that finalizes all statements the data manager needs.
	*/
	void finalizeStatements();

	//String used for the getSituationID statement which can't be reused because of necessary string replacement.
	std::string getSituationIDStr;

	sqlite3_stmt *getObjectIDStmt;
	sqlite3_stmt *insertVehicleStmt;
	sqlite3_stmt *insertObjectValueStmt;
	sqlite3_stmt *insertSituationStmt;
	sqlite3_stmt *insertSituationObjectStmt;
	sqlite3_stmt *retrieveVehicleStmt;
	sqlite3_stmt *retrieveSituationObjectIDsStmt;
	sqlite3_stmt *getTransitionCountStmt;
	sqlite3_stmt *incrementTransitionCountStmt;
	sqlite3_stmt *insertTransitionStmt;
	sqlite3_stmt *getTransitionsFromSituationStmt;
	sqlite3_stmt *retrieveAllTransitionsStmt;
	sqlite3_stmt *getMaxObjectsStmt;
	sqlite3_stmt *getNumberOfSituationsStmt;
};

