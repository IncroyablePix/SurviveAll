/*-------------------------------------------------------------------------------//
\\  - Fonctions utiles du serveur -
//
\\  	Fonctions globales:
//
\\
//      Fonctions locales:
\\          RandomEx(min, max)
//
\\
//-------------------------------------------------------------------------------*/

#if defined FUNCTIONS_INCLUDED

	#endinput

#endif

#define FUNCTIONS_INCLUDED

#include <[SA]Defines.inc>
#include <YSI\y_va>

//---PROTOTYPES
forward Float:GetDistanceToPoint(playerid,Float:x1,Float:y1,Float:z1);
forward Float:GetDistanceBetweenPoints(Float:x1,Float:y1,Float:z1,Float:x2,Float:y2,Float:z2);
forward Float:GetDistanceBetweenPlayers(playerid,playerid2);
forward CreateRemoteExplosionEx(playerid, Float:x, Float:y, Float:z, type, Float:size);

//---MACROS
#define IsMultiple(%0,%1) 					(((%0)%(%1))==0)
#define GetZAngleBetweenPos(%0,%1,%2,%3)    (atan2(%3 - %1, %2 - %0))
#define Log	                        		printf

//---FONCTIONS
stock swap(&xp, &yp)
{
    if (xp != yp)
    {
	    xp = xp + yp;
	    yp = xp - yp;
	    xp = xp - yp;
	}
}

stock swapfloat(&Float:xp, &Float:yp)
{
    if (xp != yp)
    {
	    xp = xp + yp;
	    yp = xp - yp;
	    xp = xp - yp;
	}
}

stock TimePassedSince(tick)
{
	return gettime() - tick;
}

stock SetVehicleEngineState(vehicleid, bool:on)//Pour allumer ou �teindre le v�hicule
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, on, lights, alarm, doors, bonnet, boot, objective);
}

stock IsEngineOn(vehicleid)//On regarde si le moteur d'un v�hicule est allum�
{
	new engine, other;
	if(!IsValidVehicle(vehicleid)) return 0;
	GetVehicleParamsEx(vehicleid, engine, other, other, other, other, other, other);//On r�cup�re la variable de tous les �tats parce qu'on a pas trop trop le choix, mais on garde que 'engine'
	return engine;
}

stock Float:GetDistanceToPoint(playerid,Float:x1,Float:y1,Float:z1)//Fonction pour chopper la distance entre un joueur et un point
{
	if(IsPlayerConnected(playerid))
	{
		new Float:x2,Float:y2,Float:z2;
		GetPlayerPos(playerid,x2,y2,z2);
		return GetDistanceBetweenPoints(x1,y1,z1,x2,y2,z2);
	}
	return INF_FLOAT;
}

stock Float:GetDistanceBetweenPoints(Float:x1,Float:y1,Float:z1,Float:x2,Float:y2,Float:z2)//Fonction pour r�cup�rer la taille du vecteur entre deux points
{
	new Float:answer;
	answer = floatadd(floatadd(floatsqroot(floatpower(floatsub(x1,x2),2)),floatsqroot(floatpower(floatsub(y1,y2),2))),floatsqroot(floatpower(floatsub(z1,z2),2)));
	return answer;
}

stock Float:GetDistanceBetweenPlayers(playerid,playerid2)
{
	if(IsPlayerConnected(playerid) && IsPlayerConnected(playerid2))
	{
		new Float:x1,Float:y1,Float:z1;
		GetPlayerPos(playerid2,x1,y1,z1);
		return GetDistanceToPoint(playerid,x1,y1,z1);
	}
	return INF_FLOAT;
}

stock GetGasStationPos(stationid, &Float:x, &Float:y, &Float:z)
{
	new Float:fGasPos[19][3] =
	{
 		{1942.3717, -1772.8774, 13.6406}, // EssenceIdlewood
 		{1003.8336, -938.1062, 42.3281}, // EssenceMulholland
 		{655.7141, -565.0311, 16.3359}, // EssenceDillimore
 		{1382.3101, 460.1329, 20.3452}, // EssenceMontgommery
 		{-90.6118, -1169.1027, 2.4131}, // EssenceFlint
 		{-1605.8292, -2714.3301, 48.5335}, // EssenceWhetstone
 		{-2244.0784, -2560.8418, 31.9219}, // Essence AngelPine
 		{-2026.5938, 156.8833, 29.0391}, // EssenceDoherty
 		{-2411.4363, 975.3588, 45.4609}, // EssenceJuniperHills
 		{-1675.8247, 413.2252, 7.1797}, // EssenceEasterBassin
 		{-1328.0435, 2677.4443, 50.0625}, // EssenceElQuebrados
 		{-1471.4037, 1863.8733, 32.6328}, // EssenceTierraRobada
 		{70.7088, 1219.0365, 18.8127}, // EssenceFortCarson
 		{2202.3484, 2474.6194, 10.8203}, // EssenceEmeraldIsle
 		{2147.4009, 2747.7183, 10.8203}, // EssencePricklePine
 		{2639.8784, 1106.3835, 10.8203}, // EssenceJuliusThruway
 		{2114.5493, 920.0220, 10.8203}, // EssenceComeALot
 		{618.8538, 1684.9341, 6.9922}, // EssenceBoneCounty
 		{1596.3087, 2198.7498, 10.8203} // EssenceRedsandWest
	};
	x = fGasPos[stationid][0];
	y = fGasPos[stationid][1];
	z = fGasPos[stationid][2];
}
stock GetXYInFrontOfPoint(&Float:x, &Float:y, Float:a, Float:distance)//Pour d�terminer trigonom�triquement la position sur les axes X & Y d'une position en fonction de ses angles
{
	x += (distance * floatsin(-a, degrees));
	y += (distance * floatcos(-a, degrees));
}

stock RandomEx(min, max)//Simplification du random classique avec valeur minimale et maximale
{
	return random(max - min) + min;
}

stock Float:floatrand(Float:min, Float:max)
{
	new imin = floatround(min);
	return floatdiv(float(random((floatround(max)-imin)*100)+(imin*100)),100.0);
}

stock Success(pc)
{
	new bool:bPourcent[100] = {false};
	for(new i = 0; i < pc; i ++)
	{
		new rand = 0;
		do rand = random(100);
		while(bPourcent[rand]);
		bPourcent[rand] = true;
	}
	return bPourcent[random(100)];
}

stock strtok(const string[], &index)
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

stock strrest(const string[], &index)
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

stock GetWeaponModel(weaponid)
{
	new model = -1;
	switch(weaponid)
	{
		case 1: model = 331; case 2: model = 333; case 3: model = 334;
		case 4: model = 335; case 5: model = 336; case 6: model = 337;
		case 7: model = 338; case 8: model = 339; case 9: model = 341;
		case 10: model = 321; case 11: model = 322; case 12: model = 323;
		case 13: model = 324; case 14: model = 325; case 15: model = 326;
		case 16: model = 342; case 17: model = 343; case 18: model = 344;
		case 22: model = 346; case 23: model = 347; case 24: model = 348;
		case 25: model = 349; case 26: model = 350; case 27: model = 351;
		case 28: model = 352; case 29: model = 353; case 30: model = 355;
		case 31: model = 356; case 32: model = 372; case 33: model = 357;
		case 34: model = 358; case 35: model = 359; case 36: model = 360;
		case 37: model = 361; case 38: model = 362; case 39: model = 363;
		case 41: model = 365; case 42: model = 366; case 46: model = 371;
	}
	//if(model < 300 || model == 342 || model == 346 || model == 348 || model == 355 || model == 356 || model == 359 || model == 362) return -1;//weapons i dont want to be dropped
	return model;
}

stock FixAngle(&Float:angle)
{
	if(angle < 0.0)
	{
		if(angle < -360.0)
		{
			new dTimes = floatround(floatdiv(angle, 360.0), floatround_floor);
			angle += dTimes * 360.0;
		}
		angle = 360.0 + angle;
	}
	else if(angle > 360.0)
	{
		new dTimes = floatround(floatdiv(angle, 360.0), floatround_floor);
		angle -= dTimes * 360.0;
	}
}

stock Float:GetVehicleSpeed(vehicleid, bool:bUseMPH = false)
{
	new Float:speed_x, Float:speed_y, Float:speed_z, Float:temp_speed;
	GetVehicleVelocity(vehicleid, speed_x, speed_y, speed_z);
	if(bUseMPH == false)
	{
	    temp_speed = floatsqroot(((speed_x*speed_x)+(speed_y*speed_y))+(speed_z*speed_z))*136.666667;
	}
	else
	{
	    temp_speed = floatsqroot(((speed_x*speed_x)+(speed_y*speed_y))+(speed_z*speed_z))*85.4166672;
	}
	floatround(temp_speed,floatround_round);
	return temp_speed;
}

stock LockVehicle(vehicleid, bool:lock)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, lock, bonnet, boot, objective);
}

stock SetVehicleObjectiveForPlayer(playerid, vehicleid, bool:toggle)
{
	new dump, doors;
	GetVehicleParamsEx(vehicleid, dump, dump, dump, doors, dump, dump, dump);
	SetVehicleParamsForPlayer(vehicleid, playerid, toggle, doors);
}

stock ClearChat(playerid)
{
	for(new i = 0; i < 100; i ++)
	{
	    SendClientMessage(playerid, 0x00000000, " ");
	}
}

stock GiveVehicleHealth(vehicleid, Float:health)
{
	if(IsValidVehicle(vehicleid))
	{
		new Float:h;
		GetVehicleHealth(vehicleid, h);
		SetVehicleHealth(vehicleid, (h - health < 0.0) ? 0.0 : h + health);
	}
}

stock IsPlayerCloseToObject(objectid, Float:distance)
{
	if(IsValidObject(objectid))
	{
	    new Float:xo, Float:yo, Float:zo, Float:xp2, Float:yp, Float:zp;
	    GetObjectPos(objectid, xo, yo, zo);
		new closestplayer = INVALID_PLAYER_ID;
		new Float:closestdist = 9999.9999;
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			GetPlayerPos(i, xp2, yp, zp);
			new Float:dist = GetDistanceBetweenPoints(xo, yo, zo, xp2, yp, zp);
		    if(!IsPlayerNPC(i) && IsPlayerConnected(i))
	        {
				if(distance > dist && dist < closestdist)
				{
					closestdist = dist;
					closestplayer = i;
				}
			}
		}
		return closestplayer;
	}
	return INVALID_PLAYER_ID;
}

stock GetClosestPlayerToObject(objectid)
{
	if(IsValidObject(objectid))
	{
	    new Float:xo, Float:yo, Float:zo, Float:xp2, Float:yp, Float:zp;
	    GetObjectPos(objectid, xo, yo, zo);
		new closestplayer=-1;
		new Float:closestdist = 9999.9999;
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			GetPlayerPos(i, xp2, yp, zp);
			new Float:dist = GetDistanceBetweenPoints(xo, yo, zo, xp2, yp, zp);
		    if(!IsPlayerNPC(i) && IsPlayerConnected(i))
	        {
				if (dist < closestdist)
				{
					closestdist = dist;
					closestplayer = i;
				}
			}
		}
		return closestplayer;
	}
	return -1;
}

stock GetVectorFromSegment(Float:x, Float:y, Float:z, Float:x2, Float:y2, Float:z2, &Float:vx, &Float:vy, &Float:vz)
{
	new Float:fSize = VectorSize(x - x2, y - y2, z - z2);
	vx = floatdiv(x - x2, fSize);
	vy = floatdiv(y - y2, fSize);
	vz = floatdiv(z - z2, fSize);
}

stock SetPlayerRaceCheckpointEx(playerid, type, Float:x, Float:y, Float:z, Float:nextx, Float:nexty, Float:nextz, Float:size)
{
    SetPlayerRaceCheckpoint(playerid, type, x, y, z, nextx, nexty, nextz, size);
	/*fCPpos[playerid][0] = x;
	fCPpos[playerid][1] = y;
	fCPpos[playerid][2] = z;*/
    SetPlayerMapIcon(playerid, CP_MAPICON_ID, x, y, z, 48, 0, MAPICON_GLOBAL);
}

stock DisablePlayerRaceCheckpointEx(playerid)
{
    DisablePlayerRaceCheckpoint(playerid);
	/*fCPpos[playerid][0] = 0.0;
	fCPpos[playerid][1] = 0.0;
	fCPpos[playerid][2] = 0.0;*/
    RemovePlayerMapIcon(playerid, CP_MAPICON_ID);
}

stock GetTimeString(seconds)
{
    new hours, minutes;
    hours = floatround(floatdiv(seconds, 3600), floatround_floor);
    minutes = floatround(floatdiv(seconds, 60), floatround_floor);
    seconds = seconds % 60;
	//---
    new string[10];
    if(hours != 0)
    {
        if(hours < 10) strcat(string, "0");
        strcat(string, inttostr(hours));
		strcat(string, ":");
    }
    if(minutes != 0)
    {
        if(minutes < 10) strcat(string, "0");
        strcat(string, inttostr(minutes));
    }
	else
    {
        strcat(string, "00");
    }
	strcat(string, ":");
    if(seconds != 0)
    {
        if(seconds < 10) strcat(string, "0");
        strcat(string, inttostr(seconds));
    }
	else
    {
        strcat(string, "00");
    }
	return string;
}

stock GetPlayerVehicleSpeed(playerid)
{
    new vehicleid = GetPlayerVehicleID(playerid);
    if(!vehicleid) return -1;
    new Float:speed_x, Float:speed_y, Float:speed_z;
    GetVehicleVelocity(vehicleid,speed_x,speed_y,speed_z);
    return floatround(floatsqroot(((speed_x*speed_x)+(speed_y*speed_y))+(speed_z*speed_z))*136.666667,floatround_round);
}

stock GetPlayerSpeed(playerid)
{
    new Float:ST[4];
    GetPlayerVelocity(playerid,ST[0],ST[1],ST[2]);
    ST[3] = floatsqroot(floatpower(floatabs(ST[0]), 2.0) + floatpower(floatabs(ST[1]), 2.0) + floatpower(floatabs(ST[2]), 2.0)) * 179.28625;
    return floatround(ST[3]);
}

stock inttostr(integer)
{
    new string[11];
	valstr(string, integer);
	return string;
}

public CreateRemoteExplosionEx(playerid, Float:x, Float:y, Float:z, type, Float:size)
{
    return CallRemoteFunction("CreateExplosionEx", "ifffif", playerid, x, y, z, type, size);
}

stock LogInfo(bool:admin, const fmat[], va_args<>)
{
	new string[196];
	va_format(string, 196, fmat, va_start<2>);
	printf(string);
	if(admin)
	{
		#if defined FILE_SURVIVEALL
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) if(IsPlayerReadingLogs(i)) SendClientMessage(i, VERT_CLAIR, string);
		#endif
	}
}

stock GetWeaponBoxAmmo(weaponid)
{
	new ammo = 0;
	switch(weaponid)
	{
		case 24: ammo = 10;//.50AE
		case 25, 26, 27: ammo = 10;//12 GAUGE
		case 22, 23, 28, 29, 32: ammo = 25;//9MM
		case 30, 31, 38: ammo = 30;//7.62
		case 33, 34: ammo = 10;//.222
		default: ammo = 0;
	}
	return ammo;
}

stock GetWeaponClipSize(weaponid)
{
	switch(weaponid)
	{
		case 22: return 34;
		case 23: return 17;
		case 24: return 7;
		case 25: return 1;
		case 26: return 4;
		case 27: return 7;
		case 28: return 100;
		case 29: return 30;
		case 30: return 30;
		case 31: return 50;
		case 32: return 100;
		case 33: return 1;
		case 34: return 1;
		case 35, 36: return 1;
		case 37: return 50;
		case 38: return 500;
		default: return -1;
	}
	return -1;
}
