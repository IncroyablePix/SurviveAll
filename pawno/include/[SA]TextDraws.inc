#if defined TEXTDRAWS_INCLUDED

	#endinput

#endif

#define TEXTDRAWS_INCLUDED

//---INCLUDES---//
#include <a_samp>
#include <[SA]Functions.inc>
#include <[SA]Defines.inc>
#include <[SA]Items.inc>
#include <[SA]Vehicles.inc>
#include <[SA]Players.inc>

//---DEFINES---//
#define CRAFT_ENGINEER                      (0)
#define CRAFT_KITCHEN                       (1)
#define CRAFT_HOUSE                         (2)

//---FORWARDS---//
forward HidePlayerHUD(playerid, bool:hide);
forward bool:IsHUDHiddenForPlayer(playerid);

//---ENUMS---//

//---VARIABLES---//
new bool:bHideHUD[MAX_PLAYERS];
new bool:bRedScreen[MAX_PLAYERS];
new dNewCo[MAX_PLAYERS];

new PlayerText:tHDVCats[MAX_PLAYERS][8];
new PlayerText:tHDVItem[MAX_PLAYERS][2];

new PlayerText:tInfoText[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerText:tGroupText[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerText:tAtelierTitre[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerText:tAtelierChoice[MAX_PLAYERS][18];
new PlayerText:tAtelierItem[MAX_PLAYERS][10];
new PlayerText:tInventObjet[MAX_PLAYERS][2];
new PlayerText:tSafe[MAX_PLAYERS][12][2];
new PlayerText:tPlayerInfos[MAX_PLAYERS][8];
new PlayerText:skinSelect[MAX_PLAYERS];
new PlayerText:tPlayerHUD[MAX_PLAYERS][9];
new PlayerText:tSlots[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerText:tInventory[MAX_PLAYERS][36][2];
new PlayerText:tPlayerVehHUD[MAX_PLAYERS][5];
new PlayerText:tVehicleTrunk[MAX_PLAYERS][6][2];
new PlayerText:tLevel[MAX_PLAYERS][3];
new PlayerText:tHelpText[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerText:tBossHealth[MAX_PLAYERS][2];
//

new Text:tHDVButtons[3] = {Text:INVALID_TEXT_DRAW, ...};

new Text:tStop = Text:INVALID_TEXT_DRAW;
new Text:tKnifeAim;
new Text:tHelp;
new Text:tCompetence;
new Text:tGroup;
new Text:tOptions;
new Text:tBoutonFabriquer[6];
new Text:tBoutonCuisiner[6];
new Text:tBoutonBatir[6];
new Text:tDarkScreen;
new Text:tSkinSelect[7];
new Text:tRedScreen[10];
new Text:tHUDFont[9];
new Text:tBrokenLeg[6];
new Text:tHemoragie[6];
//new Text:tConnection;
new Text:tFondHUD[5];
new Text:tFondAtelier[3];
new Text:tBoutonsAtelier[4];
new Text:tBarettesLevel[10];
new Text:tFondLevel;
new Text:tBossBar;
new Text:tAdmin;
new Text:tFondDialog[8];
//
new Text:tRules[6][2];
new Text:tBloodSplat;

//---FONCTIONS---//
CreateHDV()
{
	tHDVButtons[0] = TextDrawCreate(270.000000, 265.000000, "<");
	TextDrawBackgroundColor(tHDVButtons[0], 255);
	TextDrawFont(tHDVButtons[0], 2);
	TextDrawLetterSize(tHDVButtons[0], 0.800000, 3.000000);
	TextDrawColor(tHDVButtons[0], -1);
	TextDrawSetOutline(tHDVButtons[0], 1);
	TextDrawSetProportional(tHDVButtons[0], 1);
	TextDrawTextSize(tHDVButtons[0], 295.000000, 20.000000);
	TextDrawSetSelectable(tHDVButtons[0], true);
	//---
	tHDVButtons[1] = TextDrawCreate(375.000000, 265.000000, ">");
	TextDrawBackgroundColor(tHDVButtons[1], 255);
	TextDrawFont(tHDVButtons[1], 2);
	TextDrawLetterSize(tHDVButtons[1], 0.800000, 3.000000);
	TextDrawColor(tHDVButtons[1], -1);
	TextDrawSetOutline(tHDVButtons[1], 1);
	TextDrawSetProportional(tHDVButtons[1], 1);
	TextDrawTextSize(tHDVButtons[1], 400.000000, 20.000000);
	TextDrawSetSelectable(tHDVButtons[1], true);
	//---
	tHDVButtons[2] = TextDrawCreate(325.000000, 276.000000, "LD_CHAT:thumbup");
	TextDrawBackgroundColor(tHDVButtons[2], 255);
	TextDrawFont(tHDVButtons[2], 4);
	TextDrawLetterSize(tHDVButtons[2], -0.399999, 6.000000);
	TextDrawColor(tHDVButtons[2], 8388761);
	TextDrawSetOutline(tHDVButtons[2], 1);
	TextDrawSetProportional(tHDVButtons[2], 1);
	TextDrawUseBox(tHDVButtons[2], 1);
	TextDrawBoxColor(tHDVButtons[2], 0);
	TextDrawTextSize(tHDVButtons[2], 20.000000, 20.000000);
	TextDrawSetSelectable(tHDVButtons[2], true);

}

CreatePlayerHDV(playerid)
{
	tHDVCats[playerid][0] = CreatePlayerTextDraw(playerid, 180.000000, 150.000000, "Outils");
	PlayerTextDrawBackgroundColor(playerid, tHDVCats[playerid][0], 255);
	PlayerTextDrawFont(playerid, tHDVCats[playerid][0], 2);
	PlayerTextDrawLetterSize(playerid, tHDVCats[playerid][0], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, tHDVCats[playerid][0], -335544167);
	PlayerTextDrawSetOutline(playerid, tHDVCats[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, tHDVCats[playerid][0], 1);
	PlayerTextDrawUseBox(playerid, tHDVCats[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, tHDVCats[playerid][0], 153);
	PlayerTextDrawTextSize(playerid, tHDVCats[playerid][0], 270.000000, 10.000000);
	PlayerTextDrawSetSelectable(playerid, tHDVCats[playerid][0], true);
	//---
	tHDVCats[playerid][1] = CreatePlayerTextDraw(playerid, 180.000000, 163.000000, "Medecine");
	PlayerTextDrawBackgroundColor(playerid, tHDVCats[playerid][1], 255);
	PlayerTextDrawFont(playerid, tHDVCats[playerid][1], 2);
	PlayerTextDrawLetterSize(playerid, tHDVCats[playerid][1], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, tHDVCats[playerid][1], -103);
	PlayerTextDrawSetOutline(playerid, tHDVCats[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, tHDVCats[playerid][1], 1);
	PlayerTextDrawUseBox(playerid, tHDVCats[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid, tHDVCats[playerid][1], 153);
	PlayerTextDrawTextSize(playerid, tHDVCats[playerid][1], 270.000000, 10.000000);
	PlayerTextDrawSetSelectable(playerid, tHDVCats[playerid][1], true);
	//---
	tHDVCats[playerid][2] = CreatePlayerTextDraw(playerid, 180.000000, 176.000000, "Armes");
	PlayerTextDrawBackgroundColor(playerid, tHDVCats[playerid][2], 255);
	PlayerTextDrawFont(playerid, tHDVCats[playerid][2], 2);
	PlayerTextDrawLetterSize(playerid, tHDVCats[playerid][2], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, tHDVCats[playerid][2], -103);
	PlayerTextDrawSetOutline(playerid, tHDVCats[playerid][2], 1);
	PlayerTextDrawSetProportional(playerid, tHDVCats[playerid][2], 1);
	PlayerTextDrawUseBox(playerid, tHDVCats[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid, tHDVCats[playerid][2], 153);
	PlayerTextDrawTextSize(playerid, tHDVCats[playerid][2], 270.000000, 10.000000);
	PlayerTextDrawSetSelectable(playerid, tHDVCats[playerid][2], true);
	//---
	tHDVCats[playerid][3] = CreatePlayerTextDraw(playerid, 180.000000, 189.000000, "Autres");
	PlayerTextDrawBackgroundColor(playerid, tHDVCats[playerid][3], 255);
	PlayerTextDrawFont(playerid, tHDVCats[playerid][3], 2);
	PlayerTextDrawLetterSize(playerid, tHDVCats[playerid][3], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, tHDVCats[playerid][3], -103);
	PlayerTextDrawSetOutline(playerid, tHDVCats[playerid][3], 1);
	PlayerTextDrawSetProportional(playerid, tHDVCats[playerid][3], 1);
	PlayerTextDrawUseBox(playerid, tHDVCats[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid, tHDVCats[playerid][3], 153);
	PlayerTextDrawTextSize(playerid, tHDVCats[playerid][3], 270.000000, 10.000000);
	PlayerTextDrawSetSelectable(playerid, tHDVCats[playerid][3], true);
	//---
	tHDVCats[playerid][4] = CreatePlayerTextDraw(playerid, 180.000000, 202.000000, "Vehicules");
	PlayerTextDrawBackgroundColor(playerid, tHDVCats[playerid][4], 255);
	PlayerTextDrawFont(playerid, tHDVCats[playerid][4], 2);
	PlayerTextDrawLetterSize(playerid, tHDVCats[playerid][4], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, tHDVCats[playerid][4], -103);
	PlayerTextDrawSetOutline(playerid, tHDVCats[playerid][4], 1);
	PlayerTextDrawSetProportional(playerid, tHDVCats[playerid][4], 1);
	PlayerTextDrawUseBox(playerid, tHDVCats[playerid][4], 1);
	PlayerTextDrawBoxColor(playerid, tHDVCats[playerid][4], 153);
	PlayerTextDrawTextSize(playerid, tHDVCats[playerid][4], 270.000000, 10.000000);
	PlayerTextDrawSetSelectable(playerid, tHDVCats[playerid][4], true);
	//---
	tHDVCats[playerid][5] = CreatePlayerTextDraw(playerid, 180.000000, 215.000000, "Vetements");
	PlayerTextDrawBackgroundColor(playerid, tHDVCats[playerid][5], 255);
	PlayerTextDrawFont(playerid, tHDVCats[playerid][5], 2);
	PlayerTextDrawLetterSize(playerid, tHDVCats[playerid][5], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, tHDVCats[playerid][5], -103);
	PlayerTextDrawSetOutline(playerid, tHDVCats[playerid][5], 1);
	PlayerTextDrawSetProportional(playerid, tHDVCats[playerid][5], 1);
	PlayerTextDrawUseBox(playerid, tHDVCats[playerid][5], 1);
	PlayerTextDrawBoxColor(playerid, tHDVCats[playerid][5], 153);
	PlayerTextDrawTextSize(playerid, tHDVCats[playerid][5], 270.000000, 10.000000);
	PlayerTextDrawSetSelectable(playerid, tHDVCats[playerid][5], true);
	//---
	tHDVCats[playerid][6] = CreatePlayerTextDraw(playerid, 180.000000, 228.000000, "Nourriture");
	PlayerTextDrawBackgroundColor(playerid, tHDVCats[playerid][6], 255);
	PlayerTextDrawFont(playerid, tHDVCats[playerid][6], 2);
	PlayerTextDrawLetterSize(playerid, tHDVCats[playerid][6], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, tHDVCats[playerid][6], -103);
	PlayerTextDrawSetOutline(playerid, tHDVCats[playerid][6], 1);
	PlayerTextDrawSetProportional(playerid, tHDVCats[playerid][6], 1);
	PlayerTextDrawUseBox(playerid, tHDVCats[playerid][6], 1);
	PlayerTextDrawBoxColor(playerid, tHDVCats[playerid][6], 153);
	PlayerTextDrawTextSize(playerid, tHDVCats[playerid][6], 270.000000, 10.000000);
	PlayerTextDrawSetSelectable(playerid, tHDVCats[playerid][6], true);
	//---
	tHDVCats[playerid][7] = CreatePlayerTextDraw(playerid, 180.000000, 241.000000, "Ressources");
	PlayerTextDrawBackgroundColor(playerid, tHDVCats[playerid][7], 255);
	PlayerTextDrawFont(playerid, tHDVCats[playerid][7], 2);
	PlayerTextDrawLetterSize(playerid, tHDVCats[playerid][7], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, tHDVCats[playerid][7], -103);
	PlayerTextDrawSetOutline(playerid, tHDVCats[playerid][7], 1);
	PlayerTextDrawSetProportional(playerid, tHDVCats[playerid][7], 1);
	PlayerTextDrawUseBox(playerid, tHDVCats[playerid][7], 1);
	PlayerTextDrawBoxColor(playerid, tHDVCats[playerid][7], 153);
	PlayerTextDrawTextSize(playerid, tHDVCats[playerid][7], 270.000000, 10.000000);
	PlayerTextDrawSetSelectable(playerid, tHDVCats[playerid][7], true);
	//---
	tHDVItem[playerid][0] = CreatePlayerTextDraw(playerid, 272.000000, 148.000000, "Objet");
	PlayerTextDrawBackgroundColor(playerid, tHDVItem[playerid][0], 153);
	PlayerTextDrawFont(playerid, tHDVItem[playerid][0], 5);
	PlayerTextDrawLetterSize(playerid, tHDVItem[playerid][0], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, tHDVItem[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, tHDVItem[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, tHDVItem[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, tHDVItem[playerid][0], 1);
	PlayerTextDrawUseBox(playerid, tHDVItem[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, tHDVItem[playerid][0], 153);
	PlayerTextDrawTextSize(playerid, tHDVItem[playerid][0], 120.000000, 120.000000);
	PlayerTextDrawSetPreviewModel(playerid, tHDVItem[playerid][0], 18888);
	PlayerTextDrawSetPreviewRot(playerid, tHDVItem[playerid][0], 0.000000, 0.000000, 0.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, tHDVItem[playerid][0], false);
	//---
	tHDVItem[playerid][1] = CreatePlayerTextDraw(playerid, 280.000000, 150.000000, "Prix");
	PlayerTextDrawBackgroundColor(playerid, tHDVItem[playerid][1], 255);
	PlayerTextDrawFont(playerid, tHDVItem[playerid][1], 2);
	PlayerTextDrawLetterSize(playerid, tHDVItem[playerid][1], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, tHDVItem[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, tHDVItem[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, tHDVItem[playerid][1], 1);
	PlayerTextDrawSetSelectable(playerid, tHDVItem[playerid][1], false);
}

stock ShowPlayerHDV(playerid) 
{
	TextDrawShowForPlayer(playerid, tHDVButtons[0]);
	TextDrawShowForPlayer(playerid, tHDVButtons[1]);
	TextDrawShowForPlayer(playerid, tHDVButtons[2]);
}

stock HidePlayerAuctionHouse(playerid)
{
    ClosePlayerAuctionHouse(playerid);
	TextDrawHideForPlayer(playerid, tHDVButtons[0]);
	TextDrawHideForPlayer(playerid, tHDVButtons[1]);
	TextDrawHideForPlayer(playerid, tHDVButtons[2]);
	PlayerTextDrawHide(playerid, tHDVItem[playerid][0]);
	PlayerTextDrawHide(playerid, tHDVItem[playerid][1]);
	PlayerTextDrawDestroy(playerid, tHDVItem[playerid][0]);
	PlayerTextDrawDestroy(playerid, tHDVItem[playerid][1]);
	tHDVItem[playerid][0] = PlayerText:INVALID_TEXT_DRAW;
	tHDVItem[playerid][1] = PlayerText:INVALID_TEXT_DRAW;
	for(new i = 0; i < 8; i ++)
	{
		PlayerTextDrawHide(playerid, tHDVCats[playerid][i]);
		PlayerTextDrawDestroy(playerid, tHDVCats[playerid][i]);
		tHDVCats[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
	}
	CancelSelectTextDraw(playerid);
}

stock UpdateAuctionCategory(categoryid, bool:exploring) 
{
	PlayerTextDrawColor(playerid, tHDVCats[playerid][categoryid], exploring ? 0xFFFFFF99 : 0xCC000099);
	PlayerTextDrawShow(playerid, tHDVCats[playerid][categoryid]);
}
		
stock UpdateAuctionCurrentItem(playerid, objectid, Float:rx, Float:ry, Float:rz, Float:zoom, const label[]) 
{
	PlayerTextDrawSetPreviewModel(playerid, tHDVItem[playerid][0], objectid);
	PlayerTextDrawSetPreviewRot(playerid, tHDVItem[playerid][0], rx, ry, rz, zoom);
	PlayerTextDrawSetString(playerid, tHDVItem[playerid][1], label);
	//---
	PlayerTextDrawShow(playerid, tHDVItem[playerid][0]);
	PlayerTextDrawShow(playerid, tHDVItem[playerid][1]);
}

stock PlayerText:CreatePlayerGroupTextDraw(playerid) 
{
    new PlayerText:textDraw = CreatePlayerTextDraw(playerid,0.000000, 140.000000, " ");
	PlayerTextDrawBackgroundColor(playerid, textDraw, 255);
	PlayerTextDrawFont(playerid, textDraw, 2);
	PlayerTextDrawLetterSize(playerid, textDraw, 0.149999, 1.000000);
	PlayerTextDrawColor(playerid, textDraw, BLEU);
	PlayerTextDrawSetOutline(playerid, textDraw, 0);
	PlayerTextDrawSetProportional(playerid, textDraw, 1);
	PlayerTextDrawSetShadow(playerid, textDraw, 1);
	PlayerTextDrawUseBox(playerid, textDraw, 1);
	PlayerTextDrawBoxColor(playerid, textDraw, 119);
	PlayerTextDrawTextSize(playerid, textDraw, 74.000000, 40.000000);
	PlayerTextDrawSetSelectable(playerid, textDraw, true);

    return textDraw;
}

stock bool:IsHUDHiddenForPlayer(playerid)
{
    return IsValidPlayer(playerid) ? bHideHUD[playerid] : false;
}

public HidePlayerHUD(playerid, bool:hide)
{
	if(hide != bHideHUD[playerid])
	{
	    if(hide == true)
	    {
	        for(new i = 0; i < 9; i ++) TextDrawHideForPlayer(playerid, tBarettesLevel[i]);
	        TextDrawHideForPlayer(playerid, tFondLevel);
	        #if defined RADAR_HIDDEN
	        for(new i = 0; i < 9; i ++) TextDrawHideForPlayer(playerid, tHUDFont[i]);
			#else
	        for(new i = 0; i < 8; i ++) TextDrawHideForPlayer(playerid, tHUDFont[i]);
			#endif
			if(GetPlayerAdminLevel(playerid) > PLAYER) TextDrawHideForPlayer(playerid, tAdmin);
	        //---
	        if(GetPlayerGroup(playerid) != -1) PlayerTextDrawHide(playerid, tGroupText[playerid]);
	        for(new i = 0; i < 2; i ++) PlayerTextDrawHide(playerid, tInventObjet[playerid][i]);
	        for(new i = 0; i < 8; i ++) PlayerTextDrawHide(playerid, tPlayerInfos[playerid][i]);
	        for(new i = 0; i < 3; i ++) PlayerTextDrawHide(playerid, tLevel[playerid][i]);
	        if(tBossHealth[playerid][0] != PlayerText:INVALID_TEXT_DRAW) for(new i = 0; i < 2; i ++) PlayerTextDrawHide(playerid, tBossHealth[playerid][i]);
	    }
	    else
	    {
	        for(new i = 0; i < 9; i ++) TextDrawShowForPlayer(playerid, tBarettesLevel[i]);
	        TextDrawShowForPlayer(playerid, tFondLevel);
	        #if defined RADAR_HIDDEN
	        for(new i = 0; i < 9; i ++) TextDrawShowForPlayer(playerid, tHUDFont[i]);
			#else
	        for(new i = 0; i < 8; i ++) TextDrawShowForPlayer(playerid, tHUDFont[i]);
			#endif
			if(GetPlayerAdminLevel(playerid) > PLAYER) TextDrawShowForPlayer(playerid, tAdmin);
	        //---
	        if(GetPlayerGroup(playerid) != -1) PlayerTextDrawShow(playerid, tGroupText[playerid]);
	        for(new i = 0; i < 2; i ++) PlayerTextDrawShow(playerid, tInventObjet[playerid][i]);
	        for(new i = 0; i < 7; i ++) PlayerTextDrawShow(playerid, tPlayerInfos[playerid][i]);
	        if(GetVehicleID(GetPlayerVehicleID(playerid)) != -1) PlayerTextDrawShow(playerid, tPlayerInfos[playerid][7]);
	        for(new i = 0; i < 3; i ++) PlayerTextDrawShow(playerid, tLevel[playerid][i]);
	        if(tBossHealth[playerid][0] != PlayerText:INVALID_TEXT_DRAW) for(new i = 0; i < 2; i ++) PlayerTextDrawShow(playerid, tBossHealth[playerid][i]);
	    }
	    bHideHUD[playerid] = hide;
  	}
}

public ShowStopMission(playerid, bool:show)
{
	if(show) TextDrawShowForPlayer(playerid, tStop);
	else TextDrawHideForPlayer(playerid, tStop);
}

CreateBloodScreen()
{
	for(new i = 0; i < 10; i ++)
	{
		tRedScreen[i] = TextDrawCreate(0.000000, 0.000000, "~n~");
		TextDrawBackgroundColor(tRedScreen[i], 255);
		TextDrawFont(tRedScreen[i], 1);
		TextDrawLetterSize(tRedScreen[i], 0.500000, 49.600006);
		TextDrawColor(tRedScreen[i], -1);
		TextDrawSetOutline(tRedScreen[i], 0);
		TextDrawSetProportional(tRedScreen[i], 1);
		TextDrawSetShadow(tRedScreen[i], 1);
		TextDrawUseBox(tRedScreen[i], 1);
		TextDrawTextSize(tRedScreen[i], 640.000000, 100.000000);
		TextDrawSetSelectable(tRedScreen[i], 0);
	}
	TextDrawBoxColor(tRedScreen[0], 0xCC0000AA);
	TextDrawBoxColor(tRedScreen[1], 0xCC000099);
	TextDrawBoxColor(tRedScreen[2], 0xCC000088);
	TextDrawBoxColor(tRedScreen[3], 0xCC000077);
	TextDrawBoxColor(tRedScreen[4], 0xCC000066);
	TextDrawBoxColor(tRedScreen[5], 0xCC000055);
	TextDrawBoxColor(tRedScreen[6], 0xCC000044);
	TextDrawBoxColor(tRedScreen[7], 0xCC000033);
	TextDrawBoxColor(tRedScreen[8], 0xCC000022);
	TextDrawBoxColor(tRedScreen[9], 0xCC000011);
}

public ShowRedScreen(playerid)
{
	if(!bRedScreen[playerid])
	{
	    bRedScreen[playerid] = true;
	    ShowPlayerBloodScreen(playerid, -4);
	}
}

public ShowPlayerRules(playerid)
{
	dNewCo[playerid] = 10;
	//---
	TextDrawShowForPlayer(playerid, tBloodSplat);
	TextDrawShowForPlayer(playerid, tRules[GetPlayerLanguage(playerid)][0]);
	TextDrawShowForPlayer(playerid, tRules[GetPlayerLanguage(playerid)][1]);
}

HidePlayerRules(playerid)
{
	if(dNewCo[playerid] != 0) dNewCo[playerid] = 0;
	//---
	TextDrawHideForPlayer(playerid, tBloodSplat);
	TextDrawHideForPlayer(playerid, tRules[GetPlayerLanguage(playerid)][0]);
	TextDrawHideForPlayer(playerid, tRules[GetPlayerLanguage(playerid)][1]);
}

public ShowPlayerBloodScreen(playerid, step)
{
	switch(step)
	{
		case -4: TextDrawShowForPlayer(playerid, tRedScreen[4]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, -3);
		case -3: TextDrawHideForPlayer(playerid, tRedScreen[4]), TextDrawShowForPlayer(playerid, tRedScreen[3]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, -2);
		case -2: TextDrawHideForPlayer(playerid, tRedScreen[3]), TextDrawShowForPlayer(playerid, tRedScreen[2]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, -1);
		case -1: TextDrawHideForPlayer(playerid, tRedScreen[2]), TextDrawShowForPlayer(playerid, tRedScreen[1]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 0);
		case 0: TextDrawHideForPlayer(playerid, tRedScreen[1]), TextDrawShowForPlayer(playerid, tRedScreen[0]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 1);
		case 1: TextDrawHideForPlayer(playerid, tRedScreen[0]), TextDrawShowForPlayer(playerid, tRedScreen[1]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 2);
		case 2: TextDrawHideForPlayer(playerid, tRedScreen[1]), TextDrawShowForPlayer(playerid, tRedScreen[2]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 3);
		case 3: TextDrawHideForPlayer(playerid, tRedScreen[2]), TextDrawShowForPlayer(playerid, tRedScreen[3]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 4);
		case 4: TextDrawHideForPlayer(playerid, tRedScreen[3]), TextDrawShowForPlayer(playerid, tRedScreen[4]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 5);
		case 5: TextDrawHideForPlayer(playerid, tRedScreen[4]), TextDrawShowForPlayer(playerid, tRedScreen[5]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 6);
		case 6: TextDrawHideForPlayer(playerid, tRedScreen[5]), TextDrawShowForPlayer(playerid, tRedScreen[6]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 7);
		case 7: TextDrawHideForPlayer(playerid, tRedScreen[6]), TextDrawShowForPlayer(playerid, tRedScreen[7]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 8);
		case 8: TextDrawHideForPlayer(playerid, tRedScreen[7]), TextDrawShowForPlayer(playerid, tRedScreen[8]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 9);
		case 9: TextDrawHideForPlayer(playerid, tRedScreen[8]), TextDrawShowForPlayer(playerid, tRedScreen[9]), SetTimerEx("ShowPlayerBloodScreen", 100, false, "ii", playerid, 10);
		case 10: TextDrawHideForPlayer(playerid, tRedScreen[9]), bRedScreen[playerid] = false;
	}
}

CreateHUDForAll()
{
	//---
	tStop = TextDrawCreate(471.500000, 120.000000, "Stop mission");
	TextDrawBackgroundColor(tStop, 85);
	TextDrawFont(tStop, 2);
	TextDrawLetterSize(tStop, 0.300000, 0.930000);
	TextDrawColor(tStop, -1);
	TextDrawSetOutline(tStop, 1);
	TextDrawSetProportional(tStop, 1);
	TextDrawUseBox(tStop, false);
	TextDrawBoxColor(tStop, 170);
	TextDrawTextSize(tStop, 536.000000, 10.000000);
	TextDrawSetSelectable(tStop, true);
	//---
    tKnifeAim = TextDrawCreate(305.000000, 160.000000, "(+)");//Cible lancer de couteau
	TextDrawBackgroundColor(tKnifeAim, 255);
	TextDrawFont(tKnifeAim, 2);
	TextDrawLetterSize(tKnifeAim, 0.500000, 1.000000);
	TextDrawColor(tKnifeAim, -1);
	TextDrawSetOutline(tKnifeAim, 1);
	TextDrawSetProportional(tKnifeAim, 1);
	TextDrawSetSelectable(tKnifeAim, 0);
	//---
    tFondDialog[0] = TextDrawCreate(160.000000, 40.000000, " ");//Mare de sang
	TextDrawBackgroundColor(tFondDialog[0], 0);
	TextDrawFont(tFondDialog[0], 5);
	TextDrawLetterSize(tFondDialog[0], 0.500000, 1.000000);
	TextDrawColor(tFondDialog[0], -1);
	TextDrawSetOutline(tFondDialog[0], 0);
	TextDrawSetProportional(tFondDialog[0], 1);
	TextDrawSetShadow(tFondDialog[0], 1);
	TextDrawUseBox(tFondDialog[0], 1);
	TextDrawBoxColor(tFondDialog[0], 0);
	TextDrawTextSize(tFondDialog[0], 310.000000, 370.000000);
	TextDrawSetPreviewModel(tFondDialog[0], 19836);
	TextDrawSetPreviewRot(tFondDialog[0], 90.000000, 0.000000, -55.000000, 1.000000);
	TextDrawSetSelectable(tFondDialog[0], 0);
	//---
	tFondDialog[1] = TextDrawCreate(140.000000, 160.000000, "~n~");//Fond
	TextDrawBackgroundColor(tFondDialog[1], 255);
	TextDrawFont(tFondDialog[1], 1);
	TextDrawLetterSize(tFondDialog[1], 0.500000, 14.000000);
	TextDrawColor(tFondDialog[1], -1);
	TextDrawSetOutline(tFondDialog[1], 0);
	TextDrawSetProportional(tFondDialog[1], 1);
	TextDrawSetShadow(tFondDialog[1], 1);
	TextDrawUseBox(tFondDialog[1], 1);
	TextDrawBoxColor(tFondDialog[1], 85);
	TextDrawTextSize(tFondDialog[1], 490.000000, 0.000000);
	TextDrawSetSelectable(tFondDialog[1], 0);
	//---
	tFondDialog[2] = TextDrawCreate(310.000000, 130.000000, "Help");//Titre
	tFondDialog[3] = TextDrawCreate(310.000000, 130.000000, "Aide");//Titre
	tFondDialog[4] = TextDrawCreate(310.000000, 130.000000, "Ayuda");//Titre
	tFondDialog[5] = TextDrawCreate(310.000000, 130.000000, "Ajuda");//Titre
	tFondDialog[6] = TextDrawCreate(310.000000, 130.000000, "Aiuto");//Titre
	tFondDialog[7] = TextDrawCreate(310.000000, 130.000000, "Hilfe");//Titre
	for(new i = 2; i < 8; i ++)
	{
		TextDrawAlignment(tFondDialog[i], 2);
		TextDrawBackgroundColor(tFondDialog[i], 255);
		TextDrawFont(tFondDialog[i], 3);
		TextDrawLetterSize(tFondDialog[i], 1.100000, 4.000000);
		TextDrawColor(tFondDialog[i], -1);
		TextDrawSetOutline(tFondDialog[i], 1);
		TextDrawSetProportional(tFondDialog[i], 1);
		TextDrawSetSelectable(tFondDialog[i], 0);
	}
	//---
	tAdmin = TextDrawCreate(610.000000, 120.000000, "LD_DRV:golfly");
	TextDrawBackgroundColor(tAdmin, 255);
	TextDrawFont(tAdmin, 4);
	TextDrawLetterSize(tAdmin, 0.300000, 6.699999);
	TextDrawColor(tAdmin, -1);
	TextDrawSetOutline(tAdmin, 1);
	TextDrawSetProportional(tAdmin, 1);
	TextDrawUseBox(tAdmin, 1);
	TextDrawBoxColor(tAdmin, 0);
	TextDrawTextSize(tAdmin, 30.000000, 30.000000);
	TextDrawSetSelectable(tAdmin, true);
	//---
	tFondLevel = TextDrawCreate(0.000000, 431.000000, "~n~");
	TextDrawBackgroundColor(tFondLevel, 255);
	TextDrawFont(tFondLevel, 1);
	TextDrawLetterSize(tFondLevel, 1.100000, 2.800000);
	TextDrawColor(tFondLevel, -1);
	TextDrawSetOutline(tFondLevel, 0);
	TextDrawSetProportional(tFondLevel, 1);
	TextDrawSetShadow(tFondLevel, 1);
	TextDrawUseBox(tFondLevel, 1);
	TextDrawBoxColor(tFondLevel, 0x00000055);
	TextDrawTextSize(tFondLevel, 142.000000, 0.000000);
	TextDrawSetSelectable(tFondLevel, 0);
	//---
	tBossBar = TextDrawCreate(480.000000, 170.000000, "~n~");
	TextDrawBackgroundColor(tBossBar, 255);
	TextDrawFont(tBossBar, 1);
	TextDrawLetterSize(tBossBar, 0.500000, 1.000000);
	TextDrawColor(tBossBar, -1);
	TextDrawSetOutline(tBossBar, 0);
	TextDrawSetProportional(tBossBar, 1);
	TextDrawSetShadow(tBossBar, 1);
	TextDrawUseBox(tBossBar, 1);
	TextDrawBoxColor(tBossBar, 85);
	TextDrawTextSize(tBossBar, 630.000000, 10.000000);
	TextDrawSetSelectable(tBossBar, false);
	//---
	tBarettesLevel[0] = TextDrawCreate(14.000000, 364.000000, ".");
	TextDrawBackgroundColor(tBarettesLevel[0], 255);
	TextDrawFont(tBarettesLevel[0], 2);
	TextDrawLetterSize(tBarettesLevel[0], 0.100000, 10.000000);
	TextDrawColor(tBarettesLevel[0], 255);
	TextDrawSetOutline(tBarettesLevel[0], 0);
	TextDrawSetProportional(tBarettesLevel[0], 1);
	TextDrawSetShadow(tBarettesLevel[0], 0);
	TextDrawSetSelectable(tBarettesLevel[0], 0);
	//---
	tBarettesLevel[1] = TextDrawCreate(28.000000, 364.000000, ".");
	TextDrawBackgroundColor(tBarettesLevel[1], 255);
	TextDrawFont(tBarettesLevel[1], 2);
	TextDrawLetterSize(tBarettesLevel[1], 0.100000, 10.000000);
	TextDrawColor(tBarettesLevel[1], 255);
	TextDrawSetOutline(tBarettesLevel[1], 0);
	TextDrawSetProportional(tBarettesLevel[1], 1);
	TextDrawSetShadow(tBarettesLevel[1], 0);
	TextDrawSetSelectable(tBarettesLevel[1], 0);
	//---
	tBarettesLevel[2] = TextDrawCreate(42.000000, 364.000000, ".");
	TextDrawBackgroundColor(tBarettesLevel[2], 255);
	TextDrawFont(tBarettesLevel[2], 2);
	TextDrawLetterSize(tBarettesLevel[2], 0.100000, 10.000000);
	TextDrawColor(tBarettesLevel[2], 255);
	TextDrawSetOutline(tBarettesLevel[2], 0);
	TextDrawSetProportional(tBarettesLevel[2], 1);
	TextDrawSetShadow(tBarettesLevel[2], 0);
	TextDrawSetSelectable(tBarettesLevel[2], 0);
	//---
	tBarettesLevel[3] = TextDrawCreate(56.000000, 364.000000, ".");
	TextDrawBackgroundColor(tBarettesLevel[3], 255);
	TextDrawFont(tBarettesLevel[3], 2);
	TextDrawLetterSize(tBarettesLevel[3], 0.100000, 10.000000);
	TextDrawColor(tBarettesLevel[3], 255);
	TextDrawSetOutline(tBarettesLevel[3], 0);
	TextDrawSetProportional(tBarettesLevel[3], 1);
	TextDrawSetShadow(tBarettesLevel[3], 0);
	TextDrawSetSelectable(tBarettesLevel[3], 0);
	//---
	tBarettesLevel[4] = TextDrawCreate(70.000000, 364.000000, ".");
	TextDrawBackgroundColor(tBarettesLevel[4], 255);
	TextDrawFont(tBarettesLevel[4], 2);
	TextDrawLetterSize(tBarettesLevel[4], 0.100000, 10.000000);
	TextDrawColor(tBarettesLevel[4], 255);
	TextDrawSetOutline(tBarettesLevel[4], 0);
	TextDrawSetProportional(tBarettesLevel[4], 1);
	TextDrawSetShadow(tBarettesLevel[4], 0);
	TextDrawSetSelectable(tBarettesLevel[4], 0);
	//---
	tBarettesLevel[5] = TextDrawCreate(84.000000, 364.000000, ".");
	TextDrawBackgroundColor(tBarettesLevel[5], 255);
	TextDrawFont(tBarettesLevel[5], 2);
	TextDrawLetterSize(tBarettesLevel[5], 0.100000, 10.000000);
	TextDrawColor(tBarettesLevel[5], 255);
	TextDrawSetOutline(tBarettesLevel[5], 0);
	TextDrawSetProportional(tBarettesLevel[5], 1);
	TextDrawSetShadow(tBarettesLevel[5], 0);
	TextDrawSetSelectable(tBarettesLevel[5], 0);
	//---
	tBarettesLevel[6] = TextDrawCreate(98.000000, 364.000000, ".");
	TextDrawBackgroundColor(tBarettesLevel[6], 255);
	TextDrawFont(tBarettesLevel[6], 2);
	TextDrawLetterSize(tBarettesLevel[6], 0.100000, 10.000000);
	TextDrawColor(tBarettesLevel[6], 255);
	TextDrawSetOutline(tBarettesLevel[6], 0);
	TextDrawSetProportional(tBarettesLevel[6], 1);
	TextDrawSetShadow(tBarettesLevel[6], 0);
	TextDrawSetSelectable(tBarettesLevel[6], 0);
	//---
	tBarettesLevel[7] = TextDrawCreate(112.000000, 364.000000, ".");
	TextDrawBackgroundColor(tBarettesLevel[7], 255);
	TextDrawFont(tBarettesLevel[7], 2);
	TextDrawLetterSize(tBarettesLevel[7], 0.100000, 10.000000);
	TextDrawColor(tBarettesLevel[7], 255);
	TextDrawSetOutline(tBarettesLevel[7], 0);
	TextDrawSetProportional(tBarettesLevel[7], 1);
	TextDrawSetShadow(tBarettesLevel[7], 0);
	TextDrawSetSelectable(tBarettesLevel[7], 0);
	//---
	tBarettesLevel[8] = TextDrawCreate(126.000000, 364.000000, ".");
	TextDrawBackgroundColor(tBarettesLevel[8], 255);
	TextDrawFont(tBarettesLevel[8], 2);
	TextDrawLetterSize(tBarettesLevel[8], 0.100000, 10.000000);
	TextDrawColor(tBarettesLevel[8], 255);
	TextDrawSetOutline(tBarettesLevel[8], 0);
	TextDrawSetProportional(tBarettesLevel[8], 1);
	TextDrawSetShadow(tBarettesLevel[8], 0);
	TextDrawSetSelectable(tBarettesLevel[8], 0);
	//---
	tBarettesLevel[9] = TextDrawCreate(140.000000, 364.000000, ".");
	TextDrawBackgroundColor(tBarettesLevel[9], 255);
	TextDrawFont(tBarettesLevel[9], 2);
	TextDrawLetterSize(tBarettesLevel[9], 0.100000, 10.000000);
	TextDrawColor(tBarettesLevel[9], 255);
	TextDrawSetOutline(tBarettesLevel[9], 0);
	TextDrawSetProportional(tBarettesLevel[9], 1);
	TextDrawSetShadow(tBarettesLevel[9], 0);
	TextDrawSetSelectable(tBarettesLevel[9], 0);
	//---
	tFondAtelier[0] = TextDrawCreate(197.000000, 140.000000, "~n~");
	TextDrawBackgroundColor(tFondAtelier[0], 255);
	TextDrawFont(tFondAtelier[0], 1);
	TextDrawLetterSize(tFondAtelier[0], 0.500000, 16.899997);
	TextDrawColor(tFondAtelier[0], -1);
	TextDrawSetOutline(tFondAtelier[0], 0);
	TextDrawSetProportional(tFondAtelier[0], 1);
	TextDrawSetShadow(tFondAtelier[0], 1);
	TextDrawUseBox(tFondAtelier[0], 1);
	TextDrawBoxColor(tFondAtelier[0], 153);
	TextDrawTextSize(tFondAtelier[0], 442.000000, 10.000000);
	TextDrawSetSelectable(tFondAtelier[0], 0);
	//---
	tFondAtelier[1] = TextDrawCreate(197.000000, 90.000000, "~n~");
	TextDrawBackgroundColor(tFondAtelier[1], 255);
	TextDrawFont(tFondAtelier[1], 1);
	TextDrawLetterSize(tFondAtelier[1], 0.519999, 5.099997);
	TextDrawColor(tFondAtelier[1], -1);
	TextDrawSetOutline(tFondAtelier[1], 0);
	TextDrawSetProportional(tFondAtelier[1], 1);
	TextDrawSetShadow(tFondAtelier[1], 1);
	TextDrawUseBox(tFondAtelier[1], 1);
	TextDrawBoxColor(tFondAtelier[1], 153);
	TextDrawTextSize(tFondAtelier[1], 442.000000, 10.000000);
	TextDrawSetSelectable(tFondAtelier[1], 0);
	//---
	tFondAtelier[2] = TextDrawCreate(197.000000, 314.000000, "~n~");
	TextDrawBackgroundColor(tFondAtelier[2], 255);
	TextDrawFont(tFondAtelier[2], 1);
	TextDrawLetterSize(tFondAtelier[2], 0.519999, 12.099995);
	TextDrawColor(tFondAtelier[2], -1);
	TextDrawSetOutline(tFondAtelier[2], 0);
	TextDrawSetProportional(tFondAtelier[2], 1);
	TextDrawSetShadow(tFondAtelier[2], 1);
	TextDrawUseBox(tFondAtelier[2], 1);
	TextDrawBoxColor(tFondAtelier[2], 153);
	TextDrawTextSize(tFondAtelier[2], 442.000000, 10.000000);
	TextDrawSetSelectable(tFondAtelier[2], 0);
	//---
	tBoutonsAtelier[0] = TextDrawCreate(210.000000, 82.000000, "Armes");
	TextDrawBackgroundColor(tBoutonsAtelier[0], 0);
	TextDrawFont(tBoutonsAtelier[0], 5);
	TextDrawLetterSize(tBoutonsAtelier[0], 0.500000, 1.000000);
	TextDrawColor(tBoutonsAtelier[0], -1);
	TextDrawSetOutline(tBoutonsAtelier[0], 0);
	TextDrawSetProportional(tBoutonsAtelier[0], 1);
	TextDrawSetShadow(tBoutonsAtelier[0], 1);
	TextDrawUseBox(tBoutonsAtelier[0], 1);
	TextDrawBoxColor(tBoutonsAtelier[0], 255);
	TextDrawTextSize(tBoutonsAtelier[0], 40.000000, 50.000000);
	TextDrawSetPreviewModel(tBoutonsAtelier[0], 346);
	TextDrawSetPreviewRot(tBoutonsAtelier[0], 0.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(tBoutonsAtelier[0], 1);
	//---
	tBoutonsAtelier[1] = TextDrawCreate(250.000000, 82.000000, "Outils");
	TextDrawBackgroundColor(tBoutonsAtelier[1], 0);
	TextDrawFont(tBoutonsAtelier[1], 5);
	TextDrawLetterSize(tBoutonsAtelier[1], 0.500000, 1.000000);
	TextDrawColor(tBoutonsAtelier[1], -1);
	TextDrawSetOutline(tBoutonsAtelier[1], 0);
	TextDrawSetProportional(tBoutonsAtelier[1], 1);
	TextDrawSetShadow(tBoutonsAtelier[1], 1);
	TextDrawUseBox(tBoutonsAtelier[1], 1);
	TextDrawBoxColor(tBoutonsAtelier[1], 255);
	TextDrawTextSize(tBoutonsAtelier[1], 40.000000, 50.000000);
	TextDrawSetPreviewModel(tBoutonsAtelier[1], 19627);
	TextDrawSetPreviewRot(tBoutonsAtelier[1], 90.000000, 0.000000, 35.000000, 1.000000);
	TextDrawSetSelectable(tBoutonsAtelier[1], 1);
	//---
	tBoutonsAtelier[2] = TextDrawCreate(300.000000, 82.000000, "Machines");
	TextDrawBackgroundColor(tBoutonsAtelier[2], 0);
	TextDrawFont(tBoutonsAtelier[2], 5);
	TextDrawLetterSize(tBoutonsAtelier[2], 0.500000, 1.000000);
	TextDrawColor(tBoutonsAtelier[2], -1);
	TextDrawSetOutline(tBoutonsAtelier[2], 0);
	TextDrawSetProportional(tBoutonsAtelier[2], 1);
	TextDrawSetShadow(tBoutonsAtelier[2], 1);
	TextDrawUseBox(tBoutonsAtelier[2], 1);
	TextDrawBoxColor(tBoutonsAtelier[2], 255);
	TextDrawTextSize(tBoutonsAtelier[2], 40.000000, 50.000000);
	TextDrawSetPreviewModel(tBoutonsAtelier[2], 19917);
	TextDrawSetPreviewRot(tBoutonsAtelier[2], 330.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(tBoutonsAtelier[2], 1);
	//---
	tBoutonsAtelier[3] = TextDrawCreate(350.000000, 82.000000, "Meubles");
	TextDrawBackgroundColor(tBoutonsAtelier[3], 0);
	TextDrawFont(tBoutonsAtelier[3], 5);
	TextDrawLetterSize(tBoutonsAtelier[3], 0.500000, 1.000000);
	TextDrawColor(tBoutonsAtelier[3], -1);
	TextDrawSetOutline(tBoutonsAtelier[3], 0);
	TextDrawSetProportional(tBoutonsAtelier[3], 1);
	TextDrawSetShadow(tBoutonsAtelier[3], 1);
	TextDrawUseBox(tBoutonsAtelier[3], 1);
	TextDrawBoxColor(tBoutonsAtelier[3], 255);
	TextDrawTextSize(tBoutonsAtelier[3], 40.000000, 50.000000);
	TextDrawSetPreviewModel(tBoutonsAtelier[3], 11503);
	TextDrawSetPreviewRot(tBoutonsAtelier[3], 0.000000, 0.000000, 180.000000, 1.000000);
	TextDrawSetSelectable(tBoutonsAtelier[3], 1);
	//------
	//---
	tFondHUD[0] = TextDrawCreate(77.000000, 140.000000, "~n~");
	TextDrawBackgroundColor(tFondHUD[0], 255);
	TextDrawFont(tFondHUD[0], 1);
	TextDrawLetterSize(tFondHUD[0], 0.500000, 18.000000);
	TextDrawColor(tFondHUD[0], -1);
	TextDrawSetOutline(tFondHUD[0], 0);
	TextDrawSetProportional(tFondHUD[0], 1);
	TextDrawSetShadow(tFondHUD[0], 1);
	TextDrawUseBox(tFondHUD[0], 1);
	TextDrawBoxColor(tFondHUD[0], 153);
	TextDrawTextSize(tFondHUD[0], 194.000000, 0.000000);
	TextDrawSetSelectable(tFondHUD[0], false);
	//---
	tFondHUD[1] = TextDrawCreate(445.000000, 140.000000, "~n~");
	TextDrawBackgroundColor(tFondHUD[1], 255);
	TextDrawFont(tFondHUD[1], 1);
	TextDrawLetterSize(tFondHUD[1], 0.500000, 18.000000);
	TextDrawColor(tFondHUD[1], -1);
	TextDrawSetOutline(tFondHUD[1], 0);
	TextDrawSetProportional(tFondHUD[1], 1);
	TextDrawSetShadow(tFondHUD[1], 1);
	TextDrawUseBox(tFondHUD[1], 1);
	TextDrawBoxColor(tFondHUD[1], 153);
	TextDrawTextSize(tFondHUD[1], 565.000000, 0.000000);
	TextDrawSetSelectable(tFondHUD[1], false);
	//---
	tFondHUD[2] = TextDrawCreate(197.000000, 140.000000, "~n~");
	TextDrawBackgroundColor(tFondHUD[2], 255);
	TextDrawFont(tFondHUD[2], 1);
	TextDrawLetterSize(tFondHUD[2], 0.500000, 33.900001);
	TextDrawColor(tFondHUD[2], -1);
	TextDrawSetOutline(tFondHUD[2], 0);
	TextDrawSetProportional(tFondHUD[2], 1);
	TextDrawSetShadow(tFondHUD[2], 1);
	TextDrawUseBox(tFondHUD[2], 1);
	TextDrawBoxColor(tFondHUD[2], 153);
	TextDrawTextSize(tFondHUD[2], 442.000000, 10.000000);
	TextDrawSetSelectable(tFondHUD[2], false);
	//---
	tFondHUD[3] = TextDrawCreate(445.000000, 306.000000, "~n~");
	TextDrawBackgroundColor(tFondHUD[3], 255);
	TextDrawFont(tFondHUD[3], 1);
	TextDrawLetterSize(tFondHUD[3], 0.500000, 15.449999);
	TextDrawColor(tFondHUD[3], -1);
	TextDrawSetOutline(tFondHUD[3], 0);
	TextDrawSetProportional(tFondHUD[3], 1);
	TextDrawSetShadow(tFondHUD[3], 1);
	TextDrawUseBox(tFondHUD[3], 1);
	TextDrawBoxColor(tFondHUD[3], 153);
	TextDrawTextSize(tFondHUD[3], 565.000000, 0.000000);
	TextDrawSetSelectable(tFondHUD[3], false);
	//---
	tFondHUD[4] = TextDrawCreate(197.000000, 28.000000, "~n~");
	TextDrawBackgroundColor(tFondHUD[4], 255);
	TextDrawFont(tFondHUD[4], 1);
	TextDrawLetterSize(tFondHUD[4], 0.500000, 11.199996);
	TextDrawColor(tFondHUD[4], -1);
	TextDrawSetOutline(tFondHUD[4], 0);
	TextDrawSetProportional(tFondHUD[4], 1);
	TextDrawSetShadow(tFondHUD[4], 1);
	TextDrawUseBox(tFondHUD[4], 1);
	TextDrawBoxColor(tFondHUD[4], 153);
	TextDrawTextSize(tFondHUD[4], 442.000000, 10.000000);
	TextDrawSetSelectable(tFondHUD[4], false);
	//---
	tDarkScreen = TextDrawCreate(0.000000, 0.000000, "~n~");
	TextDrawBackgroundColor(tDarkScreen, 255);
	TextDrawFont(tDarkScreen, 1);
	TextDrawLetterSize(tDarkScreen, 0.500000, 49.600006);
	TextDrawColor(tDarkScreen, -1);
	TextDrawSetOutline(tDarkScreen, 0);
	TextDrawSetProportional(tDarkScreen, 1);
	TextDrawSetShadow(tDarkScreen, 1);
	TextDrawUseBox(tDarkScreen, 1);
	TextDrawBoxColor(tDarkScreen, 0x00000077);
	TextDrawTextSize(tDarkScreen, 640.000000, 100.000000);
	TextDrawSetSelectable(tDarkScreen, 0);
	//---
	tHelp = TextDrawCreate(80.000000, 280.000000, "?");
	TextDrawBackgroundColor(tHelp, 255);
	TextDrawFont(tHelp, 2);
	TextDrawLetterSize(tHelp, 0.600000, 2.000000);
	TextDrawColor(tHelp, -1);
	TextDrawSetOutline(tHelp, 1);
	TextDrawSetProportional(tHelp, 1);
	TextDrawTextSize(tHelp, 100.000000, 20.000000);
	TextDrawSetSelectable(tHelp, true);
	//---
	tCompetence = TextDrawCreate(110.000000, 280.000000, "+");
	TextDrawBackgroundColor(tCompetence, 255);
	TextDrawFont(tCompetence, 2);
	TextDrawLetterSize(tCompetence, 0.600000, 2.000000);
	TextDrawColor(tCompetence, -1);
	TextDrawSetOutline(tCompetence, 1);
	TextDrawSetProportional(tCompetence, 1);
	TextDrawTextSize(tCompetence, 130.000000, 20.000000);
	TextDrawSetSelectable(tCompetence, true);
	//---
	tGroup = TextDrawCreate(140.000000, 280.000000, ")(");
	TextDrawBackgroundColor(tGroup, 255);
	TextDrawFont(tGroup, 2);
	TextDrawLetterSize(tGroup, 0.600000, 2.000000);
	TextDrawColor(tGroup, -1);
	TextDrawSetOutline(tGroup, 1);
	TextDrawSetProportional(tGroup, 1);
	TextDrawTextSize(tGroup, 160.000000, 20.000000);
	TextDrawSetSelectable(tGroup, true);
	//---
	tOptions = TextDrawCreate(170.000000, 280.000000, "§");
	TextDrawBackgroundColor(tOptions, 255);
	TextDrawFont(tOptions, 2);
	TextDrawLetterSize(tOptions, 0.600000, 2.000000);
	TextDrawColor(tOptions, -1);
	TextDrawSetOutline(tOptions, 1);
	TextDrawSetProportional(tOptions, 1);
	TextDrawTextSize(tOptions, 190.000000, 20.000000);
	TextDrawSetSelectable(tOptions, true);
	//---
	/*tConnection = TextDrawCreate(0.000000, -1.000000, "particle:coronamoon");
	TextDrawBackgroundColor(tConnection, 255);
	TextDrawFont(tConnection, 4);
	TextDrawLetterSize(tConnection, -0.099999, 40.599990);
	TextDrawColor(tConnection, -1);
	TextDrawSetOutline(tConnection, 0);
	TextDrawSetProportional(tConnection, 1);
	TextDrawSetShadow(tConnection, 1);
	TextDrawUseBox(tConnection, 1);
	TextDrawBoxColor(tConnection, 255);
	TextDrawTextSize(tConnection, 640.000000, 449.000000);
	TextDrawSetSelectable(tConnection, 0);*/
	//---
	tHUDFont[0] = TextDrawCreate(470.000000, -1.000000, "LD_DUAL:backgnd");
	TextDrawBackgroundColor(tHUDFont[0], -872414977);
	TextDrawFont(tHUDFont[0], 4);
	TextDrawLetterSize(tHUDFont[0], 0.500000, 1.000000);
	TextDrawColor(tHUDFont[0], -1);
	TextDrawSetOutline(tHUDFont[0], 1);
	TextDrawSetProportional(tHUDFont[0], 1);
	TextDrawUseBox(tHUDFont[0], 1);
	TextDrawBoxColor(tHUDFont[0], 255);
	TextDrawTextSize(tHUDFont[0], 170.000000, 120.000000);
	TextDrawSetSelectable(tHUDFont[0], 0);
	//---
	tHUDFont[1] = TextDrawCreate(488.500000, 1.000000, "~n~");
	TextDrawBackgroundColor(tHUDFont[1], 255);
	TextDrawFont(tHUDFont[1], 1);
	TextDrawLetterSize(tHUDFont[1], 0.490000, 11.800003);
	TextDrawColor(tHUDFont[1], -1);
	TextDrawSetOutline(tHUDFont[1], 0);
	TextDrawSetProportional(tHUDFont[1], 1);
	TextDrawSetShadow(tHUDFont[1], 1);
	TextDrawUseBox(tHUDFont[1], 1);
	TextDrawBoxColor(tHUDFont[1], 1275092070);
	TextDrawTextSize(tHUDFont[1], 469.000000, -30.000000);
	TextDrawSetSelectable(tHUDFont[1], 0);
	//---
	tHUDFont[2] = TextDrawCreate(471.000000, 2.000000, "hud:radar_burgershot");
	TextDrawBackgroundColor(tHUDFont[2], 255);
	TextDrawFont(tHUDFont[2], 4);
	TextDrawLetterSize(tHUDFont[2], 9.629997, 8.000000);
	TextDrawColor(tHUDFont[2], -1);
	TextDrawSetOutline(tHUDFont[2], 0);
	TextDrawSetProportional(tHUDFont[2], 1);
	TextDrawSetShadow(tHUDFont[2], 1);
	TextDrawUseBox(tHUDFont[2], 1);
	TextDrawBoxColor(tHUDFont[2], 255);
	TextDrawTextSize(tHUDFont[2], 15.000000, 15.000000);
	TextDrawSetSelectable(tHUDFont[2], 0);
	//---
	tHUDFont[3] = TextDrawCreate(471.000000, 20.000000, "hud:radar_dateDrink");
	TextDrawBackgroundColor(tHUDFont[3], 255);
	TextDrawFont(tHUDFont[3], 4);
	TextDrawLetterSize(tHUDFont[3], 9.629997, 8.000000);
	TextDrawColor(tHUDFont[3], -1);
	TextDrawSetOutline(tHUDFont[3], 0);
	TextDrawSetProportional(tHUDFont[3], 1);
	TextDrawSetShadow(tHUDFont[3], 1);
	TextDrawUseBox(tHUDFont[3], 1);
	TextDrawBoxColor(tHUDFont[3], 255);
	TextDrawTextSize(tHUDFont[3], 15.000000, 15.000000);
	TextDrawSetSelectable(tHUDFont[3], 0);
	//---
	tHUDFont[4] = TextDrawCreate(473.000000, 37.000000, "Zzz");
	TextDrawBackgroundColor(tHUDFont[4], 255);
	TextDrawFont(tHUDFont[4], 1);
	TextDrawLetterSize(tHUDFont[4], 0.199996, 2.000000);
	TextDrawColor(tHUDFont[4], -1);
	TextDrawSetOutline(tHUDFont[4], 0);
	TextDrawSetProportional(tHUDFont[4], 1);
	TextDrawSetShadow(tHUDFont[4], 1);
	TextDrawSetSelectable(tHUDFont[4], 0);
	//---
	tHUDFont[5] = TextDrawCreate(460.000000, 43.000000, "Or");
	TextDrawBackgroundColor(tHUDFont[5], 0);
	TextDrawFont(tHUDFont[5], 5);
	TextDrawLetterSize(tHUDFont[5], 0.500000, 1.000000);
	TextDrawColor(tHUDFont[5], -1);
	TextDrawSetOutline(tHUDFont[5], 0);
	TextDrawSetProportional(tHUDFont[5], 1);
	TextDrawSetShadow(tHUDFont[5], 1);
	TextDrawUseBox(tHUDFont[5], 1);
	TextDrawBoxColor(tHUDFont[5], 0);
	TextDrawTextSize(tHUDFont[5], 40.000000, 40.000000);
	TextDrawSetPreviewModel(tHUDFont[5], 19941);
	TextDrawSetPreviewRot(tHUDFont[5], -16.000000, 0.000000, -55.000000, 1.000000);
	TextDrawSetSelectable(tHUDFont[5], true);
	//---
	tHUDFont[6] = TextDrawCreate(471.000000, 74.000000, "hud:radar_girlfriend");
	TextDrawBackgroundColor(tHUDFont[6], 255);
	TextDrawFont(tHUDFont[6], 4);
	TextDrawLetterSize(tHUDFont[6], 9.629997, 8.000000);
	TextDrawColor(tHUDFont[6], -1);
	TextDrawSetOutline(tHUDFont[6], 0);
	TextDrawSetProportional(tHUDFont[6], 1);
	TextDrawSetShadow(tHUDFont[6], 1);
	TextDrawUseBox(tHUDFont[6], 1);
	TextDrawBoxColor(tHUDFont[6], 255);
	TextDrawTextSize(tHUDFont[6], 15.000000, 15.000000);
	TextDrawSetSelectable(tHUDFont[6], 0);
	//---
	tHUDFont[7] = TextDrawCreate(471.000000, 92.000000, "hud:radar_tshirt");
	TextDrawBackgroundColor(tHUDFont[7], 255);
	TextDrawFont(tHUDFont[7], 4);
	TextDrawLetterSize(tHUDFont[7], 9.629997, 8.000000);
	TextDrawColor(tHUDFont[7], -1);
	TextDrawSetOutline(tHUDFont[7], 0);
	TextDrawSetProportional(tHUDFont[7], 1);
	TextDrawSetShadow(tHUDFont[7], 1);
	TextDrawUseBox(tHUDFont[7], 1);
	TextDrawBoxColor(tHUDFont[7], 255);
	TextDrawTextSize(tHUDFont[7], 15.000000, 15.000000);
	TextDrawSetSelectable(tHUDFont[7], 0);
	//---
	tHUDFont[8] = TextDrawCreate(-40.000000, 270.000000, "New Textdraw");
	TextDrawBackgroundColor(tHUDFont[8], 0);
	TextDrawFont(tHUDFont[8], 5);
	TextDrawLetterSize(tHUDFont[8], 0.500000, 1.000000);
	TextDrawColor(tHUDFont[8], -1);
	TextDrawSetOutline(tHUDFont[8], 0);
	TextDrawSetProportional(tHUDFont[8], 1);
	TextDrawSetShadow(tHUDFont[8], 1);
	TextDrawUseBox(tHUDFont[8], 1);
	TextDrawBoxColor(tHUDFont[8], 255);
	TextDrawTextSize(tHUDFont[8], 240.000000, 220.000000);
	TextDrawSetPreviewModel(tHUDFont[8], 19836);
	TextDrawSetPreviewRot(tHUDFont[8], 90.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(tHUDFont[8], false);
	//---
	tBrokenLeg[0] = TextDrawCreate(472.000000, 109.000000, "Broken leg!");
	tBrokenLeg[1] = TextDrawCreate(472.000000, 109.000000, "Jambe cassee !");
	tBrokenLeg[2] = TextDrawCreate(472.000000, 109.000000, "¡Pierna rotta!");
	tBrokenLeg[3] = TextDrawCreate(472.000000, 109.000000, "Perna quebrada !");
	tBrokenLeg[4] = TextDrawCreate(472.000000, 109.000000, "Gamba rotta !");
	tBrokenLeg[5] = TextDrawCreate(472.000000, 109.000000, "Gebrochenes Bein!");
	for(new i = 0; i < 6; i ++)
 	{
		TextDrawBackgroundColor(tBrokenLeg[i], 255);
		TextDrawFont(tBrokenLeg[i], 2);
		TextDrawLetterSize(tBrokenLeg[i], 0.200000, 1.000000);
		TextDrawColor(tBrokenLeg[i], -872414977);
		TextDrawSetOutline(tBrokenLeg[i], 1);
		TextDrawSetProportional(tBrokenLeg[i], 1);
		TextDrawSetSelectable(tBrokenLeg[i], 0);
	}
	tHemoragie[0] = TextDrawCreate(640.000000, 109.000000, "Hemorrhage!");
	tHemoragie[1] = TextDrawCreate(640.000000, 109.000000, "Hemorragie !");
	tHemoragie[2] = TextDrawCreate(640.000000, 109.000000, "¡Hemorragia!");
	tHemoragie[3] = TextDrawCreate(640.000000, 109.000000, "Portugais");
	tHemoragie[4] = TextDrawCreate(640.000000, 109.000000, "Emorragia !");
	tHemoragie[5] = TextDrawCreate(640.000000, 109.000000, "Blutung!");
	for(new i = 0; i < 6; i ++)
 	{
		TextDrawAlignment(tHemoragie[i], 3);
		TextDrawBackgroundColor(tHemoragie[i], 255);
		TextDrawFont(tHemoragie[i], 2);
		TextDrawLetterSize(tHemoragie[i], 0.200000, 1.000000);
		TextDrawColor(tHemoragie[i], -872414977);
		TextDrawSetOutline(tHemoragie[i], 1);
		TextDrawSetProportional(tHemoragie[i], 1);
		TextDrawSetSelectable(tHemoragie[i], 0);
	}
	//---
	tBoutonFabriquer[0] = TextDrawCreate(290.000000, 420.000000, "Craft");
	tBoutonFabriquer[1] = TextDrawCreate(290.000000, 420.000000, "Fabriquer");
	tBoutonFabriquer[2] = TextDrawCreate(290.000000, 420.000000, "Espagnol");
	tBoutonFabriquer[3] = TextDrawCreate(290.000000, 420.000000, "Fazer");
	tBoutonFabriquer[4] = TextDrawCreate(290.000000, 420.000000, "Italien");
	tBoutonFabriquer[5] = TextDrawCreate(290.000000, 420.000000, "Handwerken");
	for(new i = 0; i < 6; i ++)
 	{
		TextDrawBackgroundColor(tBoutonFabriquer[i], 255);
		TextDrawFont(tBoutonFabriquer[i], 3);
		TextDrawLetterSize(tBoutonFabriquer[i], 0.500000, 1.000000);
		TextDrawColor(tBoutonFabriquer[i], -1);
		TextDrawSetOutline(tBoutonFabriquer[i], 1);
		TextDrawSetProportional(tBoutonFabriquer[i], 1);
		TextDrawSetSelectable(tBoutonFabriquer[i], true);
	}
	//---
	tBoutonCuisiner[0] = TextDrawCreate(290.000000, 420.000000, "Cook");
	tBoutonCuisiner[1] = TextDrawCreate(290.000000, 420.000000, "Cuisiner");
	tBoutonCuisiner[2] = TextDrawCreate(290.000000, 420.000000, "Cucinar");
	tBoutonCuisiner[3] = TextDrawCreate(290.000000, 420.000000, "Cozinhar");
	tBoutonCuisiner[4] = TextDrawCreate(290.000000, 420.000000, "Cucinare");
	tBoutonCuisiner[5] = TextDrawCreate(290.000000, 420.000000, "Kochen");
	for(new i = 0; i < 6; i ++)
 	{
		TextDrawBackgroundColor(tBoutonCuisiner[i], 255);
		TextDrawFont(tBoutonCuisiner[i], 3);
		TextDrawLetterSize(tBoutonCuisiner[i], 0.500000, 1.000000);
		TextDrawColor(tBoutonCuisiner[i], -1);
		TextDrawSetOutline(tBoutonCuisiner[i], 1);
		TextDrawSetProportional(tBoutonCuisiner[i], 1);
		TextDrawSetSelectable(tBoutonCuisiner[i], true);
	}
	//---
	tBoutonBatir[0] = TextDrawCreate(290.000000, 420.000000, "Build");
	tBoutonBatir[1] = TextDrawCreate(290.000000, 420.000000, "Batir");
	tBoutonBatir[2] = TextDrawCreate(290.000000, 420.000000, "Espagnol");
	tBoutonBatir[3] = TextDrawCreate(290.000000, 420.000000, "Construir");
	tBoutonBatir[4] = TextDrawCreate(290.000000, 420.000000, "Italien");
	tBoutonBatir[5] = TextDrawCreate(290.000000, 420.000000, "Bauen");
	for(new i = 0; i < 6; i ++)
 	{
		TextDrawBackgroundColor(tBoutonBatir[i], 255);
		TextDrawFont(tBoutonBatir[i], 3);
		TextDrawLetterSize(tBoutonBatir[i], 0.500000, 1.000000);
		TextDrawColor(tBoutonBatir[i], -1);
		TextDrawSetOutline(tBoutonBatir[i], 1);
		TextDrawSetProportional(tBoutonBatir[i], 1);
		TextDrawSetSelectable(tBoutonBatir[i], true);
	}
	//---RÈGLES
	tRules[0][0] = TextDrawCreate(230.000000, 150.000000, "Rules");
	tRules[1][0] = TextDrawCreate(230.000000, 150.000000, "Regles");
	tRules[2][0] = TextDrawCreate(230.000000, 150.000000, "Espagnol");
	tRules[3][0] = TextDrawCreate(230.000000, 150.000000, "Portugais");
	tRules[4][0] = TextDrawCreate(230.000000, 150.000000, "Italien");
	tRules[5][0] = TextDrawCreate(230.000000, 150.000000, "Allemand");
	for(new i = 0; i < 6; i ++)
 	{
		TextDrawBackgroundColor(tRules[i][0], -872415096);
		TextDrawFont(tRules[i][0], 3);
		TextDrawLetterSize(tRules[i][0], 0.700000, 2.000000);
		TextDrawColor(tRules[i][0], 255);
		TextDrawSetOutline(tRules[i][0], 1);
		TextDrawSetProportional(tRules[i][0], 1);
		TextDrawSetSelectable(tRules[i][0], 0);
	}
	//
	tRules[0][1] = TextDrawCreate(220.000000, 170.000000, "1.  No cheat.~n~2. No explicit vocabulary.~n~3. No ad. for other servers.~n~4. No ban evade.~n~5. No bug exploiting.");
	tRules[1][1] = TextDrawCreate(220.000000, 170.000000, "1.  Pas de cheat.~n~2. Pas d'insultes.~n~3. Pas de pub.~n~4. Pas de ban evade.~n~5. Pas d'abus de bugs.");
	tRules[2][1] = TextDrawCreate(220.000000, 170.000000, "1.  Espgnol");
	tRules[3][1] = TextDrawCreate(220.000000, 170.000000, "1.  Portugais");
	tRules[4][1] = TextDrawCreate(220.000000, 170.000000, "1.  Italien");
	tRules[5][1] = TextDrawCreate(220.000000, 170.000000, "1.  Allemand");
	for(new i = 0; i < 6; i ++)
 	{
		TextDrawBackgroundColor(tRules[i][1], 255);
		TextDrawFont(tRules[i][1], 2);
		TextDrawLetterSize(tRules[i][1], 0.200000, 1.499999);
		TextDrawColor(tRules[i][1], -1);
		TextDrawSetOutline(tRules[i][1], 0);
		TextDrawSetProportional(tRules[i][1], 1);
		TextDrawSetShadow(tRules[i][1], 1);
		TextDrawUseBox(tRules[i][1], 1);
		TextDrawBoxColor(tRules[i][1], 102);
		TextDrawTextSize(tRules[i][1], 410.000000, -20.000000);
		TextDrawSetSelectable(tRules[i][1], 0);
 	}
	//---
	tBloodSplat = TextDrawCreate(370.000000, 180.000000, " ");
	TextDrawBackgroundColor(tBloodSplat, 0);
	TextDrawFont(tBloodSplat, 5);
	TextDrawLetterSize(tBloodSplat, 0.500000, 1.000000);
	TextDrawColor(tBloodSplat, -1);
	TextDrawSetOutline(tBloodSplat, 0);
	TextDrawSetProportional(tBloodSplat, 1);
	TextDrawSetShadow(tBloodSplat, 1);
	TextDrawUseBox(tBloodSplat, 1);
	TextDrawBoxColor(tBloodSplat, 0);
	TextDrawTextSize(tBloodSplat, 80.000000, 100.000000);
	TextDrawSetPreviewModel(tBloodSplat, 19836);
	TextDrawSetPreviewRot(tBloodSplat, 90.000000, 0.000000, -55.000000, 1.000000);
	TextDrawSetSelectable(tBloodSplat, 0);
	//---SÉLECTION SKIN---//
	tSkinSelect[0] = TextDrawCreate(240.000000, 130.000000, "Sang");
	TextDrawBackgroundColor(tSkinSelect[0], 0);
	TextDrawFont(tSkinSelect[0], 5);
	TextDrawLetterSize(tSkinSelect[0], 0.500000, 1.000000);
	TextDrawColor(tSkinSelect[0], -1);
	TextDrawSetOutline(tSkinSelect[0], 0);
	TextDrawSetProportional(tSkinSelect[0], 1);
	TextDrawSetShadow(tSkinSelect[0], 1);
	TextDrawUseBox(tSkinSelect[0], 1);
	TextDrawBoxColor(tSkinSelect[0], 255);
	TextDrawTextSize(tSkinSelect[0], 200.000000, 180.000000);
	TextDrawSetPreviewModel(tSkinSelect[0], 19836);
	TextDrawSetPreviewRot(tSkinSelect[0], 90.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(tSkinSelect[0], false);
	//---
	tSkinSelect[1] = TextDrawCreate(190.000000, 240.000000, "Sang");
	TextDrawBackgroundColor(tSkinSelect[1], 0);
	TextDrawFont(tSkinSelect[1], 5);
	TextDrawLetterSize(tSkinSelect[1], 0.500000, 1.000000);
	TextDrawColor(tSkinSelect[1], -1);
	TextDrawSetOutline(tSkinSelect[1], 0);
	TextDrawSetProportional(tSkinSelect[1], 1);
	TextDrawSetShadow(tSkinSelect[1], 1);
	TextDrawUseBox(tSkinSelect[1], 1);
	TextDrawBoxColor(tSkinSelect[1], 255);
	TextDrawTextSize(tSkinSelect[1], 160.000000, 140.000000);
	TextDrawSetPreviewModel(tSkinSelect[1], 19836);
	TextDrawSetPreviewRot(tSkinSelect[1], 90.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(tSkinSelect[1], true);
	//---
	tSkinSelect[2] = TextDrawCreate(330.000000, 240.000000, "Sang");
	TextDrawBackgroundColor(tSkinSelect[2], 0);
	TextDrawFont(tSkinSelect[2], 5);
	TextDrawLetterSize(tSkinSelect[2], 0.500000, 1.000000);
	TextDrawColor(tSkinSelect[2], -1);
	TextDrawSetOutline(tSkinSelect[2], 0);
	TextDrawSetProportional(tSkinSelect[2], 1);
	TextDrawSetShadow(tSkinSelect[2], 1);
	TextDrawUseBox(tSkinSelect[2], 1);
	TextDrawBoxColor(tSkinSelect[2], 255);
	TextDrawTextSize(tSkinSelect[2], 160.000000, 140.000000);
	TextDrawSetPreviewModel(tSkinSelect[2], 19836);
	TextDrawSetPreviewRot(tSkinSelect[2], 90.000000, 0.000000, 180.000000, 1.000000);
	TextDrawSetSelectable(tSkinSelect[2], true);
	//---
	tSkinSelect[3] = TextDrawCreate(250.000000, 290.000000, "hud:arrow");
	TextDrawBackgroundColor(tSkinSelect[3], 255);
	TextDrawFont(tSkinSelect[3], 4);
	TextDrawLetterSize(tSkinSelect[3], 1.000000, 10.000000);
	TextDrawColor(tSkinSelect[3], -1);
	TextDrawSetOutline(tSkinSelect[3], 0);
	TextDrawSetProportional(tSkinSelect[3], 1);
	TextDrawSetShadow(tSkinSelect[3], 1);
	TextDrawUseBox(tSkinSelect[3], 1);
	TextDrawBoxColor(tSkinSelect[3], 255);
	TextDrawTextSize(tSkinSelect[3], 40.000000, 40.000000);
	TextDrawSetSelectable(tSkinSelect[3], false);
	//---
	tSkinSelect[4] = TextDrawCreate(390.000000, 330.000000, "hud:arrow");
	TextDrawBackgroundColor(tSkinSelect[4], 255);
	TextDrawFont(tSkinSelect[4], 4);
	TextDrawLetterSize(tSkinSelect[4], 1.000000, 10.000000);
	TextDrawColor(tSkinSelect[4], -1);
	TextDrawSetOutline(tSkinSelect[4], 0);
	TextDrawSetProportional(tSkinSelect[4], 1);
	TextDrawSetShadow(tSkinSelect[4], 1);
	TextDrawUseBox(tSkinSelect[4], 1);
	TextDrawBoxColor(tSkinSelect[4], 255);
	TextDrawTextSize(tSkinSelect[4], 40.000000, -40.000000);
	TextDrawSetSelectable(tSkinSelect[4], false);
	//---
	tSkinSelect[5] = TextDrawCreate(305.000000, 250.000000, "Spawn");
	TextDrawBackgroundColor(tSkinSelect[5], 255);
	TextDrawFont(tSkinSelect[5], 3);
	TextDrawLetterSize(tSkinSelect[5], 0.600000, 3.000000);
	TextDrawColor(tSkinSelect[5], -1);
	TextDrawSetOutline(tSkinSelect[5], 1);
	TextDrawSetProportional(tSkinSelect[5], 1);
	TextDrawTextSize(tSkinSelect[5], 370.000000, 40.000000);
	TextDrawSetSelectable(tSkinSelect[5], true);
	//---
	tSkinSelect[6] = TextDrawCreate(305.000000, 250.000000, "OK");
	TextDrawBackgroundColor(tSkinSelect[6], 255);
	TextDrawFont(tSkinSelect[6], 3);
	TextDrawLetterSize(tSkinSelect[6], 0.600000, 3.000000);
	TextDrawColor(tSkinSelect[6], -1);
	TextDrawSetOutline(tSkinSelect[6], 1);
	TextDrawSetProportional(tSkinSelect[6], 1);
	TextDrawTextSize(tSkinSelect[6], 370.000000, 40.000000);
	TextDrawSetSelectable(tSkinSelect[6], true);
}

CreatePlayerVehicleHUD(playerid)
{
	tPlayerVehHUD[playerid][0] = CreatePlayerTextDraw(playerid,440.000000, 300.000000, "Moteur");
	PlayerTextDrawBackgroundColor(playerid,tPlayerVehHUD[playerid][0], 0);
	PlayerTextDrawFont(playerid,tPlayerVehHUD[playerid][0], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerVehHUD[playerid][0], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerVehHUD[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerVehHUD[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerVehHUD[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerVehHUD[playerid][0], 1);
	PlayerTextDrawUseBox(playerid,tPlayerVehHUD[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerVehHUD[playerid][0], 255);
	PlayerTextDrawTextSize(playerid,tPlayerVehHUD[playerid][0], 30.000000, 30.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerVehHUD[playerid][0], 19917);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerVehHUD[playerid][0], 330.000000, 0.000000, 25.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerVehHUD[playerid][0], 0);
	//---
	tPlayerVehHUD[playerid][1] = CreatePlayerTextDraw(playerid,470.000000, 310.000000, "Roue");
	PlayerTextDrawBackgroundColor(playerid,tPlayerVehHUD[playerid][1], 0);
	PlayerTextDrawFont(playerid,tPlayerVehHUD[playerid][1], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerVehHUD[playerid][1], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerVehHUD[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerVehHUD[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerVehHUD[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerVehHUD[playerid][1], 1);
	PlayerTextDrawUseBox(playerid,tPlayerVehHUD[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerVehHUD[playerid][1], 255);
	PlayerTextDrawTextSize(playerid,tPlayerVehHUD[playerid][1], 20.000000, 20.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerVehHUD[playerid][1], 1098);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerVehHUD[playerid][1], 0.000000, 0.000000, 90.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerVehHUD[playerid][1], 0);
	//---
	tPlayerVehHUD[playerid][2] = CreatePlayerTextDraw(playerid,495.000000, 310.000000, "Roue");
	PlayerTextDrawBackgroundColor(playerid,tPlayerVehHUD[playerid][2], 0);
	PlayerTextDrawFont(playerid,tPlayerVehHUD[playerid][2], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerVehHUD[playerid][2], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerVehHUD[playerid][2], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerVehHUD[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerVehHUD[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerVehHUD[playerid][2], 1);
	PlayerTextDrawUseBox(playerid,tPlayerVehHUD[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerVehHUD[playerid][2], 255);
	PlayerTextDrawTextSize(playerid,tPlayerVehHUD[playerid][2], 20.000000, 20.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerVehHUD[playerid][2], 1098);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerVehHUD[playerid][2], 0.000000, 0.000000, 90.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerVehHUD[playerid][2], 0);
	//---
	tPlayerVehHUD[playerid][3] = CreatePlayerTextDraw(playerid,520.000000, 310.000000, "Roue");
	PlayerTextDrawBackgroundColor(playerid,tPlayerVehHUD[playerid][3], 0);
	PlayerTextDrawFont(playerid,tPlayerVehHUD[playerid][3], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerVehHUD[playerid][3], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerVehHUD[playerid][3], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerVehHUD[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerVehHUD[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerVehHUD[playerid][3], 1);
	PlayerTextDrawUseBox(playerid,tPlayerVehHUD[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerVehHUD[playerid][3], 255);
	PlayerTextDrawTextSize(playerid,tPlayerVehHUD[playerid][3], 20.000000, 20.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerVehHUD[playerid][3], 1098);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerVehHUD[playerid][3], 0.000000, 0.000000, 90.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerVehHUD[playerid][3], 0);
	//---
	tPlayerVehHUD[playerid][4] = CreatePlayerTextDraw(playerid,545.000000, 310.000000, "Roue");
	PlayerTextDrawBackgroundColor(playerid,tPlayerVehHUD[playerid][4], 0);
	PlayerTextDrawFont(playerid,tPlayerVehHUD[playerid][4], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerVehHUD[playerid][4], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerVehHUD[playerid][4], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerVehHUD[playerid][4], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerVehHUD[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerVehHUD[playerid][4], 1);
	PlayerTextDrawUseBox(playerid,tPlayerVehHUD[playerid][4], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerVehHUD[playerid][4], 255);
	PlayerTextDrawTextSize(playerid,tPlayerVehHUD[playerid][4], 20.000000, 20.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerVehHUD[playerid][4], 1098);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerVehHUD[playerid][4], 0.000000, 0.000000, 90.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerVehHUD[playerid][4], 0);
}

CreatePlayerVehicleInventory(playerid)
{
	new Float:xText[6] = {444.0, 485.0, 526.0, 444.0, 485.0, 526.0};
	new Float:yText[6] = {340.0, 340.0, 340.0, 391.0, 391.0, 391.0};
	for(new i = 0; i < 6; i ++)
	{
		tVehicleTrunk[playerid][i][0] = CreatePlayerTextDraw(playerid, xText[i], yText[i], "Objet1");
		PlayerTextDrawBackgroundColor(playerid,tVehicleTrunk[playerid][i][0], 153);
		PlayerTextDrawFont(playerid,tVehicleTrunk[playerid][i][0], 5);
		PlayerTextDrawLetterSize(playerid,tVehicleTrunk[playerid][i][0], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,tVehicleTrunk[playerid][i][0], -1);
		PlayerTextDrawSetOutline(playerid,tVehicleTrunk[playerid][i][0], 0);
		PlayerTextDrawSetProportional(playerid,tVehicleTrunk[playerid][i][0], 1);
		PlayerTextDrawSetShadow(playerid,tVehicleTrunk[playerid][i][0], 1);
		PlayerTextDrawUseBox(playerid,tVehicleTrunk[playerid][i][0], 1);
		PlayerTextDrawBoxColor(playerid,tVehicleTrunk[playerid][i][0], 255);
		PlayerTextDrawTextSize(playerid,tVehicleTrunk[playerid][i][0], 40.000000, 50.000000);
		PlayerTextDrawSetSelectable(playerid,tVehicleTrunk[playerid][i][0], true);
		//---
		tVehicleTrunk[playerid][i][1] = CreatePlayerTextDraw(playerid, xText[i], yText[i] + 38.0, " ");
		PlayerTextDrawBackgroundColor(playerid,tVehicleTrunk[playerid][i][1], 136);
		PlayerTextDrawFont(playerid,tVehicleTrunk[playerid][i][1], 2);
		PlayerTextDrawLetterSize(playerid,tVehicleTrunk[playerid][i][1], 0.110000, 0.599999);
		PlayerTextDrawColor(playerid,tVehicleTrunk[playerid][i][1], -1);
		PlayerTextDrawSetOutline(playerid,tVehicleTrunk[playerid][i][1], 1);
		PlayerTextDrawSetProportional(playerid,tVehicleTrunk[playerid][i][1], 1);
		PlayerTextDrawSetSelectable(playerid,tVehicleTrunk[playerid][i][1], false);
	}
}

CreatePlayerHUD(playerid)
{
	tPlayerHUD[playerid][0] = CreatePlayerTextDraw(playerid,87.000000, 150.000000, "Skin");
	PlayerTextDrawBackgroundColor(playerid,tPlayerHUD[playerid][0], 0);
	PlayerTextDrawFont(playerid,tPlayerHUD[playerid][0], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerHUD[playerid][0], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerHUD[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerHUD[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerHUD[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerHUD[playerid][0], 1);
	PlayerTextDrawUseBox(playerid,tPlayerHUD[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerHUD[playerid][0], 255);
	PlayerTextDrawTextSize(playerid,tPlayerHUD[playerid][0], 100.000000, 140.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerHUD[playerid][0], false);
	//---
	tPlayerHUD[playerid][1] = CreatePlayerTextDraw(playerid,449.000000, 150.000000, "Arme1");
	PlayerTextDrawBackgroundColor(playerid,tPlayerHUD[playerid][1], 85);
	PlayerTextDrawFont(playerid,tPlayerHUD[playerid][1], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerHUD[playerid][1], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerHUD[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerHUD[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerHUD[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerHUD[playerid][1], 1);
	PlayerTextDrawUseBox(playerid,tPlayerHUD[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerHUD[playerid][1], 255);
	PlayerTextDrawTextSize(playerid,tPlayerHUD[playerid][1], 55.000000, 60.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][1], 355);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][1], 0.000000, 315.000000, 0.000000, 2.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerHUD[playerid][1], true);
	//---
	tPlayerHUD[playerid][2] = CreatePlayerTextDraw(playerid,506.000000, 150.000000, "Arme2");
	PlayerTextDrawBackgroundColor(playerid,tPlayerHUD[playerid][2], 85);
	PlayerTextDrawFont(playerid,tPlayerHUD[playerid][2], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerHUD[playerid][2], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerHUD[playerid][2], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerHUD[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerHUD[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerHUD[playerid][2], 1);
	PlayerTextDrawUseBox(playerid,tPlayerHUD[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerHUD[playerid][2], 255);
	PlayerTextDrawTextSize(playerid,tPlayerHUD[playerid][2], 55.000000, 60.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][2], 349);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][2], 0.000000, 315.000000, 0.000000, 2.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerHUD[playerid][2], true);
	//---
	tPlayerHUD[playerid][3] = CreatePlayerTextDraw(playerid,449.000000, 212.000000, "Arme3");
	PlayerTextDrawBackgroundColor(playerid,tPlayerHUD[playerid][3], 85);
	PlayerTextDrawFont(playerid,tPlayerHUD[playerid][3], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerHUD[playerid][3], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerHUD[playerid][3], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerHUD[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerHUD[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerHUD[playerid][3], 1);
	PlayerTextDrawUseBox(playerid,tPlayerHUD[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerHUD[playerid][3], 255);
	PlayerTextDrawTextSize(playerid,tPlayerHUD[playerid][3], 55.000000, 60.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][3], 352);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][3], -16.000000, 0.000000, 0.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerHUD[playerid][3], true);
	//---
	tPlayerHUD[playerid][4] = CreatePlayerTextDraw(playerid,506.000000, 212.000000, "Arme4");
	PlayerTextDrawBackgroundColor(playerid,tPlayerHUD[playerid][4], 85);
	PlayerTextDrawFont(playerid,tPlayerHUD[playerid][4], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerHUD[playerid][4], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerHUD[playerid][4], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerHUD[playerid][4], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerHUD[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerHUD[playerid][4], 1);
	PlayerTextDrawUseBox(playerid,tPlayerHUD[playerid][4], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerHUD[playerid][4], 255);
	PlayerTextDrawTextSize(playerid,tPlayerHUD[playerid][4], 55.000000, 60.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][4], 336);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][4], 0.000000, 315.000000, 0.000000, 2.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerHUD[playerid][4], true);
	//---
	tPlayerHUD[playerid][5] = CreatePlayerTextDraw(playerid,120.000000, 142.000000, "Chapeau");
	PlayerTextDrawBackgroundColor(playerid,tPlayerHUD[playerid][5], 51);
	PlayerTextDrawFont(playerid,tPlayerHUD[playerid][5], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerHUD[playerid][5], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerHUD[playerid][5], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerHUD[playerid][5], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerHUD[playerid][5], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerHUD[playerid][5], 1);
	PlayerTextDrawUseBox(playerid,tPlayerHUD[playerid][5], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerHUD[playerid][5], 255);
	PlayerTextDrawTextSize(playerid,tPlayerHUD[playerid][5], 35.000000, 40.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][5], 18970);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][5], 0.000000, 0.000000, 90.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid,tPlayerHUD[playerid][5], true);
	//---
	tPlayerHUD[playerid][6] = CreatePlayerTextDraw(playerid,85.000000, 154.000000, "Lunettes");
	PlayerTextDrawBackgroundColor(playerid,tPlayerHUD[playerid][6], 51);
	PlayerTextDrawFont(playerid,tPlayerHUD[playerid][6], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerHUD[playerid][6], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerHUD[playerid][6], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerHUD[playerid][6], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerHUD[playerid][6], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerHUD[playerid][6], 1);
	PlayerTextDrawUseBox(playerid,tPlayerHUD[playerid][6], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerHUD[playerid][6], 255);
	PlayerTextDrawTextSize(playerid,tPlayerHUD[playerid][6], 35.000000, 40.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][6], 19033);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][6], 0.000000, 0.000000, 90.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, tPlayerHUD[playerid][6], true);
	//---
	tPlayerHUD[playerid][7] = CreatePlayerTextDraw(playerid,155.000000, 154.000000, "Masque");
	PlayerTextDrawBackgroundColor(playerid,tPlayerHUD[playerid][7], 51);
	PlayerTextDrawFont(playerid,tPlayerHUD[playerid][7], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerHUD[playerid][7], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerHUD[playerid][7], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerHUD[playerid][7], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerHUD[playerid][7], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerHUD[playerid][7], 1);
	PlayerTextDrawUseBox(playerid,tPlayerHUD[playerid][7], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerHUD[playerid][7], 255);
	PlayerTextDrawTextSize(playerid,tPlayerHUD[playerid][7], 35.000000, 40.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][7], 18913);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][7], 90.000000, 180.000000, 0.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, tPlayerHUD[playerid][7], true);
	//---
	tPlayerHUD[playerid][8] = CreatePlayerTextDraw(playerid,120.000000, 182.000000, "Torse");
	PlayerTextDrawBackgroundColor(playerid,tPlayerHUD[playerid][8], 51);
	PlayerTextDrawFont(playerid,tPlayerHUD[playerid][8], 5);
	PlayerTextDrawLetterSize(playerid,tPlayerHUD[playerid][8], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerHUD[playerid][8], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerHUD[playerid][8], 0);
	PlayerTextDrawSetProportional(playerid,tPlayerHUD[playerid][8], 1);
	PlayerTextDrawSetShadow(playerid,tPlayerHUD[playerid][8], 1);
	PlayerTextDrawUseBox(playerid,tPlayerHUD[playerid][8], 1);
	PlayerTextDrawBoxColor(playerid,tPlayerHUD[playerid][8], 255);
	PlayerTextDrawTextSize(playerid,tPlayerHUD[playerid][8], 35.000000, 40.000000);
	PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][8], 19142);
	PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][8], 0.000000, 270.000000, 0.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, tPlayerHUD[playerid][8], true);
}

CreatePlayerInventory(playerid)
{
	new Float:xPosText[6] = {197.0, 238.0, 279.0, 320.0, 361.0, 402.0};
	new Float:yPosText[6] = {140.0, 191.0, 242.0, 293.0, 344.0, 395.0};
	for(new i = 0; i < 36; i ++)
	{
		tInventory[playerid][i][0] = CreatePlayerTextDraw(playerid, xPosText[i % 6], yPosText[floatround(floatdiv(i, 6), floatround_floor)], " ");
		PlayerTextDrawBackgroundColor(playerid,tInventory[playerid][i][0], 136);
		PlayerTextDrawFont(playerid,tInventory[playerid][i][0], 5);
		PlayerTextDrawLetterSize(playerid,tInventory[playerid][i][0], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,tInventory[playerid][i][0], -1);
		PlayerTextDrawSetOutline(playerid,tInventory[playerid][i][0], 0);
		PlayerTextDrawSetProportional(playerid,tInventory[playerid][i][0], 1);
		PlayerTextDrawSetShadow(playerid,tInventory[playerid][i][0], 1);
		PlayerTextDrawUseBox(playerid,tInventory[playerid][i][0], 1);
		PlayerTextDrawBoxColor(playerid,tInventory[playerid][i][0], 255);
		PlayerTextDrawTextSize(playerid,tInventory[playerid][i][0], 40.000000, 50.000000);
		PlayerTextDrawSetPreviewModel(playerid, tInventory[playerid][i][0], 19300);
		PlayerTextDrawSetPreviewRot(playerid, tInventory[playerid][i][0], 0.000000, 0.000000, 0.000000, 1.000000);
		PlayerTextDrawSetSelectable(playerid,tInventory[playerid][i][0], true);
		//---
		tInventory[playerid][i][1] = CreatePlayerTextDraw(playerid, xPosText[i % 6], yPosText[floatround(floatdiv(i, 6), floatround_floor)] + 38.0, " ");
		PlayerTextDrawBackgroundColor(playerid,tInventory[playerid][i][1], 136);
		PlayerTextDrawFont(playerid,tInventory[playerid][i][1], 2);
		PlayerTextDrawLetterSize(playerid,tInventory[playerid][i][1], 0.110000, 0.599999);
		PlayerTextDrawColor(playerid,tInventory[playerid][i][1], -1);
		PlayerTextDrawSetOutline(playerid,tInventory[playerid][i][1], 1);
		PlayerTextDrawSetProportional(playerid,tInventory[playerid][i][1], 1);
		PlayerTextDrawSetSelectable(playerid,tInventory[playerid][i][1], false);
	}
	tSlots[playerid] = CreatePlayerTextDraw(playerid,196.000000, 130.000000, "Slots: 36/36");
	PlayerTextDrawBackgroundColor(playerid,tSlots[playerid], 255);
	PlayerTextDrawFont(playerid,tSlots[playerid], 1);
	PlayerTextDrawLetterSize(playerid,tSlots[playerid], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid,tSlots[playerid], -1);
	PlayerTextDrawSetOutline(playerid,tSlots[playerid], 1);
	PlayerTextDrawSetProportional(playerid,tSlots[playerid], 1);
	PlayerTextDrawSetSelectable(playerid,tSlots[playerid], 0);
}

CreatePlayerSafe(playerid)
{
	new Float:xPosText[6] = {197.0, 238.0, 279.0, 320.0, 361.0, 402.0};
	new Float:yPosText[2] = {28.0, 79.0};
	for(new i = 0; i < 12; i ++)
	{
		tSafe[playerid][i][0] = CreatePlayerTextDraw(playerid, xPosText[i % 6], yPosText[floatround(floatdiv(i, 6), floatround_floor)], " ");
		PlayerTextDrawBackgroundColor(playerid,tSafe[playerid][i][0], 136);
		PlayerTextDrawFont(playerid,tSafe[playerid][i][0], 5);
		PlayerTextDrawLetterSize(playerid,tSafe[playerid][i][0], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,tSafe[playerid][i][0], -1);
		PlayerTextDrawSetOutline(playerid,tSafe[playerid][i][0], 0);
		PlayerTextDrawSetProportional(playerid,tSafe[playerid][i][0], 1);
		PlayerTextDrawSetShadow(playerid,tSafe[playerid][i][0], 1);
		PlayerTextDrawUseBox(playerid,tSafe[playerid][i][0], 1);
		PlayerTextDrawBoxColor(playerid,tSafe[playerid][i][0], 255);
		PlayerTextDrawTextSize(playerid,tSafe[playerid][i][0], 40.000000, 50.000000);
		PlayerTextDrawSetPreviewModel(playerid, tSafe[playerid][i][0], 19300);
		PlayerTextDrawSetPreviewRot(playerid, tSafe[playerid][i][0], 0.000000, 0.000000, 0.000000, 1.000000);
		PlayerTextDrawSetSelectable(playerid,tSafe[playerid][i][0], true);
		//---
		tSafe[playerid][i][1] = CreatePlayerTextDraw(playerid, xPosText[i % 6], yPosText[floatround(floatdiv(i, 6), floatround_floor)] + 38.0, " ");
		PlayerTextDrawBackgroundColor(playerid,tSafe[playerid][i][1], 136);
		PlayerTextDrawFont(playerid,tSafe[playerid][i][1], 2);
		PlayerTextDrawLetterSize(playerid,tSafe[playerid][i][1], 0.110000, 0.599999);
		PlayerTextDrawColor(playerid,tSafe[playerid][i][1], -1);
		PlayerTextDrawSetOutline(playerid,tSafe[playerid][i][1], 1);
		PlayerTextDrawSetProportional(playerid,tSafe[playerid][i][1], 1);
		PlayerTextDrawSetSelectable(playerid,tSafe[playerid][i][1], false);
	}
}

public ClosePlayerHUD(playerid)
{
	if(GetPlayerInventoryUse(playerid) != -1)
	{
	    #if defined KEEP_PLAYERTEXT

		for(new i = 0; i < 9; i ++) PlayerTextDrawHide(playerid, tPlayerHUD[playerid][i]);
		for(new i = 0; i < 36; i ++) PlayerTextDrawHide(playerid, tInventory[playerid][i][0]), PlayerTextDrawHide(playerid, tInventory[playerid][i][1]);
		PlayerTextDrawHide(playerid, tSlots[playerid]);
		TextDrawHideForPlayer(playerid, tFondHUD[0]);
		TextDrawHideForPlayer(playerid, tFondHUD[1]);
		TextDrawHideForPlayer(playerid, tFondHUD[2]);
		TextDrawHideForPlayer(playerid, tFondHUD[4]);
		TextDrawHideForPlayer(playerid, tHelp);
		TextDrawHideForPlayer(playerid, tCompetence);
		TextDrawHideForPlayer(playerid, tGroup);
		TextDrawHideForPlayer(playerid, tOptions);
		for(new i = 0; i < 12; i ++) PlayerTextDrawHide(playerid, tSafe[playerid][i][0]), PlayerTextDrawHide(playerid, tSafe[playerid][i][1]);
		if(GetPlayerVehicleUse(playerid) != -1)
		{
			SetVehicleTrunkState(dVehicleInfos[GetPlayerVehicleUse(playerid)][dVehicleID], false);
			SetPlayerVehicleUse(playerid, -1);
			TextDrawHideForPlayer(playerid, tFondHUD[3]);
			for(new i = 0; i < 5; i ++) PlayerTextDrawHide(playerid, tPlayerVehHUD[playerid][i]);
			for(new i = 0; i < 6; i ++) PlayerTextDrawHide(playerid, tVehicleTrunk[playerid][i][0]), PlayerTextDrawHide(playerid, tVehicleTrunk[playerid][i][1]);
		}
	    #else

		for(new i = 0; i < 9; i ++) PlayerTextDrawHide(playerid, tPlayerHUD[playerid][i]), PlayerTextDrawDestroy(playerid, tPlayerHUD[playerid][i]), tPlayerHUD[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
		for(new i = 0; i < 36; i ++) PlayerTextDrawHide(playerid, tInventory[playerid][i][0]), PlayerTextDrawHide(playerid, tInventory[playerid][i][1]), PlayerTextDrawDestroy(playerid, tInventory[playerid][i][0]), PlayerTextDrawDestroy(playerid, tInventory[playerid][i][1]), tInventory[playerid][i][0] = PlayerText:INVALID_TEXT_DRAW, tInventory[playerid][i][1] = PlayerText:INVALID_TEXT_DRAW;
		PlayerTextDrawHide(playerid, tSlots[playerid]), PlayerTextDrawDestroy(playerid, tSlots[playerid]), tSlots[playerid] = PlayerText:INVALID_TEXT_DRAW;
		TextDrawHideForPlayer(playerid, tFondHUD[0]);
		TextDrawHideForPlayer(playerid, tFondHUD[1]);
		TextDrawHideForPlayer(playerid, tFondHUD[2]);
		TextDrawHideForPlayer(playerid, tFondHUD[4]);
		TextDrawHideForPlayer(playerid, tHelp);
		TextDrawHideForPlayer(playerid, tCompetence);
		TextDrawHideForPlayer(playerid, tGroup);
		TextDrawHideForPlayer(playerid, tOptions);
		if(GetPlayerSafeUse(playerid) != -1)
		{
			for(new i = 0; i < 12; i ++) for(new j = 0; j < 2; j ++) PlayerTextDrawHide(playerid, tSafe[playerid][i][j]), PlayerTextDrawDestroy(playerid, tSafe[playerid][i][j]), tSafe[playerid][i][j] = PlayerText:INVALID_TEXT_DRAW;
		    SetPlayerSafeUse(playerid, -1);
		}
		if(GetPlayerVehicleUse(playerid) != -1)
		{
			SetVehicleTrunkState(dVehicleInfos[GetPlayerVehicleUse(playerid)][dVehicleID], false);
			SetPlayerVehicleUse(playerid, -1);
			TextDrawHideForPlayer(playerid, tFondHUD[3]);
			for(new i = 0; i < 5; i ++) PlayerTextDrawHide(playerid, tPlayerVehHUD[playerid][i]), PlayerTextDrawDestroy(playerid, tPlayerVehHUD[playerid][i]), tPlayerVehHUD[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
			for(new i = 0; i < 6; i ++) PlayerTextDrawHide(playerid, tVehicleTrunk[playerid][i][0]), PlayerTextDrawHide(playerid, tVehicleTrunk[playerid][i][1]), PlayerTextDrawDestroy(playerid, tVehicleTrunk[playerid][i][0]), PlayerTextDrawDestroy(playerid, tVehicleTrunk[playerid][i][1]), tVehicleTrunk[playerid][i][0] = PlayerText:INVALID_TEXT_DRAW, tVehicleTrunk[playerid][i][1] = PlayerText:INVALID_TEXT_DRAW;
		}

	    #endif
        SetPlayerInventoryUse(playerid, -1);
	}
}

CreatePlayerInventoryText(playerid)
{
	tInventObjet[playerid][0] = CreatePlayerTextDraw(playerid, 570.000000, 40.000000, "Object");//Textdraw avec l'image de l'objet
	PlayerTextDrawBackgroundColor(playerid, tInventObjet[playerid][0], 255);
	PlayerTextDrawFont(playerid, tInventObjet[playerid][0], 5);
	PlayerTextDrawLetterSize(playerid, tInventObjet[playerid][0], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, tInventObjet[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, tInventObjet[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, tInventObjet[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, tInventObjet[playerid][0], 1);
	PlayerTextDrawUseBox(playerid, tInventObjet[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, tInventObjet[playerid][0], 255);
	PlayerTextDrawTextSize(playerid, tInventObjet[playerid][0], 60.000000, 70.000000);
	PlayerTextDrawSetPreviewModel(playerid, tInventObjet[playerid][0], 19300);
	PlayerTextDrawSetPreviewRot(playerid, tInventObjet[playerid][0], 0.000000, 0.000000, 0.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, tInventObjet[playerid][0], true);
	//---
	tInventObjet[playerid][1] = CreatePlayerTextDraw(playerid, 570.000000, 100.000000, "Nothing");//Textdraw avec le nom de l'objet
	PlayerTextDrawBackgroundColor(playerid, tInventObjet[playerid][1], 255);
	PlayerTextDrawFont(playerid, tInventObjet[playerid][1], 2);
	PlayerTextDrawLetterSize(playerid, tInventObjet[playerid][1], 0.190000, 1.000000);
	PlayerTextDrawColor(playerid, tInventObjet[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, tInventObjet[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, tInventObjet[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, tInventObjet[playerid][1], 1);
	PlayerTextDrawSetSelectable(playerid, tInventObjet[playerid][1], 0);
}

CreatePlayerInfoTexts(playerid)
{
	tPlayerInfos[playerid][0] = CreatePlayerTextDraw(playerid, 495.000000, 8.000000, "~n~");//Barre de faim
	PlayerTextDrawBackgroundColor(playerid, tPlayerInfos[playerid][0], 255);
	PlayerTextDrawFont(playerid, tPlayerInfos[playerid][0], 1);
	PlayerTextDrawLetterSize(playerid, tPlayerInfos[playerid][0], 0.500000, 0.499998);
	PlayerTextDrawColor(playerid, tPlayerInfos[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, tPlayerInfos[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, tPlayerInfos[playerid][0], 1);
	PlayerTextDrawUseBox(playerid, tPlayerInfos[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][0], 8388863);
	PlayerTextDrawTextSize(playerid, tPlayerInfos[playerid][0], 550.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid, tPlayerInfos[playerid][0], 0);
	//---
	tPlayerInfos[playerid][1] = CreatePlayerTextDraw(playerid, 495.000000, 26.000000, "~n~");//Barre de soif
	PlayerTextDrawBackgroundColor(playerid, tPlayerInfos[playerid][1], 255);
	PlayerTextDrawFont(playerid, tPlayerInfos[playerid][1], 1);
	PlayerTextDrawLetterSize(playerid, tPlayerInfos[playerid][1], 0.500000, 0.499998);
	PlayerTextDrawColor(playerid, tPlayerInfos[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, tPlayerInfos[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, tPlayerInfos[playerid][1], 1);
	PlayerTextDrawUseBox(playerid, tPlayerInfos[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][1], 8388863);
	PlayerTextDrawTextSize(playerid, tPlayerInfos[playerid][1], 550.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid, tPlayerInfos[playerid][1], 0);
	//---
	tPlayerInfos[playerid][2] = CreatePlayerTextDraw(playerid, 495.000000, 45.000000, "~n~");//Barre de sommeil
	PlayerTextDrawBackgroundColor(playerid, tPlayerInfos[playerid][2], 255);
	PlayerTextDrawFont(playerid, tPlayerInfos[playerid][2], 1);
	PlayerTextDrawLetterSize(playerid, tPlayerInfos[playerid][2], 0.500000, 0.499998);
	PlayerTextDrawColor(playerid, tPlayerInfos[playerid][2], -1);
	PlayerTextDrawSetOutline(playerid, tPlayerInfos[playerid][2], 1);
	PlayerTextDrawSetProportional(playerid, tPlayerInfos[playerid][2], 1);
	PlayerTextDrawUseBox(playerid, tPlayerInfos[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][2], 8388863);
	PlayerTextDrawTextSize(playerid, tPlayerInfos[playerid][2], 550.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid, tPlayerInfos[playerid][2], 0);
	//---
	tPlayerInfos[playerid][3] = CreatePlayerTextDraw(playerid, 552.000000, 60.000000, "Ouro: 1000.00");//Le textdraw qui dit combien on a d'or
	PlayerTextDrawAlignment(playerid, tPlayerInfos[playerid][3], 3);
	PlayerTextDrawBackgroundColor(playerid, tPlayerInfos[playerid][3], 255);
	PlayerTextDrawFont(playerid, tPlayerInfos[playerid][3], 2);
	PlayerTextDrawLetterSize(playerid, tPlayerInfos[playerid][3], 0.180000, 1.000000);
	PlayerTextDrawColor(playerid, tPlayerInfos[playerid][3], -2686721);
	PlayerTextDrawSetOutline(playerid, tPlayerInfos[playerid][3], 1);
	PlayerTextDrawSetProportional(playerid, tPlayerInfos[playerid][3], 1);
	PlayerTextDrawSetSelectable(playerid, tPlayerInfos[playerid][3], 0);
	//---
	tPlayerInfos[playerid][4] = CreatePlayerTextDraw(playerid, 495.000000, 80.000000, "~n~");//Barre de vie
	PlayerTextDrawBackgroundColor(playerid, tPlayerInfos[playerid][4], 255);
	PlayerTextDrawFont(playerid, tPlayerInfos[playerid][4], 1);
	PlayerTextDrawLetterSize(playerid, tPlayerInfos[playerid][4], 0.500000, 0.499998);
	PlayerTextDrawColor(playerid, tPlayerInfos[playerid][4], -1);
	PlayerTextDrawSetOutline(playerid, tPlayerInfos[playerid][4], 1);
	PlayerTextDrawSetProportional(playerid, tPlayerInfos[playerid][4], 1);
	PlayerTextDrawUseBox(playerid, tPlayerInfos[playerid][4], 1);
	PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][4], -872414977);
	PlayerTextDrawTextSize(playerid, tPlayerInfos[playerid][4], 550.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid, tPlayerInfos[playerid][4], 0);
	//---
	tPlayerInfos[playerid][5] = CreatePlayerTextDraw(playerid, 495.000000, 98.000000, "~n~");//Barre d'armure
	PlayerTextDrawBackgroundColor(playerid, tPlayerInfos[playerid][5], 255);
	PlayerTextDrawFont(playerid, tPlayerInfos[playerid][5], 1);
	PlayerTextDrawLetterSize(playerid, tPlayerInfos[playerid][5], 0.500000, 0.499998);
	PlayerTextDrawColor(playerid, tPlayerInfos[playerid][5], -1);
	PlayerTextDrawSetOutline(playerid, tPlayerInfos[playerid][5], 1);
	PlayerTextDrawSetProportional(playerid, tPlayerInfos[playerid][5], 1);
	PlayerTextDrawUseBox(playerid, tPlayerInfos[playerid][5], 1);
	PlayerTextDrawBoxColor(playerid, tPlayerInfos[playerid][5], -1);
	PlayerTextDrawTextSize(playerid, tPlayerInfos[playerid][5], 550.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid, tPlayerInfos[playerid][5], 0);
	//---
	tPlayerInfos[playerid][6] = CreatePlayerTextDraw(playerid, 595.000000, 17.000000, "Temperatura:~n~36.8");//Température
	PlayerTextDrawAlignment(playerid, tPlayerInfos[playerid][6], 2);
	PlayerTextDrawBackgroundColor(playerid, tPlayerInfos[playerid][6], 255);
	PlayerTextDrawFont(playerid, tPlayerInfos[playerid][6], 2);
	PlayerTextDrawLetterSize(playerid, tPlayerInfos[playerid][6], 0.200000, 1.000000);
	PlayerTextDrawColor(playerid, tPlayerInfos[playerid][6], 65535);
	PlayerTextDrawSetOutline(playerid, tPlayerInfos[playerid][6], 1);
	PlayerTextDrawSetProportional(playerid, tPlayerInfos[playerid][6], 1);
	PlayerTextDrawSetSelectable(playerid, tPlayerInfos[playerid][6], 0);
	//---
	tPlayerInfos[playerid][7] = CreatePlayerTextDraw(playerid,620.000000, 420.000000, "Fuel:~n~100 l");
	PlayerTextDrawAlignment(playerid,tPlayerInfos[playerid][7], 3);
	PlayerTextDrawBackgroundColor(playerid,tPlayerInfos[playerid][7], 255);
	PlayerTextDrawFont(playerid,tPlayerInfos[playerid][7], 3);
	PlayerTextDrawLetterSize(playerid,tPlayerInfos[playerid][7], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid,tPlayerInfos[playerid][7], -1);
	PlayerTextDrawSetOutline(playerid,tPlayerInfos[playerid][7], 1);
	PlayerTextDrawSetProportional(playerid,tPlayerInfos[playerid][7], 1);
	PlayerTextDrawSetSelectable(playerid,tPlayerInfos[playerid][7], 0);
	//---
	//------
	//---
	tLevel[playerid][0] = CreatePlayerTextDraw(playerid, 2.000000, 423.000000, "Level 0");
	PlayerTextDrawBackgroundColor(playerid, tLevel[playerid][0], 255);
	PlayerTextDrawFont(playerid, tLevel[playerid][0], 2);
	PlayerTextDrawLetterSize(playerid, tLevel[playerid][0], 0.200000, 1.000000);
	PlayerTextDrawColor(playerid, tLevel[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, tLevel[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, tLevel[playerid][0], 1);
	PlayerTextDrawSetSelectable(playerid, tLevel[playerid][0], 0);
	//---
	tLevel[playerid][1] = CreatePlayerTextDraw(playerid, 3.000000, 434.000000, "~n~");
	PlayerTextDrawBackgroundColor(playerid, tLevel[playerid][1], 16777215);
	PlayerTextDrawFont(playerid, tLevel[playerid][1], 1);
	PlayerTextDrawLetterSize(playerid, tLevel[playerid][1], 0.500000, 0.599999);
	PlayerTextDrawColor(playerid, tLevel[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, tLevel[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, tLevel[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, tLevel[playerid][1], 1);
	PlayerTextDrawUseBox(playerid, tLevel[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid, tLevel[playerid][1], -2147483393);
	PlayerTextDrawTextSize(playerid, tLevel[playerid][1], 138.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid, tLevel[playerid][1], 0);
	//---
	tLevel[playerid][2] = CreatePlayerTextDraw(playerid, 3.000000, 445.000000, "~n~");
	PlayerTextDrawBackgroundColor(playerid, tLevel[playerid][2], 16777215);
	PlayerTextDrawFont(playerid, tLevel[playerid][2], 1);
	PlayerTextDrawLetterSize(playerid, tLevel[playerid][2], 0.500000, 0.000000);
	PlayerTextDrawColor(playerid, tLevel[playerid][2], -1);
	PlayerTextDrawSetOutline(playerid, tLevel[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid, tLevel[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, tLevel[playerid][2], 1);
	PlayerTextDrawUseBox(playerid, tLevel[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid, tLevel[playerid][2], -872414977);
	PlayerTextDrawTextSize(playerid, tLevel[playerid][2], 138.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid, tLevel[playerid][2], 0);
}

UpdatePlayerWeaponTexts(playerid, slotid)
{
	if(slotid > GetPlayerBagSize(playerid, false))
	{
	    PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 19300);
		PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 0.0, 0.0, 1.0);
		PlayerTextDrawSetSelectable(playerid, tPlayerHUD[playerid][slotid], false);
		PlayerTextDrawHide(playerid, tPlayerHUD[playerid][slotid]);
	}
	else
	{
		new dWeapon = GetPlayerWeaponEx(playerid, slotid);
        
		switch(dWeapon)
		{
		    case 0: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 19300), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 0.0, 0.0, 1.0);
		    case 3: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 334), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 1.5);
		    case 4: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 335), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 1.0);
		    case 5: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 336), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		    case 8: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 339), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		    case 9: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 341), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 0.0, 60.0, 2.0);
		    case 22: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 346), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 0.0, 0.0, 1.0);
		    case 23: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 347), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 0.0, 0.0, 1.25);
		    case 24: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 348), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 0.0, 0.0, 1.25);
		    case 25: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 349), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		    case 26: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 350), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		    case 27: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 351), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		    case 28: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 352), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 0.0, 0.0, 1.0);
		    case 29: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 353), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		    case 30: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 355), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		    case 31: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 356), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		    case 32: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 372), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 0.0, 0.0, 1.0);
		    case 33: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 357), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		    case 34: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 358), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		    case 38: PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][slotid], 362), PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][slotid], 0.0, 315.0, 0.0, 2.0);
		}
		if(dWeapon != 0) PlayerTextDrawSetSelectable(playerid, tPlayerHUD[playerid][slotid], true);
		else PlayerTextDrawSetSelectable(playerid, tPlayerHUD[playerid][slotid], false);
		PlayerTextDrawShow(playerid, tPlayerHUD[playerid][slotid]);
	}
}

UpdateVehicleHUD(playerid)
{
	new vehicleid = GetPlayerVehicleUse(playerid);
	if(vehicleid != -1)
	{
 		PlayerTextDrawColor(playerid, tPlayerVehHUD[playerid][0], (dVehicleInfos[vehicleid][bEngine]) ? 0xFFFFFFFF : 0xCC0000FF);
		if(!IsVehicleBicycle(dVehicleInfos[vehicleid][dVehicleID])) PlayerTextDrawShow(playerid, tPlayerVehHUD[playerid][0]);
	    if(dVehicleInfos[vehicleid][dWheels] != 0)
	    {
	        for(new i = 0; i < dVehicleInfos[vehicleid][dWheels]; i ++)
	        {
	            PlayerTextDrawColor(playerid, tPlayerVehHUD[playerid][i + 1], (dVehicleInfos[vehicleid][bWheel][i]) ? 0xFFFFFFFF : 0xCC0000FF);
				PlayerTextDrawShow(playerid, tPlayerVehHUD[playerid][i + 1]);
	        }
	    }
	}
}

UpdatePlayerInventorySlots(playerid)
{
	new dObjects[2];
	if(GetPlayerInventoryUse(playerid) != -1)
	{
		new string[128];
		new sBackPacks[5][] =
		{
		    "Patrol Pack", "Czech Vest", "Assault Pack", "Alice Pack", "Coyote Backpack"
		};
		dObjects[1] = GetPlayerBagSize(playerid, .objects = true);
		for(new i = 0; i < dObjects[1]; i ++) if(GetPlayerSlotObject(playerid, i + 1)/*pPlayerInfos[playerid][BagObject][i]*/ != 0) dObjects[0] ++;
		format(string, sizeof(string), "%s - Slots: %d/%d", sBackPacks[GetPlayerBag(playerid) - 1], dObjects[0], dObjects[1]);
		PlayerTextDrawSetString(playerid, tSlots[playerid], string);
		PlayerTextDrawShow(playerid, tSlots[playerid]);
	}
	return dObjects[0];
}

UpdateSafe(playerid, safeid, slotid, objectid)//Fonction pour update une case du coffre fort
{
	if(safeid != -1)
	{
		SetPlayerTextDrawObject(playerid, objectid, true, tSafe[playerid][slotid][0], tSafe[playerid][slotid][1]);
		PlayerTextDrawShow(playerid, tSafe[playerid][slotid][0]);
		PlayerTextDrawShow(playerid, tSafe[playerid][slotid][1]);
	}
}

stock SetPlayerTextDrawObject(playerid, objectid, bool:label, PlayerText:imageTextDraw, PlayerText:labelTextDraw = _:INVALID_TEXT_DRAW)
{
	new Float:x, Float:y, Float:z, Float:zoom;
	GetObjectRotation(objectid, x, y, z, zoom);

	PlayerTextDrawSetPreviewModel(playerid, imageTextDraw, GetObjectModelID(objectid));
	PlayerTextDrawSetPreviewRot(playerid, imageTextDraw, x, y, z, zoom);
	PlayerTextDrawSetSelectable(playerid, imageTextDraw, true);
	//---
	if(label && labelTextDraw != _:INVALID_TEXT_DRAW) 
	{
		PlayerTextDrawSetString(playerid, labelTextDraw, GetObjectModelName(objectid, GetPlayerLanguage(playerid)));
	}
}

public UpdateHealthBar(playerid, health, maxhealth, const nameen[], const namefr[], const namees[], const namepg[], const nameit[], const namede[])
{
	static bool:bDisplayed[MAX_PLAYERS];
	static dBarTimer[MAX_PLAYERS] = {-1, ...};
	if(maxhealth == -1)
	{
		TextDrawHideForPlayer(playerid, tBossBar);
		PlayerTextDrawHide(playerid, tBossHealth[playerid][0]);
		PlayerTextDrawDestroy(playerid, tBossHealth[playerid][0]);
		PlayerTextDrawHide(playerid, tBossHealth[playerid][1]);
		PlayerTextDrawDestroy(playerid, tBossHealth[playerid][1]);
		tBossHealth[playerid][0] = PlayerText:INVALID_TEXT_DRAW;
		tBossHealth[playerid][1] = PlayerText:INVALID_TEXT_DRAW;
		bDisplayed[playerid] = false;
		if(dBarTimer[playerid] != -1)
		{
		    KillTimer(dBarTimer[playerid]);
		    dBarTimer[playerid] = -1;
		}
	}
	else
	{
		new sName[32];
		switch(GetPlayerLanguage(playerid))
		{
			case LANGUAGE_EN: strcpy(sName, nameen);
			case LANGUAGE_FR: strcpy(sName, namefr);
			case LANGUAGE_ES: strcpy(sName, namees);
			case LANGUAGE_PG: strcpy(sName, namepg);
			case LANGUAGE_IT: strcpy(sName, nameit);
			case LANGUAGE_DE: strcpy(sName, namede);
		}
		new dPercent = floatround(floatdiv(health, maxhealth) * 100, floatround_round);
		if(dPercent < 0) dPercent = 0;
		//---
		if(!bDisplayed[playerid])
	    {
			tBossHealth[playerid][0] = CreatePlayerTextDraw(playerid, 480.000000, 170.000000, "~n~");
			PlayerTextDrawBackgroundColor(playerid, tBossHealth[playerid][0], 255);
			PlayerTextDrawFont(playerid, tBossHealth[playerid][0], 1);
			PlayerTextDrawLetterSize(playerid, tBossHealth[playerid][0], 0.500000, 1.000000);
			PlayerTextDrawColor(playerid, tBossHealth[playerid][0], -1);
			PlayerTextDrawSetOutline(playerid, tBossHealth[playerid][0], 0);
			PlayerTextDrawSetProportional(playerid, tBossHealth[playerid][0], 1);
			PlayerTextDrawSetShadow(playerid, tBossHealth[playerid][0], 1);
			PlayerTextDrawUseBox(playerid, tBossHealth[playerid][0], 1);
			PlayerTextDrawBoxColor(playerid, tBossHealth[playerid][0], -872415181);
			PlayerTextDrawTextSize(playerid, tBossHealth[playerid][0], 630.000000, 10.000000);
			PlayerTextDrawSetSelectable(playerid, tBossHealth[playerid][0], false);
			//---
			tBossHealth[playerid][1] = CreatePlayerTextDraw(playerid, 555.000000, 160.000000, "Mob Name");
			PlayerTextDrawAlignment(playerid, tBossHealth[playerid][1], 2);
			PlayerTextDrawBackgroundColor(playerid, tBossHealth[playerid][1], -335544269);
			PlayerTextDrawFont(playerid, tBossHealth[playerid][1], 2);
			PlayerTextDrawLetterSize(playerid, tBossHealth[playerid][1], 0.200000, 1.000000);
			PlayerTextDrawColor(playerid, tBossHealth[playerid][1], 255);
			PlayerTextDrawSetOutline(playerid, tBossHealth[playerid][1], 1);
			PlayerTextDrawSetProportional(playerid, tBossHealth[playerid][1], 1);
			PlayerTextDrawSetSelectable(playerid, tBossHealth[playerid][1], false);
			//---
			if(!IsHUDHiddenForPlayer(playerid)) TextDrawShowForPlayer(playerid, tBossBar);
	    }
	    else
	    {
	        KillTimer(dBarTimer[playerid]);
	    }
		PlayerTextDrawTextSize(playerid, tBossHealth[playerid][0], CalculateBarSize(dPercent, 150.0) + 480.0, 0.000000);//On change la taille du rectangle (La barre) pour montrer combien il reste
		if(100 >= dPercent >= 50) PlayerTextDrawBoxColor(playerid, tBossHealth[playerid][0], 0x00800033);//Et on change aussi la couleur
		else if(15 < dPercent < 50) PlayerTextDrawBoxColor(playerid, tBossHealth[playerid][0], 0xFFFF0033);
		else if(15 > dPercent) PlayerTextDrawBoxColor(playerid, tBossHealth[playerid][0], 0xCC000033);
		dBarTimer[playerid] = SetTimerEx("UpdateHealthBar", 5000, false, "iiissssss", playerid, -1, -1,  "N/A",  "N/A",  "N/A",  "N/A",  "N/A",  "N/A");
		//---
		PlayerTextDrawSetString(playerid, tBossHealth[playerid][1], sName);
		//---
		if(!IsHUDHiddenForPlayer(playerid))
		{
			PlayerTextDrawShow(playerid, tBossHealth[playerid][0]);
			PlayerTextDrawShow(playerid, tBossHealth[playerid][1]);
		}
		bDisplayed[playerid] = true;
	}
}

UpdatePlayerClothesTexts(playerid, slotid)
{
	switch(slotid)
	{
		case 0://SKIN
		{
		    PlayerTextDrawSetPreviewModel(playerid, tPlayerHUD[playerid][0], pPlayerInfos[playerid][pSkin]);
			PlayerTextDrawSetPreviewRot(playerid, tPlayerHUD[playerid][0], 0.0, 0.0, 0.0, 1.0);
			PlayerTextDrawShow(playerid, tPlayerHUD[playerid][0]);
		}
		case 1://CHAPEAU
		{
			SetPlayerTextDrawObject(playerid, pPlayerInfos[playerid][pChapeau], false, tPlayerHUD[playerid][5], INVALID_TEXT_DRAW);
			//---
			PlayerTextDrawSetSelectable(playerid, tPlayerHUD[playerid][5], (pPlayerInfos[playerid][pChapeau] != 0));
			//---
			PlayerTextDrawShow(playerid, tPlayerHUD[playerid][5]);
		}
		case 2://LUNETTES
		{
			SetPlayerTextDrawObject(playerid, pPlayerInfos[playerid][pLunettes], false, tPlayerHUD[playerid][6], INVALID_TEXT_DRAW);
			//---
			PlayerTextDrawSetSelectable(playerid, tPlayerHUD[playerid][6], (pPlayerInfos[playerid][pLunettes] != 0));
			//---
			PlayerTextDrawShow(playerid, tPlayerHUD[playerid][6]);
		}
		case 3://MASQUE
		{
			SetPlayerTextDrawObject(playerid, pPlayerInfos[playerid][pMasque], false, tPlayerHUD[playerid][7], INVALID_TEXT_DRAW);
			//---
			PlayerTextDrawSetSelectable(playerid, tPlayerHUD[playerid][7], (pPlayerInfos[playerid][pMasque] != 0));
			//---
			PlayerTextDrawShow(playerid, tPlayerHUD[playerid][7]);
		}
		case 4://TORSE
		{
			SetPlayerTextDrawObject(playerid, pPlayerInfos[playerid][pTorse], false, tPlayerHUD[playerid][8], INVALID_TEXT_DRAW);
			//---
			PlayerTextDrawSetSelectable(playerid, tPlayerHUD[playerid][8], (pPlayerInfos[playerid][pTorse] != 0));
			//---
			PlayerTextDrawShow(playerid, tPlayerHUD[playerid][8]);
		}
	}
}

UpdatePlayerHand(playerid, objectid)//Fonction pour update l'objet que le mec a en main
{
	new model = GetObjectModelID(objectid);
	new Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:zoom;
	GetObjectHandPos(objectid, x, y, z, rx, ry, rz, zoom);

	SetPlayerTextDrawObject(playerid, objectid, true, tInventObjet[playerid][0], tInventObjet[playerid][1]);

	if(!IsHUDHiddenForPlayer(playerid)) PlayerTextDrawShow(playerid, tInventObjet[playerid][0]);
	if(!IsHUDHiddenForPlayer(playerid)) PlayerTextDrawShow(playerid, tInventObjet[playerid][1]);
	//---
	SetPlayerSpecialAction(playerid, IsObjectHeavy(objectid) ? SPECIAL_ACTION_CARRY : SPECIAL_ACTION_NONE);
	//---
	if(model == 19317) SetPlayerAttachedObject(playerid, HAND_OBJECT_ID, 19317, 1, -0.0447, 0.2862, 0.1587, 180.0, 150.0, 326.85, 1.0, 1.0, 1.0);//Basse
	else S_SetPlayerAttachedObject(playerid, HAND_OBJECT_ID, model, 5, x, y, z, rx, ry, rz, zoom, zoom, zoom, -1, -1);
}

stock UpdatePlayerInventory(playerid, slotid, objectid)
{
	if(GetPlayerInventoryUse(playerid) != -1)
	{
		if(slotid >= GetPlayerBagSize(playerid, .objects = true))//Si plus grand que la capacité du sac
		{
			PlayerTextDrawSetPreviewModel(playerid, tInventory[playerid][slotid][0], 19300);
			PlayerTextDrawSetPreviewRot(playerid, tInventory[playerid][slotid][0], 0.0, 0.0, 0.0, 1.0);
			PlayerTextDrawSetSelectable(playerid,tInventory[playerid][slotid][0], false);
			PlayerTextDrawHide(playerid, tInventory[playerid][slotid][0]);
			//---
			PlayerTextDrawSetString(playerid, tInventory[playerid][slotid][1], "_");

			PlayerTextDrawHide(playerid, tInventory[playerid][slotid][1]);
		}
		else
		{
			SetPlayerTextDrawObject(playerid, objectid, true, tInventory[playerid][slotid][0], tInventory[playerid][slotid][1]);
			PlayerTextDrawShow(playerid, tInventory[playerid][slotid][0]);
			PlayerTextDrawShow(playerid, tInventory[playerid][slotid][1]);
		}
	}
}

stock UpdateVehicleInventory(playerid, vehicleid, slotid, objectid)//Fonction pour update une case du coffre d'une bagnole
{
	if(objectid == -1) objectid = 0;
  	if(vehicleid != INVALID_VEHICLE_ID)
	{
		if(slotid >= dVehicleInfos[vehicleid][dItem])
		{
			PlayerTextDrawSetPreviewModel(playerid, tVehicleTrunk[playerid][slotid][0], 19300);
			PlayerTextDrawSetPreviewRot(playerid, tVehicleTrunk[playerid][slotid][0], 0.0, 0.0, 0.0, 1.0);
			PlayerTextDrawSetSelectable(playerid,tVehicleTrunk[playerid][slotid][0], false);
			PlayerTextDrawHide(playerid, tVehicleTrunk[playerid][slotid][0]);

			PlayerTextDrawSetString(playerid, "_");
			PlayerTextDrawHide(playerid, tVehicleTrunk[playerid][slotid][1]);
		}
		else
		{
			SetPlayerTextDrawObject(playerid, objectid, label, tVehicleTrunk[playerid][slotid][0], tVehicleTrunk[playerid][slotid][1]);
			PlayerTextDrawShow(playerid, tVehicleTrunk[playerid][slotid][1]);
			PlayerTextDrawShow(playerid, tVehicleTrunk[playerid][slotid][1]);
		}
	}
}

stock CreateEngineerChoice(playerid)
{
	new Float:xPosText[6] = {197.0, 238.0, 279.0, 320.0, 361.0, 402.0};
	new Float:yPosText[3] = {140.0, 191.0, 242.0};
	for(new i = 0; i < 18; i ++)
	{
		tAtelierChoice[playerid][i] = CreatePlayerTextDraw(playerid, xPosText[i % 6], yPosText[floatround(floatdiv(i, 6), floatround_floor)], " ");
		PlayerTextDrawBackgroundColor(playerid, tAtelierChoice[playerid][i], 0x00000055);
		PlayerTextDrawFont(playerid, tAtelierChoice[playerid][i], 5);
		PlayerTextDrawLetterSize(playerid, tAtelierChoice[playerid][i], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid, tAtelierChoice[playerid][i], -1);
		PlayerTextDrawSetOutline(playerid, tAtelierChoice[playerid][i], 0);
		PlayerTextDrawSetProportional(playerid, tAtelierChoice[playerid][i], 1);
		PlayerTextDrawSetShadow(playerid, tAtelierChoice[playerid][i], 1);
		PlayerTextDrawUseBox(playerid, tAtelierChoice[playerid][i], 1);
		PlayerTextDrawBoxColor(playerid, tAtelierChoice[playerid][i], 0x00000000);
		PlayerTextDrawTextSize(playerid, tAtelierChoice[playerid][i], 40.000000, 50.000000);
		PlayerTextDrawSetPreviewModel(playerid, tAtelierChoice[playerid][i], 19300);
		PlayerTextDrawSetPreviewRot(playerid, tAtelierChoice[playerid][i], 0.000000, 0.000000, 0.000000, 1.000000);
		PlayerTextDrawSetSelectable(playerid, tAtelierChoice[playerid][i], false);
	}
}

stock CreateEngineerRecipe(playerid)
{
	//---Objets recette
	tAtelierItem[playerid][0] = CreatePlayerTextDraw(playerid, 207.000000, 330.000000, " ");
	tAtelierItem[playerid][1] = CreatePlayerTextDraw(playerid, 258.000000, 330.000000, " ");
	tAtelierItem[playerid][2] = CreatePlayerTextDraw(playerid, 309.000000, 330.000000, " ");
	tAtelierItem[playerid][3] = CreatePlayerTextDraw(playerid, 207.000000, 377.000000, " ");
	tAtelierItem[playerid][4] = CreatePlayerTextDraw(playerid, 258.000000, 377.000000, " ");
	tAtelierItem[playerid][5] = CreatePlayerTextDraw(playerid, 309.000000, 377.000000, " ");
	for(new i = 0; i < 6; i ++)
	{
		PlayerTextDrawBackgroundColor(playerid, tAtelierItem[playerid][i], 0x00000088);
		PlayerTextDrawFont(playerid, tAtelierItem[playerid][i], 5);
		PlayerTextDrawLetterSize(playerid, tAtelierItem[playerid][i], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid, tAtelierItem[playerid][i], -1);
		PlayerTextDrawSetOutline(playerid, tAtelierItem[playerid][i], 0);
		PlayerTextDrawSetProportional(playerid, tAtelierItem[playerid][i], 1);
		PlayerTextDrawSetShadow(playerid, tAtelierItem[playerid][i], 1);
		PlayerTextDrawUseBox(playerid, tAtelierItem[playerid][i], 1);
		PlayerTextDrawBoxColor(playerid, tAtelierItem[playerid][i], 0x00000055);
		PlayerTextDrawTextSize(playerid, tAtelierItem[playerid][i], 50.000000, 40.000000);
		PlayerTextDrawSetPreviewModel(playerid, tAtelierItem[playerid][i], 19300);
		PlayerTextDrawSetPreviewRot(playerid, tAtelierItem[playerid][i], 0.000000, 0.000000, 0.000000, 1.000000);
		PlayerTextDrawSetSelectable(playerid, tAtelierItem[playerid][i], true);
	}
	//---
	//---Prix
	tAtelierItem[playerid][6] = CreatePlayerTextDraw(playerid, 436.000000, 317.000000, "Gold: 0.0 g");
	PlayerTextDrawAlignment(playerid, tAtelierItem[playerid][6], 3);
	PlayerTextDrawBackgroundColor(playerid, tAtelierItem[playerid][6], 255);
	PlayerTextDrawFont(playerid, tAtelierItem[playerid][6], 2);
	PlayerTextDrawLetterSize(playerid, tAtelierItem[playerid][6], 0.200000, 1.000000);
	PlayerTextDrawColor(playerid, tAtelierItem[playerid][6], 0xFFD700FF);
	PlayerTextDrawSetOutline(playerid, tAtelierItem[playerid][6], 1);
	PlayerTextDrawSetProportional(playerid, tAtelierItem[playerid][6], 1);
	PlayerTextDrawSetSelectable(playerid, tAtelierItem[playerid][6], false);
	//---Icone de l'objet
	tAtelierItem[playerid][7] = CreatePlayerTextDraw(playerid, 367.000000, 330.000000, " ");
	PlayerTextDrawBackgroundColor(playerid, tAtelierItem[playerid][7], 255);
	PlayerTextDrawFont(playerid, tAtelierItem[playerid][7], 5);
	PlayerTextDrawLetterSize(playerid, tAtelierItem[playerid][7], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, tAtelierItem[playerid][7], -1);
	PlayerTextDrawSetOutline(playerid, tAtelierItem[playerid][7], 0);
	PlayerTextDrawSetProportional(playerid, tAtelierItem[playerid][7], 1);
	PlayerTextDrawSetShadow(playerid, tAtelierItem[playerid][7], 1);
	PlayerTextDrawUseBox(playerid, tAtelierItem[playerid][7], 1);
	PlayerTextDrawBoxColor(playerid, tAtelierItem[playerid][7], 255);
	PlayerTextDrawTextSize(playerid, tAtelierItem[playerid][7], 70.000000, 80.000000);
	PlayerTextDrawSetPreviewModel(playerid, tAtelierItem[playerid][7], 19300);
	PlayerTextDrawSetPreviewRot(playerid, tAtelierItem[playerid][7], 0.000000, 0.000000, 0.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, tAtelierItem[playerid][7], true);
	//---Nom de l'objet à fabriquer
	tAtelierItem[playerid][8] = CreatePlayerTextDraw(playerid, 368.000000, 400.000000, " ");
	PlayerTextDrawBackgroundColor(playerid, tAtelierItem[playerid][8], 255);
	PlayerTextDrawFont(playerid, tAtelierItem[playerid][8], 2);
	PlayerTextDrawLetterSize(playerid, tAtelierItem[playerid][8], 0.209999, 1.000000);
	PlayerTextDrawColor(playerid, tAtelierItem[playerid][8], -1);
	PlayerTextDrawSetOutline(playerid, tAtelierItem[playerid][8], 0);
	PlayerTextDrawSetProportional(playerid, tAtelierItem[playerid][8], 1);
	PlayerTextDrawSetShadow(playerid, tAtelierItem[playerid][8], 1);
	PlayerTextDrawSetSelectable(playerid, tAtelierItem[playerid][8], false);
	//---Chances de réussite
	tAtelierItem[playerid][9] = CreatePlayerTextDraw(playerid, 183.000000, 415.000000, "100 %~n~chances");
	PlayerTextDrawBackgroundColor(playerid, tAtelierItem[playerid][9], 255);
	PlayerTextDrawFont(playerid, tAtelierItem[playerid][9], 2);
	PlayerTextDrawLetterSize(playerid, tAtelierItem[playerid][9], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, tAtelierItem[playerid][9], -1);
	PlayerTextDrawSetOutline(playerid, tAtelierItem[playerid][9], 1);
	PlayerTextDrawSetProportional(playerid, tAtelierItem[playerid][9], 1);
	PlayerTextDrawSetSelectable(playerid, tAtelierItem[playerid][9], false);
}

stock ShowPlayerEngineerTexts(playerid) 
{
	TextDrawShowForPlayer(playerid, tFondAtelier[0]);
	TextDrawShowForPlayer(playerid, tFondAtelier[1]);
	TextDrawShowForPlayer(playerid, tFondAtelier[2]);
	//---
	TextDrawShowForPlayer(playerid, tBoutonsAtelier[0]);
	TextDrawShowForPlayer(playerid, tBoutonsAtelier[1]);
	TextDrawShowForPlayer(playerid, tBoutonsAtelier[2]);
	TextDrawShowForPlayer(playerid, tBoutonsAtelier[3]);
	//---
	TextDrawShowForPlayer(playerid, tBoutonFabriquer[GetPlayerLanguage(playerid)]);
	//---
	CreateEngineerChoice(playerid);
	CreateEngineerRecipe(playerid);
	//---
	tAtelierTitre[playerid] = CreatePlayerTextDraw(playerid, 320.000000, 82.000000, " ");
	PlayerTextDrawAlignment(playerid, tAtelierTitre[playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, tAtelierTitre[playerid], 255);
	PlayerTextDrawFont(playerid, tAtelierTitre[playerid], 0);
	PlayerTextDrawLetterSize(playerid, tAtelierTitre[playerid], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, tAtelierTitre[playerid], -1);
	PlayerTextDrawSetOutline(playerid, tAtelierTitre[playerid], 1);
	PlayerTextDrawSetProportional(playerid, tAtelierTitre[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, tAtelierTitre[playerid], 0);
}

stock HidePlayerEngineer(playerid)
{
	TextDrawHideForPlayer(playerid, tFondAtelier[0]);
	TextDrawHideForPlayer(playerid, tFondAtelier[1]);
	TextDrawHideForPlayer(playerid, tFondAtelier[2]);
	//---
	TextDrawHideForPlayer(playerid, tBoutonsAtelier[0]);
	TextDrawHideForPlayer(playerid, tBoutonsAtelier[1]);
	TextDrawHideForPlayer(playerid, tBoutonsAtelier[2]);
	TextDrawHideForPlayer(playerid, tBoutonsAtelier[3]);
	//---
	TextDrawHideForPlayer(playerid, tBoutonFabriquer[GetPlayerLanguage(playerid)]);
	//---
	for(new i = 0; i < 18; i ++)
	{
		PlayerTextDrawDestroy(playerid, tAtelierChoice[playerid][i]);
		tAtelierChoice[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
	}
	for(new i = 0; i < 10; i ++)
	{
		PlayerTextDrawDestroy(playerid, tAtelierItem[playerid][i]);
		tAtelierItem[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
	}
	//---
	PlayerTextDrawDestroy(playerid, tAtelierTitre[playerid]);
	tAtelierTitre[playerid] = PlayerText:INVALID_TEXT_DRAW;
}

stock ShowPlayerCrafting(playerid, type)
{
	TextDrawShowForPlayer(playerid, tFondAtelier[0]);
	TextDrawShowForPlayer(playerid, tFondAtelier[2]);
	//---
    switch(type)
    {
        case CRAFT_ENGINEER:
        {
        }
        case CRAFT_KITCHEN:
        {
            TextDrawShowForPlayer(playerid, tBoutonCuisiner[GetPlayerLanguage(playerid)]);
        }
        case CRAFT_HOUSE:
        {
	        TextDrawShowForPlayer(playerid, tBoutonBatir[GetPlayerLanguage(playerid)]);
        }
    }
}

stock HidePlayerCrafting(playerid, type)
{
	TextDrawHideForPlayer(playerid, tFondAtelier[0]);
	TextDrawHideForPlayer(playerid, tFondAtelier[2]);

	for(new i = 0; i < 18; i ++)
	{
		PlayerTextDrawDestroy(playerid, tAtelierChoice[playerid][i]);
		tAtelierChoice[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
	}
	for(new i = 0; i < 10; i ++)
	{
		PlayerTextDrawDestroy(playerid, tAtelierItem[playerid][i]);
		tAtelierItem[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
	}

	//---
    switch(type)
    {
        case CRAFT_ENGINEER:
        {
        }
        case CRAFT_KITCHEN:
        {
            TextDrawHideForPlayer(playerid, tBoutonCuisiner[GetPlayerLanguage(playerid)]);
        }
        case CRAFT_HOUSE:
        {
	        TextDrawHideForPlayer(playerid, tBoutonBatir[GetPlayerLanguage(playerid)]);
        }
    }
}

stock PlayerText:CreatePlayerSkinSelect(playerid) 
{
    new PlayerText:skinSelect = CreatePlayerTextDraw(playerid,260.000000, 140.000000, "Skin");
    PlayerTextDrawBackgroundColor(playerid, skinSelect, 0);
    PlayerTextDrawFont(playerid, skinSelect, 5);
    PlayerTextDrawLetterSize(playerid, skinSelect, 0.500000, 1.000000);
    PlayerTextDrawColor(playerid, skinSelect, -1);
    PlayerTextDrawSetOutline(playerid, skinSelect, 0);
    PlayerTextDrawSetProportional(playerid, skinSelect, 1);
    PlayerTextDrawSetShadow(playerid, skinSelect, 1);
    PlayerTextDrawUseBox(playerid, skinSelect, 1);
    PlayerTextDrawBoxColor(playerid, skinSelect, 255);
    PlayerTextDrawTextSize(playerid, skinSelect, 160.000000, 140.000000);
    PlayerTextDrawSetPreviewModel(playerid, skinSelect, 0);
    PlayerTextDrawSetPreviewRot(playerid, skinSelect, 0.000000, 0.000000, 0.000000, 1.000000);
    PlayerTextDrawSetSelectable(playerid, skinSelect, 0);

	return skinSelect;
}

stock GetPlayerRulesTime(playerid) 
{
    return IsValidPlayer(playerid) ? dNewCo[playerid] : 0;
}

stock SetPlayerRulesTime(playerid, time) 
{
    if(IsValidPlayer(playerid)) 
    {
        if(time <= 0) 
        {
            dNewCo[playerid] = 0;
             HidePlayerRules(i);
        }
        dNewCo[playerid] = time;
    }
}

stock ResetPlayerTextDraws(playerid)
{
	tInfoText[playerid] = PlayerText:INVALID_TEXT_DRAW;
	//---
	tGroupText[playerid] = PlayerText:INVALID_TEXT_DRAW;
	//---
	tInventObjet[playerid][0] = PlayerText:INVALID_TEXT_DRAW;
	tInventObjet[playerid][1] = PlayerText:INVALID_TEXT_DRAW;
	//---
	tPlayerSkin[playerid] = PlayerText:INVALID_TEXT_DRAW;
	tSlots[playerid] = PlayerText:INVALID_TEXT_DRAW;
	tAtelierTitre[playerid] = PlayerText:INVALID_TEXT_DRAW;
	for(new j = 0; j < 8; j ++) tHDVCats[playerid][j] = PlayerText:INVALID_TEXT_DRAW;
	for(new j = 0; j < 2; j ++) tHDVItem[playerid][j] = PlayerText:INVALID_TEXT_DRAW;
	for(new j = 0; j < 2; j ++) tBossHealth[playerid][j] = PlayerText:INVALID_TEXT_DRAW;
	for(new j = 0; j < 18; j ++) tAtelierChoice[playerid][j] = PlayerText:INVALID_TEXT_DRAW;
	for(new j = 0; j < 9; j ++) tAtelierItem[playerid][j] = PlayerText:INVALID_TEXT_DRAW;
	for(new j = 0; j < 8; j ++) tPlayerInfos[playerid][j] = PlayerText:INVALID_TEXT_DRAW;
	for(new j = 0; j < 3; j ++) tLevel[playerid][j] = PlayerText:INVALID_TEXT_DRAW;
	for(new j = 0; j < 9; j ++) tPlayerHUD[playerid][j] = PlayerText:INVALID_TEXT_DRAW;
	for(new j = 0; j < 5; j ++) tPlayerVehHUD[playerid][j] = PlayerText:INVALID_TEXT_DRAW;
	for(new j = 0; j < 6; j ++)
	{
		tVehicleTrunk[playerid][j][0] = PlayerText:INVALID_TEXT_DRAW;
		tVehicleTrunk[playerid][j][1] = PlayerText:INVALID_TEXT_DRAW;
	}
	for(new j = 0; j < 36; j ++)
	{
		tInventory[playerid][j][0] = PlayerText:INVALID_TEXT_DRAW;
		tInventory[playerid][j][1] = PlayerText:INVALID_TEXT_DRAW;
	}
}