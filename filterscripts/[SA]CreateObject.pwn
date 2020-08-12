#include <a_samp>

public OnPlayerCommandText(playerid, cmdtext[])
{
	new idx = 0;
	new cmd[128];
	cmd = strtok(cmdtext, idx);
	if (strcmp("/createitem", cmdtext, true, 10) == 0)
	{
	    new Float:x, Float:y, Float:z;
  		new tmp[256];
		tmp = strtok(cmdtext, idx);
		new objectid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		GetPlayerPos(playerid, x, y, z);
		CallRemoteFunction("CreateItem", "ifffbd", objectid, x, y, z, false, -1);
		return 1;
	}
	return 0;
}

strtok(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}
	new offset = index;
	new result[20];
	while ((index < length) && (string[index] > ' ') && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}
