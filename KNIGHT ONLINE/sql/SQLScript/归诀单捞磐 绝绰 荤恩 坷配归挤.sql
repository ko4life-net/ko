declare @strUserID char(21), @Level tinyint, @Exp int, @Race tinyint
declare @Str tinyint, @Sta tinyint, @Dex tinyint, @Intel tinyint, @Cha tinyint, @Points tinyint, @strSkill varchar(10)

SET @strUserID = null SET @Level = 0 SET @Exp = 0 SET @Race = 0
SET @Str = 0 SET @Sta = 0 SET @Dex = 0 SET @Intel = 0 SET @Cha = 0 SET @Points = 0 SET @strSkill = null

declare @row tinyint
SET @row = 0

DECLARE Backup_Cursor CURSOR
	FOR
	SELECT strUserID, [Level], [Exp], Strong, Sta, Dex, Intel, Cha, Points, strSkill FROM USERDATA

	OPEN Backup_Cursor

	FETCH NEXT FROM Backup_Cursor INTO @strUserID, @Level, @Exp, @Str, @Sta, @Dex, @Intel, @Cha, @Points, @strSkill

WHILE @@FETCH_STATUS = 0
BEGIN
	select @row = count(*) from BK_THU_USERDATA where strUserID = @strUserID
	IF @row = 0 
	BEGIN
		SELECT @Race = Race FROM USERDATA where  strUserID = @strUserID and Nation = 2 and [Level] > 9

		IF @Race = 1 or @Race = 2 or @Race = 11
		BEGIN
			SET @str = 65
			SET @sta = 65
			SET @dex = 60
			SET @intel = 50
			SET @cha = 50
		END
		IF @Race = 3
		BEGIN
			SET @str = 50
			SET @sta = 50
			SET @dex = 70
			SET @intel = 70
			SET @cha = 50
		END
		IF @Race = 4
		BEGIN
			SET @str = 50
			SET @sta = 60
			SET @dex = 60
			SET @intel = 70
			SET @cha = 50
		END
		IF @Race = 12
		BEGIN
			SET @str = 60
			SET @sta = 60
			SET @dex = 70
			SET @intel = 50
			SET @cha = 50
		END
		IF @Race = 13
		BEGIN
			SET @str = 50
			SET @sta = 50
			SET @dex = 70
			SET @intel = 70
			SET @cha = 50
		END

		SET @Points = 27
		SET @strSkill = CAST(2 AS varbinary(1))

		UPDATE USERDATA 
		SET [Level] = 10, [Exp] = 0, Strong = @Str, Sta = @Sta, Dex = @Dex, Intel = @Intel, Cha = @Cha, Points = @Points, strSkill = @strSkill WHERE strUserID = @strUserID and Nation = 2 and [Level] > 9
		
		SET @Race = 0
	END
	
	FETCH NEXT FROM Backup_Cursor INTO @strUserID, @Level, @Exp, @Str, @Sta, @Dex, @Intel, @Cha, @Points, @strSkill
END

CLOSE Backup_Cursor
DEALLOCATE Backup_Cursor
