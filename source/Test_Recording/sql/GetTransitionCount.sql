SELECT Transition_Count
FROM Transitions
WHERE From_Situation = @FromSituation AND To_Situation = @ToSituation;