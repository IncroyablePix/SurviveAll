#if defined DYNAMICS_INCLUDED

	#endinput

#endif

#define DYNAMICS_INCLUDED

//---INCLUDES---//
#include <a_samp>
#include <YSI\y_ini>
#include <[SA]Defines.inc>

//---DEFINES---//
#define MAX_COLLECTORS                      (250)
#define MAX_SHREDDERS                      	(150)
#define MAX_FRIDGES                        	(50)
#define MAX_BRASEROS                        (200)
#define MAX_BOARDS                          (300)
#define MAX_PLANTS                          (500)
#define MAX_FIRES                         	(50)

#define FRPATH                              "/Survive-All/General/Fridges.ini"
#define CPATH                               "/Survive-All/General/Collectors.ini"
#define SHPATH                              "/Survive-All/General/Shredders.ini"
#define BRPATH                              "/Survive-All/General/Braseros.ini"
#define PPATH                               "/Survive-All/General/Plants.ini"
#define BBPATH                              "/Survive-All/General/Board.ini"
#define FPATH                               "/Survive-All/General/Fires.ini"

#define CAN_PICKUP_BRAZIER                  //Peut-on récupérer un braséro ?

//---FORWARDS---//
forward LoadFridges_data(name[], value[]);
forward LoadCollectors_data(name[], value[]);
forward LoadShredders_data(name[], value[]);
forward LoadBraseros_data(name[], value[]);
forward LoadPlants_data(name[], value[]);
forward LoadBoards_data(name[], value[]);
forward LoadFires_data(name[], value[]);

//---ENUMS---//
enum Collecteur
{
	dEau,
	oCollector[2],
	Text3D:CollectorText,
	Float:xEau,
	Float:yEau,
	Float:zEau,
	Float:aEau
}

enum Broyeur
{
	dBroyeur,
	oBroyeur,
	Float:xBroyeur,
	Float:yBroyeur,
	Float:zBroyeur,
	Float:aBroyeur
}

enum Brasero
{
	oBrasero,
	oFire,
	dBrasero,
	Float:xBrasero,
	Float:yBrasero,
	Float:zBrasero,
	Float:aBrasero
}

enum FridgeInfo
{
	oFridge,
	bool:bFridge,
	Float:xFridge,
	Float:yFridge,
	Float:zFridge,
	Float:aFridge,
	dFood[5],
	dFoodAmount[5]
}

enum Plante
{
	dPlantID,//1 = Blé - 2 = Oranger - 3 = Pommier - 4 = Tomates - 5  = Sapin
	dResistance,
	dFruits,
	oPlantObject,
	dGrowTime,
	Float:xPlant,
	Float:yPlant,
	Float:zPlant,
	Float:aPlant
}

enum Board
{
	oBoard[2],
	bool:bBoard,
	sBoardText[128],
	Float:xBoard,
	Float:yBoard,
	Float:zBoard,
	Float:aBoard
}

enum Feu
{
	oFeu,
	dTempsFeu,
	Float:xFeu,
	Float:yFeu,
	Float:zFeu,
	Float:aFeu
}

//---VARIABLES---//
static dFridgeInfos[MAX_FRIDGES][FridgeInfo];
static dCollector[MAX_COLLECTORS][Collecteur];
static dShredder[MAX_SHREDDERS][Broyeur];
static dBraseroInfos[MAX_BRASEROS][Brasero];
static dPlant[MAX_PLANTS][Plante];
static dBoard[MAX_BOARDS][Board];
static dFire[MAX_FIRES][Feu];

static pFridge[MAX_PLAYERS] = {-1, ...};

//---FONCTIONS---//
stock InitializeBraseroVariables() 
{
	for(new i = 0; i < MAX_BRASEROS; i ++)
	{
		dBraseroInfos[i][oBrasero] = INVALID_OBJECT_ID;
		dBraseroInfos[i][oFire] = INVALID_OBJECT_ID;
		dBraseroInfos[i][dBrasero] = 0;
		dBraseroInfos[i][xBrasero] = 0.0;
		dBraseroInfos[i][yBrasero] = 0.0;
		dBraseroInfos[i][zBrasero] = 0.0;
		dBraseroInfos[i][aBrasero] = 0.0;
	}
}

stock InitializeFridgesVariables() 
{
	for(new i = 0; i < MAX_FRIDGES; i ++)
	{
	    dFridgeInfos[i][oFridge] = INVALID_OBJECT_ID;
	    dFridgeInfos[i][bFridge] = false;
	    dFridgeInfos[i][xFridge] = 0.0;
	    dFridgeInfos[i][yFridge] = 0.0;
	    dFridgeInfos[i][zFridge] = 0.0;
	    dFridgeInfos[i][aFridge] = 0.0;
	    for(new j = 0; j < 5; j ++)
		{
	    	dFridgeInfos[i][dFood][j] = 0;
	    	dFridgeInfos[i][dFoodAmount][j] = 0;
		}
	}
}

stock InitializeCollectorsVariables()
{
	for(new i = 0; i < MAX_COLLECTORS; i ++)
	{
		dCollector[i][dEau] = -1;
		dCollector[i][oCollector][0] = INVALID_OBJECT_ID;
		dCollector[i][oCollector][1] = INVALID_OBJECT_ID;
		dCollector[i][CollectorText] = Text3D:INVALID_3DTEXT_ID;
		dCollector[i][xEau] = 0.0;
		dCollector[i][yEau] = 0.0;
		dCollector[i][zEau] = 0.0;
		dCollector[i][aEau] = 0.0;
	}
}

stock InitializeShreddersVariables()
{
	for(new i = 0; i < MAX_SHREDDERS; i ++)
	{
		dShredder[i][dBroyeur] = -1;
		dShredder[i][oBroyeur] = INVALID_OBJECT_ID;
		dShredder[i][xBroyeur] = 0.0;
		dShredder[i][yBroyeur] = 0.0;
		dShredder[i][zBroyeur] = 0.0;
		dShredder[i][aBroyeur] = 0.0;
	}
}

stock IsPlayerNearBrasero(playerid)
{
	for(new i = 0; i < MAX_BRASEROS; i ++) 
	{
		if(IsPlayerInRangeOfPoint(playerid, 3.5, dBraseroInfos[i][xBrasero], dBraseroInfos[i][yBrasero], dBraseroInfos[i][zBrasero]) && dBraseroInfos[i][dBrasero] == 2) return true;
	}
	return false;
}

stock CreateBrasero(Float:x, Float:y, Float:z, Float:angle, braserostate, load = -1)
{
	static slotid;
	if(load == -1)
	{
		if(dBraseroInfos[slotid][dBrasero] != 0)
		{
		    for(new i = 0; i < MAX_BRASEROS; i ++)
		    {
		        if(dBraseroInfos[i][dBrasero] == 0)
		        {
		            slotid = i;
		            break;
		        }
		    }
		}
	}
	if(load == -1 && dBraseroInfos[slotid][dBrasero] != 0) DestroyDynamicObject(dBraseroInfos[slotid][oBrasero]);
	if(load == -1 && dBraseroInfos[slotid][dBrasero] == 2)
	{
		DestroyDynamicObject(dBraseroInfos[slotid][oFire]);
		dBraseroInfos[slotid][oFire] = INVALID_OBJECT_ID;
	}
	//---
	dBraseroInfos[(load == -1) ? slotid : load][oBrasero] = CreateDynamicObject(11725, x, y, z - 0.6119, 0.0, 0.0, angle);
	if(braserostate == 2) dBraseroInfos[(load == -1) ? slotid : load][oFire] = CreateDynamicObject(18693, x, y, z - 2.265, 0.0, 0.0, angle, -1, -1, -1, 25.0);
	//---
	dBraseroInfos[(load == -1) ? slotid : load][xBrasero] = x;
	dBraseroInfos[(load == -1) ? slotid : load][yBrasero] = y;
	dBraseroInfos[(load == -1) ? slotid : load][zBrasero] = z;
	dBraseroInfos[(load == -1) ? slotid : load][aBrasero] = angle;
	dBraseroInfos[(load == -1) ? slotid : load][dBrasero] = braserostate;
	//---
    slotid ++;
    if(slotid == MAX_BRASEROS) slotid = 0;
    return (slotid == 0) ? MAX_BRASEROS : slotid - 1;
}

stock DestroyBrasero(braseroid)
{
	if(dBraseroInfos[braseroid][dBrasero] != 0)
	{
		DestroyDynamicObject(dBraseroInfos[braseroid][oBrasero]);
		dBraseroInfos[braseroid][oBrasero] = INVALID_OBJECT_ID;
		if(dBraseroInfos[braseroid][dBrasero] == 2)
		{
			DestroyDynamicObject(dBraseroInfos[braseroid][oFire]);
			dBraseroInfos[braseroid][oFire] = INVALID_OBJECT_ID;
		}
		dBraseroInfos[braseroid][dBrasero] = 0;
		dBraseroInfos[braseroid][xBrasero] = 0.0;
		dBraseroInfos[braseroid][yBrasero] = 0.0;
		dBraseroInfos[braseroid][zBrasero] = 0.0;
		dBraseroInfos[braseroid][aBrasero] = 0.0;
	}
}

stock LightBrasero(braseroid, bool:bstate)
{
	if(bstate && dBraseroInfos[braseroid][dBrasero] == 1)
	{
	    dBraseroInfos[braseroid][dBrasero] = 2;
	    dBraseroInfos[braseroid][oFire] = CreateDynamicObject(18693, dBraseroInfos[braseroid][xBrasero], dBraseroInfos[braseroid][yBrasero], dBraseroInfos[braseroid][zBrasero] - 2.265, 0.0, 0.0, dBraseroInfos[braseroid][aBrasero], -1, -1, -1, 25.0);
	}
	else if(!bstate && dBraseroInfos[braseroid][dBrasero] == 2)
	{
	    dBraseroInfos[braseroid][dBrasero] = 1;
	    if(dBraseroInfos[braseroid][oFire] != INVALID_OBJECT_ID)
		{
			DestroyDynamicObject(dBraseroInfos[braseroid][oFire]);
			dBraseroInfos[braseroid][oFire] = INVALID_OBJECT_ID;
		}
	}
	return 1;
}

public LoadBraseros_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_FIRES; i ++)
	{
	    format(string, sizeof(string), "Brasero%d", i);
		INI_Int(string, dBraseroInfos[i][dBrasero]);
	    format(string, sizeof(string), "xBrasero%d", i);
		INI_Float(string, dBraseroInfos[i][xBrasero]);
	    format(string, sizeof(string), "yBrasero%d", i);
		INI_Float(string, dBraseroInfos[i][yBrasero]);
	    format(string, sizeof(string), "zBrasero%d", i);
		INI_Float(string, dBraseroInfos[i][zBrasero]);
	    format(string, sizeof(string), "aBrasero%d", i);
		INI_Float(string, dBraseroInfos[i][aBrasero]);
	}
	return 1;
}

stock SaveBraseros()
{
	new string[50];
	new INI:File = INI_Open(BRPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_BRASEROS; i ++)
	{
	    format(string, sizeof(string), "Brasero%d", i);
		INI_WriteInt(File,string, dBraseroInfos[i][dBrasero]);
	    format(string, sizeof(string), "xBrasero%d", i);
		INI_WriteFloat(File,string, dBraseroInfos[i][xBrasero]);
	    format(string, sizeof(string), "yBrasero%d", i);
		INI_WriteFloat(File,string, dBraseroInfos[i][yBrasero]);
	    format(string, sizeof(string), "zBrasero%d", i);
		INI_WriteFloat(File,string, dBraseroInfos[i][zBrasero]);
	    format(string, sizeof(string), "aBrasero%d", i);
		INI_WriteFloat(File,string, dBraseroInfos[i][aBrasero]);
	}
	INI_Close(File);
}	

stock FindNearbyBraseros(playerid, Float:x, Float:y, Float:z, slot, tInArray[][][])
{
    //new len = sizeof(tInArray[]);
	new Float:fTrash;
	#if defined CAN_PICKUP_BRAZIER
    for(new i = 0; i < MAX_BRASEROS; i ++)
    {
        if(slot < 9) break;
        if(dItems[i][ItemID] == 0) continue;
	    if(IsPlayerInRangeOfPoint(playerid, 3.0, dBraseroInfos[i][xBrasero], dBraseroInfos[i][yBrasero], dBraseroInfos[i][zBrasero]))
        {
			if(CA_RayCastLine(x, y, z, dBraseroInfos[i][xBrasero], dBraseroInfos[i][yBrasero], dBraseroInfos[i][zBrasero], fTrash, fTrash, fTrash) != 0) continue;
            tInArray[playerid][slot][0] = i;
            tInArray[playerid][slot][1] = 0;
            slot ++;
        }
    }
	#endif

    return slot;
}

stock SetupBraseros() 
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_BRASEROS; i ++) dBraseroInfos[i][dBrasero] = 0;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(BRPATH, "LoadBraseros_data");
    #endif
	for(new i = 0; i < MAX_BRASEROS; i ++)
	{
		if(dBraseroInfos[i][dBrasero] != 0)
		{
			CreateBrasero(dBraseroInfos[i][xBrasero], dBraseroInfos[i][yBrasero], dBraseroInfos[i][zBrasero], dBraseroInfos[i][aBrasero], dBraseroInfos[i][dBrasero], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_BRASEROS; i ++) dBraseroInfos[i][dBrasero] = 0;
    LogInfo(true, "[INIT]Braseros charges");
}

stock IsValidBrasero(braseroid)
{
	return MAX_BRASEROS > braseroid >= 0;
}

stock GetBraseroObject(braseroid)
{
	return IsValidBrasero(braseroid) ? dBraseroInfos[i][oBrasero] : INVALID_OBJECT_ID;
}

stock IsBrasero(braseroid)
{
	return (dBraseroInfos[i][dBrasero] != 0);
}

stock IsPlayerInRangeOfBrasero(playerid, bool:onlyoff) {
	for(new i = 0; i < MAX_BRASEROS; i ++)
	{
		if(onlyoff && dBraseroInfos[i][dBrasero] != 1) continue;
		if(IsPlayerInRangeOfPoint(playerid, 2.0, dBraseroInfos[i][xBrasero], dBraseroInfos[i][yBrasero], dBraseroInfos[i][zBrasero]))
		{
			return i;
		}
	}

	return -1;
}

stock CreateFridge(Float:x, Float:y, Float:z, Float:angle, load = -1)
{
	static slotid;
	if(load == -1)
	{
		if(dFridgeInfos[slotid][bFridge])
		{
			for(new i = 0; i < MAX_FRIDGES; i ++)
		 	{
				if(!dFridgeInfos[i][bFridge])
			    {
			    	slotid = i;
			        break;
			    }
			}
			if(dFridgeInfos[slotid][bFridge])
			{
				DestroyDynamicObject(dFridgeInfos[slotid][oFridge]);
			}
		}
	}
	//---
	dFridgeInfos[(load == -1) ? slotid : load][bFridge] = true;
	dFridgeInfos[(load == -1) ? slotid : load][xFridge] = x;
	dFridgeInfos[(load == -1) ? slotid : load][yFridge] = y;
	dFridgeInfos[(load == -1) ? slotid : load][zFridge] = z;
	dFridgeInfos[(load == -1) ? slotid : load][aFridge] = angle;
	dFridgeInfos[(load == -1) ? slotid : load][oFridge] = CreateDynamicObject(2144, x, y, z - 1.0, 0.0, 0.0, angle, -1, -1, -1, 25.0, 20.0);
	//---
    slotid ++;
    if(slotid == MAX_FRIDGES) slotid = 0;
    return (slotid == 0) ? MAX_FRIDGES : slotid - 1;
}

stock DestroyFridge(fridgeid)
{
	DestroyDynamicObject(dFridgeInfos[fridgeid][oFridge]);
	dFridgeInfos[fridgeid][oFridge] = INVALID_OBJECT_ID;
	for(new i = 0; i < 5; i ++)
	{
	    if(dFridgeInfos[fridgeid][dFood][i] != 0)
	    {
			dFridgeInfos[fridgeid][dFood][i] = 0;
			dFridgeInfos[fridgeid][dFoodAmount][i] = 0;
		}
	}
	dFridgeInfos[fridgeid][xFridge] = 0.0;
	dFridgeInfos[fridgeid][yFridge] = 0.0;
	dFridgeInfos[fridgeid][zFridge] = 0.0;
	dFridgeInfos[fridgeid][aFridge] = 0.0;
}

stock AddFridgeFood(fridgeid, food, amount)
{
	new slotid = HasFridgeFood(fridgeid, food);
	if(slotid == -1)
	{
		slotid = GetFridgeNextFreeSlot(fridgeid);
		if(slotid != -1)
		{
			dFridgeInfos[fridgeid][dFood][slotid] = food;
			dFridgeInfos[fridgeid][dFoodAmount][slotid] += amount;
		}
	}
	else
	{
		dFridgeInfos[fridgeid][dFoodAmount][slotid] += amount;
		if(dFridgeInfos[fridgeid][dFoodAmount][slotid] == 0)
		{
			dFridgeInfos[fridgeid][dFood][slotid] = 0;
		}
	}
	return dFridgeInfos[fridgeid][dFoodAmount][slotid];
}

stock GetPlayerFridgeEdit(playerid)
{
	return IsValidPlayer(playerid) ? pFridge[playerid] : -1;
}

stock SetPlayerFridgeEdit(playerid, fridgeid)
{
	if(IsValidPlayer(playerid)) 
	{
		pFridge[playerid] = fridgeid;
	}
}

stock GetFridgeObject(fridgeid)
{
	return IsValidFridge(fridgeid) ? dFridgeInfos[fridgeid][oFridge] : INVALID_OBJECT_ID;
}

stock IsValidFridge(fridgeid)
{
	return MAX_FRIDGES > fridgeid >= 0;
}

stock HasFridgeFood(fridgeid, food)
{
	for(new i = 0; i < 5; i ++) if(dFridgeInfos[fridgeid][dFood][i] == food) return i;
	return -1;
}

stock GetFridgeFood(fridgeid, slotid, &food, &amount)
{
	if(4 >= slotid >= 0)
	{
		food = dFridgeInfos[fridgeid][dFood][slotid];
		amount = dFridgeInfos[fridgeid][dFoodAmount][slotid];
	}
}

stock IsPlayerNearFridge(playerid)
{
	new Float:x, Float:y, Float:z, Float:fTrash;
	GetPlayerPos(playerid, x, y, z);
	for(new i = 0; i < MAX_FRIDGES; i ++)
	{
		if(dFridgeInfos[i][bFridge] && IsPlayerInRangeOfPoint(playerid, 1.5, dFridgeInfos[i][xFridge], dFridgeInfos[i][yFridge], dFridgeInfos[i][zFridge]))
		{
			if(CA_RayCastLine(x, y, z, dFridgeInfos[i][xFridge], dFridgeInfos[i][yFridge], dFridgeInfos[i][zFridge], fTrash, fTrash, fTrash) != 0) continue;
			return i;
		}
	}
	return -1;
}

stock GetFridgeNextFreeSlot(fridgeid)
{
	if(fridgeid == -1) return -1;
	for(new i = 0; i < 5; i ++) if(dFridgeInfos[fridgeid][dFood][i] == 0) return i;
	return -1;
}

/*IsFridgeEmpty(fridgeid)
{
	if(rackid == -1) return true;
	new dSlots = 0;
	for(new i = 0; i < 5; i ++) if(dFridgeInfos[fridgeid][dFood][i] == 0) dSlots ++;
	return (dSlots == 5) ? true : false;
}*/

public LoadFridges_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_FRIDGES; i ++)
	{
	    format(string, sizeof(string), "Fridge%d", i);
		INI_Bool(string, dFridgeInfos[i][bFridge]);
	    format(string, sizeof(string), "xFridge%d", i);
		INI_Float(string, dFridgeInfos[i][xFridge]);
	    format(string, sizeof(string), "yFridge%d", i);
		INI_Float(string, dFridgeInfos[i][yFridge]);
	    format(string, sizeof(string), "zFridge%d", i);
		INI_Float(string, dFridgeInfos[i][zFridge]);
	    format(string, sizeof(string), "aFridge%d", i);
		INI_Float(string, dFridgeInfos[i][aFridge]);
		for(new j = 0; j < 5; j ++)
		{
	    	format(string, sizeof(string), "%dFood%d", j, i);
			INI_Int(string, dFridgeInfos[i][dFood][j]);
	    	format(string, sizeof(string), "%dFoodAmount%d", j, i);
			INI_Int(string, dFridgeInfos[i][dFoodAmount][j]);
		}
	}
	return 1;
}

stock SaveFridges()
{
	new string[50];
	new INI:File = INI_Open(FRPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_FRIDGES; i ++)
	{
	    format(string, sizeof(string), "Fridge%d", i);
		INI_WriteBool(File,string, dFridgeInfos[i][bFridge]);
	    format(string, sizeof(string), "xFridge%d", i);
		INI_WriteFloat(File,string, dFridgeInfos[i][xFridge]);
	    format(string, sizeof(string), "yFridge%d", i);
		INI_WriteFloat(File,string, dFridgeInfos[i][yFridge]);
	    format(string, sizeof(string), "zFridge%d", i);
		INI_WriteFloat(File,string, dFridgeInfos[i][zFridge]);
	    format(string, sizeof(string), "aFridge%d", i);
		INI_WriteFloat(File,string, dFridgeInfos[i][aFridge]);
		for(new j = 0; j < 5; j ++)
		{
	    	format(string, sizeof(string), "%dFood%d", j, i);
			INI_WriteInt(File,string, dFridgeInfos[i][dFood][j]);
	    	format(string, sizeof(string), "%dFoodAmount%d", j, i);
			INI_WriteInt(File,string, dFridgeInfos[i][dFoodAmount][j]);
		}
	}
	INI_Close(File);
}

stock FindNearbyFridges(playerid, Float:x, Float:y, Float:z, slot, tInArray[][][])
{
    //new len = sizeof(tInArray[]);
	new Float:fTrash;
    for(new i = 0; i < MAX_FRIDGES; i ++)
    {
        if(slot < 9) break;
	    if(!dFridgeInfos[i][bFridge]) continue;
	    if(IsPlayerInRangeOfPoint(playerid, 3.0, dFridgeInfos[i][xFridge], dFridgeInfos[i][yFridge], dFridgeInfos[i][zFridge]))
        {
			if(CA_RayCastLine(x, y, z, dFridgeInfos[i][xFridge], dFridgeInfos[i][yFridge], dFridgeInfos[i][zFridge], fTrash, fTrash, fTrash) != 0) continue;
            tInArray[playerid][slot][0] = i;
            tInArray[playerid][slot][1] = 0;
            slot ++;
        }
    }

    return slot;
}

stock SetupFridges()
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_FRIDGES; i ++) dFridgeInfos[i][bFridge] = false;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(FRPATH, "LoadFridges_data");
    #endif
	for(new i = 0; i < MAX_FRIDGES; i ++)
	{
	    if(dFridgeInfos[i][xFridge] == 0.0 && dFridgeInfos[i][yFridge] == 0.0 && dFridgeInfos[i][zFridge] == 0.0) dFridgeInfos[i][bFridge] = false;
		if(dFridgeInfos[i][bFridge])
		{
			CreateFridge(dFridgeInfos[i][xFridge], dFridgeInfos[i][yFridge], dFridgeInfos[i][zFridge], dFridgeInfos[i][aFridge], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_FRIDGES; i ++)
	{
		dFridgeInfos[i][bFridge] = false;
	    for(new j = 0; j < 5; j ++)
	    {
	        dFridgeInfos[i][dFood][j] = 0;
	        dFridgeInfos[i][dFoodAmount][j] = 0;
	    }
	}
    LogInfo(true, "[INIT]Frigos charges");
}

stock IsFridgeExisting(fridgeid) 
{
	return (dFridgeInfos[fridgeid][bFridge]);
}

stock CreateCollector(Float:x, Float:y, Float:z, Float:angle, water, load = -1)
{
	static slotid;
	if(load == -1)
	{
		if(dCollector[slotid][dEau] != -1)
		{
		    for(new i = 0; i < MAX_COLLECTORS; i ++)
		    {
		        if(dCollector[i][dEau] == -1)
		        {
		            slotid = i;
		            break;
		        }
		    }
		}
	}
	if(load == -1 && dCollector[slotid][dEau] != -1) DestroyDynamicObject(dCollector[slotid][oCollector][0]), DestroyDynamicObject(dCollector[slotid][oCollector][1]), DestroyDynamic3DTextLabel(dCollector[slotid][CollectorText]);
	//---
	dCollector[(load == -1) ? slotid : load][oCollector][0] = CreateDynamicObject(3134, x, y, z - 0.4619, 90.0, 0.0, angle);
	dCollector[(load == -1) ? slotid : load][oCollector][1] = CreateDynamicObject(18633, x, y, z + 0.2581, 90.0, 180.0, angle);
	SetDynamicObjectMaterial(dCollector[(load == -1) ? slotid : load][oCollector][0], 0, 17067, "cw2_logcabins", "cw2_logwall", 0xFFFFFFFF);
	//---
	dCollector[(load == -1) ? slotid : load][CollectorText] = CreateDynamic3DTextLabel("0.0 l", JAUNE, x, y, z, 3.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 5.0);
	GiveCollectorWater((load == -1) ? slotid : load, 0);
	//---
	dCollector[(load == -1) ? slotid : load][xEau] = x;
	dCollector[(load == -1) ? slotid : load][yEau] = y;
	dCollector[(load == -1) ? slotid : load][zEau] = z;
	dCollector[(load == -1) ? slotid : load][aEau] = angle;
	dCollector[(load == -1) ? slotid : load][dEau] = water;
	//---
    slotid ++;
    if(slotid == MAX_COLLECTORS) slotid = 0;
    return (slotid == 0) ? MAX_COLLECTORS : slotid - 1;
}

stock GiveCollectorWater(collectorid, water)
{
	new string[10];
    if(dCollector[collectorid][dEau] + water < 0) dCollector[collectorid][dEau] = 0;//Si la flotte est inférieure à 0, on la met à 0 l
	else if(dCollector[collectorid][dEau] + water > 100) dCollector[collectorid][dEau] = 100;//Si la flotte devient supérieure à 10 l, on la met à 10
    else dCollector[collectorid][dEau] += water;//Sinon, c'est bon
    format(string, sizeof(string), "%.1f l", floatdiv(dCollector[collectorid][dEau], 10));
    UpdateDynamic3DTextLabelText(dCollector[collectorid][CollectorText], BLEU, string);
	return dCollector[collectorid][dEau];
}

stock GetCollectorWater(collectorid)
{
	return dCollector[collectorid][dEau];
}

stock IsPlayerNearCollector(playerid)
{
	for(new i = 0; i < MAX_COLLECTORS; i ++) if(dCollector[i][dEau] != -1 && IsPlayerInRangeOfPoint(playerid, 2.0, dCollector[i][xEau], dCollector[i][yEau], dCollector[i][zEau])) return i;
	return -1;
}

stock DestroyCollector(collectid)
{
	DestroyDynamicObject(dCollector[collectid][oCollector][0]);
	DestroyDynamicObject(dCollector[collectid][oCollector][1]);
	DestroyDynamic3DTextLabel(dCollector[collectid][CollectorText]);
	dCollector[collectid][oCollector][0] = INVALID_OBJECT_ID;
	dCollector[collectid][oCollector][1] = INVALID_OBJECT_ID;
	dCollector[collectid][CollectorText] = Text3D:INVALID_3DTEXT_ID;
	dCollector[collectid][dEau] = 0;
	dCollector[collectid][xEau] = 0.0;
	dCollector[collectid][yEau] = 0.0;
	dCollector[collectid][zEau] = 0.0;
	dCollector[collectid][aEau] = 0.0;
}

public LoadCollectors_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_COLLECTORS; i ++)
	{
	    format(string, sizeof(string), "EauCollector%d", i);
		INI_Int(string, dCollector[i][dEau]);
	    format(string, sizeof(string), "xCollector%d", i);
		INI_Float(string, dCollector[i][xEau]);
	    format(string, sizeof(string), "yCollector%d", i);
		INI_Float(string, dCollector[i][yEau]);
	    format(string, sizeof(string), "zCollector%d", i);
		INI_Float(string, dCollector[i][zEau]);
	    format(string, sizeof(string), "aCollector%d", i);
		INI_Float(string, dCollector[i][aEau]);
	}
	return 1;
}

stock SaveCollectors()
{
	new string[50];
	new INI:File = INI_Open(CPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_COLLECTORS; i ++)
	{
	    format(string, sizeof(string), "EauCollector%d", i);
		INI_WriteInt(File,string, dCollector[i][dEau]);
	    format(string, sizeof(string), "xCollector%d", i);
		INI_WriteFloat(File,string, dCollector[i][xEau]);
	    format(string, sizeof(string), "yCollector%d", i);
		INI_WriteFloat(File,string, dCollector[i][yEau]);
	    format(string, sizeof(string), "zCollector%d", i);
		INI_WriteFloat(File,string, dCollector[i][zEau]);
	    format(string, sizeof(string), "aCollector%d", i);
		INI_WriteFloat(File,string, dCollector[i][aEau]);
	}
	INI_Close(File);
}

stock FindNearbyCollectors(playerid, Float:x, Float:y, Float:z, slot, tInArray[][][])
{
    //new len = sizeof(tInArray[]);
	new Float:fTrash;
    for(new i = 0; i < MAX_COLLECTORS; i ++)
    {
        if(slot < 9) break;
		if(dCollector[i][dEau] == -1) continue;
		if(dCollector[i][dEau] == -1) continue;
		if(IsPlayerInRangeOfPoint(playerid, 3.0, dCollector[i][xEau], dCollector[i][yEau], dCollector[i][zEau]))
        {
			if(CA_RayCastLine(x, y, z, dCollector[i][xEau], dCollector[i][yEau], dCollector[i][zEau], fTrash, fTrash, fTrash) != 0) continue;
            tInArray[playerid][slot][0] = i;
            tInArray[playerid][slot][1] = 0;
            slot ++;
        }
    }

    return slot;
}

stock SetupCollectors()
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_COLLECTORS; i ++) dCollector[i][dEau] = -1;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(CPATH, "LoadCollectors_data");
    #endif
	for(new i = 0; i < MAX_COLLECTORS; i ++)
	{
	    if(dCollector[i][xEau] == 0.0 && dCollector[i][yEau] == 0.0 && dCollector[i][zEau] == 0.0) dCollector[i][dEau] = -1;
		if(dCollector[i][dEau] != -1)
		{
			CreateCollector(dCollector[i][xEau], dCollector[i][yEau], dCollector[i][zEau], dCollector[i][aEau], dCollector[i][dEau], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_COLLECTORS; i ++) dCollector[i][dEau] = -1;
    LogInfo(true, "[INIT]Collecteurs d'eaux charges");
}

stock GetCollectorWater(collectorid)
{
	return dCollector[collectorid][dEau];
}

stock CreateShredder(Float:x, Float:y, Float:z, Float:angle, shredstate, load = -1)
{
	static slotid;
	if(load == -1)
	{
		if(dShredder[slotid][dBroyeur] != -1)
		{
		    for(new i = 0; i < MAX_SHREDDERS; i ++)
		    {
		        if(dShredder[i][dBroyeur] == -1)
		        {
		            slotid = i;
		            break;
		        }
		    }
		}
	}
	if(load == -1 && dShredder[slotid][dBroyeur] != -1) DestroyDynamicObject(dShredder[slotid][oBroyeur]);
	//---
	dShredder[(load == -1) ? slotid : load][oBroyeur] = CreateDynamicObject(920, x, y, z - 0.516, 0.0, 0.0, angle + 270.0);
	//---
	dShredder[(load == -1) ? slotid : load][xBroyeur] = x;
	dShredder[(load == -1) ? slotid : load][yBroyeur] = y;
	dShredder[(load == -1) ? slotid : load][zBroyeur] = z;
	dShredder[(load == -1) ? slotid : load][aBroyeur] = angle;
	dShredder[(load == -1) ? slotid : load][dBroyeur] = shredstate;
	//---
    slotid ++;
    if(slotid == MAX_SHREDDERS) slotid = 0;
    return (slotid == 0) ? MAX_SHREDDERS : slotid - 1;
}

stock IsPlayerNearShredder(playerid)
{
	for(new i = 0; i < MAX_SHREDDERS; i ++)
	{
	    if(dShredder[i][dBroyeur] == -1) continue;
		if(IsPlayerInRangeOfPoint(playerid, 2.0, dShredder[i][xBroyeur], dShredder[i][yBroyeur], dShredder[i][zBroyeur])) return i;
	}
	return -1;
}

stock DestroyShredder(shredderid)
{
	DestroyDynamicObject(dShredder[shredderid][oBroyeur]);
	dShredder[shredderid][oBroyeur] = INVALID_OBJECT_ID;
	dShredder[shredderid][dBroyeur] = -1;
	dShredder[shredderid][xBroyeur] = 0.0;
	dShredder[shredderid][xBroyeur] = 0.0;
	dShredder[shredderid][zBroyeur] = 0.0;
	dShredder[shredderid][aBroyeur] = 0.0;
}

public LoadShredders_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_SHREDDERS; i ++)
	{
	    format(string, sizeof(string), "StateShredder%d", i);
		INI_Int(string, dShredder[i][dBroyeur]);
	    format(string, sizeof(string), "xShredder%d", i);
		INI_Float(string, dShredder[i][xBroyeur]);
	    format(string, sizeof(string), "yShredder%d", i);
		INI_Float(string, dShredder[i][yBroyeur]);
	    format(string, sizeof(string), "zShredder%d", i);
		INI_Float(string, dShredder[i][zBroyeur]);
	    format(string, sizeof(string), "aShredder%d", i);
		INI_Float(string, dShredder[i][aBroyeur]);
	}
	return 1;
}

stock SaveShredders()
{
	new string[50];
	new INI:File = INI_Open(SHPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_SHREDDERS; i ++)
	{
	    format(string, sizeof(string), "StateShredder%d", i);
		INI_WriteInt(File,string, dShredder[i][dBroyeur]);
	    format(string, sizeof(string), "xShredder%d", i);
		INI_WriteFloat(File,string, dShredder[i][xBroyeur]);
	    format(string, sizeof(string), "yShredder%d", i);
		INI_WriteFloat(File,string, dShredder[i][yBroyeur]);
	    format(string, sizeof(string), "zShredder%d", i);
		INI_WriteFloat(File,string, dShredder[i][zBroyeur]);
	    format(string, sizeof(string), "aShredder%d", i);
		INI_WriteFloat(File,string, dShredder[i][aBroyeur]);
	}
	INI_Close(File);
}

stock FindNearbyShredders(playerid, Float:x, Float:y, Float:z, slot, tInArray[][][])
{
	new Float:fTrash;
	for(new i = 0; i < MAX_SHREDDERS; i ++)
	{
	    if(slot == 9) break;
		if(dShredder[i][dBroyeur] == -1) continue;
		if(IsPlayerInRangeOfPoint(playerid, 3.0, dShredder[i][xBroyeur], dShredder[i][yBroyeur], dShredder[i][zBroyeur]))
		{
			if(CA_RayCastLine(x, y, z, dShredder[i][xBroyeur], dShredder[i][yBroyeur], dShredder[i][zBroyeur], fTrash, fTrash, fTrash) != 0) continue;
		    tInArray[playerid][slot][0] = i;
		    tInArray[playerid][slot][1] = 7;
			slot ++;
		}
	}
}
	
stock SetupShredders()
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_SHREDDERS; i ++) dShredder[i][dBroyeur] = -1;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(SHPATH, "LoadShredders_data");
    #endif
	for(new i = 0; i < MAX_SHREDDERS; i ++)
	{
	    if(dShredder[i][xBroyeur] == 0.0 && dShredder[i][yBroyeur] == 0.0 && dShredder[i][zBroyeur] == 0.0) dShredder[i][dBroyeur] = -1;
		if(dShredder[i][dBroyeur] != -1)
		{
			CreateShredder(dShredder[i][xBroyeur], dShredder[i][yBroyeur], dShredder[i][zBroyeur], dShredder[i][aBroyeur], dShredder[i][dBroyeur], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_SHREDDERS; i ++) dShredder[i][dBroyeur] = -1;
    LogInfo(true, "[INIT]Broyeurs charges");
}

stock GetShredderState(shredderid)
{
	return IsValidShredder(shredderid) ? dShredder[shredderid][dBroyeur] : -1;
}

stock SetShredderState(shredderid, state)
{
	if(IsValidShredder(shredderid) && (1 >= state && state >= -1))
	{
		dShredder[shredderid][dBroyeur] = state;
	}
}

stock GetShredderPos(shredderid, &Float:x, &Float:y, &Float:z) 
{
	if(IsValidShredder(shredderid)) {
		x = dShredder[shredderid][xBroyeur];
		y = dShredder[shredderid][yBroyeur];
		z = dShredder[shredderid][zBroyeur];
	}
}

stock GetShredderAngle(shredderid, &Float:angle) 
{
	if(IsValidShredder(shredderid)) {
		angle = dShredder[shredderid][aBroyeur];
	}
}

stock IsValidShredder(shredderid) 
{
	return MAX_SHREDDERS > shredderid >= 0;
}

stock GetShredderObject(shredderid) 
{
	return IsValidShredder(shredderid) ? dShredder[shredderid][oBroyeur] : INVALID_OBJECT_ID;
}

//---PLANTS---//
stock InitliazePlantVariables()
{
	for(new i = 0; i < MAX_PLANTS; i ++)
	{
		dPlant[i][dPlantID] = 0;
		dPlant[i][dResistance] = 0;
		dPlant[i][dFruits] = 0;
		dPlant[i][oPlantObject] = INVALID_OBJECT_ID;
		dPlant[i][dGrowTime] = 0;
		dPlant[i][xPlant] = 0.0;
		dPlant[i][yPlant] = 0.0;
		dPlant[i][zPlant] = 0.0;
		dPlant[i][aPlant] = 0.0;
	}
}

CreatePlant(plant, fruits, growtime, Float:x, Float:y, Float:z, Float:angle, load = -1)
{
	static slotid;
	if(load == -1 && dPlant[slotid][dPlantID] != 0)
	{
		for(new i = 0; i < MAX_PLANTS; i ++)
		{
			if(dPlant[i][dPlantID] == 0)
		    {
		    	slotid = i;
		        break;
		    }
		}
	}
	if(load == -1 && dPlant[slotid][dPlantID] != 0) DestroyDynamicObject(dPlant[slotid][oPlantObject]);
	//---
	new dModelID;
	switch(plant)
	{
	    case 1, 4: dModelID = 19837;
	    case 2: dModelID = 19838;
	    case 3, 5: dModelID = 19839;
	}
	dPlant[(load == -1) ? slotid : load][oPlantObject] = CreateDynamicObject(dModelID, x, y, z - 1.128, 0.0, 0.0, angle, -1, -1, -1, 25.0, 20.0);
	//---
	dPlant[(load == -1) ? slotid : load][xPlant] = x;
	dPlant[(load == -1) ? slotid : load][yPlant] = y;
	dPlant[(load == -1) ? slotid : load][zPlant] = z;
	dPlant[(load == -1) ? slotid : load][aPlant] = angle;
	dPlant[(load == -1) ? slotid : load][dGrowTime] = (growtime > 0) ? growtime : 0;
	dPlant[(load == -1) ? slotid : load][dPlantID] = plant;
	dPlant[(load == -1) ? slotid : load][dFruits] = fruits;
	switch(plant)
	{
	    case 1: dPlant[(load == -1) ? slotid : load][dResistance] = 1;
	    case 2: dPlant[(load == -1) ? slotid : load][dResistance] = 2;
	    case 3: dPlant[(load == -1) ? slotid : load][dResistance] = 5;
	    case 4: dPlant[(load == -1) ? slotid : load][dResistance] = 2;
	    case 5: dPlant[(load == -1) ? slotid : load][dResistance] = 10;
	}
	//---
	if(dPlant[(load == -1) ? slotid : load][dGrowTime] == 0) GrowPlant((load == -1) ? slotid : load);
	//---
    slotid ++;
    if(slotid == MAX_PLANTS) slotid = 0;
    return (slotid == 0) ? MAX_PLANTS : slotid - 1;
}

stock IsValidPlant(plantid)
{
	return MAX_PLANTS > plantid >= 0;
}

stock GetPlantGrowTime(plantid)
{
	return IsValidPlant(plantid) ? dPlant[plantid][dGrowTime] : 0;
}

stock SetPlantGrowTime(plantid, growtime)
{
	if(IsValidPlant(plantid))
	{
		dPlant[plantid][dGrowTime] = growtime < 0 ? 0 : growtime;
		if(dPlant[plantid][dGrowTime] == 0)
		{
			GrowPlant(i);
		}
	}
}

DestroyPlant(plantid)
{
	DestroyDynamicObject(dPlant[plantid][oPlantObject]);
	dPlant[plantid][oPlantObject] = INVALID_OBJECT_ID;
	dPlant[plantid][dGrowTime] = 0;
	dPlant[plantid][dFruits] = 0;
	dPlant[plantid][dPlantID] = 0;
	dPlant[plantid][xPlant] = 0.0;
	dPlant[plantid][yPlant] = 0.0;
	dPlant[plantid][zPlant] = 0.0;
	dPlant[plantid][aPlant] = 0.0;
}

GrowPlant(plantid)
{
    //							Blé  	Oranger     Pommier     Tomatier    Sapin
	new dPlantModels[] = 		{862, 	904,		673,		2010,		687};
	new Float:fPlantZOffset[] = {2.0,	1.0097,		3.7,		1.297,		2.0};
	switch(dPlant[plantid][dPlantID])
	{
	    case 1: dPlant[plantid][dResistance] = 1;
	    case 2: dPlant[plantid][dResistance] = 2;
	    case 3: dPlant[plantid][dResistance] = 5;
	    case 4: dPlant[plantid][dResistance] = 2;
	    case 5: dPlant[plantid][dResistance] = 10;
	}
	DestroyDynamicObject(dPlant[plantid][oPlantObject]);
	dPlant[plantid][oPlantObject] = CreateDynamicObject(dPlantModels[dPlant[plantid][dPlantID] - 1], dPlant[plantid][xPlant], dPlant[plantid][yPlant], dPlant[plantid][zPlant] - fPlantZOffset[dPlant[plantid][dPlantID] - 1], 0.0, 0.0, dPlant[plantid][aPlant], -1, -1, -1, 25.0, 20.0);
}

public LoadPlants_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_PLANTS; i ++)
	{
	    format(string, sizeof(string), "Plante%d", i);
		INI_Int(string, dPlant[i][dPlantID]);
	    format(string, sizeof(string), "Fruits%d", i);
		INI_Int(string, dPlant[i][dFruits]);
	    format(string, sizeof(string), "GrowTime%d", i);
		INI_Int(string, dPlant[i][dGrowTime]);
	    format(string, sizeof(string), "xPlant%d", i);
		INI_Float(string, dPlant[i][xPlant]);
	    format(string, sizeof(string), "yPlant%d", i);
		INI_Float(string, dPlant[i][yPlant]);
	    format(string, sizeof(string), "zPlant%d", i);
		INI_Float(string, dPlant[i][zPlant]);
	    format(string, sizeof(string), "aPlant%d", i);
		INI_Float(string, dPlant[i][aPlant]);
	}
	return 1;
}

SavePlants()
{
	new string[50];
	new INI:File = INI_Open(PPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_PLANTS; i ++)
	{
	    format(string, sizeof(string), "Plante%d", i);
		INI_WriteInt(File,string, dPlant[i][dPlantID]);
	    format(string, sizeof(string), "Fruits%d", i);
		INI_WriteInt(File,string, dPlant[i][dFruits]);
	    format(string, sizeof(string), "GrowTime%d", i);
		INI_WriteInt(File,string, dPlant[i][dGrowTime]);
	    format(string, sizeof(string), "xPlant%d", i);
		INI_WriteFloat(File,string, dPlant[i][xPlant]);
	    format(string, sizeof(string), "yPlant%d", i);
		INI_WriteFloat(File,string, dPlant[i][yPlant]);
	    format(string, sizeof(string), "zPlant%d", i);
		INI_WriteFloat(File,string, dPlant[i][zPlant]);
	    format(string, sizeof(string), "aPlant%d", i);
		INI_WriteFloat(File,string, dPlant[i][aPlant]);
	}
	INI_Close(File);
}

stock SetupPlants()
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_PLANTS; i ++) dPlant[i][oPlantObject] = INVALID_OBJECT_ID;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(PPATH, "LoadPlants_data");
    #endif
	for(new i = 0; i < MAX_PLANTS; i ++)
	{
	    if(dPlant[i][xPlant] == 0.0 && dPlant[i][yPlant] == 0.0 && dPlant[i][zPlant] == 0.0) dPlant[i][dPlantID] = 0;
		if(dPlant[i][dPlantID] != 0)
		{
			CreatePlant(dPlant[i][dPlantID], dPlant[i][dFruits], dPlant[i][dGrowTime], dPlant[i][xPlant], dPlant[i][yPlant], dPlant[i][zPlant], dPlant[i][aPlant], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_PLANTS; i ++) dPlant[i][dPlantID] = 0;
    LogInfo(true, "[INIT]Plantes chargees");
}


stock PlayerCutPlants(playerid)
{
	new Float:x, Float:y, Float:z;

	for(new i = 0; i < MAX_PLANTS; i ++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 1.5, dPlant[i][xPlant], dPlant[i][yPlant], dPlant[i][zPlant]) && dPlant[i][dResistance] != 0 && dPlant[i][dGrowTime] == 0)
		{
			if((GetPlayerWeapon(playerid) == 4 && dPlant[i][dResistance] == 1) || (GetPlayerWeapon(playerid) == 8 && dPlant[i][dResistance] <= 2) || GetPlayerWeapon(playerid) == 9)
			{
				dPlant[i][dResistance] --;
				new dLogs = 0;
				if(dPlant[i][dResistance] == 0)
				{
					switch(dPlant[i][dPlantID])
					{
						case 1: dLogs = 0;
						case 2: dLogs = 2;
						case 3: dLogs = 3;
						case 4: dLogs = 1;
						case 5: dLogs = 6;
					}
					if(dPlant[i][dPlantID] == 1)//BLÉ
					{
						if(RandomEx(0, 10) > 3)//On crée éventuellement des graines
						{
							x = dPlant[i][xPlant] + floatdiv(RandomEx(-15, 15), 10);
							y = dPlant[i][yPlant] + floatdiv(RandomEx(-15, 15), 10);
							z = dPlant[i][zPlant];
							//---
							new Float:x2, Float:y2, Float:z2;
							CA_RayCastLine(dPlant[i][xPlant], dPlant[i][yPlant], dPlant[i][zPlant], x, y, z - 2.0, x2, y2, z2);
							CreateItem(115, x2, y2, z2 + 1.0, false, -1);
						}
						//---
						if(RandomEx(0, 10) > 6) CreateItem(116, dPlant[i][xPlant], dPlant[i][yPlant], dPlant[i][zPlant] + 1.0, false, -1);//Et le ballot de blé
					}
					else if(dPlant[i][dPlantID] == 5)//SAPIN
					{
						if(RandomEx(0, 10) > 2)//On crée éventuellement des graines
						{
							x = dPlant[i][xPlant] + floatdiv(RandomEx(-15, 15), 10);
							y = dPlant[i][yPlant] + floatdiv(RandomEx(-15, 15), 10);
							z = dPlant[i][zPlant];
							//---
							new Float:x2, Float:y2, Float:z2;
							CA_RayCastLine(dPlant[i][xPlant], dPlant[i][yPlant], dPlant[i][zPlant], x, y, z - 2.0, x2, y2, z2);
							CreateItem(148, x2, y2, z2 + 1.0, false, -1);
							if(RandomEx(0, 10) > 6)//On crée éventuellement des graines
							{
								x = dPlant[i][xPlant] + floatdiv(RandomEx(-15, 15), 10);
								y = dPlant[i][yPlant] + floatdiv(RandomEx(-15, 15), 10);
								z = dPlant[i][zPlant];
								//---
								CA_RayCastLine(dPlant[i][xPlant], dPlant[i][yPlant], dPlant[i][zPlant], x, y, z - 2.0, x2, y2, z2);
								CreateItem(148, x2, y2, z2 + 1.0, false, -1);
							}
						}
					}
					if(dLogs != 0)
					{
						//---
						for(new j = 0; j < dLogs; j ++)
						{
							x = dPlant[i][xPlant] + floatdiv(RandomEx(-25, 25), 10);
							y = dPlant[i][yPlant] + floatdiv(RandomEx(-25, 25), 10);
							z = dPlant[i][zPlant];
							//---
							new Float:x2, Float:y2, Float:z2;
							CA_RayCastLine(dPlant[i][xPlant], dPlant[i][yPlant], dPlant[i][zPlant], x, y, z - 2.0, x2, y2, z2);
							CreateItem(71, x2, y2, z2 + 1.0, false, -1);
						}
						SetTimerEx("DestroyObjectEx", 15000, false, "ib", CreateDynamicObject(834, dPlant[i][xPlant], dPlant[i][yPlant], dPlant[i][zPlant] - 2.8669, 0.0, 0.0, dPlant[i][aPlant]), true);
					}
					DestroyPlant(i);
				}
				else
				{
					new string[45];
					strcat(string, "~n~~n~~n~~n~~n~~n~~n~~n~~n~");
					strcat(string, "~r~");
					for(new j = 0; j < dPlant[i][dResistance]; j ++) strcat(string, "|");
					GameTextForPlayer(playerid, string, 3000, 3);
				}
				return 1;
			}
		}
	}
}

stock PlantsGrow()
{
	for(new i = 0; i < MAX_PLANTS; i++)
	{
		if(dPlant[i][dPlantID] != 0)
		{
			if(dPlant[i][dGrowTime] != 0)
			{
				dPlant[i][dGrowTime] -= (IsRaining()) ? 2 : 1;
				if(dPlant[i][dGrowTime] < 0) dPlant[i][dGrowTime] = 0;
				if(dPlant[i][dGrowTime] == 0) GrowPlant(i);
			}
			else if(dPlant[i][dGrowTime] == 0 && IsMultiple(dMinutes, 3))
			{
				if(dPlant[i][dFruits] > 0)
				{
					dPlant[i][dFruits] --;
					new Float:x, Float:y, Float:z;
					x = dPlant[i][xPlant] + floatdiv(RandomEx(5, 25), 10);
					y = dPlant[i][yPlant] + floatdiv(RandomEx(5, 25), 10);
					z = dPlant[i][zPlant];
					//---
					new Float:x2, Float:y2, Float:z2;
					CA_RayCastLine(dPlant[i][xPlant], dPlant[i][yPlant], dPlant[i][zPlant], x, y, z - 2.0, x2, y2, z2);
					//---
					switch(dPlant[i][dPlantID])
					{
						case 2: CreateItem(72, x2, y2, z2 + 1.0, false, -1);
						case 3: CreateItem(73, x2, y2, z2 + 1.0, false, -1);
						case 4: CreateItem(74, x2, y2, z2 + 1.0, false, -1);
					}
				}
			}
		}
	}
}

stock CanPlayerPlantHere(playerid)
{
	#pragma unused playerid
	/*new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	switch(GetMapGroundType(floatround(x, floatround_round), floatround(y, floatround_round)))
	{
	    case 2, 3, 4, 5: return true;
	    case 0, 1, 6: return false;
	}
	return false;*/
	return true;
}

stock PlayerWatersPlants(playerid)
{
	for(new i = 0; i < MAX_PLANTS; i++)
	{
		if(dPlant[i][dPlantID] != 0)
		{
			if(IsPlayerInRangeOfPoint(playerid, 2.0, dPlant[i][xPlant], dPlant[i][yPlant], dPlant[i][zPlant]))
			{
				new dGrowTime = GetPlantGrowTime(i);
				if((IsNight() && dGrowTime > 1) || (!IsNight() && dGrowTime > 0))
				{
					dGrowTime -= 2;
					SetPlantGrowTime(i, dGrowTime);
					GivePlayerSlotObject(playerid, 40, slot);
					ApplyAnimation(playerid, "GRENADE", "WEAPON_throwu", 4.0, 0, 0, 0, 0, 0, 1);
					return true;
				}
			}
		}
	}
	return false;
}

//---PANNEAUX---//
stock InitializeBoardVariables() 
{
	for(new i = 0; i < MAX_BOARDS; i ++)
	{
		dBoard[i][bBoard] = false;
		dBoard[i][oBoard] = INVALID_OBJECT_ID;
		dBoard[i][xBoard] = 0.0;
		dBoard[i][yBoard] = 0.0;
		dBoard[i][zBoard] = 0.0;
		dBoard[i][aBoard] = 0.0;
	}
}

stock CreateBoard(Float:x, Float:y, Float:z, Float:angle, load = -1)
{
	static slotid;
	if(load == -1)
	{
		if(dBoard[slotid][bBoard])
		{
			for(new i = 0; i < MAX_BOARDS; i ++)
		 	{
				if(!dBoard[i][bBoard])
			    {
			    	slotid = i;
			        break;
			    }
			}
			if(dBoard[slotid][bBoard])
			{
				DestroyDynamicObject(dBoard[slotid][oBoard][0]);
				DestroyDynamicObject(dBoard[slotid][oBoard][1]);
			}
		}
	}
	//---
	dBoard[(load == -1) ? slotid : load][bBoard] = true;
	dBoard[(load == -1) ? slotid : load][xBoard] = x;
	dBoard[(load == -1) ? slotid : load][yBoard] = y;
	dBoard[(load == -1) ? slotid : load][zBoard] = z;
	dBoard[(load == -1) ? slotid : load][aBoard] = angle;
    /*-118.68917063
    -0.4128
    -0.2259*/
	dBoard[(load == -1) ? slotid : load][oBoard][0] = CreateDynamicObject(3927, x, y, z, 0.0, 0.0, angle, -1, -1, -1, 50.0, 45.0);
    angle -= 77.38;
	dBoard[(load == -1) ? slotid : load][oBoard][1] = CreateDynamicObject(19805, x - (0.13 * floatsin(-angle, degrees)), y - (0.13 * floatcos(-angle, degrees)), z + 0.7601, 0.0, 0.0, angle + 77.39, -1, -1, -1, 25.0, 20.0);
    SetDynamicObjectMaterialText(dBoard[(load == -1) ? slotid : load][oBoard][1], 0, " ", OBJECT_MATERIAL_SIZE_256x128, "Century Gothic", 48, 0, 0xCC0000FF, 0x00000000, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
	//---
    slotid ++;
    if(slotid == MAX_BOARDS) slotid = 0;
    return (slotid == 0) ? MAX_BOARDS : slotid - 1;
}

stock SetBoardText(boardid, const text[])
{
	if(!(MAX_BOARDS > boardid >= 0)) return false;
	if(!dBoard[boardid][bBoard]) return false;
	if(strlen(text) < 1) return false;
	//---
    new string[135];
	strcpy(string, text);
	new nl, ok;
	do
	{
		nl ++;
		if(strlen(string) > nl*20) strins(string, "\n", nl*20);
		else ok = true;
	}
	while(!ok);
	while(string[strlen(string)] == ' ')
    {
        string[strlen(string)] = '\0';
    }
    //---
	format(dBoard[boardid][sBoardText], 128, "%s", text);
    SetDynamicObjectMaterialText(dBoard[boardid][oBoard][1], 0, string, OBJECT_MATERIAL_SIZE_256x128, "Olde English", 30, 1, 0xFFCC0000, 0x00000000, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
	return true;
}

stock DestroyBoard(boardid)
{
	DestroyDynamicObject(dBoard[boardid][oBoard][0]);
	DestroyDynamicObject(dBoard[boardid][oBoard][1]);
	dBoard[boardid][oBoard][0] = INVALID_OBJECT_ID;
	dBoard[boardid][oBoard][1] = INVALID_OBJECT_ID;
	//---
	dBoard[boardid][xBoard] = 0.0;
	dBoard[boardid][yBoard] = 0.0;
	dBoard[boardid][zBoard] = 0.0;
	dBoard[boardid][aBoard] = 0.0;
}

public LoadBoards_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_BOARDS; i ++)
	{
	    format(string, sizeof(string), "Panneau%d", i);
		INI_Bool(string, dBoard[i][bBoard]);
	    format(string, sizeof(string), "Texte%d", i);
		INI_String(string, dBoard[i][sBoardText], 128);
	    format(string, sizeof(string), "xSafe%d", i);
		INI_Float(string, dBoard[i][xBoard]);
	    format(string, sizeof(string), "ySafe%d", i);
		INI_Float(string, dBoard[i][yBoard]);
	    format(string, sizeof(string), "zSafe%d", i);
		INI_Float(string, dBoard[i][zBoard]);
	    format(string, sizeof(string), "aSafe%d", i);
		INI_Float(string, dBoard[i][aBoard]);
	}
	return 1;
}

stock IsValidBoard(boardid)
{
	return MAX_BOARDS > boardid >= 0;
}

stock GetBoardObject(boardid)
{
	return IsValidBoard(boardid) ? dBoard[boardid][oBoard][0] : INVALID_OBJECT_ID;
}

stock SaveBoards()
{
	new string[50];
	new INI:File = INI_Open(BBPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_BOARDS; i ++)
	{
	    format(string, sizeof(string), "Panneau%d", i);
		INI_WriteBool(File,string, dBoard[i][bBoard]);
	    format(string, sizeof(string), "Texte%d", i);
		INI_WriteString(File,string, dBoard[i][sBoardText]);
	    format(string, sizeof(string), "xSafe%d", i);
		INI_WriteFloat(File,string, dBoard[i][xBoard]);
	    format(string, sizeof(string), "ySafe%d", i);
		INI_WriteFloat(File,string, dBoard[i][yBoard]);
	    format(string, sizeof(string), "zSafe%d", i);
		INI_WriteFloat(File,string, dBoard[i][zBoard]);
	    format(string, sizeof(string), "aSafe%d", i);
		INI_WriteFloat(File,string, dBoard[i][aBoard]);
	}
	INI_Close(File);
}

stock SetupBoards()
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_BOARDS; i ++) dBoard[i][bBoard] = false;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(BBPATH, "LoadBoards_data");
    #endif
	for(new i = 0; i < MAX_BOARDS; i ++)
	{
	    if(dBoard[i][xBoard] == 0.0 && dBoard[i][yBoard] == 0.0 && dBoard[i][zBoard] == 0.0) dBoard[i][bBoard] = false;
		if(dBoard[i][bBoard])
		{
			CreateBoard(dBoard[i][xBoard], dBoard[i][yBoard], dBoard[i][zBoard], dBoard[i][aBoard], i);
			SetBoardText(i, dBoard[i][sBoardText]);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_BOARDS; i ++) dBoard[i][bBoard] = false;
    LogInfo(true, "[INIT]Panneaux charges");
}

stock PlayerCutBoards(playerid) 
{
	new Float:x, Float:y, Float:z;
	for(new i = 0; i < MAX_BOARDS; i ++)
	{
		if(!dBoard[i][bBoard]) continue;
		static dBoardResistance[MAX_BOARDS] = {5, ...};
		if(IsPlayerInRangeOfPoint(playerid, 1.5, dBoard[i][xBoard], dBoard[i][yBoard], dBoard[i][zBoard]) && dBoardResistance[i] != 0)
		{
			dBoardResistance[i] --;
			if(dBoardResistance[i] <= 0)
			{
				for(new j = 0; j < 2; j ++)
				{
					x = dBoard[i][xBoard] + floatdiv(RandomEx(-25, 25), 10);
					y = dBoard[i][yBoard] + floatdiv(RandomEx(-25, 25), 10);
					z = dBoard[i][zBoard];
					//---
					new Float:x2, Float:y2, Float:z2;
					CA_RayCastLine(dBoard[i][xBoard], dBoard[i][yBoard], dBoard[i][zBoard], x, y, z - 2.0, x2, y2, z2);
					CreateItem(71, x2, y2, z2 + 1.0, false, -1);
				}
				dBoardResistance[i] = 5;
				DestroyBoard(i);
			}
			else
			{
				new string[45];
				strcat(string, "~n~~n~~n~~n~~n~~n~~n~~n~~n~");
				strcat(string, "~r~");
				for(new j = 0; j < dBoardResistance[i]; j ++) strcat(string, "|");
				GameTextForPlayer(playerid, string, 3000, 3);
			}
			return 1;
		}
	}
}

stock InitiializeFireVariables()
{
	for(new i = 0; i < MAX_FIRES; i ++)
	{
		dFire[i][oFeu] = INVALID_OBJECT_ID;
		dFire[i][dTempsFeu] = 0;
		dFire[i][xFeu] = 0.0;
		dFire[i][yFeu] = 0.0;
		dFire[i][zFeu] = 0.0;
		dFire[i][aFeu] = 0.0;
	}
}

CreateFire(Float:x, Float:y, Float:z, Float:angle, time, load = -1)
{
	static slotid;
	if(load == -1 && dFire[slotid][dTempsFeu] != 0) DestroyDynamicObject(dFire[slotid][oFeu]);
	//---
	dFire[(load == -1) ? slotid : load][oFeu] = CreateDynamicObject(19632, x, y, z - 1.023, 0.0, 0.0, angle);
	//---
	dFire[(load == -1) ? slotid : load][xFeu] = x;
	dFire[(load == -1) ? slotid : load][yFeu] = y;
	dFire[(load == -1) ? slotid : load][zFeu] = z;
	dFire[(load == -1) ? slotid : load][aFeu] = angle;
	dFire[(load == -1) ? slotid : load][dTempsFeu] = time;
	//---
    slotid ++;
    if(slotid == MAX_FIRES) slotid = 0;
    return (slotid == 0) ? MAX_FIRES : slotid - 1;
}

DestroyFire(fireid)
{
	DestroyDynamicObject(dFire[fireid][oFeu]);
	dFire[fireid][oFeu] = INVALID_OBJECT_ID;
	dFire[fireid][dTempsFeu] = 0;
	dFire[fireid][xFeu] = 0.0;
	dFire[fireid][yFeu] = 0.0;
	dFire[fireid][zFeu] = 0.0;
	dFire[fireid][aFeu] = 0.0;
}

IsPlayerNextToFire(playerid)
{
	for(new i = 0; i < MAX_FIRES; i ++) if(IsPlayerInRangeOfPoint(playerid, 3.5, dFire[i][xFeu], dFire[i][yFeu], dFire[i][zFeu]) && dFire[i][dTempsFeu] != 0) return true;
	if(IsPlayerNearBrasero(playerid)) return true;
	if(IsPlayerInRangeOfPoint(playerid, 3.5, -1952.3737, -2452.9717, 30.6250)) return true;//Camp Angel Pine
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, 569.598, -203.8609, 26.694)) return true;//Camp clodos 1
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, 1027.3179, -54.861, 25.8339)) return true;//Camp clodos 2
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, -1995.0689, -1548.1629, 84.7959)) return true;//Camp Mont Chilliad
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, -568.721, -1062.363, 23.7)) return true;//Camping Flint County
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, -1301.0479, 2514.9189, 87.593)) return true;//Camp Aldea Malvada
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, 1087.2144, 1273.0037, 10.8203)) return true;//Moto École
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, 1043.5376, 1269.6462, 20.3791)) return true;//Moto École toit
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, 960.3007, 2448.7773, 10.8203)) return true;//LV Nord
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, 1086.9797, 1272.4633, 10.8203)) return true;//LV Nord
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, -2669.231, -2411.172, 2.562)) return true;//Reggae
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, -16.7251, -294.0468, 6.5064)) return true;//Blueberry
	return false;
}

public LoadFires_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_FIRES; i ++)
	{
	    format(string, sizeof(string), "FireTime%d", i);
		INI_Int(string, dFire[i][dTempsFeu]);
	    format(string, sizeof(string), "xFeu%d", i);
		INI_Float(string, dFire[i][xFeu]);
	    format(string, sizeof(string), "yFeu%d", i);
		INI_Float(string, dFire[i][yFeu]);
	    format(string, sizeof(string), "zFeu%d", i);
		INI_Float(string, dFire[i][zFeu]);
	    format(string, sizeof(string), "aFeu%d", i);
		INI_Float(string, dFire[i][aFeu]);
	}
	return 1;
}

SaveFires()
{
	new string[50];
	new INI:File = INI_Open(FPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_FIRES; i ++)
	{
	    format(string, sizeof(string), "FireTime%d", i);
		INI_WriteInt(File,string, dFire[i][dTempsFeu]);
	    format(string, sizeof(string), "xFeu%d", i);
		INI_WriteFloat(File,string, dFire[i][xFeu]);
	    format(string, sizeof(string), "yFeu%d", i);
		INI_WriteFloat(File,string, dFire[i][yFeu]);
	    format(string, sizeof(string), "zFeu%d", i);
		INI_WriteFloat(File,string, dFire[i][zFeu]);
	    format(string, sizeof(string), "aFeu%d", i);
		INI_WriteFloat(File,string, dFire[i][aFeu]);
	}
	INI_Close(File);
}

stock SetupFires()
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_FIRES; i ++) dFire[i][oFeu] = INVALID_OBJECT_ID;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(FPATH, "LoadFires_data");
    #endif
	for(new i = 0; i < MAX_FIRES; i ++)
	{
		if(dFire[i][dTempsFeu] != 0)
		{
			CreateFire(dFire[i][xFeu], dFire[i][yFeu], dFire[i][zFeu], dFire[i][aFeu], dFire[i][dTempsFeu], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_FIRES; i ++) dFire[i][dTempsFeu] = 0;
    LogInfo(true, "[INIT]Feux charges");
}

stock IsValidFire(fireid)
{
	return MAX_FIRES > fireid >= 0;
}

stock GetFireTime(fireid)
{
	return IsValidFire(fireid) ? dFire[fireid][dTempsFeu] : -1;
}

stock SetFireTime(fireid, timeid)
{
	if(IsValidFire(fireid))
	{
		dFire[fireid][dTempsFeu] = timeid;
	}
}