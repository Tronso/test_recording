SELECT DISTINCT v.Object_ID
FROM Object_Values v JOIN Objects o ON v.Object_ID = o.Object_ID
WHERE v.Value_ID IN (@RelX, @RelV, @Lane, @Acc, @LnChg)
	AND o.is_Stable = @IsStable
GROUP BY v.Object_ID
HAVING COUNT(DISTINCT Value_ID) = 5;