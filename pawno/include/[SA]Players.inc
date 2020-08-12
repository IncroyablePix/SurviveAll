#if defined PLAYERS_INCLUDED

	#endinput

#endif

#define PLAYERS_INCLUDED

//---INCLUDES---//
#include <a_samp>
#include <[SA]TextDraws.inc>
#include <[SA]Defines.inc>
#include <[SA]TextDraws.inc>

//---DEFINES---//

//---MACROS---//
#define IsPlayerDead(%0)                    (pPlayerInfos[%0][pHealth] <= 0)
#define GetPlayerHealth(%0)                 (pPlayerInfos[%0][pHealth])
#define GetPlayerArmour(%0)					(pPlayerInfos[%0][pArmour])

//---FORWARDS---//
forward HasPlayerItem(playerid, objectid);
forward GetPlayerNextFreeSlot(playerid);
forward GetPlayerFreeSlots(playerid);
forward GivePlayerSlotObject(playerid, objectid, slot);
forward HasPlayerSameTypeWeapon(playerid, weaponid);
forward GivePlayerWeaponEx(playerid, weaponid, ammo);
forward GetPlayerNextFreeWeaponSlot(playerid);
forward LoadUserGold_auctions(name[], value[]);
forward LoadUserItems_auctions(name[],value[]);
forward LoadUserOffline_auctions(playerid, name[], value[]);
forward GivePlayerGold(playerid, amount);
forward GivePlayerExp(playerid, exp);
forward bool:IsThereUnsoldItem(playerid);
forward bool:GetPlayerLegState(playerid);
forward bool:GetPlayerBleedingState(playerid);
forward ShowPlayerHelp(playerid, helpid, time);//Fonction pour afficher une partie de l'aide.
forward HidePlayerHelp(playerid);//Fonction pour cacher l'aide
forward bool:GiveOfflineItems(playerid);

//---ENUMS---//
enum PlayerInfos
{
	dLogState,
	pBan,
	pVIP[2],
	sFirstCo[30],
	sLastCo[30],
	pFreeze,
	pMute,
	pPass,
	pLangue,
	pBag,//1 = PATROL PACK (8)--- 2 = CZECH VEST POUCH (16)--- 3 = ASSAULT PACK (20)--- 4 = ALICE BACKPACK (28)--- 5 = COYOTE BACKPACK (36)
	pHunger,
	pThirst,
	pSleep,
 	pGold,
	pHealth,
	pArmour,
	Float:xPos,
	Float:yPos,
	Float:zPos,
	Float:aPos,
	bool:bLeg,
	bool:bHemorragie,
	pTemperature,
 	pAdmin,
 	pKills,
 	pHeadshots,
 	pDeaths,
 	pZombies,
 	pBosses,
 	pMissions,
	Float:fDistanceWheel,
	Float:fDistanceFoot,
 	pGameTime,
	pSkin,
	pChapeau,
	Float:fPosChapeau[9],
	pLunettes,
	Float:fPosLunettes[9],
	pMasque,
	Float:fPosMasque[9],
	pTorse,
	Float:fPosTorse[9],
	pArme1[3],
	pArme2[3],
	pArme3[3],
	pArme4[3],
	HandObject,
	BagObject[36],
	pLevel,
	pExp,
	pCompetence,
	bool:bAide[32],
	dAide1_16,
	dAide17_32,
	dBoucher,
	dMedecine,
	dSante,
	dArtisan,
	dTransporteur,
	dPecheur,
	dMecano,
	dJardinier,
	dAthlete,
	dSurvivaliste,
	dTank,
	dBomberman,
	dHydra,
	pReggaeShark,
	pIntro,
	pAmy,
	pKen,
	pDPO
}

enum PlayerOffInfos
{
	//---HDV
	dRecievedGold,
	dReturnedItem[50]
}

//---VARIABLES---//
static pPlayerInfos[MAX_PLAYERS][PlayerInfos];//Variables du joueur (Sauvegardes, etc.)
static pPlayerOfflineInfos[MAX_PLAYERS][PlayerOffInfos];//Variables du joueur (Sauvegardes, etc.)

static lastco[30];

//---FONCTIONS---//
stock bool:GiveOfflineItems(playerid)
{
	new bool:bItemFound = false;
	for(new i = 0; i < 50; i ++)
	{
		if(pPlayerOfflineInfos[playerid][dReturnedItem][i] != 0)
		{
			PlayerDropObject(playerid, pPlayerOfflineInfos[playerid][dReturnedItem][i], floatdiv(RandomEx(5, 20), 10));
			pPlayerOfflineInfos[playerid][dReturnedItem][i] = 0;
			if(!bItemFound) bItemFound = true;
		}
	}

	return bItemFound;
}

stock GivePlayerHandObject(playerid, objectid)//Fonction pour give un objet à un mec dans son main
{
	if(objectid == -1 || objectid == 0)//Si l'objet est nul, on remet ses variables à 0
	{
	    if(pPlayerInfos[playerid][HandObject] != 0) UpdatePlayerHand(playerid, 0);
		pPlayerInfos[playerid][HandObject] = 0;
	}
	else
	{
		if(pPlayerInfos[playerid][HandObject] != objectid) UpdatePlayerHand(playerid, objectid);
		pPlayerInfos[playerid][HandObject] = objectid;
		if(!pPlayerInfos[playerid][bAide][1]) ShowPlayerHelp(playerid, 2, 10000);
	}
	return 1;
}

stock GivePlayerInventoryObject(playerid, objectid, slotid)//Fonction pour give un objet à un mec dans son sac
{
	if(objectid == -1 || objectid == 0)//Si l'objet est nul, on remet ses variables à 0
	{
	    if(pPlayerInfos[playerid][BagObject][slotid] != 0) UpdatePlayerInventory(playerid, slotid, 0);
		pPlayerInfos[playerid][BagObject][slotid] = 0;
	}
	else
	{
		if(pPlayerInfos[playerid][BagObject][slotid] != objectid) UpdatePlayerInventory(playerid, slotid, objectid);
		pPlayerInfos[playerid][BagObject][slotid] = objectid;
		if(!pPlayerInfos[playerid][bAide][1]) ShowPlayerHelp(playerid, 2, 10000);
	}
	UpdatePlayerInventorySlots(playerid);
	return 1;
}

stock GivePlayerTrunkObject(playerid, vehicleid, objectid, slotid)//Fonction pour foutre un objet dans le coffre d'une bagnole
{
	if(vehicleid != -1)
	{
		if(objectid == -1 || objectid == 0)//Si l'objet est nul, on remet ses variables à 0
		{
		    if(playerid != INVALID_PLAYER_ID && dVehicleInfos[vehicleid][TrunkObject][slotid] != 0) UpdateVehicleInventory(playerid, vehicleid, slotid, 0);
			dVehicleInfos[vehicleid][TrunkObject][slotid] = 0;
		}
		else
		{
			if(playerid != INVALID_PLAYER_ID && dVehicleInfos[vehicleid][TrunkObject][slotid] != objectid) UpdateVehicleInventory(playerid, vehicleid, slotid, objectid);
			dVehicleInfos[vehicleid][TrunkObject][slotid] = objectid;
		}
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++)
		{
		    if(i != playerid && GetPlayerVehicleInventory(i) == vehicleid)
		    {
				UpdateVehicleInventory(i, vehicleid, slotid, (objectid == -1 || objectid == 0) ? 0 : objectid);
		    }
		}
	}
	return 1;
}

stock GetPlayerBagSize(playerid, bool:objects)
{
	switch(pPlayerInfos[playerid][pBag])
	{
	    case 1: return (objects ? 8 : 2);
	    case 2: return (objects ? 16 : 2);
	    case 3: return (objects ? 20 : 3);
	    case 4: return (objects ? 28 : 4);
	    case 5: return (objects ? 36 : 4);
	}
	return 1;
}

public HasPlayerItem(playerid, objectid)
{
	new dBagSize = GetPlayerBagSize(playerid, true);
	for(new i = 0; i < dBagSize + 1; i ++) if(GetPlayerSlotObject(playerid, i) == objectid) return i;
	return -1;
}

public GetPlayerNextFreeSlot(playerid)
{
	new dBagSize = GetPlayerBagSize(playerid, true);
	for(new i = 0; i < dBagSize + 1; i ++) if(GetPlayerSlotObject(playerid, i) == 0) return i;
	return -1;
}

public GetPlayerFreeSlots(playerid)
{
	new dFreeSlots;
	new dBagSize = GetPlayerBagSize(playerid, true);
	for(new i = 0; i < dBagSize + 1; i ++) if(GetPlayerSlotObject(playerid, i) == 0) dFreeSlots ++;
	return dFreeSlots;
}

stock GetPlayerSlotObject(playerid, slot)
{
	switch(slot)
	{
	    case 0, 37: return pPlayerInfos[playerid][HandObject];
	    case 1 .. 36: return pPlayerInfos[playerid][BagObject][slot - 1];
		case 38 .. 43: 
		{
			new pVehicleInventory = GetPlayerVehicleInventory(playerid);
			return (pVehicleInventory != -1) ? dVehicleInfos[pVehicleInventory][TrunkObject][slot - 38] : 0;
		}
	    case 44 .. 55: return GetPlayerSafeItem(pPlayerSafe[playerid], slot - 44);
	}
	return 0;
}

stock GetPlayerSafeUse(playerid) 
{
    return IsValidPlayer(playerid) ? pPlayerSafe[playerid] : -1;
}

stock SetPlayerSafeUse(playerid, useInventory) 
{
    if(IsValidPlayer(playerid)) 
    {
        pPlayerSafe[playerid] = useInventory;
    }
}

public GivePlayerSlotObject(playerid, objectid, slot)
{
	switch(slot)
	{
	    case 0, 37:
		{
			GivePlayerHandObject(playerid, objectid);
		}
	    case 1 .. 36:
		{
			GivePlayerInventoryObject(playerid, objectid, slot - 1);
		}
	    case 38 .. 43:
		{
			new pVehicleInventory = GetPlayerVehicleInventory(playerid);
			if(objectid == 0) LogInfo(true, "[ADMIN]%s prend %s du vehicule %d.", GetName(playerid), NoNewLineSign(aObjects[GetPlayerSlotObject(playerid, slot)][ObjectFrName]), pVehicleInventory);
			else LogInfo(true, "[ADMIN]%s met %s dans le vehicule %d.", GetName(playerid), NoNewLineSign(aObjects[objectid][ObjectFrName]), pVehicleInventory);
			GivePlayerTrunkObject(playerid, pVehicleInventory, objectid, slot - 38);
		}
	    case 44 .. 55:
		{
			if(objectid == 0) LogInfo(true, "[ADMIN]%s prend %s du coffre %d.", GetName(playerid), NoNewLineSign(aObjects[GetPlayerSlotObject(playerid, slot)][ObjectFrName]), GetPlayerSafeUse(playerid));
			else LogInfo(true, "[ADMIN]%s met %s dans le coffre %d.", GetName(playerid), NoNewLineSign(aObjects[objectid][ObjectFrName]), GetPlayerSafeUse(playerid));
			GivePlayerSafeObject(playerid, GetPlayerSafeUse(playerid), objectid, slot - 44);
		}
	}
}

stock GetPlayerWeaponEx(playerid, slotid) 
{
	switch(slotid)
	{
		case 1: return pPlayerInfos[playerid][pArme1][0];
		case 2: return pPlayerInfos[playerid][pArme2][0];
		case 3: return pPlayerInfos[playerid][pArme3][0];
		case 4: return pPlayerInfos[playerid][pArme4][0];
	}

	return 0;
}

stock GetPlayerWeaponAmmoEx(playerid, slotid) 
{
	switch(slotid)
	{
		case 1: return pPlayerInfos[playerid][pArme1][1];
		case 2: return pPlayerInfos[playerid][pArme2][1];
		case 3: return pPlayerInfos[playerid][pArme3][1];
		case 4: return pPlayerInfos[playerid][pArme4][1];
	}

	return 0;
}

public GetPlayerNextFreeWeaponSlot(playerid)
{
	new dWeapons[4];
	dWeapons[0] = pPlayerInfos[playerid][pArme1][0];
	dWeapons[1] = pPlayerInfos[playerid][pArme2][0];
	dWeapons[2] = pPlayerInfos[playerid][pArme3][0];
	dWeapons[3] = pPlayerInfos[playerid][pArme4][0];
	for(new i = 0; i < GetPlayerBagSize(playerid, false); i ++) if(dWeapons[i] == 0) return i + 1;
	return 0;
}

stock HasPlayerWeapon(playerid, weaponid)
{
	if(pPlayerInfos[playerid][pArme1][0] == weaponid) return 1;
	else if(pPlayerInfos[playerid][pArme2][0] == weaponid) return 2;
	else if(pPlayerInfos[playerid][pArme3][0] == weaponid) return 3;
	else if(pPlayerInfos[playerid][pArme4][0] == weaponid) return 4;
	return 0;
}

stock ApplyReloadAnim(playerid, weaponid)
{
	if(IsPlayerConnected(playerid) && HasPlayerWeapon(playerid, weaponid))
	{
	    SetPlayerArmedWeapon(playerid, 0);
		switch(weaponid)
		{
		    case 22: ApplyAnimation(playerid,"COLT45", "colt45_reload", 4.0, 0, 0, 0, 0, 0);//Pistolet
		    case 23: ApplyAnimation(playerid,"SILENCED", "Silence_reload", 4.0, 0, 0, 0, 0, 0);//Silencieux
		    case 24: ApplyAnimation(playerid,"PYTHON", "python_reload", 4.0, 0, 0, 0, 0, 0);//Deagle
		    case 25: ApplyAnimation(playerid,"SHOTGUN", "shotgun_fire_poor", 4.0, 0, 0, 0, 0, 0);//Fusil à pompe
		    case 26: ApplyAnimation(playerid,"COLT45", "sawnoff_reload", 4.0, 0, 0, 0, 0, 0);//Sawn off
		    case 27: ApplyAnimation(playerid,"SHOTGUN", "shotgun_fire_poor", 4.0, 0, 0, 0, 0, 0);//Spas 12
		    case 28: ApplyAnimation(playerid,"UZI", "UZI_reload", 4.0, 0, 0, 0, 0, 0);//UZI
		    case 29: ApplyAnimation(playerid,"PYTHON","python_reload", 4.0, 0, 0, 0, 0, 0);//MP5
		    case 32: ApplyAnimation(playerid,"TEC", "TEC_reload", 4.0, 0, 0, 0, 0, 0);//TEC9
		    case 30: ApplyAnimation(playerid,"PYTHON","python_reload", 4.0, 0, 0, 0, 0, 0);//AK47
		    case 31: ApplyAnimation(playerid,"PYTHON","python_reload", 4.0, 0, 0, 0, 0, 0);//M4
		    case 33: ApplyAnimation(playerid,"RIFLE", "RIFLE_load", 4.0, 0, 0, 0, 0, 0);//Fusil de chasse
		    case 34: ApplyAnimation(playerid,"RIFLE", "RIFLE_load", 4.0, 0, 0, 0, 0, 0);//Sniper
		    case 38: ApplyAnimation(playerid,"PYTHON","python_reload", 4.0, 0, 0, 0, 0, 0);//Minigun
		}
		PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
	    SetPlayerArmedWeapon(playerid, weaponid);
	}
	return 1;
}

public HasPlayerSameTypeWeapon(playerid, weaponid)//Retourne 0 si le mec n'a pas d'armes du même type, 1 si le mec a une arme du même type, 2 si le mec a l'arme en question
{
	switch(weaponid)
	{
	    case 3, 4, 5, 8, 9: if(HasPlayerWeapon(playerid, 3) || HasPlayerWeapon(playerid, 4) || HasPlayerWeapon(playerid, 5) || HasPlayerWeapon(playerid, 8) || HasPlayerWeapon(playerid, 9))//ARMES BLANCHES
		{
			if(HasPlayerWeapon(playerid, weaponid)) return 2;
			else return 1;
		}
	    case 22, 23, 24: if(HasPlayerWeapon(playerid, 22) || HasPlayerWeapon(playerid, 23) || HasPlayerWeapon(playerid, 24))//PISTOLETS
		{
			if(HasPlayerWeapon(playerid, weaponid)) return 2;
			else return 1;
		}
	    case 25, 26, 27: if(HasPlayerWeapon(playerid, 25) || HasPlayerWeapon(playerid, 26) || HasPlayerWeapon(playerid, 27))//FUSILS À POMPE
		{
			if(HasPlayerWeapon(playerid, weaponid)) return 2;
			else return 1;
		}
	    case 28, 29, 32: if(HasPlayerWeapon(playerid, 28) || HasPlayerWeapon(playerid, 29) || HasPlayerWeapon(playerid, 32))//PISTOLETS MITRAILLEURS
		{
			if(HasPlayerWeapon(playerid, weaponid)) return 2;
			else return 1;
		}
	    case 30, 31: if(HasPlayerWeapon(playerid, 30) || HasPlayerWeapon(playerid, 31))//FUSILS D'ASSAUT
		{
			if(HasPlayerWeapon(playerid, weaponid)) return 2;
			else return 1;
		}
	    case 33, 34: if(HasPlayerWeapon(playerid, 33) || HasPlayerWeapon(playerid, 34))//FUSILS DE PRÉCISION
		{
			if(HasPlayerWeapon(playerid, weaponid)) return 2;
			else return 1;
		}
	    case 38: if(HasPlayerWeapon(playerid, 38))//ARMES LOURDES
		{
			if(HasPlayerWeapon(playerid, weaponid)) return 2;
			else return 1;
		}
	}
	return 0;
}

public GivePlayerWeaponEx(playerid, weaponid, ammo)
{
	new dWeaponSlot = HasPlayerWeapon(playerid, weaponid);
	if(dWeaponSlot != 0)
	{
	 	switch(dWeaponSlot)
	 	{
	 	    case 1:
	 	    {
	 	        pPlayerInfos[playerid][pArme1][1] += ammo;
	 	        GivePlayerWeapon(playerid, weaponid, ammo);
	 	        return 1;
	 	    }
	 	    case 2:
	 	    {
	 	        pPlayerInfos[playerid][pArme2][1] += ammo;
	 	        GivePlayerWeapon(playerid, weaponid, ammo);
	 	        return 2;
	 	    }
	 	    case 3:
	 	    {
	 	        pPlayerInfos[playerid][pArme3][1] += ammo;
	 	        GivePlayerWeapon(playerid, weaponid, ammo);
	 	        return 3;
	 	    }
	 	    case 4:
	 	    {
	 	        pPlayerInfos[playerid][pArme4][1] += ammo;
	 	        GivePlayerWeapon(playerid, weaponid, ammo);
	 	        return 4;
	 	    }
		}
		SetPlayerWeaponSkill(playerid, dWeaponSlot, WEAPON_AKIMBO);
	}
	else
	{
	    new dFreeSlot = GetPlayerNextFreeWeaponSlot(playerid);
	 	switch(dFreeSlot)
	 	{
	 	    case 1:
	 	    {
	 	        pPlayerInfos[playerid][pArme1][0] = weaponid;
	 	        pPlayerInfos[playerid][pArme1][1] = ammo;
	 	        GivePlayerWeapon(playerid, weaponid, ammo);
	 	        return 1;
	 	    }
	 	    case 2:
	 	    {
	 	        pPlayerInfos[playerid][pArme2][0] = weaponid;
	 	        pPlayerInfos[playerid][pArme2][1] = ammo;
	 	        GivePlayerWeapon(playerid, weaponid, ammo);
	 	        return 2;
	 	    }
	 	    case 3:
	 	    {
	 	        pPlayerInfos[playerid][pArme3][0] = weaponid;
	 	        pPlayerInfos[playerid][pArme3][1] = ammo;
	 	        GivePlayerWeapon(playerid, weaponid, ammo);
	 	        return 3;
	 	    }
	 	    case 4:
	 	    {
	 	        pPlayerInfos[playerid][pArme4][0] = weaponid;
	 	        pPlayerInfos[playerid][pArme4][1] = ammo;
	 	        GivePlayerWeapon(playerid, weaponid, ammo);
	 	        return 4;
	 	    }
		}
		PlayerPlaySound(playerid, 36401, 0.0, 0.0, 0.0);
		SetPlayerWeaponSkill(playerid, dFreeSlot, WEAPON_SIMPLE);
	}
 	return 0;
}

stock SetPlayerSlotWeaponEx(playerid, slot, weaponid, ammo, skill)
{
    if(IsValidPlayer(playerid))
    {
        switch(slot)
        {
            case 1:
            {
                pPlayerInfos[playerid][pArme1][0] = weaponid;
                pPlayerInfos[playerid][pArme1][1] = ammo;
                pPlayerInfos[playerid][pArme1][2] = skill;
            }
            case 2:
            {
                pPlayerInfos[playerid][pArme2][0] = weaponid;
                pPlayerInfos[playerid][pArme2][1] = ammo;
                pPlayerInfos[playerid][pArme2][2] = skill;
            }
            case 3:
            {
                pPlayerInfos[playerid][pArme3][0] = weaponid;
                pPlayerInfos[playerid][pArme3][1] = ammo;
                pPlayerInfos[playerid][pArme3][2] = skill;
            }
            case 4:
            {
                pPlayerInfos[playerid][pArme4][0] = weaponid;
                pPlayerInfos[playerid][pArme4][1] = ammo;
                pPlayerInfos[playerid][pArme4][2] = skill;
            }
        }
    }
}

stock SetPlayerWeaponSkill(playerid, slotid, skill)
{
	switch(slotid)
	{
	    case 1:
	    {
	        pPlayerInfos[playerid][pArme1][2] = skill;
	        switch(pPlayerInfos[playerid][pArme1][0])
	        {
	            case 22: SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, (skill == WEAPON_SIMPLE) ? 998 : 999);//Pistolets
	            case 26: SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, (skill == WEAPON_SIMPLE) ? 998 : 999);//Sawn off
	            case 28: SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, (skill == WEAPON_SIMPLE) ? 998 : 999);//UZI
	            case 32: SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, (skill == WEAPON_SIMPLE) ? 998 : 999);//Tec9
	            default: pPlayerInfos[playerid][pArme1][2] = WEAPON_SIMPLE;
	        }
	    }
	    case 2:
	    {
	        pPlayerInfos[playerid][pArme2][2] = skill;
	        switch(pPlayerInfos[playerid][pArme2][0])
	        {
	            case 22: SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, (skill == WEAPON_SIMPLE) ? 998 : 999);//Pistolets
	            case 26: SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, (skill == WEAPON_SIMPLE) ? 998 : 999);//Sawn off
	            case 28: SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, (skill == WEAPON_SIMPLE) ? 998 : 999);//UZI
	            case 32: SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, (skill == WEAPON_SIMPLE) ? 998 : 999);//Tec9
	            default: pPlayerInfos[playerid][pArme2][2] = WEAPON_SIMPLE;
	        }
	    }
	    case 3:
	    {
	        pPlayerInfos[playerid][pArme3][2] = skill;
	        switch(pPlayerInfos[playerid][pArme3][0])
	        {
	            case 22: SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, (skill == WEAPON_SIMPLE) ? 998 : 999);//Pistolets
	            case 26: SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, (skill == WEAPON_SIMPLE) ? 998 : 999);//Sawn off
	            case 28: SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, (skill == WEAPON_SIMPLE) ? 998 : 999);//UZI
	            case 32: SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, (skill == WEAPON_SIMPLE) ? 998 : 999);//Tec9
	            default: pPlayerInfos[playerid][pArme3][2] = WEAPON_SIMPLE;
	        }
	    }
	    case 4:
	    {
	        pPlayerInfos[playerid][pArme4][2] = skill;
	        switch(pPlayerInfos[playerid][pArme4][0])
	        {
	            case 22: SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, (skill == WEAPON_SIMPLE) ? 998 : 999);//Pistolets
	            case 26: SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, (skill == WEAPON_SIMPLE) ? 998 : 999);//Sawn off
	            case 28: SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, (skill == WEAPON_SIMPLE) ? 998 : 999);//UZI
	            case 32: SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, (skill == WEAPON_SIMPLE) ? 998 : 999);//Tec9
	            default: pPlayerInfos[playerid][pArme4][2] = WEAPON_SIMPLE;
	        }
	    }
	}
}

stock GetPlayerWeaponSkill(playerid, slotid)
{
	switch(slotid)
	{
		case 1: return pPlayerInfos[playerid][pArme1][2];
		case 2: return pPlayerInfos[playerid][pArme2][2];
		case 3: return pPlayerInfos[playerid][pArme3][2];
		case 4: return pPlayerInfos[playerid][pArme4][2];
	}
	return WEAPON_SIMPLE;
}

stock RemovePlayerWeapon(playerid, weaponid)
{
	new dRemoveSlot = -1;
	new dArmedWeapon = GetPlayerWeapon(playerid);
    new dWeapons[13][2];
	for (new i = 0; i < 13; i++) GetPlayerWeaponData(playerid, i, dWeapons[i][0], dWeapons[i][1]);
	switch(weaponid)
	{
	    case 0, 1: dRemoveSlot = 0;
	    case 2, 3, 4, 5, 6, 7, 8, 9: dRemoveSlot = 1;
	    case 22, 23, 24: dRemoveSlot = 2;
	    case 25, 26, 27: dRemoveSlot = 3;
	    case 28, 29, 32: dRemoveSlot = 4;
	    case 30, 31: dRemoveSlot = 5;
	    case 33, 34: dRemoveSlot = 6;
	    case 35, 36, 37, 38: dRemoveSlot = 7;
	    case 16, 17, 18, 39: dRemoveSlot = 8;
	    case 41, 42, 43: dRemoveSlot = 9;
	    case 10, 11, 12, 13, 14, 15: dRemoveSlot = 10;
	    case 44, 45: dRemoveSlot = 11;
	    case 40: dRemoveSlot = 12;
	}
	ResetPlayerWeapons(playerid);
	for(new i = 0; i < 13; i ++) if(i != dRemoveSlot) GivePlayerWeapon(playerid, dWeapons[i][0], dWeapons[i][1]);
	SetPlayerArmedWeapon(playerid, (dArmedWeapon != weaponid) ? dArmedWeapon : 0);
}

public GetPlayerLanguage(playerid)
{
	return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pLangue] : LANGUAGE_EN;
}

stock SetPlayerLanguage(playerid, language)
{
    if(IsValidPlayer(playerid))
    {
        pPlayerInfos[playerid][pLangue] = (LANGUAGE_DE >= language >= LANGUAGE_EN) ? language : LANGUAGE_EN; 
    }
}

public SaveMissionProgress(playerid, const missionname[], info)
{
	if(!strcmp(missionname, "Intro", false))
	{
	    pPlayerInfos[playerid][pIntro] = info;
	}
	if(!strcmp(missionname, "Amy", false))
	{
	    pPlayerInfos[playerid][pAmy] = info;
	}
	if(!strcmp(missionname, "Ken", false))
	{
	    pPlayerInfos[playerid][pKen] = info;
	}
	if(!strcmp(missionname, "DPO", false))
	{
	    pPlayerInfos[playerid][pDPO] = info;
	}
	if(!strcmp(missionname, "Reggae Shark", false))
	{
	    pPlayerInfos[playerid][pReggaeShark] = info;
	}
	return false;
}

public LoadMissionProgress(playerid, const missionname[])
{
	if(!strcmp(missionname, "Intro", false))
	{
	    return pPlayerInfos[playerid][pIntro];
	}
	if(!strcmp(missionname, "Amy", false))
	{
	    return pPlayerInfos[playerid][pAmy];
	}
	if(!strcmp(missionname, "Ken", false))
	{
	    return pPlayerInfos[playerid][pKen];
	}
	if(!strcmp(missionname, "DPO", false))
	{
	    return pPlayerInfos[playerid][pDPO];
	}
	if(!strcmp(missionname, "Reggae Shark", false))
	{
	    return pPlayerInfos[playerid][pReggaeShark];
	}
	return 0;
}

public GetUserLastCo_data(name[],value[])
{
	INI_String("LastConnection", lastco, 30);
	return 1;
}

public LoadUser_data(playerid,name[],value[])
{
	INI_Int("Password",pPlayerInfos[playerid][pPass]);
    INI_Int("Admin", pPlayerInfos[playerid][pAdmin]);
	INI_String("FirstCo", pPlayerInfos[playerid][sFirstCo], 30);
	INI_String("LastConnection", pPlayerInfos[playerid][sLastCo], 30);
	INI_Int("VIP", pPlayerInfos[playerid][pVIP][0]);
	INI_Int("VIPTime", pPlayerInfos[playerid][pVIP][1]);
	INI_Int("Ban", pPlayerInfos[playerid][pBan]);
    INI_Int("Langue", pPlayerInfos[playerid][pLangue]);
    INI_Int("Or", pPlayerInfos[playerid][pGold]);
    INI_Int("Kills", pPlayerInfos[playerid][pKills]);
    INI_Int("Morts", pPlayerInfos[playerid][pDeaths]);
    INI_Int("Headshots", pPlayerInfos[playerid][pHeadshots]);
    INI_Int("Zombies", pPlayerInfos[playerid][pZombies]);
    INI_Int("Bosses", pPlayerInfos[playerid][pBosses]);
    INI_Int("Missions", pPlayerInfos[playerid][pMissions]);
    INI_Float("DistanceVeh", pPlayerInfos[playerid][fDistanceWheel]);
    INI_Float("DistancePied", pPlayerInfos[playerid][fDistanceFoot]);
    INI_Int("TempsDeJeu", pPlayerInfos[playerid][pGameTime]);
    INI_Int("Sac", pPlayerInfos[playerid][pBag]);
    INI_Int("Faim", pPlayerInfos[playerid][pHunger]);
    INI_Int("Soif", pPlayerInfos[playerid][pThirst]);
    INI_Int("Sommeil", pPlayerInfos[playerid][pSleep]);
    INI_Int("Vie", pPlayerInfos[playerid][pHealth]);
    INI_Int("Armure", pPlayerInfos[playerid][pArmour]);
    INI_Float("X", pPlayerInfos[playerid][xPos]);
    INI_Float("Y", pPlayerInfos[playerid][yPos]);
    INI_Float("Z", pPlayerInfos[playerid][zPos]);
    INI_Float("A", pPlayerInfos[playerid][aPos]);
    INI_Bool("Jambe", pPlayerInfos[playerid][bLeg]);
    INI_Bool("Hemorragie", pPlayerInfos[playerid][bHemorragie]);
    INI_Int("Temperature", pPlayerInfos[playerid][pTemperature]);
    //ARMES
    INI_Int("Arme1", pPlayerInfos[playerid][pArme1][0]);
    INI_Int("Arme1Muns", pPlayerInfos[playerid][pArme1][1]);
    INI_Int("Arme1Skill", pPlayerInfos[playerid][pArme1][2]);
    INI_Int("Arme2", pPlayerInfos[playerid][pArme2][0]);
    INI_Int("Arme2Muns", pPlayerInfos[playerid][pArme2][1]);
    INI_Int("Arme2Skill", pPlayerInfos[playerid][pArme2][2]);
    INI_Int("Arme3", pPlayerInfos[playerid][pArme3][0]);
    INI_Int("Arme3Muns", pPlayerInfos[playerid][pArme3][1]);
    INI_Int("Arme3Skill", pPlayerInfos[playerid][pArme3][2]);
    INI_Int("Arme4", pPlayerInfos[playerid][pArme4][0]);
    INI_Int("Arme4Muns", pPlayerInfos[playerid][pArme4][1]);
    INI_Int("Arme4Skill", pPlayerInfos[playerid][pArme4][2]);
    //VÊTEMENTS
    INI_Int("Skin", pPlayerInfos[playerid][pSkin]);
    INI_Int("Chapeau", pPlayerInfos[playerid][pChapeau]);
    INI_Float("PosChapeau1", pPlayerInfos[playerid][fPosChapeau][0]);
    INI_Float("PosChapeau2", pPlayerInfos[playerid][fPosChapeau][1]);
    INI_Float("PosChapeau3", pPlayerInfos[playerid][fPosChapeau][2]);
    INI_Float("PosChapeau4", pPlayerInfos[playerid][fPosChapeau][3]);
    INI_Float("PosChapeau5", pPlayerInfos[playerid][fPosChapeau][4]);
    INI_Float("PosChapeau6", pPlayerInfos[playerid][fPosChapeau][5]);
    INI_Float("PosChapeau7", pPlayerInfos[playerid][fPosChapeau][6]);
    INI_Float("PosChapeau8", pPlayerInfos[playerid][fPosChapeau][7]);
    INI_Float("PosChapeau9", pPlayerInfos[playerid][fPosChapeau][8]);
    INI_Int("Lunettes", pPlayerInfos[playerid][pLunettes]);
    INI_Float("PosLunettes1", pPlayerInfos[playerid][fPosLunettes][0]);
    INI_Float("PosLunettes2", pPlayerInfos[playerid][fPosLunettes][1]);
    INI_Float("PosLunettes3", pPlayerInfos[playerid][fPosLunettes][2]);
    INI_Float("PosLunettes4", pPlayerInfos[playerid][fPosLunettes][3]);
    INI_Float("PosLunettes5", pPlayerInfos[playerid][fPosLunettes][4]);
    INI_Float("PosLunettes6", pPlayerInfos[playerid][fPosLunettes][5]);
    INI_Float("PosLunettes7", pPlayerInfos[playerid][fPosLunettes][6]);
    INI_Float("PosLunettes8", pPlayerInfos[playerid][fPosLunettes][7]);
    INI_Float("PosLunettes9", pPlayerInfos[playerid][fPosLunettes][8]);
    INI_Int("Masque", pPlayerInfos[playerid][pMasque]);
    INI_Float("PosMasque1", pPlayerInfos[playerid][fPosMasque][0]);
    INI_Float("PosMasque2", pPlayerInfos[playerid][fPosMasque][1]);
    INI_Float("PosMasque3", pPlayerInfos[playerid][fPosMasque][2]);
    INI_Float("PosMasque4", pPlayerInfos[playerid][fPosMasque][3]);
    INI_Float("PosMasque5", pPlayerInfos[playerid][fPosMasque][4]);
    INI_Float("PosMasque6", pPlayerInfos[playerid][fPosMasque][5]);
    INI_Float("PosMasque7", pPlayerInfos[playerid][fPosMasque][6]);
    INI_Float("PosMasque8", pPlayerInfos[playerid][fPosMasque][7]);
    INI_Float("PosMasque9", pPlayerInfos[playerid][fPosMasque][8]);
    INI_Int("Torse", pPlayerInfos[playerid][pTorse]);
    INI_Float("PosTorse1", pPlayerInfos[playerid][fPosTorse][0]);
    INI_Float("PosTorse2", pPlayerInfos[playerid][fPosTorse][1]);
    INI_Float("PosTorse3", pPlayerInfos[playerid][fPosTorse][2]);
    INI_Float("PosTorse4", pPlayerInfos[playerid][fPosTorse][3]);
    INI_Float("PosTorse5", pPlayerInfos[playerid][fPosTorse][4]);
    INI_Float("PosTorse6", pPlayerInfos[playerid][fPosTorse][5]);
    INI_Float("PosTorse7", pPlayerInfos[playerid][fPosTorse][6]);
    INI_Float("PosTorse8", pPlayerInfos[playerid][fPosTorse][7]);
    INI_Float("PosTorse9", pPlayerInfos[playerid][fPosTorse][8]);
    //INVENTAIRE
    INI_Int("ObjetMain", pPlayerInfos[playerid][HandObject]);
    INI_Int("Objet1", pPlayerInfos[playerid][BagObject][0]);
    INI_Int("Objet2", pPlayerInfos[playerid][BagObject][1]);
    INI_Int("Objet3", pPlayerInfos[playerid][BagObject][2]);
    INI_Int("Objet4", pPlayerInfos[playerid][BagObject][3]);
    INI_Int("Objet5", pPlayerInfos[playerid][BagObject][4]);
    INI_Int("Objet6", pPlayerInfos[playerid][BagObject][5]);
    INI_Int("Objet7", pPlayerInfos[playerid][BagObject][6]);
    INI_Int("Objet8", pPlayerInfos[playerid][BagObject][7]);
    INI_Int("Objet9", pPlayerInfos[playerid][BagObject][8]);
    INI_Int("Objet10", pPlayerInfos[playerid][BagObject][9]);
    INI_Int("Objet11", pPlayerInfos[playerid][BagObject][10]);
    INI_Int("Objet12", pPlayerInfos[playerid][BagObject][11]);
    INI_Int("Objet13", pPlayerInfos[playerid][BagObject][12]);
    INI_Int("Objet14", pPlayerInfos[playerid][BagObject][13]);
    INI_Int("Objet15", pPlayerInfos[playerid][BagObject][14]);
    INI_Int("Objet16", pPlayerInfos[playerid][BagObject][15]);
    INI_Int("Objet17", pPlayerInfos[playerid][BagObject][16]);
    INI_Int("Objet18", pPlayerInfos[playerid][BagObject][17]);
    INI_Int("Objet19", pPlayerInfos[playerid][BagObject][18]);
    INI_Int("Objet20", pPlayerInfos[playerid][BagObject][19]);
    INI_Int("Objet21", pPlayerInfos[playerid][BagObject][20]);
    INI_Int("Objet22", pPlayerInfos[playerid][BagObject][21]);
    INI_Int("Objet23", pPlayerInfos[playerid][BagObject][22]);
    INI_Int("Objet24", pPlayerInfos[playerid][BagObject][23]);
    INI_Int("Objet25", pPlayerInfos[playerid][BagObject][24]);
    INI_Int("Objet26", pPlayerInfos[playerid][BagObject][25]);
    INI_Int("Objet27", pPlayerInfos[playerid][BagObject][26]);
    INI_Int("Objet28", pPlayerInfos[playerid][BagObject][27]);
    INI_Int("Objet29", pPlayerInfos[playerid][BagObject][28]);
    INI_Int("Objet30", pPlayerInfos[playerid][BagObject][29]);
    INI_Int("Objet31", pPlayerInfos[playerid][BagObject][30]);
    INI_Int("Objet32", pPlayerInfos[playerid][BagObject][31]);
    INI_Int("Objet33", pPlayerInfos[playerid][BagObject][32]);
    INI_Int("Objet34", pPlayerInfos[playerid][BagObject][33]);
    INI_Int("Objet35", pPlayerInfos[playerid][BagObject][34]);
    INI_Int("Objet36", pPlayerInfos[playerid][BagObject][35]);
    //---
    INI_Int("Level", pPlayerInfos[playerid][pLevel]);
    INI_Int("Exp", pPlayerInfos[playerid][pExp]);
    INI_Int("PointsCompetence", pPlayerInfos[playerid][pCompetence]);
    //---
    INI_Int("Boucher", pPlayerInfos[playerid][dBoucher]);
    INI_Int("Medecine", pPlayerInfos[playerid][dMedecine]);
    INI_Int("Sante", pPlayerInfos[playerid][dSante]);
    INI_Int("Artisan", pPlayerInfos[playerid][dArtisan]);
    INI_Int("Transporteur", pPlayerInfos[playerid][dTransporteur]);
    INI_Int("Pecheur", pPlayerInfos[playerid][dPecheur]);
    INI_Int("Mecano", pPlayerInfos[playerid][dMecano]);
    INI_Int("Jardinie", pPlayerInfos[playerid][dJardinier]);
    INI_Int("Athlete", pPlayerInfos[playerid][dAthlete]);
    INI_Int("Survivaliste", pPlayerInfos[playerid][dSurvivaliste]);
    INI_Int("Tank", pPlayerInfos[playerid][dTank]);
    INI_Int("Bomberman", pPlayerInfos[playerid][dBomberman]);
    INI_Int("Hydratation", pPlayerInfos[playerid][dHydra]);
    //---
    INI_Int("Aide1_16", pPlayerInfos[playerid][dAide1_16]);
    INI_Int("Aide17_32", pPlayerInfos[playerid][dAide17_32]);
    //---
    INI_Int("MissionReggae", pPlayerInfos[playerid][pReggaeShark]);
    INI_Int("MissionBeginning", pPlayerInfos[playerid][pIntro]);
    INI_Int("MissionAmy", pPlayerInfos[playerid][pAmy]);
    INI_Int("MissionKen", pPlayerInfos[playerid][pKen]);
    INI_Int("MissionDPO", pPlayerInfos[playerid][pDPO]);
	return 1;
}

stock ProcessPlayerSave(playerid, bool:save)
{
	if(save)//Si c'est pour sauvegarder
	{
	    for(new i = 0; i < 16; i ++)
	    {
			pPlayerInfos[playerid][dAide1_16] = (pPlayerInfos[playerid][dAide1_16] | (pPlayerInfos[playerid][bAide][i] ? 1 : 0) << i);
		}
	    for(new i = 16; i < 32; i ++)
	    {
			pPlayerInfos[playerid][dAide17_32] = (pPlayerInfos[playerid][dAide17_32] | (pPlayerInfos[playerid][bAide][i] ? 1 : 0) << i);
		}
	}
	else
	{
	    for(new i = 0; i < 16; i ++)
	    {
		    pPlayerInfos[playerid][bAide][i] = (pPlayerInfos[playerid][dAide1_16] >> i & 1 == 1 ? true : false);
		}
	    for(new i = 16; i < 32; i ++)
	    {
		    pPlayerInfos[playerid][bAide][i] = (pPlayerInfos[playerid][dAide17_32] >> i & 1 == 1 ? true : false);
		}
	}
}

public SaveUser(playerid)
{
	ProcessPlayerSave(playerid, .save = true);
	//---
    new INI:File, string[30];
	File = INI_Open(UserPath(playerid));
    INI_SetTag(File,"data");
    INI_WriteInt(File,"Password", pPlayerInfos[playerid][pPass]);
    INI_WriteInt(File,"Admin", pPlayerInfos[playerid][pAdmin]);
    //---
	new date[6];
	getdate(date[0], date[1], date[2]);
	gettime(date[3], date[4], date[5]);
	format(string, sizeof(string), "%d/%d/%d - %d:%d:%d", date[2], date[1], date[0], date[3], date[4], date[5]);
	INI_WriteString(File,"LastConnection", string);
	//---
	INI_WriteInt(File,"VIP", pPlayerInfos[playerid][pVIP][0]);
	INI_WriteInt(File,"VIPTime", pPlayerInfos[playerid][pVIP][1]);
	INI_WriteInt(File,"Ban", pPlayerInfos[playerid][pBan]);
    INI_WriteInt(File,"Langue", pPlayerInfos[playerid][pLangue]);
    INI_WriteInt(File,"Or", pPlayerInfos[playerid][pGold]);
    INI_WriteInt(File,"Kills", pPlayerInfos[playerid][pKills]);
    INI_WriteInt(File,"Morts", pPlayerInfos[playerid][pDeaths]);
    INI_WriteInt(File,"Headshots", pPlayerInfos[playerid][pHeadshots]);
    INI_WriteInt(File,"Zombies", pPlayerInfos[playerid][pZombies]);
    INI_WriteInt(File,"Bosses", pPlayerInfos[playerid][pBosses]);
    INI_WriteInt(File,"Missions", pPlayerInfos[playerid][pMissions]);
    INI_WriteFloat(File,"DistanceVeh", pPlayerInfos[playerid][fDistanceWheel]);
    INI_WriteFloat(File,"DistancePied", pPlayerInfos[playerid][fDistanceFoot]);
    INI_WriteInt(File,"TempsDeJeu", pPlayerInfos[playerid][pGameTime]);
    INI_WriteInt(File,"Sac", pPlayerInfos[playerid][pBag]);
    INI_WriteInt(File,"Faim", pPlayerInfos[playerid][pHunger]);
    INI_WriteInt(File,"Soif", pPlayerInfos[playerid][pThirst]);
    INI_WriteInt(File,"Sommeil", pPlayerInfos[playerid][pSleep]);
    INI_WriteInt(File,"Vie", pPlayerInfos[playerid][pHealth]);
    INI_WriteInt(File,"Armure", pPlayerInfos[playerid][pArmour]);
    INI_WriteFloat(File,"X", pPlayerInfos[playerid][xPos]);
    INI_WriteFloat(File,"Y", pPlayerInfos[playerid][yPos]);
    INI_WriteFloat(File,"Z", pPlayerInfos[playerid][zPos]);
    INI_WriteFloat(File,"A", pPlayerInfos[playerid][aPos]);
    INI_WriteBool(File,"Jambe", pPlayerInfos[playerid][bLeg]);
    INI_WriteBool(File,"Hemorragie", pPlayerInfos[playerid][bHemorragie]);
	INI_WriteInt(File,"Temperature", pPlayerInfos[playerid][pTemperature]);
    //ARMES
    INI_WriteInt(File,"Arme1", pPlayerInfos[playerid][pArme1][0]);
    INI_WriteInt(File,"Arme1Muns", pPlayerInfos[playerid][pArme1][1]);
    INI_WriteInt(File,"Arme1Skill", pPlayerInfos[playerid][pArme1][2]);
    INI_WriteInt(File,"Arme2", pPlayerInfos[playerid][pArme2][0]);
    INI_WriteInt(File,"Arme2Muns", pPlayerInfos[playerid][pArme2][1]);
    INI_WriteInt(File,"Arme2Skill", pPlayerInfos[playerid][pArme2][2]);
    INI_WriteInt(File,"Arme3", pPlayerInfos[playerid][pArme3][0]);
    INI_WriteInt(File,"Arme3Muns", pPlayerInfos[playerid][pArme3][1]);
    INI_WriteInt(File,"Arme3Skill", pPlayerInfos[playerid][pArme3][2]);
    INI_WriteInt(File,"Arme4", pPlayerInfos[playerid][pArme4][0]);
    INI_WriteInt(File,"Arme4Muns", pPlayerInfos[playerid][pArme4][1]);
    INI_WriteInt(File,"Arme4Skill", pPlayerInfos[playerid][pArme4][2]);
    //VÊTEMENTS
    INI_WriteInt(File,"Skin", pPlayerInfos[playerid][pSkin]);
    INI_WriteInt(File,"Chapeau", pPlayerInfos[playerid][pChapeau]);
    INI_WriteFloat(File,"PosChapeau1", pPlayerInfos[playerid][fPosChapeau][0]);
    INI_WriteFloat(File,"PosChapeau2", pPlayerInfos[playerid][fPosChapeau][1]);
    INI_WriteFloat(File,"PosChapeau3", pPlayerInfos[playerid][fPosChapeau][2]);
    INI_WriteFloat(File,"PosChapeau4", pPlayerInfos[playerid][fPosChapeau][3]);
    INI_WriteFloat(File,"PosChapeau5", pPlayerInfos[playerid][fPosChapeau][4]);
    INI_WriteFloat(File,"PosChapeau6", pPlayerInfos[playerid][fPosChapeau][5]);
    INI_WriteFloat(File,"PosChapeau7", pPlayerInfos[playerid][fPosChapeau][6]);
    INI_WriteFloat(File,"PosChapeau8", pPlayerInfos[playerid][fPosChapeau][7]);
    INI_WriteFloat(File,"PosChapeau9", pPlayerInfos[playerid][fPosChapeau][8]);
    INI_WriteInt(File,"Lunettes", pPlayerInfos[playerid][pLunettes]);
    INI_WriteFloat(File,"PosLunettes1", pPlayerInfos[playerid][fPosLunettes][0]);
    INI_WriteFloat(File,"PosLunettes2", pPlayerInfos[playerid][fPosLunettes][1]);
    INI_WriteFloat(File,"PosLunettes3", pPlayerInfos[playerid][fPosLunettes][2]);
    INI_WriteFloat(File,"PosLunettes4", pPlayerInfos[playerid][fPosLunettes][3]);
    INI_WriteFloat(File,"PosLunettes5", pPlayerInfos[playerid][fPosLunettes][4]);
    INI_WriteFloat(File,"PosLunettes6", pPlayerInfos[playerid][fPosLunettes][5]);
    INI_WriteFloat(File,"PosLunettes7", pPlayerInfos[playerid][fPosLunettes][6]);
    INI_WriteFloat(File,"PosLunettes8", pPlayerInfos[playerid][fPosLunettes][7]);
    INI_WriteFloat(File,"PosLunettes9", pPlayerInfos[playerid][fPosLunettes][8]);
    INI_WriteInt(File,"Masque", pPlayerInfos[playerid][pMasque]);
    INI_WriteFloat(File,"PosMasque1", pPlayerInfos[playerid][fPosMasque][0]);
    INI_WriteFloat(File,"PosMasque2", pPlayerInfos[playerid][fPosMasque][1]);
    INI_WriteFloat(File,"PosMasque3", pPlayerInfos[playerid][fPosMasque][2]);
    INI_WriteFloat(File,"PosMasque4", pPlayerInfos[playerid][fPosMasque][3]);
    INI_WriteFloat(File,"PosMasque5", pPlayerInfos[playerid][fPosMasque][4]);
    INI_WriteFloat(File,"PosMasque6", pPlayerInfos[playerid][fPosMasque][5]);
    INI_WriteFloat(File,"PosMasque7", pPlayerInfos[playerid][fPosMasque][6]);
    INI_WriteFloat(File,"PosMasque8", pPlayerInfos[playerid][fPosMasque][7]);
    INI_WriteFloat(File,"PosMasque9", pPlayerInfos[playerid][fPosMasque][8]);
    INI_WriteInt(File,"Torse", pPlayerInfos[playerid][pTorse]);
    INI_WriteFloat(File,"PosTorse1", pPlayerInfos[playerid][fPosTorse][0]);
    INI_WriteFloat(File,"PosTorse2", pPlayerInfos[playerid][fPosTorse][1]);
    INI_WriteFloat(File,"PosTorse3", pPlayerInfos[playerid][fPosTorse][2]);
    INI_WriteFloat(File,"PosTorse4", pPlayerInfos[playerid][fPosTorse][3]);
    INI_WriteFloat(File,"PosTorse5", pPlayerInfos[playerid][fPosTorse][4]);
    INI_WriteFloat(File,"PosTorse6", pPlayerInfos[playerid][fPosTorse][5]);
    INI_WriteFloat(File,"PosTorse7", pPlayerInfos[playerid][fPosTorse][6]);
    INI_WriteFloat(File,"PosTorse8", pPlayerInfos[playerid][fPosTorse][7]);
    INI_WriteFloat(File,"PosTorse9", pPlayerInfos[playerid][fPosTorse][8]);
    //INVENTAIRE
    INI_WriteInt(File,"ObjetMain", pPlayerInfos[playerid][HandObject]);
    INI_WriteInt(File,"Objet1", pPlayerInfos[playerid][BagObject][0]);
    INI_WriteInt(File,"Objet2", pPlayerInfos[playerid][BagObject][1]);
    INI_WriteInt(File,"Objet3", pPlayerInfos[playerid][BagObject][2]);
    INI_WriteInt(File,"Objet4", pPlayerInfos[playerid][BagObject][3]);
    INI_WriteInt(File,"Objet5", pPlayerInfos[playerid][BagObject][4]);
    INI_WriteInt(File,"Objet6", pPlayerInfos[playerid][BagObject][5]);
    INI_WriteInt(File,"Objet7", pPlayerInfos[playerid][BagObject][6]);
    INI_WriteInt(File,"Objet8", pPlayerInfos[playerid][BagObject][7]);
    INI_WriteInt(File,"Objet9", pPlayerInfos[playerid][BagObject][8]);
    INI_WriteInt(File,"Objet10", pPlayerInfos[playerid][BagObject][9]);
    INI_WriteInt(File,"Objet11", pPlayerInfos[playerid][BagObject][10]);
    INI_WriteInt(File,"Objet12", pPlayerInfos[playerid][BagObject][11]);
    INI_WriteInt(File,"Objet13", pPlayerInfos[playerid][BagObject][12]);
    INI_WriteInt(File,"Objet14", pPlayerInfos[playerid][BagObject][13]);
    INI_WriteInt(File,"Objet15", pPlayerInfos[playerid][BagObject][14]);
    INI_WriteInt(File,"Objet16", pPlayerInfos[playerid][BagObject][15]);
    INI_WriteInt(File,"Objet17", pPlayerInfos[playerid][BagObject][16]);
    INI_WriteInt(File,"Objet18", pPlayerInfos[playerid][BagObject][17]);
    INI_WriteInt(File,"Objet19", pPlayerInfos[playerid][BagObject][18]);
    INI_WriteInt(File,"Objet20", pPlayerInfos[playerid][BagObject][19]);
    INI_WriteInt(File,"Objet21", pPlayerInfos[playerid][BagObject][20]);
    INI_WriteInt(File,"Objet22", pPlayerInfos[playerid][BagObject][21]);
    INI_WriteInt(File,"Objet23", pPlayerInfos[playerid][BagObject][22]);
    INI_WriteInt(File,"Objet24", pPlayerInfos[playerid][BagObject][23]);
    INI_WriteInt(File,"Objet25", pPlayerInfos[playerid][BagObject][24]);
    INI_WriteInt(File,"Objet26", pPlayerInfos[playerid][BagObject][25]);
    INI_WriteInt(File,"Objet27", pPlayerInfos[playerid][BagObject][26]);
    INI_WriteInt(File,"Objet28", pPlayerInfos[playerid][BagObject][27]);
    INI_WriteInt(File,"Objet29", pPlayerInfos[playerid][BagObject][28]);
    INI_WriteInt(File,"Objet30", pPlayerInfos[playerid][BagObject][29]);
    INI_WriteInt(File,"Objet31", pPlayerInfos[playerid][BagObject][30]);
    INI_WriteInt(File,"Objet32", pPlayerInfos[playerid][BagObject][31]);
    INI_WriteInt(File,"Objet33", pPlayerInfos[playerid][BagObject][32]);
    INI_WriteInt(File,"Objet34", pPlayerInfos[playerid][BagObject][33]);
    INI_WriteInt(File,"Objet35", pPlayerInfos[playerid][BagObject][34]);
    INI_WriteInt(File,"Objet36", pPlayerInfos[playerid][BagObject][35]);
    //---
    INI_WriteInt(File,"Level", pPlayerInfos[playerid][pLevel]);
    INI_WriteInt(File,"Exp", pPlayerInfos[playerid][pExp]);
    INI_WriteInt(File,"PointsCompetence", pPlayerInfos[playerid][pCompetence]);
    //---
    INI_WriteInt(File,"Boucher", pPlayerInfos[playerid][dBoucher]);
    INI_WriteInt(File,"Medecine", pPlayerInfos[playerid][dMedecine]);
    INI_WriteInt(File,"Sante", pPlayerInfos[playerid][dSante]);
    INI_WriteInt(File,"Artisan", pPlayerInfos[playerid][dArtisan]);
    INI_WriteInt(File,"Transporteur", pPlayerInfos[playerid][dTransporteur]);
    INI_WriteInt(File,"Pecheur", pPlayerInfos[playerid][dPecheur]);
    INI_WriteInt(File,"Mecano", pPlayerInfos[playerid][dMecano]);
    INI_WriteInt(File,"Jardinie", pPlayerInfos[playerid][dJardinier]);
    INI_WriteInt(File,"Athlete", pPlayerInfos[playerid][dAthlete]);
    INI_WriteInt(File,"Survivaliste", pPlayerInfos[playerid][dSurvivaliste]);
    INI_WriteInt(File,"Tank", pPlayerInfos[playerid][dTank]);
    INI_WriteInt(File,"Bomberman", pPlayerInfos[playerid][dBomberman]);
    INI_WriteInt(File,"Hydratation", pPlayerInfos[playerid][dHydra]);
    //---
    INI_WriteInt(File,"Aide1_16", pPlayerInfos[playerid][dAide1_16]);
    INI_WriteInt(File,"Aide17_32", pPlayerInfos[playerid][dAide17_32]);
    //---
    INI_WriteInt(File,"MissionReggae", pPlayerInfos[playerid][pReggaeShark]);
    INI_WriteInt(File,"MissionBeginning", pPlayerInfos[playerid][pIntro]);
    INI_WriteInt(File,"MissionAmy", pPlayerInfos[playerid][pAmy]);
    INI_WriteInt(File,"MissionKen", pPlayerInfos[playerid][pKen]);
    INI_WriteInt(File,"MissionDPO", pPlayerInfos[playerid][pDPO]);
	INI_Close(File);
}

stock WriteBlankSave(playerid, const inputtext[])
{
	pPlayerInfos[playerid][pPass] = udb_hash(inputtext);

    new INI:File = INI_Open(UserPath(playerid));
    INI_SetTag(File,"data");
    INI_WriteInt(File,"Password", udb_hash(inputtext));
    //---
    new date[6];
    getdate(date[0], date[1], date[2]);
    gettime(date[3], date[4], date[5]);
    format(string, sizeof(string), "%d/%d/%d - %d:%d:%d", date[2], date[1], date[0], date[3], date[4], date[5]);
    INI_WriteString(File,"FirstCo", string);
    INI_WriteString(File,"LastConnection", string);
    strcpy(pPlayerInfos[playerid][sFirstCo], string);
    strcpy(pPlayerInfos[playerid][sLastCo], string);
    //---
    INI_WriteInt(File,"Admin", 0);
    INI_WriteInt(File,"VIP", 0);
    INI_WriteInt(File,"VIPTime", 0);
    INI_WriteInt(File,"Ban", 0);
    INI_WriteInt(File,"Langue", pPlayerInfos[playerid][pLangue]);
    INI_WriteInt(File,"Or", 0);
    INI_WriteInt(File,"Kills", 0);
    INI_WriteInt(File,"Morts", 0);
    INI_WriteInt(File,"TempsDeJeu", 0);
    INI_WriteInt(File,"Sac", 1);
    INI_WriteInt(File,"Faim", 100);
    INI_WriteInt(File,"Soif", 100);
    INI_WriteInt(File,"Sommeil", 100);
    INI_WriteInt(File,"Vie", 1000);
    INI_WriteInt(File,"Armure", 1000);
    INI_WriteFloat(File,"X", 0.0);
    INI_WriteFloat(File,"Y", 0.0);
    INI_WriteFloat(File,"Z", 0.0);
    INI_WriteFloat(File,"A", 0.0);
    INI_WriteBool(File,"Jambe", true);
    INI_WriteInt(File,"Temperature", 370);
    INI_WriteInt(File,"Skin", 0);
    INI_WriteInt(File,"Chapeau", 0);
    INI_WriteInt(File,"Lunettes", 0);
    INI_WriteInt(File,"Masque", 0);
    INI_WriteInt(File,"Torse", 0);
    INI_WriteInt(File,"Camouflage", 0);
    INI_WriteInt(File,"Arme1", 0);
    INI_WriteInt(File,"Arme1Muns", 0);
    INI_WriteInt(File,"Arme2", 0);
    INI_WriteInt(File,"Arme2Muns", 0);
    INI_WriteInt(File,"Arme3", 0);
    INI_WriteInt(File,"Arme3Muns", 0);
    INI_WriteInt(File,"Arme4", 0);
    INI_WriteInt(File,"Arme4Muns", 0);
    //---
    INI_WriteInt(File,"Password", udb_hash(inputtext));
    INI_WriteInt(File,"Admin", 0);
    INI_WriteInt(File,"Or", 0);
    INI_WriteInt(File,"Kills", 0);
    INI_WriteInt(File,"Morts", 0);
    INI_WriteInt(File,"TempsDeJeu", 0);
    INI_WriteInt(File,"Sac", 1);
    INI_WriteInt(File,"Faim", 100);
    INI_WriteInt(File,"Soif", 100);
    INI_WriteInt(File,"Sommeil", 100);
    INI_WriteInt(File,"Vie", 1000);
    INI_WriteInt(File,"Armure", 1000);
    INI_WriteFloat(File,"X", 0.0);
    INI_WriteFloat(File,"Y", 0.0);
    INI_WriteFloat(File,"Z", 0.0);
    INI_WriteFloat(File,"A", 0.0);
    INI_WriteBool(File,"Jambe", true);
    INI_WriteInt(File,"Temperature", 370);
    //ARMES
    INI_WriteInt(File,"Arme1", 0);
    INI_WriteInt(File,"Arme1Muns", 0);
    INI_WriteInt(File,"Arme2", 0);
    INI_WriteInt(File,"Arme2Muns", 0);
    INI_WriteInt(File,"Arme3", 0);
    INI_WriteInt(File,"Arme3Muns", 0);
    INI_WriteInt(File,"Arme4", 0);
    INI_WriteInt(File,"Arme4Muns", 0);
    //VÊTEMENTS
    INI_WriteInt(File,"Skin", 0);
    INI_WriteInt(File,"Chapeau", 0);
    INI_WriteFloat(File,"PosChapeau1", 0.0);
    INI_WriteFloat(File,"PosChapeau2", 0.0);
    INI_WriteFloat(File,"PosChapeau3", 0.0);
    INI_WriteFloat(File,"PosChapeau4", 0.0);
    INI_WriteFloat(File,"PosChapeau5", 0.0);
    INI_WriteFloat(File,"PosChapeau6", 0.0);
    INI_WriteFloat(File,"PosChapeau7", 0.0);
    INI_WriteFloat(File,"PosChapeau8", 0.0);
    INI_WriteFloat(File,"PosChapeau9", 0.0);
    INI_WriteInt(File,"Lunettes", 0);
    INI_WriteFloat(File,"PosLunettes1", 0.0);
    INI_WriteFloat(File,"PosLunettes2", 0.0);
    INI_WriteFloat(File,"PosLunettes3", 0.0);
    INI_WriteFloat(File,"PosLunettes4", 0.0);
    INI_WriteFloat(File,"PosLunettes5", 0.0);
    INI_WriteFloat(File,"PosLunettes6", 0.0);
    INI_WriteFloat(File,"PosLunettes7", 0.0);
    INI_WriteFloat(File,"PosLunettes8", 0.0);
    INI_WriteFloat(File,"PosLunettes9", 0.0);
    INI_WriteInt(File,"Masque", 0);
    INI_WriteFloat(File,"PosMasque1", 0.0);
    INI_WriteFloat(File,"PosMasque2", 0.0);
    INI_WriteFloat(File,"PosMasque3", 0.0);
    INI_WriteFloat(File,"PosMasque4", 0.0);
    INI_WriteFloat(File,"PosMasque5", 0.0);
    INI_WriteFloat(File,"PosMasque6", 0.0);
    INI_WriteFloat(File,"PosMasque7", 0.0);
    INI_WriteFloat(File,"PosMasque8", 0.0);
    INI_WriteFloat(File,"PosMasque9", 0.0);
    INI_WriteInt(File,"Torse", 0);
    INI_WriteFloat(File,"PosTorse1", 0.0);
    INI_WriteFloat(File,"PosTorse2", 0.0);
    INI_WriteFloat(File,"PosTorse3", 0.0);
    INI_WriteFloat(File,"PosTorse4", 0.0);
    INI_WriteFloat(File,"PosTorse5", 0.0);
    INI_WriteFloat(File,"PosTorse6", 0.0);
    INI_WriteFloat(File,"PosTorse7", 0.0);
    INI_WriteFloat(File,"PosTorse8", 0.0);
    INI_WriteFloat(File,"PosTorse9", 0.0);
    //INVENTAIRE
    INI_WriteInt(File,"ObjetMain", 0);
    INI_WriteInt(File,"Objet1", 0);
    INI_WriteInt(File,"Objet2", 0);
    INI_WriteInt(File,"Objet3", 0);
    INI_WriteInt(File,"Objet4", 0);
    INI_WriteInt(File,"Objet5", 0);
    INI_WriteInt(File,"Objet6", 0);
    INI_WriteInt(File,"Objet7", 0);
    INI_WriteInt(File,"Objet8", 0);
    INI_WriteInt(File,"Objet9", 0);
    INI_WriteInt(File,"Objet10", 0);
    INI_WriteInt(File,"Objet11", 0);
    INI_WriteInt(File,"Objet12", 0);
    INI_WriteInt(File,"Objet13", 0);
    INI_WriteInt(File,"Objet14", 0);
    INI_WriteInt(File,"Objet15", 0);
    INI_WriteInt(File,"Objet16", 0);
    INI_WriteInt(File,"Objet17", 0);
    INI_WriteInt(File,"Objet18", 0);
    INI_WriteInt(File,"Objet19", 0);
    INI_WriteInt(File,"Objet20", 0);
    INI_WriteInt(File,"Objet21", 0);
    INI_WriteInt(File,"Objet22", 0);
    INI_WriteInt(File,"Objet23", 0);
    INI_WriteInt(File,"Objet24", 0);
    INI_WriteInt(File,"Objet25", 0);
    INI_WriteInt(File,"Objet26", 0);
    INI_WriteInt(File,"Objet27", 0);
    INI_WriteInt(File,"Objet28", 0);
    INI_WriteInt(File,"Objet29", 0);
    INI_WriteInt(File,"Objet30", 0);
    INI_WriteInt(File,"Objet31", 0);
    INI_WriteInt(File,"Objet32", 0);
    INI_WriteInt(File,"Objet33", 0);
    INI_WriteInt(File,"Objet34", 0);
    INI_WriteInt(File,"Objet35", 0);
    INI_WriteInt(File,"Objet36", 0);
    //---
    INI_WriteInt(File,"Level", 1);
    INI_WriteInt(File,"Exp", 0);
    INI_WriteInt(File,"PointsCompetence", 0);
    //---
    INI_WriteInt(File,"Boucher", 0);
    INI_WriteInt(File,"Medecine", 0);
    INI_WriteInt(File,"Sante", 0);
    INI_WriteInt(File,"Artisan", 0);
    INI_WriteInt(File,"Transporteur", 0);
    INI_WriteInt(File,"Pecheur", 0);
    INI_WriteInt(File,"Mecano", 0);
    INI_WriteInt(File,"Jardinie", 0);
    INI_WriteInt(File,"Athlete", 0);
    //---
    INI_WriteBool(File,"Aide1", false);
    INI_WriteBool(File,"Aide2", false);
    INI_WriteBool(File,"Aide3", false);
    INI_WriteBool(File,"Aide4", false);
    INI_WriteBool(File,"Aide5", false);
    INI_WriteBool(File,"Aide6", false);
    INI_WriteBool(File,"Aide7", false);
    INI_WriteBool(File,"Aide8", false);
    INI_WriteBool(File,"Aide9", false);
    INI_WriteBool(File,"Aide10", false);
    INI_WriteBool(File,"Aide11", false);
    INI_WriteBool(File,"Aide12", false);
    INI_WriteBool(File,"Aide13", false);
    INI_WriteBool(File,"Aide14", false);
    INI_WriteBool(File,"Aide15", false);
    INI_WriteBool(File,"Aide16", false);
    //---
    INI_WriteInt(File,"MissionReggae", 0);
    INI_Close(File);
}

public LoadUserOffline_auctions(playerid, name[], value[])
{
	new string[32];
    INI_Int("Or", pPlayerOfflineInfos[playerid][dRecievedGold]);
	for(new i = 0; i < 50; i ++)
	{
		format(string, sizeof(string), "Return%d", i);
    	INI_Int(string, pPlayerOfflineInfos[playerid][dReturnedItem][i]);
    }
    return 1;
}

SaveUserOffline(playerid)
{
	new string[64];
    new INI:File;
    format(string, sizeof(string), OFFPATH, GetName(playerid));
	File = INI_Open(string);
    INI_SetTag(File,"auctions");
    INI_WriteInt(File,"Or", pPlayerOfflineInfos[playerid][dRecievedGold]);
	for(new i = 0; i < 50; i ++)
	{
		format(string, sizeof(string), "Return%d", i);
    	INI_WriteInt(File,string, pPlayerOfflineInfos[playerid][dReturnedItem][i]);
    }
	INI_Close(File);
}

stock ReturnOnlinePlayerItem(playerid, itemid) 
{
	for(new i = 0; i < 50; i ++) 
	{
		if(pPlayerOfflineInfos[playerid][dReturnedItem][i] == 0) 
		{
			pPlayerOfflineInfos[playerid][dReturnedItem][i] = itemid;
		}
	}
}

stock ReturnOfflinePlayerItem(const playername[], itemid)
{
    new string[50];
    format(string,sizeof(string), OFFPATH, playername);
    if(!fexist(string)) return 0;
	INI_ParseFile(string, "LoadUserItems_%s");
	for(new i = 0; i < 50; i ++)
	{
	    if(dAuctionParsingVar[i] == 0)
	    {
		    new INI:File = INI_Open(string);
			INI_SetTag(File,"auctions");
			format(string, sizeof(string), "Return%d", i);
			INI_WriteInt(File, string, itemid);
			INI_Close(File);
	        return true;
	    }
	}
	return false;
}

stock GetPlayerHashedPassword(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pPass] : 0;
}

stock SetPlayerHashedPassword(playerid, newpass)
{
    if(IsValidPlayer(playerid))
    {
        pPlayerInfos[playerid][pPass] = newpass;
    }
}

ShowStats(playerid, toplayerid)
{
    new sStats[512], string[128];
	switch(GetPlayerLanguage(toplayerid))
	{
	    case LANGUAGE_EN:
	    {
			format(string, sizeof(string), "{008800}%s - Registered on %s:\n\n\n", GetName(playerid), pPlayerInfos[playerid][sFirstCo]);
		  	strcat(sStats, string);
			format(string, sizeof(string), "{CC0000}DM:\n");
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Kills: {ffff00}%d\n", pPlayerInfos[playerid][pKills]);
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Headshots:{ffff00} %d\n", pPlayerInfos[playerid][pHeadshots]);
		  	strcat(sStats, string);
		  	format(string, sizeof(string), "{0033cc}Deaths: {ffff00}%d\n", pPlayerInfos[playerid][pDeaths]);
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Ratio:{ffff00} %.2f\n", floatdiv(pPlayerInfos[playerid][pKills], pPlayerInfos[playerid][pDeaths]));
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Zombies:{ffff00} %d\n", pPlayerInfos[playerid][pZombies]);
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Bosses:{ffff00} %d\n\n\n", pPlayerInfos[playerid][pBosses]);
		  	strcat(sStats, string);
		  	//---
			format(string, sizeof(string), "{CC0000}Game:\n");
		  	strcat(sStats, string);
		  	//---
		  	new dDays = floatround(floatdiv(pPlayerInfos[playerid][pGameTime], 1440), floatround_floor);
			format(string, sizeof(string), "{0033cc}Time online: {ffff00}%d days and %s\n", dDays, GetTimeString(pPlayerInfos[playerid][pGameTime] - (dDays * 1440)));
		  	strcat(sStats, string);
		  	//---
			format(string, sizeof(string), "{0033cc}Missions passed: {ffff00}%d\n", pPlayerInfos[playerid][pMissions]);
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Distance by foot: {ffff00}%.1fkm\n", floatdiv(pPlayerInfos[playerid][fDistanceFoot], 1000));
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Distance by wheels: {ffff00}%.1fkm\n", floatdiv(pPlayerInfos[playerid][fDistanceWheel], 1000));
		  	strcat(sStats, string);
		  	//---
			ShowPlayerDialog(playerid, DIALOG_STATS, DIALOG_STYLE_MSGBOX, "Stats", sStats, "Close", "");
		}
		case LANGUAGE_FR:
	    {
			format(string, sizeof(string), "{008800}%s\n{ffff00}Inscrit le %s:\n\n\n", GetName(playerid), pPlayerInfos[playerid][sFirstCo]);
		  	strcat(sStats, string);
			format(string, sizeof(string), "{CC0000}DM:\n");
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Meurtres: {ffff00}%d\t\t\t", pPlayerInfos[playerid][pKills]);
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Headshots:{ffff00} %d\n", pPlayerInfos[playerid][pHeadshots]);
		  	strcat(sStats, string);
		  	format(string, sizeof(string), "{0033cc}Morts: {ffff00}%d\t\t\t", pPlayerInfos[playerid][pDeaths]);
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Ratio:{ffff00} %.2f\n", floatdiv(pPlayerInfos[playerid][pKills], pPlayerInfos[playerid][pDeaths]));
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Zombies:{ffff00} %d\n", pPlayerInfos[playerid][pZombies]);
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Bosses:{ffff00} %d\n\n\n", pPlayerInfos[playerid][pBosses]);
		  	strcat(sStats, string);
		  	//---
			format(string, sizeof(string), "{CC0000}Jeu:\n");
		  	strcat(sStats, string);
		  	//---
		  	new dDays = floatround(floatdiv(pPlayerInfos[playerid][pGameTime], 1440), floatround_floor);
			format(string, sizeof(string), "{0033cc}Temps de jeu: {ffff00}%d jours et %s\n", dDays, GetTimeString(pPlayerInfos[playerid][pGameTime] - (dDays * 1440)));
		  	strcat(sStats, string);
		  	//---
			format(string, sizeof(string), "{0033cc}Missions accomplies: {ffff00}%d\n", pPlayerInfos[playerid][pMissions]);
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Distance à pied: {ffff00}%.1fkm\n", floatdiv(pPlayerInfos[playerid][fDistanceFoot], 1000));
		  	strcat(sStats, string);
			format(string, sizeof(string), "{0033cc}Distance sur roues: {ffff00}%.1fkm\n", floatdiv(pPlayerInfos[playerid][fDistanceWheel], 1000));
		  	strcat(sStats, string);
		  	//---
			ShowPlayerDialog(playerid, DIALOG_STATS, DIALOG_STYLE_MSGBOX, "Stats", sStats, "Fermer", "");
		}
		case LANGUAGE_ES:
	    {
		}
		case LANGUAGE_PG:
	    {
		}
		case LANGUAGE_IT:
	    {
		}
		case LANGUAGE_DE:
	    {
		}
	}
}

ResetPlayerVariables(playerid)//Pour remettre les variables à 0 quand un mec se co par exemple
{
	pPlayerInfos[playerid][dLogState] = UNLOGGED;
	pPlayerInfos[playerid][pPass] = 70123830;
	pPlayerInfos[playerid][pAdmin] = PLAYER;
	format(pPlayerInfos[playerid][sFirstCo], 30, " ");
	format(pPlayerInfos[playerid][sLastCo], 30, " ");
	pPlayerInfos[playerid][pBan] = 0;
	pPlayerInfos[playerid][pKills] = 0;
	pPlayerInfos[playerid][pDeaths] = 0;
	pPlayerInfos[playerid][pGameTime] = 0;
	pPlayerInfos[playerid][pHeadshots] = 0;
	pPlayerInfos[playerid][pZombies] = 0;
	pPlayerInfos[playerid][pBosses] = 0;
	pPlayerInfos[playerid][pMissions] = 0;
	pPlayerInfos[playerid][fDistanceWheel] = 0.0;
	pPlayerInfos[playerid][fDistanceFoot] = 0.0;
	pPlayerInfos[playerid][pGameTime] = 0;
	pPlayerInfos[playerid][pLangue] = LANGUAGE_EN;
	pPlayerInfos[playerid][pBag] = PATROL_PACK;
	pPlayerInfos[playerid][pHunger] = 100;
	pPlayerInfos[playerid][pThirst] = 100;
	pPlayerInfos[playerid][pSleep] = 100;
	pPlayerInfos[playerid][pGold] = 0;
	pPlayerInfos[playerid][pHealth] = 1000;
	pPlayerInfos[playerid][pArmour] = 0;
	pPlayerInfos[playerid][xPos] = 0.0;
	pPlayerInfos[playerid][yPos] = -250.0;
	pPlayerInfos[playerid][zPos] = 5.0;
	pPlayerInfos[playerid][aPos] = 0.0;
	pPlayerInfos[playerid][pTemperature] = 372;
	pPlayerInfos[playerid][bLeg] = true;
	pPlayerInfos[playerid][bHemorragie] = false;
	//---
	pPlayerInfos[playerid][pArme1][0] = 0;
	pPlayerInfos[playerid][pArme1][1] = 0;
	pPlayerInfos[playerid][pArme1][2] = 0;
	pPlayerInfos[playerid][pArme2][0] = 0;
	pPlayerInfos[playerid][pArme2][1] = 0;
	pPlayerInfos[playerid][pArme2][2] = 0;
	pPlayerInfos[playerid][pArme3][0] = 0;
	pPlayerInfos[playerid][pArme3][1] = 0;
	pPlayerInfos[playerid][pArme3][2] = 0;
	pPlayerInfos[playerid][pArme4][0] = 0;
	pPlayerInfos[playerid][pArme4][1] = 0;
	pPlayerInfos[playerid][pArme4][2] = 0;
	//---
	pPlayerInfos[playerid][pSkin] = 0;
	pPlayerInfos[playerid][pChapeau] = 0;
	pPlayerInfos[playerid][pLunettes] = 0;
	pPlayerInfos[playerid][pMasque] = 0;
	pPlayerInfos[playerid][pTorse] = 0;
	for(new i = 0; i < 9; i ++)
	{
	    pPlayerInfos[playerid][fPosChapeau][i] = 0.0;
	    pPlayerInfos[playerid][fPosLunettes][i] = 0.0;
	    pPlayerInfos[playerid][fPosMasque][i] = 0.0;
	    pPlayerInfos[playerid][fPosTorse][i] = 0.0;
	}
	//---
    pPlayerInfos[playerid][HandObject] = 0;
    for(new i = 0; i < 36; i ++) pPlayerInfos[playerid][BagObject][i] = 0;
    //---
    pPlayerInfos[playerid][pLevel] = 1;
    pPlayerInfos[playerid][pExp] = 0;
    pPlayerInfos[playerid][pCompetence] = 1;
    pPlayerInfos[playerid][dBoucher] = 0;
    pPlayerInfos[playerid][dMedecine] = 0;
    pPlayerInfos[playerid][dSante] = 0;
    pPlayerInfos[playerid][dArtisan] = 0;
	pPlayerInfos[playerid][dTransporteur] = 0;
	pPlayerInfos[playerid][dPecheur] = 0;
	pPlayerInfos[playerid][dMecano] = 0;
	pPlayerInfos[playerid][dJardinier] = 0;
	pPlayerInfos[playerid][dAthlete] = 0;
	pPlayerInfos[playerid][dSurvivaliste] = 0;
	pPlayerInfos[playerid][dTank] = 0;
    pPlayerInfos[playerid][dBomberman]  = 0;
    pPlayerInfos[playerid][dHydra]  = 0;
    //---
	pPlayerInfos[playerid][pReggaeShark] = 0;
	pPlayerInfos[playerid][pIntro] = 0;
	pPlayerInfos[playerid][pAmy] = 0;
	pPlayerInfos[playerid][pKen] = 0;
	pPlayerInfos[playerid][pDPO] = 0;
    //---
    for(new i = 0; i < 32; i ++) pPlayerInfos[playerid][bAide][i] = false;
	pPlayerInfos[playerid][dAide1_16] = 0;
	pPlayerInfos[playerid][dAide17_32] = 0;
}

public HasPlayerGold(playerid, gold)
{
	return (pPlayerInfos[playerid][pGold] >= gold) ? true : false;
}

stock GivePlayerHunger(playerid, amount)//Pour donner de la faim ou en enlever, dépend du signe de amount
{
	if(pPlayerInfos[playerid][pHunger] + amount > 100) pPlayerInfos[playerid][pHunger] = 100;//Si ça devient supérieur à 100, on lui met à 100
	else if(pPlayerInfos[playerid][pHunger] + amount < 0) pPlayerInfos[playerid][pHunger] = 0;//Si ça devient inférieur à 0, on lui met à 0
    else pPlayerInfos[playerid][pHunger] += amount;//Sinon, on respecte la consigne originale
    if(pPlayerInfos[playerid][pHunger] == 0)//Si le joueur a 0 points de bouffe, il meurt
 	{
	 	if(!IsPlayerInvincible(playerid)) GivePlayerHealth(playerid, -50, REASON_HUNGER);//On le fait donc mourir
	}
    UpdateInfo(playerid, 1);
    //---AIDE
    if(!pPlayerInfos[playerid][bAide][3] && pPlayerInfos[playerid][pHunger] < 50) ShowPlayerHelp(playerid, 4, 10000);
    //-------
	return pPlayerInfos[playerid][pHunger];
}

stock GetPlayerHunger(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pHunger] : 0;
}

stock GivePlayerThirst(playerid, amount)
{
	if(pPlayerInfos[playerid][pThirst] + amount > 100) pPlayerInfos[playerid][pThirst] = 100;
	else if(pPlayerInfos[playerid][pThirst] + amount < 0) pPlayerInfos[playerid][pThirst] = 0;
    else pPlayerInfos[playerid][pThirst] += amount;
    if(pPlayerInfos[playerid][pThirst] == 0)
	{
		if(!IsPlayerInvincible(playerid)) GivePlayerHealth(playerid, -200, REASON_THIRST);
	}
    UpdateInfo(playerid, 2);
    //---AIDE
    if(!pPlayerInfos[playerid][bAide][4] && pPlayerInfos[playerid][pThirst] < 50) ShowPlayerHelp(playerid, 5, 10000);
    //-------
	return pPlayerInfos[playerid][pThirst];
}

stock GetPlayerThirst(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pThirst] : 0;
}

stock GivePlayerSleep(playerid, amount)//Pour donner ou pas des points de dodo à un joueur
{
	if(pPlayerInfos[playerid][pSleep] + amount > 100) pPlayerInfos[playerid][pSleep] = 100;//S'il a plus de 100, on lui met à 100
	else if(pPlayerInfos[playerid][pSleep] + amount < 0) pPlayerInfos[playerid][pSleep] = 0;//S'il a en dessous de 0, on lui met à 0
    else pPlayerInfos[playerid][pSleep] += amount;//Sinon c'bon, on respecte la consigne
    UpdateInfo(playerid, 3);//On augmente sa barre de dodo
    //---AIDE
    if(!pPlayerInfos[playerid][bAide][5] && pPlayerInfos[playerid][pSleep] < 50) ShowPlayerHelp(playerid, 6, 10000);
    //-------
	return pPlayerInfos[playerid][pSleep];
	/*----------------------------------------------------------------------------¦
	¦Pour le sommeil, on tue pas le mec s'il arrive à 0,                          ¦
	¦par contre on lui fout un blackscreen pour montrer que le perso est fatigué, ¦
	¦et s'il essaie de courir ou de sauter, le mec tombe tellement il est fatigué ¦
	¦-----------------------------------------------------------------------------¦*/
}

stock GetPlayerSleep(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pSleep] : 0;
}

public GivePlayerGold(playerid, amount)//Pour donner de l'or
{
	if(amount > 50) PlayerPlaySound(playerid, 4201, 0.0, 0.0, 0.0);
	else if(amount > 0) PlayerPlaySound(playerid, 1100, 0.0, 0.0, 0.0);
    pPlayerInfos[playerid][pGold] += amount;//On additionne l'or présent du joueur avec le nouveau
    UpdateInfo(playerid, 4);//On actualise le textdraw
	LogInfo(true, "[JOUEUR]%s a gagne %.1fg d'or: %.1fg.", GetName(playerid), floatdiv(amount, 10), floatdiv(pPlayerInfos[playerid][pGold], 10));
	return pPlayerInfos[playerid][pGold];
}

stock GiveOfflinePlayerGold(const playername[], amount)
{
    new string[50];
    format(string,sizeof(string), OFFPATH, playername);
    if(!fexist(string)) return 0;
	INI_ParseFile(string, "LoadUserGold_%s");
    new INI:File = INI_Open(string);
	INI_SetTag(File,"auctions");
	INI_WriteInt(File,"Or", dGoldDumpVar + amount);
	INI_Close(File);
	return dGoldDumpVar + amount;
}

public LoadUserGold_auctions(name[],value[])
{
    INI_Int("Or", dGoldDumpVar);
    return 1;
}

stock bool:IsThereUnsoldItem(playerid)
{
	for(new i = 0; i < 50; i ++) if(pPlayerOfflineInfos[playerid][dReturnedItem][i] != 0) return true;
	return false;
}

//---

stock GetPlayerMaxHealth(playerid)
{
	new dMaxHealth;
	switch(pPlayerInfos[playerid][dSante])
	{
		case 1: dMaxHealth = 1150;
		case 2: dMaxHealth = 1250;
		default: dMaxHealth = 1000;
	}
	return dMaxHealth;
}

stock GetPlayerMaxArmour(playerid)
{
	new dMaxArmour;
	switch(pPlayerInfos[playerid][dTank])
	{
		case 1: dMaxArmour = 1250;
		default: dMaxArmour = 1000;
	}
	return dMaxArmour;
}

stock GivePlayerTemperature(playerid, amount)//Donner ou enlever de la chaleur (Dépend du signe de amount)
{
    if(IsValidPlayer(playerid))
    {
        if(420 > pPlayerInfos[playerid][pTemperature] + amount) pPlayerInfos[playerid][pTemperature] += amount;//Pour que la température bouge encore faut qu'elle soit entre 20 et 42
        UpdateInfo(playerid, 7);//On actualise la température
        if(200 >= pPlayerInfos[playerid][pTemperature] && !IsPlayerInvincible(playerid)) GivePlayerHealth(playerid, -100, WEAPON_DROWN);//Si la température est inférieure à 20°, on zigouille le mec
        //---AIDE
        if(!pPlayerInfos[playerid][bAide][6] && pPlayerInfos[playerid][pTemperature] < 350) ShowPlayerHelp(playerid, 7, 10000);
        //-------
        return pPlayerInfos[playerid][pTemperature];
    }
    return 0;
}

stock SetPlayerTemperature(playerid, amount)
{
    if(IsValidPlayer(playerid))
    {
        pPlayerInfos[playerid][pTemperature] = (420 > amount >= 200) ? amount : 370;
        UpdateInfo(playerid, 7);
        if(!pPlayerInfos[playerid][bAide][6] && pPlayerInfos[playerid][pTemperature] < 350) ShowPlayerHelp(playerid, 7, 10000);
    }
}

stock GetPlayerTemperature(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pTemperature] : 0;
}

stock SetHealthForPlayer(playerid, amount, reason, issuerid = INVALID_PLAYER_ID)//Changer la vie sans prendre en considération ce qu'elle était avant
{
	new dMaxHealth = GetPlayerMaxHealth(playerid);
    if(amount > dMaxHealth) pPlayerInfos[playerid][pHealth] = dMaxHealth;//Si on tente de mettre la vie supérieure à 1000, on la met à 1000
    else if(amount < 0) pPlayerInfos[playerid][pHealth] = 0;//Pareil si on tente de la mettre à moins de 0, on la met à 0
    else pPlayerInfos[playerid][pHealth] = amount;//Sinon, c'est bon, on respecte
    UpdateInfo(playerid, 5);//On actualise la barre de vie
    if(pPlayerInfos[playerid][pHealth] == 0) OnPlayerDies(playerid, issuerid, reason), SetPlayerHealth(playerid, 0.0);//Si la vie est à 0, le joueur meurt
    else SetPlayerHealth(playerid, 999999999.9);//Sinon, on le garde avec la vie à l'infini, pour pas qu'il meurt autrement que si on le décide
	return pPlayerInfos[playerid][pHealth];
}

stock SetArmourForPlayer(playerid, amount)//Pour changer l'armure sans prendre en considération ce qu'elle était avant
{
	new dMaxHealth = GetPlayerMaxArmour(playerid);
	//---
    if(amount > dMaxHealth) pPlayerInfos[playerid][pArmour] = dMaxHealth;//Si on tente de la mettre à plus que 1000, on la met à 1000
    else if(amount < 0) pPlayerInfos[playerid][pArmour] = 0;//Si on tente de la mettre à moins que 0, on la met à 0
    else pPlayerInfos[playerid][pArmour] = amount;//Sinon, on respecte la consigne originale
    //---GILETS
    if(pPlayerInfos[playerid][pArmour] < dMaxHealth && pPlayerInfos[playerid][pTorse] == 83)
    {
        pPlayerInfos[playerid][pTorse] = 84;
        DressPlayer(playerid, 4);
    }
    //---
    UpdateInfo(playerid, 6);//On change la barre
	return pPlayerInfos[playerid][pArmour];
}

stock GivePlayerHealth(playerid, amount, reason, issuerid = INVALID_PLAYER_ID)//Donner ou enlever de la vie (Dépend du signe de amount)
{
	if(IsPlayerInvincible(playerid)) return -1;
	if(pPlayerInfos[playerid][dLogState] == SPAWNED)
	{
		if(amount < 0)
		{
		    #if defined SHOW_DAMAGE_SCREEN
			ShowRedScreen(playerid);
			#endif
			new Float:x, Float:y, Float:z, dBlood = AbsoluteValue(floatround(floatdiv(amount, 10), floatround_ceil));
			GetPlayerPos(playerid, x, y, z);
			CreateBloodSplat(x, y, z);
			for(new i = 0; i < dBlood + 1; i ++) CreateBlood(x + floatdiv(RandomEx(0, 15), 10), y + floatdiv(RandomEx(0, 15), 10), z);
		}
		//---VIE MAXI
		new dMaxHealth = GetPlayerMaxHealth(playerid);
	    if(pPlayerInfos[playerid][pHealth] + amount < 0) pPlayerInfos[playerid][pHealth] = 0;//Si la vie est inférieure à 0, on la met à 0
		else if(pPlayerInfos[playerid][pHealth] + amount > dMaxHealth) pPlayerInfos[playerid][pHealth] = dMaxHealth;//Si la vie devient supérieure à 1000, on la met à 1000
	    else pPlayerInfos[playerid][pHealth] += amount;//Sinon, c'est bon
	    UpdateInfo(playerid, 5);//On actualise la barre de vie
	    if(pPlayerInfos[playerid][pHealth] == 0) OnPlayerDies(playerid, issuerid, reason), SetPlayerHealth(playerid, 0.0);//Si la vie est égale à 0, on zigouille le mec
	    else SetPlayerHealth(playerid, 999999999.9);//Sinon, on lui met la vie à l'infini, comme ça, il meurt que si on le décide
    }
	return pPlayerInfos[playerid][pHealth];
}

stock GivePlayerArmour(playerid, amount, reason, issuerid = INVALID_PLAYER_ID)//Pour ajouter ou enlever de la vie (Dépend du signe de amount)
{
	if(IsPlayerInvincible(playerid)) return -1;
	if(pPlayerInfos[playerid][dLogState] == SPAWNED)
	{
		new dMaxArmour = GetPlayerMaxArmour(playerid);
		//---
	    if(pPlayerInfos[playerid][pArmour] + amount < 0)//Si on enlève plus que l'armure disponnible, on reporte le reste sur la vie
		{
		    new dHealth = pPlayerInfos[playerid][pArmour] + amount;
			GivePlayerHealth(playerid, dHealth, reason, issuerid);
			pPlayerInfos[playerid][pArmour] = 0;//...et on met l'armure à 0
		}
		else if(pPlayerInfos[playerid][pArmour] + amount > dMaxArmour)//Si ça devient supérieur à 1000, on lui remet à 1000
		{
			pPlayerInfos[playerid][pArmour] = dMaxArmour;
		}
	    else pPlayerInfos[playerid][pArmour] += amount;//Sinon, on respecte la consigne originale
	    //---GILETS
	    if(pPlayerInfos[playerid][pArmour] < dMaxArmour && pPlayerInfos[playerid][pTorse] == 83)
	    {
	        pPlayerInfos[playerid][pTorse] = 84;
	        DressPlayer(playerid, 4);
	    }
	    //---
	    UpdateInfo(playerid, 6);//On actualise la barre
    }
	return pPlayerInfos[playerid][pArmour];
}

stock UpdateInfo(playerid, infoid)//Fonction pour actualiser des éléments de l'interface
{
	new string[25];
	switch(infoid)
	{
	    case 1://La faim
		{
			PlayerTextDrawTextSize(playerid, tPlayerInfos[playerid][0], CalculateBarSize(GetPlayerHunger(playerid), 58.0) + 492.0, 0.000000);//On change la taille du rectangle (La barre) pour montrer combien il reste
	    	if(100 >= GetPlayerHunger(playerid) >= 50) PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][0], 0x008000FF);//Et on change aussi la couleur
	    	else if(15 < GetPlayerHunger(playerid) < 50) PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][0], 0xFFFF00FF);
	    	else if(15 > GetPlayerHunger(playerid)) PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][0], 0xCC0000FF);
			if(!IsHUDHiddenForPlayer(playerid)) PlayerTextDrawShow(playerid, tPlayerInfos[playerid][0]);
		}
	    case 2://La soif
		{
			PlayerTextDrawTextSize(playerid, tPlayerInfos[playerid][1], CalculateBarSize(GetPlayerThirst(playerid), 58.0) + 492.0, 0.000000);//Pareil
	    	if(100 >= GetPlayerThirst(playerid) >= 50) PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][1], 0x008000FF);//Et on change aussi la couleur
	    	else if(15 < GetPlayerThirst(playerid) < 50) PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][1], 0xFFFF00FF);
	    	else if(15 > GetPlayerThirst(playerid)) PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][1], 0xCC0000FF);
			if(!IsHUDHiddenForPlayer(playerid)) PlayerTextDrawShow(playerid, tPlayerInfos[playerid][1]);
		}
	    case 3://Le dodo
		{
			PlayerTextDrawTextSize(playerid, tPlayerInfos[playerid][2], CalculateBarSize(GetPlayerSleep(playerid), 58.0) + 492.0, 0.000000);//Pareil
	    	if(100 >= GetPlayerSleep(playerid) >= 50) PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][2], 0x008000FF), TextDrawHideForPlayer(playerid, tDarkScreen);//Et on change aussi la couleur
	    	else if(15 < GetPlayerSleep(playerid) < 50) PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][2], 0xFFFF00FF), TextDrawHideForPlayer(playerid, tDarkScreen);
	    	else if(15 > GetPlayerSleep(playerid)) PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][2], 0xCC0000FF), TextDrawShowForPlayer(playerid, tDarkScreen);
			if(!IsHUDHiddenForPlayer(playerid)) PlayerTextDrawShow(playerid, tPlayerInfos[playerid][2]);
		}
	    case 4://L'or
	    {
	        new sGold[6][6] =
			{
				"Gold", "Or", "Oro", "Ouro", "Oro", "Gold"//On crée un tableau avec 'Or' dans les 6 langues du serveur
			};
	  		format(string, sizeof(string), "%s: %.1fg", sGold[GetPlayerLanguage(playerid)], floatdiv(GetPlayerGold(playerid), 10));//Pour pas créer une float, l'integer de la variable est divisée par 10 pour faire swagg
			PlayerTextDrawSetString(playerid, tPlayerInfos[playerid][3], string);
			if(!IsHUDHiddenForPlayer(playerid)) PlayerTextDrawShow(playerid, tPlayerInfos[playerid][3]);
	    }
	    case 5://La vie
	    {
	        PlayerTextDrawTextSize(playerid, tPlayerInfos[playerid][4], CalculateBarSize(floatdiv(pPlayerInfos[playerid][pHealth], 10), 58.0) + 492.0, 0.000000);//On change juste la barre de vie, la couleur reste pareille
			if(!IsHUDHiddenForPlayer(playerid)) PlayerTextDrawShow(playerid, tPlayerInfos[playerid][4]);
			UpdatePlayerTag(playerid);
		}
	    case 6://L'armure
	    {
	        PlayerTextDrawTextSize(playerid, tPlayerInfos[playerid][5], CalculateBarSize(floatdiv(pPlayerInfos[playerid][pArmour], 10), 58.0) + 492.0, 0.000000);//Pareil pour l'armure
			if(!IsHUDHiddenForPlayer(playerid)) PlayerTextDrawShow(playerid, tPlayerInfos[playerid][5]);
			UpdatePlayerTag(playerid);
		}
	    case 7://La température
	 	{
	    	new sTemperatures[6][15] =
			{
				"Temperature", "Temperature", "Temperatura", "Temperatura", "Temperatura", "Temperatur"//On crée un tableau avec 'Température' dans les 6 langues du serveur
			};
	  		format(string, sizeof(string), "%s:~n~%.1f", sTemperatures[GetPlayerLanguage(playerid)], floatdiv(GetPlayerTemperature(playerid), 10));//On crée pas de float non plus pour la température
			PlayerTextDrawSetString(playerid, tPlayerInfos[playerid][6], string);
			if(!IsHUDHiddenForPlayer(playerid)) PlayerTextDrawShow(playerid, tPlayerInfos[playerid][6]);
	    }
	    case 8://Si le mec a la jambe pêtée
		{
			if(!GetPlayerLegState(playerid) && !IsHUDHiddenForPlayer(playerid)) TextDrawShowForPlayer(playerid, tBrokenLeg[GetPlayerLanguage(playerid)]);
			else TextDrawHideForPlayer(playerid, tBrokenLeg[GetPlayerLanguage(playerid)]);//On cache ou on affiche le textdraw en fonction de si le mec a la jambe pêtée ou pas
		}
		case 9://Si le mec fait une hémorragie
		{
			if(GetPlayerBleedingState(playerid) && !IsHUDHiddenForPlayer(playerid)) TextDrawShowForPlayer(playerid, tHemoragie[GetPlayerLanguage(playerid)]);
			else TextDrawHideForPlayer(playerid, tHemoragie[GetPlayerLanguage(playerid)]);//On cache ou on affiche le textdraw en fonction de si le mec a la jambe pêtée ou pas
		}
		case 10://Pour l'essence
		{
		    if(IsPlayerInAnyVehicle(playerid) && GetVehicleID(GetPlayerVehicleID(playerid)) != -1 && !IsVehicleBicycle(GetPlayerVehicleID(playerid)))
		    {
	      		format(string, sizeof(string), "Fuel:~n~%.2f l", floatdiv(dVehicleInfos[GetVehicleID(GetPlayerVehicleID(playerid))][dFuel], 100));//On crée pas de float non plus pour la température
				if(!IsHUDHiddenForPlayer(playerid)) PlayerTextDrawSetString(playerid, tPlayerInfos[playerid][7], string);
				PlayerTextDrawShow(playerid, tPlayerInfos[playerid][7]);
			}
			else
			{
				PlayerTextDrawHide(playerid, tPlayerInfos[playerid][7]);
			}
		}
		case 11://Level
		{
		    format(string, sizeof(string), "Level %d", pPlayerInfos[playerid][pLevel]);
		    PlayerTextDrawSetString(playerid, tLevel[playerid][0], string);
			SetPlayerScore(playerid, pPlayerInfos[playerid][pLevel]);
		    //---
			new dExpInfos;
			//---
			dExpInfos = floatround(floatdiv(pPlayerInfos[playerid][pExp], GetLevelExp(pPlayerInfos[playerid][pLevel])) * 10.0, floatround_floor);
		    PlayerTextDrawTextSize(playerid, tLevel[playerid][1], dExpInfos * 14.0, 0.000000);
		    PlayerTextDrawTextSize(playerid, tLevel[playerid][2], (10 * floatdiv(pPlayerInfos[playerid][pExp], GetLevelExp(pPlayerInfos[playerid][pLevel])) - (dExpInfos)) * 140.0, 0.000000);
		    format(string, sizeof(string), "Level %d", pPlayerInfos[playerid][pLevel]);
			//---
			if(!IsHUDHiddenForPlayer(playerid))
			{
				TextDrawShowForPlayer(playerid, tFondLevel);
				PlayerTextDrawShow(playerid, tLevel[playerid][0]);
				PlayerTextDrawShow(playerid, tLevel[playerid][1]);
				//---
				for(new i = 0; i < 10; i ++) TextDrawShowForPlayer(playerid, tBarettesLevel[i]);
				//---
				PlayerTextDrawShow(playerid, tLevel[playerid][2]);
			}
		}
	}
}

public GivePlayerExp(playerid, exp)
{
	if(pGroup[playerid] != -1 && exp > 0)//Si le joueur est dans un groupe, on partage l'exp
	{
	    GiveGroupExp(pGroup[playerid], floatround(floatdiv(exp, GetGroupSize(pGroup[playerid]) + 0.1) , floatround_ceil));
	    return 1;
	}
	if(pPlayerInfos[playerid][pExp] + exp >= GetLevelExp(pPlayerInfos[playerid][pLevel]))
	{
		LogInfo(true, "[EXP]%s obtient %d exp.", GetName(playerid), exp);
	    new dLeftExp = exp;
		while(pPlayerInfos[playerid][pExp] + dLeftExp >= GetLevelExp(pPlayerInfos[playerid][pLevel]))
		{
			pPlayerInfos[playerid][pExp] += (dLeftExp - GetLevelExp(pPlayerInfos[playerid][pLevel]));
			pPlayerInfos[playerid][pLevel] ++;
			pPlayerInfos[playerid][pCompetence] ++;
			if(pPlayerInfos[playerid][pExp] + dLeftExp > GetLevelExp(pPlayerInfos[playerid][pLevel])) dLeftExp = GetLevelExp(pPlayerInfos[playerid][pLevel] - pPlayerInfos[playerid][pExp]);
			OnPlayerLevelUp(playerid, pPlayerInfos[playerid][pLevel]);
			LogInfo(true, "[EXP]%s passe au niveau %d.", GetName(playerid), pPlayerInfos[playerid][pLevel]);
		}
	}
	else if(pPlayerInfos[playerid][pExp] + exp < 0)
	{
		LogInfo(true, "[EXP]%s obtient %d exp.", GetName(playerid), -pPlayerInfos[playerid][pExp]);
		pPlayerInfos[playerid][pExp] = 0;
	}
	else
	{
		LogInfo(true, "[EXP]%s obtient %d exp.", GetName(playerid), exp);
		pPlayerInfos[playerid][pExp] += exp;
	}
	//---
	UpdateInfo(playerid, 11);
	return pPlayerInfos[playerid][pExp];
}

stock OnPlayerLevelUp(playerid, level)
{
	if(FCNPC_IsValid(playerid)) return false;
	LogInfo(true, "[EXP]%s monte au niveau %d.", GetName(playerid), level);
	if(!pPlayerInfos[playerid][bAide][16]) ShowPlayerHelp(playerid, 17, 10000);
	//---
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    //---
    new objectid = CreateObject(18671, x, y, z - 2.5, 0.0, 0.0, 0.0, 50.0);
    AttachObjectToPlayer(objectid, playerid, 0.0, 0.0, -2.5, 0.0, 0.0, 0.0);
	SetTimerEx("DestroyObjectEx", 2000, false, "ib", objectid, false);
    objectid = CreateObject(18728, x, y, z - 1.5, 0.0, 0.0, 0.0, 50.0);
    AttachObjectToPlayer(objectid, playerid, 0.0, 0.0, -1.5, 0.0, 0.0, 0.0);
	SetTimerEx("DestroyObjectEx", 2000, false, "ib", objectid, false);
	//---
	PlayerPlaySound(playerid, 11422, 0.0, 0.0, 0.0);
	return true;
}

stock ShowPlayerUpgradeDialog(playerid, upgradeprice)
{
	new sSkills[512];
    switch(GetPlayerLanguage(playerid))
    {
        case LANGUAGE_EN: format(string, sizeof(string), "\n{FFFFFF}You can upgrade this skill for %d skill points.\nYou currently have %d.", upgradeprice, GetPlayerSkillPoint(playerid));
        case LANGUAGE_FR: format(string, sizeof(string), "\n{FFFFFF}Vous pouvez augmenter cette compétence pour %d points de compétence.\nVous en avez %d.", upgradeprice, GetPlayerSkillPoint(playerid));
        case LANGUAGE_ES: format(string, sizeof(string), "\n{FFFFFF}Espagnol %s%d Espagnol.\nEspagnol %d.", upgradeprice, GetPlayerSkillPoint(playerid));
        case LANGUAGE_PG: format(string, sizeof(string), "\n{FFFFFF}Portugais %s%d Portugais.\nPortugais %d.", upgradeprice, GetPlayerSkillPoint(playerid));
        case LANGUAGE_IT: format(string, sizeof(string), "\n{FFFFFF}Italien %s%d Italien.\nItalien %d.", upgradeprice, GetPlayerSkillPoint(playerid));
        case LANGUAGE_DE: format(string, sizeof(string), "\n{FFFFFF}Allemand %s%d Allemand.\nAllemand %d.", upgradeprice, GetPlayerSkillPoint(playerid));
    }
    strcat(sSkills, string);
    switch(GetPlayerLanguage(playerid))
    {
        case LANGUAGE_EN: ShowPlayerDialog(playerid, DIALOG_SKILLS_UPGRADE, DIALOG_STYLE_MSGBOX, "Skills", sSkills, "Upgrade", "Back");
        case LANGUAGE_FR: ShowPlayerDialog(playerid, DIALOG_SKILLS_UPGRADE, DIALOG_STYLE_MSGBOX, "Compétences", sSkills, "Augmenter", "Retour");
        case LANGUAGE_ES: ShowPlayerDialog(playerid, DIALOG_SKILLS_UPGRADE, DIALOG_STYLE_MSGBOX, "Espagnol", sSkills, "Ok", "Cancelar");
        case LANGUAGE_PG: ShowPlayerDialog(playerid, DIALOG_SKILLS_UPGRADE, DIALOG_STYLE_MSGBOX, "Portugais", sSkills, "Ok", "Portugais");
        case LANGUAGE_IT: ShowPlayerDialog(playerid, DIALOG_SKILLS_UPGRADE, DIALOG_STYLE_MSGBOX, "Italien", sSkills, "Ok", "Italien");
        case LANGUAGE_DE: ShowPlayerDialog(playerid, DIALOG_SKILLS_UPGRADE, DIALOG_STYLE_MSGBOX, "Allemand", sSkills, "Ok", "Schlissen");
    }
}

stock SetPlayerFreezeState(playerid, state)
{
    if(IsValidPlayer(playerid) && state >= 0)
    {
        pPlayerInfos[playerid][pFreeze] = 0;
    }
}

stock GetPlayerFreezeState(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pFreeze] : 0;
}

stock DecreasePlayerFreeze(playerid)
{
    if(IsValidPlayer(playerid))
    {
        if(pPlayerInfos[playerid][pFreeze] > 0) 
        {
            if(-- pPlayerInfos[playerid][pFreeze] == 0) 
            {
                return true;
            }
        }
    }
    return false;
}

stock SetPlayerMuteState(playerid, state)
{
    if(IsValidPlayer(playerid) && state >= 0)
    {
        pPlayerInfos[playerid][pMute] = 0;
    }
}

stock GetPlayerMuteState(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pMute] : 0;
}

stock DecreasePlayerMute(playerid)
{
    if(IsValidPlayer(playerid))
    {
        if(pPlayerInfos[playerid][pMute] > 0) 
        {
            if(-- pPlayerInfos[playerid][pMute] == 0) 
            {
                return true;
            }
        }
    }
    return false;
}

stock SetPlayerAdminLevel(playerid, level)
{
	if(IsValidPlayer(playerid))
	{
		pPlayerInfos[playerid][pAdmin] = level;
		//---
		if(level == PLAYER) TextDrawHideForPlayer(playerid, tAdmin);
		else TextDrawShowForPlayer(playerid, tAdmin);
	}
}

stock GetPlayerAdminLevel(playerid)
{
	return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pAdmin] : PLAYER;
}

stock GetPlayerBan(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pBan] : -1;
}

stock GetPlayerBag(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pBag] : PATROL_PACK;
}

stock SetPlayerBag(playerid, bagid) 
{
    if(IsValidPlayer(playerid))
    {
        pPlayerInfos[playerid][pBag] = (COYOTE_PACK >= bagid >= PATROL_PACK) ? bagid : PATROL_PACK;
    }
}

stock bool:GetPlayerLegState(playerid)
{
	return IsValidPlayer(playerid) ? pPlayerInfos[playerid][bLeg] : true;
}

stock SetPlayerLegState(playerid, bool:leg)
{
    if(IsValidPlayer(playerid))
    {
        pPlayerInfos[playerid][bLeg] = leg;
        UpdateInfo(playerid, 8);
    }
}

stock bool:GetPlayerBleedingState(playerid)
{
	return IsValidPlayer(playerid) ? pPlayerInfos[playerid][bHemorragie] : false;
}

stock SetPlayerBleedingState(playerid, bool:bleeding)
{
    if(IsValidPlayer(playerid))
    {
        pPlayerInfos[playerid][bHemorragie] = bleeding;
        UpdateInfo(playerid, 9);
    }
}

stock bool:GetPlayerHelpState(playerid, helpid)
{
	return (IsValidPlayer(playerid) && (sizeof(pPlayerInfos[playerid][bAide]) > helpid >= 0)) ? pPlayerInfos[playerid][bAide][helpid] : false;
}

stock SetPlayerHelpState(playerid, helpid, bool:state)
{
    if(IsValidPlayer(playerid) && (sizeof(pPlayerInfos[playerid][bAide]) > helpid >= 0))
    {
        pPlayerInfos[playerid][bAide][helpid] = state;
    }
}

stock GetPlayerLogState(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][dLogState] : UNLOGGED;
}

stock SetPlayerLogState(playerid, state)
{
    if(IsValidPlayer(playerid)) 
    {
        pPlayerInfos[playerid][dLogState] = state;
    }
}

stock SetPlayerSkinEx(playerid, skinid)
{
    if(IsValidPlayer(playerid)) 
    {
        pPlayerInfos[playerid][pSkin] = state;
    }
}

stock GetPlayerSkinEx(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pSkin] : 0;
}

stock GetPlayerSkill(playerid, skillid)
{
    if(IsValidPlayer(playerid))
    {
        switch(skillid)
        {
            case BUTCHER_ID: return pPlayerInfos[playerid][dBoucher];
            case DOCTOR_ID: return pPlayerInfos[playerid][dMedecine];
            case HEALTH_ID: return pPlayerInfos[playerid][dSante];
            case CRAFTER_ID: return pPlayerInfos[playerid][dArtisan];
            case TRANSPORTER_ID: return pPlayerInfos[playerid][dTransporteur];
            case FISHERMAN_ID: return pPlayerInfos[playerid][dPecheur];
            case MECANIC_ID: return pPlayerInfos[playerid][dMecano];
            case FARMER_ID: return pPlayerInfos[playerid][dJardinier];
            case ATHLETE_ID: return pPlayerInfos[playerid][dAthlete];
            case SURVIVOR_ID: return pPlayerInfos[playerid][dSurvivaliste];
            case TANK_ID: return pPlayerInfos[playerid][dTank];
            case BOMBERMAN_ID: return pPlayerInfos[playerid][dBomberman];
            case HYDRATATION_ID: return pPlayerInfos[playerid][dHydra];
            default: return 0;
        }
    }
}

stock SetPlayerSkill(playerid, skillid, level)
{
    if(IsValidPlayer(playerid))
    {
        switch(skillid)
        {
            case BUTCHER_ID: pPlayerInfos[playerid][dBoucher] = ((level > BUTCHER) ? BUTCHER : (level < 0) ? 0 : level);
            case DOCTOR_ID: pPlayerInfos[playerid][dMedecine] = ((level > DOCTOR) ? DOCTOR : (level < 0) ? 0 : level);
            case HEALTH_ID: pPlayerInfos[playerid][dSante] = ((level > HEALTH) ? HEALTH : (level < 0) ? 0 : level);
            case CRAFTER_ID: pPlayerInfos[playerid][dArtisan] = ((level > CRAFTER) ? CRAFTER : (level < 0) ? 0 : level);
            case TRANSPORTER_ID: pPlayerInfos[playerid][dTransporteur] = ((level > TRANSPORTER) ? TRANSPORTER : (level < 0) ? 0 : level);
            case FISHERMAN_ID: pPlayerInfos[playerid][dPecheur] = ((level > FISHERMAN) ? FISHERMAN : (level < 0) ? 0 : level);
            case MECANIC_ID: pPlayerInfos[playerid][dMecano] = ((level > MECANIC) ? MECANIC : (level < 0) ? 0 : level);
            case FARMER_ID: pPlayerInfos[playerid][dJardinier] = ((level > FARMER) ? FARMER : (level < 0) ? 0 : level);
            case ATHLETE_ID: pPlayerInfos[playerid][dAthlete] = ((level > ATHLETE) ? ATHLETE : (level < 0) ? 0 : level);
            case SURVIVOR_ID: pPlayerInfos[playerid][dSurvivaliste] = ((level > SURVIVOR) ? SURVIVOR : (level < 0) ? 0 : level);
            case TANK_ID: pPlayerInfos[playerid][dTank] = ((level > TANK) ? TANK : (level < 0) ? 0 : level);
            case BOMBERMAN_ID: pPlayerInfos[playerid][dBomberman] = ((level > BOMBERMAN) ? BOMBERMAN : (level < 0) ? 0 : level);
            case HYDRATATION_ID: pPlayerInfos[playerid][dHydra] = ((level > HYDRATATION) ? HYDRATATION : (level < 0) ? 0 : level);
        }
    }
}

stock GetPlayerSkillPoint(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pCompetence] : 0;
}

stock GivePlayerSkillPoint(playerid, skillpoints)
{
    if(IsValidPlayer(playerid))
    {
        pPlayerInfos[playerid][pCompetence] += skillpoints;
    }
}

stock GetPlayerClothes(playerid, part)
{
    if(IsValidPlayer(playerid))
    {
        switch(part)
        {
            case HAT_OBJECT_ID: return pPlayerInfos[playerid][pChapeau];
            case GLASSES_OBJECT_ID: return pPlayerInfos[playerid][pLunettes];
            case MASK_OBJECT_ID: return pPlayerInfos[playerid][pMasque];
            case TORSO_OBJECT_ID: return pPlayerInfos[playerid][pTorse];
            default: return 0;
        }
    }
}

stock GivePlayerClothe(playerid, slot, objectid, bool:repositionning, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, Float:sx = 0.0, Float:sy = 0.0, Float:sz = 0.0)
{
	switch(slot)
	{
		case HAT_OBJECT_ID://CHAPEAU
		{
			if(pPlayerInfos[playerid][pChapeau] != 0 && !repositionning) PlayerDropObject(playerid, pPlayerInfos[playerid][pChapeau], floatdiv(RandomEx(5, 20), 10));
 			pPlayerInfos[playerid][fPosChapeau][0] = x;
			pPlayerInfos[playerid][fPosChapeau][1] = y;
			pPlayerInfos[playerid][fPosChapeau][2] = z;
			pPlayerInfos[playerid][fPosChapeau][3] = rx;
			pPlayerInfos[playerid][fPosChapeau][4] = ry;
			pPlayerInfos[playerid][fPosChapeau][5] = rz;
			pPlayerInfos[playerid][fPosChapeau][6] = sx;
			pPlayerInfos[playerid][fPosChapeau][7] = sy;
			pPlayerInfos[playerid][fPosChapeau][8] = sz;
		    S_SetPlayerAttachedObject(playerid, HAT_OBJECT_ID, GetObjectModelID(objectid), 2, x, y, z, rx, ry, rz, sx, sy, sz, -1, -1);
		    if(objectid != 0 && objectid != pPlayerInfos[playerid][pChapeau]) EditAttachedObject(playerid, HAT_OBJECT_ID);
			pPlayerInfos[playerid][pChapeau] = objectid;
		}
		case GLASSES_OBJECT_ID://LUNETTES
		{
			if(pPlayerInfos[playerid][pLunettes] != 0 && !repositionning) PlayerDropObject(playerid, pPlayerInfos[playerid][pLunettes], floatdiv(RandomEx(5, 20), 10));
 			pPlayerInfos[playerid][fPosLunettes][0] = x;
			pPlayerInfos[playerid][fPosLunettes][1] = y;
			pPlayerInfos[playerid][fPosLunettes][2] = z;
			pPlayerInfos[playerid][fPosLunettes][3] = rx;
			pPlayerInfos[playerid][fPosLunettes][4] = ry;
			pPlayerInfos[playerid][fPosLunettes][5] = rz;
			pPlayerInfos[playerid][fPosLunettes][6] = sx;
			pPlayerInfos[playerid][fPosLunettes][7] = sy;
			pPlayerInfos[playerid][fPosLunettes][8] = sz;
		    S_SetPlayerAttachedObject(playerid, GLASSES_OBJECT_ID, GetObjectModelID(objectid), 2, x, y, z, rx, ry, rz, sx, sy, sz, -1, -1);
		    if(objectid != 0 && objectid != pPlayerInfos[playerid][pLunettes]) EditAttachedObject(playerid, GLASSES_OBJECT_ID);
			pPlayerInfos[playerid][pLunettes] = objectid;
		}
		case MASK_OBJECT_ID://MASQUE
		{
			if(pPlayerInfos[playerid][pMasque] != 0 && !repositionning) PlayerDropObject(playerid, pPlayerInfos[playerid][pMasque], floatdiv(RandomEx(5, 20), 10));
 			pPlayerInfos[playerid][fPosMasque][0] = x;
			pPlayerInfos[playerid][fPosMasque][1] = y;
			pPlayerInfos[playerid][fPosMasque][2] = z;
			pPlayerInfos[playerid][fPosMasque][3] = rx;
			pPlayerInfos[playerid][fPosMasque][4] = ry;
			pPlayerInfos[playerid][fPosMasque][5] = rz;
			pPlayerInfos[playerid][fPosMasque][6] = sx;
			pPlayerInfos[playerid][fPosMasque][7] = sy;
			pPlayerInfos[playerid][fPosMasque][8] = sz;
		    S_SetPlayerAttachedObject(playerid, MASK_OBJECT_ID, GetObjectModelID(objectid), 2, x, y, z, rx, ry, rz, sx, sy, sz, -1, -1);
		    if(objectid != 0 && objectid != pPlayerInfos[playerid][pMasque]) EditAttachedObject(playerid, MASK_OBJECT_ID);
			pPlayerInfos[playerid][pMasque] = objectid;
		}
		case TORSO_OBJECT_ID://TORSE
		{
			if(pPlayerInfos[playerid][pTorse] != 0 && !repositionning) PlayerDropObject(playerid, pPlayerInfos[playerid][pTorse], floatdiv(RandomEx(5, 20), 10));
 			pPlayerInfos[playerid][fPosTorse][0] = x;
			pPlayerInfos[playerid][fPosTorse][1] = y;
			pPlayerInfos[playerid][fPosTorse][2] = z;
			pPlayerInfos[playerid][fPosTorse][3] = rx;
			pPlayerInfos[playerid][fPosTorse][4] = ry;
			pPlayerInfos[playerid][fPosTorse][5] = rz;
			pPlayerInfos[playerid][fPosTorse][6] = sx;
			pPlayerInfos[playerid][fPosTorse][7] = sy;
			pPlayerInfos[playerid][fPosTorse][8] = sz;
		    S_SetPlayerAttachedObject(playerid, TORSO_OBJECT_ID, GetObjectModelID(objectid), 1, x, y, z, rx, ry, rz, sx, sy, sz, -1, -1);
		    if(objectid != 0 && objectid != pPlayerInfos[playerid][Torse]) EditAttachedObject(playerid, TORSO_OBJECT_ID);
			pPlayerInfos[playerid][pTorse] = objectid;
		}
	}
}

stock DressPlayer(playerid, part)
{
	switch(part)
	{
	    case 0://SKIN
	    {
	        SetPlayerSkin(playerid, GetPlayerSkinEx(playerid));
	    }
	    case 1://CHAPEAU
	    {
	        S_RemovePlayerAttachedObject(playerid, HAT_OBJECT_ID);
	        if(pPlayerInfos[playerid][pChapeau] != 0) S_SetPlayerAttachedObject(playerid, HAT_OBJECT_ID, aObjects[pPlayerInfos[playerid][pChapeau]][ObjectModelID], 2, pPlayerInfos[playerid][fPosChapeau][0], pPlayerInfos[playerid][fPosChapeau][1], pPlayerInfos[playerid][fPosChapeau][2], pPlayerInfos[playerid][fPosChapeau][3], pPlayerInfos[playerid][fPosChapeau][4], pPlayerInfos[playerid][fPosChapeau][5], pPlayerInfos[playerid][fPosChapeau][6], pPlayerInfos[playerid][fPosChapeau][7], pPlayerInfos[playerid][fPosChapeau][8], -1, -1);
	    }
	    case 2://LUNETTES
	    {
	        S_RemovePlayerAttachedObject(playerid, GLASSES_OBJECT_ID);
	        if(pPlayerInfos[playerid][pLunettes] != 0) S_SetPlayerAttachedObject(playerid, GLASSES_OBJECT_ID, aObjects[pPlayerInfos[playerid][pLunettes]][ObjectModelID], 2, pPlayerInfos[playerid][fPosLunettes][0], pPlayerInfos[playerid][fPosLunettes][1], pPlayerInfos[playerid][fPosLunettes][2], pPlayerInfos[playerid][fPosLunettes][3], pPlayerInfos[playerid][fPosLunettes][4], pPlayerInfos[playerid][fPosLunettes][5], pPlayerInfos[playerid][fPosLunettes][6], pPlayerInfos[playerid][fPosLunettes][7], pPlayerInfos[playerid][fPosLunettes][8], -1, -1);
	    }
	    case 3://MASQUE
	    {
	        S_RemovePlayerAttachedObject(playerid, MASK_OBJECT_ID);
	        if(pPlayerInfos[playerid][pMasque] != 0) S_SetPlayerAttachedObject(playerid, MASK_OBJECT_ID, aObjects[pPlayerInfos[playerid][pMasque]][ObjectModelID], 2, pPlayerInfos[playerid][fPosMasque][0], pPlayerInfos[playerid][fPosMasque][1], pPlayerInfos[playerid][fPosMasque][2], pPlayerInfos[playerid][fPosMasque][3], pPlayerInfos[playerid][fPosMasque][4], pPlayerInfos[playerid][fPosMasque][5], pPlayerInfos[playerid][fPosMasque][6], pPlayerInfos[playerid][fPosMasque][7], pPlayerInfos[playerid][fPosMasque][8], -1, -1);
	    }
	    case 4://TORSE
	    {
	        S_RemovePlayerAttachedObject(playerid, TORSO_OBJECT_ID);
	        if(pPlayerInfos[playerid][pTorse] != 0) S_SetPlayerAttachedObject(playerid, TORSO_OBJECT_ID, aObjects[pPlayerInfos[playerid][pTorse]][ObjectModelID], 1, pPlayerInfos[playerid][fPosTorse][0], pPlayerInfos[playerid][fPosTorse][1], pPlayerInfos[playerid][fPosTorse][2], pPlayerInfos[playerid][fPosTorse][3], pPlayerInfos[playerid][fPosTorse][4], pPlayerInfos[playerid][fPosTorse][5], pPlayerInfos[playerid][fPosTorse][6], pPlayerInfos[playerid][fPosTorse][7], pPlayerInfos[playerid][fPosTorse][8], -1, -1);
	    }
	    case 5://ARME 1
	    {
			S_RemovePlayerAttachedObject(playerid, GUN1_OBJECT_ID);
			if(GetPlayerWeapon(playerid) != pPlayerInfos[playerid][pArme1][0] && pPlayerInfos[playerid][pArme1][0] != 0) AttachPlayerWeapon(playerid, pPlayerInfos[playerid][pArme1][0], GUN1_OBJECT_ID);
	    }
	    case 6://ARME 2
	    {
			S_RemovePlayerAttachedObject(playerid, GUN2_OBJECT_ID);
			if(GetPlayerWeapon(playerid) != pPlayerInfos[playerid][pArme2][0] && pPlayerInfos[playerid][pArme2][0] != 0) AttachPlayerWeapon(playerid, pPlayerInfos[playerid][pArme2][0], GUN2_OBJECT_ID);
	    }
	    case 7://ARME 3
	    {
			S_RemovePlayerAttachedObject(playerid, GUN3_OBJECT_ID);
			if(GetPlayerWeapon(playerid) != pPlayerInfos[playerid][pArme3][0] && pPlayerInfos[playerid][pArme3][0] != 0) AttachPlayerWeapon(playerid, pPlayerInfos[playerid][pArme3][0], GUN3_OBJECT_ID);
	    }
	    case 8://ARME 4
	    {
			S_RemovePlayerAttachedObject(playerid, GUN4_OBJECT_ID);
			if(GetPlayerWeapon(playerid) != pPlayerInfos[playerid][pArme4][0] && pPlayerInfos[playerid][pArme4][0] != 0) AttachPlayerWeapon(playerid, pPlayerInfos[playerid][pArme4][0], GUN4_OBJECT_ID);
	    }
	}
}

stock AttachPlayerWeapon(playerid, weaponid, slotid)
{
	enum WeaponPos
	{
	    dModelID,
		dBodyPart,
		Float:x,
		Float:y,
		Float:z,
		Float:rx,
		Float:ry,
		Float:rz
	}
	new aWeaponPos[][WeaponPos] =
	{
		{19300, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//POING
		{331, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//POING AMÉRICAIN
		{333, 7, -0.2848, 0.0382, -0.0914, 0.0, 90.0, 270.0},//CLUB DE GOLF
		{334, 7, 0.0903, 0.0899, -0.0914, 0.0, 90.0, 270.0},//MATRAQUE
		{335, 7, -0.0828, 0.0899, -0.0914, 0.0, 90.0, 270.0},//COUTEAU
		{336, 7, -0.0828, 0.0899, -0.0914, 0.0, 90.0, 270.0},//BATTE
		{337, 7, -0.0828, 0.0267, -0.1993, 0.0, 90.0, 180.0},//PELLE
		{338, 7, -0.0828, 0.0267, -0.1993, 0.0, 90.0, 180.0},//QUEUE
		{339, 7, -0.0854, 0.0371, -0.1505, 90.0, 60.0, 15.0},//KATANA
		{341, 7, -0.4333, 0.1525, -0.2142, 90.0, 0.0, 0.0},//TRONCONNEUSE
		{321, 7, -0.0690, 0.0347, -0.1654, 90.0, 90.0, 0.0},//GODE MAUVE
		{322, 7, 0.1035, -0.0542, -0.1654, 90.0, 90.0, 0.0},//PETIT GODE
		{323, 7, 0.1035, -0.0542, -0.1654, 90.0, 90.0, 0.0},//GODE BLANC
		{324, 7, 0.1035, -0.0542, -0.1654, 90.0, 90.0, 0.0},//GODE ARGENTÉ
		{325, 7, 0.1035, -0.0542, -0.1654, 90.0, 90.0, 0.0},//FLEURS
		{326, 7, 0.1035, -0.0542, -0.1654, 90.0, 90.0, 0.0},//CANNE
		{342, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//GRENADE
		{343, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//LACRYMO
		{344, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//MOLOTOVS
		{19300, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//...
		{19300, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//...
		{19300, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//...
		{346, 12, 0.2477, -0.0357, -0.0733, 270.0, 0.0, 0.0},//PISTOLET
		{347, 12, 0.1658, -0.0357, -0.0733, 270.0, 0.0, 0.0},//SILENCIEUX
		{348, 12, 0.1658, -0.0357, -0.0733, 270.0, 0.0, 0.0},//DEAGLE
		{349, 1, -0.1225, -0.1543, -0.1081, 0.0, 345.0, 0.0},//FUSIL À POMPE
		{350, 1, -0.1225, -0.1543, -0.0472, 0.0, 345.0, 0.0},//SAWN OFF
		{351, 1, -0.1225, -0.1543, -0.1081, 0.0, 345.0, 0.0},//SPAS 12
		{352, 8, 0.0820, -0.0033, 0.1159, 270.0, 0.0, 0.0},//UZI
		{353, 8, -0.0186, -0.1150, 0.1159, 270.0, 0.0, 0.0},//MP5
		{355, 1, -0.1225, -0.0901, 0.0359, 180.0, 345.0, 0.0},//AK-47
		{356, 1, -0.1225, -0.0901, 0.0359, 180.0, 345.0, 0.0},//M4
		{372, 8, 0.0820, -0.0033, 0.1159, 270.0, 0.0, 0.0},//TEC-9
		{357, 1, -0.1225, -0.1749, 0.0359, 270.0, 18.5918, 0.0},//FUSIL DE CHASSE
		{358, 1, -0.1225, -0.1749, 0.0359, 270.0, 18.5918, 0.0},//SNIPER
		{359, 1, 0.3667, -0.0225, 0.1114, 90.0, 0.0, 0.0},//RPG
		{360, 1, 0.3667, -0.0225, 0.1114, 90.0, 0.0, 0.0},//HEAT SEEKER
		{361, 1, -0.3851, -0.3565, 0.114, 90.0, 29.5, 0.0},//LANCE-FLAMMES
		{362, 1, -0.3851, -0.3565, 0.114, 90.0, 29.5, 0.0},//MINIGUN
		{363, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//CHARGES EXPLOSIVES
		{19300, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},//...
		{365, 7, -0.0035, -0.0190, -0.1908, 0.0, 270.0, 0.0},//BOMBE DE PEINTURE
		{366, 7, -0.0035, -0.0190, -0.1908, 0.0, 270.0, 0.0}//EXTINCTEUR
	};
	S_SetPlayerAttachedObject(playerid, slotid, aWeaponPos[weaponid][dModelID], aWeaponPos[weaponid][dBodyPart], aWeaponPos[weaponid][x], aWeaponPos[weaponid][y], aWeaponPos[weaponid][z], aWeaponPos[weaponid][rx], aWeaponPos[weaponid][ry], aWeaponPos[weaponid][rz], 1.0, 1.0, 1.0, -1, -1);
}

stock SaveCurrentPos(playerid)
{
    if(IsValidPlayer(playerid))
    {
        new Float:x, Float:y, Float:z, Float:a;
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);

        SetPlayerPosEx(playerid, x, y, z);
        SetPlayerFacingAngleEx(playerid, a);
    }
}

stock LoadPos(playerid)
{
    if(IsValidPlayer(playerid))
    {
        SetPlayerPos(playerid, pPlayerInfos[playerid][xPos], pPlayerInfos[playerid][yPos], pPlayerInfos[playerid][zPos]);
        SetPlayerFacingAngle(playerid, pPlayerInfos[playerid][aPos]);
    }
}

stock SetPlayerPosEx(playerid, Float:x, Float:y, Float:z)
{
    if(IsValidPlayer(playerid))
    {
        pPlayerInfos[playerid][xPos] = x;
        pPlayerInfos[playerid][yPos] = y;
        pPlayerInfos[playerid][zPos] = z;
    }
}

stock GetPlayerPosEx(playerid, &Float:x, &Float:y, &Float:z)
{
    if(IsValidPlayer(playerid))
    {
        x = pPlayerInfos[playerid][xPos];
        y = pPlayerInfos[playerid][yPos];
        z = pPlayerInfos[playerid][zPos];
    }
}

stock SetPlayerFacingAngleEx(playerid, Float:a)
{
    if(IsValidPlayer(playerid))
    {
        pPlayerInfos[playerid][aPos] = a;
    }
}

stock GetPlayerFacingAngleEx(playerid, &Float:a)
{
    if(IsValidPlayer(playerid))
    {
        a = pPlayerInfos[playerid][aPos];
    }
}

stock SetPlayerLevel(playerid, level)
{
    if(IsValidPlayer(playerid) && level >= 0)
    {
        pPlayerInfos[playerid][pLevel] = level;
    }
}

stock GetPlayerLevel(playerid)
{
    return IsValidPlayer(playerid) ? pPlayerInfos[playerid][pLevel] : 0;
}

//---AIDE---//
/*
AIDE:

1- Ramasser un objet:
	Vous pouvez ramasser un objet, un lit, une tente sur le sol.
	Appuyez sur H pour ramasser quelque chose à côté de vous.
	S'il y a plusieurs objets à côté de vous, vous pourrez choisir.

2- Utiliser un objet:
	Si vous avez un objet dans la main, appuyez sur N pour l'utiliser.
	Sinon, ouvrez votre sac à dos avec Y et double-cliquez sur l'objet.

3- Le sac à dos:
	Appuyez sur Y pour ouvrir votre sac à dos.
	Pour changer deux objets de place, cliquez consécutivement sur eux.
	Pour utiliser ou jeter un objet, double-cliquez sur celui-ci.
	Vous pouvez trouver de meilleurs sac à dos ou en acheter !
	Si vous mourrez, vous perdrez tout le contenu de votre sac.

4- Faim:
	Si vous ne mangez pas, vous allez mourir de faim.
	Vous pouvez trouver, acheter ou faire vous-même à manger.
	Vous pouvez chasser, pêcher ou cultiver !

5- Soif:
	Si vous ne buvez pas, vous allez mourir de soif.
	Vous pouvez trouver ou collecter vous-même à boire.
	Vous pouvez fabriquer ou trouver un collecteur d'eau !

6- Sommeil:
	Si vous ne dormez pas, vous allez vous affaiblir.
	Vous pouvez trouver ou acheter un lit.
	Vous ne pouvez poser de lits que dans les maisons ou dans les tentes.
	Vous pouvez aussi prendre une pillule de caféine pour vous réveiller.

7- Température:
	Si vous passez votre temps dans l'eau, votre température baissera.
	Si votre température de surface atteint 20°, vous mourrez de froid.
	En courrant ou en restant près du feu, vous vous réchaufferez.

8- Récupérateurs d'eau:
	Vous pouvez fabriquer ou voler un collecteur d'eau.
	Il se remplira quand il pleuvra, et vous pourrez remplir vos bouteilles.
	Pour le fabriquer, trouvez un ingénieur.

9- Coffres forts:
	Pour stocker vos objets les plus précieux, vous aurez besoin d'un coffre.
	Vous pouvez fabriquer un coffre chez un ingénieur.
	Utilisez un coffre pour le poser, vous devrez mettre un code à 4 chiffres.
	Appuyez sur C pour l'ouvrir, Y pour regarder dedans.

10- Véhicules:
	Pour aller plus vite, vous pourrez trouver des véhicules.
	Pour qu'un véhicule roule, il doit avoir un moteur et de l'essence.
	Vous pourrez trouver un moteur près des garages.
	Vous pourrez trouver de l'essence un peu partout ou dans les stations.
	Vous pourrez réparer votre véhicule avec une clé anglaise.

11- Coffre de véhicule:
	Un véhicule a un coffre dans lequel vous pourrez ranger des objets.
	Appuyez sur Y à l'intérieur du véhicule pour regarder.

12- Culture:
	Si vous trouvez des graines, vous pourrez planter dans la terre.
	Les plantes poussent le jour, et plus vite s'il pleut.
	Une plante donne des fruits.
	Vous pourrez couper avec une tronçonneuse si vous voulez du bois.

13- Magasins:
	Vous pourrez trouver des magasins dans les camps de survivants.
	Vous pourrez vendre des objets en double-cliquant dessus dans votre sac.
	Vous gagnerez de l'or que vous pourrez dépenser pour d'autres objets.
	Certains objets ne peuvent être trouvés nulle part ailleurs.

14- Ingénieurs:
	Vous pourrez trouver des ingénieurs dans les camps de survivants.
	Vous pourrez démonter des objets en double-cliquant dessus dans votre sac.
	Vous obtiendrez du bois, du fer ou autres.
	Vous pourrez aussi fabriquer de nouveaux objets uniques !

15- Armes:
	Pour survivre, chasser ou vous battre, vous aurez besoin d'armes.
	Vous pourrez en trouver/acheter un peu partout ou en fabriquer chez un ingénieur.
	Pour une arme à feu, vous aurez besoin de munitions.
	Si vous mourrez, vous perdrez toutes vos armes.

16- Cuisine:
	Trouvez, achetez ou fabriquez une casserole pour cuisiner.
	Utilisez-la près d'un feu.
	Sélectionnez une recette dont vous avez les ingrédients et cuisinez !
	La nourriture issue de la cuisine est plus nourrissante que tout !
*/
public ShowPlayerHelp(playerid, helpid, time)//Fonction pour afficher une partie de l'aide.
{
	if(dHelp[playerid] != 0 || CallRemoteFunction("IsPlayerOnCinematic", "i", playerid) || IsPlayerOnSpectate(playerid)) return 0;
	new string[1024];
	
	SetPlayerHelpState(playerid, helpid -1, true);

	switch(helpid)
	{
		case 1://Ramasser un objet
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "You can pick-up an item, a bed, a tent from the ground.~n~Press ~k~~GROUP_CONTROL_BWD~ to pick something up.~n~If there are more than just a single one, you'll have to choose.");
				case LANGUAGE_FR: strcpy(string, "Vous pouvez ramasser un objet, un lit, une tente sur le sol.~n~Appuyez sur ~k~~GROUP_CONTROL_BWD~ pour ramasser quelque chose à côté de vous.~n~S'il y a plusieurs objets à côté de vous, vous pourrez choisir.");
   				case LANGUAGE_ES: strcpy(string, "Puede tomar un objecto o otrás del suelo.~n~Imprensa ~k~~GROUP_CONTROL_BWD~ para tomar alguna cosa.~n~Si están varios, podrá eligir esto que quiere.");
   				case LANGUAGE_PG: strcpy(string, "Portugais");
				case LANGUAGE_IT: strcpy(string, "Italien");
				case LANGUAGE_DE: strcpy(string, "Sie können einen Gegenstand, ein Bett, ein Zelt vom Boden aufheben.~n~Drücken auf ~k~~GROUP_CONTROL_BWD~ um etwas aufzuheben.~n~Liegen mehrere Gegenstände neben Ihnen, dann müssen Sie wählen.");
			}
		}
	    case 2://Utiliser un objet
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "If you have any object in your hand, press ~k~~CONVERSATION_NO~ to use it.~n~Else, open your backpack with ~k~~CONVERSATION_YES~ and double-click on the object.");
				case LANGUAGE_FR: strcpy(string, "Si vous avez un objet dans la main, appuyez sur ~k~~CONVERSATION_NO~ pour l'utiliser.~n~Sinon, ouvrez votre sac à dos avec ~k~~CONVERSATION_YES~ et double-cliquez sur l'objet.");
				case LANGUAGE_ES: strcpy(string, "Si tiene un objecto en la mano, presiona la tecla k~~CONVERSATION_NO para lo utilizar.~n~De lo contrario, abra su mochila con  k~~CONVERSATION_YES~ y haga un doble clic sobre el objecto.");
				case LANGUAGE_PG: strcpy(string, "Portugais");
				case LANGUAGE_IT: strcpy(string, "Italien");
				case LANGUAGE_DE: strcpy(string, "Wenn Sie einen Gegenstand in ihrer hand halten, drücken Sie auf ~k~~CONVERSATION_NO~ um es zu benutzen.~n~Sonst, öffnen Sie ihren Rucksack durch drücken auf ~k~~CONVERSATION_YES~;~n~dann zwei mal darauf clicken.");
			}
		}
	    case 3://Le sac à dos
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "Hit ~k~~CONVERSATION_YES~ to open your backpack.~n~To switch two items, click on both.~n~To use or to drop an object, double-click on it.~n~You can find or buy better backpacks.~n~If you get whacked, you'll loose your backpack and its content.");
				case LANGUAGE_FR: strcpy(string, "Appuyez sur ~k~~CONVERSATION_YES~ pour ouvrir votre sac à dos.~n~Pour changer deux objets de place, cliquez consécutivement sur eux.~n~Pour utiliser ou jeter un objet, double-cliquez sur celui-ci.~n~Vous pouvez trouver de meilleurs sac à dos ou en acheter !~n~Si vous mourrez, vous perdrez tout le contenu de votre sac.");
			   	case LANGUAGE_ES: strcpy(string, "Imprensa ~k~~CONVERSATION_YES para abrir su mochila.~n~Pulsar consicutivamente sobre ambos objectos que quiere intercambiar.~n~Para utilizar o tirar un objecto, hacer un doble clic sobre éste.~n~¡ Puede encontrar mejores mochilas o comprarlos !~n~Si muere, perderá todos los objectos que estabán en su mochila.");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Drücken Sie auf ~k~~CONVERSATION_YES~ um ihren Rucksack zu öffnen.~n~Um die Stelle zweier Objekte zu tauschen, clicken Sie nacheinande auf beide.~n~Um ein Objekt zu benutzen oder wegzuwerfen, clicken Sie zwei mal darauf.~n~Sie können besserer Rucksäcke finden oder kaufen!~n~Sollten Sie sterben, verlieren Sie den ganzen Inhalt ihres Rucksacks.");
			}
		}
	    case 4://Faim
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "Eating is what keeps you from dying.~n~You can find, buy or make yourself something to eat.~n~You can hunt, go fishing or grow things to eat.~n~If you get a pan, you'll be able to enhance your food.");
				case LANGUAGE_FR: strcpy(string, "Si vous ne mangez pas, vous allez mourir de faim.~n~Vous pouvez trouver, acheter ou faire vous-même à manger.~n~Vous pouvez chasser, pêcher ou cultiver !~n~Si vous trouvez une casserole, vous pourrez améliorer de la nourriture.");
			   	case LANGUAGE_ES: strcpy(string, "Si no come, va a morir de hambre.~n~Puede encontrar, comprar o hacer comida.~n~¡ Puede cazar, pescar o cultivar !~n~Si encontrar una olla, podrá mejorar la comida.");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 5://Soif
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "If you don't drink, you'll die.~n~You can find or get yourself drinks.~n~You can build or find a water collector, being able to get free water.");
				case LANGUAGE_FR: strcpy(string, "Si vous ne buvez pas, vous allez mourir de soif.~n~Vous pouvez trouver ou collecter vous-même à boire.~n~Vous pouvez fabriquer ou trouver un collecteur d'eau !");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 6://Sommeil
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "If you don't sleep, you'll get weak.~n~You can buy or find a bed.~n~You will have to find a house or a tent to set your bed.~n~You can also find cofein pills to wake up without sleeping.");
				case LANGUAGE_FR: strcpy(string, "Si vous ne dormez pas, vous allez vous affaiblir.~n~Vous pouvez trouver ou acheter un lit.~n~Vous ne pouvez poser de lits que dans les maisons ou dans les tentes.~n~Vous pouvez aussi prendre une pillule de caféine pour vous réveiller.");
			   	case LANGUAGE_ES: strcpy(string, "Si no duerme, va a debilitarse.~n~Puede encontrar o comprar un cama.~n~Puede poner camas que en casas o tiendas.~n~Puede tomar nodoses para despetarse también.");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 7://Température
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "If you go into water, your body temperature will drop.~n~If your surface temperature reaches 20° or below, you'll die.~n~By running or staying close to fire, you'll heat up.");
				case LANGUAGE_FR: strcpy(string, "Si vous passez votre temps dans l'eau, votre température baissera.~n~Si votre température de surface atteint 20°, vous mourrez de froid.~n~En courrant ou en restant près du feu, vous vous réchaufferez.");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 8://Récupérateurs d'eau
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "You can build or steal a water collector.~n~It'll fill by itself when it's raining, making you able to refill empty bottles.~n~To build it, find a ingeneer.");
				case LANGUAGE_FR: strcpy(string, "Vous pouvez fabriquer ou voler un collecteur d'eau.~n~Il se remplira quand il pleuvra, et vous pourrez remplir vos bouteilles.~n~Pour le fabriquer, trouvez un ingénieur.");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 9://Coffres forts
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "To save your precious items, you'll maybe need a safe.~n~You can build a safe at an ingeneer.~n~Use a safe to set it on the ground, you'll have to set a 4 numbered code.~n~Hit ~k~~PED_DUCK~ to open it, ~k~~CONVERSATION_YES~ to look inside.");
				case LANGUAGE_FR: strcpy(string, "Pour stocker vos objets les plus précieux, vous aurez besoin d'un coffre.~n~Vous pouvez fabriquer un coffre chez un ingénieur.~n~Utilisez un coffre pour le poser, vous devrez mettre un code à 4 chiffres.~n~Appuyez sur ~k~~PED_DUCK~ pour l'ouvrir, ~k~~CONVERSATION_YES~ pour regarder dedans.");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 10://Véhicules
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "You'll be able to find vehicles to go faster.~n~For a vehicle to move, it has to have an engine and gas.~n~You may find an engine near garages or others.~n~You'll be able to find gas pretty much anywhere or in gas stations.~n~If you find a wrench, you'll be able to repair your vehicle everytime.");
				case LANGUAGE_FR: strcpy(string, "Pour aller plus vite, vous pourrez trouver des véhicules.~n~Pour qu'un véhicule roule, il doit avoir un moteur et de l'essence.~n~Vous pourrez trouver un moteur près des garages.~n~Vous pourrez trouver de l'essence un peu partout ou dans les stations.~n~Vous pourrez réparer votre véhicule avec une clé anglaise.");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 11://Coffre du véhicule
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "A vehicle has a trunk in which you can store items.~n~Hit ~k~~CONVERSATION_YES~ inside of a vehicle to look what's inside.");
				case LANGUAGE_FR: strcpy(string, "Un véhicule a un coffre dans lequel vous pourrez ranger des objets.~n~Appuyez sur ~k~~CONVERSATION_YES~ à l'intérieur du véhicule pour regarder.");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 12://Culture
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "If you find seeds, you'll be able to plant them in the ground.~n~Plants only grow on daylight, and faster if it's raining.~n~A plant gives fruits and others.~n~You can saw your plant if you need wood.");
				case LANGUAGE_FR: strcpy(string, "Si vous trouvez des graines, vous pourrez planter dans la terre.~n~Les plantes poussent le jour, et plus vite s'il pleut.~n~Une plante donne des fruits et autres.~n~Vous pourrez la couper avec une tronçonneuse si vous voulez du bois.");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 13://Magasin
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "You can find shops in survivor camps.~n~You can sell items by double-clicking on them in your backpack.~n~You'll get gold, which you'll be able to spend for other items at shops.~n~Some objects cannot be found in any other place than in shops.");
				case LANGUAGE_FR: strcpy(string, "Vous pourrez trouver des magasins dans les camps de survivants.~n~Vous pourrez vendre des objets en double-cliquant dessus dans votre sac.~n~Vous gagnerez de l'or que vous pourrez dépenser pour d'autres objets.~n~Certains objets ne peuvent être trouvés nulle part ailleurs.");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 14://Ingénieur
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "You can find engineer in survivor camps.~n~You may disassemble objects by double-clicking on them in your backpack.~n~You'll get wood, iron or others.~n~You can also craft other items!");
				case LANGUAGE_FR: strcpy(string, "Vous pourrez trouver des ingénieurs dans les camps de survivants.~n~Vous pourrez démonter des objets en double-cliquant dessus dans votre sac.~n~Vous obtiendrez du bois, du fer ou autres.~n~Vous pourrez aussi fabriquer de nouveaux objets uniques !");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 15://Armes
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "To survive, hunt or fight, you'll need a weapon.~n~You can find/buy them pretty much everywhere or craft them at an engineer.~n~For a firearm, you'll need ammo which you also can find/buy.~n~If you die, you'll drop all your guns.");
				case LANGUAGE_FR: strcpy(string, "Pour survivre, chasser ou vous battre, vous aurez besoin d'armes.~n~Vous pourrez en trouver/acheter un peu partout ou en fabriquer chez un ingénieur.~n~Pour une arme à feu, vous aurez besoin de munitions.~n~Si vous mourrez, vous perdrez toutes vos armes.");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 16://Cuisine
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "Find, buy or craft a pan to become a cook.~n~Use it near a fire.~n~Select a recipe you have all the ingredients for and cook!~n~Food that has been cooked is better than any other!");
				case LANGUAGE_FR: strcpy(string, "Trouvez, achetez ou fabriquez une casserole pour cuisiner.~n~Utilisez-la près d'un feu.~n~Sélectionnez une recette dont vous avez les ingrédients et cuisinez !~n~La nourriture issue de la cuisine est plus nourrissante que tout !");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	    case 17://Levels
	    {
			switch(GetPlayerLanguage(playerid))//On l'affiche dans la bonne langue...
			{
				case LANGUAGE_EN: strcpy(string, "You earn experience by doing things.~n~Get killed and you'll lose some.~n~When your exp bars get full, you'll level up.~n~Each level gives you a skill point.~n~You can buy new skills:~n~- Open your backpack and click on the '~r~+~w~'.~n~- Select a skill and power it up.");
				case LANGUAGE_FR: strcpy(string, "Vous gagnez de l'expérience en faisant des trucs.~n~Si vous mourrez, vous en perdrez.~n~Quand votre jauge sera pleine, vous gagnez un niveau.~n~Chaque niveau vous donne un point de compétence.~n~Vous pouvez acheter de nouvelles compétences:~n~- Ouvrez votre sac et cliquez sur '~r~+~w~'.~n~Sélectionnez une compétence et augmentez !");
			   	case LANGUAGE_ES: strcpy(string, "Espagnol");
			   	case LANGUAGE_PG: strcpy(string, "Portugais");
			   	case LANGUAGE_IT: strcpy(string, "Italien");
			   	case LANGUAGE_DE: strcpy(string, "Allemand");
			}
		}
	}
	ConvertGameText(string);
	if(!dHelp[playerid])//Si y'a pas déjà un texte, on crée le textdraw
	{
		tHelpText[playerid] = CreatePlayerTextDraw(playerid, 140.000000, 170.000000, string);
		PlayerTextDrawAlignment(playerid, tHelpText[playerid], 1);//On change les paramètres du textdraw
		PlayerTextDrawBackgroundColor(playerid, tHelpText[playerid], 255);
		PlayerTextDrawFont(playerid, tHelpText[playerid], 1);
		PlayerTextDrawLetterSize(playerid, tHelpText[playerid], 0.270000, 1.000000);
		PlayerTextDrawColor(playerid, tHelpText[playerid], -1);
		PlayerTextDrawSetOutline(playerid, tHelpText[playerid], 0);
		PlayerTextDrawSetProportional(playerid, tHelpText[playerid], 1);
		PlayerTextDrawSetShadow(playerid, tHelpText[playerid], 1);
		PlayerTextDrawSetSelectable(playerid, tHelpText[playerid], false);
	}
	else//Si y'a déjà un texte d'affiché...
	{
	 	KillTimer(dHelpTimer[playerid]);//On casse le timer-
		PlayerTextDrawSetString(playerid, tHelpText[playerid], string);
	}
	//---
	CloseTextDraws(playerid);
 	dHelp[playerid] = helpid;//On dit que y'a un texte
	TextDrawShowForPlayer(playerid, tFondDialog[0]);
	TextDrawShowForPlayer(playerid, tFondDialog[1]);
	TextDrawShowForPlayer(playerid, tFondDialog[2 + GetPlayerLanguage(playerid)]);
	PlayerTextDrawShow(playerid, tHelpText[playerid]);//On affiche le textdraw
	if(time != -1) dHelpTimer[playerid] = SetTimerEx("HidePlayerHelp", time, false, "i", playerid);//Et on met un timer
	return 1;
}

public HidePlayerHelp(playerid)//Fonction pour cacher le texte
{
	for(new i = 0; i < 8; i ++) TextDrawHideForPlayer(playerid, tFondDialog[i]);
	PlayerTextDrawHide(playerid, tHelpText[playerid]);
	PlayerTextDrawDestroy(playerid, tHelpText[playerid]);
	tHelpText[playerid] = PlayerText:INVALID_TEXT_DRAW;
	KillTimer(dHelpTimer[playerid]);
	dHelpTimer[playerid] = -1;
	dHelp[playerid] = 0;
}

stock SetPlayerBoard(playerid, boardid) 
{
	if(IsValidPlayer(playerid)) 
	{
		pBoard[playerid] = door;
	}
}

stock GetPlayerBoard(playerid) 
{
	return IsValidPlayer(playerid) ? pBoard[playerid] : -1;
}

stock SetPlayerRack(playerid, rackid) 
{
	if(IsValidPlayer(playerid)) 
	{
		pRack[playerid] = rackid;
	}
}

stock GetPlayerRack(playerid) 
{
	return IsValidPlayer(playerid) ? pRack[playerid] : -1;
}

stock SetPlayerBrasero(playerid, braseroid) 
{
	if(IsValidPlayer(playerid)) 
	{
		pBrasero[playerid] = braseroid;
	}
}

stock GetPlayerBrasero(playerid) 
{
	return IsValidPlayer(playerid) ? pBrasero[playerid] : -1;
}

stock SetPlayerShredder(playerid, shredderid) 
{
	if(IsValidPlayer(playerid)) 
	{
		pShredder[playerid] = shredderid;
	}
}

stock GetPlayerShredder(playerid) 
{
	return IsValidPlayer(playerid) ? pShredder[playerid] : -1;
}

stock SetPlayerSafe(playerid, safeid) 
{
	if(IsValidPlayer(playerid) && MAX_SAFES > safeid >= 0) 
	{
		pCreateSafe[playerid] = safeid;
	}
}

stock GetPlayerSafe(playerid) 
{
	return IsValidPlayer(playerid) ? pCreateSafe[playerid] : -1;
}