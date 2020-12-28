CREATE TABLE IF NOT EXISTS Object_Types (
	Type_ID INTEGER PRIMARY KEY,
	Type_Description TEXT
);

CREATE TABLE IF NOT EXISTS Object_Roles (
	Role_ID INTEGER PRIMARY KEY,
	Role_DeSCRIPTION TEXT
);

CREATE TABLE IF NOT EXISTS Value_Table (
	Value_ID INTEGER PRIMARY KEY,
	Value_Description TEXT
);

CREATE TABLE IF NOT EXISTS Objects (
	Object_ID INTEGER PRIMARY KEY,
	Object_Type INTEGER,
	Is_Stable INTEGER,
	FOREIGN KEY(Object_Type) REFERENCES Object_Types(Type_ID)
);

CREATE TABLE IF NOT EXISTS Object_Values (
	Object_ID INTEGER,
	Value_Type INTEGER,
	Value_ID INTEGER,
	FOREIGN KEY(Object_ID) REFERENCES Objects(Object_ID)
);

CREATE TABLE IF NOT EXISTS Situations (
	Situation_ID INTEGER PRIMARY KEY,
	Is_Initial BOOLEAN
);

CREATE TABLE IF NOT EXISTS Situation_Objects (
	Situation_ID INTEGER,
	Role_in_Situation INTEGER,
	Object_ID INTEGER,
	FOREIGN KEY(Object_ID) REFERENCES Objects(Object_ID)
);

CREATE TABLE IF NOT EXISTS Transitions (
	From_Situation INTEGER,
	To_Situation INTEGER,
	Transition_Count INTEGER,
	Transition_Probability FLOAT,
	FOREIGN KEY(From_Situation) REFERENCES Situations(Situation_ID),
	FOREIGN KEY(To_Situation) REFERENCES Situations(Situation_ID),
	PRIMARY KEY(From_Situation, To_Situation)
);

CREATE TRIGGER IF NOT EXISTS UpdateTransitionProbabilityDelete
AFTER DELETE ON Transitions
BEGIN
	UPDATE Transitions
	SET Transition_Probability = 
		CAST(Transition_Count AS FLOAT) / CAST((
			SELECT T_Sum FROM (
				SELECT DISTINCT From_Situation AS F_S, SUM(Transition_Count) AS T_Sum
				FROM Transitions
				GROUP BY From_Situation
			)
			WHERE From_Situation = F_S) AS FLOAT
		);
END;

CREATE TRIGGER IF NOT EXISTS UpdateTransitionProbabilityInsert
AFTER Insert ON Transitions
BEGIN
	UPDATE Transitions
	SET Transition_Probability = 
		CAST(Transition_Count AS FLOAT) / CAST((
			SELECT T_Sum FROM (
				SELECT DISTINCT From_Situation AS F_S, SUM(Transition_Count) AS T_Sum
				FROM Transitions
				GROUP BY From_Situation
			)
			WHERE From_Situation = F_S) AS FLOAT
		);
END;

CREATE TRIGGER IF NOT EXISTS UpdateTransitionProbabilityUpdate
AFTER UPDATE OF Transition_Count ON Transitions
BEGIN
	UPDATE Transitions
	SET Transition_Probability = 
		CAST(Transition_Count AS FLOAT) / CAST((
			SELECT T_Sum FROM (
				SELECT DISTINCT From_Situation AS F_S, SUM(Transition_Count) AS T_Sum
				FROM Transitions
				GROUP BY From_Situation
			)
			WHERE From_Situation = F_S) AS FLOAT
		);
END;

CREATE VIEW IF NOT EXISTS MultiObjectCountView AS
	SELECT Situation_ID, Role_in_Situation, Object_ID, Count(Object_ID) AS Object_Count
	FROM Situation_Objects
	GROUP BY Situation_ID, Object_ID, Role_in_Situation;

CREATE VIEW IF NOT EXISTS SituationObjectCount AS
	SELECT Situation_ID, Count(Object_ID) AS CountOfObjects
	FROM Situation_Objects
	GROUP BY Situation_ID;

INSERT OR IGNORE INTO Object_Types(Type_ID, Type_Description) VALUES
	(10, 'Vehicle');

INSERT OR IGNORE INTO Object_Roles(Role_ID, Role_Description) VALUES
	(10, 'VUT'),
	(11, 'Other Vehicle');

INSERT OR IGNORE INTO Value_Table(Value_ID, Value_Description) VALUES
	(102030, 'RelX::BEHIND'),
	(102040, 'RelX::CLOSE_BEHIND'),
	(102050, 'RelX::SAME_HEIGHT'),
	(102060, 'RelX::CLOSE_IN_FRONT'),
	(102070, 'RelX::IN_FRONT'),

	(103040, 'RelV::SLOWER'),
	(103050, 'RelV::SAME'),
	(103060, 'RelV::FASTER'),

	(105040, 'Acc::BRAKE'),
	(105050, 'Acc::NONE'),
	(105060, 'Acc::ACCELERATE'),

	(106040, 'LnChg::RIGHT'),
	(106050, 'LnChg::NONE'),
	(106060, 'LnChg::LEFT');
