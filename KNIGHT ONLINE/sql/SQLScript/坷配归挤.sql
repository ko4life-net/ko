declare @strUserID char(21), @Level tinyint, @Exp int
declare @Str tinyint, @Sta tinyint, @Dex tinyint, @Intel tinyint, @Cha tinyint, @Points tinyint, @strSkill varchar(10)

SET @strUserID = null SET @Level = 0 SET @Exp = 0 
SET @Str = 0 SET @Sta = 0 SET @Dex = 0 SET @Intel = 0 SET @Cha = 0 SET @Points = 0 SET @strSkill = null

declare @row tinyint
SET @row = 0

DECLARE Backup_Cursor CURSOR
	FOR
	SELECT strUserID, [Level], [Exp], Strong, Sta, Dex, Intel, Cha, Points, strSkill FROM BK_THU_USERDATA

	OPEN Backup_Cursor

	FETCH NEXT FROM Backup_Cursor INTO @strUserID, @Level, @Exp, @Str, @Sta, @Dex, @Intel, @Cha, @Points, @strSkill

WHILE @@FETCH_STATUS = 0
BEGIN
	UPDATE USERDATA 
	SET [Level] = @Level, [Exp] = @Exp, Strong = @Str, Sta = @Sta, Dex = @Dex, Intel = @Intel, Cha = @Cha, Points = @Points, strSkill = @strSkill WHERE strUserID = @strUserID and Nation = 2
	
	FETCH NEXT FROM Backup_Cursor INTO @strUserID, @Level, @Exp, @Str, @Sta, @Dex, @Intel, @Cha, @Points, @strSkill
END

CLOSE Backup_Cursor
DEALLOCATE Backup_Cursor
