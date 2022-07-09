declare @nYear smallint, @nAge smallint, @strSocNo char(14)
declare @n10 smallint, @n13 smallint, @n16 smallint, @n19 smallint, @n24 smallint, @n29 smallint, @n30 smallint, @n40 smallint, @n50 smallint, @n60 smallint
declare @nMale int, @nFemale int, @nGender int

set @n10 = 0 set @n13 = 0 set @n16 = 0 set @n19 = 0 set @n24 = 0 set @n29 = 0 set @n30 = 0 set @n40 = 0 set @n50 = 0 set @n60 = 0
set @nMale = 0 set @nFemale = 0

DECLARE Age_Cursor CURSOR
	FOR
	SELECT strSocNo FROM [knight].TB_USER 

	OPEN Age_Cursor

	FETCH NEXT FROM Age_Cursor INTO @strSocNo

WHILE @@FETCH_STATUS = 0
BEGIN
	SET @nYear = CONVERT( smallint, SUBSTRING( @strSocNo, 1, 2 ) )
	SET @nAge = 102 - @nYear + 1

	IF @nAge < 11  
		SET @n10 = @n10 + 1
	ELSE IF @nAge > 10 and @nAge < 14 
		SET @n13 = @n13 + 1
	ELSE IF @nAge > 13 and @nAge < 17 
		SET @n16 = @n16 + 1
	ELSE IF @nAge > 16 and @nAge < 20 
		SET @n19 = @n19 + 1
	ELSE IF @nAge > 19 and @nAge < 25
		SET @n24 = @n24 + 1
	ELSE IF @nAge > 24 and @nAge < 30 
		SET @n29 = @n29 + 1
	ELSE IF @nAge > 29 and @nAge < 40 
		SET @n30 = @n30 + 1
	ELSE IF @nAge > 39 and @nAge < 50 
		SET @n40 = @n40 + 1
	ELSE IF @nAge > 49 and @nAge < 60 
		SET @n50 = @n50 + 1
	ELSE IF @nAge > 59 
		SET @n60 = @n60 + 1

	SET @nGender = CONVERT( smallint, SUBSTRING( @strSocNo, 8, 1 ) )
	IF @nGender = 1
		SET @nMale = @nMale + 1
	ELSE IF @nGender = 2
		SET @nFemale = @nFemale + 1

	FETCH NEXT FROM Age_Cursor INTO @strSocNo
END

CLOSE Age_Cursor
DEALLOCATE Age_Cursor

SELECT @n10, @n13, @n16, @n19, @n24, @n29, @n30, @n40, @n50, @n60, @nMale, @nFemale