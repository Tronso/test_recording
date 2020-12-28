SELECT To_Situation, Transition_Count, Transition_Probability
FROM Transitions
WHERE From_Situation = @FromSituation;