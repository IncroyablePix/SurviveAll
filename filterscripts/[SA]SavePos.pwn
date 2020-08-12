#include <a_samp>

public OnPlayerCommandText(playerid, cmdtext[])
{
	new cmd[256];
	new idx;
	cmd = strtok(cmdtext,idx);
	if(strcmp(cmd, "/savepos", true) == 0)
	{
		new tmp[255];
		new Float:x, Float:y, Float:z;
		tmp = strrest(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xCC0000FF, "USAGE: /savepos [INFOS]");
			return 1;
		}
		GetPlayerPos(playerid, x, y, z);
		format(tmp, sizeof(tmp), "{%.4f, %.4f, %.4f},//%s\r\n", x, y, z, tmp);
		new File:rfile;
		if(fexist("/Survive-All/ItemPos.ini"))
		{
  			rfile = fopen("/Survive-All/ItemPos.ini",io_append);
		}
		else
		{
  			rfile = fopen("/Survive-All/ItemPos.ini", io_write);
		}
		if(!rfile)
		{
			return 0;
		}
		fwrite(rfile, tmp);
		fclose(rfile);
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
	

strrest(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}
	new offset = index;
	new result[128];
	while ((index < length) && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}
