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
#include <streamer.inc>
#include <[SA]Defines.inc>
#include <[SA]Functions.inc>

//---FONCTIONS

//---MACROS

//---VARIABLES
new aTestArea;
new bool:bSendMessages;
new oTest = INVALID_OBJECT_ID;
new dLastHitNPC;
new bool:bHUDHidden[MAX_PLAYERS];
//PORTES CAMPS


//------------------------------------------------//

public OnFilterScriptInit()
{
	aTestArea = CreateDynamicCircle(0.0, 0.0, 10.0, -1, -1, -1);
	print("\n--------------------------------------");
	print("Survive-All debugmode - [Pix]");
	print("- Loaded");
	print("--------------------------------------\n");
	return 1;
}

public OnFilterScriptExit()
{
	DestroyDynamicArea(aTestArea);
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

public OnPlayerConnect(playerid)
{
    bHUDHidden[playerid] = false;
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    new string[256];
	new cmd[256];
	new idx;
	cmd = strtok(cmdtext, idx);
	if(strcmp(cmd, "/mission", false) == 0)
	{
		format(string, sizeof(string), "Current mission: %d", CallRemoteFunction("GetPlayerMission", "i", playerid));
		SendClientMessage(playerid, 0xFFFFFFFF, string);
	    return 1;
	}
	if(strcmp(cmd, "/wphk", false) == 0)
	{
		GivePlayerWeapon(playerid, 24, 10);
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
	if(strcmp(cmd, "/sound", true) == 0)
	{
		new tmp[128];
		new soundid = 0;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /sound [SOUDID]");
 			return 1;
		}
		soundid = strval(tmp);
		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);
		PlayerPlaySound(playerid, soundid, x, y, z);
		return 1;
   	}
   	if(strcmp(cmd, "/bridge", true) == 0)
   	{
   	    static bBridge;
   	    if(!bBridge)
   	    {
   	        CallRemoteFunction("MoveMissionMapping", "ib", 0, true);
   	        bBridge = true;
   	    }
   	    else
   	    {
   	        CallRemoteFunction("MoveMissionMapping", "ib", 0, false);
            bBridge = false;
   	    }
   	    return 1;
   	}
	if(strcmp(cmd, "/testobject", true)== 0)
	{
	    static bool:bObject;
	    if(!bObject)
	    {
			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid, x, y, z);
			oTest = CreateObject(-1003, x, y, z, 0.0, 12.0, 90.0);
			format(string, sizeof(string), "ID objet: %d", oTest);
			SendClientMessage(playerid, 0xCC000000, string);
			bObject = true;
		}
		else
		{
		    DestroyObject(oTest);
		    oTest = INVALID_OBJECT_ID;
		    bObject = false;
		}
 		return 1;
	}
	if(strcmp(cmd, "/helper", true)== 0)
	{
	    static dObject = INVALID_OBJECT_ID;
	    new Float:x, Float:y, Float:z;
	    GetPlayerPos(playerid, x, y, z);
	    if(IsValidObject(dObject))
	    {
			/*new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid, x, y, z);*/
			dObject = CreateObject(-1000, x, y, z, 0.0, 0.0, 0.0);
		}
		else
		{
		    DestroyObject(dObject);
		    dObject = INVALID_OBJECT_ID;
		}
 		return 1;
	}
	if(strcmp(cmd, "/saveveh", true) == 0)
    {
        new description[128];
		description = strrest(cmdtext, idx);
		new Float:x, Float:y, Float:z, Float:angle;
		GetVehiclePos(GetPlayerVehicleID(playerid), x, y, z);
		GetVehicleZAngle(GetPlayerVehicleID(playerid), angle);
		format(string, sizeof(string), "{%.4f, %.4f, %.4f, %.4f}, //%s\r\n", x, y, z, angle, description);
		new File:rfile;
		if(fexist("/Survive-All/Positions/vehicles.ini"))
			{
			    rfile = fopen("/Survive-All/Positions/vehicles.ini",io_append);
			}
		else
			{
			    rfile = fopen("/Survive-All/Positions/vehicles.ini", io_write);

			}
		if(!rfile)
			{
			    return 0;
			}
		fwrite(rfile, string);
		fclose(rfile);
        return 1;
    }
	if(strcmp(cmd, "/saveobj", true) == 0)
    {
        new description[128];
		description = strrest(cmdtext, idx);
		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);
		format(string, sizeof(string), "{%.4f, %.4f, %.4f, CAT}, //%s\r\n", x, y, z, description);
		new File:rfile;
		if(fexist("/Survive-All/Positions/items.ini"))
			{
			    rfile = fopen("/Survive-All/Positions/items.ini",io_append);
			}
		else
			{
			    rfile = fopen("/Survive-All/Positions/items.ini", io_write);

			}
		if(!rfile)
			{
			    return 0;
			}
		fwrite(rfile, string);
		fclose(rfile);
        return 1;
    }
    if(strcmp(cmd, "/hud", true) == 0)
    {
        bHUDHidden[playerid] = (bHUDHidden[playerid]) ? false : true;
        CallRemoteFunction("HidePlayerHUD", "ib", playerid, bHUDHidden[playerid]);
        return 1;
    }
    if(strcmp(cmd, "/closearea", true) == 0)
    {
		CallRemoteFunction("ChangeAreaGate", "b", false);
        return 1;
    }
    if(strcmp(cmd, "/debug", true) == 0)
    {
        bSendMessages = !bSendMessages;
        return 1;
    }
	if(strcmp(cmd, "/killactors", true) == 0)
	{
	    for(new i = 0; i < MAX_ACTORS; i ++) if(IsValidActor(i)) DestroyActor(i);
	    return 1;
	}
    if(strcmp(cmd, "/choosezombie", true) == 0)
    {
        format(string, sizeof(string), "Zombie: %d", CallRemoteFunction("GetFreeZombie", ""));
		SendClientMessage(playerid, 0xFFFFFFFF, string);
        return 1;
    }
    if(strcmp(cmd, "/lasthit", true) == 0)
    {
        format(string, sizeof(string), "NPC: %d", dLastHitNPC);
		SendClientMessage(playerid, 0xFFFFFFFF, string);
        return 1;
    }
	if(strcmp(cmd, "/distanceto", true) == 0)
    {
		new tmp[256];
        new giveplayerid, info[128];
		tmp = strtok(cmdtext, idx);
		giveplayerid = strval(tmp);
		info = strrest(cmdtext, idx);
		new Float:distance = GetDistanceBetweenPlayers(playerid, giveplayerid);
		format(string, sizeof(string), "Distance vers %d: %f (%s)", giveplayerid, distance, info);
		SendClientMessage(playerid, 0xFFFFFFFF, string);
        return 1;
    }
	if(strcmp(cmd, "/ztm", true) == 0)
	{
		new tmp[256];
		new zid, missionid, Float:x, Float:y, Float:z;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /ztm [ID] [MISSIONID] [X] [Y] [Z]");
			return 1;
		}
		zid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /ztm [ID] [MISSIONID] [X] [Y] [Z]");
			return 1;
		}
		missionid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /ztm [ID] [MISSIONID] [X] [Y] [Z]");
			return 1;
		}
		x = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /ztm [ID] [MISSIONID] [X] [Y] [Z]");
			return 1;
		}
		y = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /ztm [ID] [MISSIONID] [X] [Y] [Z]");
			return 1;
		}
		z = floatstr(tmp);
		CallRemoteFunction("AddZombieToMission", "iifff", zid, missionid, x, y, z);
		return 1;
	}
	if(strcmp(cmd, "/cp", true) == 0)
	{
		SetPlayerRaceCheckpoint(playerid, 2, 253.3275, -22.6066, 1.6141, 0.0, 0.0, 0.0, 1.0);
	    return 1;
	}
	if(strcmp(cmd, "/jtm", true) == 0)
	{
		new tmp[256];
		new zid, skin, weaponid, type, missionid, friendid, Float:x, Float:y, Float:z;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /jtm [ID] [SKINID] [WEAPONID] [TYPE] [MISSIONID] [FRIENDID] [X] [Y] [Z]");
			return 1;
		}
		zid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /jtm [ID] [SKINID] [WEAPONID] [TYPE] [MISSIONID] [FRIENDID] [X] [Y] [Z]");
			return 1;
		}
		skin = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /jtm [ID] [SKINID] [WEAPONID] [TYPE] [MISSIONID] [FRIENDID] [X] [Y] [Z]");
			return 1;
		}
		weaponid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /jtm [ID] [SKINID] [WEAPONID] [TYPE] [MISSIONID] [FRIENDID] [X] [Y] [Z]");
			return 1;
		}
		type = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /jtm [ID] [SKINID] [WEAPONID] [TYPE] [MISSIONID] [FRIENDID] [X] [Y] [Z]");
			return 1;
		}
		missionid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /jtm [ID] [SKINID] [WEAPONID] [TYPE] [MISSIONID] [FRIENDID] [X] [Y] [Z]");
			return 1;
		}
		friendid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /jtm [ID] [SKINID] [WEAPONID] [TYPE] [MISSIONID] [FRIENDID] [X] [Y] [Z]");
			return 1;
		}
		x = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /jtm [ID] [SKINID] [WEAPONID] [TYPE] [MISSIONID] [FRIENDID] [X] [Y] [Z]");
			return 1;
		}
		y = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /jtm [ID] [SKINID] [WEAPONID] [TYPE] [MISSIONID] [FRIENDID] [X] [Y] [Z]");
			return 1;
		}
		z = floatstr(tmp);
		CallRemoteFunction("AddJohnToMission", "iiiiiiifff", zid, skin, type, weaponid, missionid, 1, friendid, x, y, z);
		return 1;
	}
	if(strcmp(cmd, "/giveweapon", true) == 0)
	{
		new tmp[256];
		new giveplayerid, weaponid, ammo;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /giveweapon [ID] [WEAPONID] [AMMO]");
			return 1;
		}
		giveplayerid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /giveweapon [ID] [WEAPONID] [AMMO]");
			return 1;
		}
		weaponid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /giveweapon [ID] [WEAPONID] [AMMO]");
			return 1;
		}
		ammo = strval(tmp);
		//---
  		CallRemoteFunction("GivePlayerWeaponEx", "iii", giveplayerid, weaponid, ammo);
		return 1;
	}
	if(strcmp(cmd, "/setzombiespos", true) == 0)
	{
		new tmp[256];
		new zid, missionid, Float:x, Float:y, Float:z;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setzombiespos [AMOUT] [MISSIONID] [X] [Y] [Z]");
			return 1;
		}
		zid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setzombiespos [AMOUT] [MISSIONID] [X] [Y] [Z]");
			return 1;
		}
		missionid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setzombiespos [AMOUT] [MISSIONID] [X] [Y] [Z]");
			return 1;
		}
		x = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setzombiespos [AMOUT] [MISSIONID] [X] [Y] [Z]");
			return 1;
		}
		y = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setzombiespos [AMOUT] [MISSIONID] [X] [Y] [Z]");
			return 1;
		}
		z = floatstr(tmp);
		for(new i = 0; i < zid; i ++) CallRemoteFunction("AddZombieToMission", "iifff", i, missionid, x, y, z);
		return 1;
	}
	if(strcmp(cmd, "/waypoint", true) == 0)
	{
		new tmp[256];
		new zid, Float:x, Float:y, Float:z;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /waypoint [ID] [X] [Y] [Z]");
			return 1;
		}
		zid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /waypoint [ID] [X] [Y] [Z]");
			return 1;
		}
		x = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /waypoint [ID] [X] [Y] [Z]");
			return 1;
		}
		y = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /waypoint [ID] [X] [Y] [Z]");
			return 1;
		}
		z = floatstr(tmp);
		CallRemoteFunction("SetZombieWayPoint", "ifff", zid, x, y, z);
		return 1;
	}
	if(strcmp(cmd, "/death", true) == 0)
	{
	    static aDeath = INVALID_ACTOR_ID;
	    if(aDeath == INVALID_ACTOR_ID)
	    {
/*AddPlayerClass(20006,-1843.8413,580.3185,242.0142,240.3487,0,0,0,0,0,0); // MortNW
AddPlayerClass(20006,-1769.4579,580.4846,242.0156,120.3499,0,0,0,0,0,0); // MortNE
AddPlayerClass(20006,-1806.9146,515.8952,242.0156,359.4685,0,0,0,0,0,0); // MortS
*/
			aDeath = CallRemoteFunction("CreateActorEx", "iffff", 20100, -1806.9146, 515.8952, 242.0156, 359.4685);
	    }
	    else
	    {
	        DestroyActor(aDeath);
	        aDeath = INVALID_ACTOR_ID;
	    }
	    return 1;
	}
	if(strcmp(cmd, "/setavel", true) == 0)
	{
		new tmp[256];
		new giveplayerid, Float:x, Float:y, Float:z;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setavel [ID] [X] [Y] [Z]");
			return 1;
		}
		giveplayerid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setavel [ID] [X] [Y] [Z]");
			return 1;
		}
		x = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setavel [ID] [X] [Y] [Z]");
			return 1;
		}
		y = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setavel [ID] [X] [Y] [Z]");
			return 1;
		}
		z = floatstr(tmp);
		SetVehicleAngularVelocity(GetPlayerVehicleID(giveplayerid), x, y, z);
		return 1;
	}
	if(strcmp(cmd, "/setvel", true) == 0)
	{
		new tmp[256];
		new giveplayerid, Float:x, Float:y, Float:z;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setvel [ID] [X] [Y] [Z]");
			return 1;
		}
		giveplayerid = strval(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setvel [ID] [X] [Y] [Z]");
			return 1;
		}
		x = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setvel [ID] [X] [Y] [Z]");
			return 1;
		}
		y = floatstr(tmp);
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /setvel [ID] [X] [Y] [Z]");
			return 1;
		}
		z = floatstr(tmp);
		if(IsPlayerInAnyVehicle(giveplayerid)) SetVehicleVelocity(GetPlayerVehicleID(giveplayerid), x, y, z);
		else SetPlayerVelocity(giveplayerid, x, y, z);
		return 1;
	}
	return 0;
}

public OnPlayerEnterDynamicArea(playerid, areaid)
{
	if(bSendMessages)
	{
	    if(areaid == aTestArea)
	    {
		    new string[128];
	        format(string, sizeof(string), "One entered test area.", dLastHitNPC);
			SendClientMessageToAll(0xFFFFFFFF, string);
	    }
	}
	return 1;
}

public OnPlayerLeaveDynamicArea(playerid, areaid)
{
	if(bSendMessages)
	{
	    if(areaid == aTestArea)
	    {
		    new string[128];
	        format(string, sizeof(string), "One entered test area.", dLastHitNPC);
			SendClientMessageToAll(0xFFFFFFFF, string);
	    }
	}
	return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	if(IsPlayerNPC(damagedid))
	{
		dLastHitNPC = damagedid;
		if(bSendMessages)
		{
		    new string[128];
	        format(string, sizeof(string), "NPC: %d", dLastHitNPC);
			SendClientMessage(playerid, 0xFFFFFFFF, string);
	    }
	}
	return 1;
}

/*public OnPlayerStreamIn(playerid, forplayerid)
{
    new string[128], sName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, sName, MAX_PLAYER_NAME + 1);
    format(string, sizeof(string), "%s streamed", sName);
    SendClientMessage(forplayerid, 0xFFFFFFFF, string);
    return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
    new string[128], sName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, sName, MAX_PLAYER_NAME + 1);
    format(string, sizeof(string), "%s streamed out", sName);
    SendClientMessage(forplayerid, 0xFFFFFFFF, string);
    return 1;
}*/
