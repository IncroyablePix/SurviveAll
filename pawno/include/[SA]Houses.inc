#if defined HOUSES_INCLUDED

	#endinput

#endif

#define HOUSES_INCLUDED

//---INCLUDES---//
#include <a_samp>
#include <[SA]Defines.inc>

//---DEFINES---//
#define MAX_HOUSES                          (250)
#define MAX_TENTS                           (100)
#define MAX_BEDS                            (200)
#define BUD_BED                             (MAX_BEDS + 1)
#define MAX_GARAGES                         (150)
#define MAX_SEATS                           (100)
#define MAX_FURN                            (300)
#define MAX_TANKS                           (50)
#define MAX_TANK_FUEL      					(15000)

#define HPATH                               "/Survive-All/General/Houses.ini"
#define TPATH                               "/Survive-All/General/Tents.ini"
#define BPATH                               "/Survive-All/General/Beds.ini"
#define SEPATH                              "/Survive-All/General/Seats.ini"
#define FUPATH                              "/Survive-All/General/Furniture.ini"
#define GARPATH                             "/Survive-All/General/Garages.ini"
#define TKPATH                              "/Survive-All/General/Tanks.ini"

//---FORWARDS---//
forward LoadHouses_data(name[], value[]);
forward LoadTents_data(name[], value[]);
forward LoadBeds_data(name[], value[]);
forward LoadGarages_data(name[], value[]);
forward LoadTanks_data(name[], value[]);
forward LoadSeats_data(name[], value[]);
forward LoadFurniture_data(name[], value[]);

forward bool:IsHouseIDValid(houseid);
forward bool:IsHouseValid(houseid);

//---ENUMS---//
enum House
{
	dHouseType,
	oHouse[3],
	bool:bPorte[2],
	oPorte,
	sCodePorte[5],
	Float:xHouse,
	Float:yHouse,
	Float:zHouse,
	Float:aHouse
}

enum Tent
{
	bool:bTent,
	oTent[6],
	Float:xTent,
	Float:yTent,
	Float:zTent,
	Float:aTent
}

enum Bed
{
	dBedType,
	oBed,
	Float:xBed,
	Float:yBed,
	Float:zBed,
	Float:aBed
}

enum Garage
{
	oGarage[2],
	bool:bGarage[2],
	sCodeGarage[5],
	Float:xGarage,
	Float:yGarage,
	Float:zGarage,
	Float:aGarage
}

enum Tank
{
	oTank[2],
	Text3D:tTankFuel,
	dTankGas,
	Float:xTank,
	Float:yTank,
	Float:zTank,
	Float:aTank
}

enum Seat
{
	dSeatType,
	oSeat,
	Float:xSeat,
	Float:ySeat,
	Float:zSeat,
	Float:aSeat
}
enum Furniture
{
	dFurnitureID,
	oFurniture,
	Float:xFurn,
	Float:yFurn,
	Float:zFurn,
	Float:rxFurn,
	Float:ryFurn,
	Float:rzFurn
}

//---VARIABLES---//
static dHouseBuild[MAX_PLAYERS];
static dHouseID[MAX_PLAYERS];

static dHouse[MAX_HOUSES][House];
static dTent[MAX_TENTS][Tent];
static dBed[MAX_BEDS][Bed];
static dGarage[MAX_GARAGES][Garage];
static dTanks[MAX_TANKS][Tank];
static dSeat[MAX_SEATS][Seat];
static dFurn[MAX_FURN][Furniture];

static pCreateSafe[MAX_PLAYERS] = {-1, ...};
static dDoor[MAX_PLAYERS] = {-1, ...};
static pBed[MAX_PLAYERS] = {-1, ...};
static pSeat[MAX_PLAYERS] = {-1, ...};
static pBoard[MAX_PLAYERS] = {-1, ...};
static pRack[MAX_PLAYERS] = {-1, ...};
static pBrasero[MAX_PLAYERS] = {-1, ...};
static pShredder[MAX_PLAYERS] = {-1, ...};
static pPlayerSafe[MAX_PLAYERS] = {-1, ...};
static pGarage[MAX_PLAYERS] = {-1, ...};
static pTank[MAX_PLAYERS] = {-1, ...};
static pFurn[MAX_PLAYERS] = {-1, ...};

//---FONCTIONS---//
/**
	<summary>
        Initialise les variables des entités maison.
    </summary>

    <param>
        /
    </param>

    <returns>
        void
    </returns>

    <remarks>
        Indispensable pour pas avoir des destructions d'objets au hasard !
    </remarks>
*/
stock InitializeHouseVariables() {
	for(new i = 0; i < MAX_HOUSES; i ++)
	{
		dHouse[i][dHouseType] = 0;
		dHouse[i][oHouse][0] = INVALID_OBJECT_ID;
		dHouse[i][oHouse][1] = INVALID_OBJECT_ID;
		dHouse[i][oHouse][2] = INVALID_OBJECT_ID;
		dHouse[i][oPorte] = INVALID_OBJECT_ID;
		dHouse[i][bPorte][0] = false;
		dHouse[i][bPorte][1] = false;
		dHouse[i][xHouse] = 0.0;
		dHouse[i][yHouse] = 0.0;
		dHouse[i][zHouse] = 0.0;
		dHouse[i][aHouse] = 0.0;
	}
}

/**
	<summary>
        Crée une maison
    </summary>

    <param name="type">
		int - Le type de maison
    </param>
    
    <param name="door">
		bool - Si on met une porte dessus ou pas
    </param>

    <param name="x">
		Float - Position x
    </param>

    <param name="y">
		Float - Position y
    </param>

    <param name="z">
		Float - Position z
    </param>

    <param name="angle">
		Float - Angle de la maison
    </param>
    
    <param name="load">
		int - L'ID chargé de la maison à partir du fichier;
		La maison sera créée dans le tableau de l'index fourni dans load.
		Si une maison existe déjà cet index, elle sera détruite.
    </param>

    <returns>
		L'ID de la maison créée
    </returns>
*/
stock CreateHouse(type, bool:door, Float:x, Float:y, Float:z, Float:angle, load = -1)
{
	static slotid;
	if(load == -1)
	{
		if(dHouse[slotid][dHouseType] != 0)
		{
		    for(new i = 0; i < MAX_HOUSES; i ++)
		    {
		        if(dHouse[i][dHouseType] == 0)
		        {
		            slotid = i;
		            break;
		        }
		    }
		}
		CA_DestroyObject_DC(dHouse[slotid][oHouse][0]), dHouse[slotid][oHouse][0] = INVALID_OBJECT_ID;
		//---
		if(dHouse[slotid][dHouseType] == 1)
		{
			CA_DestroyObject_DC(dHouse[slotid][oHouse][1]);
			dHouse[slotid][oHouse][1] = INVALID_OBJECT_ID;
			CA_DestroyObject_DC(dHouse[slotid][oHouse][2]);
			dHouse[slotid][oHouse][2] = INVALID_OBJECT_ID;
		}
		if(dHouse[slotid][dHouseType] == 2)
		{
			CA_DestroyObject_DC(dHouse[slotid][oHouse][1]);
			dHouse[slotid][oHouse][1] = INVALID_OBJECT_ID;
		}
		//---
		if(door) CA_DestroyObject_DC(dHouse[slotid][oPorte]), dHouse[slotid][oPorte] = INVALID_OBJECT_ID;
	}
	//---TYPES
	/*
	HouseID = 3414
	Joueur: 2502.664, 2092.843, 31.3729
	Maison: 2502.664, 2092.843, 32.5719
	Porte: 2504.708, 2097.0109, 30.3729, 270.0
	Objet1: 5302, 2496.2619, 2100.3439, 29.472
	Objet2: 19429, 2499.663, 2098.0859, 29.899, 90.0
	//---
	HouseID = 3418
	Joueur: 2502.664, 2092.843, 31.3729
	Maison: 2502.664, 2100.1059, 32.5369
	Porte: 2506.1140, 2096.0891, 30.3729
	Objet1: 19429, 2499.363, 2096.165, 29.899, 90.0
	//---
	HouseID = 12991
	Porte: 1.965, 2.992, -2.161, 180.0
	//---
	HouseID = 18259
	Porte: -5.171, 0.2389, 0.86, 272.0
	19802
	*/
	new dObjectID;
	new Float:zDiff, Float:aDiff;
	switch(type)
	{
	    case 1:
		{
			dObjectID = 3414;
			zDiff = 1.199;
		}
	    case 2:
		{
			dObjectID = 3418;
			zDiff = 1.164;
		}
	    case 3:
		{
			dObjectID = 12991;
			zDiff = -1.0;
		}
	    case 4:
		{
			dObjectID = 18259;
			zDiff = 0.208;
		}
	    case 5://GARAGE
		{
			dObjectID = 3359;
			zDiff = 0.208;
		}
	}
	//---
	FixAngle(angle);
	dHouse[(load == -1) ? slotid : load][xHouse] = x;
	dHouse[(load == -1) ? slotid : load][yHouse] = y;
	dHouse[(load == -1) ? slotid : load][zHouse] = z;
	dHouse[(load == -1) ? slotid : load][aHouse] = angle;
	dHouse[(load == -1) ? slotid : load][dHouseType] = type;
	dHouse[(load == -1) ? slotid : load][bPorte][0] = door;
	dHouse[(load == -1) ? slotid : load][bPorte][1] = false;
	//---
	dHouse[(load == -1) ? slotid : load][oHouse][0] = CA_CreateDynamicObject_DC(dObjectID, x, y, z + zDiff, 0.0, 0.0, angle, -1, -1, -1, 550.0, 550.0);
    new dObject;
	switch(type)
	{
		case 1:
		{
			/*HouseID = 3414
			Joueur: 2502.664, 2092.843, 31.3729
			Maison: 2502.664, 2092.843, 32.5719
			Porte: 2504.708, 2097.0109, 30.3729, 270.0
			Objet1: 5302, 2496.2619, 2100.3439, 29.472
			Objet2: 19429, 2499.663, 2098.0859, 29.899, 90.0*/
			//---
			aDiff = angle + 178.0118 + 270.0;
			dHouse[(load == -1) ? slotid : load][oHouse][1] = CA_CreateDynamicObject_DC(5302, x + /*6.4021*/6.4 * floatsin(-aDiff, degrees), y + 6.4 * floatcos(-aDiff, degrees) , z + zDiff - 3.0999, 0.0, 0.0, angle, -1, -1, -1, 550.0, 550.0);
			aDiff = angle - 147.2722 + 270.0;
			dHouse[(load == -1) ? slotid : load][oHouse][2] = CA_CreateDynamicObject_DC(19429, x + /*3.001*/3.6 * floatsin(-aDiff, degrees), y + 3.6 * floatcos(-aDiff, degrees) , z + zDiff - 1.6729, 0.0, 0.0, angle + 90.0, -1, -1, -1, 550.0, 550.0);
			//---
			dObject = CA_GetObjectID(dHouse[(load == -1) ? slotid : load][oHouse][1]);
			for(new i = 0; i < 30; i ++) SetDynamicObjectMaterial(dObject, i, 3355, "cxref_savhus", "sw_barnwood5", 0xFFFFFFFF);
            dObject = CA_GetObjectID(dHouse[(load == -1) ? slotid : load][oHouse][2]);
			for(new i = 0; i < 30; i ++) SetDynamicObjectMaterial(dObject, i, 3355, "cxref_savhus", "sw_barnwood5", 0xFFFFFFFF);
			if(door)
			{
				aDiff = angle - 56.867 + 270.0;
				dHouse[(load == -1) ? slotid : load][oPorte] = CA_CreateDynamicObject_DC(19802, x + 3.75/*4.1679*/ * floatsin(-aDiff, degrees), y + 3.75 * floatcos(-aDiff, degrees) , z + zDiff - 2.199, 0.0, 0.0, angle + 270.0, -1, -1, -1, 550.0, 550.0);
                dObject = CA_GetObjectID(dHouse[(load == -1) ? slotid : load][oPorte]);
				for(new i = 0; i < 30; i ++) SetDynamicObjectMaterial(dObject, i, 3355, "cxref_savhus", "sw_barnwood5", 0xFFFFFFFF);
			}
		}
		case 2:
		{
			/*HouseID = 3418
			Joueur: 2502.664, 2092.843, 31.3729
			Maison: 2502.664, 2100.1059, 32.5369
			Porte: 2506.1140, 2096.0891, 30.3729
			Objet1: 19429, 2499.363, 2096.165, 29.899, 90.0*/
			//---
			//aDiff = angle - 60.2087;// - 90.0;
			aDiff = angle - 130.5697 + 270.0;
			dHouse[(load == -1) ? slotid : load][oHouse][1] = CA_CreateDynamicObject_DC(19429, x + 5.2 * floatsin(-aDiff, degrees), y + 5.2 * floatcos(-aDiff, degrees) , z + zDiff - 2.6379, 0.0, 0.0, angle + 90.0, -1, -1, -1, 550.0, 550.0);
            dObject = CA_GetObjectID(dHouse[(load == -1) ? slotid : load][oHouse][1]);
			for(new i = 0; i < 30; i ++) SetDynamicObjectMaterial(dObject, i, 3355, "cxref_savhus", "sw_barnwood5", 0xFFFFFFFF);
			if(door)
			{
				aDiff = angle - 49.715 + 270.0;
				dHouse[(load == -1) ? slotid : load][oPorte] = CA_CreateDynamicObject_DC(19802, x + 5.25/*4.0168*/ * floatsin(-aDiff, degrees), y + 5.25 * floatcos(-aDiff, degrees), z + zDiff - 2.199, 0.0, 0.0, angle + 90.0, -1, -1, -1, 550.0, 550.0);
                dObject = CA_GetObjectID(dHouse[(load == -1) ? slotid : load][oPorte]);
				for(new i = 0; i < 30; i ++) SetDynamicObjectMaterial(dObject, i, 3355, "cxref_savhus", "sw_barnwood5", 0xFFFFFFFF);
			}
		}
		/*HouseID = 12991
		Porte: 1.965, 2.992, -2.161, 180.0
		//---
		HouseID = 18259
		Porte: -5.171, 0.2389, 0.86, 272.0*/
		case 3:
		{
			/*HouseID = 12991
			Joueur: 2502.664, 2092.843, 31.3729
			Maison: 2502.664, 2101.4089, 30.3729, 180.0
			Porte: 2500.5449, 2098.4379, 30.207
			Porte2: 1446.0479, 1668.791, 9.8199, 0.0, 0.0, 0.0
			Maison2: 1445.505, 1665.831, 9.8199, 0.0, 0.0, 0.0*/
			//---
			if(door)
			{
				//aDiff = angle + 108.9805;
				aDiff = angle + 56.7050 + 270.0;
				dHouse[(load == -1) ? slotid : load][oPorte] = CA_CreateDynamicObject_DC(19802, x + 3.6/*6.1611 */* floatsin(-aDiff, degrees), y + 3.6 * floatcos(-aDiff, degrees), z + zDiff, 0.0, 0.0, angle + 180.0, -1, -1, -1, 550.0, 550.0);
                dObject = CA_GetObjectID(dHouse[(load == -1) ? slotid : load][oPorte]);
				for(new i = 0; i < 30; i ++) SetDynamicObjectMaterial(dObject, i, 3355, "cxref_savhus", "sw_barnwood5", 0xFFFFFFFF);
			}
		}
		case 4:
		{
			/*HouseID = 18259
			Joueur: 2502.664, 2092.843, 31.3729
			Maison: 2502.664, 2106.319, 31.5809, 90.0
			Porte: 2502.3701, 2101.1579, 31.6669
			Porte2: 1445.7709, 1658.0059, 12.416, 0.0, 0.0, 90.0
			Maison2: 1450.9150, 1659.29, 12.1689, 0.0, 0.0, 0.0*/
			//---
			if(door)
			{
				//aDiff = angle - 93.2592;
				aDiff = angle + 177.3548 + 90.0;
				dHouse[(load == -1) ? slotid : load][oPorte] = CA_CreateDynamicObject_DC(19802, x - 5.1099/*5.1611*/ * floatsin(-aDiff, degrees), y - 5.1099 * floatcos(-aDiff, degrees), z + 0.8 + zDiff, 0.0, 0.0, angle + 272.0, -1, -1, -1, 550.0, 550.0);
                dObject = CA_GetObjectID(dHouse[(load == -1) ? slotid : load][oPorte]);
				for(new i = 0; i < 30; i ++) SetDynamicObjectMaterial(dObject, i, 3355, "cxref_savhus", "sw_barnwood5", 0xFFFFFFFF);
			}
		}
	}
	//---
    slotid ++;
    if(slotid == MAX_HOUSES) slotid = 0;
    return (slotid == 0) ? MAX_HOUSES : slotid - 1;
}

/**
	<summary>
		Est-ce qu'il y a déjà une maison dans le rayon spécifié ?
    </summary>

    <param name="distance">
		Float - Le rayon à vérifier
    </param>

    <param name="x">
		Float - Position x
    </param>

    <param name="y">
		Float - Position y
    </param>

    <param name="z">
		Float - Position z
    </param>

    <returns>
		L'ID de la première maison trouvée
    </returns>
*/
stock IsHouseNearToPoint(Float:distance, Float:x, Float:y, Float:z)
{
	if(distance >= 0.0) {
		for(new i = 0; i < MAX_HOUSES; i ++) {
			if(GetDistanceBetweenPoints(x, y, z, dHouse[i][xHouse], dHouse[i][yHouse], dHouse[i][zHouse]) < distance) return i;
		}
	}
	return -1;
}

/**
	<summary>
        Ouvre ou ferme une porte
    </summary>

    <param name="houseid">
		int - Le type de maison
    </param>

    <param name="open">
		bool - Si on ouvre ou si on ferme la porte
    </param>

    <param name="x">
		bool - Si l'on est face à la porte ou derrière
    </param>

    <returns>
		void
    </returns>
    
    <remarks>
        Le paramètre face fonctionne, mais il y a un bug à corriger dans l'acquisition du ccôté de la porte où l'on est. (Voir GetDoorFacingForPlayer)
    </remarks>
*/
stock ChangeHouseDoorState(houseid, bool:open, bool:face = true)
{
	if(IsHouseIDValid(houseid))
	{
		new Float:aDiff, Float:fDiff, Float:zDiff;
		switch(dHouse[houseid][dHouseType])
		{
		    case 1:
			{
				fDiff = 270.0 + (open ? (face ? 90.0 : 270.0) : 0.0);
				zDiff = 1.199;
				aDiff = dHouse[houseid][aHouse] - 56.867 + 270.0;
				MoveDynamicObject(CA_GetObjectID(dHouse[houseid][oPorte]), dHouse[houseid][xHouse] + 3.75/*4.1679*/ * floatsin(-aDiff, degrees), dHouse[houseid][yHouse] + 3.75 * floatcos(-aDiff, degrees) , dHouse[houseid][zHouse] + zDiff - 2.199 + (open ? 0.01 : 0.005), 0.005, 0.0, 0.0, dHouse[houseid][aHouse] + fDiff);
			}
		    case 2:
			{
				fDiff = 0.0 + (open ? (face ? 90.0 : 270.0) : 0.0);
				zDiff = 1.164;
				aDiff = dHouse[houseid][aHouse] - 49.715 + 270.0;
				MoveDynamicObject(CA_GetObjectID(dHouse[houseid][oPorte]), dHouse[houseid][xHouse] + 5.25/*4.0168*/ * floatsin(-aDiff, degrees), dHouse[houseid][yHouse] + 5.25 * floatcos(-aDiff, degrees), dHouse[houseid][zHouse] + zDiff - 2.199 + (open ? 0.01 : 0.005), 0.005, 0.0, 0.0, dHouse[houseid][aHouse] + fDiff);
			}
		    case 3:
			{
				fDiff = 180.0 + (open ? (face ? 90.0 : 270.0) : 0.0);
				zDiff = -1.0;
				aDiff = dHouse[houseid][aHouse] + 56.7050 + 270.0;
				MoveDynamicObject(CA_GetObjectID(dHouse[houseid][oPorte]), dHouse[houseid][xHouse] + 3.6/*6.1611 */* floatsin(-aDiff, degrees), dHouse[houseid][yHouse] + 3.6 * floatcos(-aDiff, degrees), dHouse[houseid][zHouse] + zDiff + (open ? 0.01 : 0.005), 0.005, 0.0, 0.0, dHouse[houseid][aHouse] + fDiff);
			}
		    case 4:
			{
				fDiff = 272.0 + (open ? (face ? 90.0 : 270.0) : 0.0);
				zDiff = 0.208;
				aDiff = dHouse[houseid][aHouse] + 177.3548 + 90.0;
				MoveDynamicObject(CA_GetObjectID(dHouse[houseid][oPorte]), dHouse[houseid][xHouse] - 5.1099/*5.1611*/ * floatsin(-aDiff, degrees), dHouse[houseid][yHouse] - 5.1099 * floatcos(-aDiff, degrees), dHouse[houseid][zHouse] + zDiff + (open ? 0.01 : 0.005), 0.005, 0.0, 0.0, dHouse[houseid][aHouse] + fDiff);
			}
		}
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) if(IsPlayerInRangeOfPoint(i, 20.0, dHouse[houseid][xHouse], dHouse[houseid][yHouse], dHouse[houseid][zHouse])) PlayerPlaySound(i, 12201, dHouse[houseid][xHouse], dHouse[houseid][yHouse], dHouse[houseid][zHouse]);
		dHouse[houseid][bPorte][1] = open;
	}
}

/**
	<summary>
        Vérifie si on fait face à une porte ou si l'on est derrière
    </summary>

    <param name="houseid">
		int - L'ID de la maison
    </param>

    <param name="playerid">
		bool - Si on met une porte dessus ou pas
    </param>

    <returns>
		bool - true si on est face ï¿½ elle - false si l'on est derrière
    </returns>
    
    <remarks>
        Ne fonctionne pas - retourne toujours la même chose...
    </remarks>
*/
stock bool:GetDoorFacingForPlayer(houseid, playerid)
{
	new Float:x, Float:y, Float:z, Float:angle, Float:dx, Float:dy, Float:dz;
	GetPlayerPos(playerid, x, y, z);
	GetDynamicObjectPos(CA_GetObjectID(dHouse[houseid][oPorte]), dx, dy, dz);
	//new bool:bReturn = ;
	switch(dHouse[houseid][dHouseType])
	{
	    case 1: angle = dHouse[houseid][aHouse];// + 270.0;
	    case 2: angle = dHouse[houseid][aHouse];// + 0.0;
	    case 3: angle = dHouse[houseid][aHouse];// + 180.0;
	    case 4: angle = dHouse[houseid][aHouse];// + 272.0;
	}
	new Float:fAngle[2];
	fAngle[0] = angle + 180.0;
	fAngle[1] = angle;
	FixAngle(fAngle[0]);
	FixAngle(fAngle[1]);
	new bool:bReturn = (fAngle[0] >= GetZAngleBetweenPos(x, y, dx, dy) >= fAngle[1]) ? false : true;
	if(fAngle[0] < fAngle[1]) bReturn = !bReturn;
	return bReturn;
}

/**
	<summary>
        Détruit une maison
    </summary>

    <param name="houseid">
		int - L'ID de la maison
    </param>

    <returns>
		void
    </returns>
*/
stock DestroyHouse(houseid)
{
	if(IsHouseValid(houseid)) {
		CA_DestroyObject_DC(dHouse[houseid][oHouse][0]);
		dHouse[houseid][oHouse][0] = INVALID_OBJECT_ID;
		//---
		if(dHouse[houseid][dHouseType] == 1)
		{
			CA_DestroyObject_DC(dHouse[houseid][oHouse][1]);
			dHouse[houseid][oHouse][1] = INVALID_OBJECT_ID;
			CA_DestroyObject_DC(dHouse[houseid][oHouse][2]);
			dHouse[houseid][oHouse][2] = INVALID_OBJECT_ID;
		}
		if(dHouse[houseid][dHouseType] == 2)
		{
			CA_DestroyObject_DC(dHouse[houseid][oHouse][1]);
			dHouse[houseid][oHouse][1] = INVALID_OBJECT_ID;
		}
		if(dHouse[houseid][bPorte][0])
		{
			CA_DestroyObject_DC(dHouse[houseid][oPorte]);
			dHouse[houseid][oPorte] = INVALID_OBJECT_ID;
		}
		//---
		dHouse[houseid][bPorte][0] = false;
		dHouse[houseid][xHouse] = 0.0;
		dHouse[houseid][yHouse] = 0.0;
		dHouse[houseid][zHouse] = 0.0;
		dHouse[houseid][aHouse] = 0.0;
	    dHouse[houseid][dHouseType] = 0;
    }
}

/**
	<summary>
        Charge toutes les maisons
    </summary>

    <returns>
		void
    </returns>
*/
stock SetupHouses()
{
	new dLastLoaded = 0;
	
	for(new i = 0; i < MAX_HOUSES; i ++) dHouse[i][dHouseType] = 0;
	//CHARGEMENT
    INI_ParseFile(HPATH, "LoadHouses_data");
	for(new i = 0; i < MAX_HOUSES; i ++)
	{
	    if(dHouse[i][xHouse] == 0.0 && dHouse[i][yHouse] == 0.0 && dHouse[i][zHouse] == 0.0)
		{
			dHouse[i][dHouseType] = 0;
		}

		if(dHouse[i][dHouseType] != 0)
		{
		    new bool:bDoor = dHouse[i][bPorte][1];
			CreateHouse(dHouse[i][dHouseType], dHouse[i][bPorte][0], dHouse[i][xHouse], dHouse[i][yHouse], dHouse[i][zHouse], dHouse[i][aHouse], i);
			ChangeHouseDoorState(i, bDoor);
			dLastLoaded = i;
		}
	}
	
	for(new i = dLastLoaded + 1; i < MAX_HOUSES; i ++)
	{
		dHouse[i][dHouseType] = 0;
	}

    LogInfo(true, "[INIT]Maisons chargees");
}

/**
	<summary>
        Charge les maisons à partir du fichier
    </summary>

    <param name="name">
		string - Nom des champs
    </param>

    <param name="value">
		/ - Les valeurs retournées
    </param>

    <returns>
		int - 1
    </returns>

    <remarks>
        Ne pas utiliser tel quel
    </remarks>
*/
public LoadHouses_data(name[], value[])
{
	new string[50];
	for(new i = 0; i < MAX_HOUSES; i ++)
	{
	    format(string, sizeof(string), "HouseType%d", i);
		INI_Int(string, dHouse[i][dHouseType]);
	    format(string, sizeof(string), "Door%d", i);
		INI_Bool(string, dHouse[i][bPorte][0]);
	    format(string, sizeof(string), "DoorOpened%d", i);
		INI_Bool(string, dHouse[i][bPorte][1]);
	    format(string, sizeof(string), "DoorCode%d", i);
		INI_String(string, dHouse[i][sCodePorte], 5);
	    format(string, sizeof(string), "xHouse%d", i);
		INI_Float(string, dHouse[i][xHouse]);
	    format(string, sizeof(string), "yHouse%d", i);
		INI_Float(string, dHouse[i][yHouse]);
	    format(string, sizeof(string), "zHouse%d", i);
		INI_Float(string, dHouse[i][zHouse]);
	    format(string, sizeof(string), "aHouse%d", i);
		INI_Float(string, dHouse[i][aHouse]);
	}
	return 1;
}

/**
	<summary>
        Sauvegarde les informations concernant les maisons dans un fichier
    </summary>

    <returns>
		void
    </returns>
*/
stock SaveHouses()
{
	new string[50];
	new INI:File = INI_Open(HPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_HOUSES; i ++)
	{
	    format(string, sizeof(string), "HouseType%d", i);
		INI_WriteInt(File,string, dHouse[i][dHouseType]);
	    format(string, sizeof(string), "Door%d", i);
		INI_WriteBool(File,string, dHouse[i][bPorte][0]);
	    format(string, sizeof(string), "DoorOpened%d", i);
		INI_WriteBool(File,string, dHouse[i][bPorte][1]);
	    format(string, sizeof(string), "DoorCode%d", i);
		INI_WriteString(File,string, dHouse[i][sCodePorte]);
	    format(string, sizeof(string), "xHouse%d", i);
		INI_WriteFloat(File,string, dHouse[i][xHouse]);
	    format(string, sizeof(string), "yHouse%d", i);
		INI_WriteFloat(File,string, dHouse[i][yHouse]);
	    format(string, sizeof(string), "zHouse%d", i);
		INI_WriteFloat(File,string, dHouse[i][zHouse]);
	    format(string, sizeof(string), "aHouse%d", i);
		INI_WriteFloat(File,string, dHouse[i][aHouse]);
	}
	INI_Close(File);
}

/**
	<summary>
        Est-ce qu'un joueur est proche de la porte
    </summary>

    <param name="playerid">
		int - Le joueur dont la position est à vérifier
    </param>

    <returns>
		int - L'ID de la maison dont la porte est proche du joueur
    </returns>
*/
stock IsPlayerNearHouseDoor(playerid)
{
	for(new i = 0; i < MAX_HOUSES; i ++)
	{
	    if(IsHouseValid(i) && dHouse[i][bPorte][0])
	    {
	        if(IsDynamicObjectNearToPlayer(2.5, playerid, CA_GetObjectID(dHouse[i][oPorte])))
	        {
	            return i;
	        }
		}
	}
	return -1;
}

/**
	<summary>
        Est-ce que le joueur est proche d'une maison
    </summary>

    <param name="playerid">
		int - L'ID du joueur à vérifier
    </param>

    <param name="radius">
		Float - Le rayon à vérifier
    </param>

    <returns>
		bool - true si une maison est trouvée à proximité du joueur
    </returns>
*/
stock IsPlayerNearHouse(playerid, Float:radius)
{
	for(new i = 0; i < MAX_HOUSES; i ++) {
	    if(IsHouseValid(i)) {
			if(IsPlayerInRangeOfPoint(playerid, radius, dHouse[i][xHouse], dHouse[i][yHouse], dHouse[i][zHouse])) return true;
		}
	}
	return false;
}

/**
	<summary>
        Est-ce que le joueur est proche d'une maison spécifique
    </summary>

    <param name="playerid">
		int - L'ID du joueur à vérifier
    </param>

    <param name="houseid">
		int - L'ID de la maison à vérifier
    </param>

    <param name="radius">
		Float - Le rayon à vérifier
    </param>

    <returns>
		bool - true si la maison est trouvée à proximité du joueur
    </returns>
*/
stock IsPlayerNearHouse(playerid, houseid, Float:radius)
{
	if(MAX_HOUSES > houseid >= 0 && dHouse[houseid][dHouseType])
	{
		return IsPlayerInRangeOfPoint(playerid, radius, dHouse[houseid][xHouse], dHouse[houseid][yHouse], dHouse[houseid][zHouse]);
	}
	return false;
}

/**
	<summary>
	    Est-ce que cette maison existe ?
	</summary>
	
	<param name="houseid">
	    int - L'ID de la maison ï¿½ vï¿½rifier
	</param>
	
	<return>
		bool - Si la maison est valide
	</return>
*/
stock bool:IsHouseValid(houseid)
{
	return (IsHouseIDValid(houseid) && (5 > dHouse[houseid][dHouseType] > 0));
}

/**
	<summary>
	    Est-ce que cet ID de maison est valide
	</summary>

	<param name="houseid">
	    int - L'ID de la maison ï¿½ vï¿½rifier
	</param>

	<return>
		bool - Si l'ID de la maison est dans les limites du tableau
	</return>
*/
stock bool:IsHouseIDValid(houseid)
{
	return (MAX_HOUSES > houseid >= 0);
}

stock SetPlayerHouseBuild(playerid, buildstate)
{
	if(IsValidPlayer(playerid)) 
	{
		dHouseBuild[playerid] = buildstate;
	}
}

stock GetPlayerHouseBuild(playerid)
{
	return IsValidPlayer(playerid) ? dHouseBuild[playerid] : 0;
}

stock SetPlayerHouseEdit(playerid, houseid) 
{
	if(IsValidPlayer(playerid)) 
	{
		dHouse[playerid] = (MAX_HOUSES > houseid >= 0) ? houseid : -1;
	}
}

stock GetPlayerHouseEdit(playerid)
{
	return (IsValidPlayer(playerid)) ? dHouse[playerid] : -1;
}

stock SetPlayerActionDoor(playerid, door) 
{
	if(IsValidPlayer(playerid) && IsValidHouse(door)) 
	{
		dDoor[playerid] = door;
	}
}

stock GetPlayerGarage(playerid) 
{
	return IsValidPlayer(playerid) ? pGarage[playerid] : -1;
}

stock SetPlayerGarage(playerid, garageid) 
{
	if(IsValidPlayer(playerid)) 
	{
		pGarage[playerid] = garageid;
	}
}

stock GetPlayerTank(playerid) 
{
	return IsValidPlayer(playerid) ? pTank[playerid] : -1;
}

stock SetPlayerTank(playerid, tankid) 
{
	if(IsValidPlayer(playerid)) 
	{
		pTank[playerid] = tankid;
	}
}

stock GetPlayerFurn(playerid) 
{
	return IsValidPlayer(playerid) ? pFurn[playerid] : -1;
}

stock SetPlayerFurn(playerid, furnid) 
{
	if(IsValidPlayer(playerid)) 
	{
		pFurn[playerid] = furnid;
	}
}

stock GetPlayerBed(playerid) 
{
	return IsValidPlayer(playerid) ? pBed[playerid] : -1;
}

stock SetPlayerBed(playerid, bedid) 
{
	if(IsValidPlayer(playerid)) 
	{
		pBed[playerid] = bedid;
	}
}

stock GetPlayerActionDoor(playerid) 
{
	return IsValidPlayer(playerid) ? pBed[playerid] : -1;
}

stock SetPlayerSeat(playerid, seatid) 
{
	if(IsValidPlayer(playerid)) 
	{
		pSeat[playerid] = seatid;
	}
}

stock GetPlayerSeat(playerid) 
{
	return IsValidPlayer(playerid) ? pSeat[playerid] : -1;
}

stock GetSeatObject(seatid) 
{
	return IsValidSeat(seatid) ? dSeat[seatid][oSeat] : INVALID_OBJECT_ID;
}

stock GetHousePos(houseid, &Float:x, &Float:y, &Float:z)
{
	if(MAX_HOUSES > houseid >= 0) 
	{
		if(dHouse[dHouseType] != 0) 
		{
			x = dHouse[houseid][x];
			y = dHouse[houseid][y];
			z = dHouse[houseid][z];
		}
		else 
		{
			x = 0.0;
			y = 0.0;
			z = 0.0;
		}
	}
}

stock GetHouseAngle(houseid, &Float:angle)
{
	if(MAX_HOUSES > houseid >= 0) 
	{
		angle = (dHouse[dHouseType] != 0) ? dHouse[house][aHouse] : 0.0;
	}
}

stock GetHouseType(houseid)
{
	return (MAX_HOUSES > houseid >= 0) ? dHouse[houseid][dHouseType] : 0;
}

stock GetHouseCode(houseid)
{
	return (MAX_HOUSES > houseid >= 0) ? dHouse[houseid][sCodePorte] : "";
}

stock SetHouseCode(houseid, const code[]) 
{
	format(dHouse[house][sCodePorte], 5, "%s", code);
}

stock IsValidHouse(houseid) 
{
	return MAX_HOUSES > hosueid >= 0;
}

stock GetHouseDoorExists(houseid) 
{
	return IsValidHouse(houseid) ? dHouse[houseid][bPorte][0] : false;
}

stock GetHouseDoorState(houseid)
{
	return IsValidHouse(houseid) ? dHouse[houseid][bPorte][1] : false;
}

stock InitializeTentVariables() 
{
	for(new i = 0; i < MAX_TENTS; i ++)
	{
		dTent[i][bTent] = false;
		dTent[i][oTent][0] = INVALID_OBJECT_ID;
		dTent[i][oTent][1] = INVALID_OBJECT_ID;
		dTent[i][oTent][2] = INVALID_OBJECT_ID;
		dTent[i][oTent][3] = INVALID_OBJECT_ID;
		dTent[i][oTent][4] = INVALID_OBJECT_ID;
		dTent[i][oTent][5] = INVALID_OBJECT_ID;
		dTent[i][xTent] = 0.0;
		dTent[i][yTent] = 0.0;
		dTent[i][zTent] = 0.0;
		dTent[i][aTent] = 0.0;
	}
}

stock CreateTent(Float:x, Float:y, Float:z, Float:angle, load = -1)
{
	static slotid;
	if(load == -1)
	{
		if(dTent[slotid][bTent])
		{
		    for(new i = 0; i < MAX_TENTS; i ++)
		    {
		        if(!dTent[i][bTent])
		        {
		            slotid = i;
		            break;
		        }
		    }
		}
		if(dTent[slotid][bTent]) for(new i = 0; i < 6; i ++) CA_DestroyObject_DC(dTent[slotid][oTent][i]);
	}
	//---
	dTent[(load == -1) ? slotid : load][xTent] = x;
	dTent[(load == -1) ? slotid : load][yTent] = y;
	dTent[(load == -1) ? slotid : load][zTent] = z;
	dTent[(load == -1) ? slotid : load][aTent] = angle;
	dTent[(load == -1) ? slotid : load][bTent] = true;
	//---- 11.363850276 dérivation 1 - 10.8165809 dérivation 2
	dTent[(load == -1) ? slotid : load][oTent][0] = CA_CreateDynamicObject_DC(19325, x - 1.1031 * floatsin(-(angle - 82.121), degrees), y - 1.1031 * floatcos(-(angle - 82.121), degrees), z - 0.1919, 0.0, 32.0, angle, -1, -1, -1, 550.0, 550.0);
	dTent[(load == -1) ? slotid : load][oTent][1] = CA_CreateDynamicObject_DC(19325, x - 1.1031 * floatsin(-(angle + 81.8452), degrees), y - 1.1031 * floatcos(-(angle + 81.8452), degrees) , z - 0.1919, 0.0, 328.0, angle, -1, -1, -1, 550.0, 550.0);
	//---
	SetDynamicObjectMaterial(CA_GetObjectID(dTent[(load == -1) ? slotid : load][oTent][0]), 0, 3066, "ammotrx", "ammotrn92tarp128", -1); //object(lsmall_window01) (2)
	SetDynamicObjectMaterial(CA_GetObjectID(dTent[(load == -1) ? slotid : load][oTent][1]), 0, 3066, "ammotrx", "ammotrn92tarp128", -1); //object(lsmall_window01) (2)
	//---
	dTent[(load == -1) ? slotid : load][oTent][2] = CA_CreateDynamicObject_DC(2068, x + 2.563 * floatsin(-(angle + 163.0529), degrees), y + 2.563 * floatcos(-(angle + 163.0529), degrees), z - 4.0539, 42.0, 285.0, 260.0 + angle, -1, -1, -1, 550.0, 550.0);
	dTent[(load == -1) ? slotid : load][oTent][3] = CA_CreateDynamicObject_DC(2068, x + 3.771 * floatsin(-(angle - 168.883), degrees), y + 3.771 * floatcos(-(angle - 168.883), degrees), z - 4.0539, 42.0, 285.0, 80.0 + angle, -1, -1, -1, 550.0, 550.0);
	dTent[(load == -1) ? slotid : load][oTent][4] = CA_CreateDynamicObject_DC(2068, x + 2.563 * floatsin(-(angle - 14.3909), degrees), y + 2.563 * floatcos(-(angle - 14.3909), degrees), z - 4.0539, 42.0, 285.0, 80.0 + angle, -1, -1, -1, 550.0, 550.0);
	dTent[(load == -1) ? slotid : load][oTent][5] = CA_CreateDynamicObject_DC(2068, x + 3.771 * floatsin(-(angle + 10.7698), degrees), y + 3.771 * floatcos(-(angle + 10.7698), degrees), z - 4.0539, 42.0, 285.0, 260.0 + angle, -1, -1, -1, 550.0, 550.0);
	//---2.8879    4.1059
	/*
	Le mec
	2486.1689, 1037.478, 57.5019, 0.0

	Parois
	19325, 2485.0568, 1037.6319, 57.31, 0.0, 32.0, 0.0
	19325, 2487.2429, 1037.6319, 57.31, 0.0, 328.0, 0.0
	//---
    -1.1121, 0.1539     -1.4332828885831745
    1.074, 0.1539       1.4284691310527518

	Camouflage
	2068, 2485.3879, 1034.915, 53.448, 42.0, 285.0, 260.0
	2068, 2486.9099, 1033.707, 53.448, 42.0, 285.0, 80.0
	2068, 2486.9221, 1040.3659, 53.448, 42.0, 285.0, 80.0
	2068, 2485.3879, 1041.5839, 53.448, 42.0, 285.0, 260.0
	//---
	-0.781, -2.563      -2.8458102864047072
	0.741, -3.771       2.947565115297439
	0.741, 2.8879       0.25116930040828855
	-0.781, 4.1059      -0.18796856148048377
	*/
    slotid ++;
    if(slotid == MAX_TENTS) slotid = 0;
    return (slotid == 0) ? MAX_TENTS : slotid - 1;
}

stock DestroyTent(tentid)
{
	for(new i = 0; i < 6; i ++)
	{
		CA_DestroyObject_DC(dTent[tentid][oTent][i]);
		dTent[tentid][oTent][i] = INVALID_OBJECT_ID;
	}
	dTent[tentid][xTent] = 0.0;
	dTent[tentid][yTent] = 0.0;
	dTent[tentid][zTent] = 0.0;
	dTent[tentid][aTent] = 0.0;
    dTent[tentid][bTent] = false;
}

public LoadTents_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_TENTS; i ++)
	{
	    format(string, sizeof(string), "Tente%d", i);
		INI_Bool(string, dTent[i][bTent]);
	    format(string, sizeof(string), "xTente%d", i);
		INI_Float(string, dTent[i][xTent]);
	    format(string, sizeof(string), "yTente%d", i);
		INI_Float(string, dTent[i][yTent]);
	    format(string, sizeof(string), "zTente%d", i);
		INI_Float(string, dTent[i][zTent]);
	    format(string, sizeof(string), "aTente%d", i);
		INI_Float(string, dTent[i][aTent]);
	}
	return 1;
}

stock SaveTents()
{
	new string[50];
	new INI:File = INI_Open(TPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_TENTS; i ++)
	{
	    format(string, sizeof(string), "Tente%d", i);
		INI_WriteBool(File,string, dTent[i][bTent]);
	    format(string, sizeof(string), "xTente%d", i);
		INI_WriteFloat(File,string, dTent[i][xTent]);
	    format(string, sizeof(string), "yTente%d", i);
		INI_WriteFloat(File,string, dTent[i][yTent]);
	    format(string, sizeof(string), "zTente%d", i);
		INI_WriteFloat(File,string, dTent[i][zTent]);
	    format(string, sizeof(string), "aTente%d", i);
		INI_WriteFloat(File,string, dTent[i][aTent]);
	}
	INI_Close(File);
}

stock FindNearbyTents(playerid, Float:x, Float:y, Float:z, slot, tInArray[][][])
{
    //new len = sizeof(tInArray[]);
	new Float:fTrash;
    for(new i = 0; i < MAX_TENTS; i ++)
    {
        if(slot < 9) break;
        if(dTent[i][bTent]) continue;
        if(IsPlayerInRangeOfPoint(playerid, 3.0, dTent[i][xTent], dTent[i][yTent], dTent[i][zTent]))
        {
            if(CA_RayCastLine(x, y, z, dTent[i][xTent], dTent[i][yTent], dTent[i][zTent], fTrash, fTrash, fTrash) != 0) continue;
            tInArray[playerid][slot][0] = i;
            tInArray[playerid][slot][1] = 3;
            slot ++;
        }
    }

    return slot;
}

stock IsTentSet(tentid)
{
	return IsValidTent(tentid) ? dTent[tentid][bTent] : false;
}

stock IsValidTent(tentid)
{
	return MAX_TENTS > tentid >= 0;
}

stock SetupTents() 
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_TENTS; i ++) dTent[i][bTent] = false;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(TPATH, "LoadTents_data");
    #endif
	for(new i = 0; i < MAX_TENTS; i ++)
	{
	    if(dTent[i][xTent] == 0.0 && dTent[i][yTent] == 0.0 && dTent[i][zTent] == 0.0) dTent[i][bTent] = false;
		if(dTent[i][bTent])
		{
			CreateTent(dTent[i][xTent], dTent[i][yTent], dTent[i][zTent], dTent[i][aTent], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_TENTS; i ++) dTent[i][bTent] = false;
    LogInfo(true, "[INIT]Tentes charges");
}

stock FindNearbyBeds(playerid, Float:x, Float:y, Float:z, slot, tInArray[][][])
{
	new Float:fTrash;
	for(new i = 0; i < MAX_BEDS; i ++)
	{
	    if(slot == 9) break;
		if(dBed[i][dBedType] == 0) continue;
		if(IsPlayerInRangeOfPoint(playerid, 3.0, dBed[i][xBed], dBed[i][yBed], dBed[i][zBed]))
		{
			if(CA_RayCastLine(x, y, z, dBed[i][xBed], dBed[i][yBed], dBed[i][zBed], fTrash, fTrash, fTrash) != 0) continue;
		    tInArray[playerid][dSlot][0] = i;
		    tInArray[playerid][dSlot][1] = 2;
			slot ++;
		}
	}
}

CanPlayerCreateBed(playerid)
{
	for(new i = 0; i < MAX_TENTS; i ++) 
	{
		if(IsPlayerInRangeOfPoint(playerid, 10.0, dTent[i][xTent], dTent[i][yTent], dTent[i][zTent])) return true;
	}
	for(new i = 0; i < MAX_HOUSES; i ++) 
	{
		if(IsPlayerNearHouse(playerid, i, 10.0)) return true;
	}
	return false;
}

CreateBed(type, Float:x, Float:y, Float:z, Float:angle, load = -1)
{
	//18122486.114, 1038.588, 56.5019, 0.0, 0.0, 0.0
	static slotid;
	if(type != 0)
	{
		if(load == -1)
		{
			if(dBed[slotid][dBedType] != 0)
			{
			    for(new i = 0; i < MAX_BEDS; i ++)
			    {
			        if(dBed[i][dBedType] == 0)
			        {
			            slotid = i;
			            break;
			        }
			    }
			}
			if(dBed[slotid][dBedType] != 0) DestroyDynamicObject(dBed[slotid][oBed]);
		}
		//---
		dBed[(load == -1) ? slotid : load][xBed] = x;
		dBed[(load == -1) ? slotid : load][yBed] = y;
		dBed[(load == -1) ? slotid : load][zBed] = z;
		dBed[(load == -1) ? slotid : load][aBed] = angle;
		//---
		dBed[(load == -1) ? slotid : load][dBedType] = type;
		switch(type)
		{
			case 1: z -= 1.0;
			case 2: z -= 1.0;
		}
		//---
		new dBeds[] = {1812, 1801};
		//---
		dBed[(load == -1) ? slotid : load][oBed] = CreateDynamicObject(dBeds[type - 1], x, y, z, 0.0, 0.0, angle);
	    slotid ++;
    }
    if(slotid == MAX_BEDS) slotid = 0;
    return (slotid == 0) ? MAX_BEDS : slotid - 1;
}

DestroyBed(bedid)
{
	DestroyDynamicObject(dBed[bedid][oBed]);
	dBed[bedid][oBed] = INVALID_OBJECT_ID;
	dBed[bedid][dBedType] = 0;
	dBed[bedid][xBed] = 0.0;
	dBed[bedid][yBed] = 0.0;
	dBed[bedid][zBed] = 0.0;
	dBed[bedid][aBed] = 0.0;
}

IsPlayerNearBed(playerid)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	if(IsPlayerInRangeOfPoint(playerid, 3.0, -55.3701, -230.7322, 5.9485) && LoadMissionProgress(playerid, "Intro") >= 4)
	{
	    return BUD_BED;
	}
	for(new i = 0; i < MAX_BEDS; i ++)
	{
		if(dBed[i][dBedType] != 0 && IsPlayerInRangeOfPoint(playerid, 3.0, dBed[i][xBed], dBed[i][yBed], dBed[i][zBed]))
		{
		    if(IsColBetweenTwoPos(x, y, z, dBed[i][xBed], dBed[i][yBed], dBed[i][zBed])) continue;
			return i;
		}
	}
	return -1;
}

public LoadBeds_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_BEDS; i ++)
	{
	    format(string, sizeof(string), "LitID%d", i);
		INI_Int(string, dBed[i][dBedType]);
	    format(string, sizeof(string), "xLit%d", i);
		INI_Float(string, dBed[i][xBed]);
	    format(string, sizeof(string), "yLit%d", i);
		INI_Float(string, dBed[i][yBed]);
	    format(string, sizeof(string), "zLit%d", i);
		INI_Float(string, dBed[i][zBed]);
	    format(string, sizeof(string), "aLit%d", i);
		INI_Float(string, dBed[i][aBed]);
	}
	return 1;
}

SaveBeds()
{
	new string[50];
	new INI:File = INI_Open(BPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_BEDS; i ++)
	{
	    format(string, sizeof(string), "LitID%d", i);
		INI_WriteInt(File,string, dBed[i][dBedType]);
	    format(string, sizeof(string), "xLit%d", i);
		INI_WriteFloat(File,string, dBed[i][xBed]);
	    format(string, sizeof(string), "yLit%d", i);
		INI_WriteFloat(File,string, dBed[i][yBed]);
	    format(string, sizeof(string), "zLit%d", i);
		INI_WriteFloat(File,string, dBed[i][zBed]);
	    format(string, sizeof(string), "aLit%d", i);
		INI_WriteFloat(File,string, dBed[i][aBed]);
	}
	INI_Close(File);
}

stock InitializeBedVariables()
{
	for(new i = 0; i < MAX_BEDS; i ++)
	{
		dBed[i][dBedType] = -1;
		dBed[i][oBed] = INVALID_OBJECT_ID;
		dBed[i][xBed] = 0.0;
		dBed[i][yBed] = 0.0;
		dBed[i][zBed] = 0.0;
		dBed[i][aBed] = 0.0;
	}
}

stock SetupBeds()
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_BEDS; i ++) dBed[i][dBedType] = 0;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(BPATH, "LoadBeds_data");
    #endif
	for(new i = 0; i < MAX_BEDS; i ++)
	{
	    if(dBed[i][xBed] == 0.0 && dBed[i][yBed] == 0.0 && dBed[i][zBed] == 0.0) dBed[i][dBedType] = 0;
		if(dBed[i][dBedType] != 0)
		{
			CreateBed(dBed[i][dBedType], dBed[i][xBed], dBed[i][yBed], dBed[i][zBed], dBed[i][aBed], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_BEDS; i ++) dBed[i][dBedType] = 0;
    LogInfo(true, "[INIT]Lits charges");
}

stock IsValidBed(bedid) 
{
	return MAX_BEDS > bedid >= 0;
}

stock GetBedType(bedid)
{
	return IsValidBed(bedid) ? dBed[bedid][dBedType] : 0;
}

stock GetBedPos(bedid, &Float:x, &Float:y, &Float:z, &Float:a)
{
	if(IsValidBed(bedid))
	{
		x = dBed[bedid][xBed];
		y = dBed[bedid][yBed];
		z = dBed[bedid][zBed];
		a = dBed[bedid][aBed];
	}
}

stock GetBedObject(bedid)
{
	return IsValidBed(bedid) ? dBed[bedid][oBed] : INVALID_OBJECT_ID;
}

stock InitializeGarageVariables() 
{	
	for(new i = 0; i < MAX_GARAGES; i ++)
	{
		dGarage[i][oGarage][0] = INVALID_OBJECT_ID;
		dGarage[i][oGarage][1] = INVALID_OBJECT_ID;
		dGarage[i][xGarage] = 0.0;
		dGarage[i][yGarage] = 0.0;
		dGarage[i][zGarage] = 0.0;
		dGarage[i][aGarage] = 0.0;
		dGarage[i][bGarage][0] = false;
		dGarage[i][bGarage][1] = false;
	}
}

stock CreateGarage(Float:x, Float:y, Float:z, Float:angle, load = -1)
{
	static slotid;
	if(load == -1)
	{
		if(dGarage[slotid][bGarage][0])
		{
		    for(new i = 0; i < MAX_GARAGES; i ++)
		    {
		        if(!dGarage[i][bGarage][0])
		        {
		            slotid = i;
		            break;
		        }
		    }
		}
		if(dGarage[slotid][bGarage][0])
		{
			CA_DestroyObject_DC(dGarage[slotid][oGarage][0]), dGarage[slotid][oGarage][0] = INVALID_OBJECT_ID;
			CA_DestroyObject_DC(dGarage[slotid][oGarage][1]), dGarage[slotid][oGarage][1] = INVALID_OBJECT_ID;
		}
	}
	//---
	dGarage[(load == -1) ? slotid : load][xGarage] = x;
	dGarage[(load == -1) ? slotid : load][yGarage] = y;
	dGarage[(load == -1) ? slotid : load][zGarage] = z;
	dGarage[(load == -1) ? slotid : load][aGarage] = angle;
	dGarage[(load == -1) ? slotid : load][bGarage][0] = true;
	dGarage[(load == -1) ? slotid : load][bGarage][1] = false;
	//---
	dGarage[(load == -1) ? slotid : load][oGarage][0] = CA_CreateDynamicObject_DC(3359, x, y, z, 0.0, 0.0, angle, -1, -1, -1, 550.0, 550.0);
	dGarage[(load == -1) ? slotid : load][oGarage][1] = CA_CreateDynamicObject_DC(11416, x, y, z, 0.0, 0.0, angle, -1, -1, -1, 550.0, 550.0);
	//---
    slotid ++;
    if(slotid == MAX_GARAGES) slotid = 0;
    return (slotid == 0) ? MAX_GARAGES : slotid - 1;
}

stock ChangeGarageDoorState(garageid, bool:open)
{
	new Float:aDiff = dGarage[garageid][aGarage] + 90.0 - (open ? 63.4608054 : 74.0490654);
	new Float:x, Float:y;
	x = dGarage[garageid][xGarage] - (open ? 2.8585 : 4.6468/*4.4677735 : 2.5570069*/) * floatsin(-aDiff, degrees);
	y = dGarage[garageid][yGarage] - (open ? 2.8585 : 4.6468/*4.4677735 : 2.5570069*/) * floatcos(-aDiff, degrees);
	MoveDynamicObject(CA_GetObjectID(dGarage[garageid][oGarage][1]), x, y, dGarage[garageid][zGarage] + (open ? 3.958 : 2.094), 3.5, 0.0, open ? 90.0 : 0.0, dGarage[garageid][aGarage] + 90.0);
	dGarage[garageid][bGarage][1] = open;
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) if(IsPlayerConnected(i)) PlayerPlaySound(i, 1153, dGarage[garageid][xGarage], dGarage[garageid][yGarage], dGarage[garageid][zGarage]);
}

stock DestroyGarage(garageid)
{
	CA_DestroyObject_DC(dGarage[garageid][oGarage][0]);
	dGarage[garageid][oGarage][0] = INVALID_OBJECT_ID;
	CA_DestroyObject_DC(dGarage[garageid][oGarage][1]);
	dGarage[garageid][oGarage][1] = INVALID_OBJECT_ID;
	//---
	dGarage[garageid][xGarage] = 0.0;
	dGarage[garageid][yGarage] = 0.0;
	dGarage[garageid][zGarage] = 0.0;
	dGarage[garageid][aGarage] = 0.0;
	dGarage[garageid][bGarage][0] = false;
	dGarage[garageid][bGarage][1] = false;
}

public LoadGarages_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_GARAGES; i ++)
	{
	    format(string, sizeof(string), "Garage%d", i);
		INI_Bool(string, dGarage[i][bGarage][0]);
	    format(string, sizeof(string), "DoorOpened%d", i);
		INI_Bool(string, dGarage[i][bGarage][1]);
	    format(string, sizeof(string), "DoorCode%d", i);
		INI_String(string, dGarage[i][sCodeGarage], 5);
	    format(string, sizeof(string), "xGarage%d", i);
		INI_Float(string, dGarage[i][xGarage]);
	    format(string, sizeof(string), "yGarage%d", i);
		INI_Float(string, dGarage[i][yGarage]);
	    format(string, sizeof(string), "zGarage%d", i);
		INI_Float(string, dGarage[i][zGarage]);
	    format(string, sizeof(string), "aGarage%d", i);
		INI_Float(string, dGarage[i][aGarage]);
	}
	return 1;
}

stock SaveGarages()
{
	new string[50];
	new INI:File = INI_Open(GARPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_GARAGES; i ++)
	{
	    format(string, sizeof(string), "Garage%d", i);
		INI_WriteBool(File,string, dGarage[i][bGarage][0]);
	    format(string, sizeof(string), "DoorOpened%d", i);
		INI_WriteBool(File,string, dGarage[i][bGarage][1]);
	    format(string, sizeof(string), "DoorCode%d", i);
		INI_WriteString(File,string, dGarage[i][sCodeGarage]);
	    format(string, sizeof(string), "xGarage%d", i);
		INI_WriteFloat(File,string, dGarage[i][xGarage]);
	    format(string, sizeof(string), "yGarage%d", i);
		INI_WriteFloat(File,string, dGarage[i][yGarage]);
	    format(string, sizeof(string), "zGarage%d", i);
		INI_WriteFloat(File,string, dGarage[i][zGarage]);
	    format(string, sizeof(string), "aGarage%d", i);
		INI_WriteFloat(File,string, dGarage[i][aGarage]);
	}
	INI_Close(File);
}

stock IsPlayerNearGarageDoor(playerid)
{
	for(new i = 0; i < MAX_GARAGES; i ++)
	{
	    if(dGarage[i][bGarage][0])
	    {
	        if(IsPlayerInRangeOfPoint(playerid, 8.0, dGarage[i][xGarage], dGarage[i][yGarage], dGarage[i][zGarage]))
	        {
	            return i;
	        }
		}
	}
	return -1;
}

stock IsPlayerNearGarage(playerid, Float:range)
{
	for(new i = 0; i < MAX_GARAGES; i ++) if(IsPlayerInRangeOfPoint(playerid, range, dGarage[i][xGarage], dGarage[i][yGarage], dGarage[i][zGarage]))
	{
		return i;
		break;
	}
}

stock GetGarageObject(garageid) 
{
	return IsValidGarage(garageid) ? dGarage[garageid][oGarage][0] : INVALID_OBJECT_ID;
}

stock IsValidGarage(garageid)
{
	return MAX_GARAGES > garageid >= 0;
}

stock SetupGarages() 
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_GARAGES; i ++) dGarage[i][bGarage][0] = false;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(GARPATH, "LoadGarages_data");
    #endif
	for(new i = 0; i < MAX_GARAGES; i ++)
	{
	    if(dGarage[i][xGarage] == 0.0 && dGarage[i][yGarage] == 0.0 && dGarage[i][zGarage] == 0.0) dGarage[i][bGarage][0] = false;
		if(dGarage[i][bGarage][0])
		{
			CreateGarage(dGarage[i][xGarage], dGarage[i][yGarage], dGarage[i][zGarage], dGarage[i][aGarage], i);
		    new bool:bDoor = dGarage[i][bGarage][1];
			ChangeGarageDoorState(i, bDoor);
			dLastLoaded = i;
		}
	}
    LogInfo(true, "[INIT]Garages charges");
}

stock CheckObjectForMovement(objectid)
{
	for(new k = 0; k < MAX_GARAGES; k ++)
	{
		if(objectid == CA_GetObjectID(dGarage[k][oGarage][1]))
		{
			for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) if(IsPlayerConnected(i)) PlayerPlaySound(i, 1154, dGarage[k][xGarage], dGarage[k][yGarage], dGarage[k][zGarage]);
		    break;
		}
	}
}

stock IsGarageDoorOpen(garageid)
{
	return IsValidGarage(garageid) ? dGarage[garageid][bGarage][1] : false;
}

stock SetGaragePass(garageid, const pass[])
{
	if(IsValidGarage(garageid)) 
	{
		format(dGarage[garageid][sCodeGarage], 5, "%s", pass);
	}
}

stock GetGaragePass(garageid) 
{
	return IsValidGarage(garageid) ? dGarage[garageid][sCodeGarage] : "";
}
			    
//---TANKS---//
stock InitializeTanksVariables()
{
	for(new i = 0; i < MAX_TANKS; i ++)
	{
		dTanks[i][oTank][0] = INVALID_OBJECT_ID;
		dTanks[i][oTank][1] = INVALID_OBJECT_ID;
		//dTanks[i][oTank][2] = INVALID_OBJECT_ID;
		dTanks[i][dTankGas] = -1;
		dTanks[i][tTankFuel] = Text3D:INVALID_3DTEXT_ID;
		dTanks[i][xTank] = 0.0;
		dTanks[i][yTank] = 0.0;
		dTanks[i][zTank] = 0.0;
		dTanks[i][aTank] = 0.0;
	}
}

stock IsValidTank(tankid) 
{
	return MAX_TANKS > tankid >= 0;
}

stock GetTankObject(pTank[playerid])
{
	return IsValidTank ? dTanks[pTank[playerid]][oTank][0] : INVALID_OBJECT_ID;
}

stock SetupTanks() 
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_TANKS; i ++) dTanks[i][dTankGas] = -1;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(TKPATH, "LoadTanks_data");
    #endif
	for(new i = 0; i < MAX_TANKS; i ++)
	{
	    if(dTanks[i][xTank] == 0.0 && dTanks[i][yTank] == 0.0 && dTanks[i][zTank] == 0.0) dTanks[i][dTankGas] = -1;
		if(dTanks[i][dTankGas] != -1)
		{
			CreateTank(dTanks[i][xTank], dTanks[i][yTank], dTanks[i][zTank], dTanks[i][aTank], dTanks[i][dTankGas], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_TANKS; i ++) dTanks[i][dTankGas] = -1;
    LogInfo(true, "[INIT]Citernes charges");
}

stock CreateTank(Float:x, Float:y, Float:z, Float:angle, gas, load = -1)
{
	static slotid;
	if(load == -1)
	{
		if(dTanks[slotid][dTankGas] != -1)
		{
		    for(new i = 0; i < MAX_TANKS; i ++)
		    {
		        if(dTanks[slotid][dTankGas] == -1)
		        {
		            slotid = i;
		            break;
		        }
		    }
		}
		if(dTanks[slotid][dTankGas] != -1)
		{
			CA_DestroyObject_DC(dTanks[slotid][oTank][0]), dTanks[slotid][oTank][0] = INVALID_OBJECT_ID;
			DestroyDynamicObject(dTanks[slotid][oTank][1]), dTanks[slotid][oTank][1] = INVALID_OBJECT_ID;
			//CA_DestroyObject_DC(dTanks[slotid][oTank][2]), dTanks[slotid][oTank][2] = INVALID_OBJECT_ID;
		}
	}
	//---
	dTanks[(load == -1) ? slotid : load][xTank] = x;
	dTanks[(load == -1) ? slotid : load][yTank] = y;
	dTanks[(load == -1) ? slotid : load][zTank] = z;
	dTanks[(load == -1) ? slotid : load][aTank] = angle;
	if(load == -1) dTanks[slotid][dTankGas] = (load == -1) ? gas : 0;
	//---
	dTanks[(load == -1) ? slotid : load][tTankFuel] = CreateDynamic3DTextLabel("0.00l", KAKI, x, y, z, 15.0);
	UpdateTankInfo((load == -1) ? slotid : load);
	//---
	//---
	dTanks[(load == -1) ? slotid : load][oTank][0] = CA_CreateDynamicObject_DC(3287, x, y, z, 0.0, 0.0, angle, -1, -1, -1, 550.0, 550.0);
	new dObject = CA_GetObjectID(dTanks[(load == -1) ? slotid : load][oTank][0]);
	for(new i = 0; i < 30; i ++) SetDynamicObjectMaterial(dObject, i, 18633, "mattextures", "metalic128", 0xFFFFFFFF);
	//---
	dTanks[(load == -1) ? slotid : load][oTank][1] = CreateDynamicObject(2983, x + 5.23112 * floatsin(-(angle + 180.0/* - 89.6177*/), degrees), y + 5.23112 * floatcos(-(angle + 180.0/* - 89.6177*/), degrees), z - 0.794, 0.0, 0.0, angle, -1, -1, -1, 550.0, 550.0);
	//dTanks[(load == -1) ? slotid : load][oTank][2] = CA_CreateDynamicObject_DC(18633, x + 5.69694 * floatsin(-(angle/* - 89.0153*/), degrees), y + 5.69694 * floatsin(-(angle/* - 89.0153*/), degrees), z - 0.8451, 0.0, 0.0, angle, -1, -1, -1, 550.0, 550.0);
	//---
    slotid ++;
    if(slotid == MAX_TANKS) slotid = 0;
    return (slotid == 0) ? MAX_TANKS : slotid - 1;
}

DestroyTank(tankid)
{
	CA_DestroyObject_DC(dTanks[tankid][oTank][0]);
	dTanks[tankid][oTank][0] = INVALID_OBJECT_ID;
	DestroyDynamicObject(dTanks[tankid][oTank][1]);
	dTanks[tankid][oTank][1] = INVALID_OBJECT_ID;
	//CA_DestroyObject_DC(dTanks[tankid][oTank][2]);
	//dTanks[tankid][oTank][2] = INVALID_OBJECT_ID;
	//---
	DestroyDynamic3DTextLabel(dTanks[tankid][tTankFuel]);
	dTanks[tankid][tTankFuel] = Text3D:INVALID_3DTEXT_ID;
	//---
	dTanks[tankid][xTank] = 0.0;
	dTanks[tankid][yTank] = 0.0;
	dTanks[tankid][zTank] = 0.0;
	dTanks[tankid][aTank] = 0.0;
	dTanks[tankid][dTankGas] = -1;
}

public LoadTanks_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_TANKS; i ++)
	{
	    format(string, sizeof(string), "Citerne%d", i);
		INI_Int(string, dTanks[i][dTankGas]);
	    format(string, sizeof(string), "xCiterne%d", i);
		INI_Float(string, dTanks[i][xTank]);
	    format(string, sizeof(string), "yCiterne%d", i);
		INI_Float(string, dTanks[i][yTank]);
	    format(string, sizeof(string), "zCiterne%d", i);
		INI_Float(string, dTanks[i][zTank]);
	    format(string, sizeof(string), "aCiterne%d", i);
		INI_Float(string, dTanks[i][aTank]);
	}
	return 1;
}

SaveTanks()
{
	new string[50];
	new INI:File = INI_Open(TKPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_TANKS; i ++)
	{
	    format(string, sizeof(string), "Citerne%d", i);
		INI_WriteInt(File, string, dTanks[i][dTankGas]);
	    format(string, sizeof(string), "xCiterne%d", i);
		INI_WriteFloat(File, string, dTanks[i][xTank]);
	    format(string, sizeof(string), "yCiterne%d", i);
		INI_WriteFloat(File, string, dTanks[i][yTank]);
	    format(string, sizeof(string), "zCiterne%d", i);
		INI_WriteFloat(File, string, dTanks[i][zTank]);
	    format(string, sizeof(string), "aCiterne%d", i);
		INI_WriteFloat(File, string, dTanks[i][aTank]);
	}
	INI_Close(File);
}

IsPlayerNearTank(playerid)
{
	for(new i = 0; i < MAX_TANKS; i ++) if(dTanks[i][dTankGas] != -1 && IsPlayerInRangeOfPoint(playerid, 10.0, dTanks[i][xTank], dTanks[i][yTank], dTanks[i][zTank])) return i;
	return -1;
}

GetTankFuel(tankid)
{
	return dTanks[tankid][dTankGas];
}

UpdateTankInfo(tankid)
{
	new string[10];
	format(string, sizeof(string), "%.2f l", floatdiv(dTanks[tankid][dTankGas], 100));
	UpdateDynamic3DTextLabelText(dTanks[tankid][tTankFuel], KAKI, string);
}

GiveTankFuel(tankid, fuel)//Pour donner de l'essence ou en enlever d'une citerne
{
	if(dTanks[tankid][dTankGas] + fuel > MAX_TANK_FUEL) dTanks[tankid][dTankGas] = MAX_TANK_FUEL;//Si ça devient supérieur à 100, on lui met à 100
	else if(dTanks[tankid][dTankGas] + fuel < 0) dTanks[tankid][dTankGas] = 0;//Si ça devient inférieur à 0, on lui met à 0
    else dTanks[tankid][dTankGas] += fuel;//Sinon, on respecte la consigne originale
	UpdateTankInfo(tankid);
	return dTanks[tankid][dTankGas];
}

//---SEATS---//
InitializeSeatVariables()
{
	for(new i = 0; i < MAX_SEATS; i ++)
	{
		dSeat[i][dSeatType] = -1;
		dSeat[i][oSeat] = INVALID_OBJECT_ID;
		dSeat[i][xSeat] = 0.0;
		dSeat[i][ySeat] = 0.0;
		dSeat[i][zSeat] = 0.0;
		dSeat[i][aSeat] = 0.0;
	}
}
CreateSeat(type, Float:x, Float:y, Float:z, Float:angle, load = -1)
{
	//18122486.114, 1038.588, 56.5019, 0.0, 0.0, 0.0
	static slotid;
	if(type != 0)
	{
		if(load == -1)
		{
			if(dSeat[slotid][dSeatType] != 0)
			{
			    for(new i = 0; i < MAX_SEATS; i ++)
			    {
			        if(dSeat[i][dSeatType] == 0)
			        {
			            slotid = i;
			            break;
			        }
			    }
			}
			if(dSeat[slotid][dSeatType] != 0) DestroyDynamicObject(dSeat[slotid][oSeat]);
		}
		//---
		dSeat[(load == -1) ? slotid : load][xSeat] = x;
		dSeat[(load == -1) ? slotid : load][ySeat] = y;
		dSeat[(load == -1) ? slotid : load][zSeat] = z;
		dSeat[(load == -1) ? slotid : load][aSeat] = angle;
		//---
		dSeat[(load == -1) ? slotid : load][dSeatType] = type;
		switch(type)
		{
			case 1: z -= 1.0;
		}
		//---
		new dSeats[] = {1729, 0};
		//---
		dSeat[(load == -1) ? slotid : load][oSeat] = CreateDynamicObject(dSeats[type - 1], x, y, z, 0.0, 0.0, angle);
	    slotid ++;
    }
    if(slotid == MAX_SEATS) slotid = 0;
    return (slotid == 0) ? MAX_SEATS : slotid - 1;
}

DestroySeat(seatid)
{
	DestroyDynamicObject(dSeat[seatid][oSeat]);
	dSeat[seatid][oSeat] = INVALID_OBJECT_ID;
	dSeat[seatid][dSeatType] = 0;
	dSeat[seatid][xSeat] = 0.0;
	dSeat[seatid][ySeat] = 0.0;
	dSeat[seatid][zSeat] = 0.0;
	dSeat[seatid][aSeat] = 0.0;
}

IsPlayerNearSeat(playerid)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	for(new i = 0; i < MAX_SEATS; i ++)
	{
		if(dSeat[i][dSeatType] != 0 && IsPlayerInRangeOfPoint(playerid, 3.0, dSeat[i][xSeat], dSeat[i][ySeat], dSeat[i][zSeat]))
		{
		    if(IsColBetweenTwoPos(x, y, z, dSeat[i][xSeat], dSeat[i][ySeat], dSeat[i][zSeat])) continue;
			return i;
		}
	}
	return -1;
}

public LoadSeats_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_SEATS; i ++)
	{
	    format(string, sizeof(string), "SiegeID%d", i);
		INI_Int(string, dSeat[i][dSeatType]);
	    format(string, sizeof(string), "xSiege%d", i);
		INI_Float(string, dSeat[i][xSeat]);
	    format(string, sizeof(string), "ySiege%d", i);
		INI_Float(string, dSeat[i][ySeat]);
	    format(string, sizeof(string), "zSiege%d", i);
		INI_Float(string, dSeat[i][zSeat]);
	    format(string, sizeof(string), "aSiege%d", i);
		INI_Float(string, dSeat[i][aSeat]);
	}
	return 1;
}

SaveSeats()
{
	new string[50];
	new INI:File = INI_Open(SEPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_SEATS; i ++)
	{
	    format(string, sizeof(string), "SiegeID%d", i);
		INI_WriteInt(File,string, dSeat[i][dSeatType]);
	    format(string, sizeof(string), "xSiege%d", i);
		INI_WriteFloat(File,string, dSeat[i][xSeat]);
	    format(string, sizeof(string), "ySiege%d", i);
		INI_WriteFloat(File,string, dSeat[i][ySeat]);
	    format(string, sizeof(string), "zSiege%d", i);
		INI_WriteFloat(File,string, dSeat[i][zSeat]);
	    format(string, sizeof(string), "aSiege%d", i);
		INI_WriteFloat(File,string, dSeat[i][aSeat]);
	}
	INI_Close(File);
}

stock FindNearbySeats(playerid, Float:x, Float:y, Float:z, slot, tInArray[][][])
{
	new Float:fTrash;
	for(new i = 0; i < MAX_SEATS; i ++)
	{
	    if(slot == 9) break;
		if(dSeat[i][dSeatType] == 0) continue;
		if(IsPlayerInRangeOfPoint(playerid, 3.0, dSeat[i][xSeat], dSeat[i][ySeat], dSeat[i][zSeat]))
		{
			if(CA_RayCastLine(x, y, z, dSeat[i][xSeat], dSeat[i][ySeat], dSeat[i][zSeat], fTrash, fTrash, fTrash) != 0) continue;
		    tInArray[playerid][dSlot][0] = i;
		    tInArray[playerid][dSlot][1] = 11;
			slot ++;
		}
	}
}

stock IsValidSeat(seatid)
{
	return MAX_SEATS > seatid >= 0;
}

stock PickupSeat(seatid)
{
	if(IsValidSeat(seatid))
	{
		switch(GetSeatType(seatid))
		{
			case 1:
			{
				GivePlayerSlotObject(playerid, 155, dFreeSlot);
			}
		}
	}
}

stock SetupSeats()
{
	//INITIALISATION
	new dLastLoaded = 0;
	for(new i = 0; i < MAX_SEATS; i ++) dSeat[i][dSeatType] = 0;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(SEPATH, "LoadSeats_data");
    #endif
	for(new i = 0; i < MAX_SEATS; i ++)
	{
	    if(dSeat[i][xSeat] == 0.0 && dSeat[i][ySeat] == 0.0 && dSeat[i][zSeat] == 0.0) dSeat[i][dSeatType] = 0;
		if(dSeat[i][dSeatType] != 0)
		{
			CreateSeat(dSeat[i][dSeatType], dSeat[i][xSeat], dSeat[i][ySeat], dSeat[i][zSeat], dSeat[i][aSeat], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_SEATS; i ++) dSeat[i][dSeatType] = 0;
    LogInfo(true, "[INIT]Fauteuils charges");
}

stock SetPlayerOnSeat(playerid, seatid)
{
	switch(GetSeatType(seatid))
	{
		case 1:
		{
			new Float:angle = dSeat[seatid][aSeat] - 180.0;
			SetPlayerPos(playerid, dSeat[seatid][xSeat] + 0.6 * floatsin(-angle, degrees), dSeat[seatid][ySeat] + 0.6 * floatcos(-angle, degrees), dSeat[seatid][zSeat]);
			SetPlayerFacingAngle(playerid, angle);
			ApplyAnimation(playerid,"PED","SEAT_IDLE",4,0,0,0, 1,0,1);
			InterpolateCameraPos(playerid, dSeat[seatid][xSeat] - 3.0 * floatsin(-(angle - 45), degrees), dSeat[seatid][ySeat] - 3.0 * floatcos(-(angle - 45), degrees), dSeat[seatid][zSeat] + 1.0, dSeat[seatid][xSeat] - 3.0 * floatsin(-(angle + 45), degrees), dSeat[seatid][ySeat] - 3.0 * floatcos(-(angle + 45), degrees), dSeat[seatid][zSeat] + 1.0, 35000);
			InterpolateCameraLookAt(playerid, dSeat[seatid][xSeat], dSeat[seatid][ySeat], dSeat[seatid][zSeat], dSeat[seatid][xSeat], dSeat[seatid][ySeat], dSeat[seatid][zSeat], 35000);
		}
	}
}

stock GetSeatType(seatid)
{
	return IsValidSeat(seatid) ? dSeat[seatid][dSeatType] : 0;
}

//---FURN---//
stock InitializeFurnVariables() 
{
	for(new i = 0; i < MAX_FURN; i ++)
	{
		dFurn[i][dFurnitureID] = 0;
		dFurn[i][oFurniture] = INVALID_OBJECT_ID;
		dFurn[i][xFurn] = 0.0;
		dFurn[i][yFurn] = 0.0;
		dFurn[i][zFurn] = 0.0;
		dFurn[i][rxFurn] = 0.0;
		dFurn[i][ryFurn] = 0.0;
		dFurn[i][rzFurn] = 0.0;
	}
}

public LoadFurniture_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_FURN; i ++)
	{
	    format(string, sizeof(string), "Deco%d", i);
		INI_Int(string, dFurn[i][dFurnitureID]);
	    format(string, sizeof(string), "xFurn%d", i);
		INI_Float(string, dFurn[i][xFurn]);
	    format(string, sizeof(string), "yFurn%d", i);
		INI_Float(string, dFurn[i][yFurn]);
	    format(string, sizeof(string), "zFurn%d", i);
		INI_Float(string, dFurn[i][zFurn]);
	    format(string, sizeof(string), "rxFurn%d", i);
		INI_Float(string, dFurn[i][rxFurn]);
	    format(string, sizeof(string), "ryFurn%d", i);
		INI_Float(string, dFurn[i][ryFurn]);
	    format(string, sizeof(string), "rzFurn%d", i);
		INI_Float(string, dFurn[i][rzFurn]);
	}
	return 1;
}

SaveFurniture()
{
	new string[50];
	new INI:File = INI_Open(FUPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_FURN; i ++)
	{
	    format(string, sizeof(string), "Deco%d", i);
		INI_WriteInt(File, string, dFurn[i][dFurnitureID]);
	    format(string, sizeof(string), "xFurn%d", i);
		INI_WriteFloat(File, string, dFurn[i][xFurn]);
	    format(string, sizeof(string), "yFurn%d", i);
		INI_WriteFloat(File, string, dFurn[i][yFurn]);
	    format(string, sizeof(string), "zFurn%d", i);
		INI_WriteFloat(File, string, dFurn[i][zFurn]);
	    format(string, sizeof(string), "rxFurn%d", i);
		INI_WriteFloat(File, string, dFurn[i][rxFurn]);
	    format(string, sizeof(string), "ryFurn%d", i);
		INI_WriteFloat(File, string, dFurn[i][ryFurn]);
	    format(string, sizeof(string), "rzFurn%d", i);
		INI_WriteFloat(File, string, dFurn[i][rzFurn]);
	}
	INI_Close(File);
}

CreateFurniture(objectid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, load = -1)
{
	static slotid;
	if(objectid != 0)
	{
		if(load == -1)
		{
			if(dFurn[slotid][dFurnitureID] != 0)
			{
			    for(new i = 0; i < MAX_FURN; i ++)
			    {
			        if(dFurn[i][dFurnitureID] == 0)
			        {
			            slotid = i;
			            break;
			        }
			    }
			}
			if(dFurn[slotid][dFurnitureID] != 0) DestroyDynamicObject(dFurn[slotid][oFurniture]);
		}
		//---
		dFurn[(load == -1) ? slotid : load][xFurn] = x;
		dFurn[(load == -1) ? slotid : load][yFurn] = y;
		dFurn[(load == -1) ? slotid : load][zFurn] = z;
		dFurn[(load == -1) ? slotid : load][rxFurn] = rx;
		dFurn[(load == -1) ? slotid : load][ryFurn] = ry;
		dFurn[(load == -1) ? slotid : load][rzFurn] = rz;
		//---
		dFurn[(load == -1) ? slotid : load][dFurnitureID] = objectid;
		//---
		dFurn[(load == -1) ? slotid : load][oFurniture] = CreateDynamicObject(GetFurnitureObjectID(objectid, false), x, y, z, rx, ry, rz);
	    slotid ++;
    }
    if(slotid == MAX_FURN) slotid = 0;
    return (slotid == 0) ? MAX_FURN : slotid - 1;
}

PlayerSetFurniture(playerid, furnitureid)
{
	new Float:x, Float:y, Float:z, Float:angle;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, angle);
	SetPlayerFurn(playerid, CreateFurniture(furnitureid, x, y, z, 0.0, 0.0, angle, -1));
	EditDynamicObject(playerid, dFurn[GetPlayerFurn(playerid)][oFurniture]);
}

DestroyFurniture(furnid)
{
	DestroyDynamicObject(dFurn[furnid][oFurniture]);
	dFurn[furnid][oFurniture] = INVALID_OBJECT_ID;
	dFurn[furnid][dFurnitureID] = 0;
	dFurn[furnid][xFurn] = 0.0;
	dFurn[furnid][yFurn] = 0.0;
	dFurn[furnid][zFurn] = 0.0;
	dFurn[furnid][rxFurn] = 0.0;
	dFurn[furnid][ryFurn] = 0.0;
	dFurn[furnid][rzFurn] = 0.0;
}

/*IsPlayerNearFurniture(playerid)
{
	new Float:x, Float:y, Float:z, Float:fTrash;
	GetPlayerPos(playerid, x, y, z);
	for(new i = 0; i < MAX_FURN; i ++)
	{
		if(dFurn[i][dObjectID] != 0 && IsPlayerInRangeOfPoint(playerid, 3.0, dFurn[i][xFurn], dFurn[i][yFurn], dFurn[i][zFurn]))
		{
		    if(CA_RayCastLine(x, y, z, dFurn[i][xFurn], dFurn[i][yFurn], dFurn[i][zFurn], fTrash, fTrash, fTrash) != 0) continue;
			return i;
		}
	}
	return -1;
}*/

GetFurnitureObjectID(furnid, bool:item)
{
	switch(furnid)
	{
		case 1: return item ? 137 : 19279;
		case 2: return item ? 143 : 1736;
		case 3: return item ? 144 : 1828;
	}
	return 0;
}

stock FindNearbyFurn(playerid, Float:x, Float:y, Float:z, slot, tInArray[][][])
{
	new Float:fTrash;
	for(new i = 0; i < MAX_FURN; i ++)
	{
	    if(slot == 9) break;
	    if(dFurn[i][dFurnitureID] == 0) continue;
	    if(IsPlayerInRangeOfPoint(playerid, 3.0, dFurn[i][xFurn], dFurn[i][yFurn], dFurn[i][zFurn]))
		{
			if(CA_RayCastLine(x, y, z, dFurn[i][xFurn], dFurn[i][yFurn], dFurn[i][zFurn], fTrash, fTrash, fTrash) != 0) continue;
		    tInArray[playerid][slot][0] = i;
		    tInArray[playerid][slot][1] = 10;
			slot ++;
		}
	}
}

stock IsValidFurn(furnid) 
{
	return MAX_FURN > furnid >= 0;
}

stock GetFurnitureID(furnid)
{
	return IsValidFurn(furnid) ? dFurn[furnid][dFurnitureID] : 0;
}

stock SetupFurn()
{
	//INITIALISATION
	new dLastLoaded = 0;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(FUPATH, "LoadFurniture_data");
    #endif
	for(new i = 0; i < MAX_FURN; i ++)
	{
		if(dFurn[i][dFurnitureID] > 0)
		{
			CreateFurniture(dFurn[i][dFurnitureID], dFurn[i][xFurn], dFurn[i][yFurn], dFurn[i][zFurn], dFurn[i][rxFurn], dFurn[i][ryFurn], dFurn[i][rzFurn], i);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_FURN; i ++) dFurn[i][dFurnitureID] = 0;
    LogInfo(true, "[INIT]Decorations charges");
}