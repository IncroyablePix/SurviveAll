#if defined VEHICLES_INCLUDED

	#endinput

#endif

#define VEHICLES_INCLUDED

//---INCLUDES---//
#include <a_samp>
#include <[SA]Defines.inc>
#include <[SA]Items.inc>

//---DEFINES---//
#undef MAX_VEHICLES
#define MAX_VEHICLES                        (200)
#define MAX_SPAWN_VEHICLES                  (66)

#define VPATH                               "/Survive-All/General/Vehicles.ini"
#define GASPATH                             "/Survive-All/General/GasStations.ini"

//---ESSENCE
#define BIKE_FUEL                       	(2000)
#define CAR_FUEL                        	(5000)
#define TRUCK_FUEL                      	(10000)
#define BOAT_FUEL                       	(10000)
#define PLANE_FUEL                      	(250000)
#define HELI_FUEL                       	(250000)

//---FORWARDS---//
forward LoadVehicles_data(name[], value[]);
forward LoadGasStations_data(name[], value[]);
forward CreateRandomVehicle();
forward DestroyVeh(vehicleid);
forward SetVehicleBonnetState(vehicleid, bool:open);
forward SetVehicleTrunkState(vehicleid, bool:open);
forward bool:IsPlayerNearVehicle(playerid);
forward bool:GetVehicleWheelState(vehicleid, wheelid);

//---ENUMS---//
enum VehicleInfos
{
	dVehicleID,
	dVehicleModel,
	dWheels,
	dItem,
	dFuel,
	Float:fHealth,
	bool:bEngine,
	bool:bWheel[4],
	Float:xVeh,
	Float:yVeh,
	Float:zVeh,
	Float:aVeh,
	TrunkObject[6],
	dColor[2]
}

enum GasStation
{
	Text3D:tGasText,
	dStationGas
}

//---VARIABLES---//
new dVehicleInfos[MAX_SPAWN_VEHICLES][VehicleInfos];
new dRepair[MAX_PLAYERS][4];
new dGasStation[19][GasStation];

//---FONCTIONS---//
/**
	<summary>
        Initialise les variables des entités véhicule.
    </summary>

    <param>
        /
    </param>

    <returns>
        void
    </returns>
*/
InitializeVehiclesVariables() 
{
	for(new i = 0; i < MAX_SPAWN_VEHICLES; i ++)
	{
	    dVehicleInfos[i][dVehicleID] = INVALID_VEHICLE_ID;
	    dVehicleInfos[i][dVehicleModel] = 0;
	    dVehicleInfos[i][dWheels] = 0;
	    dVehicleInfos[i][dItem] = 0;
	    dVehicleInfos[i][dFuel] = 0;
	    dVehicleInfos[i][fHealth] = 1000.0;
	    dVehicleInfos[i][bEngine] = false;
	    for(new j = 0; j < 4; j ++) dVehicleInfos[i][bWheel][j] = false;
	    dVehicleInfos[i][xVeh] = 0.0;
	    dVehicleInfos[i][yVeh] = 0.0;
	    dVehicleInfos[i][zVeh] = 0.0;
	    dVehicleInfos[i][aVeh] = 0.0;
	    for(new j = 0; j < 6; j ++) dVehicleInfos[i][TrunkObject][j] = 0;
		dVehicleInfos[i][dColor][0] = 86;
		dVehicleInfos[i][dColor][1] = 86;
	}
}

/**
	<summary>
        Charge tous les véhicules
    </summary>

    <returns>
		void
    </returns>
*/
stock SetupVehicles() 
{
 	#if defined LOAD_DYNAMICS
	INI_ParseFile(VPATH, "LoadVehicles_data");
	#endif
    for(new i = 0; i < MAX_SPAWN_VEHICLES; i ++)
	{
	    if(dVehicleInfos[i][dVehicleModel] != 0)
		{
			dVehicleInfos[i][dVehicleID] = CreateVehicle(dVehicleInfos[i][dVehicleModel], dVehicleInfos[i][xVeh], dVehicleInfos[i][yVeh], dVehicleInfos[i][zVeh],dVehicleInfos[i][aVeh], dVehicleInfos[i][dColor][0],dVehicleInfos[i][dColor][1], -1, false);
			SetVehicleEngineState(dVehicleInfos[i][dVehicleID], false);
			if(dVehicleInfos[i][fHealth] < 250.0) dVehicleInfos[i][fHealth] = 251.0;
			SetVehicleHealth(i, dVehicleInfos[i][fHealth]);
			SetVehicleWheels(i);
		}
	}
    LogInfo(true, "[INIT]Vehicules charges");
}

/**
	<summary>
        Charge les véhicules à partir du fichier
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
public LoadVehicles_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_SPAWN_VEHICLES; i ++)
	{
	    format(string, sizeof(string), "VehicleID%d", i);
		INI_Int(string, dVehicleInfos[i][dVehicleModel]);
	    format(string, sizeof(string), "Wheels%d", i);
		INI_Int(string, dVehicleInfos[i][dWheels]);
	    format(string, sizeof(string), "Items%d", i);
		INI_Int(string, dVehicleInfos[i][dItem]);
	    format(string, sizeof(string), "Fuel%d", i);
		INI_Int(string, dVehicleInfos[i][dFuel]);
	    format(string, sizeof(string), "Health%d", i);
		INI_Float(string, dVehicleInfos[i][fHealth]);
		//---
	    format(string, sizeof(string), "Engine%d", i);
		INI_Bool(string, dVehicleInfos[i][bEngine]);
	    format(string, sizeof(string), "WheelOne%d", i);
		INI_Bool(string, dVehicleInfos[i][bWheel][0]);
	    format(string, sizeof(string), "WheelTwo%d", i);
		INI_Bool(string, dVehicleInfos[i][bWheel][1]);
	    format(string, sizeof(string), "WheelThree%d", i);
		INI_Bool(string, dVehicleInfos[i][bWheel][2]);
	    format(string, sizeof(string), "WheelFour%d", i);
		INI_Bool(string, dVehicleInfos[i][bWheel][3]);
		//---
	    format(string, sizeof(string), "xVeh%d", i);
		INI_Float(string, dVehicleInfos[i][xVeh]);
	    format(string, sizeof(string), "yVeh%d", i);
		INI_Float(string, dVehicleInfos[i][yVeh]);
	    format(string, sizeof(string), "zVeh%d", i);
		INI_Float(string, dVehicleInfos[i][zVeh]);
	    format(string, sizeof(string), "aVeh%d", i);
		INI_Float(string, dVehicleInfos[i][aVeh]);
		//---
	    format(string, sizeof(string), "ObjectOne%d", i);
		INI_Int(string, dVehicleInfos[i][TrunkObject][0]);
	    format(string, sizeof(string), "ObjectTwo%d", i);
		INI_Int(string, dVehicleInfos[i][TrunkObject][1]);
	    format(string, sizeof(string), "ObjectThree%d", i);
		INI_Int(string, dVehicleInfos[i][TrunkObject][2]);
	    format(string, sizeof(string), "ObjectFour%d", i);
		INI_Int(string, dVehicleInfos[i][TrunkObject][3]);
	    format(string, sizeof(string), "ObjectFive%d", i);
		INI_Int(string, dVehicleInfos[i][TrunkObject][4]);
	    format(string, sizeof(string), "ObjectSix%d", i);
		INI_Int(string, dVehicleInfos[i][TrunkObject][5]);
		//---
	    format(string, sizeof(string), "ColorOne%d", i);
		INI_Int(string, dVehicleInfos[i][dColor][0]);
	    format(string, sizeof(string), "ColorTwo%d", i);
		INI_Int(string, dVehicleInfos[i][dColor][1]);
	}
	return 1;
}

/**
	<summary>
        Sauvegarde les informations concernant les véhicules dans un fichier
    </summary>

    <returns>
		void
    </returns>
*/
SaveVehicles()
{
	new string[50];
	new INI:File = INI_Open(VPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_SPAWN_VEHICLES; i ++)
	{
	    format(string, sizeof(string), "VehicleID%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][dVehicleModel]);
	    format(string, sizeof(string), "Wheels%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][dWheels]);
	    format(string, sizeof(string), "Items%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][dItem]);
	    format(string, sizeof(string), "Fuel%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][dFuel]);
	    format(string, sizeof(string), "Health%d", i);
		INI_WriteFloat(File,string, dVehicleInfos[i][fHealth]);
		//---
	    format(string, sizeof(string), "Engine%d", i);
		INI_WriteBool(File,string, dVehicleInfos[i][bEngine]);
	    format(string, sizeof(string), "WheelOne%d", i);
		INI_WriteBool(File,string, dVehicleInfos[i][bWheel][0]);
	    format(string, sizeof(string), "WheelTwo%d", i);
		INI_WriteBool(File,string, dVehicleInfos[i][bWheel][1]);
	    format(string, sizeof(string), "WheelThree%d", i);
		INI_WriteBool(File,string, dVehicleInfos[i][bWheel][2]);
	    format(string, sizeof(string), "WheelFour%d", i);
		INI_WriteBool(File,string, dVehicleInfos[i][bWheel][3]);
		//---
	    format(string, sizeof(string), "xVeh%d", i);
		INI_WriteFloat(File,string, dVehicleInfos[i][xVeh]);
	    format(string, sizeof(string), "yVeh%d", i);
		INI_WriteFloat(File,string, dVehicleInfos[i][yVeh]);
	    format(string, sizeof(string), "zVeh%d", i);
		INI_WriteFloat(File,string, dVehicleInfos[i][zVeh]);
	    format(string, sizeof(string), "aVeh%d", i);
		INI_WriteFloat(File,string, dVehicleInfos[i][aVeh]);
		//---
	    format(string, sizeof(string), "ObjectOne%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][TrunkObject][0]);
	    format(string, sizeof(string), "ObjectTwo%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][TrunkObject][1]);
	    format(string, sizeof(string), "ObjectThree%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][TrunkObject][2]);
	    format(string, sizeof(string), "ObjectFour%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][TrunkObject][3]);
	    format(string, sizeof(string), "ObjectFive%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][TrunkObject][4]);
	    format(string, sizeof(string), "ObjectSix%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][TrunkObject][5]);
		//---
	    format(string, sizeof(string), "ColorOne%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][dColor][0]);
	    format(string, sizeof(string), "ColorTwo%d", i);
		INI_WriteInt(File,string, dVehicleInfos[i][dColor][1]);
	}
	INI_Close(File);
}

/**
	<summary>
        Rajoute ou retire de l'essence à un véhicule
    </summary>

    <param name="vehicleid">
		int - ID du véhicule
    </param>

    <param name="amount">
		int - Quantité d'essence à ajouter
    </param>

    <returns>
		int - Quantité d'essence dans le véhicule
    </returns>
*/
GiveVehicleFuel(vehicleid, amount)//Pour donner de l'essence ou en enlever
{
	if(dVehicleInfos[vehicleid][dFuel] + amount > GetVehicleMaxFuel(vehicleid)) dVehicleInfos[vehicleid][dFuel] = GetVehicleMaxFuel(vehicleid);//Si ça devient supérieur à 100, on lui met à 100
	else if(dVehicleInfos[vehicleid][dFuel] + amount < 0) dVehicleInfos[vehicleid][dFuel] = 0;//Si ça devient inférieur à 0, on lui met à 0
    else dVehicleInfos[vehicleid][dFuel] += amount;//Sinon, on respecte la consigne originale

    if(dVehicleInfos[vehicleid][dFuel] == 0)//Si le véhicule a 0 d'essence, il s'éteint
 	{
	    SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
	}
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) 
    {
        if(GetPlayerVehicleID(i) == dVehicleInfos[vehicleid][dVehicleID] && GetPlayerVehicleSeat(i) == 0) 
        {
            UpdateInfo(i, 10);
        }
    }
	return dVehicleInfos[vehicleid][dFuel];
}

/**
	<summary>
        Diminution automatique de l'essence du véhicule (en conduisant)
    </summary>

    <param name="vehicleid">
		int - ID du véhicule
    </param>

    <returns>
		void
    </returns>
*/
DecreaseVehicleFuel(vehicleid)
{
	new Float:fSpeed = GetVehicleSpeed(dVehicleInfos[vehicleid][dVehicleID]);
	switch(GetVehicleFuelType(dVehicleInfos[vehicleid][dVehicleModel]))
	{
    	case 1: GiveVehicleFuel(vehicleid, -(floatround(floatdiv(fSpeed, 35), floatround_round)));//Motos
  		case 2: GiveVehicleFuel(vehicleid, -(floatround(floatdiv(fSpeed, 35), floatround_round)));//Voitures
  		case 3: GiveVehicleFuel(vehicleid, -(floatround(floatdiv(fSpeed, 35), floatround_round)));//Camions & Utilitaires
  		case 4: GiveVehicleFuel(vehicleid, -(floatround(floatdiv(fSpeed, 35), floatround_round)));//Bateaux
  		case 5: GiveVehicleFuel(vehicleid, -3 * (floatround(floatdiv(fSpeed, 35), floatround_round)));//Avions
  		case 6: GiveVehicleFuel(vehicleid, -2 * (floatround(floatdiv(fSpeed, 35), floatround_round)));//Hélicos
	}
}

/**
	<summary>
        Gère le temps de réparation des véhicules
    </summary>

    <param name="playerid">
		int - ID du joueur
    </param>

    <returns>
		int - 1
    </returns>
*/
UpdateRepairTimer(playerid)
{
	if(dRepair[playerid][0] != 0)
	{
		dRepair[playerid][0] --;
		//---
		if(dRepair[playerid][0] == 0)
		{
			ShowPlayerTextInfo(playerid, 5000, "~r~You failed to repair your engine!", "~r~Vous avez échoué votre réparation !", "~r~¡Espagnol!", "~r~Portugais!", "~r~Italien!", "~r~Allemand!");
			TogglePlayerControllable(playerid, true);
			SetVehicleBonnetState(dRepair[playerid][3], false);
			dRepair[playerid][1] = -1;
			dRepair[playerid][2] = 0;
			dRepair[playerid][3] = -1;
  		}
		else if(0 < dRepair[playerid][0] < 15)
		{
			new Float:x, Float:y, Float:z;
			GetVehiclePos(dRepair[playerid][3], x, y, z);
			if(!IsPlayerInRangeOfPoint(playerid, 11.0, x, y, z))
			{
				ShowPlayerTextInfo(playerid, 5000, "~r~The vehicle is too far from you!", "~r~Le véhicule est trop loin de vous !", "~r~¡Espagnol!", "~r~Portugais!", "~r~Italien!", "~r~Allemand!");
				TogglePlayerControllable(playerid, true);
				SetVehicleBonnetState(dRepair[playerid][3], false);
				dRepair[playerid][0] = 0;
				dRepair[playerid][1] = -1;
				dRepair[playerid][2] = 0;
				dRepair[playerid][3] = -1;
				return 1;
			}
			ApplyAnimation(playerid, "RAPPING", "RAP_A_Loop", 4.0, 0, 1, 1, 1, 0);
			if(Success(45))
   			{
			   switch(RandomEx(0, 4))
				{
    				case 0://S'accroupir
					{
						if(dRepair[playerid][1] != -1) HidePlayerTextInfo(playerid);
      					dRepair[playerid][1] = 0;
						ShowPlayerTextInfo(playerid, -1, "~r~Hit ~w~~k~~PED_DUCK~~r~!", "~r~Appuyez sur ~w~~k~~PED_DUCK~ ~r~ !", "~r~¡Prensa usted ~w~~k~~PED_DUCK~~r~!", "~r~Imprensa ~w~~k~~PED_DUCK~ ~r~!", "~r~Italien ~w~~k~~PED_DUCK~ ~r~!", "~r~Drücken sie auf ~w~~k~~PED_DUCK~~r~!");
					}
     				case 1://No
					{
						if(dRepair[playerid][1] != -1) HidePlayerTextInfo(playerid);
      					dRepair[playerid][1] = 1;
						ShowPlayerTextInfo(playerid, -1, "~r~Hit ~w~~k~~CONVERSATION_NO~~r~!", "~r~Appuyez sur ~w~~k~~CONVERSATION_NO~ ~r~ !", "~r~¡Prensa usted ~w~~k~~CONVERSATION_NO~~r~!", "~r~Imprensa ~w~~k~~CONVERSATION_NO~ ~r~!", "~r~Italien ~w~~k~~CONVERSATION_NO~ ~r~!", "~r~Drücken sie auf ~w~~k~~CONVERSATION_NO~~r~!");
					}
     				case 2://Yes
					{
						if(dRepair[playerid][1] != -1) HidePlayerTextInfo(playerid);
					 	dRepair[playerid][1] = 2;
						ShowPlayerTextInfo(playerid, -1, "~r~Hit ~w~~k~~CONVERSATION_YES~~r~!", "~r~Appuyez sur ~w~~k~~CONVERSATION_YES~ ~r~ !", "~r~¡Prensa usted ~w~~k~~CONVERSATION_YES~~r~!", "~r~Imprensa ~w~~k~~CONVERSATION_YES~ ~r~!", "~r~Italien ~w~~k~~CONVERSATION_YES~ ~r~!", "~r~Drücken sie auf ~w~~k~~CONVERSATION_YES~~r~!");
					}
					case 3://H
					{
						if(dRepair[playerid][1] != -1) HidePlayerTextInfo(playerid);
						dRepair[playerid][1] = 3;
						ShowPlayerTextInfo(playerid, -1, "~r~Hit ~w~~k~~GROUP_CONTROL_BWD~~r~!", "~r~Appuyez sur ~w~~k~~GROUP_CONTROL_BWD~ ~r~ !", "~r~¡Prensa usted ~w~~k~~GROUP_CONTROL_BWD~~r~!", "~r~Imprensa ~w~~k~~GROUP_CONTROL_BWD~ ~r~!", "~r~Italien ~w~~k~~GROUP_CONTROL_BWD~ ~r~!", "~r~Drücken sie auf ~w~~k~~GROUP_CONTROL_BWD~~r~!");
					}
				}
			}
		}
	}
	return 1;
}

/**
	<summary>
        Crée une onde de choc pour les véhicules environnants
    </summary>

    <param name="playerid">
		int - ID du joueur responsable
    </param>

    <param name="x">
		Float - Position X de l'explosion
    </param>

    <param name="y">
		Float - Position Y de l'explosion
    </param>

    <param name="z">
		Float - Position Z de l'explosion
    </param>

    <param name="radius">
		Float - Rayon d'action
    </param>

    <returns>
		void
    </returns>
*/
ShockWaveVehicles(playerid, Float:x, Float:y, Float:z, Float:radius)
{
	new Float:dist;
	for(new i = 1; i < MAX_VEHICLES + 1; i ++)
	{
	    if(!IsValidVehicle(i)) continue;
		new Float:vx, Float:vy, Float:vz;
		GetVehiclePos(i, vx, vy, vz);
	    if(IsColBetweenTwoPos(x, y, z, vx, vy, vz)) continue;
	    dist = GetDistanceBetweenPoints(vx, vy, vz, x, y, z);
        
	    if(dist > radius) continue;
	    else if(dist < radius * 0.4) ShockWaveVehicle(playerid, i, GetZAngleBetweenPos(x, y, vx, vy), 5);
		else if(dist < radius * 0.6) ShockWaveVehicle(playerid, i, GetZAngleBetweenPos(x, y, vx, vy), 4);
		else if(dist < radius * 0.8) ShockWaveVehicle(playerid, i, GetZAngleBetweenPos(x, y, vx, vy), 3);
		else if(dist < radius * 1.0) ShockWaveVehicle(playerid, i, GetZAngleBetweenPos(x, y, vx, vy), 2);
		else if(dist < radius * 1.5) ShockWaveVehicle(playerid, i, GetZAngleBetweenPos(x, y, vx, vy), 1);
	}
}

/**
	<summary>
        Action de l'onde de choc sur un véhicule
    </summary>

    <param name="playerid">
		int - ID du joueur responsable
    </param>

    <param name="vehicleid">
		int - ID du véhicule à impacter
    </param>

    <param name="atan2">
		Float - Angle de répulsion
    </param>

    <param name="strength">
		int - Force de projection
    </param>

    <returns>
		void
    </returns>
*/
ShockWaveVehicle(playerid, vehicleid, Float:atang2, strenght)
{
	#pragma unused playerid
	new Float:vx, Float:vy, Float:vz, Float:angle;
	GetVehicleVelocity(vehicleid, vx, vy, vz);
	//---
	new Float:fPower;
	switch(strenght)
	{
		case 1:
		{
		    GiveVehicleHealth(vehicleid, -100.0);
			fPower = 0.05;
		}
		case 2:
		{
		    GiveVehicleHealth(vehicleid, -250.0);
			fPower = 0.12;
		}
		case 3:
		{
		    GiveVehicleHealth(vehicleid, -400.0);
			fPower = 0.17;
		}
		case 4:
		{
		    GiveVehicleHealth(vehicleid, -600.0);
			fPower = 0.25;
		}
		case 5:
		{
		    GiveVehicleHealth(vehicleid, -800.0);
			fPower = 0.35;
		}
	}
	//---
	SetVehicleVelocity(vehicleid, vx + fPower * floatsin(angle, degrees), vy + fPower * floatcos(angle, degrees), vz);
	SetVehicleAngularVelocity(vehicleid, fPower * floatsin(atang2, degrees), fPower * floatcos(atang2, degrees), fPower);
}

/**
	<summary>
        Permet de récupérer un véhicule proche d'une position
    </summary>

    <param name="x1">
		Float - Position X de l'explosion
    </param>

    <param name="y1">
		Float - Position Y de l'explosion
    </param>

    <param name="z1">
		Float - Position Z de l'explosion
    </param>

    <param name="dist">
		Float - Rayon à vérifier
    </param>

    <param name="vehi">
		int - Variable pour stocker le véhicule trouvé
    </param>

    <param name="strength">
		int - Force de projection
    </param>

    <returns>
		void
    </returns>
*/
stock GetVehicleWithinDistance(Float:x1, Float:y1, Float:z1, Float:dist, &vehi)
{
	new Float:x2, Float:y2, Float:z2, Float:d;
	for(new i = 1; i <= MAX_VEHICLES + 1; i++)
	{
		if(GetVehicleModel(i) > 0)
		{
			GetVehiclePos(i, x2, y2, z2);
			x2 -= x1; y2 -= y1; z2 -= z1;
			d = x2*x2 + y2*y2 + z2*z2;
			if(d < dist)
			{
				vehi = i;
				dist = d;
			}
		}
	}
}

/**
	<summary>
        Est-ce que le joueur est proche d'un véhicule ?
    </summary>

    <param name="playerid">
		int - ID du joueur à vérifier
    </param>

    <returns>
		bool - true s'il y a un véhicule à proximité
    </returns>
*/
stock bool:IsPlayerNearVehicle(playerid)//Fonction pour savoir si un joueur est proche d'un véhicule
{
	new Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2;
	GetPlayerPos(playerid, x1, y1, z1);
	for(new i = 1; i < MAX_VEHICLES + 1; i++)
	{
		if(GetVehicleModel(i) > 0)
		{
			GetVehiclePos(i, x2, y2, z2);
			x2 -= x1; y2 -= y1; z2 -= z1;
			if(x2 * x2 + y2 * y2 + z2 * z2 < 10.0) return true;
		}
	}
	return false;
}

/**
	<summary>
        Changer l'état d'ouverture du capot
    </summary>

    <param name="vehicleid">
		int - ID du véhicule possédant le capot
    </param>

    <param name="open">
		bool - Ouvert/Fermé
    </param>

    <returns>
		void
    </returns>
*/
public SetVehicleBonnetState(vehicleid, bool:open)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, open, boot, objective);
}

/**
	<summary>
        Changer l'état d'ouverture du coffre
    </summary>

    <param name="vehicleid">
		int - ID du véhicule possédant le coffre
    </param>

    <param name="open">
		bool - Ouvert/Fermé
    </param>

    <returns>
		void
    </returns>
*/
public SetVehicleTrunkState(vehicleid, bool:open)//Pour ouvrir ou fermer le coffre du véhicule
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, open, objective);
}

/**
	<summary>
        Trouve l'ID du véhicule dans le tableau à partir de son vehicleid réel
    </summary>

    <param name="vehicleid">
		int - Le vehicleid à traiter
    </param>

    <returns>
		int -   L'ID du véhicule
                -1 si aucun véhicule ne correspond
    </returns>
*/
GetVehicleID(vehicleid)
{
	if(!IsValidVehicle(vehicleid)) return -1;
	for(new i = 0; i < MAX_SPAWN_VEHICLES; i ++) if(dVehicleInfos[i][dVehicleID] == vehicleid) return i;
	return -1;
}

/**
	<summary>
        Détruit un véhicule
    </summary>

    <param name="vehicleid">
		int - ID du véhicule à détruire
    </param>

    <returns>
		void
    </returns>
*/
public DestroyVeh(vehicleid)
{
    if(MAX_SPAWN_VEHICLES > vehicleid > 0 && IsValidVehicle(dVehicleInfos[vehicleid][dVehicleID]))
    {
        DestroyVehicle(dVehicleInfos[vehicleid][dVehicleID]);
        SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
        dVehicleInfos[vehicleid][dVehicleModel] = 0;
        dVehicleInfos[vehicleid][dWheels] = 0;
        dVehicleInfos[vehicleid][dItem] = 0;
        dVehicleInfos[vehicleid][dColor][0] = 0;
        dVehicleInfos[vehicleid][dColor][1] = 0;
        dVehicleInfos[vehicleid][xVeh] = 0.0;
        dVehicleInfos[vehicleid][yVeh] = 0.0;
        dVehicleInfos[vehicleid][zVeh] = 0.0;
        dVehicleInfos[vehicleid][aVeh] = 0.0;
        dVehicleInfos[vehicleid][dFuel] = 0;
        dVehicleInfos[vehicleid][bEngine] = false;
        dVehicleInfos[vehicleid][bWheel][0] = false;
        dVehicleInfos[vehicleid][bWheel][1] = false;
        dVehicleInfos[vehicleid][bWheel][2] = false;
        dVehicleInfos[vehicleid][bWheel][3] = false;
        dVehicleInfos[vehicleid][TrunkObject][0] = 0;
        dVehicleInfos[vehicleid][TrunkObject][1] = 0;
        dVehicleInfos[vehicleid][TrunkObject][2] = 0;
        dVehicleInfos[vehicleid][TrunkObject][3] = 0;
        dVehicleInfos[vehicleid][TrunkObject][4] = 0;
        dVehicleInfos[vehicleid][TrunkObject][5] = 0;
        dVehicleInfos[vehicleid][dVehicleID] = INVALID_VEHICLE_ID;
    }
}

/**
	<summary>
        Crée un véhicule aléatoirement sur la map
    </summary>

    <returns>
		int -   1
    </returns>
*/
public CreateRandomVehicle()
{
	new vehicleid = -1;
	for(new i = 0; i < MAX_SPAWN_VEHICLES; i ++)
	{
	    if(dVehicleInfos[i][dVehicleModel] == 0)
	    {
	        vehicleid = i;
	        break;
	    }
	}
	if(vehicleid != -1 && !IsValidVehicle(dVehicleInfos[vehicleid][dVehicleID]))
	{
	    new dType = RandomEx(0, 10);
	    if((dType == 4 || dType == 5 || dType == 6) && RandomEx(0, 5) > 2) dType = RandomEx(0, 10);
	    switch(dType)
	    {
	        case 0://4x4
	        {
	            if(GetRandomVehicleSpawn(0, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]) == 0) return 1;
			    new d4x4IDs[][2] =
			    {
			        {400, 4}, {422, 4}, {424, 2},
					{470, 6}, {489, 6}, {495, 3},
					{568, 1}
			    };
			    //GetRandomVehicleSpawn(0, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]);
			    new dRand = random(sizeof(d4x4IDs));
				dVehicleInfos[vehicleid][dVehicleModel] = d4x4IDs[dRand][0];
				dVehicleInfos[vehicleid][dItem] = d4x4IDs[dRand][1];
			    dVehicleInfos[vehicleid][dWheels] = 4;
				dVehicleInfos[vehicleid][dColor][0] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dColor][1] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dFuel] = RandomEx(0, 4000);
 				dVehicleInfos[vehicleid][dVehicleID] = CreateVehicle(dVehicleInfos[vehicleid][dVehicleModel], dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh],dVehicleInfos[vehicleid][aVeh], dVehicleInfos[vehicleid][dColor][0],dVehicleInfos[vehicleid][dColor][1], -1, false);
			    SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
			    dVehicleInfos[vehicleid][bEngine] = (RandomEx(0, 5) < 3) ? true : false;
			    for(new i = 0; i < 4; i ++) dVehicleInfos[vehicleid][bWheel][i] = (RandomEx(0, 4) < 3) ? true : false;
			    SetVehicleWheels(dVehicleInfos[vehicleid][dVehicleID]);
			    //---
			    if(RandomEx(0, 10) < 3)
			    {
					GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 0);
					if(d4x4IDs[dRand][1] > 1 && RandomEx(0, 10) < 3)
					{
						GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 1);
					}
			    }
	        }
	        case 1://Motos
	        {
	            if(GetRandomVehicleSpawn(0, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]) == 0) return 1;
			    new dBikesIDs[][2] =
			    {
			        {461, 1}, {462, 2}, {463, 3},
					{468, 1}, {471, 3}, {581, 1},
					{586, 4}
			    };
			    new dRand = random(sizeof(dBikesIDs));
				dVehicleInfos[vehicleid][dVehicleModel] = dBikesIDs[dRand][0];
				dVehicleInfos[vehicleid][dItem] = dBikesIDs[dRand][1];
			    dVehicleInfos[vehicleid][dWheels] = (dRand == 4) ? 4 : 2;
				dVehicleInfos[vehicleid][dColor][0] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dColor][1] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dFuel] = RandomEx(0, 1500);
 				dVehicleInfos[vehicleid][dVehicleID] = CreateVehicle(dVehicleInfos[vehicleid][dVehicleModel], dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh],dVehicleInfos[vehicleid][aVeh], dVehicleInfos[vehicleid][dColor][0],dVehicleInfos[vehicleid][dColor][1], -1, false);
			    SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
			    dVehicleInfos[vehicleid][bEngine] = (RandomEx(0, 5) < 2) ? true : false;
			    for(new i = 0; i < 2; i ++) dVehicleInfos[vehicleid][bWheel][i] = (RandomEx(0, 4) < 2) ? true : false;
			    SetVehicleWheels(dVehicleInfos[vehicleid][dVehicleID]);
			    //---
			    if(RandomEx(0, 10) < 3)
			    {
					GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 0);
					if(dBikesIDs[dRand][1] > 1 && RandomEx(0, 10) < 3)
					{
						GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 1);
					}
			    }
			}
	        case 2://Industriel
	        {
	            if(GetRandomVehicleSpawn(0, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]) == 0) return 1;
	            new dInduIDs[][2] =
	            {
	                {413, 6}, {456, 6}, {478, 4},
	                {482, 6}, {498, 6}, {531, 1}
	            };
			    new dRand = random(sizeof(dInduIDs));
				dVehicleInfos[vehicleid][dVehicleModel] = dInduIDs[dRand][0];
				dVehicleInfos[vehicleid][dItem] = dInduIDs[dRand][1];
			    dVehicleInfos[vehicleid][dWheels] = 4;
				dVehicleInfos[vehicleid][dColor][0] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dColor][1] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dFuel] = RandomEx(0, 5000);
 				dVehicleInfos[vehicleid][dVehicleID] = CreateVehicle(dVehicleInfos[vehicleid][dVehicleModel], dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh],dVehicleInfos[vehicleid][aVeh], dVehicleInfos[vehicleid][dColor][0],dVehicleInfos[vehicleid][dColor][1], -1, false);
			    SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
			    dVehicleInfos[vehicleid][bEngine] = (RandomEx(0, 5) < 4) ? true : false;
			    for(new i = 0; i < 4; i ++) dVehicleInfos[vehicleid][bWheel][i] = (RandomEx(0, 4) < 3) ? true : false;
			    SetVehicleWheels(dVehicleInfos[vehicleid][dVehicleID]);
			    //---
			    if(RandomEx(0, 10) < 3)
			    {
					GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 0);
					if(dInduIDs[dRand][1] > 1 && RandomEx(0, 10) < 3)
					{
						GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 10, 15, 30, 15, 5, 15), 1);
					}
			    }
	        }
	        case 3://Sportives
	        {
	            if(GetRandomVehicleSpawn(0, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]) == 0) return 1;
	            new dSportIDs[][2] =
	            {
	                {402, 3}, {415, 3}, {429, 3},
	                {451, 2}, {475, 4}, {541, 3},
	                {559, 3}, {560, 3}, {562, 2},
					{587, 3}, {603, 2}
	            };
			    new dRand = random(sizeof(dSportIDs));
				dVehicleInfos[vehicleid][dVehicleModel] = dSportIDs[dRand][0];
				dVehicleInfos[vehicleid][dItem] = dSportIDs[dRand][1];
			    dVehicleInfos[vehicleid][dWheels] = 4;
				dVehicleInfos[vehicleid][dColor][0] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dColor][1] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dFuel] = RandomEx(0, 2500);
 				dVehicleInfos[vehicleid][dVehicleID] = CreateVehicle(dVehicleInfos[vehicleid][dVehicleModel], dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh],dVehicleInfos[vehicleid][aVeh], dVehicleInfos[vehicleid][dColor][0],dVehicleInfos[vehicleid][dColor][1], -1, false);
			    SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
			    dVehicleInfos[vehicleid][bEngine] = (RandomEx(0, 5) < 1) ? true : false;
			    for(new i = 0; i < 4; i ++) dVehicleInfos[vehicleid][bWheel][i] = (RandomEx(0, 4) < 1) ? true : false;
			    SetVehicleWheels(dVehicleInfos[vehicleid][dVehicleID]);
			    //---
			    if(RandomEx(0, 10) < 2)
			    {
					GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 25, 25, 25, 15, 5, 5), 0);
					if(dSportIDs[dRand][1] > 1 && RandomEx(0, 10) < 3)
					{
						GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 25, 25, 25, 15, 5, 5), 1);
					}
			    }
	        }
	        case 4://Avions
	        {
	            if(GetRandomVehicleSpawn(2, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]) == 0) return 1;
	            new dPlaneIDs[][2] =
	            {
	                {511, 6}, {513, 6}, {593, 6}
	            };
			    new dRand = random(sizeof(dPlaneIDs));
				dVehicleInfos[vehicleid][dVehicleModel] = dPlaneIDs[dRand][0];
				dVehicleInfos[vehicleid][dItem] = dPlaneIDs[dRand][1];
			    dVehicleInfos[vehicleid][dWheels] = 0;
				dVehicleInfos[vehicleid][dColor][0] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dColor][1] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dFuel] = RandomEx(2500, 15000);
			    dVehicleInfos[vehicleid][bEngine] = (RandomEx(0, 5) < 3) ? true : false;
 				dVehicleInfos[vehicleid][dVehicleID] = CreateVehicle(dVehicleInfos[vehicleid][dVehicleModel], dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh],dVehicleInfos[vehicleid][aVeh], dVehicleInfos[vehicleid][dColor][0],dVehicleInfos[vehicleid][dColor][1], -1, false);
 				SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
			    //---
			    if(RandomEx(0, 10) < 3)
			    {
					GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 0);
					if(dPlaneIDs[dRand][1] > 1 && RandomEx(0, 10) < 3)
					{
						GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 1);
					}
			    }
	        }
	        case 5://Bâteaux
	        {
	            if(GetRandomVehicleSpawn(3, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]) == 0) return 1;
	            new dBoatIDs[][2] =
	            {
	                {446, 4}, {452, 4}, {454, 6},
	                {472, 6}, {473, 2}, {493, 4}
	            };
			    new dRand = random(sizeof(dBoatIDs));
				dVehicleInfos[vehicleid][dVehicleModel] = dBoatIDs[dRand][0];
				dVehicleInfos[vehicleid][dItem] = dBoatIDs[dRand][1];
			    dVehicleInfos[vehicleid][dWheels] = 0;
				dVehicleInfos[vehicleid][dColor][0] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dColor][1] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dFuel] = RandomEx(2500, 5000);
			    dVehicleInfos[vehicleid][bEngine] = (RandomEx(0, 5) < 3) ? true : false;
 				dVehicleInfos[vehicleid][dVehicleID] = CreateVehicle(dVehicleInfos[vehicleid][dVehicleModel], dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh],dVehicleInfos[vehicleid][aVeh], dVehicleInfos[vehicleid][dColor][0],dVehicleInfos[vehicleid][dColor][1], -1, false);
 				SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
			    //---
			    if(RandomEx(0, 10) < 3)
			    {
					GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 0);
					if(dBoatIDs[dRand][1] > 1 && RandomEx(0, 10) < 3)
					{
						GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 1);
					}
			    }
	        }
	        case 6://Hélicos
	        {
	            if(GetRandomVehicleSpawn(1, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]) == 0) return 1;
	            new dHeliIDs[][2] =
	            {
	                {417, 6}, {469, 4}, {487, 4}
	            };
			    new dRand = random(sizeof(dHeliIDs));
				dVehicleInfos[vehicleid][dVehicleModel] = dHeliIDs[dRand][0];
				dVehicleInfos[vehicleid][dItem] = dHeliIDs[dRand][1];
			    dVehicleInfos[vehicleid][dWheels] = 0;
				dVehicleInfos[vehicleid][dColor][0] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dColor][1] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dFuel] = RandomEx(0, 8000);
			    dVehicleInfos[vehicleid][bEngine] = (RandomEx(0, 5) < 3) ? true : false;
			    dVehicleInfos[vehicleid][dVehicleID] = CreateVehicle(dVehicleInfos[vehicleid][dVehicleModel], dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh],dVehicleInfos[vehicleid][aVeh], dVehicleInfos[vehicleid][dColor][0], dVehicleInfos[vehicleid][dColor][1], -1, false);
 				SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
			    //---
			    if(RandomEx(0, 10) < 3)
			    {
					GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 0);
					if(dHeliIDs[dRand][1] > 1 && RandomEx(0, 10) < 3)
					{
						GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 1);
					}
			    }
	        }
	        case 7://Publiques
	        {
	            if(GetRandomVehicleSpawn(0, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]) == 0) return 1;
	            new dPublicIDs[][2] =
	            {
	                {407, 6}, {416, 6}, {420, 2},
	                {490, 4}, {596, 3}, {599, 3}
	            };
			    new dRand = random(sizeof(dPublicIDs));
				dVehicleInfos[vehicleid][dVehicleModel] = dPublicIDs[dRand][0];
				dVehicleInfos[vehicleid][dItem] = dPublicIDs[dRand][1];
			    dVehicleInfos[vehicleid][dWheels] = 4;
				dVehicleInfos[vehicleid][dColor][0] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dColor][1] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dFuel] = RandomEx(0, 2500);
 				CreateVehicle(dVehicleInfos[vehicleid][dVehicleModel], dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh],dVehicleInfos[vehicleid][aVeh], dVehicleInfos[vehicleid][dColor][0],dVehicleInfos[vehicleid][dColor][1], -1, false);
			    SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
			    dVehicleInfos[vehicleid][bEngine] = (RandomEx(0, 5) < 4) ? true : false;
			    for(new i = 0; i < 4; i ++) dVehicleInfos[vehicleid][bWheel][i] = (RandomEx(0, 4) < 3) ? true : false;
			    SetVehicleWheels(dVehicleInfos[vehicleid][dVehicleID]);
			    //---
			    if(RandomEx(0, 10) < 5)//PickRandomItem(neutral, gun, vehicle, medic, clothes, bag)
			    {
					if(dRand == 0 || dRand == 2) GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 0);
					else if(dRand == 1) GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 20, 10, 10, 50, 5, 5), 0);
					else if(dRand == 3 || dRand == 4 || dRand == 5) GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 15, 50, 15, 10, 5, 5), 0);
					if(dPublicIDs[dRand][1] > 1 && RandomEx(0, 10) < 5)
					{
						if(dRand == 0 || dRand == 2) GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 0);
						else if(dRand == 1) GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 20, 10, 10, 50, 5, 5), 0);
						else if(dRand == 3 || dRand == 4 || dRand == 5) GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 15, 50, 15, 10, 5, 5), 0);
					}
			    }
	        }
	        case 8://Vélos
	        {
	            if(GetRandomVehicleSpawn(0, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]) == 0) return 1;
	            new dBicycIDs[][2] =
	            {
	                {481, 0}, {509, 0}, {510, 0}
	            };
			    new dRand = random(sizeof(dBicycIDs));
				dVehicleInfos[vehicleid][dVehicleModel] = dBicycIDs[dRand][0];
				dVehicleInfos[vehicleid][dItem] = dBicycIDs[dRand][1];
			    dVehicleInfos[vehicleid][dWheels] = 0;
				dVehicleInfos[vehicleid][dColor][0] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dColor][1] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dFuel] = -1;
			    dVehicleInfos[vehicleid][bEngine] = true;
 				dVehicleInfos[vehicleid][dVehicleID] = CreateVehicle(dVehicleInfos[vehicleid][dVehicleModel], dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh],dVehicleInfos[vehicleid][aVeh], dVehicleInfos[vehicleid][dColor][0],dVehicleInfos[vehicleid][dColor][1], -1, false);
	        }
	        case 9://Normales
	        {
	            if(GetRandomVehicleSpawn(0, dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh], dVehicleInfos[vehicleid][aVeh]) == 0) return 1;
	            new dNormIDs[][2] =
	            {
	                {405, 3}, {421, 3}, {426, 3},
	                {445, 3}, {492, 2}, {504, 3},
	                {542, 3}, {549, 3}, {551, 4},
	                {567, 4}, {604, 4}
	            };
			    new dRand = random(sizeof(dNormIDs));
				dVehicleInfos[vehicleid][dVehicleModel] = dNormIDs[dRand][0];
				dVehicleInfos[vehicleid][dItem] = dNormIDs[dRand][1];
			    dVehicleInfos[vehicleid][dWheels] = 4;
				dVehicleInfos[vehicleid][dColor][0] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dColor][1] = RandomEx(0, 256);
				dVehicleInfos[vehicleid][dFuel] = RandomEx(1500, 3500);
 				dVehicleInfos[vehicleid][dVehicleID] = CreateVehicle(dVehicleInfos[vehicleid][dVehicleModel], dVehicleInfos[vehicleid][xVeh], dVehicleInfos[vehicleid][yVeh], dVehicleInfos[vehicleid][zVeh],dVehicleInfos[vehicleid][aVeh], dVehicleInfos[vehicleid][dColor][0],dVehicleInfos[vehicleid][dColor][1], -1, false);
				SetVehicleEngineState(dVehicleInfos[vehicleid][dVehicleID], false);
			    dVehicleInfos[vehicleid][bEngine] = (RandomEx(0, 5) < 4) ? true : false;
			    for(new i = 0; i < 4; i ++) dVehicleInfos[vehicleid][bWheel][i] = (RandomEx(0, 4) < 3) ? true : false;
			    SetVehicleWheels(dVehicleInfos[vehicleid][dVehicleID]);
			    //---
			    if(RandomEx(0, 10) < 5)
			    {
					GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 0);
					if(dNormIDs[dRand][1] > 1 && RandomEx(0, 10) < 5)
					{
						GivePlayerTrunkObject(INVALID_PLAYER_ID, vehicleid, CallRemoteFunction("PickRandomItem", "dddddd", 30, 15, 30, 15, 5, 5), 1);
					}
			    }
	        }
	    }
 	}
 	return 1;
}

/**
	<summary>
        Trouve une position adéquate pour un nouveau véhicule en fonction du type de celui-ci
    </summary>

    <param name="type">
		int - Type de véhicule
    </param>

    <param name="x">
		Float - Variable pour récupérer la position X
    </param>

    <param name="y">
		Float - Variable pour récupérer la position Y
    </param>

    <param name="z">
		Float - Variable pour récupérer la position Z
    </param>

    <param name="angle">
		Float - Variable pour récupérer l'angle
    </param>
    
    <returns>
		int -   0 si aucune position n'a pu être trouvée
                1 si une position a été trouvée
    </returns>
*/
GetRandomVehicleSpawn(type, &Float:x, &Float:y, &Float:z, &Float:angle)
{
	new dTry = 0;
	switch(type)
	{
	    case 0://Sur terre
	    {
			new Float:fCarPos[][4] =
			{
			    //---RED COUNTY
			    {-741.887, -110.0589, 67.314, 16.0},
			    {-472.872, -64.0899, 60.7869, 90.0},
			    {-534.5609, -61.1479, 63.242, 270.0},
			    {-61.18, -78.0429, 3.367, 340.0},
			    {-63.2, 103.953, 3.367, 240.0},
			    {164.436, -33.9589, 1.8279, 180.0},
			    {265.588, 17.006, 2.6919, 90.0},
			    {287.212, -55.9329, 1.8279, 180.0},
			    {312.3909, -53.928, 1.8279, 180.0},
			    {314.048, -252.281, 1.8339, 260.0},
			    {90.6449, -303.5889, 1.8279, 0.0},
			    {202.8609, -306.346, 1.761, 180.0},
			    {509.8739, -212.832, 37.825, 180.0},
			    {667.5939, -543.354, 16.486, 90.0},
			    {827.3569, -497.625, 17.478, 180.0},
			    {762.8010, -510.333, 17.253, 180.0},
			    {700.6929, -455.1719, 16.486, 220.0},
			    {614.1929, -601.668, 17.3829, 220.0},
			    {797.4959, -617.106, 16.486, 0.0},
			    {1058.6169, -290.3789, 74.1419, 0.0},
			    {888.322, -25.5729, 63.375, 156.0},
			    {759.7429, 293.94, 20.693, 226.0},
			    {760.5599, 383.881, 23.322, 280.0},
			    {1134.467, 139.759, 29.7189, 280.0},
			    {1235.967, 212.781, 19.7049, 40.0},
			    {1408.383, 454.345, 20.354, 130.0},
			    {1394.1939, 387.8099, 19.908, 130.0},
			    {1422.479, 347.131, 18.9939, 40.0},
			    {1397.416, 302.54199, 19.7049, 270.0},
			    {1387.1469, 265.0729, 19.7169, 20.0},
			    {1432.722, 224.996, 19.7049, 64.0},
			    {1314.291, 194.712, 20.617, 64.0},
			    {1200.874, 142.8659, 20.687, 338.0},
			    {1585.939, 25.7399, 24.2479, 94.0},
			    {2152.7241, -90.85259, 2.851, 94.0},
			    {2252.7028, 160.6799, 27.3309, 180.0},
			    {2298.9331, 161.513, 26.634, 180.0},
			    {2253.5239, 105.551, 27.249, 180.0},
			    {2265.268, -122.205, 27.391, 210.0},
			    {2319.456, -72.1309, 26.634, 210.0},
			    {2397.404, -50.089, 27.547, 180.0},
			    {2446.7819, 11.3549, 26.634, 270.0},
			    {2451.2219, 87.679, 27.061, 270.0},
			    {2495.7719, 129.2489, 27.169, 180.0},
			    {2551.6589, 14.0799, 27.1709, 90.0},
			    {2360.9431, -652.306, 128.162, 150.0},
			    {2543.123, -761.13, 95.04, 270.0},
			    {1613.732, -327.898, 19.3299, 270.0},
			    //---FLINT COUNTY & WHETSTONE
			    {-2107.559, -2241.8911, 30.875, 160.0},
			    {-2140.8068, -2255.5048, 30.875, 230.0},
			    {-2208.809, -2255.7438, 30.9069, 230.0},
			    {-2230.009, -2311.996, 30.8519, 200.0},
			    {-2168.7939, -2283.56, 30.875, 140.0},
			    {-2218.582, -2406.456, 31.743, 240.0},
			    {-2198.458, -2435.5739, 30.875, 240.0},
			    {-2236.104, -2479.2939, 31.4309, 320.0},
			    {-2178.507, -2475.4951, 30.867, 320.0},
			    {-2200.7399, -2503.3769, 30.867, 320.0},
			    {-2168.6721, -2528.604, 30.867, 320.0},
			    {-2149.75, -2544.53, 30.867, 320.0},
			    {-2124.1931, -2504.1608, 30.867, 230.0},
			    {-2114.0991, -2488.2949, 30.875, 230.0},
			    {-2067.362, -2508.923, 30.892, 230.0},
			    {-2069.7629, -2530.25, 30.875, 320.0},
			    {-2068.4689, -2438.163, 30.875, 20.0},
			    {-2176.4431, -2428.404, 30.875, 50.0},
			    {-1582.9389, -2716.9221, 48.789, 110.0},
			    {26.624, -2638.4299, 40.673, 95.0},
			    {-20.0109, -2521.356, 36.8979, 65.0},
			    {-253.839, -2216.6589, 29.0219, 105.0},
			    {-84.2119, -1584.1879, 2.861, 185.0},
			    {-94.5459, -1195.0319, 2.568, 185.0},
			    {-62.359, -1140.411, 1.3279, 245.0},
			    {-397.1329, -1415.6429, 25.9769},
			    {-388.691, -1149.285, 69.6589, 355.0},
			    {-582.736, -1046.38, 23.8619, 0.0},
			    {-594.2299, -1084.253, 23.8239, 60.0},
			    {-577.765, -1474.328, 11.2969, 120.0},
			    {-1106.4439, -1621.225, 76.6169, 270.0},
			    {-1112.561, -1673.1519, 76.6169, 0.0},
			    {-1039.609, -1189.6309, 129.386, 0.0},
			    {-1033.3549, -1184.045, 129.386, 90.0},
			    {-1443.76, -1486.01, 102.008, 140.0},
			    {-490.6809, -487.8049, 25.7679, 180.0},
			    {-495.358, -487.6449, 25.7679, 0.0},
			    {-544.0689, -471.97198, 25.7679, 0.0},
			    {-350.696, -1051.8029, 59.5769, 90.0},
			    //---BONE COUNTY
			    {260.402, 1434.243, 10.836, 40.0},
			    {231.628, 1375.7919, 10.836, 0.0},
			    {197.3849, 1877.102, 17.898, 0.0},
			    {203.557, 1877.14, 17.891, 0.0},
			    {277.17, 1955.6109, 17.891, 270.0},
			    {142.7899, 1969.9169, 19.7649, 10.0},
			    {255.5070, 1808.233, 17.891, 0.0},
			    {-158.4049, 1648.274, 17.159, 60.0},
			    {-3.38, 1505.4179, 13.005, 120.0},
			    {3.6349, 1347.618, 9.4219, 120.0},
			    {-2.8699, 1395.953, 9.4219, 120.0},
			    {-105.647, 1372.0579, 10.5229, 120.0},
			    {-82.5579, 1340.477, 11.123, 200.0},
			    {-361.742, 1167.5589, 19.992, 200.0},
			    {-368.674, 1129.1739, 20.03, 105.0},
			    {-363.161, 1107.0319, 20.0049, 105.0},
			    {-321.747, 1056.5019, 19.992, 55.0},
			    {-304.605, 1032.1109, 19.8439, 105.0},
			    {-303.9609, 1018.991, 19.8439, 65.0},
			    {-346.403, 1025.1989, 20.2329, 65.0},
			    {-414.613, 1020.3439, 11.281, 105.0},
			    {-317.4899, 845.2509, 14.4919, 225.0},
			    {-223.992, 992.8499, 19.8099, 310.0},
			    {-211.75, 996.776, 19.8619, 50.0},
			    {-218.511, 1047.78, 19.992, 160.0},
			    {-247.1369, 1052.6519, 20.3059, 80.0},
			    {-249.546, 1074.874, 20.4039, 340.0},
			    {-254.6139, 1129.5489, 20.3439, 290.0},
			    {-261.0559, 1155.40, 19.992, 290.0},
			    {-287.769, 1179.191, 20.1539, 260.0},
			    {-320.1279, 1131.526, 19.999, 260.0},
			    {-301.8519, 1113.6379, 19.999, 200.0},
			    {-220.011, 1168.5329, 19.992, 200.0},
			    {-217.233, 1214.473, 19.992, 310.0},
			    {-241.5919, 1214.8919, 19.992, 10.0},
			    {-204.4389, 1215.1379, 19.992, 10.0},
			    {-194.673, 1222.629, 19.992, 30.0},
			    {-177.434, 1220.5159, 19.992, 110.0},
			    {-157.5169, 12229.42, 19.992, 190.0},
			    {-134.561, 1179.425, 20.0, 190.0},
			    {-161.19, 1169.625, 19.992, 230.0},
			    {-160.113, 1131.8079, 19.992, 200.0},
			    {-220.6329, 1126.3549, 19.992, 200.0},
			    {-236.7109, 1059.01, 19.9839, 200.0},
			    {-167.7259, 1029.4809, 19.9839, 200.0},
			    {-143.7619, 1038.067, 20.068, 200.0},
			    {-137.787, 1072.5229, 19.992, 150.0},
			    {-155.524, 1131.8649, 19.992, 150.0},
			    {-135.2389, 1161.0629, 20.0, 100.0},
			    {-83.652, 1222.556, 19.992, 120.0},
			    {-81.4489, 1143.5169, 20.0, 120.0},
			    {-47.608, 1125.1679, 20.086, 160.0},
			    {-22.572, 1118.9539, 19.992, 160.0},
			    {2.755, 1125.2309, 20.218, 160.0},
			    {3.0859, 1080.259, 19.992, 80.0},
			    {46.8619, 1170.118, 18.9139, 80.0},
			    {94.6139, 1173.7259, 18.9139, 20.0},
			    {101.276, 1067.978, 13.859, 20.0},
			    {-142.625, 934.6619, 19.8159, 20.0},
			    {-146.5, 884.405, 18.874, 20.0},
			    {-80.0339, 936.1989, 20.8209, 10.0},
			    {-54.1399, 967.8679, 20.0709, 10.0},
			    {33.3849, 963.689, 20.027, 10.0},
			    {179.106, 1168.7619, 15.008, 50.0},
			    {314.95, 1149.2869, 8.836, 80.0},
			    {410.2739, 1157.061, 8.1579, 80.0},
			    {506.4939, 1113.211, 15.0799, 0.0},
			    {590.414, 1240.3699, 11.9689, 30.0},
			    {707.9019, 1202.8349, 13.6459, 100.0},
			    {-322.8779, 1534.03, 75.609, 100.0},
			    {-320.441, 1514.196, 75.609, 350.0},
			    {-282.984, 1559.5219, 75.609, 340.0},
			    {-291.3829, 1301.8, 54.157, 270.0},
			    {-301.0369, 1775.253, 42.9379, 270.0},
			    {-397.286, 2070.833, 62.3019, 270.0},
			    {-353.6669, 2230.8449, 42.734, 270.0},
			    {-411.6879, 2253.2009, 42.68, 270.0},
			    {-230.3809, 2733.291, 62.9379, 300.0},
			    {-204.4550, 2594.1149, 62.9529, 30.0},
			    {-230.871, 2595.9279, 62.959, 350.0},
			    {-239.845, 2608.944, 62.9529, 350.0},
			    {-278.7149, 2761.893, 62.3689, 350.0},
			    {-234.9429, 2812.852, 61.984, 350.0},
			    {-158.8459, 2731.4289, 62.3779, 350.0},
			    {407.8789, 2537.1621, 16.797, 350.0},
			    {244.365, 1618.123, 16.7269, 350.0},
			    {-642.7169, 2714.175, 72.625, 30.0},
			    {-742.1339, 2751.5319, 47.479, 350.0},
			    {-792.853, 2771.289, 45.95, 270.0},
			    {-1278.399, 2723.424, 50.3129, 320.0},
			    {-1399.5699, 2649.5891, 55.9379, 280.0},
			    {-1451.425, 2564.5358, 56.0859, 280.0},
			    {-1525.0679, 2524.843, 56.0139, 190.0},
			    {-1515.963, 2570.666, 56.0859, 190.0},
			    {-1449.899, 2645.061, 56.0859, 270.0},
			    {-1472.7239, 2687.7919, 56.0859, 190.0},
			    {-1525.0739, 2693.8959, 56.0859, 190.0},
			    {-1600.437, 2683.3559, 55.3989, 190.0},
			    {-1565.8549, 2643.1889, 56.0859, 260.0},
			    {-1663.687, 2599.623, 81.5849, 260.0},
			    {-1665.546, 2560.2189, 85.3769, 260.0},
			    {-1661.034, 2494.0659, 87.189, 300.0},
			    {-1517.3459, 2529.1499, 55.956, 330.0},
			    {-1519.451, 2307.467, 47.678, 330.0},
			    {-1366.805, 2059.1069, 52.9189, 0.0},
			    {-1482.3449, 1878.41, 32.8829, 0.0},
			    {-1210.873, 1840.7349, 41.969, 0.0},
			    {-1043.5939, 1552.1899, 33.636, 310.0},
			    {-934.8239, 1421.4289, 30.3859, 310.0},
			    {-682.4130, 965.6179, 12.383, 80.0},
			    {-643.539, 1449.921, 13.8669, 350.0},
			    {-720.1799, 1439.5739, 18.7269, 350.0},
			    {-743.7849, 1440.5219, 16.6389, 20.0},
			    {-748.492, 1599.5799, 27.367, 20.0},
			    {-781.642, 1550.212, 27.367, 70.0},
			    {-865.5159, 1542.317, 23.0279, 70.0},
			    {-765.546, 1484.7039, 24.965, 70.0},
			    {-880.5369, 1534.196, 26.1609, 70.0},
			    {-905.3959, 1537.213, 26.1639, 100.0},
			    {-1939.3179, 2388.6196, 49.742, 100.0},
			    {-2610.1508, 2257.393, 8.461, 150.0},
			    {-2552.51, 2271.4929, 5.314, 150.0},
			    {-2529.614, 2290.789, 5.234, 180.0},
			    {-2529.124, 2250.52, 5.229, 160.0},
			    {-2479.4929, 2242.7829, 5.0939, 180.0},
			    {-2444.383, 2238.614, 5.0939, 140.0},
			    {-2264.621, 2300.5329, 5.07, 180.0},
			    {-2302.873, 2419.3139, 5.164, 180.0},
			    {-2458.581, 2491.602, 16.8659, 180.0},
			    {-2537.4619, 2359.0419, 5.235, 220.0},
			    {-2475.882, 2316.854, 5.234, 220.0},
			    {-2489.6608, 2291.465, 5.234, 250.0},
			    {-2447.3859, 2304.873, 5.234, 234.0},
			    //---LAS VENTURAS
			    {2172.4895, 1989.5502, 10.7999},
			    {2154.6945, 2112.7673, 10.6529},
			    {2251.3613, 2357.8713, 10.8015},
			    {2297.9404, 2450.996, 3.2504},
			    {2143.1938, 2814.8913, 10.803},
			    {2435.2849, 2730.8139, 10.7935},
			    {2104.363, 2046.5655, 10.7961},
			    {2470.8715, 2531.2832, 21.8602},
			    {2627.3762, 2205.8032, 10.7985},
			    {2827.1694, 2399.7556, 10.8316},
			    {2759.6472, 1442.2363, 10.7516},
			    {2764.4284, 1268.262, 10.7287},
			    {2451.6547, 1358.8393, 10.7965},
			    {2292.8579, 1494.5377, 16.6536},
			    {2295.3623, 1519.2037, 30.0039},
			    {2109.9753, 1398.5837, 10.811},
			    {2040.4324, 1481.0935, 10.6565},
			    {2039.9967, 1059.1791, 10.6551},
			    {2310.1401, 968.1272, 10.6799},
			    {2478.1733, 1211.1579, 10.7998},
			    {2632.1577, 1278.586, 10.6776},
			    {2486.7751, 1533.7832, 10.7643},
			    {2592.0734, 1840.4114, 10.803},
			    {2589.4418, 2057.28, 10.8016},
			    {1686.0439, 2186.9118, 10.8044},
			    {1614.061, 2201.0253, 10.801},
			    {1512.1071, 2280.9277, 10.7961},
			    {1712.7346, 2260.6284, 10.7985},
			    {2007.0258, 2465.5734, 10.8},
			    {2153.6835, 2498.7988, 10.8019},
			    {1959.395, 2650.4455, 10.8088},
			    {1650.9017, 2745.6625, 10.8171},
			    {1314.4819, 2648.8571, 10.8003},
			    {1012.9846, 2441.9992, 10.7668},
			    {985.58581, 1894.4852, 11.4468},
			    {1125.8001, 2112.4125, 10.8047},
			    {1371.4211, 2013.0855, 11.2491},
			    {1437.1591, 2017.3601, 10.8051},
			    {1898.5354, 2105.6174, 10.7998},
			    {1891.1606, 1718.9912, 10.7579},
			    {1710.9689, 1590.5622, 10.3186},
			    {1308.9111, 1280.3957, 10.8024},
			    {1851.8863, 698.0588, 11.2434},
			    {2185.4162, 733.182, 11.4244},
			    {2445.7431, 707.1785, 11.3961},
			    {2488.1198, 937.4649, 10.8063},
			    {2534.2009, 930.1321, 10.809},
			    {2432.4458, 1217.2485, 10.676},
			    {1885.9648, 1989.7786, 7.5822},
			    {1090.0004, 1994.1853, 11.3775},
			    {1111.4775, 1794.8199, 10.808},
			    {1005.2276, 1418.0944, 10.64943},
			    //---SAN FIERRO---//
				{-1899.8194, -899.0355, 31.8546, 52.5709}, //
				{-2119.1818, -768.7343, 31.8547, 231.2678}, //
				{-1898.5168, -430.1375, 38.0735, 0.3961}, //
				{-1950.4211, 265.4665, 40.8810, 41.4962}, //
				{-2024.8176, 137.9355, 28.6710, 294.9035}, //
				{-2103.7521, 243.3782, 35.6576, 32.7757}, //
				{-2211.2348, -2.1243, 35.1517, 187.8324}, //
				{-2131.7121, -255.2577, 35.1526, 351.0605}, //
				{-2407.1416, -1.0398, 35.1517, 159.3550}, //
				{-2417.3857, -597.8253, 132.4416, 66.8022}, //
				{-2657.8596, -285.5600, 7.3703, 30.8139}, //
				{-2752.7126, -68.0908, 6.9482, 291.3888}, //
				{-2654.6906, -50.8606, 4.1675, 60.1842}, //
				{-2760.6835, -7.2294, 6.7613, 89.5077}, //
				{-2842.3796, 277.6821, 10.4556, 339.5779}, //
				{-2665.4106, 274.2315, 4.1677, 37.2494}, //
				{-2596.8671, 481.5646, 14.4378, 36.6033}, //
				{-2563.9921, 633.2875, 14.2905, 172.9955}, //
				{-2460.7399, 728.6209, 34.8535, 233.7398}, //
				{-2387.9182, 742.7619, 34.8468, 328.1292}, //
				{-2559.2236, 934.8797, 64.8161, 108.0881}, //
				{-2647.7131, 1211.5455, 55.0943, 173.4225}, //
				{-2454.7617, 1220.8079, 34.8889, 14.1908}, //
				{-2618.6054, 1398.0932, 6.9326, 214.3679}, //
				{-2076.6696, 1351.0412, 6.9329, 330.8707}, //
				{-2178.4582, 1174.7172, 55.4097, 279.1444}, //
				{-1639.3524, 1295.3684, 6.8698, 135.7657}, //
				{-1587.0556, 937.6354, 7.4090, 101.7423}, //
				{-2063.0048, 960.6832, 59.6124, 236.4768}, //
				{-2080.8657, 759.8980, 69.3940, 178.1470}, //
				{-2128.1215, 654.4971, 52.1986, 86.8838}, //
				{-2189.9113, 610.0966, 34.9952, 289.3354}, //
				{-2217.7219, 419.2337, 34.9970, 177.9366}, //
				{-2418.7839, 322.6217, 34.8001, 327.4637}, //
				{-2589.3310, 328.6742, 4.6203, 270.8941}, //
				{-1951.9956, 572.5726, 35.0712, 270.9610}, //
				{-1747.6416, 762.2486, 24.7217, 275.8394}, //
				{-1956.5844, 581.2523, 34.9988, 174.9294}, //
				{-1700.4394, 297.7603, 7.0189, 283.1952}, //
				{-1431.1791, 460.8336, 7.0122, 283.9593}, //
				{-1333.3049, 459.6366, 7.0193, 103.8985}, //
				{-1720.0961, 117.4107, 3.3858, 0.9655}, //
				{-1677.0476, 28.0117, 3.3859, 197.9853}, //
				{-1820.4783, -175.7762, 9.2302, 5.7281}, //
				{-2090.8659, 60.8704, 34.6915, 107.5846}, //
				{-2203.9919, 296.8808, 34.9488, 336.5366} //

			};
			pickpos:
			dTry ++;
			new dRand = random(sizeof(fCarPos));
			new vehicleid = 0;
			GetVehicleWithinDistance(fCarPos[dRand][0], fCarPos[dRand][1], fCarPos[dRand][2], 10.0, vehicleid);
			if(vehicleid != 0)
			{
				if(dTry == 3)
				{
					return 0;
				}
				else goto pickpos;
			}
			else
			{
				x = fCarPos[dRand][0];
				y = fCarPos[dRand][1];
				z = fCarPos[dRand][2];
				angle = fCarPos[dRand][3];
			}
	    }
		case 1://Hélicos
		{
		    new Float:fChopperPos[][4] =
		    {
		        //---RED COUNTY
		        {615.8590, -578.3209, 26.408, 270.0},
		        //---FLINT COUNTY & WHETSTONE
		        {-43.833, -1147.7419, 1.343, 0.0},
		        {-2153.9399, -2275.082, 37.219, 52.0},
		        //---BONE COUNTY
		        {273.0769, 1866.8769, 17.906, 90.0},
		        {176.85, 1929.4449, 18.3789, 180.0},
		        {-2229.2758, 2330.4899, 7.8119, 110.0},
		        {366.419, 2537.676, 16.9249, 160.0},
		        {382.5899, 2537.6579, 16.804, 185.9997},
		        {-225.197, 1006.1599, 20.4659, 193.9997},
		        //---LAS VENTURAS
				{2083.1428, 2164.7028, 10.913},
				{2094.7011, 2415.8063, 74.670},
				{1332.7813, 1567.6512, 10.9091},
				{1351.6169, 2650.9196, 10.9145},
				//---SAN FIERRO
				{-1223.6445, -11.3415, 14.3250, 43.7462}, //HelicoAeroport
				{-1607.8555, 285.0718, 7.3616, 0.3918}, //HelicoMilitaire
				{-1685.2705, 702.4854, 30.7788, 31.8836} //HelicoPolice

		    };
			pickpos:
			new dRand = random(sizeof(fChopperPos));
			new vehicleid = 0;
			GetVehicleWithinDistance(fChopperPos[dRand][0], fChopperPos[dRand][1], fChopperPos[dRand][2], 10.0, vehicleid);
			if(vehicleid != 0)
			{
				if(dTry == 3)
				{
					return 0;
				}
				else goto pickpos;
			}
			else
			{
				x = fChopperPos[dRand][0];
				y = fChopperPos[dRand][1];
				z = fChopperPos[dRand][2];
				angle = fChopperPos[dRand][3];
			}
		}
		case 2://Avions
		{
			new Float:fPlanePos[][4] =
			{
			    //---RED COUNTY
			    {-495.22, 219.2319, 9.4189, 240.0},
			    {1405.774, -113.3059, 22.127, 300.0},
			    //---FLINT COUNTY & WHETSTONE
			    {-1209.0689, -928.6989, 129.1889, 270.0},
			    //---BONE COUNTY
			    {308.376, 2044.369, 19.1569, 180.0},
			    {371.686, 1978.515, 19.1569, 60.0},
			    {408.4599, 2501.4309, 18.7189, 90.0},
			    {326.4129, 2527.944, 19.027, 170.0},
			    {223.477, 2463.6289, 17.7189, 340.0},
			    //---LAS VENTURAS
			    {1482.8653, 1773.0648, 11.5072},
			    {1460.4775, 1774.5147, 11.3715}
			};
			pickpos:
			new dRand = random(sizeof(fPlanePos));
			new vehicleid = 0;
			GetVehicleWithinDistance(fPlanePos[dRand][0], fPlanePos[dRand][1], fPlanePos[dRand][2], 10.0, vehicleid);
			if(vehicleid != 0)
			{
				if(dTry == 3)
				{
					return 0;
				}
				else goto pickpos;
			}
			else
			{
				x = fPlanePos[dRand][0];
				y = fPlanePos[dRand][1];
				z = fPlanePos[dRand][2];
				angle = fPlanePos[dRand][3];
			}
		}
		case 3://Bateaux
		{
			new Float:fBoatPos[][4] =
			{
			    //---RED COUNTY
			    {1403.5699, -249.1739, 0.0, 90.0},
			    {2093.3378, -111.774, 0.0, 30.0},
			    {1984.6309, -46.2229, 0.0, 110.0},
			    {2124.114, 176.5769, 0.0, 170.0},
			    {2270.854, 425.2539, 0.0, 100.0},
			    {591.057, -245.852, 0.0, 100.0},
			    {391.2189, -275.1010, 0.0, 130.0},
			    {-65.495, -587.143, 0.0, 80.0},
			    {-750.5339, 248.38, 0.0, 140.0},
			    {-484.377, 316.29, 0.0, 270.0},
			    //---FLINT COUNTY & WHETSTONE
			    {-321.9219, -471.5698, 0.0, 318.0},
			    {-216.876, -783.364, 0.0, 248.0},
			    {-223.742, -1737.614, 0.0, 198.0},
			    {-1667.88, -1686.682, 0.0, 248.0},
			    //---BONE COUNTY
			    {-493.9309, 611.741, 0.0, 0.0},
			    {-644.2, 861.0759, 0.0, 240.0},
			    {-425.933, 1161.4, 0.0, 20.0},
			    {-757.4819, 1381.6099, 0.0, 270.0},
			    {-615.114, 1804.988, 0.0, 80.0},
			    {-934.0529, 2656.642, 40.75, 130.0},
			    {-1376.192, 2118.53, 40.75, 240.0},
			    {-2219.592, 2425.7019, 0.0, 320.0},
			    {-2224.1608, 2402.3491, 0.0, 45.0},
			    {-2321.111, 2318.31, 0.0, 180.0},
			    {-2419.009, 2304.779, 260.0},
			    {-2037.285, 2332.6779, 0.0, 40.0},
			    //---LAS VENTURAS
			    {2363.9121, 513.479, 0.0, 90.0},
			    {1227.9255, 635.6099, 0.0, 90.0},
			    {1629.9709, 568.197, 0.0, 90.0},
			    //---SAN FIERRO
			    {-1631.3043, 161.5726, -0.5212, 135.6955}, //BateauSF
				{-1466.1694, 1095.0920, -0.5082, 305.6648}, //BateauSF
				{-1893.6547, 1400.6180, -0.3995, 90.2900}, //BateauSF
				{-2980.0590, 508.6404, -0.7971, 318.2321} //BateauSF
			};
			pickpos:
			new dRand = random(sizeof(fBoatPos));
			new vehicleid = 0;
			GetVehicleWithinDistance(fBoatPos[dRand][0], fBoatPos[dRand][1], fBoatPos[dRand][2], 10.0, vehicleid);
			if(vehicleid != 0)
			{
				if(dTry == 3)
				{
					return 0;
				}
				else goto pickpos;
			}
			else
			{
				x = fBoatPos[dRand][0];
				y = fBoatPos[dRand][1];
				z = fBoatPos[dRand][2];
				angle = fBoatPos[dRand][3];
			}
		}
	}
	return 1;
}

/**
	<summary>
        Récupère le type d'un véhicule
    </summary>

    <param name="vehicleid">
		int - ID du véhicule dont le type est à récupérer
    </param>
    
    <returns>
		int - le type de véhicule
    </returns>
*/
GetVehicleFuelType(modelid)
{
	switch(modelid)
	{
	    case 481, 509, 510: return 0;//VÉLOS
		case 435, 449, 450, 537, 538, 570, 584, 590, 591, 606, 608, 610, 611: return 0; //REMORQUES & TRAINS
		case 448, 461, 462, 463, 468, 521, 522, 523, 581, 586: return 1; //MOTOS
		case 400, 401, 402, 404, 405, 409, 410, 411, 412, 415, 418, 419, 420, 421, 422, 424, 426, 429, 434, 436, 438, 439, 441,
		442, 444, 445, 451, 457, 458, 466, 467, 470, 471, 474, 475, 477, 478, 479, 480, 485, 489, 490, 491, 492, 494, 495,
		496, 500, 502, 503, 504, 505, 506, 507, 516, 517, 518, 525, 526, 527, 529, 533, 534, 535, 536, 540, 541, 542, 543,
		545, 546, 547, 549, 550, 551, 552, 554, 555, 556, 557, 558, 559, 560, 561, 562, 565, 566, 567, 568, 571, 575, 576,
		579, 580, 585, 587, 589, 594, 596, 597, 598, 599, 600, 602, 603, 604, 605: return 2; //VOITURES
		case 403, 406, 407, 408, 413, 414, 416, 423, 427, 428, 431, 432, 433, 437, 440, 443, 455, 456, 459, 482, 483, 486, 498,
		499, 508, 514, 515, 524, 528, 530, 531, 532, 544, 564, 572, 573, 574, 578, 582, 583, 588, 601, 609: return 3; //CAMIONS & UTILITAIRES
		case 430, 446, 452, 453, 454, 472, 473, 484, 493, 539, 595: return 4;//BATEAUX
		case 460, 464, 476, 511, 512, 513, 519, 520, 553, 577, 592, 593: return 5; //AVIONS
		case 417, 425, 447, 465, 469, 487, 488, 497, 501, 548, 563: return 6; //HÉLICOS
	}
	return 0;
}

/**
	<summary>
        Récupère la quantité maximale d'un véhicule en fonction de son type.
    </summary>

    <param name="vehicleid">
		int - ID du véhicule à traiter
    </param>
    
    <returns>
		void
    </returns>
*/
GetVehicleMaxFuel(vehicleid)
{
	switch(GetVehicleFuelType(dVehicleInfos[vehicleid][dVehicleModel]))
	{
	    case 0: return 0;
	    case 1: return BIKE_FUEL;
	    case 2: return CAR_FUEL;
	    case 3: return TRUCK_FUEL;
	    case 4: return BOAT_FUEL;
	    case 5: return PLANE_FUEL;
	    case 6: return HELI_FUEL;
	}
	return 0;
}

GetVehicleFuel(vehicleid) 
{
	return dVehicleInfos[vehicleid][dFuel];
}

/**
	<summary>
        Stocke l'état des roues dans un bitmask
    </summary>

    <param name="vehicleid">
		int - ID du véhicule à traiter
    </param>
    
    <returns>
		void
    </returns>
*/
stock GetVehicleWheels(vehicleid)
{
	if(IsValidVehicle(dVehicleInfos[vehicleid][dVehicleID]))
	{
		new tires, tire[4];
		GetVehicleDamageStatus(dVehicleInfos[vehicleid][dVehicleID], tires, tires, tires, tires);
	    tire[0] = tires & 1;
	    tire[1] = tires >> 1 & 1;
	    tire[2] = tires >> 2 & 1;
	    tire[3] = tires >> 3 & 1;
	    for(new i = 0; i < 4; i ++) dVehicleInfos[vehicleid][bWheel][i] = (tire[i] == 0) ? true : false;
	}
}

/**
	<summary>
        Change l'état des roues d'un véhicule à partir de la variable contenant le bitmask
    </summary>

    <param name="vehicleid">
		int - ID du véhicule à changer
    </param>

    <returns>
		void
    </returns>
*/
stock SetVehicleWheels(vehicleid)
{
	if(IsValidVehicle(dVehicleInfos[vehicleid][dVehicleID]))
	{
		new panels, doors, lights, tires;
		GetVehicleDamageStatus(dVehicleInfos[vehicleid][dVehicleID], panels, doors, lights, tires);
		//---
		new tire[4];
		tire[0] = (dVehicleInfos[vehicleid][bWheel][0]) ? 0 : 1;
		tire[1] = (dVehicleInfos[vehicleid][bWheel][1]) ? 0 : 1;
		tire[2] = (dVehicleInfos[vehicleid][bWheel][2]) ? 0 : 1;
		tire[3] = (dVehicleInfos[vehicleid][bWheel][3]) ? 0 : 1;
		//---
		tires = (tire[0] | (tire[1] << 1) | (tire[2] << 2) | (tire[3] << 3));
		UpdateVehicleDamageStatus(dVehicleInfos[vehicleid][dVehicleID], panels, doors, lights, tires);
	}
}

stock GetVehicleWheelsAmount(vehicleid) 
{
	return dVehicleInfos[vehicleid][dWheels];
}

stock bool:GetVehicleWheelState(vehicleid, wheelid) 
{
	return dVehicleInfos[vehicleid][bWheel][wheelid];
}

stock SetVehicleWheelState(vehicleid, wheelid, bool:state) 
{
	dVehicleInfos[vehicleid][bWheel][wheelid] = state;
}

stock GetVehicleEngine(vehicleid) 
{
	return dVehicleInfos[vehicleid][bEngine];
}

stock SetVehicleEngine(vehicleid, bool:state)
{
	dVehicleInfos[vehicleid][bEngine] = state;
}

stock PlayerStartsRepairing(playerid)
{
	if(dRepair[playerid][0] == 0)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			SendClientMessageEx(playerid, ROUGE, "You cannot fix the engine while in the vehicle!", "Vous ne pouvez pas réparer le moteur depuis un véhicule !", "¡No puede arreglar el motor si esta en un vehículo!", "Portugais", "Italien", "Allemand");
			return 1;
		}
		new Float:x, Float:y, Float:z, Float:a, vehicleid = INVALID_VEHICLE_ID;
		GetPlayerPos(playerid, x, y, z);
		GetVehicleWithinDistance(x, y, z, 7.0, vehicleid);
		if(vehicleid == INVALID_VEHICLE_ID)
		{
			SendClientMessageEx(playerid, ROUGE, "You are not near a vehicle!", "Vous n'êtes pas proche d'un véhicule !", "¡No esta cerca de un vehículo!", "Portugais", "Italien", "Allemand");
			return 1;
		}
		//---
		new Float:vHealth;
		GetVehicleHealth(vehicleid, vHealth);
		if(vHealth > 900.0)
		{
			SendClientMessageEx(playerid, ROUGE, "This vehicle doesn't need to be repaired!", "Ce véhicule n'a pas besoin de réparations !", "¡Esto vehículo no se necessita arreglos!", "Portugais", "Italien", "Allemand");
			return 1;
		}
		//---
		if(slot != 0) SwapPlayerObjects(playerid, 0, slot);
		//---
		new Float:sx, Float:sy, Float:sz, Float:fLenght;
		GetVehiclePos(vehicleid, x, y, z);
		GetVehicleZAngle(vehicleid, a);
		GetVehicleModelInfo(GetVehicleModel(vehicleid), VEHICLE_MODEL_INFO_SIZE, sx, fLenght, sz);
		sx = x, sy = y, sz = z;
		GetXYInFrontOfPoint(sx, sy, a, floatdiv(fLenght, 2));
		if(IsColBetweenTwoPos(x, y, z, sx, sy, sz))
		{
			SendClientMessageEx(playerid, ROUGE, "There is not enough room here for you to be able to repair your vehicle.", "Il n'y a pas assez de place ici pour réparer votre véhicule.", "Espagnol", "Portugais", "Italien", "Allemand");
			return 1;
		}
		//---
		SetPlayerPos(playerid, sx, sy, sz);
		GetVehiclePos(vehicleid, x, y, z);
		SetPlayerFacingToPos(playerid, x, y);
		SetCameraBehindPlayer(playerid);
		SetVehicleEngineState(vehicleid, false);
		SetVehicleBonnetState(vehicleid, true);
		//---
		TogglePlayerControllable(playerid, false);
		dRepair[playerid][0] = 20;
		dRepair[playerid][1] = -1;
		dRepair[playerid][2] = 0;
		dRepair[playerid][3] = vehicleid;
	}
}

IsPlayerNearGasStation(playerid)
{
	new Float:x, Float:y, Float:z;
	for(new i = 0; i < 19; i ++)
	{
	    GetGasStationPos(i, x, y, z);
		if(IsPlayerInRangeOfPoint(playerid, 10.0, x, y, z)) return i;
	}
	return -1;
}

public GetStationFuel(stationid)
{
	return dGasStation[stationid][dStationGas];
}

public GiveStationFuel(stationid, fuel)//Pour donner de l'essence ou en enlever d'une station essence
{
	if(dGasStation[stationid][dStationGas] + fuel > 38000) dGasStation[stationid][dStationGas] = 38000;//Si ça devient supérieur à 100, on lui met à 100
	else if(dGasStation[stationid][dStationGas] + fuel < 0) dGasStation[stationid][dStationGas] = 0;//Si ça devient inférieur à 0, on lui met à 0
    else dGasStation[stationid][dStationGas] += fuel;//Sinon, on respecte la consigne originale
	UpdateGasStationInfo(stationid);
	return dGasStation[stationid][dStationGas];
}

UpdateGasStationInfo(stationid)
{
	new string[10];
	format(string, sizeof(string), "%.2f l", floatdiv(dGasStation[stationid][dStationGas], 100));
	UpdateDynamic3DTextLabelText(dGasStation[stationid][tGasText], KAKI, string);
}

public LoadGasStations_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < 19; i ++)
	{
	    format(string, sizeof(string), "Fuel%d", i);
		INI_Int(string, dGasStation[i][dStationGas]);
	}
	return 1;
}

SaveGasStations()
{
	new string[50];
	new INI:File = INI_Open(GASPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < 19; i ++)
	{
	    format(string, sizeof(string), "Fuel%d", i);
		INI_WriteInt(File,string, dGasStation[i][dStationGas]);
	}
	INI_Close(File);
}

stock SetupGasStations()
{
	//INITIALISATION
	for(new i = 0; i < 19; i ++) dGasStation[i][tGasText] = Text3D:INVALID_3DTEXT_ID;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(GASPATH, "LoadGasStations_data");
    #endif
	for(new i = 0; i < 19; i ++)
	{
	    new Float:x, Float:y, Float:z;
	    GetGasStationPos(i, x, y, z);
	    dGasStation[i][tGasText] = CreateDynamic3DTextLabel("0 l", KAKI, x, y, z, 40.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 50.0);
	    UpdateGasStationInfo(i);
	}
    LogInfo(true, "[INIT]Stations services chargees");
}