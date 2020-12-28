SELECT Value_Type, Value_ID, Is_Stable
FROM Object_Values JOIN  Objects ON Object_Values.Object_ID = Objects.Object_ID
WHERE Objects.Object_ID = @ObjectID;