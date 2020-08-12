/*-------------------------------------------------------------------------------//
\\  - Acteurs statiques du serveur -
//
\\  	Fonctions globales:
//          GetMissionActorID(missionid)
\\          MakeGuardWave(gateid)
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
//#include <actorex.inc>
#include <colandreas.inc>
#include <[SA]Defines.inc>
#include <[SA]Functions.inc>
#undef PROFILING
#if defined PROFILING
#include <profiler.inc>
#endif

//---FONCTIONS
forward MakeGuardWave(gateid);
forward GetMissionActorID(missionid);
forward OnActorSpawn(actorid);
forward CreateActorEx(modelid, Float:x, Float:y, Float:z, Float:rotation);
forward IsActorDead(actorid);

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
	Vendeurs[7],
	Artisans[4],
	Medecins[2],
	Mecaniciens[2],
	CampAngelPine[2],
	CampArbres[3],
	CampChilliad[2],
	CampHillTop[2],
	CampBlueberry[4],
	SoldatsArea[22],
	Intro[9],
	Roof[3],
	HDV[2],
	Mission[MAX_MISSION_ACTORS]
}

static aActeur[Acteurs];//Variables des acteurs

//------------------------------------------------//

public OnFilterScriptInit()
{
	print("--------------------------------------");
	print("Survive-All static actors loader - [Pix]");
	print("- Loaded");
	print("...");
	//---ACTEURS---//
	CreateStaticActors();
	//------------------------//
	#if defined PROFILING
	Profiler_Start();
	#endif
	print("- Loaded");
	print("--------------------------------------\n");
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
	#if defined PROFILING
 	Profiler_Stop();
	#endif
	return 1;
}

/*public OnActorDeath(actorid, killerid, reason)
{
}

public OnPlayerTargetActor(playerid, newtarget, oldtarget)
{
}

public OnActorSpawn(actorid)
{
}*/

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
	for(new i = 0; i < sizeof(aActeur[Mecaniciens]); i ++)
	{
		DestroyActor(aActeur[Mecaniciens][i]);
		aActeur[Mecaniciens][i] = INVALID_ACTOR_ID;
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
	for(new i = 0; i < sizeof(aActeur[Intro]); i ++)
	{
	    DestroyActor(aActeur[Intro][i]);
	    aActeur[Intro][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[Mission]); i ++)
	{
		DestroyActor(aActeur[Mission][i]);
		aActeur[Mission][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[Roof]); i ++)
	{
		DestroyActor(aActeur[Roof][i]);
		aActeur[Roof][i] = INVALID_ACTOR_ID;
	}
	for(new i = 0; i < sizeof(aActeur[HDV]); i ++)
	{
		DestroyActor(aActeur[HDV][i]);
		aActeur[HDV][i] = INVALID_ACTOR_ID;
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
	aActeur[Gardes][2] = CreateActor(134, -74.3340, -353.9449, 1.3829, 23.6952);
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
	//KACC
	aActeur[Gardes][7] = CreateActor(287, 2499.7400, 2775.8755, 17.3244, 91.2104);
	SetActorHealth(aActeur[Gardes][7], 100.0);
	SetActorInvulnerable(aActeur[Gardes][7], true);
	//Aldea Malvada
	aActeur[Gardes][8] = CreateActor(106, -1300.2803, 2485.5779, 90.6366, 182.6908);
	SetActorHealth(aActeur[Gardes][8], 100.0);
	SetActorInvulnerable(aActeur[Gardes][8], true);
	//LV Nord
	aActeur[Gardes][9] = CreateActor(44, 1007.3849, 2463.6934, 10.8203, 180.0);
	SetActorHealth(aActeur[Gardes][9], 100.0);
	SetActorInvulnerable(aActeur[Gardes][9], true);
	//Garde Moto-école
	aActeur[Gardes][10] = CreateActor(44, 1060.7551, 1361.5688, 16.3125, 0.0);
	SetActorHealth(aActeur[Gardes][10], 100.0);
	SetActorInvulnerable(aActeur[Gardes][10], true);
	//Reggae Shark
	aActeur[Gardes][11] = CreateActor(139, -2682.1941, -2413.0906, 3.0642, 175.9613);
	SetActorHealth(aActeur[Gardes][11], 100.0);
	SetActorInvulnerable(aActeur[Gardes][11], true);
	//Grove St. Sud
	aActeur[Gardes][12] = CreateActor(105, 2485.1818, -1722.0129, 18.58, 140.0);
	SetActorHealth(aActeur[Gardes][12], 100.0);
	SetActorInvulnerable(aActeur[Gardes][12], true);
	//Grove Ouest
	aActeur[Gardes][13] = CreateActor(106, 2434.1721, -1652.4019, 13.5469, 120.0);
	SetActorHealth(aActeur[Gardes][13], 100.0);
	SetActorInvulnerable(aActeur[Gardes][13], true);
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
	SetActorHealth(aActeur[Vendeurs][4], 100.0);
	SetActorInvulnerable(aActeur[Vendeurs][4], true);
	//Vendeur Grove Street
	aActeur[Vendeurs][5] = CreateActor(107, 2453.5739, -1645.4639, 13.463, 180.0);
	SetActorHealth(aActeur[Vendeurs][5], 100.0);
	SetActorInvulnerable(aActeur[Vendeurs][5], true);
	//Vendeur Blueberry
	aActeur[Vendeurs][6] = CreateActor(67, 2.5493, -291.0689, 5.4297, 87.3347);
	SetActorHealth(aActeur[Vendeurs][6], 100.0);
	SetActorInvulnerable(aActeur[Vendeurs][6], true);
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
	aActeur[Artisans][3] = CreateActor(35, 1096.3636, 1279.3676, 10.8203, 90.1581);
	SetActorHealth(aActeur[Artisans][3], 100.0);
	SetActorInvulnerable(aActeur[Artisans][3], true);
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
	//Tom
	aActeur[CampBlueberry][2] = CreateActor(14, -36.6581, -301.7112, 5.4297, 265.4257);
	SetActorHealth(aActeur[CampBlueberry][2], 100.0);
	ApplyActorAnimation(aActeur[CampBlueberry][2], "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0); // Smoke
	SetActorInvulnerable(aActeur[CampBlueberry][2], true);
	//Bob
	aActeur[CampBlueberry][3] = CreateActor(15, -34.9341, -301.3189, 5.4229, 117.3181);
	SetActorHealth(aActeur[CampBlueberry][3], 100.0);
	ApplyActorAnimation(aActeur[CampBlueberry][3], "COP_AMBIENT", "Coplook_loop", 4.0, 0, 1, 1, 1, -1); // Arms crossed
	SetActorInvulnerable(aActeur[CampBlueberry][3], true);
	//---SOLDATS AREA
	for(new i = 0; i < 20; i ++)
	{
		aActeur[SoldatsArea][i] = CreateActor(287, 112.6439 + (1.5 * floatround(floatdiv(i, 4), floatround_floor)), 1893.5949 + (1.5 * (i % 4)), 18.419, 270.0);
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
	//Médecin Blueberry
	aActeur[Medecins][1] = CreateActor(70, -11.0567, -370.3130, 5.4297, 0.0);
	SetActorHealth(aActeur[Medecins][1], 100.0);
	SetActorInvulnerable(aActeur[Medecins][1], true);
	//---MÉCANICIENS
	//Mécano Grove Street
	aActeur[Mecaniciens][0] = CreateActor(42, 2443.8588, -1674.453, 13.527, 270.0);
	SetActorHealth(aActeur[Mecaniciens][0], 100.0);
	SetActorInvulnerable(aActeur[Mecaniciens][0], true);
	//Mécano Blueberry
	aActeur[Mecaniciens][1] = CreateActor(268, -26.5084, -227.0425, 5.4297, 306.2509);
	SetActorHealth(aActeur[Mecaniciens][1], 100.0);
	SetActorInvulnerable(aActeur[Mecaniciens][1], true);
	//---INTRO
	aActeur[Intro][0] = CreateActor(71, -1817.446, 128.763, 1055.1689, 90.0);
	SetActorHealth(aActeur[Intro][0], 100.0);
	SetActorInvulnerable(aActeur[Intro][0], true);
	//---
	aActeur[Intro][1] = CreateActor(14, -29.5357, 10.7478, 1202.3929, 357.5691);
	SetActorHealth(aActeur[Intro][1], 100.0);
	ApplyActorAnimation(aActeur[Intro][1], "PED","SEAT_IDLE", 4, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Intro][1], true);
	//---
	aActeur[Intro][2] = CreateActor(24, -28.7604, 10.8831, 1202.3929, 3.6929);
	SetActorHealth(aActeur[Intro][2], 100.0);
	ApplyActorAnimation(aActeur[Intro][2], "PED","SEAT_IDLE", 4, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Intro][2], true);
	//---
	aActeur[Intro][3] = CreateActor(290, -29.5593, 14.1250, 1202.3929, 359.6435);
	SetActorHealth(aActeur[Intro][3], 100.0);
	ApplyActorAnimation(aActeur[Intro][3], "PED","SEAT_IDLE", 4, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Intro][3], true);
	//---
	aActeur[Intro][4] = CreateActor(120,-28.7051, 14.3891, 1202.3929, 349.5235);
	SetActorHealth(aActeur[Intro][4], 100.0);
 	ApplyActorAnimation(aActeur[Intro][4], "BLOWJOBZ", "BJ_COUCH_LOOP_P", 4.0, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Intro][4], true);
	//---
	aActeur[Intro][5] = CreateActor(41, -30.2539, 15.6868, 1202.3929, 357.0946);
	SetActorHealth(aActeur[Intro][5], 100.0);
	ApplyActorAnimation(aActeur[Intro][5], "PED","SEAT_IDLE", 4, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Intro][5], true);
	//---
	aActeur[Intro][6] = CreateActor(46, -31.1198, 16.1197, 1202.3929, 358.1555);
	SetActorHealth(aActeur[Intro][6], 100.0);
	ApplyActorAnimation(aActeur[Intro][6], "BLOWJOBZ", "BJ_COUCH_LOOP_P", 4.0, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Intro][6], true);
	//---
	aActeur[Intro][7] = CreateActor(44, -31.1233, 12.3887, 1202.3929, 1.4546);//CURT CHIMAYO
	SetActorHealth(aActeur[Intro][7], 100.0);
	ApplyActorAnimation(aActeur[Intro][7], "PED","SEAT_IDLE", 4, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Intro][7], true);
	//---
	/*aActeur[Intro][8] = CreateActor(299, -37.9478,-714.0899,12.2031,254.7182);
	ApplyActorAnimation(aActeur[Intro][8], "PLAYIDLES","stretch",4.0,0,0,0, 0, 0);
	SetActorHealth(aActeur[Intro][8], 100.0);
	SetActorInvulnerable(aActeur[Intro][8], true);*/
	//---
	//aActeur[Intro][1] = CreateActor(71, -1818.5492, 125.0826, 1055.1689, 270.0);
	//SetActorHealth(aActeur[Intro][1], 100.0);
	//ApplyActorAnimation(aActeur[Intro][1],"PED","SEAT_IDLE",4,0,0,0, 1,0);
	//SetActorInvulnerable(aActeur[Intro][1], true);
	//---MISSIONS
	//Mission essence Moto-école
	aActeur[Mission][0] = CreateActor(42, 1091.7297, 1335.6772, 10.8203, 90.0);
	SetActorHealth(aActeur[Mission][0], 100.0);
	SetActorInvulnerable(aActeur[Mission][0], true);
	//Angelina
	aActeur[Mission][1] = CreateActor(55, -1301.6082, 2516.6787, 87.1649, 180.0);
	SetActorHealth(aActeur[Mission][1], 100.0);
	SetActorInvulnerable(aActeur[Mission][1], true);
	//REGGAE SHARK
	aActeur[Mission][2] = CreateActor(183, -2674.4809, -2383.3168, 3.0000, 180.0);
	SetActorHealth(aActeur[Mission][2], 100.0);
	//ApplyActorAnimation(aActeur[ReggaeShark][0],"PED", "SEAT_IDLE", 4.1, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Mission][2], true);
	//---
	aActeur[Mission][3] = CreateActor(220, -2681.645, -2396.3659, 3.0000, 90.0);
	SetActorHealth(aActeur[Mission][3], 100.0);
	ApplyActorAnimation(aActeur[Mission][3], "PED", "IDLE_CHAT", 4.1, 1, 1, 1, 0, 0);
	SetActorInvulnerable(aActeur[Mission][3], true);
	//---
	aActeur[Mission][4] = CreateActor(142, -2682.717, -2396.6789, 3.0000, 300.0);
	SetActorHealth(aActeur[Mission][4], 100.0);
	ApplyActorAnimation(aActeur[Mission][4], "PED", "IDLE_CHAT", 4.1, 1, 1, 1, 0, 0);
	SetActorInvulnerable(aActeur[Mission][4], true);
	//---
	aActeur[Mission][5] = CreateActor(241, -2682.133, -2383.447, 3.0000, 180.0);
	SetActorHealth(aActeur[Mission][5], 100.0);
	//ApplyActorAnimation(aActeur[ReggaeShark][3],"PED", "SEAT_IDLE", 4.1, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Mission][5], true);
	//---
	aActeur[Mission][6] = CreateActor(136, -2664.0793, -2397.3757, 3.0000, 255.9941);
	SetActorHealth(aActeur[Mission][6], 100.0);
 	ApplyActorAnimation(aActeur[Mission][6],"GRAVEYARD","mrnM_loop", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[Mission][6], true);
	//---CARIBOU
	aActeur[Mission][7] = CreateActor(296, -697.9556, -2097.4727, 30.1593, 229.7047);
	SetActorHealth(aActeur[Mission][7], 100.0);
	SetActorInvulnerable(aActeur[Mission][7], true);
	//---GARDE AÉROPORT
	aActeur[Mission][8] = CreateActor(71, -1817.4589, 125.1279, 1055.1689, 90.0);
	SetActorHealth(aActeur[Mission][8], 100.0);
	SetActorInvulnerable(aActeur[Mission][8], true);
	//---CURT MATTERS
	aActeur[Mission][9] = CreateActor(44, -36.8606, -714.2477, 12.2031, 77.0763);
	SetActorHealth(aActeur[Mission][9], 100.0);
	//ApplyActorAnimation(aActeur[Mission][9],"SWAT", "gnstwall_injurd", 4.0, 1, 0, 0, 0, 0);
 	ApplyActorAnimation(aActeur[Mission][9], "KNIFE", "KILL_Knife_Ped_Die", 4.1, 0, 1, 1, 1, 1);
	SetActorInvulnerable(aActeur[Mission][9], true);
	//---SCIENTIFIQUE
	aActeur[Mission][10] = CreateActor(70, 145.7800, 1835.6778, 17.6460, 270.9321);
	SetActorHealth(aActeur[Mission][10], 100.0);
	ApplyActorAnimation(aActeur[Mission][10],"BSKTBALL", "BBALL_def_loop", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[Mission][10], true);
	//---KEN
	aActeur[Mission][11] = CreateActor(132, 15.8516, -222.6447, 5.4297, 138.3376);
	SetActorHealth(aActeur[Mission][11], 100.0);
	//ApplyActorAnimation(aActeur[Mission][11],"BSKTBALL", "BBALL_def_loop", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[Mission][11], true);
	//---SOLDAT GATE
	aActeur[Mission][12] = CreateActor(287, 211.6714, 1812.2910, 21.8594, 1.0963);
	SetActorHealth(aActeur[Mission][12], 100.0);
	//ApplyActorAnimation(aActeur[Mission][12],"BSKTBALL", "BBALL_def_loop", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[Mission][12], true);
	//---aBerf
	aActeur[Mission][13] = CreateActor(76, -2959.5332, 1155.4758, 13.9609, 273.0178); // aBerf
	SetActorHealth(aActeur[Mission][13], 100.0);
	//ApplyActorAnimation(aActeur[Mission][13],"BSKTBALL", "BBALL_def_loop", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[Mission][13], true);
	//Mission Essence Hilltop
	aActeur[Mission][14] = CreateActor(8, 1023.6859, -315.6013, 73.9889, 180.0);
	SetActorHealth(aActeur[Mission][14], 100.0);
    ApplyActorAnimation(aActeur[Mission][14], "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0); // Smoke
	//ApplyActorAnimation(aActeur[Mission][13],"BSKTBALL", "BBALL_def_loop", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[Mission][14], true);
	//Enola
	aActeur[Mission][15] = CreateActor(197, 1923.2904, 167.6050, 37.2573, 72.6150);
	SetActorHealth(aActeur[Mission][15], 100.0);
    ApplyActorAnimation(aActeur[Mission][15], "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0); // Smoke
	SetActorInvulnerable(aActeur[Mission][15], true);
	//Frank
	aActeur[Mission][16] = CreateActor(265, 613.5021, -592.9421, 17.2330, 356.1874);
	SetActorHealth(aActeur[Mission][16], 100.0);
    ApplyActorAnimation(aActeur[Mission][16], "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0); // Smoke
	SetActorInvulnerable(aActeur[Mission][16], true);
	//Gedevan
	aActeur[Mission][17] = CreateActor(79, 2146.9448, 1127.4003, 24.3125, 278.5058);
	SetActorHealth(aActeur[Mission][17], 100.0);
	ApplyActorAnimation(aActeur[Mission][17], "PED","WEAPON_crouch", 4.1, 1, 0, 0, 1, 0);// Smoke
	SetActorInvulnerable(aActeur[Mission][17], true);
	//Ace
	aActeur[Mission][18] = CreateActor(79, 1081.7371, -314.3696, 73.9922, 198.1821);
	SetActorHealth(aActeur[Mission][18], 100.0);
    ApplyActorAnimation(aActeur[Mission][18], "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0); // Smoke
	SetActorInvulnerable(aActeur[Mission][18], true);
	//Roger
	aActeur[Mission][19] = CreateActor(181, 415.2504, 2537.1272, 19.1484, 176.8710);
	SetActorHealth(aActeur[Mission][19], 100.0);
    //ApplyActorAnimation(aActeur[Mission][18], "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0); // Smoke
	SetActorInvulnerable(aActeur[Mission][19], true);
	//Roger
	aActeur[Mission][20] = CreateActor(8, -2084.3752, 889.0592, 81.6239, 270.9654);
	SetActorHealth(aActeur[Mission][20], 100.0);
    //ApplyActorAnimation(aActeur[Mission][18], "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0); // Smoke
	SetActorInvulnerable(aActeur[Mission][20], true);
	//Roger
	aActeur[Mission][21] = CreateActor(288, -1298.4633, 2537.4341, 87.7422, 174.9482);
	SetActorHealth(aActeur[Mission][21], 100.0);
    ApplyActorAnimation(aActeur[Mission][21], "BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0); // Smoke
	SetActorInvulnerable(aActeur[Mission][21], true);
	//Marty
	aActeur[Mission][22] = CreateActor(19, 1173.1464, 1346.2797, 10.9219, 358.5773);
	SetActorHealth(aActeur[Mission][22], 100.0);
	ApplyActorAnimation(aActeur[Mission][22], "ped", "cower", 4.1, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Mission][22], true);
	//---TRAVAILLEURS
	//CHEF
	aActeur[Mission][23] = CreateActor(153, 725.5118, 2062.4067, 13.5628, 269.9786);
	SetActorHealth(aActeur[Mission][23], 100.0);
	ApplyActorAnimation(aActeur[Mission][23], "DEALER", "DEALER_IDLE_01", 4.0, 1, 1, 1, 1, 0);
	SetActorInvulnerable(aActeur[Mission][23], true);
	//---
	aActeur[Mission][24] = CreateActor(260, 736.3357, 2053.4021, 8.5912, 56.4283);
	SetActorHealth(aActeur[Mission][24], 100.0);
	ApplyActorAnimation(aActeur[Mission][24], "BOMBER", "BOM_Plant", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[Mission][24], true);
	//---
	aActeur[Mission][25] = CreateActor(16, 738.8035, 2075.1892, 9.9881, 128.4473);
	SetActorHealth(aActeur[Mission][25], 100.0);
	ApplyActorAnimation(aActeur[Mission][25], "BOMBER", "BOM_Plant", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[Mission][25], true);
	//---
	aActeur[Mission][26] = CreateActor(27, 739.9588, 2060.1140, 9.0628, 175.6644);
	SetActorHealth(aActeur[Mission][26], 100.0);
	ApplyActorAnimation(aActeur[Mission][26], "FAT", "IDLE_tired", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[Mission][26], true);
	//---JOCK
	aActeur[Mission][27] = CreateActor(25, 254.0149, -54.7119, 5.8828, 180.0);
	SetActorHealth(aActeur[Mission][27], 100.0);
	ApplyActorAnimation(aActeur[Mission][27], "FAT", "IDLE_tired", 4.0, 1, 0, 0, 0, 0);
	SetActorInvulnerable(aActeur[Mission][27], true);
	//---BIKER
	aActeur[Mission][28] = CreateActor(248, -472.0905, -53.9370, 60.2103, 240.0);
	SetActorHealth(aActeur[Mission][28], 100.0);
	ApplyActorAnimation(aActeur[Mission][28], "ped", "cower", 4.1, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Mission][28], true);
	//-------//[3],
	aActeur[Roof][0] = CreateActor(139, 256.1857, -54.8130, 5.8828, 157.7371);
	SetActorHealth(aActeur[Roof][0], 100.0);
	ApplyActorAnimation(aActeur[Roof][0], "ped", "cower", 4.1, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Roof][0], true);
	//Angelina
	aActeur[Roof][1] = CreateActor(140, 252.3440, -55.7297, 5.8828, 238.2645);
	SetActorHealth(aActeur[Roof][1], 100.0);
	ApplyActorAnimation(aActeur[Roof][1], "ped", "cower", 4.1, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Roof][1], true);
	//REGGAE SHARK
	aActeur[Roof][2] = CreateActor(92, 252.9092, -55.0697, 5.8828, 226.8722);
	SetActorHealth(aActeur[Roof][2], 100.0);
	ApplyActorAnimation(aActeur[Roof][2], "ped", "cower", 4.1, 0, 0, 0, 1, 0);
	SetActorInvulnerable(aActeur[Roof][2], true);
	//---HDV
	//Gedevan
	aActeur[HDV][0] = CreateActor(79, -69.3038,-372.8597,5.4659,270.1075);
	SetActorHealth(aActeur[HDV][0], 100.0);
	SetActorInvulnerable(aActeur[HDV][0], true);
	//Ace
	aActeur[HDV][1] = CreateActor(79, -69.3038,-370.4025,5.4659,270.1075);
	SetActorHealth(aActeur[HDV][1], 100.0);
	SetActorInvulnerable(aActeur[HDV][1], true);
/*AddPlayerClass(299,-19.2647,-277.1054,6.1050,175.6355,0,0,0,0,0,0); // Syd
AddPlayerClass(299,-20.0700,-279.5252,5.4297,339.2256,0,0,0,0,0,0); // JoueurSyd
AddPlayerClass(299,-18.3787,-279.5391,5.4297,11.3608,0,0,0,0,0,0); // EveSyd
AddPlayerClass(299,-29.8982,-300.6817,5.4229,149.6816,0,0,0,0,0,0); // JoueurEve
AddPlayerClass(299,-31.0457,-302.7295,5.4229,325.1487,0,0,0,0,0,0); // EveJoueur*/
}

public GetMissionActorID(missionid)
{
	return (sizeof(aActeur[Mission]) > missionid > 0) ? aActeur[Mission][missionid] : INVALID_ACTOR_ID;
}

public MakeGuardWave(gateid)
{
	ApplyActorAnimation(aActeur[Gardes][gateid], "ON_LOOKERS", "wave_loop", 4.0, 0, 0, 0, 0, 0);
}

public OnPlayerGiveDamageActor(playerid, damaged_actorid, Float: amount, weaponid, bodypart)
{
    if(!IsValidActor(damaged_actorid) || (!(0 <= amount <= 500.0)) || (!(0 <= weaponid <= 42)))
    {
        return 0;
    }
    else if(!IsActorInvulnerable(damaged_actorid))
    {
        new Float:health;
        GetActorHealth(damaged_actorid, health);
        SetActorHealth(damaged_actorid, health - amount);
		//---
        if(health - amount < 1.0)
        {
            ClearActorAnimations(damaged_actorid);
            SetActorHealth(damaged_actorid, 0.0);
            CallRemoteFunction("OnActorDeath", "iii", damaged_actorid, playerid, weaponid);
        }
    }
    return 1;
}

//---FONCTIONS SUPPLÉMENTAIRES---//
public OnActorSpawn(actorid)
{
}

public CreateActorEx(modelid, Float:x, Float:y, Float:z, Float:rotation)
{
    new actorid = INVALID_ACTOR_ID;
	//---
    if(0 <= modelid <= 311 || modelid >= 20000)
    {
        actorid = CreateActor(modelid, x, y, z, rotation);
		//---
        if(actorid != INVALID_ACTOR_ID)
        {
            PreloadActorAnimations(actorid);
            CallLocalFunction("OnActorSpawn", "i", actorid);
        }
    }
    return actorid;
}

public IsActorDead(actorid)
{
    if(IsValidActor(actorid))
    {
        new Float:health;
        GetActorHealth(actorid, health);
        return (health <= 0.0);
    }
    return 0;
}

stock static PreloadActorAnimations(actorid)
{
		new s_AnimationLibraries[][] =
		{
  			{!"AIRPORT"},    	{!"ATTRACTORS"},   	{!"BAR"},          	{!"BASEBALL"},
		    {!"BD_FIRE"},    	{!"BEACH"},        	{!"BENCHPRESS"},   	{!"BF_INJECTION"},
		    {!"BIKED"},      	{!"BIKEH"},        	{!"BIKELEAP"},     	{!"BIKES"},
	      	{!"BIKEV"},      	{!"BIKE_DBZ"},     	{!"BMX"},          	{!"BOMBER"},
	      	{!"BOX"},        	{!"BSKTBALL"},     	{!"BUDDY"},        	{!"BUS"},
	      	{!"CAMERA"},     	{!"CAR"},          	{!"CARRY"},        	{!"CAR_CHAT"},
	      	{!"CASINO"},     	{!"CHAINSAW"},     	{!"CHOPPA"},       	{!"CLOTHES"},
	      	{!"COACH"},      	{!"COLT45"},       	{!"COP_AMBIENT"},  	{!"COP_DVBYZ"},
	      	{!"CRACK"},      	{!"CRIB"},         	{!"DAM_JUMP"},     	{!"DANCING"},
	      	{!"DEALER"},     	{!"DILDO"},       	{!"DODGE"},        	{!"DOZER"},
	      	{!"DRIVEBYS"},   	{!"FAT"},          	{!"FIGHT_B"},      	{!"FIGHT_C"},
	      	{!"FIGHT_D"},   	{!"FIGHT_E"},      	{!"FINALE"},       	{!"FINALE2"},
	      	{!"FLAME"},      	{!"FLOWERS"},      	{!"FOOD"},         	{!"FREEWEIGHTS"},
	      	{!"GANGS"},      	{!"GHANDS"},       	{!"GHETTO_DB"},    	{!"GOGGLES"},
	      	{!"GRAFFITI"},   	{!"GRAVEYARD"},    	{!"GRENADE"},      	{!"GYMNASIUM"},
	      	{!"HAIRCUTS"},   	{!"HEIST9"},       	{!"INT_HOUSE"},    	{!"INT_OFFICE"},
	      	{!"INT_SHOP"},   	{!"JST_BUISNESS"}, 	{!"KART"},         	{!"KISSING"},
	      	{!"KNIFE"},      	{!"LAPDAN1"},      	{!"LAPDAN2"},      	{!"LAPDAN3"},
	      	{!"LOWRIDER"},   	{!"MD_CHASE"},     	{!"MD_END"},       	{!"MEDIC"},
	      	{!"MISC"},       	{!"MTB"},          	{!"MUSCULAR"},     	{!"NEVADA"},
	      	{!"ON_LOOKERS"}, 	{!"OTB"},          	{!"PARACHUTE"},    	{!"PARK"},
	      	{!"PAULNMAC"},   	{!"PED"},          	{!"PLAYER_DVBYS"}, 	{!"PLAYIDLES"},
	      	{!"POLICE"},     	{!"POOL"},         	{!"POOR"},         	{!"PYTHON"},
	      	{!"QUAD"},       	{!"QUAD_DBZ"},     	{!"RAPPING"},      	{!"RIFLE"},
	      	{!"RIOT"},       	{!"ROB_BANK"},     	{!"ROCKET"},       	{!"RUSTLER"},
	      	{!"RYDER"},      	{!"SCRATCHING"},   	{!"SHAMAL"},       	{!"SHOP"},
	      	{!"SHOTGUN"},    	{!"SILENCED"},     	{!"SKATE"},        	{!"SMOKING"},
	      	{!"SNIPER"},     	{!"SPRAYCAN"},     	{!"STRIP"},        	{!"SUNBATHE"},
	      	{!"SWAT"},       	{!"SWEET"},        	{!"SWIM"},         	{!"SWORD"},
	      	{!"TANK"},       	{!"TATTOOS"},      	{!"TEC"},          	{!"TRAIN"},
	      	{!"TRUCK"},      	{!"UZI"},          	{!"VAN"},          	{!"VENDING"},
	      	{!"VORTEX"},     	{!"WAYFARER"},     	{!"WEAPONS"},      	{!"WUZI"},
	      	{!"WOP"},        	{!"GFUNK"},        	{!"RUNNINGMAN"}
		};
        for(new i = 0; i < sizeof(s_AnimationLibraries); i ++)
        {
            ApplyActorAnimation(actorid, s_AnimationLibraries[i], "null", 0.0, 0, 0, 0, 0, 0);
        }
}