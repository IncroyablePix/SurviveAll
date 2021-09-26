#if defined DBCONFIG_INCLUDED

	#endinput

#endif

#define DBCONFIG_INCLUDED

static stock RemoveNewLine(str[])
{
	new position = -1;

	if((position = strfind(str, "\r\n")) != -1)
	{
	    str[position] = '\0';
	}
	else if((position = strfind(str, "\n")) != -1)
	{
	    str[position] = '\0';
	}
}

forward bool:ReadDBConfig(const path[], host[128], user[128], password[128], db[128]);
stock bool:ReadDBConfig(const path[], host[128], user[128], password[128], db[128])
{
	if(fexist(path))// fDbConfig)
	{
    	new File:fDbConfig = fopen(path, io_read);

		fread(fDbConfig, host);
		fread(fDbConfig, user);
		fread(fDbConfig, password);
		fread(fDbConfig, db);

		//--- Removing \n from each line

		printf("%d\n", strlen(host));
		RemoveNewLine(host);
		RemoveNewLine(user);
		RemoveNewLine(password);
		RemoveNewLine(db);

		fclose(fDbConfig);

		return true;
	}
	else
	{
	    return false;
 	}
}
