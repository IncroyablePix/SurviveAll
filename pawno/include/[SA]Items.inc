#if defined ITEMS_INCLUDED

	#endinput

#endif

#define ITEMS_INCLUDED

//---INCLUDES---//
#include <a_samp>
#include <YSI\y_ini>
#include <[SA]Defines.inc>
#include <[SA]Functions.inc>
#include <[SA]TextDraws.inc>
#include <[SA]Dynamics.inc>
#include <[SA]Houses.inc>

//---DEFINES---//
#define MAX_ITEMS                       	(159)
#define MAX_SAFES                           (250)
#define MAX_GROUND_ITEMS                    (1500)
#define MAX_AUCTION_ITEMS                   (100)

#define IPATH                               "/Survive-All/General/Items.ini"
#define SPATH                               "/Survive-All/General/Safes.ini"
#define APATH                               "/Survive-All/General/Auctions.ini"

#define TYPE_NOSELL                         (0)
#define TYPE_TOOL                           (1)
#define TYPE_MEDIC                          (2)
#define TYPE_WEAP                           (3)
#define TYPE_OTHER                          (4)
#define TYPE_VEH                            (5)
#define TYPE_CLOTHE                         (6)
#define TYPE_FOOD                           (7)
#define TYPE_RSRC                           (8)

//---FORWARDS---//
forward LoadItems_data(name[], value[]);
forward LoadSafes_data(name[], value[]);
forward LoadAuctionHouse_data(name[],value[]);
forward bool:IsItemAroundPlayer(itemid, playerid, Float:radius);
forward bool:IsPlayerNearItem(playerid);
forward GetObjectName(playerid, objectid, language);

//---ENUMS---//
enum ObjectsInfos
{
	ObjectModelID,
	//---IMAGE DE L'INVENTAIRE
	Float:ObjectRotX,
	Float:ObjectRotY,
	Float:ObjectRotZ,
	Float:ObjectZoom,
	//---DANS LA MAIN
	Float:HandOffSetX,
	Float:HandOffSetY,
	Float:HandOffSetZ,
	Float:HandRotX,
	Float:HandRotY,
	Float:HandRotZ,
	Float:HandZoom,
	//---PAR TERRE
	Float:GroundRotX,
	Float:GroundRotY,
	Float:GroundRotZ,
	Float:GroundOffSetZ,
	//---PRIX REVENTE & HDV
	dSellPrice,
	dObjectType,
	bool:bHeavy,
	//---NOMS---//
	ObjectEnName[30],
	ObjectFrName[30],
	ObjectEsName[30],
	ObjectPgName[30],
	ObjectItName[30],
	ObjectDeName[30]
}

enum Items
{
	ItemID,
	ObjectID,
	bool:bAutoSpawn,
	Text3D:ObjectText,
	Float:xItem,
	Float:yItem,
	Float:zItem
}

enum SafeInfos
{
	oSafe[2],
	bool:bSafe,
	bool:bOpenSafe,
	sSafeCode[5],
	Float:xSafe,
	Float:ySafe,
	Float:zSafe,
	Float:aSafe,
	dItem[12]
}

enum ItemForSale
{
	dItemSale,//ID de l'objet
	dItemPrice,//Prix
	dSalesID,//
	sSalesman[MAX_PLAYER_NAME + 1],
	dTimeLeft
}

//---VARIABLES---//
new dSpawnedItems = 0;
new dItems[MAX_GROUND_ITEMS][Items];
new dSafeInfos[MAX_SAFES][SafeInfos];
new pAroundItems[MAX_PLAYERS][9][2];
new dUsingItem[MAX_PLAYERS] = {-1, ...};
new pUseInventory[MAX_PLAYERS] = {-1, ...};
new pVehicleInventory[MAX_PLAYERS] = {-1, ...};

new pHDVSale[MAX_PLAYERS][3];
new pHDV[MAX_PLAYERS][2];
new dAuctionParsingVar[50];

new dAuctionSellerTool[MAX_AUCTION_ITEMS][ItemForSale];
new dAuctionSellerMedic[MAX_AUCTION_ITEMS][ItemForSale];
new dAuctionSellerWeapon[MAX_AUCTION_ITEMS][ItemForSale];
new dAuctionSellerOther[MAX_AUCTION_ITEMS][ItemForSale];
new dAuctionSellerVehicle[MAX_AUCTION_ITEMS][ItemForSale];
new dAuctionSellerClothes[MAX_AUCTION_ITEMS][ItemForSale];
new dAuctionSellerFood[MAX_AUCTION_ITEMS][ItemForSale];
new dAuctionSellerRessource[MAX_AUCTION_ITEMS][ItemForSale];

static aObjects[MAX_ITEMS][ObjectsInfos] =
{   //ID OBJET	//AFFICHAGE             //DANS LA MAIN                          			//POSITION AU SOL   			//PRIX-HDV				//NOM OBJET
	{19300, 	0.0, 0.0, 0.0, 1.0, 	0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 					0.0, 0.0, 0.0, 0.0, 			0, TYPE_NOSELL,	false,	"Nothing", "Rien", "Nada", "Nada", "Niente", "Nichts"},//0
	{3082, 		0.0, 0.0, 0.0, 1.0, 	0.0867, 0.0127, -0.0767, 0.0, 0.0, 0.0, 0.5,		90.0, 0.0, 90.0, -0.714, 		5, TYPE_OTHER, false,	"Tent", "Tente", "Espagnol", "Portugais", "Tenda", "Zelt"},
	{11738, 	0.0, 0.0, 0.0, 1.0, 	0.2680, 0.0111, 0.0, 0.0, 270.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.953, 			3, TYPE_MEDIC, false,	"Medikit", "Medikit", "Medikit", "Medikit", "Medikit", "Medikit"},
	{334,		0.0, 315.0, 0.0, 1.5,	0.0036, 0.0111, 0.0, 0.0, 0.0, 0.0, 1.0,			90.0, 0.0, 100.0, -0.958,		3, TYPE_WEAP, false,	"Nitestick", "Matraque", "Espagnol", "Portugais", "Sfollagente", "Knüppel"},
	{335,		0.0, 315.0, 0.0, 1.0,	0.0036, 0.0111, 0.0, 0.0, 0.0, 0.0, 1.0,			90.0, 0.0, 100.0, -0.958,		3, TYPE_WEAP, false,	"Knife", "Couteau", "Espagnol", "Portugais", "Coltello", "Messer"},
	{336,		0.0, 315.0, 0.0, 2.0,	0.0036, 0.0056, -0.336, 0.0, 0.0, 0.0, 1.0,			90.0, 0.0, 100.0, -0.958,		3, TYPE_WEAP, false,	"Bat", "Batte", "Espagnol", "Portugais", "Italien", "Schläger"},
	{339,		0.0, 315.0, 0.0, 2.0,	0.0036, 0.0056, -0.336, 0.0, 0.0, 0.0, 1.0,			90.0, 0.0, 100.0, -0.958,		4, TYPE_WEAP, false,	"Katana", "Katana", "Espagnol", "Portugais", "Katana", "Schwert"},
	{341,		0.0, 0.0, 60.0, 2.0,	0.0036, -0.0053, -0.0065, 0.0, 0.0, 0.0, 1.0, 		90.0, 0.0, 100.0, -0.958,		5, TYPE_WEAP, false,	"Chainsaw", "Tronconneuse", "Espagnol", "Portugais", "Italien", "Motor-~n~säge"},
	{342,		0.0, 0.0, 60.0, 1.0,	0.0332, -0.0053, -0.0065, 0.0, 0.0, 0.0, 1.0,		0.0, 0.0, 100.0, -0.958,		5, TYPE_WEAP, false,	"Grenade", "Grenade", "Espagnol", "Portugais", "Italien", "Granate"},
	{344,		0.0, 0.0, 60.0, 1.0,	0.0332, -0.0053, -0.0065, 0.0, 0.0, 0.0, 1.0,		0.0, 0.0, 100.0, -0.792,		3, TYPE_WEAP, false,	"Molotovs", "Molotovs", "Molotovs", "Molotovs", "Molotovs", "Molotovs"},
	{346,		0.0, 0.0, 0.0, 1.0,		0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -1.026,		8, TYPE_WEAP, false,	"Pistol", "Pistolet", "Espagnol", "Portugais", "Italien", "Pistole"},//10
	{347,		0.0, 0.0, 0.0, 1.25,	0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -1.026,		8, TYPE_WEAP, false,	"Silenced pistol", "Silencieux", "Espagnol", "Portugais", "Italien", "Allemand"},
	{348,		0.0, 0.0, 0.0, 1.25,	0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -1.026,		9, TYPE_WEAP, false,	"Desert Eagle", "Desert Eagle", "Desert Eagle", "Desert Eagle", "Desert Eagle", "Desert Eagle"},
	{349,		0.0, 315.0, 0.0, 2.0,	0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -0.938,		10, TYPE_WEAP, false,	"Shotgun", "Fusil a pompe", "Espagnol", "Portugais", "Italien", "Allemand"},
	{350,		0.0, 315.0, 0.0, 2.0,	0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -0.926,		8, TYPE_WEAP, false,	"Sawed-off~n~shotgun", "Fusil a~n~canon scie", "Espagnol", "Portugais", "Italien", "Allemand"},
	{351,		0.0, 315.0, 0.0, 2.0,	0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -0.926,		10, TYPE_WEAP, false,	"Spas 12", "Spas 12", "Spas 12", "Spas 12", "Spaz 12", "Spas 12"},
	{352,		0.0, 0.0, 0.0, 1.0,		0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -1.026,		8, TYPE_WEAP, false,	"Uzi", "Uzi", "Uzi", "Uzi", "Uzi", "Uzi"},
	{353,		0.0, 315.0, 0.0, 2.0,	0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -0.926,		8, TYPE_WEAP, false,	"MP-5", "MP-5", "MP-5", "MP-5", "MP-5", "MP-5"},
	{355,		0.0, 315.0, 0.0, 2.0,	0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -0.926,		10, TYPE_WEAP, false,	"AK-47", "AK-47", "AK-47", "AK-47", "AK-47", "AK-47"},
	{356,		0.0, 315.0, 0.0, 2.0,	00.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -0.926,		10,	TYPE_WEAP, false,	"M4", "M4", "M4", "M4", "M4", "M4"},
	{372,		0.0, 0.0, 0.0, 1.0,		0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -1.026,		6, TYPE_WEAP, false,	"Tec-9", "Tec-9", "Tec-9", "Tec-9", "Tec-9", "Tec-9"},//20
	{357,		0.0, 315.0, 0.0, 2.0,	0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -0.926,		7, TYPE_WEAP, false,	"Countryrifle", "Fusil de~n~chasse", "Espagnol", "Portugais", "Italien", "Jagtgewehr"},
	{358,		0.0, 315.0, 0.0, 2.0,	0.1705, 0.0056, -0.1698, 0.0, 270.0, 0.0, 1.0,		90.0, 0.0, 100.0, -0.926,		8, TYPE_WEAP, false,	"Sniper", "Sniper", "Sniper", "Sniper", "Sniper", "Sniper"},
	{359,		0.0, 315.0, 0.0, 2.0,	0.0, 0.0, 0.0, 1.7664, 102.8639, 10.3771, 1.0,		90.0, 0.0, 100.0, -0.926,		10, TYPE_WEAP, true,	"Rocket-launcher", "Lance-roquettes", "Espagnol", "Portugais", "Italien", "Allemand"},
	{362,		0.0, 315.0, 0.0, 2.0,	-0.2784, 0.0432, -0.4341, 15.5, 315.0, 10.0, 1.0,	90.0, 0.0, 100.0, -0.926,		10, TYPE_WEAP, true,	"Minigun", "Minigun", "Minigun", "Minigun", "Minigun", "Minigun"},
	{2043,		315.0, 0.0, 90.0, 1.0,	0.0816, 0.0445, 0.1031, 90.0, 270.0, 0.0, 0.5,		0.0, 0.0, 90.0, -0.896,			2, TYPE_WEAP, false,	"7.62", "7.62", "7.62", "7.62", "7.62", "7.62"},
	{3016,		315.0, 0.0, 90.0, 1.0,	0.1495, 0.016, -0.0054, 90.0, 270.0, 0.0, 0.5,		0.0, 0.0, 100.0, -0.853,		2, TYPE_WEAP, false,	"9mm", "9mm", "9mm", "9mm", "9mm", "9mm"},
	{2038,		315.0, 0.0, 90.0, 1.0,	0.1495, 0.0041, -0.0385, 0.0, 270.0, 0.0, 1.0,		270.0, 0.0, 90.0, -0.923,		2, TYPE_WEAP, false,	".50ae", ".50ae", ".50ae", ".50ae", ".50ae", ".50ae"},
	{2041,		315.0, 0.0, 90.0, 1.0,	0.0816, 0.0445, -0.0058, 90.0, 270.0, 0.0, 0.5,		0.0, 0.0, 90.0, -0.896,			2, TYPE_WEAP, false,	"12 Gauge", "12 Gauge", "12 Gauge", "12 Gauge", "12 Gauge", "12 Gauge"},
	{2039,		0.0, 0.0, 135.0, 1.0,	0.0816, 0.0445, -0.0058, 0.0, 270.0, 0.0, 1.0,		0.0, 0.0, 90.0, -0.946,			2, TYPE_WEAP, false,	".222", ".222", ".222", ".222", ".222", ".222"},
	{1650, 		0.0, 0.0, 0.0, 1.0, 	0.1556, 0.0411, -0.0361, 0.0, 270.0, 200.0, 1.0,	90.0, 90.0, 0.0, -0.9275,		2, TYPE_VEH, false,		"Full gas can", "Bidon plein~n~d'essence", "Espagnol", "Portugais", "Italien", "Voller~n~Benzinkanister"},//30
	{1650, 		0.0, 0.0, 0.0, 1.0, 	0.1556, 0.0411, -0.0361, 0.0, 270.0, 200.0, 1.0,	90.0, 90.0, 0.0, -0.9275,		1, TYPE_VEH, false,		"Empty gas~n~can", "Bidon vide~n~d'essence", "Espagnol", "Portugais", "Italien", "Leerer~n~Benzinkanister"},
	{371, 		0.0, 0.0, 0.0, 1.5, 	0.36, 0.0, 0.0, 0.0, 270.0, 0.0, 1.0,				270.0, 90.0, 0.0, -0.75,		1, TYPE_OTHER, false,	"Czech Vest", "Czech Vest", "Czech Vest", "Czech Vest", "Czech Vest", "Czech Vest"},
	{3026, 		0.0, 0.0, 0.0, 1.0, 	0.5299, 0.0, 0.0, 0.0, 180.0, 0.0, 1.0,				270.0, 90.0, 0.0, -1.003,		2, TYPE_OTHER, false,	"Assault Pack", "Assault Pack", "Assault Pack", "Assault Pack", "Assault Pack", "Assault Pack"},
	{1310, 		0.0, 0.0, 0.0, 1.0, 	0.5531, 0.0, 0.0, 90.0, 270.0, 90.0, 1.0,			270.0, 90.0, 0.0, -0.875,		3, TYPE_OTHER, false,	"Alice Pack", "Alice Pack", "Alice Pack", "Alice Pack", "Alice Pack", "Alice Pack"},
	{19559, 	0.0, 0.0, 180.0, 1.0, 	0.1831, -0.1331, 0.0, 90.0, 270.0, 90.0, 1.0,		270.0, 90.0, 0.0, -1.008,		5, TYPE_OTHER, false,	"Coyote Pack", "Coyote Pack", "Coyote Pack", "Coyote Pack", "Coyote Pack", "Coyote Pack"},
	{11736,		0.0, 0.0, 0.0, 1.0,		0.2648, 0.0556, 0.0, 90.0, 0.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.973,			1, TYPE_MEDIC, false,	"Bandages", "Bandages", "Espagnol", "Portugais", "Italien", "Binden"},
	{2709,		0.0, 0.0, 0.0, 1.0,		0.0684, 0.0556, 0.0, 180.0, 0.0, 0.0, 0.5,			0.0, 0.0, 0.0, -0.973,			1, TYPE_MEDIC, false,	"Painkiller", "Anti-douleur", "Espagnol", "Portugais", "Italien", "Medikament"},
	{1580,		90.0, 0.0, 0.0, 1.0,	0.0119, 0.0072, 0.0, 270.0, 0.0, 270.0, 0.5,		0.0, 0.0, 0.0, -1.022,			1, TYPE_MEDIC, false,	"Bloodbag", "Poche de~n~sang", "Espagnol", "Portugais", "Italien", "Blutbeutel"},
	{1241,		0.0, 135.0, 180.0, 1.0,	0.0895, 0.058, 0.0, 165.0, 0.0, 132.0, 0.5,			0.0, 120.0, 0.0, -0.943,		2, TYPE_MEDIC, false,	"Cofein pill", "Cafeine", "Cafeina", "Portugais", "Italien", "Koffeïn"},
	//#if defined DOWNLOAD
	//{-1000,		0.0, 0.0, 0.0, 1.0, 	0.0528, 0.0483, 0.0688, 180.0, 0.0, 0.0, 0.7,		0.0, 0.0, 0.0, -1.0,			1, TYPE_OTHER, false,	"Empty bottle", "Bouteille~n~vide", "Espagnol", "Portugais", "Italien", "Leere~n~Flasche"},//40
	//#else
	{1484,		0.0, 0.0, 0.0, 1.0,		-0.0335, 0.0533, -0.0897, 190.0, 15.0, 328.0, 1.0,	350.0, 31.0, 6.0, -0.8101,		1, TYPE_OTHER, false,	"Empty bottle", "Bouteille~n~vide", "Espagnol", "Portugais", "Italien", "Leere~n~Flasche"},//40
	//#endif
	{18970,		0.0, 0.0, 90.0, 1.0,	0.229, 0.0, 0.0, 90.0, 0.0, 0.0, 1.0,				0.0, 10.0, 0.0, -0.961,			2, TYPE_CLOTHE, false,	"Leopard hat", "Chapeau~n~leopard", "Espagnol", "Portugais", "Italien", "Leopardhut"},
	{18973, 	0.0, 0.0, 90.0, 1.0,	0.229, 0.0, 0.0, 90.0, 0.0, 0.0, 1.0,				0.0, 10.0, 0.0, -0.961,			2, TYPE_CLOTHE,	false,	"Leopard hat", "Chapeau~n~leopard", "Espagnol", "Portugais", "Italien", "Leopardhut"},
	{18968, 	0.0, 0.0, 90.0, 1.0,	0.229, 0.0, 0.0, 90.0, 0.0, 0.0, 1.0,				0.0, 10.0, 0.0, -0.961,			2, TYPE_CLOTHE, false,	"Bob hat", "Chapeau~n~bob", "Espagnol", "Portugais", "Italien", "Bobhut"},
	{18971, 	0.0, 0.0, 90.0, 1.0,	0.229, 0.0, 0.0, 90.0, 0.0, 0.0, 1.0,				0.0, 10.0, 0.0, -0.961,			2, TYPE_CLOTHE,	false,	"Disco hat", "Chapeau~n~disco", "Espagnol", "Portugais", "Italien", "Discohut"},
	{19528,		90.0, 270.0, 0.0, 1.0,	0.2683, 0.0406, 0.0, 0.0, 90.0, 270.0, 1.0,			0.0, 270.0, 0.0, -0.961,		2, TYPE_CLOTHE, false,	"Witch hat", "Chapeau de~n~sorciere", "Espagnol", "Portugais", "Italien", "Hexenhut"},
	{18924,		85.0, 25.0, 45.0, 1.0,	0.1703, 0.0649, 0.0, 54.149, 270.0, 180.0, 1.0,		0.0, 270.0, 0.0, -0.961,		2, TYPE_CLOTHE,	false,	"Beret", "Beret", "Espagnol", "Portugais", "Italien", "Berett"},
	{19106, 	0.0, 270.0, 0.0, 1.0,	0.185, 0.0804, 0.0, 0.0, 0.0, 90.0, 1.0,			0.0, 270.0, 0.0, -0.961, 		5, TYPE_CLOTHE,	false,	"Soldier~n~helmet", "Casque~n~militaire", "Espagnol", "Portugais", "Italien", "Soldatenhelm"},
	{19107, 	0.0, 270.0, 0.0, 1.0,	0.185, 0.0804, 0.0, 0.0, 0.0, 90.0, 1.0,			0.0, 180.0, 0.0, -0.961, 		5, TYPE_CLOTHE,	false,	"Marine~n~helmet", "Casque~n~de marin", "Espagnol", "Portugais", "Italien", "Marinehelm"},
	{18927,		270.0, 0.0, 270.0, 1.0,	0.158, 0.0608, 0.0, 180.0, 0.0, 270.0, 1.0,			0.0, 270.0, 0.0, -0.95,			2, TYPE_CLOTHE,	false,	"Blue cap", "Casquette~n~bleue", "Espagnol", "Portugais", "Italien", "Blaue~n~Schirmmütze"},
	{18930,		270.0, 0.0, 270.0, 1.0,	0.158, 0.0608, 0.0, 180.0, 0.0, 270.0, 1.0,			0.0, 270.0, 0.0, -0.95,			2, TYPE_CLOTHE,	false,	"Red cap", "Casquette~n~rouge", "Espagnol", "Portugais", "Italien", "Rote~n~Schirmmütze"},//50
	{18949, 	270.0, 0.0, 270.0, 1.0,	0.1956, 0.0261, 0.0, 180.0, 90.0, 70.0, 1.0,		0.0, 270.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Green~n~bowler", "Chapeau~n~melon vert", "Espagnol", "Portugais", "Italien", "Grüne~n~melone"},
	{19006,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Red~n~Predators", "Predators~n~rouges", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19007,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Yellow~n~Predators", "Predators~n~jaunes", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19008,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Green~n~Predators", "Predators~n~vertes", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19009,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Blue~n~Predators", "Predators~n~bleues", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19022,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Black~n~Aviators", "Aviators~n~noires", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19023,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Blue~n~Aviators", "Aviators~n~bleues", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19024,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Purple~n~Aviators", "Aviators~n~mauves", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19025,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Pink~n~Aviators", "Aviators~n~roses", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19027,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Orange~n~Aviators", "Aviators~n~oranges", "Espagnol", "Portugais", "Italien", "Allemand"},//60
	{19028,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Yellow~n~Aviators", "Aviators~n~jaunes", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19033,		0.0, 0.0, 90.0, 1.0,	0.1719, 0.0548, 0.0, 67.5, 0.0, 270.0, 1.0,			90.0, 90.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Black~n~Wayfarers", "Wayfarers~n~noires", "Espagnol", "Portugais", "Italien", "Allemand"},
	{1098,		0.0, 0.0, 90.0, 1.0,	0.0572, 0.0685, 0.1781, 12.6294, 90.0, 90.0, 0.7,	0.0, 0.0, 90.0, -0.5079,		2, TYPE_VEH, true,		"Wheel", "Roue", "Espagnol", "Portugais", "Italien", "Rad"},
	{19917,		330.0, 0.0, 25.0, 1.0,	0.0652, -0.0213, 0.1633, 102.6, 180.0, 18.35, 0.5,	0.0, 0.0, 0.0, -1.0189,			5, TYPE_VEH, true,		"Engine", "Moteur", "Espagnol", "Portugais", "Italien", "Motor"},
	{19472,		0.0, 90.0, 0.0, 1.0,	0.0571, 0.0111, 0.0, 0.0, 247.0, 310.0, 1.0,		90.0, 0.0, 90.0, -0.95,			1, TYPE_CLOTHE, false,	"Gas mask", "Masque a~n~gaz", "Espagnol", "Portugais", "Italien", "Gasmaske"},
	{2880,		0.0, 0.0, 0.0, 1.0,		0.159, 0.0719, 0.1024, 90.0, 0.0, 180.0, 1.0,		335.0, 270.0, 0.0, -0.9335,		2, TYPE_FOOD, false,	"Burger", "Burger", "Espagnol", "Portugais", "Hamburger", "Hamburger"},
	{2881,		180.0,315.0,270.0,1.0,	0.159, 0.1728, 0.0607, 90.0, 90.0, 180.0, 1.0,		335.0, 270.0, 0.0, -0.9335,		2, TYPE_FOOD, false,	"Pizza slice", "Part de~n~pizza", "Espagnol", "Portugais", "Pezzo di~n~pizza", "Stück~n~pizza"},
	{19582, 	90.0, 0.0, 0.0, 1.0, 	0.1284, 0.0594, 0.0, 90.0, 0.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.9925,			3, TYPE_FOOD, false,	"Raw steak", "Steak cru", "Espagnol", "Portugais", "Italien", "Rohes~n~Steak"},
	{19882, 	90.0, 0.0, 0.0, 1.0, 	0.1284, 0.0594, 0.0, 90.0, 0.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.9925,			3, TYPE_FOOD, false,	"Cooked~n~steak", "Steak cuit", "Espagnol", "Portugais", "Italien", "Gebratenes~n~Steak"},
	{19570, 	0.0, 0.0, 0.0, 1.0, 	0.0528, 0.0483, 0.0688, 180.0, 0.0, 0.0, 0.7,		0.0, 0.0, 0.0, -1.0,			1, TYPE_FOOD, false,	"Milk", "Lait", "Leche", "Portugais", "Latte", "Milch"},//70
	{19793,		0.0, 0.0, 0.0, 1.0,		0.0528, 0.0483, -0.05, 0.0, 90.0, 0.0, 0.7,			0.0, 0.0, 0.0, -0.9139,			1, TYPE_RSRC, false,	"Log", "Bois", "Espagnol", "Portugais", "Legno", "Holz"},
	{19574,		315.0, 0.0, 0.0, 1.0,	0.0805, 0.0497, 0.0, 0.0, 0.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.9609,			1, TYPE_FOOD, false,	"Orange", "Orange", "Naranja", "Portugais", "Arancia", "Apfelsine"},
	{19575,		315.0, 0.0, 0.0, 1.0,	0.0805, 0.0497, 0.0, 0.0, 0.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.9609,			1, TYPE_FOOD, false,	"Apple", "Pomme", "Manzana", "Portugais", "Mela", "Apfel"},
	{19577,		315.0, 0.0, 0.0, 1.0,	0.0805, 0.0497, 0.0, 0.0, 0.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.9609,			1, TYPE_FOOD, false,	"Tomato", "Tomate", "Tomate", "Portugais", "Pomodoro", "Tomate"},
	{18916,		90.0, 180.0, 0.0, 1.0,	0.0269, 0.0336, 0.0, 90.0, 0.0, 90.0, 1.0,			90.0, 0.0, 90.0, -0.95,			1, TYPE_CLOTHE, false,	"Plaid~n~bandana", "Bandana~n~a carreaux", "Espagnol", "Portugais", "Italien", "Karriertes~n~band"},
	{18911,		90.0, 180.0, 0.0, 1.0,	0.0269, 0.0336, 0.0, 90.0, 0.0, 90.0, 1.0,			90.0, 0.0, 90.0, -0.95,			1, TYPE_CLOTHE, false,	"Skull~n~bandana", "Bandana~n~cranes", "Espagnol", "Portugais", "Italien", "Allemand"},
	{18913,		90.0, 180.0, 0.0, 1.0,	0.0269, 0.0336, 0.0, 90.0, 0.0, 90.0, 1.0,			90.0, 0.0, 90.0, -0.95,			1, TYPE_CLOTHE,	false,	"Green~n~bandana", "Bandana~n~vert", "Espagnol", "Portugais", "Italien", "Allemand"},
	{18912,		90.0, 180.0, 0.0, 1.0,	0.0269, 0.0336, 0.0, 90.0, 0.0, 90.0, 1.0,			90.0, 0.0, 90.0, -0.95,			1, TYPE_CLOTHE,	false,	"Black~n~bandana", "Bandana~n~noir", "Espagnol", "Portugais", "Italien", "Allemand"},
	{18914,		90.0, 180.0, 0.0, 1.0,	0.0269, 0.0336, 0.0, 90.0, 0.0, 90.0, 1.0,			90.0, 0.0, 90.0, -0.95,			2, TYPE_CLOTHE,	false,	"Army~n~bandana", "Bandana~n~soldat", "Espagnol", "Portugais", "Italien", "Allemand"},
	{1812, 		135.0, 0.0, 0.0, 1.0,	0.259, -0.393, 0.997, 279.2831, 349.0, 0.0, 0.7,	0.0, 0.0, 0.0, -0.987,			15, TYPE_OTHER,	true,	"Shitty bed", "Lit miteux", "Espagnol", "Portugais", "Italien", "Matratze"},//80
	//#if defined DOWNLOAD
	//{-1002, 	0.0, 0.0, 0.0, 1.0, 	0.0528, 0.0483, 0.0688, 180.0, 0.0, 0.0, 0.7,		0.0, 0.0, 0.0, -1.0,			1, TYPE_FOOD, false,	"Water bottle", "Bouteille~n~d'eau", "Bottela de~n~agua", "Portugais", "Italien", "Wasser~n~flasche"},
	//#else
	{1669, 		0.0, 0.0, 35.0, 1.0,	0.08, 0.06, -0.07, 0.0, 180.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.8101,			1, TYPE_FOOD, false,	"Water bottle", "Bouteille~n~d'eau", "Bottela de~n~agua", "Portugais", "Italien", "Wasser~n~flasche"},
	//#endif
	{2226, 		0.0, 0.0, 180.0, 1.0,	0.0, 0.0, 0.0, 0.0, 0.0, 180.0, 1.0,				0.0, 0.0, 180.0, -1.0,			8, TYPE_OTHER, false,	"Boombox", "Boombox", "Boombox", "Boombox", "Boombox", "Boombox"},
	{19515, 	0.0, 270.0, 0.0, 1.0,	0.2796, 0.0, 0.0, 0.0, 180.0, 0.0, 1.0,				0.0, 0.0, 0.0, -1.0118,			20, TYPE_CLOTHE, false,	"SWAT Armour", "Gilet SWAT", "Espagnol", "Portugais", "Italien", "Schutzweste"},
	{19515, 	0.0, 270.0, 0.0, 1.0,	0.2796, 0.0, 0.0, 0.0, 180.0, 0.0, 1.0,				0.0, 0.0, 0.0, -1.0118,			4, TYPE_CLOTHE, false,	"Shot~n~SWAT Armour", "Gilet SWAT~n~perce", "Espagnol", "Portugais", "Italien", "Zerschossene~n~Schutzweste"},
	{805,		0.0, 0.0, 0.0, 1.0,		0.1862, 0.0, 0.0, 90.0, 0.0, 0.0, 0.2,				90.0, 90.0, 0.0, -0.4628, 		2, TYPE_CLOTHE,	 false,	"Camo", "Camouflage", "Espagnol", "Portugais", "Italien", "Tarnung"},
	{1599,		0.0, 0.0, 90.0, 1.0,	0.228, 0.0283, 0.0, 180.0, 180.0, 90.0, 0.5,		0.0, 106.75, 90.0, -0.91,		1, TYPE_FOOD, false,	"Fish", "Poisson", "Espagnol", "Portugais", "Pesce", "Fisch"},
	{1600,		0.0, 0.0, 90.0, 1.0,	0.228, 0.0283, 0.0, 180.0, 180.0, 90.0, 0.5,		0.0, 106.75, 90.0, -0.91,		1, TYPE_FOOD, false,	"Cooked fish", "Poisson cuit", "Espagnol", "Portugais", "Pesce cotto", "Gebratener~n~fisch"},
	{19630,		0.0, 0.0, 0.0, 1.0,		0.0842, 0.0381, 0.0, 0.0, 270.0, 0.0, 1.0,			270.0, 0.0, 0.0, -0.993,		1, TYPE_FOOD, false,	"Bream", "Breme", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19630,		0.0, 0.0, 0.0, 1.0,		0.0842, 0.0381, 0.0, 0.0, 270.0, 0.0, 1.0,			270.0, 0.0, 0.0, -0.993,		2, TYPE_FOOD, false,	"Cooked Bream", "Breme cuite", "Espagnol", "Portugais", "Italien", "Allemand"},
	{18632,		0.0, 0.0, 0.0, 1.0,		0.0842, 0.0381, 0.0, 41.8433, 337.5301, 0.0, 1.0,	0.0, 90.0, 0.0, -0.921,			10, TYPE_TOOL, false,	"Fishing rod", "Canne a~n~peche", "Espagnol", "Portugais", "Italien", "Angel"},//90
	{19998,		0.0, 0.0, 0.0, 1.0,		0.0606, 0.0435, 0.0, 180.0, 0.0, 0.0, 1.0,			90.0, 90.0, 0.0, -0.9054,		5, TYPE_TOOL, false,	"Lighter", "Briquet", "Espagnol", "Portugais", "Accendino", "Feuerzeug"},
	{19574,		315.0, 0.0, 0.0, 1.0,	0.0805, 0.0497, 0.0, 0.0, 0.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.9609,			1, TYPE_FOOD, false,	"Orange seeds", "Graines~n~d'orange", "Semillas de~n~naranja", "Portugais", "Semi di~n~arancia", "Orange~n~kern"},
	{19575,		315.0, 0.0, 0.0, 1.0,	0.0805, 0.0497, 0.0, 0.0, 0.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.9609,			1, TYPE_FOOD, false,	"Apple seeds", "Graines~n~de pomme", "Semillas de~n~manzana", "Portugais", "Semi di~n~mela", "Apfel~n~saat"},
	{19577,		315.0, 0.0, 0.0, 1.0,	0.0805, 0.0497, 0.0, 0.0, 0.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.9609,			1, TYPE_FOOD, false,	"Tomato seeds", "Graines~n~de tomate", "Semillas de~n~tomate", "Portugais", "Semi di~n~pomodoro", "Tomate~n~saat"},
	{3134,		0.0, 0.0, 0.0, 1.0,		0.0, 0.1165, 0.1425, 90.0, 317.428, 340.638, 0.5,	0.0, 90.0, 0.0, -0.578,			30, TYPE_TOOL, true,	"Water~n~collector", "Recuperateur~n~d'eau", "Espagnol", "Portugais", "Italien", "Wasser-~n~kollector"},
	{19627,		90.0, 0.0, 0.0, 1.0,	0.0827, 0.0275, 0.0, 90.0, 0.0, 270.0, 1.5,			0.0, 0.0, 0.0, -0.987,			25, TYPE_TOOL, false,	"Wrench", "Cle anglaise", "Espagnol", "Portugais", "Italien", "Gaszange"},
	{2332,		0.0, 0.0, 180.0, 1.0,	0.004, 0.1746, 0.24, 286.3804, 350.0, 107.657, 0.5,	300.0, 0.0, 0.0, -0.964,		1, TYPE_TOOL, true,		"Safe", "Coffre fort", "Espagnol", "Portugais", "Italien", "Safe"},
	//#if defined DOWNLOAD
	//{-1001,		0.0, 0.0, 0.0, 1.0, 	0.0528, 0.0483, 0.0688, 180.0, 0.0, 0.0, 0.7,		0.0, 0.0, 0.0, -1.0,			2, TYPE_VEH, false,		"Bottle full~n~of gas",	"Bouteille~n~d'essence", "Bottela de~n~gasolino", "Portugais", "Italien", "Gasflasche"},
	//#else
	{1544, 		0.0, 0.0, 0.0, 1.0, 	0.08, 0.06, -0.07, 0.0, 180.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.8101,			2, TYPE_VEH, false,		"Bottle full~n~of gas",	"Bouteille~n~d'essence", "Bottela de~n~gasolino", "Portugais", "Italien", "Gasflasche"},
	//#endif
	{19602, 	90.0, 0.0, 0.0, 1.0, 	0.1686, 0.0486, 0.0, 90.0, 0.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.7,			5, TYPE_WEAP, false,	"Landmine",	"Mine", "Espagnol", "Portugais", "Italien", "Mine"},
	{1252, 		0.0, 0.0, 0.0, 1.0, 	0.1686, 0.0486, 0.0, 0.0, 0.0, 0.0, 0.7,			90.0, 0.0, 0.0, -0.9,			5, TYPE_WEAP, false,	"Timerbomb", "Bombe a~n~timer", "Espagnol", "Portugais", "Italien", "Zeitbombe"},//100
	{19801,		0.0, 0.0, 0.0, 1.0,		0.1808, 0.0216, 0.0, 0.0, 90.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.886,			2, TYPE_CLOTHE, false, 	"Hood", "Cagoule", "Capucha", "Capô", "Cappuccio", "Kapuze"},
	{11718,		0.0, 0.0, 0.0, 1.0,		0.3206, -0.0131, -0.265, 270.0, 230.0, 43.38, 1.0,	0.0, 0.0, 0.0, -1.0,			8, TYPE_TOOL, false, 	"Pan", "Casserole", "Espagnol", "Portugais", "Italien", "Kochtopf"},
	{19846,		90.0, 0.0, 0.0, 1.0,	0.1418, 0.0251, 0.0, 71.8962, 0.0, 90.0, 0.5,		0.0, 90.0, 0.0, -1.1611,		3, TYPE_RSRC, false, 	"Metal", "Metal", "Espagnol", "Portugais", "Italien", "Schrott"},
	{19843,		90.0, 0.0, 0.0, 1.0,	-0.05, 0.2573, 0.1691, 270.3665, 55.3, 193.0, 0.7,	0.0, 90.0, 0.0, -1.1611,		5, TYPE_RSRC, true,		"Metal plate", "Plaque de~n~metal", "Espagnol", "Portugais", "Italien", "Metallplatte"},
	{19433,		90.0, 90.0, 0.0, 1.0,	0.2645, 0.0362, 0.1021, 11.0, 0.0, 90.0, 0.5,		0.0, 0.0, 0.0, -2.604,			2, TYPE_RSRC, true,		"Plank", "Planche", "Espagnol", "Portugais", "Italien", "Brett"},
	{19804,		90.0, 0.0, 270.0, 0.7,	0.0863, 0.0311, -0.0024, 0.0, 270.0, 0.0, 1.0,		0.0, 180.0, 90.0, -1.0131,		5, TYPE_TOOL, false,	"Lock", "Serrure", "Espagnol", "Portugais", "Italien", "Schloss"},
	{1575,		0.0, 0.0, 0.0, 1.0,		0.0834, 0.0682, -0.0218, 60.6669, 0.0, 90.0, 0.3,	0.0, 0.0, 0.0, -0.798,			3, TYPE_RSRC, false,	"Gunpowder", "Poudre", "Espagnol", "Portugais", "Italien", "Schiesspulver"},
	{2891,		0.0, 0.0, 0.0, 1.0,		0.2717, 0.1273, -0.1037, 43.4869, 0.0, 0.0, 1.0, 	0.0, 0.0, 0.0, -1.0,			1, TYPE_FOOD, false,	"Flour", "Farine", "Espagnol", "Portugais", "Italien", "Mehl"},
	{19580, 	90.0, 180.0, 0.0, 1.0, 	0.2621, 0.0268, 0.0, 73.9384, 0.0, 0.0, 1.0, 		0.0, 0.0, 0.0, -0.988,			3, TYPE_FOOD, false,	"Pizza", "Pizza", "Pizza", "Pizza", "Pizza", "Pizza"},
	{19579, 	90.0, 0.0, 0.0, 1.0, 	0.14, 0.0378, 0.0, 76.4778, 0.0, 0.0, 1.3,			0.0, 0.0, 0.0, -1.0,			2, TYPE_FOOD, false,	"Bread", "Pain", "Pan", "Portugais", "Pan", "Brodt"},//110
	{19366, 	90.0, 90.0, 0.0, 1.0,	0.6666, 0.0585, 0.0, 11.2624, 0.0, 90.0, 0.5, 		0.0, 90.0, 0.0, -1.089,			5, TYPE_OTHER, true,	"Wall", "Mur", "Espagnol", "Portugais", "Paretto", "Mauer"},
	{19802, 	90.0, 0.0, 90.0, 1.0,	-0.0415, 0.1514, -0.7014, 11.7901, 0.0, 20.0, 0.5,	90.0, 0.0, 90.0, -0.998,		10,	TYPE_OTHER,	true,	"Door", "Porte", "Puerta", "Porta", "Porta", "Tur"},
	{3017, 		0.0, 0.0, 0.0, 1.0,		0.1654, 0.1, 0.0, 0.0, 270.0, 0.0, 0.7, 			0.0, 0.0, 0.0, -0.985,			15, TYPE_OTHER,	false,	"Construction~n~plans", "Plans", "Espagnol", "Portugais", "Italien", "Allemand"},
	{19621,		0.0, 0.0, 0.0, 1.0, 	0.0981, 0.0275, 0.0, 330.0, 180.0, 270.0, 1.0,		0.0, 0.0, 0.0, -0.906,			0, TYPE_NOSELL,	false,	"Can", "Burette", "Espagnol", "Portugais", "Italien", "Ölkanne"},
	{19837,		0.0, 0.0, 0.0, 1.0,		0.0921, -0.0223, 0.0516, 180.0, 0.0, 0.0, 0.5,		0.0, 0.0, 0.0, -1.0,			1, TYPE_OTHER, false,	"Wheat seeds", "Graines~n~de ble", "Semillas de~n~trigo", "Portugais", "Semi di~n~grano", "Weizen~n~saat"},
	{1453,		0.0, 90.0, 0.0, 1.0,	0.0935, 0.0277, 0.0, 0.0, 0.0, 0.0, 0.3,			0.0, 0.0, 0.0, -2.018,			1, TYPE_OTHER, false,	"Wheat", "Ble", "Trigo", "Portugais", "Grano", "Weizen"},
	{920,		0.0, 0.0, 0.0, 1.0,		0.0593, 0.2803, 0.3084, 98.0, 180.0, 113.38, 0.6,	0.0, 0.0, 270.0, -0.516,		30, TYPE_TOOL, true,	"Shredder", "Broyeur", "Espagnol", "Portugais", "Italien", "Mühle"},
	{19563,		0.0, 0.0, 0.0, 1.0,		0.1243, 0.0455, 0.1193, 0.0, 180.0, 0.0, 1.0,		0.0, 0.0, 270.0, -1.0,			1, TYPE_FOOD, false,	"Orange~n~juice", "Jus d'orange", "Espagnol", "Portugais", "Italien", "Orangesaft"},
	{19564,		0.0, 0.0, 0.0, 1.0,		0.1243, 0.0455, 0.1193, 0.0, 180.0, 0.0, 1.0,		0.0, 0.0, 270.0, -1.0,			1, TYPE_FOOD, false,	"Apple~n~juice", "Jus de~n~pomme", "Espagnol", "Portugais", "Italien", "Apfelsaft"},
	{19585,		0.0, 0.0, 0.0, 1.0,		0.0837, -0.0482, 0.0, 90.0, 270.0, 0.0, 0.7,		0.0, 0.0, 0.0, -0.759,			3, TYPE_FOOD, false,	"Fish soup", "Soupe au~n~poisson", "Espagnol", "Portugais", "Minestre~n~di pesce", "Fischsuppe"},//120
	{19811,		0.0, 0.0, 0.0, 1.0,		0.1923, 0.0, 0.0, 90.0, 0.0, 0.0, 1.0,				0.0, 0.0, 0.0, -0.94,			2, TYPE_FOOD, false,	"Meat~n~sandwich", "Sandwich a~n~la viande", "Espagnol", "Portugais", "Italien", "Fleischbrot"},
	{19317,		0.0, 1.0, 1.0, 1.0,		-0.0447, 0.2862, 0.1587, 180.0, 150.0, 326.85, 1.0,	0.0, 0.0, 0.0, 0.0,				0, TYPE_NOSELL, false,	"Bass guitar", "Basse", "Espagnol", "Portugais", "Italien", "Bassguitarre"},
	{801,		0.0, 0.0, 0.0, 1.0,		0.0, 0.0, 0.0, 0.0, 90.0, 0.0, 0.2,					0.0, 0.0, 0.0, 0.0,				0, TYPE_NOSELL, false,	"Fern", "Fougere", "Espagnol", "Portugais", "Italien", "Farnkraut"},
	{1602,		0.0, 0.0, 0.0, 1.0,		0.05, 0.0, 0.0, 0.0, 270.0, 0.0, 0.7,				0.0, 0.0, 0.0, 0.0,				0, TYPE_NOSELL,	false,	"Jellyfish", "Meduse", "Espagnol", "Portugais", "Italien", "Qualle"},
	{2901, 		0.0, 0.0, 0.0, 1.0, 	0.2156, 0.2, 0.0, 0.0, 90.0, 0.0, 1.0,				0.0, 0.0, 0.0, 0.0,				0, TYPE_NOSELL, true, 	"Ballot", "Fagot", "Espagnol", "Portugais", "Italien", "Grassballen"},
	//#if defined DOWNLOAD
	//{-1002,		0.0, 0.0, 0.0, 1.0, 	0.0528, 0.0483, 0.0688, 180.0, 0.0, 0.0, 0.7,		0.0, 0.0, 0.0, -1.0,			1, TYPE_FOOD, false,	"Undrinkable~n~water", "Eau non~n~potable", "Aqua no~n~potable", "Portugais", "Italien", "Nicht~n~trinkwasser"},
 	//#else
	{1669, 		0.0, 0.0, 35.0, 1.0,	0.08, 0.06, -0.07, 0.0, 180.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.8101,			1, TYPE_FOOD, false,	"Undrinkable~n~water", "Eau non~n~potable", "Aqua no~n~potable", "Portugais", "Italien", "Nicht~n~trinkwasser"},
	//#endif
	{2328,		180.0, 270.0, 0.0, 0.5,	1.3, 0.0, 0.2, 90.0, 180.0, 270.0, 1.0,				0.0, 0.0, 0.0, -1.0,			25, TYPE_TOOL, true,	"Gunrack", "Etagere", "Espagnol", "Portugais", "Italien", "Regal"},
	{11725,		0.0, 0.0, 0.0, 1.0,		0.0, 0.23, 0.25, 282.0, 0.0, 90.0, 1.0,				0.0, 0.0, 0.0, -0.6119,			35, TYPE_TOOL, true,	"Brazier", "Brasero", "Brasero", "Portugais", "Italien", "Kohlebecken"},
	{19273,		0.0, 0.0, 0.0, 1.0,		0.2, 0.055, 0.0, 0.0, 90.0, 0.0, 0.5,				270.0, 0.0, 0.0, -0.9961,		50, TYPE_TOOL, false,	"Keycode", "Codeur", "Espagnol", "Portugais", "Italien", "Allemand"},
	{2144,		0.0, 0.0, 0.0, 1.0,		0.2, 0.0, 0.1, 280.0, 345.0, 0.0, 0.7,				270.0, 0.0, 0.0, -0.9,			50, TYPE_TOOL, true,	"Fridge", "Frigo", "Espagnol", "Portugais", "Italien", "Allemand"},//130
 	{2976, 		0.0, 0.0, 330.0, 1.0, 	0.18, 0.0, -0.2, 0.0, 0.0, 0.0, 0.5,				0.0, 0.0, 0.0, -0.991, 			2, TYPE_OTHER, false, 	"u238", "u238", "u238", "u238", "u238", "u238"},
 	{18888, 	0.0, 0.0, 330.0, 1.0, 	0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,					0.0, 0.0, 0.0, -1.0, 			1, TYPE_OTHER, false, 	"Exp", "Exp", "Exp", "Exp", "Exp", "Exp"},
 	{2034, 		0.0, 0.0, 0.0, 1.0,		0.17, 0.05, -0.05, 270.0, 0.0, 0.0, 1.0,			90.0, 0.0, 0.0, -0.955,			5, TYPE_WEAP, false,	"Grip", "Crosse", "Espagnol", "Portugais", "Italien", "Allemand"},
 	{2033, 		0.0, 0.0, 0.0, 1.0,		0.1, 0.05, -0.05, 270.0, 0.0, 90.0, 1.0,			90.0, 0.0, 0.0, -0.955,			5, TYPE_WEAP, false,	"Steel barrel", "Canon en acier", "Espagnol", "Portugais", "Italien", "Allemand"},
 	{19904, 	0.0, 270.0, 0.0, 1.0,	0.2796, 0.0, 0.0, 0.0, 180.0, 0.0, 1.0,				0.0, 0.0, 0.0, -1.0118,			10, TYPE_CLOTHE, false,	"Rain Coat", "Gilet thermique", "Espagnol", "Portugais", "Italien", "Wasserdichtweste"},
 	{1210, 		0.0, 0.0, 0.0, 1.0,		0.2873, 0.107, 0.0807, 0.0, 253.1701, 0.0, 1.0,		0.0, 0.0, 0.0, -0.8519,			10, TYPE_CLOTHE, false,	"Suitcase", "Valise", "Espagnol", "Portugais", "Italien", "Allemand"},
 	{19279,		0.0, 0.0, 0.0, 1.0,		0.0, 0.15, 0.2, 0.0, 110.0, 0.0, 1.0,				0.0, 90.0, 0.0, -0.7559,		4, TYPE_TOOL, true,		"Light", "Lampe", "Espagnol", "Portugais", "Italien", "Lampe"},
	{343,		0.0, 0.0, 60.0, 1.0,	0.0332, -0.0053, -0.0065, 0.0, 0.0, 0.0, 1.0,		0.0, 0.0, 100.0, -0.792,		3, TYPE_WEAP, false,	"Tear gas", "Lacrymogène", "Espagnol", "Portugais", "Italien", "Allemand"},
	{2247,		0.0, 0.0, 0.0, 1.0, 	0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,					0.0, 0.0, 0.0, -0.5359,			1, TYPE_OTHER, false, 	" ", " ", "Espagnol", "Portugais", "Italien", "Allemand"},
	{857,		0.0, 0.0, 0.0, 1.0,		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.3,					0.0, 0.0, 0.0, -0.653,/*-0.407*/1, TYPE_OTHER, false,	"Belladonna", "Belladone", "Belladonna", "Portugais", "Italien", "Tollkirsche"},//140
	{19106, 	0.0, 270.0, 0.0, 1.0,	0.185, 0.0804, 0.0, 0.0, 0.0, 90.0, 1.0,			0.0, 270.0, 0.0, -0.961, 		5, TYPE_CLOTHE,	false,	"Shot soldier~n~helmet", "Casque troue~n~militaire", "Espagnol", "Portugais", "Italien", "Zerschossene~n~soldatenhelm"},
	{19107, 	0.0, 270.0, 0.0, 1.0,	0.185, 0.0804, 0.0, 0.0, 0.0, 90.0, 1.0,			0.0, 180.0, 0.0, -0.961, 		5, TYPE_CLOTHE,	false,	"Shot marine~n~helmet", "Casque troue~n~de marin", "Espagnol", "Portugais", "Italien", "Zerschossene~n~marinehelm"},
	{1736, 		0.0, 0.0, 0.0, 1.0,		0.1704, -0.1775, 0.0, 340.49, 270.0, 0.0, 0.7,		270.0, 0.0, 0.0, -0.704, 		15, TYPE_OTHER,	false,	"Hunting~n~trophy", "Trophee~n~de chasse", "Espagnol", "Portugais", "Italien", "Allemand"},
	{1828, 		0.0, 0.0, 0.0, 1.0,		0.338, 0.0675, 0.0, 244.06, 173.63, 278.59, 0.4,	0.0, 0.0, 0.0, -1.0, 			15, TYPE_OTHER,	false,	"Fur rug", "Tapis de fourrure", "Espagnol", "Portugais", "Italien", "Allemand"},
	{366,       0.0, 0.0, 0.0, 1.0,     0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,                  0.0, 0.0, 0.0, -1.0,            6, TYPE_WEAP, false,  	"Extinguisher", "Exctincteur", "Espagnol", "Portugais", "Italien", "Allemand"},
	{3014,		0.0, 0.0, 0.0, 1.0,		0.014, 0.211, 0.0735, 288.56, 336.02, 288.84, 1.0,	0.0, 0.0, 0.0, -0.763,			10, TYPE_OTHER, true,	"Crate", "Boite", "Espagnol", "Portugais", "Italien", "Allemand"},
	{365,		0.0, 0.0, 0.0, 1.0,		0.1192, 0.0173, -0.1168, 0.0, 180.0, 0.0, 1.0,		0.0, 0.0, 0.0, 0.0,				10, TYPE_VEH, false,	"Spraycan", "Peinture", "Pintura", "Portugais", "Pintura", "Allemand"},
	{15038,		0.0, 0.0, 0.0, 1.0,		0.0529, 0.4305, 0.2043, 270.0, 0.0, 0.0, 0.7, 		0.0, 0.0, 0.0, -0.39,			5, TYPE_OTHER, true,	"Fir sapling", "Pousse de~n~sapin", "Pimpollo de~n~abeto", "Portugais", "Italien", "Allemand"},
	{2060,		0.0, 0.0, 0.0, 1.0,		-0.0194, 0.1036, 0.1782, 101.0, 0.0, 79.6399, 1.0,	0.0, 0.0, 0.0, -0.854,			10, TYPE_OTHER, true, 	"Weird bag", "Drole de sac", "Espagnol", "Portugais", "Italien", "Allemand"},
	{953,		0.0, 0.0, 180.0, 1.0,	0.0451, 0.2588, 0.1398, 270.0, 4.364, 117.31, 1.0,	0.0, 0.0, 0.0, -0.7131,			50, TYPE_OTHER, true,	"Oyster", "Huitre", "Espagnol", "Portugais", "Italien", "Allemand"},//150
	{1801, 		135.0, 0.0, 0.0, 1.0,	0.421, -0.13, 1.727, 270.0, 350.732, 10.697, 0.7,	0.0, 0.0, 0.0, -0.987,			35, TYPE_OTHER,	true,	"Bed", "Lit", "Espagnol", "Portugais", "Italien", "Matratze"},
	{964,		0.0, 0.0, 0.0, 1.0,		0.2, 0.0, 0.1, 280.0, 0.0, 20.0, 0.5,				0.0, 0.0, 0.0, -0.1,			0, TYPE_NOSELL, true,	"Box", "Boite", "Espagnol", "Portugais", "Italien", "Kiste"},
	{19346,		0.0, -45.0, 45.0, 1.0,	0.0811, 0.0353, 0.0, 0.0, 90.0, 90.0, 0.7,			0.0, 0.0, 0.0, -1.0,			1, TYPE_FOOD, false,	"Hotdog", "Hotdog", "Hotdog", "Hotdog", "Hotdog", "Hotdog"},
	{11743,		0.0, 0.0, 0.0, 1.0,		0.0, 0.014, 0.0, 0.0, 90.0, 0.0, 0.7,				0.0, 0.0, 0.0, -1.0,			10, TYPE_OTHER, true,	"Coffee machine", "Machine a~n~cafe", "Espagnol", "Portugais", "Italien", "Allemand"},
	{1729, 		135.0, 90.0, 0.0, 1.0,	0.133, -0.064, 0.213, 281.69, 350.0, 108.9, 0.7,	0.0, 0.0, 0.0, -1.0,			20, TYPE_OTHER,	true,	"Seat", "Siege", "Espagnol", "Portugais", "Italien", "Setz"},/*0.421, -0.13, 0.0, 270.0, 350.732, 10.697, 0.7*/
	{3927, 		0.0, 0.0, 0.0, 1.0,		0.029, 0.0413, -0.4417, 0.0, 180.0, 0.0, 0.3,		0.0, 0.0, 0.0, 0.0,				10, TYPE_OTHER,	false,	"Board", "Panneau", "Espagnol", "Portugais", "Italien", "Allemand"},
	{918, 		0.0, 0.0, 0.0, 1.0,		0.029, 0.0413, -0.4417, 0.0, 180.0, 0.0, 0.3,		0.0, 0.0, 0.0, -0.6,			10, TYPE_TOOL,	false,	"Compressor", "Compresseur", "Espagnol", "Portugais", "Italien", "Allemand"},
	{964,		0.0, 0.0, 0.0, 1.0,		0.2, 0.0, 0.1, 280.0, 0.0, 20.0, 0.5,				0.0, 0.0, 0.0, -0.1,			0, TYPE_OTHER, true,	"Return to~n~sender", "Retour a~n~l'envoyeur", "Espagnol", "Portugais", "Italien", "Allemand"}
};

//---FONCTIONS---//
stock InitializeItemsVariables() 
{
	for(new i = 0; i < MAX_GROUND_ITEMS; i ++)
	{
	    dItems[i][ItemID] = -1;
	    dItems[i][ObjectID] = INVALID_OBJECT_ID;
	    dItems[i][bAutoSpawn] = false;
	    dItems[i][ObjectText] = Text3D:INVALID_3DTEXT_ID;
	    dItems[i][xItem] = 0.0;
	    dItems[i][yItem] = 0.0;
	    dItems[i][zItem] = 0.0;
	}
}

stock GetItemName(objectid, language)
{
	new string[30];
	switch(language)
	{
	    case LANGUAGE_EN: strcpy(string, NoNewLineSign(aObjects[objectid][ObjectEnName]));
	    case LANGUAGE_FR: strcpy(string, NoNewLineSign(aObjects[objectid][ObjectFrName]));
	    case LANGUAGE_ES: strcpy(string, NoNewLineSign(aObjects[objectid][ObjectEsName]));
	    case LANGUAGE_PG: strcpy(string, NoNewLineSign(aObjects[objectid][ObjectPgName]));
	    case LANGUAGE_IT: strcpy(string, NoNewLineSign(aObjects[objectid][ObjectItName]));
	    case LANGUAGE_DE: strcpy(string, NoNewLineSign(aObjects[objectid][ObjectDeName]));
	}
	return string;
}

stock bool:IsItemAroundPlayer(itemid, playerid, Float:radius)
{
	if(IsPlayerInRangeOfPoint(playerid, radius, dItems[itemid][xItem], dItems[itemid][yItem], dItems[itemid][zItem])) return true;
	return false;
}

stock bool:IsPlayerNearItem(playerid)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	new Float:fTrash;
	for(new i = 0; i < MAX_GROUND_ITEMS; i ++)
	{
	    if(dItems[i][ItemID] == 0) continue;
	    if(IsDynamicObjectMoving(dItems[i][ObjectID])) continue;
	    if(IsItemAroundPlayer(i, playerid, 2.0))
	    {
			if(CA_RayCastLine(x, y, z, dItems[i][xItem], dItems[i][yItem], dItems[i][zItem], fTrash, fTrash, fTrash) != 0) continue;
			return true;
		}
	}
	return false;
}

stock FindNearbyItems(playerid, Float:x, Float:y, Float:z, slot, tInArray[][][])
{
    //new len = sizeof(tInArray[]);
	new Float:fTrash;
    for(new i = 0; i < MAX_GROUND_ITEMS; i ++)
    {
        if(slot < 9) break;
        if(dItems[i][ItemID] == 0) continue;
        if(!IsDynamicObjectMoving(dItems[i][ObjectID]) && IsItemAroundPlayer(i, playerid, 2.0))
        {
            if(CA_RayCastLine(x, y, z, dItems[i][xItem], dItems[i][yItem], dItems[i][zItem], fTrash, fTrash, fTrash) != 0) continue;
            tInArray[playerid][slot][0] = i;
            tInArray[playerid][slot][1] = 0;
            slot ++;
        }
    }

    return slot;
}

stock GetPlayerInventoryUse(playerid) 
{
    return IsValidPlayer(playerid) ? pUseInventory[playerid] : -1;
}

stock SetPlayerInventoryUse(playerid, useInventory) 
{
    if(IsValidPlayer(playerid)) 
    {
        pUseInventory[playerid] = useInventory;
    }
}

stock GetPlayerVehicleUse(playerid) 
{
    return IsValidPlayer(playerid) ? pVehicleInventory[playerid] : -1;
}

stock SetPlayerVehicleUse(playerid, useInventory) 
{
    if(IsValidPlayer(playerid)) 
    {
        pVehicleInventory[playerid] = useInventory;
    }
}

CreateSafe(Float:x, Float:y, Float:z, Float:angle, load = -1)
{
	static slotid;
	if(load == -1)
	{
		if(dSafeInfos[slotid][bSafe])
		{
			for(new i = 0; i < MAX_SAFES; i ++)
		 	{
				if(!dSafeInfos[i][bSafe])
			    {
			    	slotid = i;
			        break;
			    }
			}
			if(dSafeInfos[slotid][bSafe])
			{
				DestroyDynamicObject(dSafeInfos[slotid][oSafe][0]);
				DestroyDynamicObject(dSafeInfos[slotid][oSafe][1]);
			}
		}
	}
	//---
	dSafeInfos[(load == -1) ? slotid : load][bSafe] = true;
	dSafeInfos[(load == -1) ? slotid : load][bOpenSafe] = false;
	dSafeInfos[(load == -1) ? slotid : load][xSafe] = x;
	dSafeInfos[(load == -1) ? slotid : load][ySafe] = y;
	dSafeInfos[(load == -1) ? slotid : load][zSafe] = z;
	dSafeInfos[(load == -1) ? slotid : load][aSafe] = angle;
    /*-118.68917063
    -0.4128
    -0.2259*/
    angle += 119.01892652;
	dSafeInfos[(load == -1) ? slotid : load][oSafe][0] = CreateDynamicObject(19618, x, y, z - 0.539, 0.0, 0.0, angle - 119.01892652, -1, -1, -1, 25.0, 20.0);
	dSafeInfos[(load == -1) ? slotid : load][oSafe][1] = CreateDynamicObject(19619, x + (0.4128 * floatsin(-angle, degrees)), y + (0.4128 * floatcos(-angle, degrees)), z - 0.539, 0.0, 0.0, angle - 119.01892652, -1, -1, -1, 25.0, 20.0);
	//---
    slotid ++;
    if(slotid == MAX_SAFES) slotid = 0;
    return (slotid == 0) ? MAX_SAFES : slotid - 1;
}

DestroySafe(safeid)
{
	DestroyDynamicObject(dSafeInfos[safeid][oSafe][0]);
	DestroyDynamicObject(dSafeInfos[safeid][oSafe][1]);
	dSafeInfos[safeid][oSafe][0] = INVALID_OBJECT_ID;
	dSafeInfos[safeid][oSafe][1] = INVALID_OBJECT_ID;
	for(new i = 0; i < 12; i ++) dSafeInfos[safeid][dItem][i] = 0;
	dSafeInfos[safeid][xSafe] = 0.0;
	dSafeInfos[safeid][ySafe] = 0.0;
	dSafeInfos[safeid][zSafe] = 0.0;
	dSafeInfos[safeid][aSafe] = 0.0;
}

IsPlayerNearSafe(playerid)
{
	new Float:x, Float:y, Float:z, Float:fTrash;
	GetPlayerPos(playerid, x, y, z);
	for(new i = 0; i < MAX_SAFES; i ++)
	{
		if(dSafeInfos[i][bSafe] && IsPlayerInRangeOfPoint(playerid, 1.5, dSafeInfos[i][xSafe], dSafeInfos[i][ySafe], dSafeInfos[i][zSafe]))
		{
			if(CA_RayCastLine(x, y, z, dSafeInfos[i][xSafe], dSafeInfos[i][ySafe], dSafeInfos[i][zSafe], fTrash, fTrash, fTrash) != 0) continue;
			return i;
		}
	}
	return -1;
}

IsSafeEmpty(safeid)
{
	for(new i = 0; i < 12; i ++) if(dSafeInfos[safeid][dItem][i] != 0) return false;
	return true;
}

ChangeSafeDoorState(safeid, bool:open)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i ++) if(IsPlayerInRangeOfPoint(i, 20.0, dSafeInfos[safeid][xSafe], dSafeInfos[safeid][ySafe], dSafeInfos[safeid][zSafe])) PlayerPlaySound(i, 11200, dSafeInfos[safeid][xSafe], dSafeInfos[safeid][ySafe], dSafeInfos[safeid][zSafe]);
	MoveDynamicObject(dSafeInfos[safeid][oSafe][1], dSafeInfos[safeid][xSafe] + (0.4128 * floatsin(-(dSafeInfos[safeid][aSafe] + 119.01892652), degrees)), dSafeInfos[safeid][ySafe] + (0.4128 * floatcos(-(dSafeInfos[safeid][aSafe] + 119.01892652), degrees)), open ? dSafeInfos[safeid][zSafe] - 0.539 : dSafeInfos[safeid][zSafe] - 0.529, 0.005, 0.0, 0.0, open ? dSafeInfos[safeid][aSafe] - 90.0 : dSafeInfos[safeid][aSafe]);
	dSafeInfos[safeid][bOpenSafe] = open;
}

GivePlayerSafeObject(playerid, safeid, objectid, slotid)//Fonction pour foutre un objet dans un coffre fort
{
	if(safeid != -1)
	{
		if(objectid == -1 || objectid == 0)//Si l'objet est nul, on remet ses variables à 0
		{
		    if(playerid != INVALID_PLAYER_ID && dSafeInfos[safeid][dItem][slotid] != 0) UpdateSafe(playerid, safeid, slotid, 0);
			dSafeInfos[safeid][dItem][slotid] = 0;
		}
		else
		{
			if(playerid != INVALID_PLAYER_ID && dSafeInfos[safeid][dItem][slotid] != objectid) UpdateSafe(playerid, safeid, slotid, objectid);
			dSafeInfos[safeid][dItem][slotid] = objectid;
		}
	}
	return 1;
}

stock InitializeSafesVariables()
{
	for(new i = 0; i < MAX_SAFES; i ++)
	{
	    dSafeInfos[i][oSafe][0] = INVALID_OBJECT_ID;
	    dSafeInfos[i][oSafe][1] = INVALID_OBJECT_ID;
	    dSafeInfos[i][bSafe] = false;
	    dSafeInfos[i][bOpenSafe] = false;
	    dSafeInfos[i][xSafe] = 0.0;
	    dSafeInfos[i][ySafe] = 0.0;
	    dSafeInfos[i][zSafe] = 0.0;
	    dSafeInfos[i][aSafe] = 0.0;
	    for(new j = 0; j < 12; j ++) dSafeInfos[i][dItem][j] = 0;
	}
}

public LoadSafes_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_SAFES; i ++)
	{
	    format(string, sizeof(string), "Coffre%d", i);
		INI_Bool(string, dSafeInfos[i][bSafe]);
	    format(string, sizeof(string), "Porte%d", i);
		INI_Bool(string, dSafeInfos[i][bOpenSafe]);
	    format(string, sizeof(string), "Code%d", i);
		INI_String(string, dSafeInfos[i][sSafeCode], 5);
	    format(string, sizeof(string), "xSafe%d", i);
		INI_Float(string, dSafeInfos[i][xSafe]);
	    format(string, sizeof(string), "ySafe%d", i);
		INI_Float(string, dSafeInfos[i][ySafe]);
	    format(string, sizeof(string), "zSafe%d", i);
		INI_Float(string, dSafeInfos[i][zSafe]);
	    format(string, sizeof(string), "aSafe%d", i);
		INI_Float(string, dSafeInfos[i][aSafe]);
		for(new j = 0; j < 12; j ++)
		{
	    	format(string, sizeof(string), "%dObjet%d", j, i);
			INI_Int(string, dSafeInfos[i][dItem][j]);
		}
	}
	return 1;
}

SaveSafes()
{
	new string[50];
	new INI:File = INI_Open(SPATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_SAFES; i ++)
	{
	    format(string, sizeof(string), "Coffre%d", i);
		INI_WriteBool(File,string, dSafeInfos[i][bSafe]);
	    format(string, sizeof(string), "Porte%d", i);
		INI_WriteBool(File,string, dSafeInfos[i][bOpenSafe]);
	    format(string, sizeof(string), "Code%d", i);
		INI_WriteString(File,string, dSafeInfos[i][sSafeCode]);
	    format(string, sizeof(string), "xSafe%d", i);
		INI_WriteFloat(File,string, dSafeInfos[i][xSafe]);
	    format(string, sizeof(string), "ySafe%d", i);
		INI_WriteFloat(File,string, dSafeInfos[i][ySafe]);
	    format(string, sizeof(string), "zSafe%d", i);
		INI_WriteFloat(File,string, dSafeInfos[i][zSafe]);
	    format(string, sizeof(string), "aSafe%d", i);
		INI_WriteFloat(File,string, dSafeInfos[i][aSafe]);
		for(new j = 0; j < 12; j ++)
		{
	    	format(string, sizeof(string), "%dObjet%d", j, i);
			INI_WriteInt(File,string, dSafeInfos[i][dItem][j]);
		}
	}
	INI_Close(File);
}

stock SetupSafes()
{
	//INITIALISATION
	new dLastLoaded = 0;
	//CHARGEMENT
 	#if defined LOAD_DYNAMICS
    INI_ParseFile(SPATH, "LoadSafes_data");
    #endif
	for(new i = 0; i < MAX_SAFES; i ++)
	{
	    if(dSafeInfos[i][xSafe] == 0.0 && dSafeInfos[i][ySafe] == 0.0 && dSafeInfos[i][zSafe] == 0.0) dSafeInfos[i][bSafe] = false;
		if(dSafeInfos[i][bSafe])
		{
		    new bool:bDoor = dSafeInfos[i][bOpenSafe];
			CreateSafe(dSafeInfos[i][xSafe], dSafeInfos[i][ySafe], dSafeInfos[i][zSafe], dSafeInfos[i][aSafe], i);
			ChangeSafeDoorState(i, bDoor);
			dLastLoaded = i;
		}
	}
	for(new i = dLastLoaded + 1; i < MAX_SAFES; i ++)
	{
		dSafeInfos[i][bSafe] = false;
	    for(new j = 0; j < 12; j ++)
	    {
	        dSafeInfos[i][dItem][j] = 0;
	    }
	}
    LogInfo(true, "[INIT]Coffres forts charges");
}

stock IsValidSafe(safeid)
{
	return MAX_SAFES > safeid >= 0;
}

stock GetPlayerSafeItem(safeid, slotid) 
{
	return (IsValidSafe(safeid) && 12 > slotid >= 0) ? dSafeInfos[safeid][dItem][slotid] : 0;
}

stock SetPlayerAuctionHouse(playerid, category, item)
{
	if(category == 8)
	{
	    pHDV[playerid][0] = 8;
        pHDV[playerid][1] = 0;
	    UpdatePlayerAuctionHouse(playerid, 8, 0);
	    return true;
	}
 	new i = 0;
 	new slot = -1;
	if(category != pHDV[playerid][0])
	{
		switch(category)
		{
			case 0:
			{
				for(i = 0; i < MAX_AUCTION_ITEMS; i ++) if(dAuctionSellerTool[i][dItemSale] != 0)
				{
					slot = i;
					break;
				}
				if(slot == -1) return false;
			}
			case 1:
			{
				for(i = 0; i < MAX_AUCTION_ITEMS; i ++) if(dAuctionSellerMedic[i][dItemSale] != 0)
				{
					slot = i;
					break;
				}
				if(slot == -1) return false;
			}
			case 2:
			{
				for(i = 0; i < MAX_AUCTION_ITEMS; i ++) if(dAuctionSellerWeapon[i][dItemSale] != 0)
				{
					slot = i;
					break;
				}
				if(slot == -1) return false;
			}
			case 3:
			{
				for(i = 0; i < MAX_AUCTION_ITEMS; i ++) if(dAuctionSellerOther[i][dItemSale] != 0)
				{
					slot = i;
					break;
				}
				if(slot == -1) return false;
			}
			case 4:
			{
				for(i = 0; i < MAX_AUCTION_ITEMS; i ++) if(dAuctionSellerVehicle[i][dItemSale] != 0)
				{
					slot = i;
					break;
				}
				if(slot == -1) return false;
			}
			case 5:
			{
				for(i = 0; i < MAX_AUCTION_ITEMS; i ++) if(dAuctionSellerClothes[i][dItemSale] != 0)
				{
					slot = i;
					break;
				}
				if(slot == -1) return false;
			}
			case 6:
			{
				for(i = 0; i < MAX_AUCTION_ITEMS; i ++) if(dAuctionSellerFood[i][dItemSale] != 0)
				{
					slot = i;
					break;
				}
				if(slot == -1) return false;
			}
			case 7:
			{
				for(i = 0; i < MAX_AUCTION_ITEMS; i ++) if(dAuctionSellerRessource[i][dItemSale] != 0)
				{
					slot = i;
					break;
				}
				if(slot == -1) return false;
			}
	    }
	}
	slot = pHDV[playerid][1];
	switch(category)
	{
		case 0:
		{
			do
			{
				slot += item;
				if(item == 1 && slot == MAX_AUCTION_ITEMS) slot = 0;
				else if(item == -1 && slot == -1) slot = MAX_AUCTION_ITEMS - 1;
			}
			while(dAuctionSellerTool[slot][dItemSale] == 0);
			if(pHDV[playerid][1] == slot && category == pHDV[playerid][0]) return false;
			else pHDV[playerid][1] = slot;
		}
		case 1:
		{
			do
			{
				slot += item;
				if(item == 1 && slot == MAX_AUCTION_ITEMS) slot = 0;
				else if(item == -1 && slot == -1) slot = MAX_AUCTION_ITEMS - 1;
			}
			while(dAuctionSellerMedic[slot][dItemSale] == 0);
			if(pHDV[playerid][1] == slot && category == pHDV[playerid][0]) return false;
			else pHDV[playerid][1] = slot;
		}
		case 2:
		{
			do
			{
				slot += item;
				if(item == 1 && slot == MAX_AUCTION_ITEMS) slot = 0;
				else if(item == -1 && slot == -1) slot = MAX_AUCTION_ITEMS - 1;
			}
			while(dAuctionSellerWeapon[slot][dItemSale] == 0);
			if(pHDV[playerid][1] == slot && category == pHDV[playerid][0]) return false;
			else pHDV[playerid][1] = slot;
		}
		case 3:
		{
			do
			{
				slot += item;
				if(item == 1 && slot == MAX_AUCTION_ITEMS) slot = 0;
				else if(item == -1 && slot == -1) slot = MAX_AUCTION_ITEMS - 1;
			}
			while(dAuctionSellerOther[slot][dItemSale] == 0);
			if(pHDV[playerid][1] == slot && category == pHDV[playerid][0]) return false;
			else pHDV[playerid][1] = slot;
		}
		case 4:
		{
			do
			{
				slot += item;
				if(item == 1 && slot == MAX_AUCTION_ITEMS) slot = 0;
				else if(item == -1 && slot == -1) slot = MAX_AUCTION_ITEMS - 1;
			}
			while(dAuctionSellerVehicle[slot][dItemSale] == 0);
			if(pHDV[playerid][1] == slot && category == pHDV[playerid][0]) return false;
			else pHDV[playerid][1] = slot;
		}
		case 5:
		{
			do
			{
				slot += item;
				if(item == 1 && slot == MAX_AUCTION_ITEMS) slot = 0;
				else if(item == -1 && slot == -1) slot = MAX_AUCTION_ITEMS - 1;
			}
			while(dAuctionSellerClothes[slot][dItemSale] == 0);
			if(pHDV[playerid][1] == slot && category == pHDV[playerid][0]) return false;
			else pHDV[playerid][1] = slot;
		}
		case 6:
		{
			do
			{
				slot += item;
				if(item == 1 && slot == MAX_AUCTION_ITEMS) slot = 0;
				else if(item == -1 && slot == -1) slot = MAX_AUCTION_ITEMS - 1;
			}
			while(dAuctionSellerFood[slot][dItemSale] == 0);
			if(pHDV[playerid][1] == slot && category == pHDV[playerid][0]) return false;
			else pHDV[playerid][1] = slot;
		}
		case 7:
		{
			do
			{
				slot += item;
				if(item == 1 && slot == MAX_AUCTION_ITEMS) slot = 0;
				else if(item == -1 && slot == -1) slot = MAX_AUCTION_ITEMS - 1;
			}
			while(dAuctionSellerRessource[slot][dItemSale] == 0);
			if(pHDV[playerid][1] == slot && category == pHDV[playerid][0]) return false;
			else pHDV[playerid][1] = slot;
		}
	}
	pHDV[playerid][0] = category;
	UpdatePlayerAuctionHouse(playerid, pHDV[playerid][0], pHDV[playerid][1]);
	return true;
}

UpdatePlayerAuctionHouse(playerid, category, item)
{
	new string[128];
	new dAuctionInfos[2];
	//---MENU---//
	for(new i = 0; i < 8; i ++)
	{
		UpdateAuctionCategory(i, pHDV[playerid][0] != i);
	}
	//---
	if(category == 8)
	{
		UpdateAuctionCurrentItem(playerid, 18888, 0.0, 0.0, 0.0, 1.0, "");
	}
	else
	{
		switch(pHDV[playerid][0])
		{
			case 0: dAuctionInfos[0] = dAuctionSellerTool[item][dItemSale], dAuctionInfos[1] = dAuctionSellerTool[item][dItemPrice];
			case 1: dAuctionInfos[0] = dAuctionSellerMedic[item][dItemSale], dAuctionInfos[1] = dAuctionSellerMedic[item][dItemPrice];
			case 2: dAuctionInfos[0] = dAuctionSellerWeapon[item][dItemSale], dAuctionInfos[1] = dAuctionSellerWeapon[item][dItemPrice];
			case 3: dAuctionInfos[0] = dAuctionSellerOther[item][dItemSale], dAuctionInfos[1] = dAuctionSellerOther[item][dItemPrice];
			case 4: dAuctionInfos[0] = dAuctionSellerVehicle[item][dItemSale], dAuctionInfos[1] = dAuctionSellerVehicle[item][dItemPrice];
			case 5: dAuctionInfos[0] = dAuctionSellerClothes[item][dItemSale], dAuctionInfos[1] = dAuctionSellerClothes[item][dItemPrice];
			case 6: dAuctionInfos[0] = dAuctionSellerFood[item][dItemSale], dAuctionInfos[1] = dAuctionSellerFood[item][dItemPrice];
			case 7: dAuctionInfos[0] = dAuctionSellerRessource[item][dItemSale], dAuctionInfos[1] = dAuctionSellerRessource[item][dItemPrice];
		}
		//---
		switch(GetPlayerLanguage(playerid))
		{
		    case LANGUAGE_EN: format(string, sizeof(string), "%s~n~Price: %.1fg", aObjects[dAuctionInfos[0]][ObjectEnName], floatdiv(dAuctionInfos[1], 10));
		    case LANGUAGE_FR: format(string, sizeof(string), "%s~n~Prix: %.1fg", aObjects[dAuctionInfos[0]][ObjectFrName], floatdiv(dAuctionInfos[1], 10));
		    case LANGUAGE_ES: format(string, sizeof(string), "%s~n~Precio: %.1fg", aObjects[dAuctionInfos[0]][ObjectEsName], floatdiv(dAuctionInfos[1], 10));
		    case LANGUAGE_PG: format(string, sizeof(string), "%s~n~Portugais: %.1fg", aObjects[dAuctionInfos[0]][ObjectPgName], floatdiv(dAuctionInfos[1], 10));
		    case LANGUAGE_IT: format(string, sizeof(string), "%s~n~Italien: %.1fg", aObjects[dAuctionInfos[0]][ObjectItName], floatdiv(dAuctionInfos[1], 10));
		    case LANGUAGE_DE: format(string, sizeof(string), "%s~n~Preis: %.1fg", aObjects[dAuctionInfos[0]][ObjectDeName], floatdiv(dAuctionInfos[1], 10));
		}
		UpdateAuctionCurrentItem(playerid, aObjects[dAuctionInfos[0]][ObjectModelID], aObjects[dAuctionInfos[0]][ObjectRotX], aObjects[dAuctionInfos[0]][ObjectRotY], aObjects[dAuctionInfos[0]][ObjectRotZ], aObjects[dAuctionInfos[0]][ObjectZoom], string);
	}
}

AddAuctionHouseItem(playerid, category, item, price, time, slotid = -1)
{
	if(item == 0) return -1;
	//---
    if(slotid == -1)
    {
		for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
		{
			switch(category)
			{
			    case 0: if(dAuctionSellerTool[i][dItemSale] == 0)
				{
					slotid = i;
					break;
				}
			    case 1: if(dAuctionSellerMedic[i][dItemSale] == 0)
				{
					slotid = i;
					break;
				}
			    case 2: if(dAuctionSellerWeapon[i][dItemSale] == 0)
				{
					slotid = i;
					break;
				}
			    case 3: if(dAuctionSellerOther[i][dItemSale] == 0)
				{
					slotid = i;
					break;
				}
			    case 4: if(dAuctionSellerVehicle[i][dItemSale] == 0)
				{
					slotid = i;
					break;
				}
			    case 5: if(dAuctionSellerClothes[i][dItemSale] == 0)
				{
					slotid = i;
					break;
				}
			    case 6: if(dAuctionSellerFood[i][dItemSale] == 0)
				{
					slotid = i;
					break;
				}
			    case 7: if(dAuctionSellerRessource[i][dItemSale] == 0)
				{
					slotid = i;
					break;
				}
			}
		}
    }
    //---
    if(slotid == -1) return -1;
    //---
    switch(category)
    {
        case 0:
        {
            dAuctionSellerTool[slotid][dItemSale] = item;
            dAuctionSellerTool[slotid][dItemPrice] = price;
            dAuctionSellerTool[slotid][dTimeLeft] = time;
            format(dAuctionSellerTool[slotid][sSalesman], MAX_PLAYER_NAME + 1, GetName(playerid));
        }
        case 1:
        {
            dAuctionSellerMedic[slotid][dItemSale] = item;
            dAuctionSellerMedic[slotid][dItemPrice] = price;
            dAuctionSellerMedic[slotid][dTimeLeft] = time;
            format(dAuctionSellerMedic[slotid][sSalesman], MAX_PLAYER_NAME + 1, "%s", GetName(playerid));
        }
        case 2:
        {
            dAuctionSellerWeapon[slotid][dItemSale] = item;
            dAuctionSellerWeapon[slotid][dItemPrice] = price;
            dAuctionSellerWeapon[slotid][dTimeLeft] = time;
            format(dAuctionSellerWeapon[slotid][sSalesman], MAX_PLAYER_NAME + 1, "%s", GetName(playerid));
        }
        case 3:
        {
            dAuctionSellerOther[slotid][dItemSale] = item;
            dAuctionSellerOther[slotid][dItemPrice] = price;
            dAuctionSellerOther[slotid][dTimeLeft] = time;
            format(dAuctionSellerOther[slotid][sSalesman], MAX_PLAYER_NAME + 1, "%s", GetName(playerid));
        }
        case 4:
        {
            dAuctionSellerVehicle[slotid][dItemSale] = item;
            dAuctionSellerVehicle[slotid][dItemPrice] = price;
            dAuctionSellerVehicle[slotid][dTimeLeft] = time;
            format(dAuctionSellerVehicle[slotid][sSalesman], MAX_PLAYER_NAME + 1, "%s", GetName(playerid));
        }
        case 5:
        {
            dAuctionSellerClothes[slotid][dItemSale] = item;
            dAuctionSellerClothes[slotid][dItemPrice] = price;
            dAuctionSellerClothes[slotid][dTimeLeft] = time;
            format(dAuctionSellerClothes[slotid][sSalesman], MAX_PLAYER_NAME + 1, "%s", GetName(playerid));
        }
        case 6:
        {
            dAuctionSellerFood[slotid][dItemSale] = item;
            dAuctionSellerFood[slotid][dItemPrice] = price;
            dAuctionSellerFood[slotid][dTimeLeft] = time;
            format(dAuctionSellerFood[slotid][sSalesman], MAX_PLAYER_NAME + 1, "%s", GetName(playerid));
        }
        case 7:
        {
            dAuctionSellerRessource[slotid][dItemSale] = item;
            dAuctionSellerRessource[slotid][dItemPrice] = price;
            dAuctionSellerRessource[slotid][dTimeLeft] = time;
            format(dAuctionSellerRessource[slotid][sSalesman], MAX_PLAYER_NAME + 1, "%s", GetName(playerid));
        }
    }
	return slotid;
}

RemoveAuctionHouseItem(category, slotid)
{
	if(slotid == -1) return -1;
	//---
    switch(category)
	{
	    case 0: if(dAuctionSellerTool[slotid][dItemSale] == 0) return 0;
	    case 1: if(dAuctionSellerMedic[slotid][dItemSale] == 0) return 0;
	    case 2: if(dAuctionSellerWeapon[slotid][dItemSale] == 0) return 0;
	    case 3: if(dAuctionSellerOther[slotid][dItemSale] == 0) return 0;
	    case 4: if(dAuctionSellerVehicle[slotid][dItemSale] == 0) return 0;
	    case 5: if(dAuctionSellerClothes[slotid][dItemSale] == 0) return 0;
	    case 6: if(dAuctionSellerFood[slotid][dItemSale] == 0) return 0;
	    case 7: if(dAuctionSellerRessource[slotid][dItemSale] == 0) return 0;
	}
    //---
    switch(category)
    {
        case 0:
        {
            dAuctionSellerTool[slotid][dItemSale] = 0;
            dAuctionSellerTool[slotid][dItemPrice] = 0;
            dAuctionSellerTool[slotid][dTimeLeft] = 0;
            format(dAuctionSellerTool[slotid][sSalesman], MAX_PLAYER_NAME + 1, "N/A");
        }
        case 1:
        {
            dAuctionSellerMedic[slotid][dItemSale] = 0;
            dAuctionSellerMedic[slotid][dItemPrice] = 0;
            dAuctionSellerMedic[slotid][dTimeLeft] = 0;
            format(dAuctionSellerMedic[slotid][sSalesman], MAX_PLAYER_NAME + 1, "N/A");
        }
        case 2:
        {
            dAuctionSellerWeapon[slotid][dItemSale] = 0;
            dAuctionSellerWeapon[slotid][dItemPrice] = 0;
            dAuctionSellerWeapon[slotid][dTimeLeft] = 0;
            format(dAuctionSellerWeapon[slotid][sSalesman], MAX_PLAYER_NAME + 1, "N/A");
        }
        case 3:
        {
            dAuctionSellerOther[slotid][dItemSale] = 0;
            dAuctionSellerOther[slotid][dItemPrice] = 0;
            dAuctionSellerOther[slotid][dTimeLeft] = 0;
            format(dAuctionSellerOther[slotid][sSalesman], MAX_PLAYER_NAME + 1, "N/A");
        }
        case 4:
        {
            dAuctionSellerVehicle[slotid][dItemSale] = 0;
            dAuctionSellerVehicle[slotid][dItemPrice] = 0;
            dAuctionSellerVehicle[slotid][dTimeLeft] = 0;
            format(dAuctionSellerVehicle[slotid][sSalesman], MAX_PLAYER_NAME + 1, "N/A");
        }
        case 5:
        {
            dAuctionSellerClothes[slotid][dItemSale] = 0;
            dAuctionSellerClothes[slotid][dItemPrice] = 0;
            dAuctionSellerClothes[slotid][dTimeLeft] = 0;
            format(dAuctionSellerClothes[slotid][sSalesman], MAX_PLAYER_NAME + 1, "N/A");
        }
        case 6:
        {
            dAuctionSellerFood[slotid][dItemSale] = 0;
            dAuctionSellerFood[slotid][dItemPrice] = 0;
            dAuctionSellerFood[slotid][dTimeLeft] = 0;
            format(dAuctionSellerFood[slotid][sSalesman], MAX_PLAYER_NAME + 1, "N/A");
        }
        case 7:
        {
            dAuctionSellerRessource[slotid][dItemSale] = 0;
            dAuctionSellerRessource[slotid][dItemPrice] = 0;
            dAuctionSellerRessource[slotid][dTimeLeft] = 0;
            format(dAuctionSellerRessource[slotid][sSalesman], MAX_PLAYER_NAME + 1, "N/A");
        }
    }
    return 1;
}

BuyAuctionHouseItem(playerid, category, itemid)
{
	new dPrice, dItemID, sSeller[MAX_PLAYER_NAME + 1];
    switch(category)
    {
        case 0:
        {
    		new dFreeSlot = GetPlayerNextFreeSlot(playerid);
    		if(dFreeSlot == -1) return -2;
            if(!HasPlayerGold(playerid, dAuctionSellerTool[itemid][dItemPrice])) return -1;
			if(dAuctionSellerTool[itemid][dItemSale] == 0) return 0;
            //---
            dPrice = dAuctionSellerTool[itemid][dItemPrice];
            strcpy(sSeller, dAuctionSellerTool[itemid][sSalesman]);
            dItemID = dAuctionSellerTool[itemid][dItemSale];
            //---
    		GivePlayerGold(playerid, -dAuctionSellerTool[itemid][dItemPrice]);
    		GivePlayerSlotObject(playerid, dAuctionSellerTool[itemid][dItemSale], dFreeSlot);
    		PayAuctionSeller(dAuctionSellerTool[itemid][sSalesman], dAuctionSellerTool[itemid][dItemPrice]);
        }
        case 1:
        {
    		new dFreeSlot = GetPlayerNextFreeSlot(playerid);
    		if(dFreeSlot == -1) return -2;
            if(!HasPlayerGold(playerid, dAuctionSellerMedic[itemid][dItemPrice])) return -1;
            if(dAuctionSellerMedic[itemid][dItemSale] == 0) return 0;
            //---
            dPrice = dAuctionSellerMedic[itemid][dItemPrice];
            strcpy(sSeller, dAuctionSellerMedic[itemid][sSalesman]);
            dItemID = dAuctionSellerMedic[itemid][dItemSale];
            //---
    		GivePlayerGold(playerid, -dAuctionSellerMedic[itemid][dItemPrice]);
    		GivePlayerSlotObject(playerid, dAuctionSellerMedic[itemid][dItemSale], dFreeSlot);
    		PayAuctionSeller(dAuctionSellerMedic[itemid][sSalesman], dAuctionSellerMedic[itemid][dItemPrice]);
        }
        case 2:
        {
    		new dFreeSlot = GetPlayerNextFreeSlot(playerid);
    		if(dFreeSlot == -1) return -2;
            if(!HasPlayerGold(playerid, dAuctionSellerWeapon[itemid][dItemPrice])) return -1;
            if(dAuctionSellerWeapon[itemid][dItemSale] == 0) return 0;
            //---
            dPrice = dAuctionSellerWeapon[itemid][dItemPrice];
            strcpy(sSeller, dAuctionSellerWeapon[itemid][sSalesman]);
            dItemID = dAuctionSellerWeapon[itemid][dItemSale];
            //---
    		GivePlayerGold(playerid, -dAuctionSellerWeapon[itemid][dItemPrice]);
    		GivePlayerSlotObject(playerid, dAuctionSellerWeapon[itemid][dItemSale], dFreeSlot);
    		PayAuctionSeller(dAuctionSellerWeapon[itemid][sSalesman], dAuctionSellerWeapon[itemid][dItemPrice]);
        }
        case 3:
        {
    		new dFreeSlot = GetPlayerNextFreeSlot(playerid);
    		if(dFreeSlot == -1) return -2;
            if(!HasPlayerGold(playerid, dAuctionSellerOther[itemid][dItemPrice])) return -1;
            if(dAuctionSellerOther[itemid][dItemSale] == 0) return 0;
            //---
            dPrice = dAuctionSellerOther[itemid][dItemPrice];
            strcpy(sSeller, dAuctionSellerOther[itemid][sSalesman]);
            dItemID = dAuctionSellerOther[itemid][dItemSale];
            //---
    		GivePlayerGold(playerid, -dAuctionSellerOther[itemid][dItemPrice]);
    		GivePlayerSlotObject(playerid, dAuctionSellerOther[itemid][dItemSale], dFreeSlot);
    		PayAuctionSeller(dAuctionSellerOther[itemid][sSalesman], dAuctionSellerOther[itemid][dItemPrice]);
        }
        case 4:
        {
    		new dFreeSlot = GetPlayerNextFreeSlot(playerid);
    		if(dFreeSlot == -1) return -2;
            if(!HasPlayerGold(playerid, dAuctionSellerVehicle[itemid][dItemPrice])) return -1;
            if(dAuctionSellerVehicle[itemid][dItemSale] == 0) return 0;
            //---
            dPrice = dAuctionSellerVehicle[itemid][dItemPrice];
            strcpy(sSeller, dAuctionSellerVehicle[itemid][sSalesman]);
            dItemID = dAuctionSellerVehicle[itemid][dItemSale];
            //---
    		GivePlayerGold(playerid, -dAuctionSellerVehicle[itemid][dItemPrice]);
    		GivePlayerSlotObject(playerid, dAuctionSellerVehicle[itemid][dItemSale], dFreeSlot);
    		PayAuctionSeller(dAuctionSellerVehicle[itemid][sSalesman], dAuctionSellerVehicle[itemid][dItemPrice]);
        }
        case 5:
        {
    		new dFreeSlot = GetPlayerNextFreeSlot(playerid);
    		if(dFreeSlot == -1) return -2;
            if(!HasPlayerGold(playerid, dAuctionSellerClothes[itemid][dItemPrice])) return -1;
            if(dAuctionSellerClothes[itemid][dItemSale] == 0) return 0;
            //---
            dPrice = dAuctionSellerClothes[itemid][dItemPrice];
            strcpy(sSeller, dAuctionSellerClothes[itemid][sSalesman]);
            dItemID = dAuctionSellerClothes[itemid][dItemSale];
            //---
    		GivePlayerGold(playerid, -dAuctionSellerClothes[itemid][dItemPrice]);
    		GivePlayerSlotObject(playerid, dAuctionSellerClothes[itemid][dItemSale], dFreeSlot);
    		PayAuctionSeller(dAuctionSellerClothes[itemid][sSalesman], dAuctionSellerClothes[itemid][dItemPrice]);
        }
        case 6:
        {
    		new dFreeSlot = GetPlayerNextFreeSlot(playerid);
    		if(dFreeSlot == -1) return -2;
            if(!HasPlayerGold(playerid, dAuctionSellerFood[itemid][dItemPrice])) return -1;
            if(dAuctionSellerFood[itemid][dItemSale] == 0) return 0;
            //---
            dPrice = dAuctionSellerFood[itemid][dItemPrice];
            strcpy(sSeller, dAuctionSellerFood[itemid][sSalesman]);
            dItemID = dAuctionSellerFood[itemid][dItemSale];
            //---
    		GivePlayerGold(playerid, -dAuctionSellerFood[itemid][dItemPrice]);
    		GivePlayerSlotObject(playerid, dAuctionSellerFood[itemid][dItemSale], dFreeSlot);
    		PayAuctionSeller(dAuctionSellerFood[itemid][sSalesman], dAuctionSellerFood[itemid][dItemPrice]);
        }
        case 7:
        {
    		new dFreeSlot = GetPlayerNextFreeSlot(playerid);
    		if(dFreeSlot == -1) return -2;
            if(!HasPlayerGold(playerid, dAuctionSellerRessource[itemid][dItemPrice])) return -1;
            if(dAuctionSellerRessource[itemid][dItemSale] == 0) return 0;
            //---
            dPrice = dAuctionSellerRessource[itemid][dItemPrice];
            strcpy(sSeller, dAuctionSellerRessource[itemid][sSalesman]);
            dItemID = dAuctionSellerRessource[itemid][dItemSale];
            //---
    		GivePlayerGold(playerid, -dAuctionSellerRessource[itemid][dItemPrice]);
    		GivePlayerSlotObject(playerid, dAuctionSellerRessource[itemid][dItemSale], dFreeSlot);
    		PayAuctionSeller(dAuctionSellerRessource[itemid][sSalesman], dAuctionSellerRessource[itemid][dItemPrice]);
        }
    }
	LogInfo(true, "[JOUEUR]%s achete le %s de %s pour %.1fg d'or a l'HDV.", GetName(playerid), NoNewLineSign(aObjects[dItemID][ObjectFrName]), sSeller, floatdiv(dPrice, 10));
    RemoveAuctionHouseItem(category, itemid);
	HidePlayerAuctionHouse(playerid);
    return 1;
}

PayAuctionSeller(const sellersname[], amount)
{
	new playerid = PlayeridFromName(sellersname);
	LogInfo(true, "Test: %s - %d", sellersname, playerid);
	if(playerid == INVALID_PLAYER_ID)//Si le vendeur n'est pas connecté
	{
	    GiveOfflinePlayerGold(sellersname, amount);
	}
	else//S'il est connecté
	{
	    new string[128];
	    GivePlayerGold(playerid, amount);
		switch(GetPlayerLanguage(playerid))
		{
		    case LANGUAGE_EN: format(string, sizeof(string), "[SALE]Your sale has paid you {FFFF00}%.1fg {784F10}of gold!", floatdiv(amount, 10));
		    case LANGUAGE_FR: format(string, sizeof(string), "[VENTE]Votre vente vous a rapporté {FFFF00}%.1fg {784F10}d'or !", floatdiv(amount, 10));
		    case LANGUAGE_ES: format(string, sizeof(string), "%s~n~Espagnol: %.1fg", floatdiv(amount, 10));
		    case LANGUAGE_PG: format(string, sizeof(string), "%s~n~Portugais: %.1fg", floatdiv(amount, 10));
		    case LANGUAGE_IT: format(string, sizeof(string), "%s~n~Italien: %.1fg", floatdiv(amount, 10));
		    case LANGUAGE_DE: format(string, sizeof(string), "%s~n~Allemand: %.1fg", floatdiv(amount, 10));
		}
		SendClientMessageEx(playerid, BRUN, string, string, string, string, string, string);
	}
}

public LoadAuctionHouse_data(name[],value[])
{
	new string[50];
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "ToolID%d", i);
		INI_Int(string, dAuctionSellerTool[i][dItemSale]);
	    format(string, sizeof(string), "ToolPrice%d", i);
		INI_Int(string, dAuctionSellerTool[i][dItemPrice]);
	    format(string, sizeof(string), "ToolTime%d", i);
		INI_Int(string, dAuctionSellerTool[i][dTimeLeft]);
	    format(string, sizeof(string), "ToolVendor%d", i);
		INI_String(string, dAuctionSellerTool[i][sSalesman], MAX_PLAYER_NAME + 1);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "MedicID%d", i);
		INI_Int(string, dAuctionSellerMedic[i][dItemSale]);
	    format(string, sizeof(string), "MedicPrice%d", i);
		INI_Int(string, dAuctionSellerMedic[i][dItemPrice]);
	    format(string, sizeof(string), "MedicTime%d", i);
		INI_Int(string, dAuctionSellerMedic[i][dTimeLeft]);
	    format(string, sizeof(string), "MedicVendor%d", i);
		INI_String(string, dAuctionSellerMedic[i][sSalesman], MAX_PLAYER_NAME + 1);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "WeaponID%d", i);
		INI_Int(string, dAuctionSellerWeapon[i][dItemSale]);
	    format(string, sizeof(string), "WeaponPrice%d", i);
		INI_Int(string, dAuctionSellerWeapon[i][dItemPrice]);
	    format(string, sizeof(string), "WeaponTime%d", i);
		INI_Int(string, dAuctionSellerWeapon[i][dTimeLeft]);
	    format(string, sizeof(string), "WeaponVendor%d", i);
		INI_String(string, dAuctionSellerWeapon[i][sSalesman], MAX_PLAYER_NAME + 1);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "OtherID%d", i);
		INI_Int(string, dAuctionSellerOther[i][dItemSale]);
	    format(string, sizeof(string), "OtherPrice%d", i);
		INI_Int(string, dAuctionSellerOther[i][dItemPrice]);
	    format(string, sizeof(string), "OtherTime%d", i);
		INI_Int(string, dAuctionSellerOther[i][dTimeLeft]);
	    format(string, sizeof(string), "OtherVendor%d", i);
		INI_String(string, dAuctionSellerOther[i][sSalesman], MAX_PLAYER_NAME + 1);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "VehicleID%d", i);
		INI_Int(string, dAuctionSellerVehicle[i][dItemSale]);
	    format(string, sizeof(string), "VehiclePrice%d", i);
		INI_Int(string, dAuctionSellerVehicle[i][dItemPrice]);
	    format(string, sizeof(string), "VehicleTime%d", i);
		INI_Int(string, dAuctionSellerVehicle[i][dTimeLeft]);
	    format(string, sizeof(string), "VehicleVendor%d", i);
		INI_String(string, dAuctionSellerVehicle[i][sSalesman], MAX_PLAYER_NAME + 1);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "ClotheID%d", i);
		INI_Int(string, dAuctionSellerClothes[i][dItemSale]);
	    format(string, sizeof(string), "ClothePrice%d", i);
		INI_Int(string, dAuctionSellerClothes[i][dItemPrice]);
	    format(string, sizeof(string), "ClotheTime%d", i);
		INI_Int(string, dAuctionSellerClothes[i][dTimeLeft]);
	    format(string, sizeof(string), "ClotheVendor%d", i);
		INI_String(string, dAuctionSellerClothes[i][sSalesman], MAX_PLAYER_NAME + 1);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "FoodID%d", i);
		INI_Int(string, dAuctionSellerFood[i][dItemSale]);
	    format(string, sizeof(string), "FoodPrice%d", i);
		INI_Int(string, dAuctionSellerFood[i][dItemPrice]);
	    format(string, sizeof(string), "FoodTime%d", i);
		INI_Int(string, dAuctionSellerFood[i][dTimeLeft]);
	    format(string, sizeof(string), "FoodVendor%d", i);
		INI_String(string, dAuctionSellerFood[i][sSalesman], MAX_PLAYER_NAME + 1);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "RessID%d", i);
		INI_Int(string, dAuctionSellerRessource[i][dItemSale]);
	    format(string, sizeof(string), "RessPrice%d", i);
		INI_Int(string, dAuctionSellerRessource[i][dItemPrice]);
	    format(string, sizeof(string), "RessTime%d", i);
		INI_Int(string, dAuctionSellerRessource[i][dTimeLeft]);
	    format(string, sizeof(string), "RessVendor%d", i);
		INI_String(string, dAuctionSellerRessource[i][sSalesman], MAX_PLAYER_NAME + 1);
	}
	return 1;
}

SaveAuctionHouse()
{
	new string[50];
	new INI:File = INI_Open(APATH);
	INI_SetTag(File, "data");
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "ToolID%d", i);
		INI_WriteInt(File, string, dAuctionSellerTool[i][dItemSale]);
	    format(string, sizeof(string), "ToolPrice%d", i);
		INI_WriteInt(File, string, dAuctionSellerTool[i][dItemPrice]);
	    format(string, sizeof(string), "ToolTime%d", i);
		INI_WriteInt(File, string, dAuctionSellerTool[i][dTimeLeft]);
	    format(string, sizeof(string), "ToolVendor%d", i);
		INI_WriteString(File, string, dAuctionSellerTool[i][sSalesman]);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "MedicID%d", i);
		INI_WriteInt(File, string, dAuctionSellerMedic[i][dItemSale]);
	    format(string, sizeof(string), "MedicPrice%d", i);
		INI_WriteInt(File, string, dAuctionSellerMedic[i][dItemPrice]);
	    format(string, sizeof(string), "MedicTime%d", i);
		INI_WriteInt(File, string, dAuctionSellerMedic[i][dTimeLeft]);
	    format(string, sizeof(string), "MedicVendor%d", i);
		INI_WriteString(File, string, dAuctionSellerMedic[i][sSalesman]);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "WeaponID%d", i);
		INI_WriteInt(File, string, dAuctionSellerWeapon[i][dItemSale]);
	    format(string, sizeof(string), "WeaponPrice%d", i);
		INI_WriteInt(File, string, dAuctionSellerWeapon[i][dItemPrice]);
	    format(string, sizeof(string), "WeaponTime%d", i);
		INI_WriteInt(File, string, dAuctionSellerWeapon[i][dTimeLeft]);
	    format(string, sizeof(string), "WeaponVendor%d", i);
		INI_WriteString(File, string, dAuctionSellerWeapon[i][sSalesman]);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "OtherID%d", i);
		INI_WriteInt(File, string, dAuctionSellerOther[i][dItemSale]);
	    format(string, sizeof(string), "OtherPrice%d", i);
		INI_WriteInt(File, string, dAuctionSellerOther[i][dItemPrice]);
	    format(string, sizeof(string), "OtherTime%d", i);
		INI_WriteInt(File, string, dAuctionSellerOther[i][dTimeLeft]);
	    format(string, sizeof(string), "OtherVendor%d", i);
		INI_WriteString(File, string, dAuctionSellerOther[i][sSalesman]);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "VehicleID%d", i);
		INI_WriteInt(File, string, dAuctionSellerVehicle[i][dItemSale]);
	    format(string, sizeof(string), "VehiclePrice%d", i);
		INI_WriteInt(File, string, dAuctionSellerVehicle[i][dItemPrice]);
	    format(string, sizeof(string), "VehicleTime%d", i);
		INI_WriteInt(File, string, dAuctionSellerVehicle[i][dTimeLeft]);
	    format(string, sizeof(string), "VehicleVendor%d", i);
		INI_WriteString(File, string, dAuctionSellerVehicle[i][sSalesman]);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "ClotheID%d", i);
		INI_WriteInt(File, string, dAuctionSellerClothes[i][dItemSale]);
	    format(string, sizeof(string), "ClothePrice%d", i);
		INI_WriteInt(File, string, dAuctionSellerClothes[i][dItemPrice]);
	    format(string, sizeof(string), "ClotheTime%d", i);
		INI_WriteInt(File, string, dAuctionSellerClothes[i][dTimeLeft]);
	    format(string, sizeof(string), "ClotheVendor%d", i);
		INI_WriteString(File, string, dAuctionSellerClothes[i][sSalesman]);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "FoodID%d", i);
		INI_WriteInt(File, string, dAuctionSellerFood[i][dItemSale]);
	    format(string, sizeof(string), "FoodPrice%d", i);
		INI_WriteInt(File, string, dAuctionSellerFood[i][dItemPrice]);
	    format(string, sizeof(string), "FoodTime%d", i);
		INI_WriteInt(File, string, dAuctionSellerFood[i][dTimeLeft]);
	    format(string, sizeof(string), "FoodVendor%d", i);
		INI_WriteString(File, string, dAuctionSellerFood[i][sSalesman]);
	}
	//---
	for(new i = 0; i < MAX_AUCTION_ITEMS; i ++)
	{
	    format(string, sizeof(string), "RessID%d", i);
		INI_WriteInt(File, string, dAuctionSellerRessource[i][dItemSale]);
	    format(string, sizeof(string), "RessPrice%d", i);
		INI_WriteInt(File, string, dAuctionSellerRessource[i][dItemPrice]);
	    format(string, sizeof(string), "RessTime%d", i);
		INI_WriteInt(File, string, dAuctionSellerRessource[i][dTimeLeft]);
	    format(string, sizeof(string), "RessVendor%d", i);
		INI_WriteString(File, string, dAuctionSellerRessource[i][sSalesman]);
	}
	INI_Close(File);
}

stock ReturnPlayerItem(category, itemid)
{
	new playerid;
	switch(category)
	{
		case 0:
		{
			playerid = PlayeridFromName(dAuctionSellerTool[itemid][sSalesman]);
			if(playerid == INVALID_PLAYER_ID)
			{
				ReturnOfflinePlayerItem(dAuctionSellerTool[itemid][sSalesman], dAuctionSellerTool[itemid][dItemSale]);
			}
			else
			{
				ReturnOnlinePlayerItem(playerid, dAuctionSellerTool[itemid][dItemSale]);
			}
		}
		case 1:
		{
			playerid = PlayeridFromName(dAuctionSellerMedic[itemid][sSalesman]);
			if(playerid == INVALID_PLAYER_ID)
			{
				ReturnOfflinePlayerItem(dAuctionSellerMedic[itemid][sSalesman], dAuctionSellerMedic[itemid][dItemSale]);
			}
			else
			{
				ReturnOnlinePlayerItem(playerid, dAuctionSellerMedic[itemid][dItemSale]);
			}
		}
		case 2:
		{
			playerid = PlayeridFromName(dAuctionSellerWeapon[itemid][sSalesman]);
			if(playerid == INVALID_PLAYER_ID)
			{
				ReturnOfflinePlayerItem(dAuctionSellerWeapon[itemid][sSalesman], dAuctionSellerWeapon[itemid][dItemSale]);
			}
			else
			{
				ReturnOnlinePlayerItem(playerid, dAuctionSellerWeapon[itemid][dItemSale]);
			}
		}
		case 3:
		{
			playerid = PlayeridFromName(dAuctionSellerOther[itemid][sSalesman]);
			if(playerid == INVALID_PLAYER_ID)
			{
				ReturnOfflinePlayerItem(dAuctionSellerOther[itemid][sSalesman], dAuctionSellerOther[itemid][dItemSale]);
			}
			else
			{
				ReturnOnlinePlayerItem(playerid, dAuctionSellerOther[itemid][dItemSale]);
			}
		}
		case 4:
		{
			playerid = PlayeridFromName(dAuctionSellerVehicle[itemid][sSalesman]);
			if(playerid == INVALID_PLAYER_ID)
			{
				ReturnOfflinePlayerItem(dAuctionSellerVehicle[itemid][sSalesman], dAuctionSellerVehicle[itemid][dItemSale]);
			}
			else
			{
				ReturnOnlinePlayerItem(playerid, dAuctionSellerVehicle[itemid][dItemSale]);
			}
		}
		case 5:
		{
			playerid = PlayeridFromName(dAuctionSellerClothes[itemid][sSalesman]);
			if(playerid == INVALID_PLAYER_ID)
			{
				ReturnOfflinePlayerItem(dAuctionSellerClothes[itemid][sSalesman], dAuctionSellerClothes[itemid][dItemSale]);
			}
			else
			{
				ReturnOnlinePlayerItem(playerid, dAuctionSellerClothes[itemid][dItemSale]);
			}
		}
		case 6:
		{
			playerid = PlayeridFromName(dAuctionSellerFood[itemid][sSalesman]);
			if(playerid == INVALID_PLAYER_ID)
			{
				ReturnOfflinePlayerItem(dAuctionSellerFood[itemid][sSalesman], dAuctionSellerFood[itemid][dItemSale]);
			}
			else
			{
				ReturnOnlinePlayerItem(playerid, dAuctionSellerFood[itemid][dItemSale]);
			}
		}
		case 7:
		{
			playerid = PlayeridFromName(dAuctionSellerRessource[itemid][sSalesman]);
			if(playerid == INVALID_PLAYER_ID)
			{
				ReturnOfflinePlayerItem(dAuctionSellerRessource[itemid][sSalesman], dAuctionSellerRessource[itemid][dItemSale]);
			}
			else
			{
				ReturnOnlinePlayerItem(playerid, dAuctionSellerRessource[itemid][dItemSale]);
			}
		}
	}
    RemoveAuctionHouseItem(category, itemid);
}

stock ShowPlayerAuctionHouse(playerid)
{
	CreatePlayerHDV(playerid);
	pHDV[playerid][0] = 8;
	pHDV[playerid][1] = 0;
	SetPlayerAuctionHouse(playerid, 8, 0);
	SelectTextDraw(playerid, VERT);
}

stock ClosePlayerAuctionHouse(playerid) 
{
	pHDV[playerid][0] = -1;
	pHDV[playerid][1] = -1;
}

stock bool:IsValidObjectID(objectid) 
{
	return (MAX_ITEMS > objectid >= 0);
}

stock GetObjectModelID(objectid) 
{
	return IsValidObjectID(objectid) ? aObjects[objectid][ObjectModelID] : 19300;
}

stock GetObjectRotation(objectid, &Float:x, &Float:y, &Float:z, &Float:zoom) 
{
	if(IsValidObjectID(objectid)) 
	{
		x = aObjects[objectid][ObjectRotX];
		y = aObjects[objectid][ObjectRotY];
		z = aObjects[objectid][ObjectRotZ];
		zoom = aObjects[objectid][ObjectZoom];
	}
	else 
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		zoom = 1.0;
	}
}

stock GetObjectHandPos(objectid, &Float:x, &Float:y, &Float:z, &Float:rx, &Float:ry, &Float:rz, &Float:zoom)
{
	if(IsValidObjectID(objectid)) 
	{
		x = aObjects[objectid][HandOffSetX];
		y = aObjects[objectid][HandOffSetY];
		z = aObjects[objectid][HandOffSetZ];
		x = aObjects[objectid][HandRotX];
		y = aObjects[objectid][HandRotY];
		z = aObjects[objectid][HandRotZ];
		zoom = aObjects[objectid][HandZoom];
	}
	else 
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		rx = 0.0;
		ry = 0.0;
		rz = 0.0;
		zoom = 1.0;
	}
}

stock GetObjectGroundPos(objectid, &Float:rx, &Float:ry, &Float:rz, &Float:z)
{
	if(IsValidObjectID(objectid)) 
	{
		rx = aObjects[objectid][GroundRotX];
		ry = aObjects[objectid][GroundRotY];
		rz = aObjects[objectid][GroundRotZ];
		z = aObjects[objectid][GroundOffSetZ];
	}
	else 
	{
		rx = 0.0;
		ry = 0.0;
		rz = 0.0;
		z = 1.0;
	}
}

stock GetObjectSellPrice(objectid)
{
	return IsValidObjectID(objectid) ? aObjects[objectid][dSellPrice] : 0;
}

stock GetObjectType(objectid)
{
	return IsValidObjectID(objectid) ? aObjects[objectid][dObjectType] : 0;
}

stock IsObjectHeavy(objectid)
{
	return IsValidObjectID(objectid) && aObjects[objectid][bHeavy];
}

stock GetPlayerVehicleInventory(playerid)
{
	return IsValidPlayer(playerid) ? pVehicleInventory[playerid] : -1;
}

stock SetPlayerVehicleInventory(playerid, vehicleid)
{
	if(IsValidPlayer(playerid))
	{
		pVehicleInventory[playerid] = vehicleid;
	}
}

/*public GetObjectName(playerid, objectid, language)
{
	if(IsValidObjectID(objectid))
	{
		switch(language)
		{
			case LANGUAGE_EN: SetPVarString(playerid, "sShopName", (IsValidObjectID(objectid) ? aObjects[objectid][ObjectEnName] : "Nothing"));
			case LANGUAGE_FR: SetPVarString(playerid, "sShopName", (IsValidObjectID(objectid) ? aObjects[objectid][ObjectFrName] : "Rien"));
			case LANGUAGE_ES: SetPVarString(playerid, "sShopName", (IsValidObjectID(objectid) ? aObjects[objectid][ObjectEsName] : "Nada"));
			case LANGUAGE_PG: SetPVarString(playerid, "sShopName", (IsValidObjectID(objectid) ? aObjects[objectid][ObjectPgName] : "Nada"));
			case LANGUAGE_IT: SetPVarString(playerid, "sShopName", (IsValidObjectID(objectid) ? aObjects[objectid][ObjectItName] : "Niente"));
			case LANGUAGE_DE: SetPVarString(playerid, "sShopName", (IsValidObjectID(objectid) ? aObjects[objectid][ObjectDeName] : "Nichts"));
			default: SetPVarString(playerid, "sShopName", (IsValidObjectID(objectid) ? aObjects[objectid][ObjectEnName] : "Nothing"));
		}
	}
}*/

stock GetObjectModelName(objectid, language) 
{
	new retVal[30] = "";
	/*switch(language)
	{
		case LANGUAGE_EN: return IsValidObjectID(objectid) ? aObjects[objectid][ObjectEnName] : "Nothing";
		case LANGUAGE_FR: return IsValidObjectID(objectid) ? aObjects[objectid][ObjectFrName] : "Rien";
		case LANGUAGE_ES: return IsValidObjectID(objectid) ? aObjects[objectid][ObjectEsName] : "Nada";
		case LANGUAGE_PG: return IsValidObjectID(objectid) ? aObjects[objectid][ObjectPgName] : "Nada";
		case LANGUAGE_IT: return IsValidObjectID(objectid) ? aObjects[objectid][ObjectItName] : "Niente";
		case LANGUAGE_DE: return IsValidObjectID(objectid) ? aObjects[objectid][ObjectDeName] : "Nichts";
		default: return IsValidObjectID(objectid) ? aObjects[objectid][ObjectEnName] : "Nothing";
	}*/
	return retVal;
}

stock UsePlayerItem(playerid, slot = 0)//Fonction à appeler lorsque le mec appuie sur la touche permettant d'use l'objet dans sa main
{
	new objectid = -1;
	objectid = GetPlayerSlotObject(playerid, slot);
	dUsingItem[playerid] = slot;
	//---
	if(objectid != 0) LogInfo(true, "[JOUEUR]%s utilise %s...", GetName(playerid), NoNewLineSign(aObjects[objectid][ObjectFrName]));
	//---
	SetPlayerHelpState(playerid, 1, true);
	//---
	switch(objectid)
	{
	    case 0://RIEN
	    {
	        SendClientMessageEx(playerid, ROUGE, "You don't have anything in your hand!", "Vous n'avez rien dans la main !", "¡ No tiene nada en la mano !", "Não tem nada na mão !", "Non hai niente in mano !", "Sie haben nichts in der Hand!");
	    }
	    case 1://TENTE
	    {
			if(IsPlayerInAnyVehicle(playerid))
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot set a tent while you're in a vehicle!", "Vous ne pouvez pas monter une tente dans un véhicule !", "Espagnol", "Portugais", "Italien", "Allemand");
			    return 1;
			}
	        new Float:x, Float:y, Float:z, Float:a;
	        GetPlayerPos(playerid, x, y, z);
	        GetPlayerFacingAngle(playerid, a);
			ApplyAnimation(playerid, "GRENADE", "WEAPON_throwu", 4.0, 0, 0, 0, 0, 0, 1);
	        CreateTent(x, y, z, a);
			GivePlayerSlotObject(playerid, -1, slot);
	    }
	    case 2://MEDIKIT
	    {
	    	ApplyAnimation(playerid,"FAT","IDLE_tired", 4.0, 0, 0, 0, 0, 0);
			SetPlayerLegState(playerid, true);
			SetPlayerBleedingState(playerid, false);
			GivePlayerSlotObject(playerid, -1, slot);
			SetHealthForPlayer(playerid, 1250, REASON_HEAL);
		}
	    case 3://MATRAQUE
		{
			if(HasPlayerWeapon(playerid, 3) || HasPlayerWeapon(playerid, 4) || HasPlayerWeapon(playerid, 5) || HasPlayerWeapon(playerid, 8) || HasPlayerWeapon(playerid, 9))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			GivePlayerSlotObject(playerid, -1, slot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 3, 1), WEAPON_SIMPLE);
		}
	    case 4://COUTEAU
		{
			if(HasPlayerWeapon(playerid, 3) || HasPlayerWeapon(playerid, 4) || HasPlayerWeapon(playerid, 5) || HasPlayerWeapon(playerid, 8) || HasPlayerWeapon(playerid, 9))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			GivePlayerSlotObject(playerid, -1, slot);
			new dWeaponSlot = GivePlayerWeaponEx(playerid, 4, 1);
			SetPlayerWeaponSkill(playerid, dWeaponSlot, WEAPON_SIMPLE);
		}
		case 5://BATTE
		{
			if(HasPlayerWeapon(playerid, 3) || HasPlayerWeapon(playerid, 4) || HasPlayerWeapon(playerid, 5) || HasPlayerWeapon(playerid, 8) || HasPlayerWeapon(playerid, 9))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			GivePlayerSlotObject(playerid, -1, slot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 5, 1), WEAPON_SIMPLE);
		}
		case 6://KATANA
		{
			if(HasPlayerWeapon(playerid, 3) || HasPlayerWeapon(playerid, 4) || HasPlayerWeapon(playerid, 5) || HasPlayerWeapon(playerid, 8) || HasPlayerWeapon(playerid, 9))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			GivePlayerSlotObject(playerid, -1, slot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 8, 1), WEAPON_SIMPLE);
		}
		case 7://TRONCONNEUSE
		{
			if(HasPlayerWeapon(playerid, 3) || HasPlayerWeapon(playerid, 4) || HasPlayerWeapon(playerid, 5) || HasPlayerWeapon(playerid, 8) || HasPlayerWeapon(playerid, 9))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dGasSlot = HasPlayerItem(playerid, 30);
			if(dGasSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need gas for using a chainsaw!", "Vous avez besoin d'essence pour une tronçonneuse !", "¡Necessita tener gasolina!", "O senhor precisa gasolina para uma serra de motoserra !", "Italien", "Sie müssen Benzin haben!");
			    return 1;
			}
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dGasSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 9, 1), WEAPON_SIMPLE);
		}
		case 8://GRENADES
		{
		    if(IsPlayerInAnyVehicle(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You can't throw anything while in a vehicle!", "Vous ne pouvez pas lancer de projectiles depuis un véhicule !", "Espagnol", "Portugais", "Italien", "Allemand");
		        return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			//GivePlayerWeapon(playerid, 16, 1);
			PlayerThrowProjectile(playerid, THROW_GRENADE);
		}
		case 9://MOLOTOVS
		{
		    if(IsPlayerInAnyVehicle(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You can't throw anything while in a vehicle!", "Vous ne pouvez pas lancer de projectiles depuis un véhicule !", "Espagnol", "Portugais", "Italien", "Allemand");
		        return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			//GivePlayerWeapon(playerid, 18, 1);
			PlayerThrowProjectile(playerid, THROW_MOLOTOV);
		}
		case 10://PISTOLETS
		{
			if(HasPlayerWeapon(playerid, 23) || HasPlayerWeapon(playerid, 24))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			//---
			new dWeaponSlot = HasPlayerWeapon(playerid, 22);
			if(dWeaponSlot != 0)
			{
			    if(GetPlayerWeaponSkill(playerid, dWeaponSlot) == WEAPON_AKIMBO)
			    {
			    	SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    	return 1;
			    }
				ApplyReloadAnim(playerid, 22);
	    		SetPlayerWeaponSkill(playerid, dWeaponSlot, WEAPON_AKIMBO);
				GivePlayerSlotObject(playerid, -1, slot);
	    		return 1;
			}
			//---
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 26);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 9mm ammo for this gun!", "Vous avez besoin de munitions 9mm !", "¡Necessita tener 9mm munición!", "Portugais", "Italien", "Sie müssen 9mm Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 22);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 22, 25), WEAPON_SIMPLE);
		}
		case 11://SILENCIEUX
		{
			if(HasPlayerWeapon(playerid, 22) || HasPlayerWeapon(playerid, 23) || HasPlayerWeapon(playerid, 24))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 26);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 9mm ammo for this gun!", "Vous avez besoin de munitions 9mm !", "¡Necessita tener 9mm munición!", "Portugais", "Italien", "Sie müssen 9mm Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 23);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 23, 25), WEAPON_SIMPLE);
		}
		case 12://DESERT EAGLE
		{
			if(HasPlayerWeapon(playerid, 22) || HasPlayerWeapon(playerid, 23) || HasPlayerWeapon(playerid, 24))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 27);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need .50AE ammo for this gun!", "Vous avez besoin de munitions .50AE !", "¡Necessita tener .50AE munición!", "Portugais", "Italien", "Sie müssen .50AE Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 24);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 24, 10), WEAPON_SIMPLE);
		}
		case 13://FUSIL À POMPE
		{
			if(HasPlayerWeapon(playerid, 25) || HasPlayerWeapon(playerid, 26) || HasPlayerWeapon(playerid, 27))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 28);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 12 Gauge ammo for this gun!", "Vous avez besoin de munitions 12 Gauge !", "¡Necessita tener 12 Gauge munición!", "Portugais", "Italien", "Sie müssen 12 Gauge Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 25);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 25, 10), WEAPON_SIMPLE);
		}
		case 14://FUSILS À CANONS SCIÉS
		{
			if(HasPlayerWeapon(playerid, 25) || HasPlayerWeapon(playerid, 27))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			//---
			new dWeaponSlot = HasPlayerWeapon(playerid, 26);
			if(dWeaponSlot != 0)
			{
			    if(GetPlayerWeaponSkill(playerid, dWeaponSlot) == WEAPON_AKIMBO)
			    {
			    	SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    	return 1;
			    }
				ApplyReloadAnim(playerid, 25);
	    		SetPlayerWeaponSkill(playerid, dWeaponSlot, WEAPON_AKIMBO);
				GivePlayerSlotObject(playerid, -1, slot);
	    		return 1;
			}
			//---
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 28);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 12 Gauge ammo for this gun!", "Vous avez besoin de munitions 12 Gauge !", "¡Necessita tener 12 Gauge munición!", "Portugais", "Italien", "Sie müssen 12 Gauge Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 26);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 26, 10), WEAPON_SIMPLE);
		}
		case 15://SPAS 12
		{
			if(HasPlayerWeapon(playerid, 25) || HasPlayerWeapon(playerid, 26) || HasPlayerWeapon(playerid, 27))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 28);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 12 Gauge ammo for this gun!", "Vous avez besoin de munitions 12 Gauge !", "¡Necessita tener 12 Gauge munición!", "Portugais", "Italien", "Sie müssen 12 Gauge Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 27);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 27, 10), WEAPON_SIMPLE);
		}
		case 16://UZI
		{
			if(HasPlayerWeapon(playerid, 29) || HasPlayerWeapon(playerid, 32))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			//---
			new dWeaponSlot = HasPlayerWeapon(playerid, 28);
			if(dWeaponSlot != 0)
			{
			    if(GetPlayerWeaponSkill(playerid, dWeaponSlot) == WEAPON_AKIMBO)
			    {
			    	SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    	return 1;
			    }
				ApplyReloadAnim(playerid, 28);
	    		SetPlayerWeaponSkill(playerid, dWeaponSlot, WEAPON_AKIMBO);
				GivePlayerSlotObject(playerid, -1, slot);
	    		return 1;
			}
			//---
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 26);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 9mm ammo for this gun!", "Vous avez besoin de munitions 9mm !", "¡Necessita tener 9mm munición!", "Portugais", "Italien", "Sie müssen 9mm Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 28);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 28, 25), WEAPON_SIMPLE);
		}
		case 17://MP-5
		{
			if(HasPlayerWeapon(playerid, 28) || HasPlayerWeapon(playerid, 29) || HasPlayerWeapon(playerid, 32))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 26);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 9mm ammo for this gun!", "Vous avez besoin de munitions 9mm !", "¡Necessita tener 9mm munición!", "Portugais", "Italien", "Sie müssen 9mm Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 29);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 29, 25), WEAPON_SIMPLE);
		}
		case 18://AK-47
		{
			if(HasPlayerWeapon(playerid, 30) || HasPlayerWeapon(playerid, 31))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 25);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 7.62 ammo for this gun!", "Vous avez besoin de munitions 7.62 !", "¡Necessita tener 7.62 munición!", "Portugais", "Italien", "Sie müssen 7.62 Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 30);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 30, 30), WEAPON_SIMPLE);
		}
		case 19://M4
		{
			if(HasPlayerWeapon(playerid, 30) || HasPlayerWeapon(playerid, 31))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 25);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 7.62 ammo for this gun!", "Vous avez besoin de munitions 7.62 !", "¡Necessita tener 7.62 munición!", "Portugais", "Italien", "Sie müssen 7.62 Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 31);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 31, 30), WEAPON_SIMPLE);
		}
		case 20://TEC-9
		{
			if(HasPlayerWeapon(playerid, 28) || HasPlayerWeapon(playerid, 29))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			//---
			new dWeaponSlot = HasPlayerWeapon(playerid, 32);
			if(dWeaponSlot != 0)
			{
			    if(GetPlayerWeaponSkill(playerid, dWeaponSlot) == WEAPON_AKIMBO)
			    {
			    	SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    	return 1;
			    }
	    		ApplyAnimation(playerid,"TEC","TEC_reload", 4.0, 0, 0, 0, 0, 0);
				GivePlayerSlotObject(playerid, -1, slot);
	    		SetPlayerWeaponSkill(playerid, dWeaponSlot, WEAPON_AKIMBO);
	    		return 1;
			}
			//---
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 26);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 9mm ammo for this gun!", "Vous avez besoin de munitions 9mm !", "¡Necessita tener 9mm munición!", "Portugais", "Italien", "Sie müssen 9mm Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 32);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 32, 25), WEAPON_SIMPLE);
		}
		case 21://FUSIL DE CHASSE
		{
			if(HasPlayerWeapon(playerid, 33) || HasPlayerWeapon(playerid, 34))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 29);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need .222 ammo for this gun!", "Vous avez besoin de munitions .222 !", "¡Necessita tener .222 munición!", "Portugais", "Italien", "Sie müssen .222 Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 33);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 33, 10), WEAPON_SIMPLE);
		}
		case 22://SNIPER
		{
			if(HasPlayerWeapon(playerid, 33) || HasPlayerWeapon(playerid, 34))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 29);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need .222 ammo for this gun!", "Vous avez besoin de munitions .222 !", "¡Necessita tener .222 munición!", "Portugais", "Italien", "Sie müssen .222 Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 34);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 34, 10), WEAPON_SIMPLE);
		}
		case 23://LANCE-ROQUETTES
		{
			if(HasPlayerWeapon(playerid, 38))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerWeapon(playerid, 35, 1);
		}
		case 24://MINIGUN
		{
			if(HasPlayerWeapon(playerid, 38))
			{
			    SendClientMessageEx(playerid, ROUGE, "You already have this type of weapon!", "Vous avez déjà une arme de ce type !", "¡Ya tiene una arma de esto tipo!", "Portugais", "Già hai una arma di questo tipo !", "Sie haben schon so eine Waffe!");
			    return 1;
			}
			if(GetPlayerNextFreeWeaponSlot(playerid) == 0)
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot carry more weapons!", "Vous ne pouvez pas porter plus d'armes !", "¡No puede llevar más armas!", "Portugais", "Italien", "Sie können nicht mehr Waffen tragen!");
			    return 1;
			}
			new dAmmoSlot = HasPlayerItem(playerid, 25);
			if(dAmmoSlot == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You need 7.62 ammo for this gun!", "Vous avez besoin de munitions 7.62 !", "¡Necessita tener 7.62 munición!", "Portugais", "Italien", "Sie müssen 7.62 Munition haben!");
			    return 1;
			}
			ApplyReloadAnim(playerid, 38);
			GivePlayerSlotObject(playerid, -1, slot);
			GivePlayerSlotObject(playerid, -1, dAmmoSlot);
			SetPlayerWeaponSkill(playerid, GivePlayerWeaponEx(playerid, 38, 30), WEAPON_SIMPLE);
		}
		case 25://7.62
		{
		    switch(GetPlayerWeapon(playerid))
		    {
		        case 30: GivePlayerWeaponEx(playerid, 30, 30), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 30);
		        case 31: GivePlayerWeaponEx(playerid, 31, 30), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 31);
		        case 38: GivePlayerWeaponEx(playerid, 38, 30), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 38);
		        default: SendClientMessageEx(playerid, ROUGE, "This ammo cannot be loaded on this type of weapon!", "Vous ne pouvez pas charger ces munitions dans cette arme !", "¡No puede recargar esta arma con esta munición!", "Portugais", "Italien", "Sie können nicht dieser Waffe mit dieser Munition laden!");
		    }
		}
		case 26://9MM
		{
		    switch(GetPlayerWeapon(playerid))
		    {
		        case 22: GivePlayerWeaponEx(playerid, 22, 25), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 22);
		        case 23: GivePlayerWeaponEx(playerid, 23, 25), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 23);
		        case 28: GivePlayerWeaponEx(playerid, 28, 25), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 28);
		        case 29: GivePlayerWeaponEx(playerid, 29, 25), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 29);
		        case 32: GivePlayerWeaponEx(playerid, 32, 25), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 32);
		        default: SendClientMessageEx(playerid, ROUGE, "This ammo cannot be loaded on this type of weapon!", "Vous ne pouvez pas charger ces munitions dans cette arme !", "¡No puede recargar esta arma con esta munición!", "Portugais", "Italien", "Sie können nicht dieser Waffe mit dieser Munition laden!");
		    }
		}
		case 27://.50AE
		{
		    switch(GetPlayerWeapon(playerid))
		    {
		        case 24: GivePlayerWeaponEx(playerid, 24, 10), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 24);
		        default: SendClientMessageEx(playerid, ROUGE, "This ammo cannot be loaded on this type of weapon!", "Vous ne pouvez pas charger ces munitions dans cette arme !", "¡No puede recargar esta arma con esta munición!", "Portugais", "Italien", "Sie können nicht dieser Waffe mit dieser Munition laden!");
		    }
		}
		case 28://12 GAUGE
		{
		    switch(GetPlayerWeapon(playerid))
		    {
		        case 25: GivePlayerWeaponEx(playerid, 25, 10), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 25);
		        case 26: GivePlayerWeaponEx(playerid, 26, 10), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 26);
		        case 27: GivePlayerWeaponEx(playerid, 27, 10), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 27);
		        default: SendClientMessageEx(playerid, ROUGE, "This ammo cannot be loaded on this type of weapon!", "Vous ne pouvez pas charger ces munitions dans cette arme !", "¡No puede recargar esta arma con esta munición!", "Portugais", "Italien", "Sie können nicht dieser Waffe mit dieser Munition laden!");
		    }
		}
		case 29://.222
		{
		    switch(GetPlayerWeapon(playerid))
		    {
		        case 33: GivePlayerWeaponEx(playerid, 33, 10), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 33);
		        case 34: GivePlayerWeaponEx(playerid, 34, 10), GivePlayerSlotObject(playerid, -1, slot), ApplyReloadAnim(playerid, 34);
		        default: SendClientMessageEx(playerid, ROUGE, "This ammo cannot be loaded on this type of weapon!", "Vous ne pouvez pas charger ces munitions dans cette arme !", "¡No puede recargar esta arma con esta munición!", "Portugais", "Italien", "Sie können nicht dieser Waffe mit dieser Munition laden!");
		    }
		}
		case 30://BIDON PLEIN D'ESSENCE
		{
			new dShredderID = IsPlayerNearShredder(playerid);
			if(dShredderID != -1 && GetShredderState(dShredderID) != 1)
		    {
				SetShredderState(dShredderID, 1);
				GivePlayerSlotObject(playerid, 31, slot);
				ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
				return 1;
			}
			//---
			new dTankID = IsPlayerNearTank(playerid);
			if(dTankID != -1 && GetTankFuel(dTankID) != 1)
			{
			    if(GetTankFuel(dTankID) + 1500 > MAX_TANK_FUEL)
			    {
			        SendClientMessageEx(playerid, ROUGE, "This tank is full!", "Cette citerne est pleine !", "Espagnol", "Portugais", "Italien", "Allemand");
			        return 1;
			    }
			    GiveTankFuel(dTankID, 2000);
				GivePlayerSlotObject(playerid, 31, slot);
				ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
				return 1;
			}
		    //---
			if(IsPlayerInAnyVehicle(playerid))
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot refill the tank while in the vehicle!", "Vous ne pouvez pas faire le plein depuis un véhicule !", "¡No puede repostar gasolina si esta en un vehículo!", "Portugais", "Italien", "Allemand");
			    return 1;
			}
			new Float:x, Float:y, Float:z, vehicleid = INVALID_VEHICLE_ID, vehicle = -1;
			GetPlayerPos(playerid, x, y, z);
			GetVehicleWithinDistance(x, y, z, 7.0, vehicleid);
			vehicle = GetVehicleID(vehicleid);
			//---
			if(IsVehicleBicycle(vehicleid)) return 1;
			if(vehicle == -1)
			{
				SendClientMessageEx(playerid, ROUGE, "This vehicle cannot be refilled!", "Vous ne pouvez pas remplir ce véhicule !", "¡No puede repostar gasolina en este vehículo!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			if(vehicleid == INVALID_VEHICLE_ID)
			{
			    SendClientMessageEx(playerid, ROUGE, "You are not near a vehicle!", "Vous n'êtes pas proche d'un véhicule !", "¡No esta cerca de un vehículo!", "Portugais", "Italien", "Sie sint in der Nähe eines Fahrzeug");
				return 1;
			}
			//---
			if(GetVehicleFuel(vehicle)  > GetVehicleMaxFuel(vehicle) - 1500)
			{
			    SendClientMessageEx(playerid, ROUGE, "This vehicle doesn't need gas!", "Ce véhicule n'a pas besoin d'essence !", "¡Esto vehículo no necessita gasolina!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			GiveVehicleFuel(vehicle, 2000);
			GivePlayerSlotObject(playerid, 31, slot);
		}
		case 31://BIDON VIDE D'ESSENCE
		{
			new dTankID = IsPlayerNearTank(playerid);
			if(dTankID != -1 && GetTankFuel(dTankID) != 1)
			{
			    if(GetTankFuel(dTankID) < 2000)
			    {
			        SendClientMessageEx(playerid, ROUGE, "There is not enough gas left!", "Cette citerne ne contient pas assez d'essence !", "Espagnol", "Portugais", "Italien", "Allemand");
			        return 1;
			    }
			    GiveTankFuel(dTankID, -2000);
				GivePlayerSlotObject(playerid, 30, slot);
				ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
				return 1;
			}
			//---
		    new dStation = IsPlayerNearGasStation(playerid);
			if(dStation == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You are not near a gas station!", "Vous n'êtes pas proche d'une station service !", "¡No esta cerca de una estacíon de gasolíno!", "Portugais", "Italien", "Sie sint in der Nähe eines Tankstelle!");
				return 1;
			}
			if(GetStationFuel(dStation) < 2000)
			{
			    SendClientMessageEx(playerid, ROUGE, "This gas station does not have enough gas left!", "Cette station service n'a pas assez d'essence !", "¡Esta estacíon de gasolíno no tiene suficiente de gasolíno!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			GiveStationFuel(dStation, -2000);
			GivePlayerSlotObject(playerid, 30, slot);
		}
		case 32://CZECH VEST
		{
		    if(GetPlayerBag(playerid) >= CZECH_VEST)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You already have a better backpack!", "Vous avez déjà un meilleur sac à dos !", "¡Ya tiene un mejor mochila!", "Portugais", "Italien", "Sie haben schon eine besseren Rucksack!");
				return 1;
			}
			GivePlayerSlotObject(playerid, -1, slot);
			SetPlayerBag(playerid, CZECH_VEST);
			AttachPlayerBackPack(playerid);
		}
		case 33://ASSAULT PACK
		{
		    if(GetPlayerBag(playerid) >= ASSAULT_PACK)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You already have a better backpack!", "Vous avez déjà un meilleur sac à dos !", "¡Ya tiene un mejor mochila!", "Portugais", "Italien", "Sie haben schon eine besseren Rucksack!");
				return 1;
			}
			switch(GetPlayerBag(playerid))
			{
				case CZECH_VEST: PlayerDropObject(playerid, 32, floatdiv(RandomEx(5, 20), 10));
			}
			GivePlayerSlotObject(playerid, -1, slot);
			SetPlayerBag(playerid, ASSAULT_PACK);
			AttachPlayerBackPack(playerid);
		}
		case 34://ALICE PACK
		{
		    if(GetPlayerBag(playerid) >= ALICE_PACK)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You already have a better backpack!", "Vous avez déjà un meilleur sac à dos !", "¡Ya tiene un mejor mochila!", "Portugais", "Italien", "Sie haben schon eine besseren Rucksack!");
				return 1;
			}
			switch(GetPlayerBag(playerid))
			{
				case CZECH_VEST: PlayerDropObject(playerid, 32, floatdiv(RandomEx(5, 20), 10));
				case ASSAULT_PACK: PlayerDropObject(playerid, 33, floatdiv(RandomEx(5, 20), 10));
			}
			GivePlayerSlotObject(playerid, -1, slot);
			SetPlayerBag(playerid, ALICE_PACK);
			AttachPlayerBackPack(playerid);
		}
		case 35://COYOTE PACK
		{
		    if(GetPlayerBag(playerid) == COYOTE_PACK)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You already have a better backpack!", "Vous avez déjà un meilleur sac à dos !", "¡Ya tiene un mejor mochila!", "Portugais", "Italien", "Sie haben schon eine besseren Rucksack!");
				return 1;
			}
			switch(GetPlayerBag(playerid))
			{
				case CZECH_VEST: PlayerDropObject(playerid, 32, floatdiv(RandomEx(5, 20), 10));
				case ASSAULT_PACK: PlayerDropObject(playerid, 33, floatdiv(RandomEx(5, 20), 10));
				case ALICE_PACK: PlayerDropObject(playerid, 34, floatdiv(RandomEx(5, 20), 10));
			}
			GivePlayerSlotObject(playerid, -1, slot);
			SetPlayerBag(playerid, COYOTE_PACK);
			AttachPlayerBackPack(playerid);
		}
		case 36://BANDAGES
		{
		    if(!GetPlayerBleedingState(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You are not bleeding!", "Vous ne faites pas d'hémorragie !", "¡No esta sangrando!", "Portugais", "Italien", "Sie bluten nicht!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
	    	ApplyAnimation(playerid,"FAT","IDLE_tired", 4.0, 0, 0, 0, 0, 0);
			SetPlayerBleedingState(playerid, false);
		}
		case 37://ANTI-DOULEUR
		{
		    if(GetPlayerLegState(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "Your leg is not broken!", "Votre jambe n'est pas cassée !", "¡Su pierna no esta rotta!", "Portugais", "Italien", "Ihren Bein ist nicht gebrochen!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
	    	ApplyAnimation(playerid,"FAT","IDLE_tired", 4.0, 0, 0, 0, 0, 0);
			SetPlayerLegState(playerid, true);
		}
		case 38://POCHE DE SANG
		{
			GivePlayerSlotObject(playerid, -1, slot);
			SetHealthForPlayer(playerid, 1250, REASON_HEAL);
		}
		case 39://CAFÉINE
		{
		    if(GetPlayerSleep(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not sleepy!", "Vous n'êtes pas fatigué !", "¡No esta cansado!", "Portugais", "Non sei stanco!", "Sie sint nicht müde!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerSleep(playerid, 50);
		}
		case 40://BOUTEILLE VIDE
		{
			new dTankID = IsPlayerNearTank(playerid);
			if(dTankID != -1 && GetTankFuel(dTankID) != 1)
			{
			    if(GetTankFuel(dTankID) < 500)
			    {
			        SendClientMessageEx(playerid, ROUGE, "There is not enough gas left!", "Cette citerne ne contient pas assez d'essence !", "Espagnol", "Portugais", "Italien", "Allemand");
			        return 1;
			    }
			    GiveTankFuel(dTankID, -500);
				GivePlayerSlotObject(playerid, 98, slot);
				ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
				return 1;
			}
			//---
		    new dCollectorID = IsPlayerNearCollector(playerid);
		    new dStation = IsPlayerNearGasStation(playerid);
			if(dCollectorID != -1)
		    {
		        if(GetCollectorWater(dCollectorID) >= 3)
		        {
			        GiveCollectorWater(dCollectorID, -3);
					GivePlayerSlotObject(playerid, 81, slot);
					return 1;
				}
		    }
			else if(dStation != -1)
			{
				if(GetStationFuel(dStation) < 500)
				{
				    SendClientMessageEx(playerid, ROUGE, "This gas station does not have enough gas left!", "Cette station service n'a pas assez d'essence !", "¡Esta estacíon de gasolíno no tiene suficiente de gasolíno!", "Portugais", "Italien", "Allemand");
					return 1;
				}
				ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
				GiveStationFuel(dStation, -500);
				GivePlayerSlotObject(playerid, 98, slot);
				return 1;
			}
			else if(IsPlayerInWater(playerid))
			{
				GivePlayerSlotObject(playerid, 126, slot);
				return 1;
			}
		}
		case 41://CHAPEAU LÉOPARD
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 41, false, 0.1262, 0.0324, -0.0042, 0.0, 89.4055, 91.9544, 1.1, 1.1, 1.1);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 42://CHAPEAU LÉOPARD 2
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 42, false, 0.1262, 0.0324, -0.0042, 0.0, 89.4055, 91.9544, 1.1, 1.1, 1.1);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 43://BOB
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 43, false, 0.1262, 0.0324, -0.0042, 0.0, 89.4055, 91.9544, 1.1, 1.1, 1.1);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 44://CHAPEAU DISCO
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 44, false, 0.1262, 0.0324, -0.0042, 0.0, 89.4055, 91.9544, 1.1, 1.1, 1.1);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 45://CHEPEAU DE SORCIERE
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 45, false, 0.1441, 0.0, 0.0, 0.0, 0.0, 0.0, 1.1, 1.1, 1.1);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 46://BÉRÊT
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 46, false, 0.1650, 0.0097, -0.0060, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 47://CASQUE MILITAIRE
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 47, false, 0.1460, 0.0036, 0.0053, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 48://CASQUE DE MARIN
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 48, false, 0.1460, 0.0036, 0.0053, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 49://CASQUETTE BLEUE
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 49, false, 0.1460, 0.0036, 0.0053, 0.0, 0.0, 0.0, 1.1, 1.1, 1.1);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 50://CASQUETTE ROUGE
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 50, false, 0.1460, 0.0036, 0.0053, 0.0, 0.0, 0.0, 1.1, 1.1, 1.1);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 51://CHAPEAU MELON VERT
		{
			GivePlayerClothe(playerid, HAT_OBJECT_ID, 51, false, 0.1460, 0.0036, 0.0053, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 52://PREDATORS ROUGES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 52, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 53://PREDATORS JAUNES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 53, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 54://PREDATORS VERTES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 54, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 55://PREDATORS BLEUES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 55, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 56://AVIATORS NOIRES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 56, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 57://AVIATORS ROUGES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 57, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 58://AVIATORS MAUVES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 58, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 59://AVIATORS ROSES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 59, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 60://AVIATORS NOIRES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 60, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 61://AVIATORS ORANGES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 61, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 62://WAYFARERS NOIRES
		{
			GivePlayerClothe(playerid, GLASSES_OBJECT_ID, 62, false, 0.1004, 0.0491, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 63://ROUE
		{
			if(IsPlayerInAnyVehicle(playerid))
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot change a wheel while in the vehicle!", "Vous ne pouvez pas changer de roue depuis un véhicule !", "¡No puede cambiar una rueda si esta en un vehículo!", "Portugais", "Italien", "Allemand");
			    return 1;
			}
			new Float:x, Float:y, Float:z, vehicleid = INVALID_VEHICLE_ID, vehicle = -1;
			GetPlayerPos(playerid, x, y, z);
			GetVehicleWithinDistance(x, y, z, 7.0, vehicleid);
			vehicle = GetVehicleID(vehicleid);
			if(vehicleid == INVALID_VEHICLE_ID)
			{
			    SendClientMessageEx(playerid, ROUGE, "You are not near a vehicle!", "Vous n'êtes pas proche d'un véhicule !", "¡No esta cerca de un vehículo!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			if(vehicle == -1)
			{
				SendClientMessageEx(playerid, ROUGE, "This vehicle's wheels cannot be changed!", "Vous ne pouvez pas changer les roues de ce véhicule !", "¡No puede repostar una rueda en este vehículo!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			new dWheelID = -1;
			new wheels = GetVehicleWheelsAmount(vehicle);
			if(wheels != 0)
			{
			    for(new i = 0; i < wheels; i ++)
			    {
			        if(!GetVehicleWheelState(vehicle, i))
			        {
			            dWheelID = i;
			        }
			    }
			}
			if(dWheelID == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "This vehicle doesn't need a new wheel!", "Ce véhicule n'a pas besoin d'une nouvelle roue !", "¡Esto vehículo no necessita una nueva rueda!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			SetVehicleWheelState(vehicle, dWheelID, true);
			SetVehicleWheels(vehicle);
			PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 64://MOTEUR
		{
			if(IsPlayerInAnyVehicle(playerid))
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot change an engine while in the vehicle!", "Vous ne pouvez pas changer de moteur depuis un véhicule !", "¡No puede cambiar el motor si esta en un vehículo!", "Portugais", "Italien", "Allemand");
			    return 1;
			}
			new Float:x, Float:y, Float:z, vehicleid = INVALID_VEHICLE_ID, vehicle = -1;
			GetPlayerPos(playerid, x, y, z);
			GetVehicleWithinDistance(x, y, z, 7.0, vehicleid);
			vehicle = GetVehicleID(vehicleid);
			if(vehicleid == INVALID_VEHICLE_ID)
			{
			    SendClientMessageEx(playerid, ROUGE, "You are not near a vehicle!", "Vous n'êtes pas proche d'un véhicule !", "¡No esta cerca de un vehículo!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			if(vehicle == -1)
			{
				SendClientMessageEx(playerid, ROUGE, "This vehicle's engine cannot be changed!", "Vous ne pouvez pas changer le moteur de ce véhicule !", "¡No puede repostar el motor de este vehículo!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			if(GetVehicleEngine(vehicle))
			{
			    SendClientMessageEx(playerid, ROUGE, "This vehicle doesn't need a new engine!", "Ce véhicule n'a pas besoin d'un nouveau moteur !", "¡Esto vehículo no necessita un nuevo motor!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			SetVehicleEngine(vehicle, true);
			PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 65://MASQUE À GAZ
		{
		    GivePlayerClothe(playerid, MASK_OBJECT_ID, 65, false, -0.0173, 0.094, 0.0, 330.0, 180.0, 15.0, 1.0, 1.0, 1.0);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 66://BURGER
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 15);
		    GivePlayerSleep(playerid, 3);
		}
		case 67://PIZZA
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 15);
		    GivePlayerSleep(playerid, 3);
		}
		case 68://STEAK CRU
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 3);
		}
		case 69://STEAK CUIT
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 10);
		    GivePlayerSleep(playerid, 3);
		}
		case 70://LAIT
		{
		    if(GetPlayerThirst(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not thirsty!", "Vous n'avez pas soif !", "¡No tiene sed!", "Portugais", "Non hai sete!", "Sie sint nicht durstig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, 40, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerThirst(playerid, 25);
		    PlayerPlaySound(playerid, 32200, 0.0, 0.0, 0.0);
		}
		case 71://BOIS
		{
		}
		case 72://ORANGE
		{
			switch(GetPlayerLanguage(playerid))
	        {
	            case LANGUAGE_EN: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Orange", "Eat\nTake the seeds", "Do", "Cancel");
	            case LANGUAGE_FR: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Orange", "Manger\nPrendre graines", "Faire", "Annuler");
	            case LANGUAGE_ES: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Espagnol", "Espagnol\nEspagnol", "Hacer", "Cancelar");
	            case LANGUAGE_PG: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Portugais", "Portugais", "Portugais", "Portugais");
				case LANGUAGE_IT: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Italien", "Italien", "Italien", "Italien");
				case LANGUAGE_DE: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Apfelsine", "Essen\nKerne nehmen" , "Machen", "Schlissen");
	        }
		}
		case 73://POMME
		{
			switch(GetPlayerLanguage(playerid))
	        {
	            case LANGUAGE_EN: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Apple", "Eat\nTake the seeds", "Do", "Cancel");
	            case LANGUAGE_FR: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Pomme", "Manger\nPrendre graines", "Faire", "Annuler");
	            case LANGUAGE_ES: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Espagnol", "Espagnol\nEspagnol", "Hacer", "Cancelar");
	            case LANGUAGE_PG: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Portugais", "Portugais", "Portugais", "Portugais");
				case LANGUAGE_IT: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Italien", "Italien", "Italien", "Italien");
				case LANGUAGE_DE: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Apfel", "Essen\nKerne nehmen" , "Machen", "Schlissen");
	        }
		}
		case 74://TOMATE
		{
			switch(GetPlayerLanguage(playerid))
	        {
	            case LANGUAGE_EN: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Apple", "Eat\nTake the seeds", "Do", "Cancel");
	            case LANGUAGE_FR: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Pomme", "Manger\nPrendre graines", "Faire", "Annuler");
	            case LANGUAGE_ES: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Tomato", "Espagnol\nEspagnol", "Hacer", "Cancelar");
	            case LANGUAGE_PG: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Portugais", "Portugais", "Portugais", "Portugais");
				case LANGUAGE_IT: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Italien", "Italien", "Italien", "Italien");
				case LANGUAGE_DE: ShowPlayerDialog(playerid, 65, DIALOG_STYLE_LIST, "Tomate", "Essen\nKerne nehmen" , "Machen", "Schlissen");
	        }
		}
		case 75://BANDANA CARREAUX
		{
		    GivePlayerClothe(playerid, MASK_OBJECT_ID, 75, false, 0.0855, 0.0368, 0.0, 90.0, 180.0, 90.0, 1.0, 1.0, 1.0);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 76://BANDANA CRÂNES
		{
		    GivePlayerClothe(playerid, MASK_OBJECT_ID, 76, false, 0.0855, 0.0368, 0.0, 90.0, 180.0, 90.0, 1.0, 1.0, 1.0);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 77://BANDANA ARMÉE
		{
		    GivePlayerClothe(playerid, MASK_OBJECT_ID, 77, false, 0.0855, 0.0368, 0.0, 90.0, 180.0, 90.0, 1.0, 1.0, 1.0);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 78://BANDANA VERT
		{
		    GivePlayerClothe(playerid, MASK_OBJECT_ID, 78, false, 0.0855, 0.0368, 0.0, 90.0, 180.0, 90.0, 1.0, 1.0, 1.0);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 79://BANDANA NOIR
		{
		    GivePlayerClothe(playerid, MASK_OBJECT_ID, 79, false, 0.0855, 0.0368, 0.0, 90.0, 180.0, 90.0, 1.0, 1.0, 1.0);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 80://Lit miteux
		{
		    if(!CanPlayerCreateBed(playerid))
		    {
				SendClientMessageEx(playerid, ROUGE, "You must be in a house or near a tent!", "Vous devez être dans une maison ou près d'une tente !", "¡Necessita estar en una casa o cerca de una tienda!", "Portugais", "Italien", "Allemand");
		        return 1;
			}
		    new Float:x, Float:y, Float:z, Float:a, Float:fTrash;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, a);
			//---
			if(CA_RayCastLine(x, y, z, x + 1.1 * floatsin(-a, degrees), y + 1.1 * floatcos(-a, degrees), z, fTrash, fTrash, fTrash) != 0)
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is not enough room here!", "Il n'y a pas assez de place ici !", "Espagnol", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			GetXYInFrontOfPoint(x, y, a, 1.0);
			SetPlayerBed(playerid, CreateBed(1, x, y, z, a));
			EditDynamicObject(playerid, GetBedObject(GetPlayerBed(playerid)));
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 81://BOUTEILLE D'EAU
		{
			if(PlayerWatersPlants(playerid))
			{
				return 1;
			}
		    if(GetPlayerThirst(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not thirsty!", "Vous n'avez pas soif !", "¡No tiene sed!", "Portugais", "Non hai sete!", "Sie sint nicht durstig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, 40, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerThirst(playerid, 20);
		    PlayerPlaySound(playerid, 32200, 0.0, 0.0, 0.0);
		}
		case 82://BOOMBOX
		{
		}
		case 83://GILET PARE-BALLE SWAT
		{
		    GivePlayerClothe(playerid, TORSO_OBJECT_ID, 83, false, 0.0675, 0.0458, 0.0, 0.0, 0.0, 0.0, 1.2, 1.2, 1.2);
		    SetArmourForPlayer(playerid, 1250);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 84://GILET PARE-BALLE SWAT UTILISÉ
		{
		    GivePlayerClothe(playerid, TORSO_OBJECT_ID, 84, false, 0.0675, 0.0458, 0.0, 0.0, 0.0, 0.0, 1.2, 1.2, 1.2);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 85://CAMOUFLAGE
		{
		    GivePlayerClothe(playerid, TORSO_OBJECT_ID, 85, false, -0.2753, -0.0488, -0.3086, 90.0, 0.0, 0.0, 0.4, 0.4, 0.4);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 86://POISSON CRU
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 2);
		}
		case 87://POISSON CUIT
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 12);
		    GivePlayerSleep(playerid, 3);
		}
		case 88://BRÊME
		{
			/*if(IsPlayerNextToFire(playerid))
			{
				GivePlayerSlotObject(playerid, 89, slot);
				ApplyAnimation(playerid, "GRENADE", "WEAPON_throwu", 4.0, 0, 0, 0, 0, 0, 1);
				return 1;
			}*/
			//---
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 2);
		}
		case 89://BRÊME CUITE
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 20);
		    GivePlayerSleep(playerid, 5);
		}
		case 90://CANNE À PÊCHE
		{
			PlayerStartsFishing(playerid);
			return 1;
		}
		case 91://BRIQUET
		{
		    if(IsPlayerInWater(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You cannot use this item while in water!", "Vous ne pouvez utiliser de briquet dans l'eau", "¡No puede usted utilizar esto objetto en agua!", "Portugais", "Italien", "Sie können nicht deiser Objekte in Wasser nutzen!");
				return 1;
		    }
		    //---BRASERO
			new dBrazier = IsPlayerInRangeOfBrasero(playerid, true);
			if(dBrazier != -1)
			{
			    PlayerPlaySound(playerid, 14200, 0.0, 0.0, 0.0);
			    LightBrasero(dBrazier, true);
				ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			    return 1;
			}
		    //---
			new dLogSlot = HasPlayerItem(playerid, 71);
			if(dLogSlot == -1)
			{
		        SendClientMessageEx(playerid, ROUGE, "You need a log to make a fire!", "Vous avez besoin de bois pour faire du feu !", "¡Necessita madera para hacer fuego!", "Portugais", "Italien", "Sie haben kein holtz zum feuer machen!");
				return 1;
			}
		    new Float:x, Float:y, Float:z, Float:angle;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
		    CreateFire(x, y, z, angle, 10);
			PlayerPlaySound(playerid, 14200, 0.0, 0.0, 0.0);
			ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			GivePlayerSlotObject(playerid, -1, dLogSlot);
		}
		case 92://GRAINES D'ORANGE
		{
		    if(!CanPlayerPlantHere(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You can't plant anything here!", "Vous ne pouvez pas planter ici !", "¡No puede plantar aquí!", "Portugais", "Italien", "Sie können nicht hier plantzen!");
				return 1;
		    }
		    new Float:x, Float:y, Float:z, Float:angle;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
		    CreatePlant(2, 5, 20 - GetPlayerSkill(playerid, FARMER_ID) * 3, x, y, z, angle);
			ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 93://GRAINES DE POMME
		{
		    if(!CanPlayerPlantHere(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You can't plant anything here!", "Vous ne pouvez pas planter ici !", "¡No puede plantar aquí!", "Portugais", "Italien", "Sie können nicht hier plantzen!");
				return 1;
		    }
		    new Float:x, Float:y, Float:z, Float:angle;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
		    CreatePlant(3, 5, 30 - GetPlayerSkill(playerid, FARMER_ID) * 3, x, y, z, angle);
			ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 94://GRAINES DE TOMATES
		{
		    if(!CanPlayerPlantHere(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You can't plant anything here!", "Vous ne pouvez pas planter ici !", "¡No puede plantar aquí!", "Portugais", "Italien", "Sie können nicht hier plantzen!");
				return 1;
		    }
		    new Float:x, Float:y, Float:z, Float:angle;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
		    CreatePlant(4, 5, 10 - GetPlayerSkill(playerid, FARMER_ID) * 3, x, y, z, angle);
			ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 95://RÉCUPÉRATEUR D'EAU
		{
		    new Float:x, Float:y, Float:z, Float:angle, Float:fTrash;
		    GetPlayerPos(playerid, x, y, z);
			if(IsPositionUnderSomething(x, y, z))
			{
			    SendClientMessageEx(playerid, ROUGE, "Put this outdoor or it won't catch water!", "Mettez cela à l'extérieur, sinon il ne pourra pas collecter d'eau !", "¡Ponga eso fuera, de otra manera, no colectará agua!", "Portugais", "Italien", "Allemand");
			    return 1;
			}
		    //---
		    GetPlayerFacingAngle(playerid, angle);
			if(CA_RayCastLine(x, y, z, x + 0.66 * floatsin(-angle, degrees), y + 0.66 * floatcos(-angle, degrees), z, fTrash, fTrash, fTrash) != 0)
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is not enough room here!", "Il n'y a pas assez de place ici !", "Espagnol", "Portugais", "Italien", "Allemand");
				return 1;
			}
			GetXYInFrontOfPoint(x, y, angle, 0.56);
			CreateCollector(x, y, z, angle, 0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 96://CLÉ ANGLAISE
		{
			PlayerStartsRepairing(playerid);
		}
		case 97://COFFRE FORT
		{
		    new Float:x, Float:y, Float:z, Float:angle, Float:fTrash;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
			//---
			if(CA_RayCastLine(x, y, z, x + 0.6 * floatsin(-angle, degrees), y + 0.6 * floatcos(-angle, degrees), z, fTrash, fTrash, fTrash) != 0)
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is not enough room here!", "Il n'y a pas assez de place ici !", "Espagnol", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			GetXYInFrontOfPoint(x, y, angle, 0.4);
			SetPlayerSafe(playerid, CreateSafe(x, y, z, angle));
			GivePlayerSlotObject(playerid, -1, slot);
			//---
			EditDynamicObject(playerid, dSafeInfos[GetPlayerSafe(playerid)][oSafe][0]);
		}
		case 98://BOUTEILLE D'ESSENCE
		{
			new dTankID = IsPlayerNearTank(playerid);
			if(dTankID != -1 && GetTankFuel(dTankID) != 1)
			{
			    if(GetTankFuel(dTankID) + 400 > MAX_TANK_FUEL)
			    {
			        SendClientMessageEx(playerid, ROUGE, "This tank is full!", "Cette citerne est pleine !", "Espagnol", "Portugais", "Italien", "Allemand");
			        return 1;
			    }
			    GiveTankFuel(dTankID, 500);
				GivePlayerSlotObject(playerid, 40, slot);
				ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
				return 1;
			}
		    //---
			if(IsPlayerInAnyVehicle(playerid))
			{
			    SendClientMessageEx(playerid, ROUGE, "You cannot refill the tank while in the vehicle!", "Vous ne pouvez pas faire le plein depuis un véhicule !", "¡No puede repostar gasolina si esta en un vehículo!", "Portugais", "Italien", "Allemand");
			    return 1;
			}
			new Float:x, Float:y, Float:z, vehicleid = INVALID_VEHICLE_ID, vehicle = -1;
			GetPlayerPos(playerid, x, y, z);
			GetVehicleWithinDistance(x, y, z, 7.0, vehicleid);
			vehicle = GetVehicleID(vehicleid);
			if(IsVehicleBicycle(vehicleid)) return 1;
			if(vehicle == -1)
			{
				SendClientMessageEx(playerid, ROUGE, "This vehicle cannot be refilled!", "Vous ne pouvez pas remplir ce véhicule !", "¡No puede repostar gasolina en este vehículo!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			if(vehicleid == INVALID_VEHICLE_ID)
			{
			    SendClientMessageEx(playerid, ROUGE, "You are not near a vehicle!", "Vous n'êtes pas proche d'un véhicule !", "¡No esta cerca de un vehículo!", "Portugais", "Italien", "Sie sint in der Nähe eines Fahrzeug");
				return 1;
			}
			//---
			if(GetVehicleFuel(vehicle)  > GetVehicleMaxFuel(vehicle) - 200)
			{
			    SendClientMessageEx(playerid, ROUGE, "This vehicle doesn't need gas!", "Ce véhicule n'a pas besoin d'essence !", "¡Esto vehículo no necessita gasolina!", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			GiveVehicleFuel(vehicle, 500);
			GivePlayerSlotObject(playerid, 40, slot);
		}
		case 99://MINE
		{
			new Float:x, Float:y, Float:z, Float:angle;
			GetPlayerPos(playerid, x, y, z);
			GetPlayerFacingAngle(playerid, angle);
		   	ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, 0, 1, 1, 0, 0, 1);
			if(GetPlayerSurfingVehicleID(playerid) != INVALID_VEHICLE_ID && !IsPlayerInAnyVehicle(playerid)) CreateBomb(1, -5, GetPlayerSurfingVehicleID(playerid), x, y, z, angle, -1, 0);
		   	else if(IsPlayerInAnyVehicle(playerid)) CreateBomb(1, -5, GetPlayerVehicleID(playerid), x, y, z, angle, -1, 0);
		   	else if(GetPlayerSurfingVehicleID(playerid) == INVALID_VEHICLE_ID) CreateBomb(1, -5, 0, x, y, z, angle, -1, 0);
			ShowPlayerTextInfo(playerid, 5000, "~r~If somebody approches the mine it will blow.", "~r~Si quelqu'un s'approche de cette mine, elle explosera.", "~r~Si alguien viene cerca de la bomba, explotará.", "~r~Portugais", "~r~Italien", "~r~Allemand");
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 100://BOMBE À TIMER
		{
		    if(CallRemoteFunction("GetPlayerMission", "i", playerid) == MISSION_SHORTCUT_BRIDGE)
		    {
			    SendClientMessageEx(playerid, ROUGE, "You can't use this item right now!", "Vous ne pouvez pas utiliser cet objet maintenant !", "¡No puedes utilizar esto objecto ahora!", "Portugais", "Italien", "Allemand");
				return 1;
		    }
			new Float:x, Float:y, Float:z, Float:angle;
			GetPlayerPos(playerid, x, y, z);
			GetPlayerFacingAngle(playerid, angle);
		   	ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, 0, 1, 1, 0, 0, 1);
			if(GetPlayerSurfingVehicleID(playerid) != INVALID_VEHICLE_ID && !IsPlayerInAnyVehicle(playerid)) CreateBomb(2, 15, GetPlayerSurfingVehicleID(playerid), x, y, z, angle, -1, 0);
		   	else if(IsPlayerInAnyVehicle(playerid)) CreateBomb(2, 15, GetPlayerVehicleID(playerid), x, y, z, angle, -1, 0);
		   	else if(GetPlayerSurfingVehicleID(playerid) == INVALID_VEHICLE_ID) CreateBomb(2, 15, 0, x, y, z, angle, -1, 0);
			ShowPlayerTextInfo(playerid, 5000, "~r~You have 15 seconds to run.", "~r~Vous avez 15 secondes pour partir.", "~r~Tiene 15 segundos para escapar.", "~r~Portugais", "~r~Italien", "~r~Sie haben 15 Sekunden zum Fluchten.");
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 101://CAGOULE
		{
		    GivePlayerClothe(playerid, MASK_OBJECT_ID, 101, false, 0.0805, -0.0216, 0.0, 180.0, 90.0, 0.0, 1.1, 1.1, 1.1);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 102://CASSEROLE
		{
		    if(!IsPlayerNextToFire(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "There is no fire nearby!", "Vous n'êtes pas proche du feu !", "¡No esta cerca del fuego!", "Portugais", "Italien", "Sie sint nicht in di Nähe von ein feuer!");
				return 1;
		    }
			ShowPlayerKitchen(playerid);
		}
		case 103://MÉTAL
		{
			if(GetPlayerClothes(playerid, TORSO_OBJECT_ID) == 84 || HasPlayerItem(playerid, 84) != -1)
		    {
				switch(GetPlayerLanguage(playerid))
				{
				 	case LANGUAGE_EN: ShowPlayerDialog(playerid, DIALOG_ARMOUR_REPAIR, DIALOG_STYLE_MSGBOX, "Armour", "You can fix your bulletproof armour using a piece of junk.\nDo you want to fix it?", "Fix", "Cancel");
				 	case LANGUAGE_FR: ShowPlayerDialog(playerid, DIALOG_ARMOUR_REPAIR, DIALOG_STYLE_MSGBOX, "Gilet pare-balles", "Vous pouvez réparer un gilet pare-balles avec du métal.\nVoulez-vous le réparer ?", "Réparer", "Annuler");
				 	case LANGUAGE_ES: ShowPlayerDialog(playerid, DIALOG_ARMOUR_REPAIR, DIALOG_STYLE_MSGBOX, "Espagnol", "Espagnol", "Ok", "");
				 	case LANGUAGE_PG: ShowPlayerDialog(playerid, DIALOG_ARMOUR_REPAIR, DIALOG_STYLE_MSGBOX, "Portugais", "Portugais", "Ok", "");
					case LANGUAGE_IT: ShowPlayerDialog(playerid, DIALOG_ARMOUR_REPAIR, DIALOG_STYLE_MSGBOX, "Italien", "Italien", "Ok", "");
					case LANGUAGE_DE: ShowPlayerDialog(playerid, DIALOG_ARMOUR_REPAIR, DIALOG_STYLE_MSGBOX, "Allemand", "Allemand" , "Ok", "");
				}
		    }
		}
		case 106://SERRURE
		{
		    new Float:x, Float:y, Float:z;
		    GetPlayerPos(playerid, x, y, z);
			SetPlayerActionDoor(playerid, IsPlayerNearHouseDoor(playerid));
			//---
			new door = GetPlayerActionDoor(playerid);

			if(door == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You are not near a house!", "Vous n'êtes pas près d'une maison !", "¡No está cerca de una casa!", "Portugais", "Italien", "Allemand");
			    return 1;
			}
			if(!GetHouseDoorExists(door))
			{
			    SendClientMessageEx(playerid, ROUGE, "This house has no door!", "Cette maison n'a pas porte !", "¡Esta casa no tiene una puerta!", "Portugais", "Questa casa non ha una porta !", "Allemand");
			    return 1;
			}
			if(!GetHouseDoorState(door))
			{
			    SendClientMessageEx(playerid, ROUGE, "Open the door to change the code.", "Ouvrez la porte pour changer le code.", "Espagnol", "Portugais", "Italien", "Allemand");
			    return 1;
			}
			switch(GetPlayerLanguage(playerid))
			{
			 	case LANGUAGE_EN: ShowPlayerDialog(playerid, DIALOG_DOOR_CHANGE, DIALOG_STYLE_INPUT, "Door", "Enter a four numbered code:", "Ok", "");
			 	case LANGUAGE_FR: ShowPlayerDialog(playerid, DIALOG_DOOR_CHANGE, DIALOG_STYLE_INPUT, "Porte", "Écrivez un nombre à quatre chiffres comme code:", "Ok", "");
			 	case LANGUAGE_ES: ShowPlayerDialog(playerid, DIALOG_DOOR_CHANGE, DIALOG_STYLE_INPUT, "Puerta", "Espagnol", "Ok", "");
			 	case LANGUAGE_PG: ShowPlayerDialog(playerid, DIALOG_DOOR_CHANGE, DIALOG_STYLE_INPUT, "Porta", "Portugais", "Ok", "");
				case LANGUAGE_IT: ShowPlayerDialog(playerid, DIALOG_DOOR_CHANGE, DIALOG_STYLE_INPUT, "Porta", "Italien", "Ok", "");
				case LANGUAGE_DE: ShowPlayerDialog(playerid, DIALOG_DOOR_CHANGE, DIALOG_STYLE_INPUT, "Tür", "Allemand" , "Ok", "");
			}
		}
		case 109://PIZZA
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
		    GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 50);
		    GivePlayerSleep(playerid, 5);
		}
		case 110://PAIN
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
		    GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 20);
		    GivePlayerSleep(playerid, 5);
		}
		case 112://PORTE
		{
		    new Float:x, Float:y, Float:z;
			new houseid = -1;
		    GetPlayerPos(playerid, x, y, z);
			houseid = IsHouseNearToPoint(15.0, x, y, z);
			SetPlayerHouseEdit(playerid, houseid);
			//---
			if(houseid == -1)
			{
			    SendClientMessageEx(playerid, ROUGE, "You are not near a house!", "Vous n'êtes pas près d'une maison !", "¡No está cerca de una casa!", "Portugais", "Italien", "Allemand");
			    return 1;
			}
			if(GetHouseDoorExists(houseid))
			{
			    SendClientMessageEx(playerid, ROUGE, "This house already has a door!", "Cette maison a déjà une porte !", "¡Esta casa ya tiene una puerta!", "Portugais", "Questa casa già ha una porta !", "Allemand");
			    return 1;
			}
			switch(GetPlayerLanguage(playerid))
			{
			 	case LANGUAGE_EN: ShowPlayerDialog(playerid, DIALOG_DOOR_CREATION, DIALOG_STYLE_INPUT, "Door", "Enter a four numbered code:", "Ok", "");
			 	case LANGUAGE_FR: ShowPlayerDialog(playerid, DIALOG_DOOR_CREATION, DIALOG_STYLE_INPUT, "Porte", "Écrivez un nombre à quatre chiffres comme code:", "Ok", "");
			 	case LANGUAGE_ES: ShowPlayerDialog(playerid, DIALOG_DOOR_CREATION, DIALOG_STYLE_INPUT, "Puerta", "Espagnol", "Ok", "");
			 	case LANGUAGE_PG: ShowPlayerDialog(playerid, DIALOG_DOOR_CREATION, DIALOG_STYLE_INPUT, "Porta", "Portugais", "Ok", "");
				case LANGUAGE_IT: ShowPlayerDialog(playerid, DIALOG_DOOR_CREATION, DIALOG_STYLE_INPUT, "Porta", "Italien", "Ok", "");
				case LANGUAGE_DE: ShowPlayerDialog(playerid, DIALOG_DOOR_CREATION, DIALOG_STYLE_INPUT, "Tür", "Allemand" , "Ok", "");
			}
			SetPlayerHouseBuild(playerid, 3);
		}
		case 113://PLANS DE MAISON
		{
		    if(GetPlayerHouseBuild(playerid) != 0)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're already building a house!", "Vous construisez déjà une maison !", "¡Espagnol!", "Portugais !", "Italien!", "Allemand!");
				return 1;
			}
			ShowPlayerHousePlans(playerid);
		}
		case 114://BURETTE
		{
		    if(CallRemoteFunction("GetPlayerMission", "i", playerid) == MISSION_LIGHT_UP_SEEK)
		    {
				new dTankID = IsPlayerNearTank(playerid);
				if(dTankID != -1 && GetTankFuel(dTankID) != 1)
				{
				    if(GetTankFuel(dTankID) < 1000)
				    {
				        SendClientMessageEx(playerid, ROUGE, "There is not enough gas left!", "Cette citerne ne contient pas assez d'essence !", "Espagnol", "Portugais", "Italien", "Allemand");
				        return 1;
				    }
				    GiveTankFuel(dTankID, -1000);
					ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
					CallRemoteFunction("OnCinematicGoesOn", "iiii", playerid, 1, 10, 0);
					return 1;
				}
				//---
			    new dStation = IsPlayerNearGasStation(playerid);
				if(dStation == -1)
				{
				    SendClientMessageEx(playerid, ROUGE, "You are not near a gas station!", "Vous n'êtes pas proche d'une station service !", "¡No esta cerca de una estacíon de gasolíno!", "Portugais", "Italien", "Sie sint in der Nähe eines Tankstelle!");
					return 1;
				}
				if(GetStationFuel(dStation) < 1000)
				{
				    SendClientMessageEx(playerid, ROUGE, "This gas station does not have enough gas left!", "Cette station service n'a pas assez d'essence !", "¡Esta estacíon de gasolíno no tiene suficiente de gasolíno!", "Portugais", "Italien", "Allemand");
					return 1;
				}
				GiveStationFuel(dStation, -1000);
				CallRemoteFunction("OnCinematicGoesOn", "iiii", playerid, 1, 10, 0);
		    }
		}
		case 115://GRAINES DE BLÉ
		{
		    if(!CanPlayerPlantHere(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You can't plant anything here!", "Vous ne pouvez pas planter ici !", "¡No puede plantar aquí!", "Portugais", "Italien", "Sie können nicht hier plantzen!");
				return 1;
		    }
		    new Float:x, Float:y, Float:z, Float:angle;
		    GetPlayerFacingAngle(playerid, angle);
		    for(new i = 0, j = RandomEx(1, 4); i < j; i ++)
		    {
		    	GetPlayerPos(playerid, x, y, z);
		        GetXYInFrontOfPoint(x, y, angle + float(RandomEx(-35, 35)), floatdiv(RandomEx(5, 25), 10));
		    	CreatePlant(1, 0, 5 - GetPlayerSkill(playerid, FARMER_ID), x, y, z, angle);
		    }
			ApplyAnimation(playerid, "GRENADE", "WEAPON_throwu", 4.0, 0, 0, 0, 0, 0, 1);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 116://BLÉ
		{
			new dShredderID = IsPlayerNearShredder(playerid);
			if(dShredderID != -1)
		    {
		        if(GetShredderState(dShredderID) != 1)
		        {
		            SendClientMessageEx(playerid, ROUGE, "Fill the shredder with gas or it won't work!", "Mettez de l'essence, sinon le broyeur ne marchera pas !", "Espagnol", "Portugais", "Italien", "Allemand");
		            return 1;
		        }
		        new Float:x, Float:y, Float:z, Float:angle;
				GetShredderPos(dShredderID, x, y, z);
				GetShredderAngle(dShredderID, angle);
		        //---
		        angle += 180.0 + float(RandomEx(-20, 20));
		        GetXYInFrontOfPoint(x, y, angle, floatdiv(RandomEx(55, 70), 100));
		        CreateItem(108, x, y, z, false, -1);
				GivePlayerSlotObject(playerid, -1, slot);
				PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
		    }
		}
		case 117://BROYEUR
		{
		    new Float:x, Float:y, Float:z, Float:angle, Float:fTrash;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
			if(CA_RayCastLine(x, y, z, x + 0.7 * floatsin(-angle, degrees), y + 0.7 * floatcos(-angle, degrees), z, fTrash, fTrash, fTrash) != 0)
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is not enough room here!", "Il n'y a pas assez de place ici !", "Espagnol", "Portugais", "Italien", "Allemand");
				return 1;
			}
			GetXYInFrontOfPoint(x, y, angle, 0.6);
			SetPlayerShredder(playerid, CreateShredder(x, y, z, angle, 0));
			EditDynamicObject(playerid, GetShredderObject(GetPlayerShredder(playerid)));
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 118://JUS D'ORANGES
		{
		    if(GetPlayerThirst(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not thirsty!", "Vous n'avez pas soif !", "¡No tiene sed!", "Portugais", "Non hai sete!", "Sie sint nicht durstig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerThirst(playerid, 50);
		    PlayerPlaySound(playerid, 32200, 0.0, 0.0, 0.0);
		}
		case 119://JUS DE POMME
		{
		    if(GetPlayerThirst(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not thirsty!", "Vous n'avez pas soif !", "¡No tiene sed!", "Portugais", "Non hai sete!", "Sie sint nicht durstig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerThirst(playerid, 35);
		    PlayerPlaySound(playerid, 32200, 0.0, 0.0, 0.0);
		}
		case 120://SOUPE AU POISSON
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 30);
		    GivePlayerThirst(playerid, 20);
		    GivePlayerSleep(playerid, 5);
		}
		case 121://SANDWICH À LA VIANDE
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 30);
		    GivePlayerSleep(playerid, 5);
		}
		case 123://FOUGÈRE
		{
		    if(CallRemoteFunction("GetPlayerMission", "i", playerid) == MISSION_REGGAE_3_VOODOO)
		    {
			    if(IsPlayerInRangeOfPoint(playerid, 5.0, -2012.5629, -2771.6755, 7.6943))
			    {
					CallRemoteFunction("OnCinematicGoesOn", "iiii", playerid, 6, 16, 0);
			    }
			    else
			    {
			    	ShowPlayerTextInfo(playerid, -1, "Use the plant inside of the ~r~Jellywitch's~n~~r~shack.", "Utilisez la fougère dans la ~r~cabane~n~~r~de la Jellywitch", "Espagnol", "Portugais", "Italien", "Allemand");
			    }
		    }
		}
		case 126://EAU NON-POTABLE
		{
			
			if(PlayerWatersPlants(playerid))
			{
				return 1;
			}
		    SendClientMessageEx(playerid, ROUGE, "Put this water in a pan, bringing it to boil will make it drinkable!", "Cuisez cette eau, la faire bouillir la rendra potable !", "Espagnol", "Portugais", "Italien", "Allemand");
			return 1;
		}
		case 127://ÉTAGÈRE
		{
		    new Float:x, Float:y, Float:z, Float:angle, Float:fTrash;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
			//---
			if(CA_RayCastLine(x, y, z, x + 1.0 * floatsin(-angle, degrees), y + 1.0 * floatcos(-angle, degrees), z, fTrash, fTrash, fTrash) != 0)
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is not enough room here!", "Il n'y a pas assez de place ici !", "Espagnol", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			GetXYInFrontOfPoint(x, y, angle, 0.9);
			SetPlayerRack(playerid, CreateGunRack(x, y, z, angle));
			EditDynamicObject(playerid, GetGunRackObject(GetPlayerRack(playerid)));
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 128://BRASERO
		{
		    new Float:x, Float:y, Float:z, Float:angle, Float:fTrash;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
			//---
			if(CA_RayCastLine(x, y, z, x + 0.5 * floatsin(-angle, degrees), y + 0.5 * floatcos(-angle, degrees), z, fTrash, fTrash, fTrash) != 0)
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is not enough room here!", "Il n'y a pas assez de place ici !", "Espagnol", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			GetXYInFrontOfPoint(x, y, angle, 0.4);
			SetPlayerBrasero(playerid, CreateBrasero(x, y, z, angle, 1));
			EditDynamicObject(playerid, GetBraseroObject(GetPlayerBrasero(playerid)));
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 130://FRIGO
		{
		    new Float:x, Float:y, Float:z, Float:angle, Float:fTrash;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
			//---
			if(CA_RayCastLine(x, y, z, x + 1.0 * floatsin(-angle, degrees), y + 1.0 * floatcos(-angle, degrees), z, fTrash, fTrash, fTrash) != 0)
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is not enough room here!", "Il n'y a pas assez de place ici !", "Espagnol", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			GetXYInFrontOfPoint(x, y, angle, 0.9);
			SetPlayerFridgeEdit(playerid, CreateFridge(x, y, z, angle));
			EditDynamicObject(playerid, GetFridgeObject(GetPlayerFridgeEdit(playerid)));
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 132://EXP
		{
		    GivePlayerExp(playerid, 125);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 135://GILET THERMIQUE
		{
		    GivePlayerClothe(playerid, TORSO_OBJECT_ID, 135, false, 0.0955, 0.0528, -0.009, 172.8, 90.7, 0.0, 1.2, 1.2, 1.2);
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 137://LAMPE
		{
		    if(!IsPlayerNearHouse(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You must be at a house to set up furniture!", "Vous devez être dans une maison pour poser des meubles !", "Espagnol", "Portugais", "Italien", "Allemand");
		        return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			PlayerSetFurniture(playerid, 1);
		}
		case 138://LACRYMOGÈNE
		{
		    if(IsPlayerInAnyVehicle(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You can't throw anything while in a vehicle!", "Vous ne pouvez pas lancer de projectiles depuis un véhicule !", "Espagnol", "Portugais", "Italien", "Allemand");
		        return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			//GivePlayerWeapon(playerid, 18, 1);
			PlayerThrowProjectile(playerid, THROW_GAS);
		}
		case 143://TÊTE DU ROI DES BAMBIS
		{
		    if(!IsPlayerNearHouse(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You must be at a house to set up furniture!", "Vous devez être dans une maison pour poser des meubles !", "Espagnol", "Portugais", "Italien", "Allemand");
		        return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			PlayerSetFurniture(playerid, 2);
		}
		case 144://TAPIS
		{
		    if(!IsPlayerNearHouse(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You must be at a house to set up furniture!", "Vous devez être dans une maison pour poser des meubles !", "Espagnol", "Portugais", "Italien", "Allemand");
		        return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			PlayerSetFurniture(playerid, 3);
		}
		case 146://BOITE
		{
		    new dBoxItems[] =
		    {
		        90, 96, 97, 104, 111, 113, 129, 134, 147, 148
		    };
			GivePlayerSlotObject(playerid, dBoxItems[random(sizeof(dBoxItems))], slot);
		}
		case 148://POUSSE DE SAPIN
		{
		    if(!CanPlayerPlantHere(playerid))
		    {
		        SendClientMessageEx(playerid, ROUGE, "You can't plant anything here!", "Vous ne pouvez pas planter ici !", "¡No puede plantar aquí!", "Portugais", "Italien", "Sie können nicht hier plantzen!");
				return 1;
		    }
		    new Float:x, Float:y, Float:z, Float:angle;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
		    CreatePlant(5, 0, 45 - GetPlayerSkill(playerid, FARMER_ID) * 5, x, y, z, angle);
			ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
			GivePlayerSlotObject(playerid, -1, slot);
		}
		case 149://BOITE
		{
		    new dBoxItems[] =
		    {
		        2, 10, 26, 30, 71, 91, 99, 126
		    };
			GivePlayerSlotObject(playerid, dBoxItems[random(sizeof(dBoxItems))], slot);
		}
		case 151://LIT
		{
		    if(!CanPlayerCreateBed(playerid))
		    {
				SendClientMessageEx(playerid, ROUGE, "You must be in a house or near a tent!", "Vous devez être dans une maison ou près d'une tente !", "¡Necessita estar en una casa o cerca de una tienda!", "Portugais", "Italien", "Allemand");
		        return 1;
			}
		    new Float:x, Float:y, Float:z, Float:a, Float:fTrash;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, a);
			//---
			if(CA_RayCastLine(x, y, z, x + 1.0 * floatsin(-a, degrees), y + 1.0 * floatcos(-a, degrees), z, fTrash, fTrash, fTrash) != 0)
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is not enough room here!", "Il n'y a pas assez de place ici !", "Espagnol", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			GetXYInFrontOfPoint(x, y, a, 0.7119);
			SetPlayerBed(playerid, CreateBed(2, x, y, z, a));
			EditDynamicObject(playerid, GetBedObject(GetPlayerBed(playerid)));
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 153://Hotdog
		{
		    if(GetPlayerHunger(playerid) > 90)
		    {
		        SendClientMessageEx(playerid, ROUGE, "You're not hungry!", "Vous n'avez pas faim !", "¡No tiene hambre!", "Portugais", "Non hai fame!", "Sie sint nicht hungrig!");
				return 1;
		    }
			GivePlayerSlotObject(playerid, -1, slot);
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0);
		    GivePlayerHunger(playerid, 15);
		    GivePlayerSleep(playerid, 5);
		}
		case 155://Fauteuil
		{
		    new Float:x, Float:y, Float:z, Float:a, Float:fTrash;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, a);
			//---
			if(CA_RayCastLine(x, y, z, x + 1.1 * floatsin(-a, degrees), y + 1.1 * floatcos(-a, degrees), z, fTrash, fTrash, fTrash) != 0)
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is not enough room here!", "Il n'y a pas assez de place ici !", "Espagnol", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			GetXYInFrontOfPoint(x, y, a, 1.0);
			SetPlayerSeat(playerid, CreateSeat(1, x, y, z, a));
			EditDynamicObject(playerid, GetSeatObject(GetPlayerSeat(playerid)));
		    GivePlayerSlotObject(playerid, -1, slot);
		}
		case 156://PANNEAU
		{
		    new Float:x, Float:y, Float:z, Float:angle, Float:fTrash;
		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, angle);
			//---
			if(CA_RayCastLine(x, y, z, x + 0.6 * floatsin(-angle, degrees), y + 0.6 * floatcos(-angle, degrees), z, fTrash, fTrash, fTrash) != 0)
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is not enough room here!", "Il n'y a pas assez de place ici !", "Espagnol", "Portugais", "Italien", "Allemand");
				return 1;
			}
			//---
			GetXYInFrontOfPoint(x, y, angle, 0.4);
			SetPlayerBoard(playerid, CreateBoard(x, y, z, angle));
			GivePlayerSlotObject(playerid, -1, slot);
			//---
			EditDynamicObject(playerid, GetBoardObject(GetPlayerBoard(playerid)));
		}
		case 158://RETOUR À L'ENVOYEUR
		{
			if(GiveOfflineItems(playerid))
			{
		    	SendClientMessageEx(playerid, BRUN, "Note: Thanks for choosing DumpYaTrash (C)!", "Note: Merci d'avoir choisi Vid'TesPoches (C) !", "Espagnol", "Portugais", "Italien", "Allemand");
			}
			else
			{
		    	SendClientMessageEx(playerid, ROUGE, "There is nothing to retrieve!", "Il n'y a rien à récupérer !", "Espagnol", "Portugais", "Italien", "Allemand");
			}
			GivePlayerSlotObject(playerid, -1, slot);
		}
		//HasPlayerItem(playerid, objectid)
	}
	return 1;
}