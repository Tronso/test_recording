UPDATE Transitions
SET Transition_Count = Transition_Count + 1
WHERE From_Situation = @FromSituation AND To_Situation = @ToSituation;