/*-------------------------------------------------------------------------------//
\\  - Mapping statique du serveur -
//
\\  	Fonctions globales:
//          DestroyMapping(playerid)
\\          CheckForPlayersToGates()
\\          
//
//      Fonctions locales:
\\          CreateGates()
//          CreateMappingObject(mappartid)
\\          DestroyGates()
//          DestroyStaticMapping()
\\          ChangeGateState(gateid, bool:open)
//          PreRemoveBuildings()
\\
//-------------------------------------------------------------------------------*/

#include <a_samp>
#include <[SA]Defines.inc>
#include <[SA]Functions.inc>

//---FONCTIONS

//---MACROS

//---VARIABLES
new oTest = INVALID_OBJECT_ID;
//PORTES CAMPS


//------------------------------------------------//

public OnFilterScriptInit()
{
	print("\n--------------------------------------");
	print("Survive-All debugmode - [Pix]");
	print("- Loaded");
	print("--------------------------------------\n");
	return 1;
}

public OnFilterScriptExit()
{
	print("\n--------------------------------------");
	print("Survive-All debugmode - [Pix]");
	print("- Unloaded");
	print("--------------------------------------\n");
	if(oTest != INVALID_OBJECT_ID)
	{
	    DestroyObject(oTest);
	}
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    new string[128];
	new cmd[256];
	new idx;
	cmd=strtok(cmdtext, idx);
	if(strcmp(cmd, "/mission", false) == 0)
	{
		format(string, sizeof(string), "Current mission: %d", CallRemoteFunction("GetPlayerMission", "i", playerid));
		SendClientMessage(playerid, 0xFFFFFFFF, string);
	    return 1;
	}
	if(strcmp(cmd, "/getvel", true)== 0)
	{
		new Float:vels[3];
		if(IsPlayerInAnyVehicle(playerid)) GetVehicleVelocity(GetPlayerVehicleID(playerid), vels[0], vels[1], vels[2]);
		else GetPlayerVelocity(playerid, vels[0], vels[1], vels[2]);
		format(string, sizeof(string), "x:%.2f - y:%.2f - z:%.2f", vels[0], vels[1], vels[2]);
		SendClientMessage(playerid, -1, string);
 		return 1;
	}
	if(strcmp(cmd, "/setpos", true) == 0)
	{
		new tmp[256];
		new giveplayerid, Float:x, Float:y, Float:z;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setpos [ID] [X] [Y] [Z]");
			return 1;
		}
		giveplayerid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setpos [ID] [X] [Y] [Z]");
			return 1;
		}
		x = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setpos [ID] [X] [Y] [Z]");
			return 1;
		}
		y = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setpos [ID] [X] [Y] [Z]");
			return 1;
		}
		z = floatstr(tmp);
		SetPlayerPos(giveplayerid, x, y, z);
		return 1;
	}
	if(strcmp(cmd, "/vtp", true) == 0)
	{
		new tmp[128];
		new vehid = INVALID_VEHICLE_ID;
		new giveplayerid = INVALID_PLAYER_ID;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
 			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /vtp [ID] [VEHID]");
 			return 1;
		}
		giveplayerid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /vtp [ID] [VEHID]");
  			return 1;
		}
		vehid = strval(tmp);
		if(vehid == INVALID_VEHICLE_ID)
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /vtp [ID] [VEHID]");
			return 1;
		}
		new Float:x, Float:y, Float:z;
		GetPlayerPos(giveplayerid, x, y, z);
		SetVehiclePos(vehid, x + 1.0, y, z);
		return 1;
   	}
	if(strcmp(cmd, "/test", true)== 0)
	{
		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);
		oTest = CreateObject(18715, x, y, z, 0.0, 0.0, 0.0);
		SetObjectMaterial(oTest, 0, 19527, "cauldron1", "AlienLiquid1", 0xFFFFFFFF);
 		return 1;
	}
	return 0;
}
