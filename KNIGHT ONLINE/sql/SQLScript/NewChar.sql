
DECLARE @nRet		smallint 
DECLARE @AccountID 	char(20) 
DECLARE @index 		tinyint
DECLARE @CharID	char(20)
DECLARE @Race 		tinyint
DECLARE @Class 	tinyint 
DECLARE @Hair 		tinyint
DECLARE @Face 		tinyint 
DECLARE @Str 		tinyint 
DECLARE @Sta 		tinyint 
DECLARE @Dex 		tinyint
DECLARE @Intel 		tinyint 
DECLARE @Cha 		tinyint



SET @AccountID = 'tigger'
SET @index = 0
SET @CharID = 'tigger2'
SET @Race = 1
SET @Class = 0
SET @Hair = 0
SET @Face = 0
SET @Str = 100
SET @Sta = 101
SET @Dex = 102
SET @Intel = 103
SET @Cha = 104

DECLARE @Row tinyint, @Nation tinyint
	SET @Row = 0	SET @Nation = 0

	SELECT @Nation = bNation, @Row = bCharNum FROM ACCOUNT_CHAR WHERE strAccountID = @AccountID
	
	IF @Row >= 5	SET @nRet =  1
	
	IF @Nation = 1 AND @Race < 10	SET @nRet = 2
	ELSE IF @Nation = 2 AND @Race > 10	SET @nRet = 2
	ELSE IF @Nation <>1 AND @Nation <> 2	SET @nRet = 2
	
	SELECT @Row = COUNT(*) FROM USERDATA WHERE strUserId = @CharID
	SELECT @Row
	IF @Row > 0 	
	BEGIN
		SET @nRet =  3
		SELECT '100'
		RETURN
	END

	IF @nRet < 0
	BEGIN
		SET @nRet =  5
		SELECT '200'
		RETURN
	END

BEGIN TRAN	
	IF @index = 0
		UPDATE ACCOUNT_CHAR SET strCharID1 = @CharID, bCharNum = bCharNum + 1 WHERE strAccountID = @AccountID
	ELSE IF @index = 1
		UPDATE ACCOUNT_CHAR SET strCharID2 = @CharID, bCharNum = bCharNum + 1 WHERE strAccountID = @AccountID
	ELSE IF @index = 2
		UPDATE ACCOUNT_CHAR SET strCharID3 = @CharID, bCharNum = bCharNum + 1 WHERE strAccountID = @AccountID
	ELSE IF @index = 3
		UPDATE ACCOUNT_CHAR SET strCharID4 = @CharID, bCharNum = bCharNum + 1 WHERE strAccountID = @AccountID
	ELSE IF @index = 4
		UPDATE ACCOUNT_CHAR SET strCharID5 = @CharID, bCharNum = bCharNum + 1 WHERE strAccountID = @AccountID
	

	INSERT INTO USERDATA (strUserId, Nation, Race, Class, HairColor, Face, [Str], Sta, Dex, Intel, Cha ) 
	VALUES	 (@CharID, @Nation, @Race, @Class, @Hair, @Face, @Str, @Sta, @Dex, @Intel, @Cha)
	
	
	IF @@ERROR <> 0
	BEGIN	 
		ROLLBACK TRAN 
		SET @nRet =  4
		SELECT @nRet
		RETURN
	END
	
COMMIT TRAN
SET @nRet =  0

SELECT @nRet
GO
