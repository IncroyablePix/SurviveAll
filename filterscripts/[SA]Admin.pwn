/*-------------------------------------------------------------------------------//
\\  - Acteurs statiques du serveur -
//
\\  	Fonctions globales:
//          DestroyMapping(playerid)
\\          MakeGuardWave()
\\
//
//      Fonctions locales:
\\          CreateStaticActors()
//          DestroyStaticActors()
\\
//
\\
//
\\
//-------------------------------------------------------------------------------*/

#include <a_samp>
#include <streamer>
#include <actorex.inc>
#include <colandreas.inc>
#include <[SA]Defines.inc>
#include <[SA]Functions.inc>

//---FONCTIONS
forward MakeGuardWave(gateid);

//---MACROS
#define CreateDynamicObject(%0,%1,%2,%3,%4,%5,%6)	CA_CreateDynamicObject_SC(%0,%1,%2,%3,%4,%5,%6,-1,-1,-1,550.0,550.0)
#define CreateObject(%0,%1,%2,%3,%4,%5,%6)          CA_CreateObject_SC(%0,%1,%2,%3,%4,%5,%6)
#define DestroyDynamicObject(%0)                    CA_DestroyObject(%0)
#define DestroyObject(%0)                           CA_DestroyObject(%0)

//---VARIABLES
//Acteurs
enum Acteurs
{
	Gardes[MAX_GATES],
	Vendeurs[5],
	Artisans[4],
	Medecins[2],
	CampAngelPine[2],
	CampArbres[3],
	CampChilliad[2],
	CampHillTop[2],
	CampBlueberry[3],
	SoldatsArea[22]
}

static aActeur[Acteurs];//Variables des acteurs

//------------------------------------------------//

public OnFilterScriptInit()
{
	print("--------------------------------------");
	print("Survive-All static actors loader - [Pix]");
	print("- Loaded");
	print("--------------------------------------\n");
	//---ACTEURS---//
	CreateStaticActors();
	//------------------------//
	return 1;
}

public OnFilterScriptExit()
{
	print("--------------------------------------");
	print("Survive-All static actors loader - [Pix]");
	print("- Unloaded");
	print("--------------------------------------\n");
	//---DESTRUCTION ACTEURS---//
    DestroyStaticActors();
	//------------------------//
	return 1;
}

public OnActorDeath(actorid, killerid, reason)
{
}

public OnPlayerTargetActor(playerid, newtarget, oldtarget)
{
}

public OnActorSpawn(actorid)
{
}

DestroyStaticActors()
{
	for(new i = 0; i < sizeof(aActeur[Gardes]); i ++)
	{
		DestroyActor(aActeur[Gardes][i]);
		aActeur[Gardes][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[Vendeurs]); i ++)
	{
		DestroyActor(aActeur[Vendeurs][i]);
		aActeur[Vendeurs][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[Artisans]); i ++)
	{
		DestroyActor(aActeur[Artisans][i]);
		aActeur[Artisans][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[Medecins]); i ++)
	{
		DestroyActor(aActeur[Medecins][i]);
		aActeur[Medecins][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[CampAngelPine]); i ++)
	{
		DestroyActor(aActeur[CampAngelPine][i]);
		aActeur[CampAngelPine][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[CampArbres]); i ++)
	{
		DestroyActor(aActeur[CampArbres][i]);
		aActeur[CampArbres][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[CampChilliad]); i ++)
	{
		DestroyActor(aActeur[CampChilliad][i]);
		aActeur[CampChilliad] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[CampHillTop]); i ++)
	{
		DestroyActor(aActeur[CampHillTop][i]);
		aActeur[CampHillTop][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[CampBlueberry]); i ++)
	{
		DestroyActor(aActeur[CampBlueberry][i]);
		aActeur[CampBlueberry][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[SoldatsArea]); i ++)
	{
		DestroyActor(aActeur[SoldatsArea][i]);
		aActeur[SoldatsArea][i] = INVALID_ACTOR_ID;
	}
}

CreateStaticActors()
{
	//---MODÈLE---//
	/*aActeur[] = CreateActor(skinid, x, y, z, angle);
	SetActorHealth(aActeur[], 100.0);
	ApplyActorAnimation(aActeur[],"BSKTBALL", "BBALL_def_loop", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[], true);*/
	//---GARDES---//
	//Garde Commissariat Dillimore
	aActeur[Gardes][0] = CreateActor(267, 627.6102,-597.6306,19.9759,265.4852);
	SetActorHealth(aActeur[Gardes][0], 100.0);
	SetActorInvulnerable(aActeur[Gardes][0], true);
	//Garde Hilltop Farm
	aActeur[Gardes][1] = CreateActor(34, 1027.0676, -362.6758, 73.9226, 178.8914);
	SetActorHealth(aActeur[Gardes][1], 100.0);
	SetActorInvulnerable(aActeur[Gardes][1], true);
	//Garde Truc Camionneurs
	aActeur[Gardes][2] = CreateActor(44, -74.3340, -353.9449, 1.3829, 23.6952);
	SetActorHealth(aActeur[Gardes][2], 100.0);
	SetActorInvulnerable(aActeur[Gardes][2], true);
	//Garde Camp Angel Pine
	aActeur[Gardes][3] = CreateActor(32, -1957.692, -2473.299, 35.06, 180.0);
	SetActorHealth(aActeur[Gardes][3], 100.0);
	SetActorInvulnerable(aActeur[Gardes][3], true);
	//Garde Mt. Chilliad
	aActeur[Gardes][4] = CreateActor(112, -1986.5756, -1584.6461, 102.7881, 178.0614);
	SetActorHealth(aActeur[Gardes][4], 100.0);
	SetActorInvulnerable(aActeur[Gardes][4], true);
	//Garde Area 1
	aActeur[Gardes][5] = CreateActor(287, 128.119, 1940.176, 19.303, 0.0);
	SetActorHealth(aActeur[Gardes][5], 100.0);
	SetActorInvulnerable(aActeur[Gardes][5], true);
	//Garde Area 2
	aActeur[Gardes][6] = CreateActor(287, 280.4509, 1827.994, 21.4699, 240.0);
	SetActorHealth(aActeur[Gardes][6], 100.0);
	SetActorInvulnerable(aActeur[Gardes][6], true);
	//---VENDEURS---//
	//Vendeur camp Angel Pine
	aActeur[Vendeurs][0] = CreateActor(33, -1954.576, -2443.906, 30.687, 190.0);
	SetActorHealth(aActeur[Vendeurs][0], 100.0);
	SetActorInvulnerable(aActeur[Vendeurs][0], true);
	//Vendeur Hilltop Farm 1
	aActeur[Vendeurs][1] = CreateActor(7, 1054.1548, -313.1850, 73.9922, 270.4908);
	SetActorHealth(aActeur[Vendeurs][1], 100.0);
	SetActorInvulnerable(aActeur[Vendeurs][1], true);
	//Vendeur Village dans les arbres
	aActeur[Vendeurs][2] = CreateActor(73, -1376.2220, -2204.2070, 52.2109, 210.4767);
	SetActorHealth(aActeur[Vendeurs][2], 100.0);
	SetActorInvulnerable(aActeur[Vendeurs][2], true);
	//Vendeur Aldea Malvada
	aActeur[Vendeurs][3] = CreateActor(261, -1321.770, 2504.1201, 89.5699, 180.0);
	SetActorHealth(aActeur[Vendeurs][3], 100.0);
	SetActorInvulnerable(aActeur[Vendeurs][3], true);
	//Vendeur LV Nord
	aActeur[Vendeurs][4] = CreateActor(33, 991.6647, 2441.0134, 10.8342, 0.0);
	SetActorHealth(aActeur[Vendeurs][3], 100.0);
	SetActorInvulnerable(aActeur[Vendeurs][3], true);
	//---ARTISANS---//
	//Artisan Hilltop Farm
	aActeur[Artisans][0] = CreateActor(35, 1054.1260, -301.9372, 73.9922, 269.3577);
	SetActorHealth(aActeur[Artisans][0], 100.0);
	SetActorInvulnerable(aActeur[Artisans][0], true);
	//Artisan Mt. Chilliad
	aActeur[Artisans][1] = CreateActor(121, -2007.1228, -1583.7250, 86.4828, 279.9272);
	SetActorHealth(aActeur[Artisans][1], 100.0);
	SetActorInvulnerable(aActeur[Artisans][1], true);
	//Artisan Aldea Malvada
	aActeur[Artisans][2] = CreateActor(27, -1316.7290, 2509.062, 87.0419, 0.0);
	SetActorHealth(aActeur[Artisans][2], 100.0);
	SetActorInvulnerable(aActeur[Artisans][2], true);
	//Artisan Moto École
	aActeur[Artisans][2] = CreateActor(35, 1096.3636, 1279.3676, 10.8203, 90.1581);
	SetActorHealth(aActeur[Artisans][2], 100.0);
	SetActorInvulnerable(aActeur[Artisans][2], true);
	//---HABITANTS CAMP ANGEL PINE
	//Mec Couché
	aActeur[CampAngelPine][0] = CreateActor(159, -1953.5962, -2453.7654, 30.6250, 317.1696);
	SetActorHealth(aActeur[CampAngelPine][0], 100.0);
	ApplyActorAnimation(aActeur[CampAngelPine][0], "BEACH", "bather", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[CampAngelPine][0], true);
	//Mec qui fume
	aActeur[CampAngelPine][1] = CreateActor(67, -1951.9424, -2449.2439, 30.6250, 183.2719);
	SetActorHealth(aActeur[CampAngelPine][1], 100.0);
	ApplyActorAnimation(aActeur[CampAngelPine][1], "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[CampAngelPine][1], true);
	//---HABITANTS CAMP ARBRES
	//Mec devant la table
	aActeur[CampArbres][0] = CreateActor(6, -1374.8894, -2205.7324, 58.4199, 144.9760);
	SetActorHealth(aActeur[CampArbres][0], 100.0);
	SetActorInvulnerable(aActeur[CampArbres][0], true);
	//Fille qui dort
	aActeur[CampArbres][1] = CreateActor(90, -1379.2690, -2208.0569, 56.0922, 299.9492);
	SetActorHealth(aActeur[CampArbres][1], 100.0);
	ApplyActorAnimation(aActeur[CampArbres][1], "BEACH", "bather", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[CampArbres][1], true);
	//Mec qui a la tête dans l'armoire
	aActeur[CampArbres][2] = CreateActor(210, -1372.1310, -2204.8853, 58.41990, 300.7307);
	SetActorHealth(aActeur[CampArbres][2], 100.0);
	ApplyActorAnimation(aActeur[CampArbres][2], "FOOD", "SHP_Thank", 4.1, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[CampArbres][2], true);
	//---HABITANTS CAMP MT. CHILLIAD
	//Mec au coin du feu
	aActeur[CampChilliad][0] = CreateActor(122, -1997.8628, -1547.7625, 84.7032, 260.7878);
	SetActorHealth(aActeur[CampChilliad][0], 100.0);
	SetActorInvulnerable(aActeur[CampChilliad][0], true);
	//Mec contre le mur
	aActeur[CampChilliad][1] = CreateActor(206, -1998.3118, -1541.3679, 84.6095, 207.7098);
	SetActorHealth(aActeur[CampChilliad][1], 100.0);
	ApplyActorAnimation(aActeur[CampChilliad][1], "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[CampChilliad][1], true);
	//---HABITANTS CAMP HILLTOP FARM
	//Mec tout cassé
	aActeur[CampHillTop][0] = CreateActor(234, 1045.6683, -310.2458, 77.3678, 133.9725);
	SetActorHealth(aActeur[CampHillTop][0], 100.0);
	ApplyActorAnimation(aActeur[CampHillTop][0], "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[CampHillTop][0], true);
	//Mec au coin du feu
	aActeur[CampHillTop][1] = CreateActor(218, 1062.8988, -330.6720, 73.9922, 138.8883);
	SetActorHealth(aActeur[CampHillTop][1], 100.0);
	ApplyActorAnimation(aActeur[CampHillTop][1], "BEACH", "bather", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[CampHillTop][1], true);
	//---HABITANT CAMP CAMIONNEURS
	//Mec tout cassé
	aActeur[CampBlueberry][0] = CreateActor(16, 619.0773, -592.7731, 17.2330, 273.7879);
	SetActorHealth(aActeur[CampBlueberry][0], 100.0);
	ApplyActorAnimation(aActeur[CampBlueberry][0], "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[CampBlueberry][0], true);
	//Mec au coin du feu
	aActeur[CampBlueberry][1] = CreateActor(27, -4.4565, -245.5224, 5.4297, 275.8951);
	SetActorHealth(aActeur[CampBlueberry][1], 100.0);
	SetActorInvulnerable(aActeur[CampBlueberry][1], true);
	//---SOLDATS AREA
	for(new i = 0; i < 20; i ++)
	{
		aActeur[SoldatsArea][i] = CreateActor(287, 112.6439 + (1.5 * floatround(floatdiv(i, 4), floatround_floor)), 1893.5949 + (1.5 * (i % 5)), 18.419, 270.0);
		ApplyActorAnimation(aActeur[SoldatsArea][i], "WUZI", "Wuzi_stand_loop", 4.0, 1, 0, 0, 0, 0);
	}
	//---
	aActeur[SoldatsArea][20] = CreateActor(287, 183.9969, 1927.453, 17.8369, 90.0);
	ApplyActorAnimation(aActeur[SoldatsArea][20], "BLOWJOBZ", "BJ_COUCH_LOOP_P", 4.0, 1, 0, 0, 1, 0);
	//---
	aActeur[SoldatsArea][21] = CreateActor(287, 182.8529, 1927.5169, 17.8369, 270.0);
	ApplyActorAnimation(aActeur[SoldatsArea][21], "PED", "IDLE_CHAT", 4.1, 1, 1, 1, 0, 0);
	//---
	for(new i = 0; i < sizeof(aActeur[SoldatsArea]); i ++)
	{
	    SetActorHealth(aActeur[SoldatsArea][i], 100.0);
		SetActorInvulnerable(aActeur[SoldatsArea][i], true);
	}
	//---MÉDECINS
	//Médecin LV Nord
	aActeur[Medecins][0] = CreateActor(70, 939.9223, 2431.6440, 12.0331, 270.0);
	SetActorHealth(aActeur[Medecins][0], 100.0);
	SetActorInvulnerable(aActeur[Medecins][0], true);
}

public MakeGuardWave(gateid)
{
	ApplyActorAnimation(aActeur[Gardes][gateid], "ON_LOOKERS", "wave_loop", 4.0, 0, 0, 0, 0, 0);
}
