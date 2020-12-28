SELECT Role_in_Situation, Object_ID, Is_Initial
FROM Situation_Objects JOIN Situations ON Situation_Objects.Situation_ID = Situations.Situation_ID
WHERE Situation_Objects.Situation_ID = @SituationID;