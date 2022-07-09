declare @nation   tinyint
declare @race	tinyint
declare @class	smallint
declare @level 	smallint
declare @exp 	int
declare @loyalty	int
declare @knights smallint
declare @fame	tinyint
declare @hp	smallint
declare @mp	smallint
declare @sp	smallint
declare @str	tinyint
declare @sta	tinyint
declare @dex	tinyint
declare @intel	tinyint
declare @cha	tinyint
declare @points	smallint
declare @gold	int
declare @zone	tinyint
declare @skillval	tinyint
declare @PX	int
declare @PZ	int


declare @value tinyint
declare @skill varchar(10)
declare @temp varbinary(1)
declare @userid char(20)

SET @value = 0

SET @level = 1
SET @exp = 0
SET @loyalty = 0
SET @knights = 0
SET @fame = 0
SET @hp = 100
SET @mp = 100
SET @sp = 0
SET @points = 10
SET @gold = 0
SET @skillval = 0



declare @tempPoint smallint
declare @tempStr smallint
declare @tempSta smallint
declare @tempDex smallint
declare @tempIntel smallint
declare @tempCha smallint
declare @templevel smallint
declare @totalpoint smallint
declare @incpoint smallint
declare @pluspoint smallint
SET @tempPoint = 0
SET @tempStr = 0
SET @tempSta = 0
SET @tempDex = 0
SET @tempIntel = 0
SET @tempCha = 0
SET @templevel = 0
SET @totalpoint = 0
SET @incpoint = 0
SET @pluspoint = 0

DECLARE User_Cursor CURSOR
	FOR
	SELECT strUserID, [Level], Strong, Sta, Dex, Intel, Cha, Points  FROM USERDATA

	OPEN User_Cursor

	FETCH NEXT FROM User_Cursor INTO @UserID,  @tempLevel, @tempStr, @tempSta, @tempDex, @tempIntel, @tempCha, @tempPoint

WHILE @@FETCH_STATUS = 0
BEGIN
	SET @totalpoint = @tempStr + @tempSta + @tempDex + @tempIntel + @tempCha + @tempPoint
	SET @incpoint =( ( @tempLevel - 1 ) * 3) + 300

	IF @totalpoint < @incpoint
	BEGIN
		SET @pluspoint = @incpoint - @totalpoint
		SET @Points = @tempPoint + @pluspoint
		select @userid, @tempPoint, @pluspoint
--		update userdata set Points = @Points where struserid = @userid
	END

	FETCH NEXT FROM User_Cursor INTO @UserID,  @tempLevel, @tempStr, @tempSta, @tempDex, @tempIntel, @tempCha, @tempPoint
END

CLOSE User_Cursor
DEALLOCATE User_Cursor

GO
