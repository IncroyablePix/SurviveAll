#include <a_samp>
#include <mapandreas>
#include <colandreas>

public OnFilterScriptInit()
{
	print("\n--------------------------------------");
	print(" Blank Filterscript by your name here");
	print("--------------------------------------\n");
	AnalyseMap();
	return 1;
}

public OnFilterScriptExit()
{
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if(strcmp("/savezpos", cmdtext, true, 10) == 0)
	{
	    new Float:x, Float:y, Float:z;
	    GetPlayerPos(playerid, x, y, z);
		MapAndreas_SetZ_For2DCoord(x, y, z - 1.0);
		return 1;
	}
	if(strcmp("/savecustomhmap", cmdtext, true, 10) == 0)
	{
	    MapAndreas_SaveCurrentHMap("scriptfiles/surviveall.hmap");
		return 1;
	}
	if(strcmp("/jetpack", cmdtext, true, 10) == 0)
	{
 		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_USEJETPACK);
 		return 1;
	}
	return 0;
}

AnalyseMap()
{
	printf("[HMAP]Analyzing map begin");
	new Float:z;
	new dTime = GetTickCount();
	for(new Float:x = -3000.0; x < 3000.0; x ++)
	{
	    for(new Float:y = -3000.0; y < 3000.0; y ++)
	    {
    		CA_FindZ_For2DCoord(x, y, z);
			MapAndreas_SetZ_For2DCoord(x, y, z);
	    }
	}
	MapAndreas_SaveCurrentHMap("scriptfiles/surviveall.hmap");
	printf("[HMAP]Analyzing map complete. Time %d", GetTickCount() - dTime);
}
