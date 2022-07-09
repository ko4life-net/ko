declare @strUserID char(21)
declare @Dex tinyint, @Cha tinyint, @Points tinyint, @tempDex tinyint, @tempCha tinyint, @Class tinyint

SET @strUserID = null
SET @Dex = 0 SET @Cha = 0 SET @Points = 0 SET @tempDex = 0 SET @tempCha = 0 SET @Class = 0

declare @row tinyint
SET @row = 0


--select @tempDex = Dex, @tempCha = Cha from userdata where strUserId = '하소연'

--SET @Cha = @tempDex - 70 + @tempCha
--SET @Dex = 70

--Update userdata set Cha = @Cha, Dex = @Dex where struserid = '하소연'



DECLARE Replace_Cursor CURSOR
	FOR
	SELECT strUserID,  Class, Dex, Cha FROM USERDATA

	OPEN Replace_Cursor

	FETCH NEXT FROM Replace_Cursor INTO @strUserID, @Class, @tempDex, @tempCha

WHILE @@FETCH_STATUS = 0
BEGIN
	IF @Class = 103 or @Class = 203 or @Class = 109 or @Class = 209
	BEGIN
		SET @Cha = @tempDex - 70 + @tempCha
		SET @Dex = 70
		UPDATE USERDATA SET  Dex = @Dex, Cha = @Cha WHERE strUserID = @strUserID
	END
	
	FETCH NEXT FROM Replace_Cursor INTO @strUserID, @Class, @tempDex, @tempCha
END

CLOSE Replace_Cursor
DEALLOCATE Replace_Cursor
