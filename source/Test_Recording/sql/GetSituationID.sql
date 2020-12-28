SELECT DISTINCT SO.Situation_ID
FROM (((MultiObjectCountView SO JOIN Situations S ON SO.Situation_ID = S.Situation_ID AND S.Is_Initial = @IsInitial)
	INNER JOIN Situation_Objects SO2 ON (SO.Situation_ID = SO2.Situation_ID AND SO2.Role_in_Situation = 10 AND SO2.Object_ID = @VUTID))
	INNER JOIN (OBJECT_COUNT_LIST) AS VC ON (SO.Object_ID = VC.Object_ID AND SO.Role_in_Situation = VC.Role_in_Situation AND SO.Object_Count = VC.Object_Count))
	INNER JOIN SituationObjectCount SOC ON SO.Situation_ID = SOC.Situation_ID
GROUP BY SO.Situation_ID
HAVING SUM(SO.Object_Count) = (SOC.CountOfObjects) AND Count(SO.Object_ID) = @DistinctObjectCount;