#include <a_samp>
#include <a_mysql>

#define HOST "localhost"
#define USERNAME "surviveall"
#define DBNAME "surviveall"
#define DBPASSWORD "dr411k31lg3r3tr0pd3s4r4c3"

#define TYPE_NOSELL                         (0)
#define TYPE_TOOL                           (1)
#define TYPE_MEDIC                          (2)
#define TYPE_WEAP                           (3)
#define TYPE_OTHER                          (4)
#define TYPE_VEH                            (5)
#define TYPE_CLOTHE                         (6)
#define TYPE_FOOD                           (7)
#define TYPE_RSRC                           (8)
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
#define MAX_ITEMS                       	(159)

new MySQL:dbMYSQL;

static aObjects[MAX_ITEMS][ObjectsInfos] =
{   //ID OBJET	//AFFICHAGE             //DANS LA MAIN                          			//POSITION AU SOL   			//PRIX-HDV				//NOM OBJET
	{19300, 	0.0, 0.0, 0.0, 1.0, 	0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 					0.0, 0.0, 0.0, 0.0, 			0, TYPE_NOSELL,	false,	"Nothing", "Rien", "Nada", "Nada", "Niente", "Nichts"},//0
	{3082, 		0.0, 0.0, 0.0, 1.0, 	0.0867, 0.0127, -0.0767, 0.0, 0.0, 0.0, 0.5,		90.0, 0.0, 90.0, -0.714, 		5, TYPE_OTHER, false,	"Tent", "Tente", "Espagnol", "Portugais", "Tenda", "Zelt"},
	{11738, 	0.0, 0.0, 0.0, 1.0, 	0.2680, 0.0111, 0.0, 0.0, 270.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.953, 			3, TYPE_MEDIC, false,	"Medikit", "Medikit", "Medikit", "Medikit", "Medikit", "Medikit"},
	{334,		0.0, 315.0, 0.0, 1.5,	0.0036, 0.0111, 0.0, 0.0, 0.0, 0.0, 1.0,			90.0, 0.0, 100.0, -0.958,		3, TYPE_WEAP, false,	"Nitestick", "Matraque", "Espagnol", "Portugais", "Sfollagente", "Kn�ppel"},
	{335,		0.0, 315.0, 0.0, 1.0,	0.0036, 0.0111, 0.0, 0.0, 0.0, 0.0, 1.0,			90.0, 0.0, 100.0, -0.958,		3, TYPE_WEAP, false,	"Knife", "Couteau", "Espagnol", "Portugais", "Coltello", "Messer"},
	{336,		0.0, 315.0, 0.0, 2.0,	0.0036, 0.0056, -0.336, 0.0, 0.0, 0.0, 1.0,			90.0, 0.0, 100.0, -0.958,		3, TYPE_WEAP, false,	"Bat", "Batte", "Espagnol", "Portugais", "Italien", "Schl�ger"},
	{339,		0.0, 315.0, 0.0, 2.0,	0.0036, 0.0056, -0.336, 0.0, 0.0, 0.0, 1.0,			90.0, 0.0, 100.0, -0.958,		4, TYPE_WEAP, false,	"Katana", "Katana", "Espagnol", "Portugais", "Katana", "Schwert"},
	{341,		0.0, 0.0, 60.0, 2.0,	0.0036, -0.0053, -0.0065, 0.0, 0.0, 0.0, 1.0, 		90.0, 0.0, 100.0, -0.958,		5, TYPE_WEAP, false,	"Chainsaw", "Tronconneuse", "Espagnol", "Portugais", "Italien", "Motor-~n~s�ge"},
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
	{1241,		0.0, 135.0, 180.0, 1.0,	0.0895, 0.058, 0.0, 165.0, 0.0, 132.0, 0.5,			0.0, 120.0, 0.0, -0.943,		2, TYPE_MEDIC, false,	"Cofein pill", "Cafeine", "Cafeina", "Portugais", "Italien", "Koffe�n"},
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
	{18927,		270.0, 0.0, 270.0, 1.0,	0.158, 0.0608, 0.0, 180.0, 0.0, 270.0, 1.0,			0.0, 270.0, 0.0, -0.95,			2, TYPE_CLOTHE,	false,	"Blue cap", "Casquette~n~bleue", "Espagnol", "Portugais", "Italien", "Blaue~n~Schirmm�tze"},
	{18930,		270.0, 0.0, 270.0, 1.0,	0.158, 0.0608, 0.0, 180.0, 0.0, 270.0, 1.0,			0.0, 270.0, 0.0, -0.95,			2, TYPE_CLOTHE,	false,	"Red cap", "Casquette~n~rouge", "Espagnol", "Portugais", "Italien", "Rote~n~Schirmm�tze"},//50
	{18949, 	270.0, 0.0, 270.0, 1.0,	0.1956, 0.0261, 0.0, 180.0, 90.0, 70.0, 1.0,		0.0, 270.0, 0.0, -0.95,			1, TYPE_CLOTHE,	false,	"Green~n~bowler", "Chapeau~n~melon vert", "Espagnol", "Portugais", "Italien", "Gr�ne~n~melone"},
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
	{2881,		180.0,315.0,270.0,1.0,	0.159, 0.1728, 0.0607, 90.0, 90.0, 180.0, 1.0,		335.0, 270.0, 0.0, -0.9335,		2, TYPE_FOOD, false,	"Pizza slice", "Part de~n~pizza", "Espagnol", "Portugais", "Pezzo di~n~pizza", "St�ck~n~pizza"},
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
	{19801,		0.0, 0.0, 0.0, 1.0,		0.1808, 0.0216, 0.0, 0.0, 90.0, 0.0, 1.0,			0.0, 0.0, 0.0, -0.886,			2, TYPE_CLOTHE, false, 	"Hood", "Cagoule", "Capucha", "Cap�", "Cappuccio", "Kapuze"},
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
	{19621,		0.0, 0.0, 0.0, 1.0, 	0.0981, 0.0275, 0.0, 330.0, 180.0, 270.0, 1.0,		0.0, 0.0, 0.0, -0.906,			0, TYPE_NOSELL,	false,	"Can", "Burette", "Espagnol", "Portugais", "Italien", "�lkanne"},
	{19837,		0.0, 0.0, 0.0, 1.0,		0.0921, -0.0223, 0.0516, 180.0, 0.0, 0.0, 0.5,		0.0, 0.0, 0.0, -1.0,			1, TYPE_OTHER, false,	"Wheat seeds", "Graines~n~de ble", "Semillas de~n~trigo", "Portugais", "Semi di~n~grano", "Weizen~n~saat"},
	{1453,		0.0, 90.0, 0.0, 1.0,	0.0935, 0.0277, 0.0, 0.0, 0.0, 0.0, 0.3,			0.0, 0.0, 0.0, -2.018,			1, TYPE_OTHER, false,	"Wheat", "Ble", "Trigo", "Portugais", "Grano", "Weizen"},
	{920,		0.0, 0.0, 0.0, 1.0,		0.0593, 0.2803, 0.3084, 98.0, 180.0, 113.38, 0.6,	0.0, 0.0, 270.0, -0.516,		30, TYPE_TOOL, true,	"Shredder", "Broyeur", "Espagnol", "Portugais", "Italien", "M�hle"},
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

public OnFilterScriptInit()
{
    dbMYSQL = mysql_connect(HOST, USERNAME, DBPASSWORD, DBNAME);
    if(mysql_errno() != 0)
        printf("Connection to database failed !");
    else
    {
        printf("|--- Connected to host %s ---|", _:dbMYSQL);
        printf("Preparing save of objects...");
        new tick = GetTickCount();
        for(new i = 0; i < sizeof(aObjects); i++)
        {
            new mysqlquery[1024];
            mysql_format(dbMYSQL, mysqlquery, sizeof(mysqlquery), "INSERT INTO Object (idmodel,orotx,oroty,orotz,ozoom,ohoffsetx,ohoffsety,ohoffsetz,ohrotx,ohroty,ohrotz,ohoffzoom,ogroundrotx,ogroundroty,ogroundrotz,ogroundoffsetz,sellprice,typeobject,heavy,name_en,name_fr,name_es,name_pg,name_it,name_de) VALUES(%d", _:aObjects[i][0]);
            for(new j = 1; j < 19; j++)
            {
                new little_query[64];
                if(j <= 15)
                    mysql_format(dbMYSQL, little_query, sizeof(little_query), ",%.4f", _:aObjects[i][j]);
                else
                    mysql_format(dbMYSQL, little_query, sizeof(little_query), ",%d", _:aObjects[i][j]);
                strcat(mysqlquery, little_query, sizeof(mysqlquery));
            }
            mysql_format(dbMYSQL, mysqlquery, sizeof(mysqlquery), "%s,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\")", mysqlquery, aObjects[i][ObjectEnName],aObjects[i][ObjectFrName],aObjects[i][ObjectEsName],aObjects[i][ObjectPgName], aObjects[i][ObjectItName],aObjects[i][ObjectDeName]);
            mysql_pquery(dbMYSQL, mysqlquery);
            printf("*** OBJECT %s saved (%d) ***", aObjects[i][ObjectEnName], i);
        }
        printf("------------------------------------\n\n|--- All objects were correctly saved in %d ms ---|", GetTickCount() - tick);
    }
    return 1;
}

public OnFilterScriptExit()
{
    if(!mysql_close())
        printf("Close the database connection failed !");
    return 1;
}