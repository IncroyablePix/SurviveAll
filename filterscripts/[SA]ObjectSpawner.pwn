/*-------------------------------------------------------------------------------//
\\  - Spawn dynamique des objets du serveur -
//
\\  	Fonctions globales:
//          CreateRandomItem()
\\          PickRandomItem(neutral, gun, vehicle, medic, clothes, bag, level)
//
//      Fonctions locales:
\\          GetItemSpawnPos(spawnid, &Float:x, &Float:y, &Float:z, &neutral, &gun, &vehicle, &medic, &clothe, &bag)
//          InitializeSpawnObject()
\\          GetRandomItem(category, level)
//          
\\
//-------------------------------------------------------------------------------*/
#include <a_samp>
#include <memory>
#include <[SA]Defines.inc>
#include <[SA]Functions.inc>
//#undef PROFILING
/*#if defined PROFILING
#include <profiler.inc>
#endif
*/
//---PARAMÉTRAGE
//#define ADVANCED_ITEM_SPAWN

//---DEFINES
#define NEUTRAL_SPAWN                       (0)
#define GUN_SPAWN                           (1)
#define VEHICLE_SPAWN                       (2)
#define MEDIC_SPAWN                       	(3)
#define CLOTHE_SPAWN                       	(4)
#define BAG_SPAWN                       	(5)
#define MAX_ITEM_SPAWNS                     (2192)
#define MAX_SPAWNED_ITEMS                   (200 + 105 + 225 + 200 + 200)

//---CATÉGORIES                             NEUTRE ARME VÉHICULE MÉDICAL VÊTEMENT SAC
#define REGULAR                             35,15,10,15,15,10,LEVEL_1
#define WAREHOUSE                           25,5,40,10,10,10,LEVEL_1
#define GUNSHOP                             10,65,5,10,5,5,LEVEL_1
#define AREA                             	5,75,5,5,5,5,LEVEL_2
#define SHOP                                45,10,5,10,20,10,LEVEL_1
#define POLICE                              30,20,20,10,10,10,LEVEL_1
#define HOSPITAL                            6,6,6,70,6,6,LEVEL_1
#define GAS                                 35,5,30,10,10,10,LEVEL_1
#define FARM                                65,15,5,5,15,10,LEVEL_1
//#define WAREHOUSE                           (45, 5, 20, 10, 5, 15)
//#define HOSPITAL                            (30, 20, 20, 10, 10, 10)

//---NIVEAUX DE LOOT
#define LEVEL_1                             (0)
#define LEVEL_2                             (1)
#define LEVEL_3                             (2)
#define LEVEL_4                             (3)

//---AUGMENTATION DU STACK/HEAP
#pragma dynamic 25000

//---FONCTIONS
forward CreateRandomItem();
forward PickRandomItem(neutral, gun, vehicle, medic, clothes, bag, level);

//---VARIABLES
static Pointer:dSpawnObjects[MAX_ITEM_SPAWNS] = {MEM_NULLPTR, ...};
static dItemsHistory[MAX_SPAWNED_ITEMS] = {-1, ...};
static dHistory;

//------------------------------------------------//

public OnFilterScriptInit()
{
	print("\n--------------------------------------");
	print("Survive-All object spawner - [Pix]");
	print("- Loading");
	print("...");
	//InitializeSpawnObject();
	/*#if defined PROFILING
 	Profiler_Start();
	#endif*/
	print("- Loaded");
	print("--------------------------------------\n");
	return 1;
}

public OnFilterScriptExit()
{
	print("\n--------------------------------------");
	print("Survive-All object spawner - [Pix]");
	print("- Unloaded");
	print("--------------------------------------\n");
	#if defined PROFILING
 	Profiler_Stop();
	#endif
	return 1;
}


GetItemSpawnPos(spawnid, &Float:x, &Float:y, &Float:z, &neutral, &gun, &vehicle, &medic, &clothe, &bag, &level)
{
	enum ItemsPos
	{
		Float:xPos,
		Float:yPos,
		Float:zPos,
		dNeutral,
		dGun,
		dVehicle,
		dMedic,
		dClothe,
		dBag,
		dLevel
	}
	new const Float:aRandomItemPos[MAX_ITEM_SPAWNS][ItemsPos] =//Les positions de spawns possibles des objets + la probabilité de ces positions par rapport à ce qu'on peut y trouver
	{
		//---BLUEBERRY
	    {244.9132, -55.5916, 1.5776, SHOP},//Magasin Blueberry
	    {245.1248, -58.9730, 1.5776, SHOP},//Magasin Blueberry
     	{203.2533,35.4381,2.5781, REGULAR}, // Blueberry
     	{133.7623,-11.1563,1.5781, REGULAR}, // Blueberry
     	{133.7853,-36.2299,1.5781, REGULAR}, // Blueberry
	    {153.8488,-60.0470,1.5781, REGULAR}, // Blueberry
	    {195.4190,-94.8737,4.8965, REGULAR}, // Blueberry
	    {157.8816,-107.6451,1.5545, REGULAR}, // Blueberry
 	   	{161.2228,-117.0082,4.8965, REGULAR}, // Blueberry
  	   	{159.2129,-154.3250,5.0786, REGULAR}, // Blueberry
  	   	{165.9979,-165.6759,6.7786, REGULAR}, // Blueberry
  	   	{170.6590,-153.9628,7.7188, REGULAR}, // Blueberry
  	   	{206.5619,-165.0000,1.5781, REGULAR}, // Blueberry
  	   	{252.2779,-116.9929,3.5354, REGULAR}, // Blueberry
  	   	{255.7188,-104.7689,3.5418, REGULAR}, // Blueberry
  	   	{260.6796,-92.3886,3.5354, REGULAR}, // Blueberry
  	   	{266.2427,-56.3335,2.7772, REGULAR}, // Blueberry
  	   	{282.8298,-0.9670,2.4186, REGULAR}, // Blueberry
  	   	{264.1959,21.7988,3.4332, REGULAR}, // Blueberry
  	   	{289.8383,40.3593,2.5596, REGULAR}, // Blueberry
  	   	{335.2148,-31.9414,8.2573, REGULAR}, // Blueberry
  	   	{330.0164,-45.6921,6.7713, REGULAR}, // Blueberry
  	   	{329.8932,-48.7472,8.2573, REGULAR}, // Blueberry
  	   	{353.3458,-103.5931,1.2970, REGULAR}, // Blueberry
  	   	{348.1770,-115.8410,1.2618, REGULAR}, // Blueberry
  	   	{363.3761,-109.8526,1.2280, REGULAR}, // Blueberry
  	   	{368.2375,-115.6503,1.2467, REGULAR}, // Blueberry
  	   	{355.0563,-66.8358,1.4047, REGULAR}, // Blueberry
  	   	{313.3176,-97.3321,3.5354, REGULAR}, // Blueberry
  	   	{316.5764,-124.5590,2.5756, REGULAR}, // Blueberry
  	   	{295.3814,-195.7444,5.5786, REGULAR}, // Blueberry
  	   	{309.0447,-191.2877,1.5781, REGULAR}, // Blueberry
  	   	{318.9026,-166.2528,1.5781, REGULAR}, // Blueberry
  	   	{265.2685,-266.7929,1.5836, REGULAR}, // Blueberry
  	   	{259.4923,-302.7936,1.9184, REGULAR}, // Blueberry
  	   	{227.3270,-303.6272,1.9262, REGULAR}, // Blueberry
  	   	{196.0722,-294.8649,1.5724, WAREHOUSE}, // BlueberryEntrepot
  	   	{152.3654,-258.1058,1.5724, WAREHOUSE}, // BlueberryEntrepot
  	   	{94.0943,-314.6150,1.5781, WAREHOUSE}, // BlueberryEntrepot
  	   	{26.5283,-288.6091,2.3355, WAREHOUSE}, // BlueberryEntrepot
  	   	{90.8688,-184.4182,1.4844, WAREHOUSE}, // BlueberryEntrepot
  	   	{254.8791,-192.1309,1.5781, GUNSHOP}, // BlueberryAmmu
  	   	{254.8715,-163.4795,1.5781, GUNSHOP}, // BlueberryAmmu
  	   	{263.1078,-163.3080,5.0786, GUNSHOP}, // BlueberryAmmu
  	   	{248.7236,-165.9337,10.0963, GUNSHOP}, // BlueberryAmmu
	    {194.3976, -225.6373, 1.7786, REGULAR}, // Blueberry
     	{195.8202, -232.7281, 1.7786, REGULAR}, // Blueberry
  	   	//---DILLIMORE
  	   	{728.2488, -469.8271, 16.6569, WAREHOUSE}, // Usine
  	   	{730.4300, -455.9790, 18.3722, WAREHOUSE}, // Usine
  	   	{666.3660, -564.0172, 16.3424, GAS}, // Station Service
  	   	{670.0964, -572.7451, 17.1502, GAS}, // Station Service
  	   	{664.5506, -567.6747, 17.2173, GAS}, // Station Service
  	   	{616.7886, -611.1608, 17.2266, POLICE}, // Commissariat Dillimore
  	   	{609.2792, -611.3864, 17.2266, POLICE}, // Commissariat Dillimore
  	   	{627.6202, -595.4998, 19.9759, POLICE}, // Commissariat Dillimore
  	   	{611.0815, -583.4946, 18.2109, POLICE}, // Commissariat Dillimore
  	   	{807.2624, -549.1729, 21.9616, POLICE}, // Dillimore Camp Toit
  	  	{808.6564, -541.9053, 21.3363, POLICE}, // Dillimore Camp Toit
  	  	{816.6102, -570.6110, 19.3363, POLICE}, // Dillimore Camp Toit
  	   	{740.3135, -555.2507, 18.0129, REGULAR}, // Dillimore
  	   	{764.1720, -597.8099, 17.3359, REGULAR}, // Dillimore
  	   	{753.9489, -591.2416, 18.0133, REGULAR}, // Dillimore
   	 	{786.1670, -488.3251, 17.3439, REGULAR}, // Dillimore
  	   	{795.3204, -497.1927, 18.0133, REGULAR}, // Dillimore
  	   	{818.2068, -493.3661, 18.0129, REGULAR}, // Dillimore
  	   	{860.0707, -603.8809, 18.4219, REGULAR}, // Dillimore
      	{831.8586, -642.1316, 16.3359, REGULAR}, // Dillimore
      	{791.2366, -640.1158, 16.3359, REGULAR}, // Dillimore
      	{681.5616, -469.8878, 22.5705, REGULAR}, // Dillimore
      	{673.1182, -466.5346, 16.5363, REGULAR}, // Dillimore
      	{668.4253, -455.3132, 17.3363, REGULAR}, // Dillimore
      	{623.1923, -506.2421, 16.3525, REGULAR}, // Dillimore
      	//---HILLTOP FARM
      	{1034.9760, -363.4945, 81.3462, FARM},
      	{1035.0504, -363.7828, 89.7461, FARM},
      	{1013.7227, -313.7285, 73.9931, FARM},
      	{1024.9248, -313.4337, 73.9931, FARM},
      	{1019.6969, -298.9383, 77.3594, FARM},
      	{1045.7572, -298.9864, 73.9931, FARM},
      	{1045.6130, -287.5745, 77.4205, FARM},
      	{1114.4220, -286.3461, 81.5712, FARM},
      	{1114.5515, -286.4113, 89.9711, FARM},
      	{1068.0331, -362.1055, 76.0474, FARM},
      	//---THE PANOPTICON
      	{-470.4603, -67.2443, 61.0473, FARM}, // ThePanopticon
      	{-466.0797, -44.7408, 60.7763, FARM}, // ThePanopticon
      	{-475.1996, -40.5346, 59.9851, FARM}, // ThePanopticon
      	{-476.3592, -58.5835, 61.1153, FARM}, // ThePanopticon
      	{-463.8313, -40.1814, 59.9545, FARM}, // ThePanopticon
      	{-546.4752, -74.7096, 63.7059, FARM}, // ThePanopticon
      	{-537.3633, -98.9340, 63.2969, FARM}, // ThePanopticon
      	{-520.4016, -181.0188, 77.9708, FARM}, // ThePanopticon
      	{-473.0079, -171.3329, 78.2109, FARM}, // ThePanopticon
      	{-552.4346, -181.4280, 78.4063, FARM}, // ThePanopticon
      	{-563.3704, -181.6205, 78.4063, FARM}, // ThePanopticon
      	{-752.5350, -131.2449, 65.8281, FARM}, // ThePanopticon
		//---FERN RIDGE
		{874.5120, -16.7314, 63.1953, REGULAR}, // ObjetFernRidge
      	{861.7542, -24.7643, 63.7234, REGULAR}, // ObjetFernRidge
		{525.1008, -93.9295, 37.3609, REGULAR}, // ObjetFernRidge
		{677.2202, -33.2995, 22.3649, REGULAR}, // ObjetFernRidge
		{724.1721, 268.8342, 22.4531, REGULAR}, // ObjetFernRidge
		{752.2087, 258.8472, 27.0859, REGULAR}, // ObjetFernRidge
		{760.0130, 378.6435, 23.1695, REGULAR},// ObjetFernRidge
      	//---MONTGOMMERY
      	{1188.8466, 146.5536, 20.5308, REGULAR}, // ObjetMontgommery
      	{1243.8511, 216.0622, 19.5547, REGULAR}, // ObjetMontgommery
      	{1188.7440, 231.0359, 19.5547, REGULAR}, // ObjetMontgommery
      	{1199.4943, 242.9037, 19.5547, REGULAR}, // ObjetMontgommery
      	{1251.3165, 280.9708, 19.7578, REGULAR}, // ObjetMontgommery
      	{1262.5326, 308.1467, 19.7500, REGULAR}, // ObjetMontgommery
      	{1245.6039, 333.2823, 19.5547, HOSPITAL}, // ObjetMontgommeryHosto
      	{1238.6534, 328.2580, 19.7555, HOSPITAL}, // ObjetMontgommeryHosto
      	{1244.8597, 325.4151, 19.7555, HOSPITAL}, // ObjetMontgommeryHosto
      	{1252.0690, 325.6419, 19.7578, HOSPITAL}, // ObjetMontgommeryHosto
      	{1234.6396, 333.4040, 19.7578, HOSPITAL}, // ObjetMontgommeryHosto
      	{1262.4506, 367.8698, 19.5547, REGULAR}, // ObjetMontgommery
      	{1272.9232, 392.5455, 19.5614, REGULAR}, // ObjetMontgommery
      	{1271.5482, 380.3263, 22.5555, REGULAR}, // ObjetMontgommery
      	{1237.6869, 374.0251, 19.5547, REGULAR}, // ObjetMontgommery
      	{1254.6448, 360.0680, 23.5555, REGULAR}, // ObjetMontgommery
      	{1282.8083, 385.6173, 27.5555, REGULAR}, // ObjetMontgommery
      	{1295.3043, 391.8799, 19.5547, REGULAR}, // ObjetMontgommery
      	{1323.3273, 392.0551, 19.5547, REGULAR}, // ObjetMontgommery
      	{1311.9265, 397.8679, 19.5547, REGULAR}, // ObjetMontgommery
      	{1350.6832, 473.1524, 24.1752, REGULAR}, // ObjetMontgommery
      	{1371.4988, 474.6391, 20.0979, GAS}, // ObjetMontgommeryEssence
      	{1386.9023, 463.4934, 20.1962, GAS}, // ObjetMontgommeryEssence
      	{1392.8896, 464.6190, 20.1556, GAS}, // ObjetMontgommeryEssence
      	{1390.0310, 472.0880, 20.0901, GAS}, // ObjetMontgommeryEssence
      	{1401.6748, 462.0825, 20.2109, GAS}, // ObjetMontgommeryEssence
      	{1395.1954, 426.7251, 19.9981, REGULAR}, // ObjetMontgommery
      	{1394.0862, 400.9739, 19.8046, REGULAR}, // ObjetMontgommery
      	{1421.7284, 385.6088, 19.1847, REGULAR}, // ObjetMontgommery
      	{1415.0356, 324.6099, 18.8438, REGULAR}, // ObjetMontgommery
      	{1382.8062, 304.7461, 22.5555, REGULAR}, // ObjetMontgommery
      	{1368.4703, 312.0078, 22.5555, REGULAR}, // ObjetMontgommery
      	{1356.0789, 312.4061, 24.5555, REGULAR}, // ObjetMontgommery
      	{1374.5034, 303.0047, 25.5555, REGULAR}, // ObjetMontgommery
      	{1411.1512, 298.0901, 19.5547, REGULAR}, // ObjetMontgommery
      	{1426.2302, 269.0697, 19.5547, REGULAR}, // ObjetMontgommery
      	{1423.8781, 233.4879, 19.5618, REGULAR}, // ObjetMontgommery
      	{1417.8513, 220.2100, 19.5618, REGULAR}, // ObjetMontgommery
      	{1415.0465, 236.2100, 24.0027, REGULAR}, // ObjetMontgommery
      	{1359.3979, 197.4415, 23.2270, REGULAR}, // ObjetMontgommery
      	{1360.9247, 184.4821, 24.2271, REGULAR}, // ObjetMontgommery
      	{1331.0382, 174.6585, 20.5196, REGULAR}, // ObjetMontgommery
      	{1310.6335, 164.9023, 20.4609, REGULAR}, // ObjetMontgommery
      	{1299.5131, 135.4693, 20.3922, REGULAR}, // ObjetMontgommery
      	{1291.5072, 202.1897, 20.0827, REGULAR}, // ObjetMontgommery
      	{1246.0345, 208.5197, 23.0555, REGULAR}, // ObjetMontgommery
      	{1239.6287, 228.8465, 28.0728, REGULAR}, // ObjetMontgommery
      	{1263.7673, 242.6158, 31.1073, REGULAR}, // ObjetMontgommery
      	{1258.5964, 289.0524, 19.5547, REGULAR}, // ObjetMontgommery
      	{1284.2198, 303.2307, 19.5547, REGULAR}, // ObjetMontgommery
      	{1289.3450, 294.2630, 25.7998, REGULAR}, // ObjetMontgommery
      	{1293.3528, 307.4137, 27.5555, REGULAR}, // ObjetMontgommery
      	{1368.7562, 356.6292, 20.5474, REGULAR}, // ObjetMontgommery
	    {1320.5599, 288.9873, 19.5547, REGULAR}, // ObjetMontgommery
	    {1339.6984, 278.1636, 19.5615, REGULAR}, // ObjetMontgommery
      	//---PALOMINO CREEK
      	{2231.0530, 171.9567, 27.4797, REGULAR}, // ObjetPalomino
      	{2236.4065, 184.0423, 28.1540, REGULAR}, // ObjetPalomino
      	{2263.2754, 167.3594, 28.1536, REGULAR}, // ObjetPalomino
      	{2281.1777, 161.1526, 28.4416, REGULAR}, // ObjetPalomino
      	{2269.1841, 119.5480, 28.4416, REGULAR}, // ObjetPalomino
      	{2249.0959, 119.2666, 28.4416, REGULAR}, // ObjetPalomino
      	{2250.1919, 111.0302, 28.4416, REGULAR}, // ObjetPalomino
      	{2195.7703, 107.7356, 28.4416, REGULAR}, // ObjetPalomino
      	{2196.0107, 61.5888, 28.4416, REGULAR}, // ObjetPalomino
      	{2190.3699, -37.1907, 28.1540, REGULAR}, // ObjetPalomino
      	{2187.5801, -60.6148, 28.1540, REGULAR}, // ObjetPalomino
      	{2246.9658, -120.5711, 28.1535, REGULAR}, // ObjetPalomino
      	{2272.6008, -128.0607, 28.1535, REGULAR}, // ObjetPalomino
      	{2293.3184, -124.2235, 28.1535, REGULAR}, // ObjetPalomino
      	{2316.7166, -123.6107, 28.1536, REGULAR}, // ObjetPalomino
      	{2321.6404, -65.9730, 26.4844, REGULAR}, // ObjetPalomino
      	{2327.2979, -86.1200, 31.4834, REGULAR}, // ObjetPalomino
      	{2327.9961, -62.2562, 30.8072, REGULAR}, // ObjetPalomino
      	{2310.3464, -33.2755, 32.6079, REGULAR}, // ObjetPalomino
      	{2310.3464, -30.6810, 32.6079, REGULAR}, // ObjetPalomino
      	{2310.3464, -27.7797, 32.6079, REGULAR}, // ObjetPalomino
      	{2310.3464, -24.8699, 32.6079, REGULAR}, // ObjetPalomino
      	{2310.3455, -21.7810, 32.6881, REGULAR}, // ObjetPalomino
      	{2306.2959, -0.8608, 26.7500, REGULAR}, // ObjetPalomino
      	{2306.1470, -7.8375, 26.7500, REGULAR}, // ObjetPalomino
      	{2324.1519, -3.7753, 26.5591, REGULAR}, // ObjetPalomino
      	{2328.9207, 8.4653, 26.5243, REGULAR}, // ObjetPalomino
      	{2317.6311, 12.2412, 26.4844, REGULAR}, // ObjetPalomino
      	{2315.9136, 40.2415, 26.4844, REGULAR}, // ObjetPalomino
      	{2315.5300, 33.6928, 27.4740, REGULAR}, // ObjetPalomino
      	{2320.9084, 44.5926, 26.4857, REGULAR}, // ObjetPalomino
      	{2314.7227, 56.5117, 26.4812, REGULAR}, // ObjetPalomino
      	{2331.3228, 42.6331, 32.9884, GUNSHOP}, // ObjetPalominoArmurerie
      	{2331.6233, 59.6848, 32.0074, GUNSHOP}, // ObjetPalominoArmurerie
      	{2326.4409, 64.0365, 26.4922, GUNSHOP}, // ObjetPalominoArmurerie
      	{2323.8250, 67.1004, 26.4867, GUNSHOP}, // ObjetPalominoArmurerie
      	{2333.6460, 61.6108, 26.7058, GUNSHOP}, // ObjetPalominoArmurerie
      	{2316.6765, 116.1675, 28.4416, REGULAR}, // ObjetPalomino
      	{2321.8164, 132.2230, 27.8438, REGULAR}, // ObjetPalomino
      	{2324.5574, 171.5934, 28.4416, REGULAR}, // ObjetPalomino
      	{2316.1138, 161.9845, 28.4416, REGULAR}, // ObjetPalomino
      	{2366.8503, 146.2766, 27.8438, REGULAR}, // ObjetPalomino
      	{2366.5374, 103.8164, 28.4480, REGULAR}, // ObjetPalomino
      	{2366.1025, 71.2609, 28.4416, REGULAR}, // ObjetPalomino
      	{2370.1841, 54.5795, 28.4480, REGULAR}, // ObjetPalomino
      	{2365.7458, 21.9532, 28.4416, REGULAR}, // ObjetPalomino
      	{2366.1741, -8.4774, 28.4416, REGULAR}, // ObjetPalomino
      	{2424.1707, 5.3989, 26.4844, REGULAR}, // ObjetPalomino
      	{2422.8564, 19.1007, 26.8633, REGULAR}, // ObjetPalomino
      	{2423.4480, 28.6875, 26.9844, REGULAR}, // ObjetPalomino
      	{2409.4045, 63.6441, 27.8438, REGULAR}, // ObjetPalomino
      	{2439.0791, 61.1093, 28.4416, REGULAR}, // ObjetPalomino
      	{2496.7991, 73.1117, 26.4844, REGULAR}, // ObjetPalomino
      	{2481.7856, 96.3293, 27.6835, REGULAR}, // ObjetPalomino
      	{2494.2781, 87.1133, 26.4844, REGULAR}, // ObjetPalomino
      	{2511.3293, 82.9334, 26.8630, REGULAR}, // ObjetPalomino
      	{2515.8608, 145.1986, 26.9766, REGULAR}, // ObjetPalomino
      	{2537.3577, 128.2917, 27.6835, REGULAR}, // ObjetPalomino
      	{2514.4604, 96.7650, 27.6835, REGULAR}, // ObjetPalomino
      	{2558.7068, 71.9831, 26.4766, REGULAR}, // ObjetPalomino
      	{2567.5852, 58.0722, 26.9766, REGULAR}, // ObjetPalomino
      	{2567.3455, 13.3949, 26.8530, REGULAR}, // ObjetPalomino
      	{2550.5830, -6.3654, 27.6756, REGULAR}, // ObjetPalomino
      	{2501.0139, -32.4174, 28.4480, REGULAR}, // ObjetPalomino
      	{2494.6694, -50.4375, 26.9711, REGULAR}, // ObjetPalomino
      	{2439.0994, -64.0748, 28.1540, REGULAR}, // ObjetPalomino
      	{2256.4607, -60.7772, 26.4985, REGULAR}, // ObjetPalomino
      	{2240.2053, -83.4083, 26.5011, REGULAR}, // ObjetPalomino
      	{2266.2910, -76.7895, 24.5859, REGULAR}, // ObjetPalomino
      	{2262.2300, -69.5527, 31.6016, REGULAR}, // ObjetPalomino
	    //---FERMES
    	{-98.1250, -94.5727, 3.1181, FARM}, // ObjetFerme
    	{-100.2571, -105.9187, 3.1181, FARM}, // ObjetFerme
    	{-87.2809, -107.9117, 3.1181, FARM}, // ObjetFerme
    	{-72.7884, -110.1682, 3.1181, FARM}, // ObjetFerme
    	{-71.0808, -98.2656, 3.1181, FARM}, // ObjetFerme
    	{-77.0064, -103.4068, 6.4844, FARM}, // ObjetFerme
    	{-90.8040, -101.3428, 6.4844, FARM}, // ObjetFerme
    	{-120.8500, -101.1512, 3.1181, FARM}, // ObjetFerme
    	{-128.8728, -88.5974, 3.1181, FARM}, // ObjetFerme
    	{-135.0515, -92.2337, 3.1181, FARM}, // ObjetFerme
    	{-140.3430, -98.2257, 3.1181, FARM}, // ObjetFerme
    	{-99.6587, -49.3896, 3.1172, FARM}, // ObjetFerme
    	{-93.1791, -38.5548, 3.1172, FARM}, // ObjetFerme
    	{-84.5921, -35.7419, 3.1172, FARM}, // ObjetFerme
    	{-92.2993, -24.3411, 3.1172, FARM}, // ObjetFerme
    	{-90.2654, -36.5113, 6.4844, FARM}, // ObjetFerme
    	{-71.6434, 35.9902, 3.1103, FARM}, // ObjetFerme
    	{-60.5383, 32.1236, 3.1103, FARM}, // ObjetFerme
    	{-52.4267, 57.6431, 3.1103, FARM}, // ObjetFerme
    	{-66.9647, 50.9517, 3.1103, FARM}, // ObjetFerme
    	{-62.2951, 50.4604, 6.4766, FARM}, // ObjetFerme
    	{-48.0590, 30.0422, 6.4844, FARM}, // ObjetFerme
    	{-39.7680, 53.2548, 3.1172, FARM}, // ObjetFerme
    	{-42.7638, 26.3583, 3.1172, FARM}, // ObjetFerme
    	{1920.7844, 170.2320, 38.7830, FARM}, // ObjetFerme2
    	{1925.6339, 168.3367, 38.7830, FARM}, // ObjetFerme2
    	{1904.6565, 163.1861, 37.1406, FARM}, // ObjetFerme2
    	{1906.6711, 173.5109, 37.1979, FARM}, // ObjetFerme2
		//---CASCADE
 		{-876.6891, -295.1016, 3.0856, 20, 20, 15, 20, 20, 5}, // ObjetCascade
 		{-894.9825, -292.3394, 4.6733, 20, 20, 15, 20, 20, 5}, // ObjetCascade
	  	{-885.4462, -290.5246, 3.8099, 20, 20, 15, 20, 20, 5}, // ObjetCascade
	  	{-875.7734, -302.9433, 1.7021, 20, 20, 15, 20, 20, 5}, // ObjetCascade
	  	{-880.9965, -300.9411, 2.6056, 20, 20, 15, 20, 20, 5}, // ObjetCascade
	  	{-889.3992, -300.0420, 1.8120, 20, 20, 15, 20, 20, 5}, // ObjetCascade
	  	{-885.4760, -307.3274, 2.0788, 20, 20, 15, 20, 20, 5}, // ObjetCascade
	  	{-877.3897, -308.0403, 1.4568, 20, 20, 15, 20, 20, 5}, // ObjetCascade
	  	{2355.9619, -652.6178, 128.0547, REGULAR}, // ObjetRedCounty
	  	{2355.8196, -648.6332, 128.0547, REGULAR}, // ObjetRedCounty
	  	{2350.9009, -647.6627, 128.0547, REGULAR}, // ObjetRedCounty
	  	{2351.6140, -657.6815, 128.0547, REGULAR}, // ObjetRedCounty
	  	{2355.0906, -644.0041, 128.2708, REGULAR}, // ObjetRedCounty
	  	//---PONT
	  	{330.0571, -564.3255, 17.4763, REGULAR}, // ObjetPont
  		{323.7649, -565.3095, 17.4763, REGULAR}, // ObjetPont
  		{315.6221, -562.5203, 16.2008, REGULAR}, // ObjetPont
  		{314.0311, -533.5262, 17.4763, REGULAR}, // ObjetPont
  		{318.4087, -534.4589, 17.4763, REGULAR}, // ObjetPont
  		{333.9867, -532.9923, 17.1093, REGULAR}, // ObjetPont
	   	//---
	   	{1553.0864, -33.2442, 21.3626, REGULAR}, // ObjetRedCounty
	   	{1549.4355, 13.5074, 24.1444, REGULAR}, // ObjetRedCounty
	   	{1566.3066, 21.4828, 24.1641, REGULAR}, // ObjetRedCounty
	   	{1569.4172, 32.0269, 24.1993, REGULAR}, // ObjetRedCounty
	   	{1698.6255, -342.2212, 38.1065, REGULAR}, // ObjetRedCounty
	   	{1728.9828, -347.3260, 44.2834, REGULAR}, // ObjetRedCounty
	   	{1750.5734, -335.8134, 46.7105, REGULAR}, // ObjetRedCounty
	   	{1549.5659, -219.2469, 13.2199, REGULAR}, // ObjetRedCounty
	   	{1534.2708, -206.2271, 13.2199, REGULAR}, // ObjetRedCounty
	   	{1394.6996, -208.7506, 6.1156, REGULAR}, // ObjetRedCounty
	   	{1.2940, -219.9657, 5.4297, WAREHOUSE}, // ObjetCamionneurs
	   	{10.1191, -238.4338, 5.4297, WAREHOUSE}, // ObjetCamionneurs
	   	{-7.9689, -241.4053, 5.4297, WAREHOUSE}, // ObjetCamionneurs
	   	{-7.9343, -262.1441, 8.5499, WAREHOUSE}, // ObjetCamionneurs
	   	{-50.6025, -269.3730, 6.6332, WAREHOUSE}, // ObjetCamionneurs
	   	{-156.6121, -268.3611, 6.1887, WAREHOUSE}, // ObjetCamionneurs
	   	//---CABANES CLODOS
		{1027.1810, -52.4612, 28.2090, 25, 45, 10, 10, 5, 5},//Clodos
		{1023.6436, -55.6926, 28.2090, 25, 45, 10, 10, 5, 5},//Clodos
		{1026.2321, -54.0645, 28.2090, 25, 45, 10, 10, 5, 5},//Clodos
		{569.4969, -205.0375, 29.0189, 25, 45, 10, 10, 5, 5},//Clodos
		{571.9524, -204.0010, 29.0189, 25, 45, 10, 10, 5, 5},//Clodos
		{575.9086, -203.8739, 29.0189, 25, 45, 10, 10, 5, 5},//Clodos
		//---ANGEL PINE
		{-2199.6528, -2243.5693, 33.3203, REGULAR}, // ObjetAngelPine
		{-2190.3022, -2256.9800, 33.3203, REGULAR}, // ObjetAngelPine
		{-2188.4558, -2258.4534, 30.6704, REGULAR}, // ObjetAngelPine
		{-2155.5225, -2287.2607, 33.2188, REGULAR}, // ObjetAngelPine
		{-2145.6550, -2286.8406, 36.9227, REGULAR}, // ObjetAngelPine
		{-2094.5955, -2252.8596, 30.6250, REGULAR}, // ObjetAngelPine
		{-2088.1992, -2244.1267, 31.0889, REGULAR}, // ObjetAngelPine
		{-2082.9207, -2239.8909, 31.4636, REGULAR}, // ObjetAngelPine
		{-2069.7178, -2249.8242, 31.7823, REGULAR}, // ObjetAngelPine
		{-2124.9302, -2304.1660, 30.6319, REGULAR}, // ObjetAngelPine
		{-2112.0640, -2313.9194, 30.6250, REGULAR}, // ObjetAngelPine
		{-2092.0969, -2330.1848, 30.6250, REGULAR}, // ObjetAngelPine
		{-2085.7817, -2343.8850, 30.6250, REGULAR}, // ObjetAngelPine
		{-2091.5151, -2344.9038, 30.6250, REGULAR}, // ObjetAngelPine
		{-2101.4050, -2341.8867, 34.8203, REGULAR}, // ObjetAngelPine
		{-2141.1074, -2382.7729, 30.6250, REGULAR}, // ObjetAngelPine
		{-2144.9063, -2339.1560, 30.6250, REGULAR}, // ObjetAngelPine
		{-2156.1504, -2352.5693, 30.7025, REGULAR}, // ObjetAngelPine
		{-2172.9851, -2366.5017, 30.6250, REGULAR}, // ObjetAngelPine
		{-2181.7866, -2368.4023, 32.0922, REGULAR}, // ObjetAngelPine
		{-2226.4768, -2395.1865, 32.5823, REGULAR}, // ObjetAngelPine
		{-2245.4053, -2418.8484, 32.7073, REGULAR}, // ObjetAngelPine
		{-2238.1641, -2423.4482, 32.7073, REGULAR}, // ObjetAngelPine
		{-2242.0984, -2440.9419, 30.9010, REGULAR}, // ObjetAngelPine
		{-2213.2913, -2448.1614, 31.8163, REGULAR}, // ObjetAngelPine
		{-2224.5676, -2481.9656, 31.8163, REGULAR}, // ObjetAngelPine
		{-2227.5364, -2489.3699, 31.8163, REGULAR}, // ObjetAngelPine
		{-2203.2412, -2511.8677, 30.6831, REGULAR}, // ObjetAngelPine
		{-2201.3152, -2517.6282, 31.0445, REGULAR}, // ObjetAngelPine
		{-2177.5798, -2520.4951, 31.8163, REGULAR}, // ObjetAngelPine
		{-2174.2173, -2535.3867, 30.6172, REGULAR}, // ObjetAngelPine
		{-2169.4795, -2549.9236, 31.1172, REGULAR}, // ObjetAngelPine
		{-2154.8528, -2551.2651, 30.6172, REGULAR}, // ObjetAngelPine
		{-2132.2180, -2512.9873, 31.8163, REGULAR}, // ObjetAngelPine
		{-2088.1799, -2510.7009, 31.0668, REGULAR}, // ObjetAngelPine
		{-2070.4617, -2495.1465, 31.0668, REGULAR}, // ObjetAngelPine
		{-2058.5999, -2503.2180, 31.0668, REGULAR}, // ObjetAngelPine
		{-2046.0487, -2523.0325, 31.0668, REGULAR}, // ObjetAngelPine
		{-2030.7146, -2539.5642, 31.0668, REGULAR}, // ObjetAngelPine
		{-2065.2720, -2565.6575, 30.6250, REGULAR}, // ObjetAngelPine
		{-2083.3345, -2424.2319, 30.6250, REGULAR}, // ObjetAngelPine
		{-2091.7131, -2417.2866, 30.6250, REGULAR}, // ObjetAngelPine
		{-2097.9033, -2409.3965, 30.6250, GAS}, // ObjetGarageAngelPine
		{-2100.3501, -2403.1731, 30.6250, GAS}, // ObjetGarageAngelPine
		{-2108.2712, -2397.1355, 31.4219, GAS}, // ObjetGarageAngelPine
		{-2115.6016, -2416.0879, 31.2266, GAS}, // ObjetGarageAngelPine
		{-2120.2620, -2412.5840, 31.2266, GAS}, // ObjetGarageAngelPine
		{-2114.2480, -2420.3423, 30.6250, GAS}, // ObjetGarageAngelPine
		{-2105.9714, -2422.1406, 30.6250, GAS}, // ObjetGarageAngelPine
		{-2163.3699, -2410.3965, 30.6250, REGULAR}, // ObjetAngelPine
		{-2168.5669, -2418.8462, 34.2969, REGULAR}, // ObjetAngelPine
		{-2175.2209, -2419.3706, 34.2969, REGULAR}, // ObjetAngelPine
		{-2177.7175, -2402.0151, 35.2969, REGULAR}, // ObjetAngelPine
		{-2180.0701, -2417.8491, 35.2969, REGULAR}, // ObjetAngelPine
		{-2192.3748, -2425.6047, 35.5234, REGULAR}, // ObjetAngelPine
		{-2182.6716, -2428.9526, 35.5234, REGULAR}, // ObjetAngelPine
		{-2178.1611, -2432.3806, 35.5234, REGULAR}, // ObjetAngelPine
		{-2199.8455, -2424.3501, 35.5234, REGULAR}, // ObjetAngelPine
		{-2197.5974, -2446.5342, 30.6172, REGULAR}, // ObjetAngelPine
		{-2199.8623, -2459.8914, 31.1172, REGULAR}, // ObjetAngelPine
		{-2189.0508, -2419.2131, 30.6250, REGULAR}, // ObjetAngelPine
		{-2183.0938, -2422.0115, 30.6250, REGULAR}, // ObjetAngelPine
		{-2176.7002, -2422.5603, 30.6250, REGULAR}, // ObjetAngelPine
		{-2169.4915, -2319.6980, 30.6250, REGULAR}, // ObjetAngelPine
		{-2114.7410, -2465.4504, 30.6250, GUNSHOP}, // ObjetArmurerieAngelPine
		{-2106.6016, -2472.5356, 30.6250, GUNSHOP}, // ObjetArmurerieAngelPine
		{-2098.3352, -2478.2402, 30.6250, GUNSHOP}, // ObjetArmurerieAngelPine
		{-2111.3667, -2478.2268, 36.1641, GUNSHOP}, // ObjetArmurerieAngelPine
		{-2093.9402, -2469.8826, 33.9242, GUNSHOP}, // ObjetArmurerieAngelPine
		{-2102.6594, -2459.8420, 33.9242, GUNSHOP}, // ObjetArmurerieAngelPine
		{-2104.5581, -2458.8928, 30.6250, GUNSHOP}, // ObjetArmurerieAngelPine
		{-2107.3909, -2457.3215, 30.6250, GUNSHOP}, // ObjetArmurerieAngelPine
		{-2186.3762, -2321.5542, 30.6250, HOSPITAL}, // ObjetHopitalAngelPine
		{-2194.1494, -2331.2983, 30.6250, HOSPITAL}, // ObjetHopitalAngelPine
		{-2204.1392, -2320.2791, 30.6250, HOSPITAL}, // ObjetHopitalAngelPine
		{-2215.2476, -2311.0305, 30.6181, HOSPITAL}, // ObjetHopitalAngelPine
		{-2206.0313, -2301.9106, 30.6250, HOSPITAL}, // ObjetHopitalAngelPine
		{-2209.1694, -2288.2590, 30.6250, HOSPITAL}, // ObjetHopitalAngelPine
		{-2222.7910, -2293.5474, 31.6719, HOSPITAL}, // ObjetHopitalAngelPine
		{-2224.7424, -2279.6260, 30.6250, HOSPITAL}, // ObjetHopitalAngelPine
		{-2234.4197, -2290.9792, 30.6250, HOSPITAL}, // ObjetHopitalAngelPine
		{-2227.4673,-2560.2407,34.8023, GAS}, // StationServiceAngelPine
		{-2233.2478,-2555.6013,34.8047, GAS}, // StationServiceAngelPine
		{-2231.5261,-2556.5430,31.9219, GAS}, // StationServiceAngelPine
		{-2232.8218,-2567.9202,31.9219, GAS}, // StationServiceAngelPine
		//---ANGEL PINE JUNKYARD
		{-1844.1454, -1605.1843, 21.7578, WAREHOUSE}, // ObjetCasse
		{-1851.6046, -1607.5422, 22.7005, WAREHOUSE}, // ObjetCasse
		{-1870.0758, -1605.5776, 21.7641, WAREHOUSE}, // ObjetCasse
		{-1862.9442, -1616.1151, 21.7963, WAREHOUSE}, // ObjetCasse
		{-1900.7930, -1683.8345, 23.0156, WAREHOUSE}, // ObjetCasse
		{-1909.3542, -1675.3057, 23.0156, WAREHOUSE}, // ObjetCasse
		{-1905.0295, -1666.2384, 23.0215, WAREHOUSE}, // ObjetCasse
		{-1827.7772, -1618.7117, 23.0156, WAREHOUSE}, // ObjetCasse
		{-1824.3295, -1611.6844, 23.0156, WAREHOUSE}, // ObjetCasse
		{-1817.9371, -1616.8187, 23.0156, WAREHOUSE}, // ObjetCasse
		{-1811.8594, -1610.5939, 23.0156, WAREHOUSE}, // ObjetCasse
		{-1806.5713, -1627.0186, 23.0215, WAREHOUSE},  // ObjetCasse
		{-1795.0519, -1612.5812, 26.9688, WAREHOUSE}, // ObjetCasse
		{-1865.7762, -1571.1034, 22.6340, WAREHOUSE}, // ObjetCasse
		{-1872.3337, -1555.1522, 22.7109, WAREHOUSE}, // ObjetCasse
		//---CAMP ANGEL PINE
		{-1941.3839,-2450.1860,30.6250, REGULAR}, // ObjetCamp
		{-1960.0238,-2455.1824,30.6250, REGULAR}, // ObjetCamp
		{-1965.1951,-2436.2021,30.6250, REGULAR}, // ObjetCamp
		{-1949.0103,-2471.9644,35.0599, REGULAR}, // ObjetCamp
		{-1965.9258,-2472.7153,30.7409, REGULAR}, // ObjetCamp
		{-1930.5699,-2437.8984,30.6250, REGULAR}, // ObjetCamp
		//---FORÊT
		{-1613.9739, -2636.7683, 52.2617, REGULAR}, // ObjetForet
		{-1362.3646, -2634.8784, 25.2357, REGULAR}, // ObjetForet
		{-989.1736, -2663.3025, 99.8859, REGULAR}, // ObjetForet
		{-991.6511, -2686.7178, 103.3778, REGULAR}, // ObjetForet
		{-993.5012, -2704.5591, 109.2185, REGULAR}, // ObjetForet
		{-695.5170, -2094.4543, 27.7548, REGULAR}, // ObjetForet
		{-697.7073, -2100.7046, 27.6876, REGULAR}, // ObjetForet
		{-701.2786, -2101.0186, 30.1631, REGULAR}, // ObjetForet
		{-697.5609, -2089.6321, 26.6871, REGULAR}, // ObjetForet
		{-604.4744, -1833.8542, 28.5786, REGULAR}, // ObjetForet
		{-418.3456, -1759.4297, 6.2188, REGULAR}, // ObjetForet
		{-495.0006, -1607.8784, 6.0528, REGULAR}, // ObjetForet
		{-491.7025, -1606.6964, 6.0704, REGULAR}, // ObjetForet
		{-473.0848, -1599.3595, 7.3285, REGULAR}, // ObjetForet
		{-476.7958, -1597.3364, 7.6820, REGULAR}, // ObjetForet
		{-475.3693, -1609.2297, 6.5510, REGULAR}, // ObjetForet
		{-363.3742, -1418.0701, 25.7266, REGULAR}, // ObjetForet
		{-369.6037, -1417.9611, 25.7266, REGULAR}, // ObjetForet
		{-365.2435, -1418.1697, 29.6406, REGULAR}, // ObjetForet
		{-382.6080, -1438.7227, 26.0073, REGULAR}, // ObjetForet
		{-389.9145, -1419.2900, 25.7266, REGULAR}, // ObjetForet
		{-393.0121, -1434.3237, 25.7266, REGULAR}, // ObjetForet
		{-398.9983, -1434.2286, 25.7266, REGULAR}, // ObjetForet
		{-367.7293, -1434.3925, 25.7266, REGULAR}, // ObjetForet
		{-575.3536, -1484.2992, 14.3438, REGULAR}, // ObjetForet
		{-1024.8300, -1181.2604, 129.2188, REGULAR}, // ObjetFerme
		{-1069.5032, -1202.7898, 129.2188, REGULAR}, // ObjetFerme
		{-1425.1492, -967.2806, 200.8261, REGULAR}, // ObjetForet
		{-1435.1022, -963.0560, 201.0104, REGULAR}, // ObjetForet
		{-1434.0642,-1543.2517,101.7578, REGULAR}, // ForetFerme2
		{-1457.1879,-1534.1796,101.7513, REGULAR}, // ForetFerme2
		{-1434.9976,-1583.0304,101.7578, REGULAR}, // ForetFerme2
		{-1433.8326,-1570.7961,101.7578, REGULAR}, // ForetFerme2
		{-1436.4517,-1575.8875,105.1802, REGULAR}, // ForetFerme2
		{-1461.2341,-1577.9349,105.2201, REGULAR}, // ForetFerme2
		{-1417.5398,-1546.9026,101.7578, REGULAR}, // ForetFerme2
		{-1417.3110,-1503.4614,101.6719, REGULAR}, // ForetFerme2
		{-1423.9000,-1479.3041,105.0321, REGULAR}, // ForetFerme2
		//---GARAGE PRÈS DE LOS SANTOS
		{-76.2371, -1103.0714, 1.0781, WAREHOUSE}, // ObjetGarageForet
		{-59.3033, -1109.4951, 1.0781, WAREHOUSE}, // ObjetGarageForet
		{-61.4086, -1112.9481, 1.0781, WAREHOUSE}, // ObjetGarageForet
		{-23.6363, -1122.2645, 1.0781, WAREHOUSE}, // ObjetGarageForet
		{-32.1813, -1118.0189, 1.0781, WAREHOUSE}, // ObjetGarageForet
		{-71.9752, -1182.5483, 1.7500, WAREHOUSE}, // ObjetGarageForet
		{-82.7820, -1203.6345, 2.8906, WAREHOUSE}, // ObjetGarageForet
		{-88.1637, -1212.6885, 2.8906, WAREHOUSE}, // ObjetGarageForet
		{-73.5314, -1168.1694, 5.6797, WAREHOUSE}, // ObjetGarageForet
		{-78.8597, -1181.8813, 5.6797, WAREHOUSE}, // ObjetGarageForet
		//---CABANES DANS LES ARBRES
		{-1378.8239,-2209.2612,58.4199, REGULAR}, // ObjetCabanes
		{-1364.9487,-2230.1936,58.4850, REGULAR}, // ObjetCabanes
		{-1375.3494,-2207.2356,62.5289, 35, 65, 0, 0, 0, 0}, // ObjetToitCabanes
		{-1373.1661,-2202.0884,55.3579, REGULAR}, // ObjetCabanes
		{-1633.5303,-2233.3779,31.4766, REGULAR},// ObjetCabaneForet
		{-1629.6858,-2246.1018,31.4766, REGULAR}, // ObjetCabaneForet
		{-1632.9589,-2250.3828,31.2970, REGULAR}, // ObjetCabaneForet
		//---LAS VENTURAS
		{1972.2074,694.3849,10.8203, REGULAR},//Normal
		{2062.0674,636.7342,11.4683, REGULAR},//Normal
		{2021.7035,644.5203,15.2890, REGULAR},//Normal
		{2079.9338,634.5045,11.0034, REGULAR},//Normal
		{2122.3506,660.4516,11.0499, REGULAR},//Normal
		{2114.7258,679.3694,10.8203, REGULAR},//Normal
		{2095.1379,710.5388,11.4609, REGULAR},//Normal
		{2125.5886,719.7866,11.4609, REGULAR},//Normal
		{2121.0051,734.9615,11.4609, REGULAR},//Normal
		{2133.2937,776.0845,11.4453, REGULAR},//Normal
		{2089.0981,786.5349,11.4531, REGULAR},//Normal
		{2070.4016,789.5549,11.4605, REGULAR},//Normal
		{2169.2354,778.8595,11.4609, REGULAR},//Normal
		{2182.2954,749.4156,11.4609, REGULAR},//Normal
		{2209.8984,727.5742,11.1693, REGULAR},//Normal
		{2250.0017,677.2896,11.4531, REGULAR},//Normal
		{2346.5400,678.5157,11.4609, REGULAR},//Normal
		{2376.2878,676.6806,11.4605, REGULAR},//Normal
		{2391.5913,668.4734,11.4609, REGULAR},//Normal
		{2455.9751,706.5283,11.4683, REGULAR},//Normal
		{2525.6975,656.4965,14.0628, REGULAR},//Normal
		{2575.4792,715.1243,14.7396, REGULAR},//Normal
		{2638.1492,732.4238,10.8203, REGULAR},//Normal
		{2736.0730,719.8900,17.0075, REGULAR},//Normal
		{2681.8667,834.1719,8.8203, REGULAR},//Normal
		{2654.3838,825.0786,8.8203, WAREHOUSE},
		{2572.6965,823.8530,5.3158, WAREHOUSE},
		{2579.4805,790.8731,5.3158, WAREHOUSE},
		{2627.9192,791.2639,5.3158, WAREHOUSE},
		{2651.5659,820.3716,8.0584, WAREHOUSE},
		{2703.6663,899.6219,10.3199, WAREHOUSE},//Chantier
		{2853.3193,945.2214,10.7500, REGULAR},
		{2891.4006,919.5600,10.8984, REGULAR},
		{2824.2483,1015.3845,10.7500, REGULAR},//Normal
		{2650.1340,1085.7067,10.8203, GAS},
		{2654.7642,1139.1429,10.8203, GAS},
		{2637.2908,1142.0033,11.1797, GAS},
		{2637.8191,1065.1160,10.8203, GAS},//Station essence
		{2567.5845,1043.0477,10.8203, REGULAR},
		{2593.3259,994.4522,10.8203, REGULAR},
		{2534.9458,997.4740,14.2725, REGULAR},
		{2347.5820,954.9826,16.4282, REGULAR},
		{2327.4675,949.4468,17.3643, REGULAR},
		{2343.7039,949.8699,10.8203, REGULAR},
		{2313.7595,953.8221,15.8853, REGULAR},
		{2320.8032,950.3414,17.3643, REGULAR},//Normal
		{2124.7195,884.9540,11.1797, GAS},
		{2103.4390,894.6339,11.1797, GAS},//Station essence
		{2161.7683,923.7853,11.0995, GUNSHOP},
		{2209.7078,938.7621,10.8203, GUNSHOP},
		{2200.8506,949.6416,15.6720, GUNSHOP},
		{2164.1047,959.0605,15.6720, GUNSHOP},
		{2194.3044,986.0734,10.8203, GUNSHOP},
		{2138.5464,1013.5311,10.9766, GUNSHOP},//Ammunation
		{2022.4508,1015.6192,10.8203, 35, 10, 10, 15, 20, 10},
		{2011.7073,990.0655,10.8203, 35, 10, 10, 15, 20, 10},
		{1935.0577,951.4676,10.8203, 35, 10, 10, 15, 20, 10},
		{1920.8484,945.0206,10.8127, 35, 10, 10, 15, 20, 10},
		{1891.0879,973.2265,10.8203, 35, 10, 10, 15, 20, 10},
		{1879.4196,998.2629,10.8203, 35, 10, 10, 15, 20, 10},
		{1894.8157,1159.0797,10.8203, 35, 10, 10, 15, 20, 10},//Casino
		{1957.6398,1342.4287,15.3746, REGULAR},
		{2097.7769,1286.4532,10.8203, REGULAR},
		{2108.7949,1237.3673,10.8203, REGULAR},
		{2206.5173,1291.4010,10.8203, REGULAR},
		{2302.1475,1285.9629,67.4688, REGULAR},
		{2318.5068,1359.4082,10.8203, REGULAR},//Normal
		{2341.5242,1387.2324,10.8203, WAREHOUSE},
		{2315.8818,1451.0017,23.6313, WAREHOUSE},
		{2348.2336,1490.7302,30.8197, WAREHOUSE},
		{2350.6094,1446.8643,42.8156, WAREHOUSE},
		{2326.0295,1388.2961,42.8203, WAREHOUSE},
		{2270.9973,1393.9762,42.8203, WAREHOUSE},//Parking
		{2235.4150,1483.2827,11.0547, REGULAR},//Normal
		{2150.9878,1461.3523,10.8203, 35, 10, 10, 15, 20, 10},
		{2101.6350,1446.7766,10.8203, 35, 10, 10, 15, 20, 10},
		{2163.7810,1448.5168,10.8203, 35, 10, 10, 15, 20, 10},
		{2166.9319,1516.3307,10.8125, 35, 10, 10, 15, 20, 10},
		{2137.6465,1500.1580,10.8203, 35, 10, 10, 15, 20, 10},
		{2147.1980,1501.3127,10.8203, 35, 10, 10, 15, 20, 10},//Casino
		{2007.0072,1545.0419,13.1684, 35, 10, 10, 15, 20, 10},
		{1999.4889,1529.0980,14.6223, 35, 10, 10, 15, 20, 10},//Bateau Pirate
		{2287.8862,1560.7000,10.8203, REGULAR},
		{2308.5718,1642.4612,11.0469, REGULAR},
		{2128.6816,1636.3949,11.0469, REGULAR},
		{2145.2969,1677.4875,10.8203, REGULAR},
		{2154.7625,1729.3370,11.0469, REGULAR},//Normal
		{1998.9209,1691.8452,14.5587, 50, 10, 10, 10, 10, 10},
		{2015.7670,1738.7748,12.7437, 50, 10, 10, 10, 10, 10},
		{1876.7126,1770.2579,10.2086, 50, 10, 10, 10, 10, 10},
		{1815.7568,2072.8994,4.0089, 50, 10, 10, 10, 10, 10},
		{1874.3158,2072.0024,11.0625, 50, 10, 10, 10, 10, 10},
		{1889.5532,2084.1511,11.0625, 50, 10, 10, 10, 10, 10},
		{1880.0392,2108.8096,11.0625, 50, 10, 10, 10, 10, 10},
		{1856.9110,2100.5264,10.8203, 50, 10, 10, 10, 10, 10},//Burger shot
		{1742.8550,2028.2217,10.8203, REGULAR},
		{1672.7292,1986.1300,10.8203, REGULAR},
		{1636.6172,1955.9175,10.8203, REGULAR},
		{1620.7710,1918.4777,10.8203, REGULAR},
		{1508.6877,1919.4996,10.8203, REGULAR},
		{1466.5289,1896.5723,11.4609, REGULAR},
		{1449.6405,1937.4935,11.4609, REGULAR},
		{1427.1691,1903.6467,10.8203, REGULAR},//Normal
		{1373.3556,1796.7583,10.8203, WAREHOUSE},
		{1319.4047,1657.2161,10.8203, WAREHOUSE},
		{1302.3669,1659.7031,10.8203, WAREHOUSE},
		{1275.1713,1587.3669,10.8203, WAREHOUSE},
		{1286.8140,1496.5416,10.8203, WAREHOUSE},
		{1329.3652,1425.8096,10.8666, WAREHOUSE},
		{1280.4033,1357.8418,10.8203, WAREHOUSE},
		{1306.1842,1261.9235,14.2645, WAREHOUSE},
		{1315.4438,1255.6925,14.2731, WAREHOUSE},
		{1329.1383,1251.6188,14.2656, WAREHOUSE},
		{1352.8417,1251.3064,10.8203, WAREHOUSE},
		{1429.1860,1237.4178,10.8203, WAREHOUSE},
		{1503.0806,1277.1945,10.8201, WAREHOUSE},
		{1556.9005,1244.9857,10.8125, WAREHOUSE},
		{1561.1060,1326.4535,14.0855, WAREHOUSE},
		{1592.3754,1373.0222,21.5843, WAREHOUSE},
		{1643.0153,1398.6365,21.4122, WAREHOUSE},
		{1637.0272,1449.2461,31.0794, WAREHOUSE},
		{1645.2937,1440.4185,23.8281, WAREHOUSE},
		{1622.3097,1485.9409,23.7976, WAREHOUSE},
		{1607.0276,1517.2421,21.5542, WAREHOUSE},
		{1602.2075,1628.7433,10.8203, WAREHOUSE},
		{1707.2372,1591.2296,10.3003, WAREHOUSE},
		{1680.0466,1744.4452,10.8246, WAREHOUSE},
		{1599.7816,1819.3767,10.8280, WAREHOUSE},//Aéroport
		{1512.3861,1888.7189,11.0234, REGULAR},
		{1254.1245,1824.5190,10.1781, REGULAR},
		{1066.0220,1871.1895,14.9297, REGULAR},
		{1071.3337,1981.7515,11.4688, REGULAR},
		{1043.5245,1983.3248,11.4688, REGULAR},
		{1048.5631,2015.6801,10.8203, REGULAR},
		{1063.3932,1995.1436,10.8203, REGULAR},
		{1066.2834,1982.2424,10.8203, REGULAR},
		{1050.8004,2036.9683,10.8203, REGULAR},
		{1029.7797,2100.4919,10.8203, REGULAR},
		{1078.7820,2268.7434,10.8126, REGULAR},
		{1030.7476,2232.1548,10.8203, REGULAR},
		{985.0674,2280.3833,11.0504, REGULAR},
		{963.8887,2280.7441,10.9976, REGULAR},
		{918.2481,2313.5100,10.8203, REGULAR},
		{943.8978,2306.1409,10.8203, REGULAR},
		{942.0090,2307.8113,10.8203, REGULAR},
		{943.7194,2308.0627,10.8203, REGULAR},
		{918.0564,2328.8127,17.6713, REGULAR},
		{917.9384,2331.9561,17.6713, REGULAR},
		{941.8076,2348.2415,17.0396, REGULAR},
		{942.7357,2335.1060,17.6863, REGULAR},
		{941.7012,2329.1677,17.6863, REGULAR},
		{941.5505,2380.5701,17.6393, REGULAR},
		{946.1670,2381.6992,17.6393, REGULAR},
		{952.8484,2378.6204,17.2309, REGULAR},
		{1047.0770,2416.9465,10.8203, REGULAR},
		{1079.5028,2420.0710,17.0819, REGULAR},
		{1071.6829,2419.3113,17.5093, REGULAR},
		{1071.4391,2416.8708,17.5093, REGULAR},
		{987.8521,2633.8823,10.8203, REGULAR},
		{1225.9301,2792.8613,10.8203, REGULAR},
		{1351.9259,2791.0925,10.8203, REGULAR},
		{1455.9166,2817.8040,10.8247, REGULAR},
		{1493.2764,2781.8589,10.8203, REGULAR},
		{1564.4768,2776.0852,10.8203, REGULAR},
		{1557.6895,2767.6694,10.8274, REGULAR},
		{1585.5242,2747.0522,10.8203, REGULAR},
		{1579.7029,2769.2163,10.8203, REGULAR},
		{1581.0474,2785.9011,10.8203, REGULAR},
		{1566.9406,2833.7415,10.8203, REGULAR},
		{1604.2366,2853.6692,10.8203, REGULAR},
		{1612.7528,2863.0256,10.8203, REGULAR},
		{1662.8954,2863.3247,10.8203, REGULAR},
		{1664.4633,2837.1016,10.8203, REGULAR},
		{1663.4650,2812.0593,10.8203, REGULAR},
		{1741.4755,2842.6943,10.8359, REGULAR},
		{1837.0533,2859.1565,10.8359, REGULAR},
		{1870.9147,2853.5505,10.8359, REGULAR},
		{1876.4027,2837.9463,10.8359, REGULAR},
		{1773.8040,2806.8430,8.3359, REGULAR},
		{1762.0835,2831.4448,8.3359, REGULAR},
		{1777.4849,2797.2092,10.8359, REGULAR},
		{1739.2793,2696.0427,10.8203, REGULAR},
		{1924.4359,2707.6958,10.8203, REGULAR},
		{1936.7302,2728.1665,10.8203, REGULAR},
		{1967.5698,2760.1836,10.8203, REGULAR},
		{1982.9294,2733.2739,10.8203, REGULAR},
		{1986.2673,2726.9778,10.8203, REGULAR},
		{2019.9850,2719.4202,10.8203, REGULAR},
		{2036.5157,2705.6702,10.8203, REGULAR},
		{2053.2278,2707.4158,10.8273, REGULAR},
		{2078.0833,2724.0247,10.8203, REGULAR},//Normal
		{2115.6541,2744.5222,10.8203, GAS},
		{2143.2900,2720.9868,11.1763, GAS},
		{2146.3691,2709.6760,10.8203, GAS},
		{2170.7544,2738.0271,11.1722, GAS},
		{2157.1326,2742.3274,10.8203, GAS},
		{2177.8687,2748.1716,10.8203, GAS},//Station essence
		{2179.7285,2792.5588,10.8203, 50, 10, 10, 10, 10, 10},
		{2184.3262,2797.9285,10.8203, 50, 10, 10, 10, 10, 10},
		{2158.4895,2807.5054,10.8203, 50, 10, 10, 10, 10, 10},
		{2140.8953,2808.6941,10.8203, 50, 10, 10, 10, 10, 10},//Burger Shot
		{2261.9583,2786.0229,10.8203, REGULAR},
		{2314.1021,2775.7227,10.8203, REGULAR},
		{2372.7798,2758.9014,10.8203, REGULAR},//Normal
		{2507.3816,2782.7703,10.8203, WAREHOUSE},
		{2508.3704,2843.0889,10.8203, WAREHOUSE},
		{2564.8687,2830.0745,10.8203, WAREHOUSE},
		{2605.1924,2811.1599,10.8203, WAREHOUSE},
		{2626.8977,2841.0212,10.8203, WAREHOUSE},
		{2594.5918,2799.7874,10.8203, WAREHOUSE},
		{2647.4658,2776.3743,19.3222, WAREHOUSE},
		{2651.1904,2742.9541,19.3222, WAREHOUSE},
		{2643.8967,2748.3359,23.8222, WAREHOUSE},
		{2610.6030,2761.9653,23.8222, WAREHOUSE},
		{2602.7100,2739.9363,23.8222, WAREHOUSE},
		{2602.1575,2712.3723,25.8222, WAREHOUSE},
		{2618.1812,2720.6733,36.5386, WAREHOUSE},//Centrale
		{2793.3054,2549.2712,22.4417, REGULAR},
		{2818.9395,2413.9155,21.9110, REGULAR},
		{2861.9521,2395.5776,10.8203, REGULAR},
		{2819.1182,2268.3745,14.6615, REGULAR},
		{2917.6357,2122.3335,17.8955, REGULAR},
		{2836.6709,2067.5459,10.8203, REGULAR},
		{2781.6179,2015.5962,10.8041, REGULAR},
		{2656.8374,1979.3092,14.1161, REGULAR},
		{2623.0198,1980.1409,14.1161, REGULAR},
		{2629.1548,1967.9598,14.1161, REGULAR},//Normal
		{2552.5613,2054.2234,10.8203, GUNSHOP},
		{2534.4915,2089.3032,10.8203, GUNSHOP},
		{2559.6465,2103.3408,10.8203, GUNSHOP},
		{2563.1526,2118.8210,10.8203, GUNSHOP},
		{2579.2820,2121.4639,10.8203, GUNSHOP},
		{2600.2063,2087.9932,10.8196, GUNSHOP},//Ammunation
		{2610.0779,2148.5752,14.1161, REGULAR},
		{2571.6931,2313.0515,17.8222, REGULAR},
		{2598.5837,2394.7925,17.8203, REGULAR},
		{2625.7363,2406.9587,17.8203, REGULAR},
		{2628.3606,2417.6106,10.8132, REGULAR},
		{2591.4194,2429.9875,10.8203, REGULAR},
		{2564.6753,2439.9749,10.8203, REGULAR},//Normal
		{2375.8066,2405.3806,10.8203, POLICE},
		{2316.4780,2454.2444,3.2734, POLICE},
		{2262.4531,2467.9602,3.5313, POLICE},
		{2297.3477,2479.9695,-7.4531, POLICE},
		{2232.2700,2449.5195,-7.4531, POLICE},
		{2230.2065,2462.0110,-7.4531, POLICE},
		{2222.5664,2455.9070,-7.4531, POLICE},
		{2227.7776,2453.7529,-7.4531, POLICE},
		{2251.9900,2440.4795,-7.4531, POLICE},
		{2301.1477,2480.1660,-7.4531, POLICE},
		{2278.1345,2488.3464,-7.4531, POLICE},
		{2302.3296,2494.1824,3.2734, POLICE},
		{2293.3374,2428.5342,10.8203, POLICE},
		{2340.1035,2452.5378,14.9742, POLICE},
		{2307.0176,2456.7329,10.8203, POLICE},
		{2238.5010,2449.3511,11.0372, POLICE},
		{2240.7393,2438.0701,10.8203, POLICE},
		{2250.4387,2476.8040,10.8203, POLICE},
		{2250.9697,2489.7710,10.9908, POLICE},
		{2251.5823,2487.3018,10.9908, POLICE},//Commissariat
		{2203.8945,2479.8716,10.8203, GAS},
		{2196.4424,2497.6868,10.8203, GAS},
		{2166.7822,2482.0361,10.8203, GAS},
		{2174.2551,2468.1667,10.8203, GAS},
		{2181.4353,2436.8193,11.2422, GAS},//Station essence
		{2115.5024,2494.4102,12.6839, REGULAR},
		{2094.0247,2504.4460,11.0781, REGULAR},
		{2041.9258,2516.1626,10.8203, REGULAR},
		{2082.2688,2461.9033,10.8203, REGULAR},
		{2118.2832,2416.0010,10.8203, REGULAR},
		{2034.3517,2425.7466,10.8203, REGULAR},
		{2016.0027,2348.5974,10.8203, REGULAR},
		{2071.8508,2364.9919,23.6016, REGULAR},
		{2085.1997,2353.1348,23.6036, REGULAR},
		{2101.9998,2382.0386,36.6172, REGULAR},
		{2100.6714,2420.0303,74.5786, REGULAR},
		{2184.5645,2415.2507,73.0339, REGULAR},
		{2187.1064,2418.0046,73.0339, REGULAR},
		{2190.0596,2420.8770,73.0308, REGULAR},
		{2141.6794,2432.1917,58.3984, REGULAR},
		{1883.3009,2339.2917,10.9799, REGULAR},
		{1961.8931,2324.0454,16.4359, REGULAR},
		{1945.6721,2254.5825,23.9141, REGULAR},
		{1974.4045,2224.6880,20.7338, REGULAR},
		{1950.7787,2234.5325,16.8082, REGULAR},
		{1908.8611,2190.3535,11.1250, REGULAR},
		{1603.9481,1951.7489,13.8722, REGULAR},
		{1299.7681,1673.8827,10.8203, REGULAR},
		{1144.6921,1353.1265,10.8203, REGULAR},
		{1124.6451,1278.5293,13.2614, REGULAR},
		{1154.4331,1216.0070,13.0049, REGULAR},
		{1170.8634,1226.2780,10.9820, REGULAR},
		{1148.8766,1117.0780,11.0023, REGULAR},
		{1114.2631,1065.7526,10.1956, REGULAR},
		{1062.1997,1094.6694,10.2492, REGULAR},
		{1312.1326,784.5438,13.9872, REGULAR},
		{1418.0500,753.3057,10.8203, REGULAR},
		{1473.1185,727.9525,10.8203, REGULAR},
		{1495.7081,685.7977,11.0810, REGULAR},
		{1542.6244,675.9132,11.3281, REGULAR},
		{1693.1522,690.0936,10.8203, REGULAR},
		{1745.9180,734.0931,10.8203, REGULAR},
		{1777.7247,687.8562,19.1375, REGULAR},
		{1785.3229,667.9405,18.2580, REGULAR},
		{1830.7911,682.6728,11.3516, REGULAR},
		{2001.2340,796.6743,10.8203, REGULAR},
		{2363.6243,1005.8816,14.2725, REGULAR},
		{2014.6451,1099.2018,10.8203, REGULAR},
		{2041.9304,1141.1827,11.2893, REGULAR},//Normal
		{2070.5623,1143.8839,10.6797, 10, 5, 20, 10, 35, 20},
		{2071.9617,1138.9182,10.6797, 10, 5, 20, 10, 35, 20},
		{2075.5103,1137.2361,10.6719, 10, 5, 20, 10, 35, 20},
		{2083.1614,1141.2809,10.8203, 10, 5, 20, 10, 35, 20},
		{2074.8352,1159.5221,10.6719, 10, 5, 20, 10, 35, 20},
		{2043.1919,1172.9253,10.6719, 10, 5, 20, 10, 35, 20},
		{2035.9113,1193.9902,10.8203, 10, 5, 20, 10, 35, 20},
		{2025.0476,1190.1217,10.8203, 10, 5, 20, 10, 35, 20},
		{2026.2513,1178.8159,10.8203, 10, 5, 20, 10, 35, 20},
		{2070.3308,1178.3755,10.6719, 10, 5, 20, 10, 35, 20},
		{2074.2480,1168.5265,10.6719, 10, 5, 20, 10, 35, 20},
		{1066.8481,1386.0922,10.8203, 10, 5, 20, 10, 35, 20},
		{1041.3876,1378.5229,10.8203, 10, 5, 20, 10, 35, 20},
		{1001.9599,1380.5284,10.8203, 10, 5, 20, 10, 35, 20},
		{987.4562,1385.0582,10.8203, 10, 5, 20, 10, 35, 20},
		{978.8893,1391.5443,10.8203, 10, 5, 20, 10, 35, 20},
		{978.0898,1385.6334,11.5629, 10, 5, 20, 10, 35, 20},
		{975.2040,1401.9259,12.3549, 10, 5, 20, 10, 35, 20},
		{974.4484,1411.1052,16.2345, 10, 5, 20, 10, 35, 20},
		{973.8245,1418.5139,20.0068, 10, 5, 20, 10, 35, 20},
		{981.5580,1407.6005,16.6651, 10, 5, 20, 10, 35, 20},
		{988.4540,1414.1238,13.4355, 10, 5, 20, 10, 35, 20},
		{998.9561,1406.5494,10.8203, 10, 5, 20, 10, 35, 20},
		{1002.5286,1409.1099,10.7957, 10, 5, 20, 10, 35, 20},
		{998.1772,1416.4155,11.7922, 10, 5, 20, 10, 35, 20},
		{992.2153,1418.4857,12.6401, 10, 5, 20, 10, 35, 20},//Crash avion
		{982.2971,1678.6749,8.6484, REGULAR},
		{947.7034,1721.3711,8.8516, REGULAR},
		{971.1179,1798.1406,8.8516, REGULAR},
		{936.3366,1920.4119,11.4683, REGULAR},
		{928.7631,1926.5950,11.4683, REGULAR},
		{944.6931,1988.8645,20.1596, REGULAR},
		{920.1514,1968.2286,18.8842, REGULAR},
		{919.7203,1965.0618,18.8842, REGULAR},
		{924.8937,1984.8778,26.5466, REGULAR},
		{936.6412,2031.7554,15.7364, REGULAR},
		{888.8105,2037.7458,11.4609, REGULAR},
		{923.6470,2137.4697,10.8203, REGULAR},
		{916.0222,2245.3318,10.8203, REGULAR},
		{913.6352,2238.3218,10.8203, REGULAR},
		{912.9944,2232.3684,10.8203, REGULAR},
		{943.2108,2348.6484,17.0627, REGULAR},
		{960.5273,2350.3804,16.3308, REGULAR},
		{1038.7534,2331.8604,11.2639, REGULAR},
		{1065.4130,2270.4734,10.8126, REGULAR},
		{1068.0692,2185.3879,16.7188, REGULAR},
		{1086.0066,2226.1692,16.7188, REGULAR},
		{1106.2565,2274.5413,16.7188, REGULAR},
		{1136.8000,2272.8909,16.7188, REGULAR},
		{1131.0325,2317.6921,16.7188, REGULAR},
		{1155.7424,2338.4893,16.7188, REGULAR},
		{1314.4739,2205.1973,16.3747, REGULAR},
		{1310.6262,2213.9453,12.0156, REGULAR},
		{1330.5631,2212.7170,12.0156, REGULAR},
		{1333.7258,2210.3894,12.0156, REGULAR},
		{1349.7299,2211.8020,12.0156, REGULAR},
		{1387.1935,2209.2864,12.0156, REGULAR},
		{1406.0547,2201.3279,12.0220, REGULAR},
		{1411.9047,2166.9409,12.0156, REGULAR},
		{1411.0583,2134.8132,12.0156, REGULAR},
		{1409.0354,2111.7329,12.0156, REGULAR},
		{1413.0604,2109.8049,12.0156, REGULAR},
		{1409.9840,2104.5811,12.0156, REGULAR},
		{1385.7854,2120.2510,11.0156, REGULAR},
		{1412.8796,2101.8525,19.3988, REGULAR},
		{1349.1465,1936.6963,11.2419, REGULAR},
		{1446.9669,1143.2729,10.6719, REGULAR},
		{1443.7598,1139.7634,10.6797, REGULAR},
		{1440.3434,1138.1116,10.8125, REGULAR},
		{1476.7942,1089.1221,10.8203, REGULAR},
		{1470.6394,1048.6650,10.8203, REGULAR},
		{1523.9161,1027.2184,10.8203, REGULAR},
		{1534.6062,1041.3544,10.8203, REGULAR},
		{1583.6385,1012.9201,10.6901, REGULAR},
		{1617.4763,998.6772,10.9569, REGULAR},
		{1654.3335,1009.1603,19.3785, REGULAR},
		{1703.5514,1055.1769,12.9698, REGULAR},
		{1729.6227,1070.9729,10.8203, REGULAR},
		{1763.1563,1009.7293,9.6882, REGULAR},
		{1709.0576,930.7248,10.8203, REGULAR},
		{1675.1722,933.6035,10.8203, REGULAR},
		{1652.6962,903.8641,11.1807, REGULAR},
		{1352.2675,948.8336,10.8203, REGULAR},
		{1352.3956,945.4682,10.8203, REGULAR},
		{1348.4789,944.4485,10.8203, REGULAR},
		{1329.5094,942.5240,10.8203, REGULAR},
		{1208.3663,882.4062,10.0283, REGULAR},
		{1192.2080,882.7371,8.5716, REGULAR},
		{1100.4547,1178.3171,10.8203, REGULAR},
		{1022.7574,1473.3550,5.8203, REGULAR},
		{1039.9232,1511.9061,5.8203, REGULAR},//Normal
		//---DÉSERT
		{-309.1475, 1303.6125, 53.6643, WAREHOUSE}, // BigEar
		{-319.5756, 1296.0289, 53.6643, WAREHOUSE}, // BigEar
		{-334.7552, 1292.4567, 53.6643, WAREHOUSE}, // BigEar
		{-337.7951, 1303.6012, 53.6643, WAREHOUSE}, // BigEar
		{-329.9216, 1536.4144, 76.6117, WAREHOUSE}, // BigEar
		{-327.6586, 1539.5457, 79.9165, WAREHOUSE}, // BigEar
		{-311.4577, 1548.9049, 80.2486, WAREHOUSE}, // BigEar
		{-339.5427, 1550.9624, 75.5625, WAREHOUSE}, // BigEar
		{-373.6612, 1514.3107, 75.5625, WAREHOUSE}, // BigEar
		{-384.0996, 1510.5040, 75.5625, WAREHOUSE}, // BigEar
		{-367.7305, 1510.6801, 76.3117, WAREHOUSE}, // BigEar
		{-456.4868, 1582.8898, 36.4471, WAREHOUSE}, // BigEar
		{-461.3746, 1573.3735, 37.1340, WAREHOUSE}, // BigEar
		{-453.4255, 1566.0165, 36.5318, WAREHOUSE}, // BigEar
		{-467.7207, 1593.1260, 37.5349, WAREHOUSE}, // BigEar
		{-415.3446, 1666.0389, 36.7238, WAREHOUSE}, // BigEar
		{-319.8695, 1746.7008, 42.8403, WAREHOUSE}, // BigEar
		{-309.4350, 1762.4028, 43.6406, WAREHOUSE}, // BigEar
		{-316.0093, 1776.5371, 43.6406, WAREHOUSE}, // BigEar
		{-317.5060, 1792.1294, 43.6331, WAREHOUSE}, // BigEar
		{-312.4660, 1800.0645, 43.6331, WAREHOUSE}, // BigEar
		{-328.9021, 1860.6611, 44.3828, WAREHOUSE}, // BigEar
		{541.8725, 2359.2703, 31.0206, WAREHOUSE}, // VerdantMeadows
		{528.9741, 2374.3044, 30.2238, WAREHOUSE}, // VerdantMeadows
		{412.6794, 2442.2634, 16.5000,  WAREHOUSE}, // VerdantMeadows
		{414.6604, 2532.8499, 16.5771,  WAREHOUSE}, // VerdantMeadows
		{415.2271, 2531.4500, 19.1661,  WAREHOUSE}, // VerdantMeadows
		{414.0647, 2537.0996, 19.1484,  WAREHOUSE}, // VerdantMeadows
		{423.2230, 2540.1536, 16.4987,  WAREHOUSE}, // VerdantMeadows
		{392.5553, 2601.2898, 16.4844,  WAREHOUSE}, // VerdantMeadows
		{375.0222, 2610.4868, 16.4844,  WAREHOUSE}, // VerdantMeadows
		{378.9411, 2599.0879, 16.4918,  WAREHOUSE}, // VerdantMeadows
		{330.9803, 2591.6038, 17.4626,  WAREHOUSE}, // VerdantMeadows
		{304.6823, 2617.4412, 17.4926,  WAREHOUSE}, // VerdantMeadows
		{301.5529, 2606.6182, 16.6802,  WAREHOUSE}, // VerdantMeadows
		{282.7591, 2590.7715, 16.4766,  WAREHOUSE}, // VerdantMeadows
		{282.0332, 2547.7744, 16.8183,  WAREHOUSE}, // VerdantMeadows
		{298.5369, 2533.8425, 16.8208,  WAREHOUSE}, // VerdantMeadows
		{319.9856, 2541.3191, 16.8103,  WAREHOUSE}, // VerdantMeadows
		{274.9581, 2474.0042, 16.4773,  WAREHOUSE}, // VerdantMeadows
		{290.7344, 2442.6841, 16.7636,  WAREHOUSE}, // VerdantMeadows
		{286.7020, 2410.4121, 16.4844,  WAREHOUSE}, // VerdantMeadows
		{255.4176, 2409.9111, 16.6815,  WAREHOUSE}, // VerdantMeadows
		{185.3574, 2438.1677, 17.2866,  WAREHOUSE}, // VerdantMeadows
		{148.2342, 2422.7700, 16.5753,  WAREHOUSE}, // VerdantMeadows
		{133.7050, 2439.0801, 16.4766,  WAREHOUSE}, // VerdantMeadows
		{117.4371, 2415.0740, 16.4999,  WAREHOUSE}, // VerdantMeadows
		{82.8517, 2457.4185, 16.4844,  WAREHOUSE}, // VerdantMeadows
		{-26.9108, 2357.6323, 24.1406,  WAREHOUSE}, // VerdantMeadows
		{-14.7927, 2338.4590, 24.1406,  WAREHOUSE}, // VerdantMeadows
		{-30.9976, 2324.2324, 24.1406,  WAREHOUSE}, // VerdantMeadows
		{-41.1274, 2347.8513, 24.1406,  WAREHOUSE}, // VerdantMeadows
		{-268.8038, 2587.7703, 63.5703, REGULAR}, // LasPayasadas
		{-276.7029, 2591.3696, 63.5703, REGULAR}, // LasPayasadas
		{-270.5283, 2607.9150, 62.8582, REGULAR}, // LasPayasadas
		{-264.5630, 2648.9351, 62.7033, REGULAR}, // LasPayasadas
		{-268.7745, 2655.7502, 62.6675, REGULAR}, // LasPayasadas
		{-277.7676, 2666.6426, 62.6012, REGULAR}, // LasPayasadas
		{-317.3647, 2659.7737, 63.8692, REGULAR}, // LasPayasadas
		{-322.6337, 2675.7471, 63.6797, REGULAR}, // LasPayasadas
		{-289.5678, 2677.4736, 65.8523, REGULAR}, // LasPayasadas
		{-265.9760, 2676.9985, 62.6875, REGULAR}, // LasPayasadas
		{-276.0164, 2717.8132, 62.6440, REGULAR}, // LasPayasadas
		{-281.4280, 2735.6633, 62.2856, REGULAR}, // LasPayasadas
		{-308.7715, 2727.2349, 62.6238, REGULAR}, // LasPayasadas
		{-286.4613, 2759.6311, 62.5121, REGULAR}, // LasPayasadas
		{-270.7507, 2769.6907, 61.8475, REGULAR}, // LasPayasadas
		{-258.5212, 2781.8831, 62.6875, REGULAR}, // LasPayasadas
		{-233.5242, 2806.8220, 62.0547, REGULAR}, // LasPayasadas
		{-222.9109, 2771.0664, 62.6875, REGULAR}, // LasPayasadas
		{-202.2478, 2766.3982, 62.2797, REGULAR}, // LasPayasadas
		{-224.2272, 2722.8179, 66.8745, REGULAR}, // LasPayasadas
		{-235.2324, 2726.0950, 66.6512, REGULAR}, // LasPayasadas
		{-227.7619, 2713.1086, 67.0891, REGULAR}, // LasPayasadas
		{-169.3082, 2707.8337, 62.5569, REGULAR}, // LasPayasadas
		{-167.0254, 2685.3853, 62.6249, REGULAR}, // LasPayasadas
		{-550.5782, 2599.0466, 53.9348, REGULAR}, // LasPayasadas
		{-549.1424, 2589.4705, 53.9348, REGULAR}, // LasPayasadas
		{-536.5092, 2596.2366, 53.4141, REGULAR}, // LasPayasadas
		{-583.4611, 2713.0347, 71.8288, REGULAR}, // LasPayasadas
		{-605.0323, 2716.8271, 72.7231, REGULAR}, // LasPayasadas
		{-621.1180, 2704.3625, 72.3750, REGULAR}, // LasPayasadas
		{-625.8583, 2709.3457, 72.3750, REGULAR}, // LasPayasadas
		{-637.5710, 2717.9983, 72.3750, REGULAR}, // LasPayasadas
		{-673.6263, 2706.6816, 70.6633, REGULAR}, // LasPayasadas
		{492.6411, 781.8281, -22.0622,  WAREHOUSE},//Carrière
		{568.9903, 823.5586, -22.1271,  WAREHOUSE},//Carrière
		{581.2667, 822.3315, -29.7989,  WAREHOUSE},//Carrière
		{585.8417, 868.0842, -42.4973,  WAREHOUSE},//Carrière
		{588.1601, 876.7419, -42.4973,  WAREHOUSE},//Carrière
		{601.8853, 868.0441, -42.9609,  WAREHOUSE},//Carrière
		{601.9465, 829.3449, -43.2765,  WAREHOUSE},//Carrière
		{637.7677, 829.9135, -40.4601,  WAREHOUSE},//Carrière
		{627.3240, 895.4099, -41.1028,  WAREHOUSE},//Carrière
		{618.4882, 890.9799, -37.1285,  WAREHOUSE},//Carrière
		{624.8803, 896.7753, -36.1361,  WAREHOUSE},//Carrière
		{712.7154, 909.2493, -18.6629,  WAREHOUSE},//Carrière
		{817.0092, 856.7293, 12.7890,  WAREHOUSE},//Carrière
		{824.1050, 867.0886, 12.1500,  WAREHOUSE},//Carrière
		{446.0502, 914.4442, -8.1410,  WAREHOUSE},//Carrière
		{423.3152, 897.1588, 2.4610,  WAREHOUSE},//Carrière
		{321.7414, 852.3714, 20.4062,  WAREHOUSE},//Carrière
		{325.2281, 861.2148, 20.4062,  WAREHOUSE},//Carrière
		{315.9750, 873.5573, 23.3549,  WAREHOUSE},//Carrière
		{301.0915, 1141.1086, 9.1374, FARM},//Ferme
		{312.2305, 1147.2611, 8.5859, FARM},//Ferme
		{398.0843, 1157.5893, 8.3480, FARM},//Ferme
		{395.2273, 1158.8057, 10.8968, FARM},//Ferme
		{406.4316, 1166.3292, 7.9101, FARM},//Ferme
		{418.5958, 1166.6201, 7.8849, FARM},//Ferme
		{510.7555, 1116.3438, 14.9415, FARM},//Ferme
		{501.3349, 1116.3029, 15.0355, FARM},//Ferme
		{513.6413, 1104.9299, 15.3138, FARM},//Ferme
		{574.0816, 1221.3127, 11.7112, FARM},//Ferme
		{585.3944, 1247.3902, 11.7187, FARM},//Ferme
		{609.7152, 1242.5561, 11.7187, FARM},//Ferme
		{631.4169, 1226.6105, 11.7112, FARM},//Ferme
		{640.9316, 1231.8199, 11.7112, FARM},//Ferme
		{706.7785, 1207.6363, 13.3964, FARM},//Ferme
		{709.9417, 1207.4667, 13.8480, FARM},//Ferme
		{714.1043, 1209.7751, 16.4388, FARM},//Ferme
		{710.8600, 1198.3540, 13.3964, FARM},//Ferme
		{709.8817, 1189.3049, 13.3906, FARM},//Ferme
		{563.9874, 1369.2633, 16.4878,  WAREHOUSE},//Raffinerie
		{577.1531, 1427.0996, 12.3312,  WAREHOUSE},//Raffinerie
		{533.9656, 1473.5344, 5.6047,  WAREHOUSE},//Raffinerie
		{542.9565, 1556.4729, 1.0000,  WAREHOUSE},//Raffinerie
		{436.1008, 1567.6029, 12.7843,  WAREHOUSE},//Raffinerie
		{290.0480, 1418.1920, 10.3262,  WAREHOUSE},//Raffinerie
		{246.6061, 1435.0234, 23.3702,  WAREHOUSE},//Raffinerie
		{246.8502, 1411.0479, 10.7075,  WAREHOUSE},//Raffinerie
		{241.8237, 1402.6250, 10.7075,  WAREHOUSE},//Raffinerie
		{247.3356, 1363.3308, 10.7075,  WAREHOUSE},//Raffinerie
		{247.8911, 1361.1956, 23.3702,  WAREHOUSE},//Raffinerie
		{204.4497, 1371.6894, 10.5859,  WAREHOUSE},//Raffinerie
		{193.3130, 1373.8278, 19.3281,  WAREHOUSE},//Raffinerie
		{152.5981, 1389.4429, 10.5859,  WAREHOUSE},//Raffinerie
		{160.7350, 1439.8306, 10.5912,  WAREHOUSE},//Raffinerie
		{203.6889, 1462.4475, 10.5859,  WAREHOUSE},//Raffinerie
		{62.0065, 1224.6611, 18.8544, GAS},//FortCarsonEssence
		{59.1917, 1214.8641, 18.8466, GAS},//FortCarsonEssence
		{51.1439, 1210.7083, 18.9104, GAS},//FortCarsonEssence
		{55.7599, 1194.3233, 18.7179, REGULAR},//FortCarson
		{68.5184, 1161.9626, 18.6640, REGULAR},//FortCarson
		{90.3931, 1157.3684, 18.6565, REGULAR},//FortCarson
		{111.5574, 1112.4333, 13.6093, REGULAR},//FortCarson
		{45.3470, 1162.1939, 18.6640, REGULAR},//FortCarson
		{25.1343, 1182.1201, 19.2576, REGULAR},//FortCarson
		{12.2916, 1181.8864, 19.4292, REGULAR},//FortCarson
		{12.9242, 1210.6026, 19.3403, REGULAR},//FortCarson
		{-17.5868, 1232.1795, 18.1328, REGULAR},//FortCarson
		{-26.6405, 1214.7050, 19.3522, REGULAR},//FortCarson
		{-69.2624, 1212.2305, 19.7421, REGULAR},//FortCarson
		{-77.2195, 1233.0413, 19.7421, REGULAR},//FortCarson
		{-92.8250, 1189.1135, 19.7421, REGULAR},//FortCarson
		{-55.6478, 1187.8398, 22.4843, REGULAR},//FortCarson
		{-41.2797, 1184.9350, 24.0859, REGULAR},//FortCarson
		{-44.3847, 1177.9467, 19.4236, REGULAR},//FortCarson
		{-68.4844, 1227.7023, 22.4402, REGULAR},//FortCarson
		{-14.8359, 1175.4366, 19.5633, REGULAR},//FortCarson
		{-11.3699, 1160.8500, 19.5095, REGULAR},//FortCarson
		{-24.7347, 1165.1510, 21.0142, REGULAR},//FortCarson
		{11.5222, 1112.8925, 20.9398, REGULAR},//FortCarson
		{14.9316, 1126.2910, 20.2421, REGULAR},//FortCarson
		{5.9418, 1083.8211, 19.7421, REGULAR},//FortCarson
		{5.8620, 1069.7025, 20.2421, REGULAR},//FortCarson
		{-47.0866, 1077.4447, 20.2421, REGULAR},//FortCarson
		{-42.6799, 1082.2985, 20.9398, REGULAR},//FortCarson
		{-38.6436, 1059.6247, 19.7421, REGULAR},//FortCarson
		{-39.0040, 1047.9263, 20.3397, REGULAR},//FortCarson
		{-33.1607, 1037.9049, 20.9398, REGULAR},//FortCarson
		{-11.8941, 975.5703, 19.8120, REGULAR},//FortCarson
		{-4.3348, 952.2449, 19.7031, REGULAR},//FortCarson
		{19.9468, 949.7181, 20.3168, REGULAR},//FortCarson
		{23.1291, 968.7778, 19.8104, REGULAR},//FortCarson
		{64.9120, 980.5238, 15.3223, REGULAR},//FortCarson
		{64.6931, 998.6292, 14.1729, REGULAR},//FortCarson
		{20.8896, 950.2539, 20.3168, REGULAR},//FortCarson
		{26.0792, 922.8106, 23.6382, REGULAR},//FortCarson
		{19.1461, 902.1484, 24.0526, REGULAR},//FortCarson
		{-52.7994, 894.7033, 22.3871, REGULAR},//FortCarson
		{-83.5675, 932.5227, 20.7043, REGULAR},//FortCarson
		{-81.3466, 915.2630, 21.3892, REGULAR},//FortCarson
		{-116.9151, 912.4946, 20.1221, REGULAR},//FortCarson
		{-116.8954, 894.1744, 19.6921, REGULAR},//FortCarson
		{-123.1989, 876.0374, 18.7308, REGULAR},//FortCarson
		{-125.8361, 857.9509, 18.3450, REGULAR},//FortCarson
		{-154.7173, 879.3729, 21.0066, REGULAR},//FortCarson
		{-153.2069, 905.4624, 19.3011, REGULAR},//FortCarson
		{-150.0321, 940.9961, 19.1275, REGULAR},//FortCarson
		{-146.0461, 981.2863, 19.5868, REGULAR},//FortCarson
		{-138.8860, 1019.4676, 19.7421, REGULAR},//FortCarson
		{-146.5116, 1035.8985, 21.2973, REGULAR},//FortCarson
		{-147.6490, 1044.8930, 20.0129, REGULAR},//FortCarson
		{-169.1282, 1030.3568, 19.7344, REGULAR},//FortCarson
		{-169.9445, 1027.3084, 21.2173, REGULAR},//FortCarson
		{-175.2141, 1038.1871, 24.0390, REGULAR},//FortCarson
		{-178.2335, 1027.7791, 24.0390, REGULAR},//FortCarson
		{-210.2795, 1075.7070, 19.7421, REGULAR},//FortCarson
		{-219.8995, 1073.4677, 19.7421, REGULAR},//FortCarson
		{-218.3503, 1057.3006, 22.6473, REGULAR},//FortCarson
		{-207.9925, 1064.5131, 23.9062, REGULAR},//FortCarson
		{-207.0673, 1043.8558, 23.9042, REGULAR},//FortCarson
		{-210.4437, 1030.7917, 24.5059, REGULAR},//FortCarson
		{-227.5636, 981.5775, 19.5589, POLICE},//FortCarsonSheriff
		{-229.4387, 977.7637, 22.2833, POLICE},//FortCarsonSheriff
		{-222.9208, 973.8927, 22.2833, POLICE},//FortCarsonSheriff
		{-216.9585, 979.1170, 22.9432, POLICE},//FortCarsonSheriff
		{-218.6505, 984.5505, 19.5405, POLICE},//FortCarsonSheriff
		{-242.9603, 995.7656, 19.7421, REGULAR},//FortCarson
		{-248.7037, 1002.0056, 20.9398, REGULAR},//FortCarson
		{-254.3713, 996.9136, 20.2421, REGULAR},//FortCarson
		{-253.7752, 993.9902, 20.9398, REGULAR},//FortCarson
		{-271.1842, 993.0317, 20.3509, REGULAR},//FortCarson
		{-280.5732, 989.5406, 20.2421, REGULAR},//FortCarson
		{-259.9070, 1045.9497, 20.9398, REGULAR},//FortCarson
		{-247.7846, 1042.4121, 20.2421, REGULAR},//FortCarson
		{-241.8360, 1046.2723, 20.1736, REGULAR},//FortCarson
		{-251.2560, 1050.2298, 20.9398, REGULAR},//FortCarson
		{-257.4310, 1083.6562, 20.9398, REGULAR},//FortCarson
		{-260.6439, 1079.2470, 20.2421, REGULAR},//FortCarson
		{-260.4693, 1076.0924, 20.9398, REGULAR},//FortCarson
		{-291.6092, 1119.7883, 20.2383, REGULAR},//FortCarson
		{-294.6596, 1112.3547, 20.0022, REGULAR},//FortCarson
		{-329.6553, 1120.6843, 20.9398, REGULAR},//FortCarson
		{-362.0080, 1111.7891, 20.9398, REGULAR},//FortCarson
		{-373.4222, 1107.7481, 19.8787, REGULAR},//FortCarson
		{-378.3786, 1101.8791, 19.7484, REGULAR},//FortCarson
		{-372.3693, 1134.5003, 20.3509, REGULAR},//FortCarson
		{-357.5414, 1138.8416, 19.9248, REGULAR},//FortCarson
		{-366.5227, 1170.7503, 20.2718, REGULAR},//FortCarson
		{-365.0546, 1195.5717, 19.6226, REGULAR},//FortCarson
		{-368.4605, 1183.7126, 19.7421, REGULAR},//FortCarson
		{-411.1702, 1172.2434, 7.2323, REGULAR},//FortCarson
		{-420.4436, 1163.6724, 1.9208, REGULAR},//FortCarson
		{-322.2197, 1173.0208, 20.2421, REGULAR},//FortCarson
		{-300.2800, 1169.7241, 20.3503, REGULAR},//FortCarson
		{-259.6943, 1171.5870, 20.9398, REGULAR},//FortCarson
		{-259.1245, 1150.4180, 20.9398, REGULAR},//FortCarson
		{-260.8663, 1121.7652, 20.9398, REGULAR},//FortCarson
		{-256.4348, 1114.4332, 19.8848, REGULAR},//FortCarson
		{-220.1561, 1114.0872, 19.7421, REGULAR},//FortCarson
		{-210.5158, 1110.9787, 19.7421, REGULAR},//FortCarson
		{-207.0377, 1119.0240, 20.4296, REGULAR},//FortCarson
		{-217.0503, 1128.5520, 19.7421, REGULAR},//FortCarson
		{-218.8806, 1135.6822, 19.7421, REGULAR},//FortCarson
		{-205.4804, 1137.8632, 19.7421, REGULAR},//FortCarson
		{-204.8530, 1144.9984, 19.7421, REGULAR},//FortCarson
		{-208.2575, 1158.8293, 19.7421, REGULAR},//FortCarson
		{-218.2101, 1154.2092, 19.7421, REGULAR},//FortCarson
		{-218.3540, 1164.4276, 21.3111, REGULAR},//FortCarson
		{-211.4614, 1159.7835, 19.7421, REGULAR},//FortCarson
		{-178.7354, 1178.3575, 22.9412, REGULAR},//FortCarson
		{-166.9944, 1177.3665, 22.9412, REGULAR},//FortCarson
		{-163.0184, 1185.7763, 26.0113, REGULAR},//FortCarson
		{-178.1529, 1183.7021, 26.7613, REGULAR},//FortCarson
		{-153.8309, 1170.3828, 24.2402, REGULAR},//FortCarson
		{-144.4314, 1170.2872, 25.1953, REGULAR},//FortCarson
		{-136.3658, 1161.8391, 19.7499, REGULAR},//FortCarson
		{-116.4484, 1134.6198, 19.7421, REGULAR},//FortCarson
		{-123.0063, 1134.3713, 19.7421, REGULAR},//FortCarson
		{-136.3618, 1117.6823, 20.1966, REGULAR},//FortCarson
		{-158.6650, 1122.9815, 19.7421, REGULAR},//FortCarson
		{-134.4812, 1078.2503, 19.7917, REGULAR},//FortCarson
		{-126.3543, 1085.2583, 19.7917, REGULAR},//FortCarson
		{-126.7198, 1080.4888, 19.7917, REGULAR},//FortCarson
		{-130.2112, 1079.1809, 19.7917, REGULAR},//FortCarson
		{-181.4936, 1112.0321, 19.7421, REGULAR},//FortCarson
		{-170.9174, 1122.8209, 19.7499, REGULAR},//FortCarson
		{-88.5560, 1340.9663, 11.1058, REGULAR},//FortCarson
		{-96.6965, 1363.7800, 12.3986, REGULAR},//FortCarson
		{-101.5050, 1366.4144, 11.0280, REGULAR},//FortCarson
		{-101.6308, 1372.4322, 10.2734, REGULAR},//FortCarson
		{-103.0759, 1375.6809, 10.2734, REGULAR},//FortCarson
		{-98.8872, 1378.9681, 10.2734, REGULAR},//FortCarson
		{-88.8432, 1383.5190, 10.2734, REGULAR},//FortCarson
		{-308.0209, 1039.4193, 19.7211, HOSPITAL},//FortCarsonHopital
		{-309.6781, 1027.5673, 19.7195, HOSPITAL},//FortCarsonHopital
		{-320.7113, 1027.6799, 25.0124, HOSPITAL},//FortCarsonHopital
		{-332.1443, 1048.7755, 26.0124, HOSPITAL},//FortCarsonHopital
		{-320.1390, 1048.9047, 25.0134, HOSPITAL},//FortCarsonHopital
		{-306.5492, 1048.0523, 26.0124, HOSPITAL},//FortCarsonHopital
		{-306.5437, 1042.2441, 26.0124, HOSPITAL},//FortCarsonHopital
		{-325.9200, 1050.0600, 20.3402, HOSPITAL},//FortCarsonHopital
		{-318.7687, 1048.6347, 20.3402, HOSPITAL},//FortCarsonHopital
		{-313.2012, 1052.6754, 20.3402, HOSPITAL},//FortCarsonHopital
		{-335.6841, 1056.2375, 19.7391, HOSPITAL},//FortCarsonHopital
		{-336.7876, 1050.3149, 19.7391, HOSPITAL},//FortCarsonHopital
		{-319.5339, 842.9440, 14.2421, GUNSHOP},//FortCarsonArmurerie
		{-314.7272, 832.4470, 14.2421, GUNSHOP},//FortCarsonArmurerie
		{-318.8441, 822.4089, 14.2421, GUNSHOP},//FortCarsonArmurerie
		{-323.5129, 818.0562, 14.4243, GUNSHOP},//FortCarsonArmurerie
		{-329.7133, 817.8332, 14.4446, GUNSHOP},//FortCarsonArmurerie
		{-329.6335, 831.3215, 14.2421, GUNSHOP},//FortCarsonArmurerie
		{-326.4515, 835.5543, 17.5454, GUNSHOP},//FortCarsonArmurerie
		{-444.0708, 2232.7053, 42.4296, REGULAR},//LasBrujas
		{-430.0440, 2240.5644, 42.9833, REGULAR},//LasBrujas
		{-404.4076, 2258.6108, 42.9104, REGULAR},//LasBrujas
		{-395.9821, 2250.8674, 42.4296, REGULAR},//LasBrujas
		{-377.2710, 2242.7309, 42.6184, REGULAR},//LasBrujas
		{-351.6464, 2226.4055, 42.4912, REGULAR},//LasBrujas
		{-332.2733, 2221.6530, 42.4888, REGULAR},//LasBrujas
		{115.8784, 1935.6108, 18.9464, AREA},//Area69
		{120.6171, 1938.3422, 19.1599, AREA},//Area69
		{190.3513, 1937.4388, 17.6480, AREA},//Area69
		{210.2580, 1924.9168, 17.6480, AREA},//Area69
		{267.4859, 1895.1528, 25.4985, AREA},//Area69
		{279.1755, 1834.2431, 17.6480, AREA},//Area69
		{172.0699, 1828.4898, 17.6430, AREA},//Area69
		{171.8380, 1840.7401, 17.6430, AREA},//Area69
		{156.2858, 1838.8780, 17.6480, AREA},//Area69
		{154.6886, 1830.9573, 17.6480, AREA},//Area69
		{169.4983, 1828.0595, 17.6406, AREA},//Area69
		{212.1997, 1811.1359, 21.8671, AREA},//Area69
		{215.5986, 1814.0371, 21.8671, AREA},//Area69
		{139.3772, 1838.8248, 17.6459, AREA},//Area69
		{140.5935, 1835.9422, 18.4404, AREA},//Area69
		{139.7297, 1841.4278, 17.6459, AREA},//Area69
		{144.1919, 1846.9232, 17.6459, AREA},//Area69
		{133.0590, 1870.2528, 17.8359, AREA},//Area69
		{135.3977, 1870.8773, 17.8359, AREA},//Area69
		{132.8701, 1879.6813, 17.8359, AREA},//Area69
		{112.7365, 1878.7752, 18.8974, AREA},//Area69
		{113.1620, 1871.4362, 17.8419, AREA},//Area69
		{149.7113, 1895.9244, 18.5951, AREA},//Area69
		{148.6223, 1895.1850, 18.6933, AREA},//Area69
		{130.4036, 1907.4725, 19.5861, AREA},//Area69
		{146.2465, 1904.7967, 19.4717, AREA},//Area69
		{142.4325, 1906.9913, 19.4717, AREA},//Area69
		{142.3853, 1901.1923, 19.4717, AREA},//Area69
		{138.6771, 1901.3985, 19.4717, AREA},//Area69
		{138.7556, 1903.2753, 19.4717, AREA},//Area69
		{149.3538, 1905.0070, 19.4717, AREA},//Area69
		{149.5995, 1903.0672, 19.4717, AREA},//Area69
		{267.7273, 1946.8233, 17.6406, AREA},//Area69
		{285.4876, 1969.1762, 17.6406, AREA},//Area69
		{282.0507, 1979.1422, 17.6406, AREA},//Area69
		{274.0690, 1982.9321, 17.6406, AREA},//Area69
		{268.5812, 2020.5821, 17.6406, AREA},//Area69
		{267.2249, 2026.4831, 17.6406, AREA},//Area69
		{202.7450, 1873.4246, 13.1406, AREA},//Area69
		{202.2968, 1862.1707, 13.1406, AREA},//Area69
		{248.6133, 1858.9865, 14.0840, AREA},//Area69
		{245.3815, 1859.7133, 14.0840, AREA},//Area69
		{255.0608, 1877.5209, 8.7578, AREA},//Area69
		{249.1933, 1856.2100, 8.7578, AREA},//Area69
		{254.7887, 1843.0689, 8.7712, AREA},//Area69
		{255.4608, 1802.1416, 7.4765, AREA},//Area69
		{243.7637, 1797.7097, 7.4140, AREA},//Area69
		{236.3702, 1831.2933, 7.4140, AREA},//Area69
		{211.6416, 1824.3516, 7.3634, AREA},//Area69
		{215.3295, 1816.5302, 6.4216, AREA},//Area69
		{225.3712, 1822.7391, 6.4140, AREA},//Area69
		{294.3076, 1820.0836, 4.7109, AREA},//Area69
		{287.6763, 1822.8587, 7.7265, AREA},//Area69
		{275.6496, 1841.0781, 9.3472, AREA},//Area69
		{331.6044, 1838.2172, 7.8281, AREA},//Area69
		{316.6742, 1853.6619, 7.7340, AREA},//Area69
		{275.3648, 1873.7210, 9.6463, AREA},//Area69
		{280.0250, 1873.8240, 9.8020, AREA},//Area69
		{275.2801, 1859.6463, 9.8133, AREA},//Area69
		{268.5913, 1858.3592, 9.8133, AREA},//Area69
		{268.8154, 1876.7348, -3.4698, AREA},//Area69
		{268.7742, 1876.7200, -22.2429, AREA},//Area69
		{263.2079, 1882.3012, -29.3429, AREA},//Area69
		{270.9729, 1878.2901, -29.5020, AREA},//Area69
		{206.8404, 1869.7024, 13.1406, AREA}, //Area
		{207.2662, 1864.3768, 13.1406, AREA}, //Area
		{210.4937, 1866.8066, 13.1406, AREA}, //Area
		{213.7407, 1862.3118, 13.1406, AREA}, //Area
		{222.0135, 1868.8850, 13.1406, AREA}, //Area
		{223.7654, 1870.1119, 13.1406, AREA}, //Area
		{210.7637, 1872.7058, 13.1406, AREA}, //Area
		{224.9577, 1870.4441, 13.7421, AREA}, //Area
		{242.3855, 1864.5680, 11.4609, AREA}, //Area
		{241.1771, 1866.2792, 11.4609, AREA}, //Area
		{241.1001, 1869.0641, 11.4609, AREA}, //Area
		{239.8036, 1859.2615, 14.0840, AREA}, //Area
		{242.0494, 1861.1919, 14.0840, AREA}, //Area
		{246.7378, 1860.8457, 14.0840, AREA}, //Area
		{248.0443, 1877.0286, 8.7578, AREA}, //Area
		{252.6468, 1874.6697, 8.7578, AREA}, //Area
		{259.1788, 1870.6609, 8.7578, AREA}, //Area
		{261.1785, 1868.1245, 8.7578, AREA}, //Area
		{257.0204, 1866.9436, 8.7578, AREA}, //Area
		{251.5016, 1860.8473, 8.7578, AREA}, //Area
		{247.3722, 1862.4406, 8.7578, AREA}, //Area
		{240.0937, 1858.1293, 8.7578, AREA}, //Area
		{240.5700, 1854.5139, 8.7578, AREA}, //Area
		{242.9165, 1853.3259, 8.7578, AREA}, //Area
		{245.7014, 1830.4393, 7.5547, AREA}, //Area
		{245.4900, 1818.4932, 7.5547, AREA}, //Area
		{249.0934, 1820.6078, 7.5547, AREA}, //Area
		{254.8119, 1803.1311, 7.4457, AREA}, //Area
		{254.0304, 1800.9524, 7.4213, AREA}, //Area
		{250.8148, 1802.0475, 7.4141, AREA}, //Area
		{251.5615, 1799.1006, 7.4141, AREA}, //Area
		{246.9296, 1802.6698, 7.4141, AREA}, //Area
		{240.7684, 1800.8943, 7.4141, AREA}, //Area
		{224.1671, 1821.1836, 6.4141, AREA}, //Area
		{223.2049, 1824.6373, 6.4141, AREA}, //Area
		{216.7686, 1826.9954, 6.4141, AREA}, //Area
		{216.4462, 1824.4607, 6.4141, AREA}, //Area
		{211.6050, 1820.7880, 7.3675, AREA}, //Area
		{213.7365, 1817.3771, 7.3359, AREA}, //Area
		{254.0302, 1831.5504, 4.7109, AREA}, //Area
		{251.4064, 1834.5532, 4.7109, AREA}, //Area
		{277.2439, 1839.7611, 7.8281, AREA}, //Area
		{279.5271, 1839.8536, 7.8281, AREA}, //Area
		{298.4492, 1848.8003, 7.7341, AREA}, //Area
		{296.9081, 1845.6102, 7.7266, AREA}, //Area
		{299.3096, 1859.5948, 7.8732, AREA}, //Area
		{298.6147, 1868.1008, 8.7656, AREA}, //Area
		{273.5821, 1868.5591, 8.7578, AREA}, //Area
		{275.7740, 1867.4287, 8.7578, AREA}, //Area
		{271.6829, 1865.7596, 8.7578, AREA}, //Area
		{280.2507, 1855.9229, 9.6464, AREA}, //Area
		{266.2888, 1855.2108, 8.7578, AREA}, //Area
		{266.5822, 1857.8546, 8.7578, AREA}, //Area
		{269.0384, 1854.9066, 8.7578, AREA}, //Area
		{270.7835, 1862.2465, 8.7649, AREA}, //Area
		{272.9914, 1886.2073, -30.3906, AREA}, //Area
		{271.2316, 1885.1722, -30.3906, AREA}, //Area
		{269.0033, 1882.6765, -30.0938, AREA}, //Area
		{265.9060, 1886.5022, -30.3906, AREA}, //Area
		{274.0191, 1878.8396, -30.3906, AREA}, //Area
		{266.4872, 1878.7087, -29.5021, AREA}, //Area
		{273.0305, 1880.8094, -30.3906, AREA}, //Area
		{270.1304, 1881.8691, -30.3906, AREA}, //Area
		{268.6596, 1878.8124, -30.3906, AREA}, //Area
		{619.8048, 1689.2127, 7.1875, GAS},//BoneCountyEssence
		{638.0023, 1679.5902, 7.1875, GAS},//BoneCountyEssence
		{645.0057, 1679.3532, 6.9921, GAS},//BoneCountyEssence
		{643.8220, 1683.7659, 9.8669, GAS},//BoneCountyEssence
		{657.2615, 1713.6691, 7.1875, GAS},//BoneCountyEssence
		{680.9860, 1723.3867, 6.9921, GAS},//BoneCountyEssence
		{679.9374, 1711.4058, 10.7081, GAS},//BoneCountyEssence
		{662.1840, 1711.4595, 10.9149, GAS},//BoneCountyEssence
		{780.7241, 1864.7154, 4.8200, GUNSHOP},//BoneCountyAmmu
		{775.5406, 1866.4990, 8.0956, GUNSHOP},//BoneCountyAmmu
		{772.1277, 1869.1247, 8.0956, GUNSHOP},//BoneCountyAmmu
		{768.2035, 1880.9002, 8.0956, GUNSHOP},//BoneCountyAmmu
		{763.6722, 1873.8002, 5.1099, GUNSHOP},//BoneCountyAmmu
		{763.8316, 1883.6263, 5.1171, GUNSHOP},//BoneCountyAmmu
		{770.5341, 1897.3972, 5.0716, GUNSHOP},//BoneCountyAmmu
		{772.1144, 1937.4438, 5.4995, REGULAR},//BoneCounty
		{784.9902, 1946.2602, 5.3593, REGULAR},//BoneCounty
		{784.2089, 1953.6712, 5.7074, REGULAR},//BoneCounty
		{774.0001, 1960.9414, 5.3359, REGULAR},//BoneCounty
		{755.7833, 1964.7558, 5.3359, REGULAR},//BoneCounty
		{757.9491, 1972.4588, 5.3431, REGULAR},//BoneCounty
		{762.6772, 1991.3912, 5.3534, REGULAR},//BoneCounty
		{768.5443, 2035.4781, 6.7109, REGULAR},//BoneCounty
		{752.9096, 2043.4422, 6.7109, REGULAR},//BoneCounty
		{763.3881, 2060.5788, 6.7109, REGULAR},//BoneCounty
		{776.5632, 2078.0908, 6.7109, REGULAR},//BoneCounty
		{769.3117, 2086.6684, 6.7109, REGULAR},//BoneCounty
		{742.7504, 2073.8029, 9.9125, REGULAR},//BoneCounty
		{710.6812, 1984.9873, 3.4729, REGULAR},//BoneCounty
		{719.5125, 1975.9415, 4.9296, REGULAR},//BoneCounty
		{711.9322, 1967.1204, 5.5313, REGULAR},//BoneCounty
		{696.3057, 1963.1260, 5.5390, REGULAR},//BoneCounty
		{680.2208, 1976.2381, 4.9375, REGULAR},//BoneCounty
		{-762.8981, 2751.5417, 45.7734, REGULAR},//NordDesertMotel
		{-756.7412, 2765.2351, 48.2555, REGULAR},//NordDesertMotel
		{-772.2578, 2765.2800, 48.2555, REGULAR},//NordDesertMotel
		{-775.2137, 2750.3947, 46.5106, REGULAR},//NordDesertMotel
		{-788.8283, 2751.6298, 45.8545, REGULAR},//NordDesertMotel
		{-789.0404, 2767.1149, 45.8545, REGULAR},//NordDesertMotel
		{-798.3288, 2762.2758, 45.7345, REGULAR},//NordDesertMotel
		{-815.5948, 2765.7827, 46.0000, REGULAR},//NordDesertMotel
		{-824.8771, 2768.0097, 46.0000, REGULAR},//NordDesertMotel
		{-838.7164, 2766.2070, 46.0000, REGULAR},//NordDesertMotel
		{-848.6480, 2765.6774, 46.0000, REGULAR},//NordDesertMotel
		{-860.8127, 2760.2133, 45.8515, REGULAR},//NordDesertMotel
		{-866.3022, 2749.9157, 46.0000, REGULAR},//NordDesertMotel
		{-877.1536, 2743.0185, 45.9954, REGULAR},//NordDesertMotel
		{-844.6740, 2746.1232, 46.1409, REGULAR},//NordDesertMotel
		{-815.6535, 2747.6877, 45.9924, REGULAR},//NordDesertMotel
		{-1273.0799, 2711.2431, 50.2662, GAS},//ElQuebradosStationEssence
		{-1279.9335, 2708.8356, 50.2662, GAS},//ElQuebradosStationEssence
		{-1283.8132, 2715.1330, 50.2662, GAS},//ElQuebradosStationEssence
		{-1274.3027, 2722.7336, 50.2662, GAS},//ElQuebradosStationEssence
		{-1270.4337, 2725.4458, 52.6167, GAS},//ElQuebradosStationEssence
		{-1262.0343, 2709.8151, 50.0625, GAS},//ElQuebradosStationEssence
		{-1317.8577, 2699.3374, 50.2662, GAS},//ElQuebradosStationEssence
		{-1311.1101, 2699.8742, 50.0625, GAS},//ElQuebradosStationEssence
		{-1390.1794, 2637.6474, 55.9843, POLICE},//ElQuebradosCommissariat
		{-1385.8074, 2626.4047, 55.7201, POLICE},//ElQuebradosCommissariat
		{-1379.5333, 2643.6472, 58.6569, POLICE},//ElQuebradosCommissariat
		{-1376.3256, 2646.9316, 55.2463, POLICE},//ElQuebradosCommissariat
		{-1386.7116, 2655.6806, 55.9769, POLICE},//ElQuebradosCommissariat
		{-1389.2769, 2649.2324, 55.9843, POLICE},//ElQuebradosCommissariat
		{-1385.1165, 2650.0854, 60.5081, POLICE},//ElQuebradosCommissariat
		{-1441.7241, 2625.8588, 55.8359, REGULAR},//ElQuebrados
		{-1451.6375, 2631.6171, 55.8359, REGULAR},//ElQuebrados
		{-1447.8670, 2636.3378, 56.2543, REGULAR},//ElQuebrados
		{-1456.3314, 2639.3378, 55.8359, REGULAR},//ElQuebrados
		{-1465.3156, 2653.2575, 55.8359, REGULAR},//ElQuebrados
		{-1457.5596, 2657.3571, 55.8359, REGULAR},//ElQuebrados
		{-1466.7052, 2660.9392, 55.8359, REGULAR},//ElQuebrados
		{-1474.7064, 2645.8937, 55.8359, REGULAR},//ElQuebrados
		{-1470.8804, 2624.7753, 55.8359, REGULAR},//ElQuebrados
		{-1474.9689, 2626.4501, 55.8359, REGULAR},//ElQuebrados
		{-1474.4102, 2621.7778, 58.7812, REGULAR},//ElQuebrados
		{-1470.7038, 2612.6259, 58.7879, REGULAR},//ElQuebrados
		{-1470.8239, 2618.1987, 58.7879, REGULAR},//ElQuebrados
		{-1470.5551, 2628.6652, 58.7734, REGULAR},//ElQuebrados
		{-1464.3316, 2628.7973, 58.7734, REGULAR},//ElQuebrados
		{-1459.5655, 2629.7033, 58.7734, REGULAR},//ElQuebrados
		{-1460.1767, 2623.0400, 58.7734, REGULAR},//ElQuebrados
		{-1456.8470, 2612.2443, 61.2810, REGULAR},//ElQuebrados
		{-1449.1013, 2612.4265, 61.2718, REGULAR},//ElQuebrados
		{-1444.6230, 2618.8056, 60.9505, REGULAR},//ElQuebrados
		{-1437.5937, 2615.9179, 61.0960, REGULAR},//ElQuebrados
		{-1484.1402, 2613.5537, 58.7879, REGULAR},//ElQuebrados
		{-1479.0498, 2619.0031, 58.7812, REGULAR},//ElQuebrados
		{-1484.4758, 2640.8471, 58.7812, REGULAR},//ElQuebrados
		{-1453.7684, 2690.2119, 55.8359, REGULAR},//ElQuebrados
		{-1465.5429, 2685.3610, 55.8359, REGULAR},//ElQuebrados
		{-1466.8007, 2695.8569, 55.8359, REGULAR},//ElQuebrados
		{-1481.1522, 2702.0473, 56.2543, REGULAR},//ElQuebrados
		{-1486.8842, 2688.6520, 55.8402, REGULAR},//ElQuebrados
		{-1500.4600, 2690.4541, 55.8359, REGULAR},//ElQuebrados
		{-1511.2160, 2694.5146, 55.8359, REGULAR},//ElQuebrados
		{-1522.3834, 2693.4985, 55.8359, REGULAR},//ElQuebrados
		{-1523.5499, 2700.5002, 55.8359, REGULAR},//ElQuebrados
		{-1539.7061, 2693.7783, 55.8402, REGULAR},//ElQuebrados
		{-1538.4739, 2686.4431, 55.8359, REGULAR},//ElQuebrados
		{-1550.9771, 2699.5463, 56.2699, REGULAR},//ElQuebrados
		{-1564.2468, 2709.1250, 55.8359, REGULAR},//ElQuebrados
		{-1571.3677, 2716.0173, 55.8402, REGULAR},//ElQuebrados
		{-1582.5273, 2709.5830, 55.6810, REGULAR},//ElQuebrados
		{-1594.2344, 2698.8686, 55.1123, REGULAR},//ElQuebrados
		{-1602.3577, 2689.5839, 55.2855, REGULAR},//ElQuebrados
		{-1587.9534, 2649.9313, 55.8593, REGULAR},//ElQuebrados
		{-1582.5111, 2645.7036, 55.8359, REGULAR},//ElQuebrados
		{-1579.4624, 2653.8432, 55.8438, REGULAR},//ElQuebrados
		{-1576.5531, 2632.9548, 55.8359, REGULAR},//ElQuebrados
		{-1566.1644, 2629.7199, 55.8402, REGULAR},//ElQuebrados
		{-1533.7115, 2624.7419, 55.8359, REGULAR},//ElQuebrados
		{-1534.9002, 2659.0078, 56.2813, REGULAR},//ElQuebrados
		{-1533.1275, 2654.0678, 56.2813, REGULAR},//ElQuebrados
		{-1513.7261, 2646.1799, 56.1761, REGULAR},//ElQuebrados
		{-1518.6419, 2651.2392, 55.8359, REGULAR},//ElQuebrados
		{-1534.6871, 2620.3549, 59.5002, REGULAR},//ElQuebrados
		{-1529.6389, 2612.0073, 59.7674, REGULAR},//ElQuebrados
		{-1526.1882, 2617.8339, 59.5809, REGULAR},//ElQuebrados
		{-1518.0058, 2612.2280, 59.6796, REGULAR},//ElQuebrados
		{-1524.1295, 2623.2141, 59.6796, REGULAR},//ElQuebrados
		{-1510.4440, 2612.5849, 59.5799, GUNSHOP},//ElQuebradosArmurerie
		{-1507.4259, 2615.0671, 59.5175, GUNSHOP},//ElQuebradosArmurerie
		{-1508.1481, 2626.6650, 59.2257, GUNSHOP},//ElQuebradosArmurerie
		{-1506.9947, 2629.2602, 59.1604, GUNSHOP},//ElQuebradosArmurerie
		{-1511.1755, 2626.1508, 59.2386, GUNSHOP},//ElQuebradosArmurerie
		{-1512.6400, 2629.5776, 55.8359, GUNSHOP},//ElQuebradosArmurerie
		{-1507.6966, 2632.8913, 55.8359, GUNSHOP},//ElQuebradosArmurerie
		{-1504.9525, 2621.4680, 55.8359, GUNSHOP},//ElQuebradosArmurerie
		{-1510.7319, 2608.9162, 55.8359, GUNSHOP},//ElQuebradosArmurerie
		{-1520.3934, 2596.4929, 55.6911, REGULAR},//ElQuebrados
		{-1531.4835, 2592.8391, 55.8359, REGULAR},//ElQuebrados
		{-1524.8542, 2587.3020, 55.8359, REGULAR},//ElQuebrados
		{-1519.9470, 2580.5529, 55.8359, REGULAR},//ElQuebrados
		{-1518.4831, 2577.2619, 56.0096, REGULAR},//ElQuebrados
		{-1511.6821, 2576.3325, 55.8359, REGULAR},//ElQuebrados
		{-1516.5166, 2568.1093, 55.8359, REGULAR},//ElQuebrados
		{-1529.2864, 2570.3247, 55.8359, REGULAR},//ElQuebrados
		{-1510.7342, 2520.0202, 55.8972, REGULAR},//ElQuebrados
		{-1501.7902, 2523.7421, 55.6875, REGULAR},//ElQuebrados
		{-1499.5941, 2516.1191, 55.9537, REGULAR},//ElQuebrados
		{-1478.4272, 2545.8740, 56.2543, REGULAR},//ElQuebrados
		{-1471.0166, 2547.5026, 55.8359, REGULAR},//ElQuebrados
		{-1475.6820, 2562.1755, 56.1761, REGULAR},//ElQuebrados
		{-1480.2333, 2568.4470, 55.8359, REGULAR},//ElQuebrados
		{-1448.0526, 2562.9501, 55.8359, REGULAR},//ElQuebrados
		{-1453.0119, 2578.2932, 55.8359, REGULAR},//ElQuebrados
		{-1456.3221, 2589.5676, 59.0312, REGULAR},//ElQuebrados
		{-1460.0644, 2585.6442, 59.0312, REGULAR},//ElQuebrados
		{-1477.6900, 2588.3461, 60.7544, REGULAR},//ElQuebrados
		{-1483.7407, 2584.1018, 60.6300, REGULAR},//ElQuebrados
		{-1484.1099, 2588.5971, 60.7618, REGULAR},//ElQuebrados
		{-1479.0927, 2579.1088, 60.4837, REGULAR},//ElQuebrados
		{-1585.0145, 2645.1140, 55.8359, REGULAR},//ElQuebrados
		{-1670.6248, 2597.9636, 81.4453, REGULAR},//ElQuebrados
		{-1673.7362, 2592.9331, 81.3593, REGULAR},//ElQuebrados
		{-1670.5961, 2552.4099, 85.3046, REGULAR},//ElQuebrados
		{-1665.7912, 2551.5139, 88.3433, REGULAR},//ElQuebrados
		{-1673.4357, 2493.9624, 87.1657, REGULAR},//ElQuebrados
		{-1667.5772, 2488.1672, 87.1565, REGULAR},//ElQuebrados
		{-1667.1058, 2478.2304, 87.2192, REGULAR},//ElQuebrados
		{-1526.6424, 2507.3505, 55.9358, HOSPITAL},//ElQuebradosHopital
		{-1515.2766, 2496.4648, 55.9490, HOSPITAL},//ElQuebradosHopital
		{-1529.3006, 2511.3684, 59.4382, HOSPITAL},//ElQuebradosHopital
		{-1533.4904, 2508.4870, 59.4382, HOSPITAL},//ElQuebradosHopital
		{-1514.6921, 2519.1997, 60.4538, HOSPITAL},//ElQuebradosHopital
		{-1502.4331, 2508.0573, 56.5904, HOSPITAL},//ElQuebradosHopital
		{-1512.6850, 2493.4416, 59.4542, HOSPITAL},//ElQuebradosHopital
		{-1509.7851, 2497.9023, 59.4542, HOSPITAL},//ElQuebradosHopital
		{-1512.7335, 2496.3789, 55.9521, HOSPITAL},//ElQuebradosHopital
		{-1510.5841, 2493.5363, 55.8908, HOSPITAL},//ElQuebradosHopital
		{-1502.4152, 2498.1496, 55.6359, HOSPITAL},//ElQuebradosHopital
		{-1502.4107, 2519.8413, 55.9023, HOSPITAL},//ElQuebradosHopital
		{-1511.9226, 2523.7360, 55.8079, HOSPITAL},//ElQuebradosHopital
		{-1515.7860, 2520.2053, 55.9420, HOSPITAL},//ElQuebradosHopital
		{-1523.9537, 2520.4165, 55.8934, HOSPITAL},//ElQuebradosHopital
		{-1421.9199, 2175.4177, 50.4454, REGULAR},//TierraRobada
		{-1427.9442, 2166.6345, 49.8226, REGULAR},//TierraRobada
		{-1438.4582, 2173.2136, 49.8298, REGULAR},//TierraRobada
		{-1375.7222, 2109.6884, 42.1999, REGULAR},//TierraRobada
		{-1387.8836, 2113.0859, 42.1912, REGULAR},//TierraRobada
		{-1364.1109, 2064.6518, 52.5956, REGULAR},//TierraRobada
		{-1367.5709, 2051.1774, 52.5161, REGULAR},//TierraRobada
		{-1359.1822, 2050.5507, 52.5156, REGULAR},//TierraRobada
		{-1353.5366, 2051.3056, 52.5156, REGULAR},//TierraRobada
		{-1352.8306, 2054.5769, 53.1171, REGULAR},//TierraRobada
		{-1353.3601, 2059.9445, 53.1171, REGULAR},//TierraRobada
		{-1224.0504, 1820.3182, 41.7187, REGULAR},//TierraRobada
		{-1232.1207, 1829.4603, 41.6339, REGULAR},//TierraRobada
		{-1230.5367, 1835.6474, 41.5207, REGULAR},//TierraRobada
		{-1221.3701, 1839.8551, 41.6589, REGULAR},//TierraRobada
		{-1222.3402, 1844.2285, 41.5231, REGULAR},//TierraRobada
		{-1212.5867, 1839.0024, 41.7187, REGULAR},//TierraRobada
		{-1211.4822, 1832.6679, 41.9296, REGULAR},//TierraRobada
		{-1218.0166, 1822.1953, 41.7244, REGULAR},//TierraRobada
		{-1448.1737, 1873.9653, 32.6902, GAS},//TierraRobadaStationEssence
		{-1461.0144, 1883.5657, 32.6398, GAS},//TierraRobadaStationEssence
		{-1477.1196, 1873.6934, 32.6328, GAS},//TierraRobadaStationEssence
		{-1468.3604, 1861.4777, 32.6328, GAS},//TierraRobadaStationEssence
		{-1456.5217, 1875.6774, 36.4296, GAS},//TierraRobadaStationEssence
		{-1465.9406, 1881.4033, 36.4296, GAS},//TierraRobadaStationEssence
		{-1931.8540, 2360.7568, 49.3852, REGULAR},//TierraRobada
		{-1946.3500, 2363.0310, 49.4921, REGULAR},//TierraRobada
		{-1957.2250, 2376.0251, 49.4999, REGULAR},//TierraRobada
		{-1952.4088, 2390.1284, 49.4921, REGULAR},//TierraRobada
		{-1948.1857, 2406.9165, 50.0093, REGULAR},//TierraRobada
		{-1962.7229, 2360.7922, 47.3013, REGULAR},//TierraRobada
		{-2091.1875, 2321.3469, 24.0684, REGULAR},//TierraRobada
		{-2097.7182, 2314.8076, 24.1026, REGULAR},//TierraRobada
		{-2086.9843, 2308.8996, 24.4563, REGULAR},//TierraRobada
		{-2090.0292, 2311.6679, 25.9140, REGULAR},//TierraRobada
		{-2094.0673, 2314.9323, 25.9140, REGULAR},//TierraRobada
		{-1808.2269, 2052.1657, 9.0355, REGULAR},//TierraRobada
		{-1803.5327, 2038.2716, 9.5902, REGULAR},//TierraRobada
		{-1818.3480, 2042.5644, 8.5245, REGULAR},//TierraRobada
		{-1829.2463, 2044.7080, 8.6960, REGULAR},//TierraRobada
		{-1836.3051, 2058.1428, 9.8007, REGULAR},//TierraRobada
		{-1819.7690, 2064.2858, 9.2887, REGULAR},//TierraRobada
		{-1045.1162, 1550.7011, 33.3829, REGULAR},//TierraRobada
		{-1051.8925, 1548.1597, 33.4376, REGULAR},//TierraRobada
		{-1044.4790, 1558.7993, 33.1856, REGULAR},//TierraRobada
		{-1056.3269, 1566.2492, 33.2095, REGULAR},//TierraRobada
		{-1063.3625, 1560.5897, 32.9582, REGULAR},//TierraRobada
		{-682.1333, 933.3876, 12.1328, REGULAR},//TierraRobada
		{-677.8728, 931.4824, 12.1328, REGULAR},//TierraRobada
		{-674.4896, 935.7983, 12.1328, REGULAR},//TierraRobada
		{-691.2049, 928.1784, 13.6293, REGULAR},//TierraRobada
		{-684.9309, 939.0118, 13.6328, REGULAR},//TierraRobada
		{-687.8344, 933.9174, 13.6328, REGULAR},//TierraRobada
		{-693.2782, 940.0563, 16.8281, REGULAR},//TierraRobada
		{-676.0696, 945.5897, 12.1328, REGULAR},//TierraRobada
		{-670.7601, 958.9614, 12.1328, REGULAR},//TierraRobada
		{-830.5890, 1421.3997, 13.9498, REGULAR},//LasBarrancas
		{-823.3491, 1445.3165, 13.9531, REGULAR},//LasBarrancas
		{-811.3260, 1447.3365, 13.9453, REGULAR},//LasBarrancas
		{-795.5802, 1448.8173, 13.9453, REGULAR},//LasBarrancas
		{-783.7926, 1449.6843, 13.9453, REGULAR},//LasBarrancas
		{-781.3809, 1427.1584, 13.9453, REGULAR},//LasBarrancas
		{-806.1057, 1423.9370, 13.9453, REGULAR},//LasBarrancas
		{-823.5767, 1469.8188, 18.7236, REGULAR},//LasBarrancas
		{-802.5924, 1474.6011, 21.0000, REGULAR},//LasBarrancas
		{-743.4111, 1437.4405, 16.3745, REGULAR},//LasBarrancas
		{-733.3109, 1443.0241, 17.1578, REGULAR},//LasBarrancas
		{-716.6899, 1438.2614, 18.8871, REGULAR},//LasBarrancas
		{-710.2361, 1431.9620, 18.4765, REGULAR},//LasBarrancas
		{-692.3790, 1437.0368, 17.4995, REGULAR},//LasBarrancas
		{-664.2846, 1447.3566, 13.7374, REGULAR},//LasBarrancas
		{-659.1406, 1440.3372, 13.6171, REGULAR},//LasBarrancas
		{-651.9086, 1448.0511, 13.6171, REGULAR},//LasBarrancas
		{-646.1315, 1450.2698, 13.6171, REGULAR},//LasBarrancas
		{-636.9537, 1446.4709, 13.9964, REGULAR},//LasBarrancas
		{-772.6363, 1484.4074, 24.3408, REGULAR},//LasBarrancas
		{-792.9244, 1482.2145, 22.5673, REGULAR},//LasBarrancas
		{-806.4669, 1482.5437, 20.8839, REGULAR},//LasBarrancas
		{-817.8597, 1479.4222, 19.5475, REGULAR},//LasBarrancas
		{-822.0136, 1504.6230, 19.8584, REGULAR},//LasBarrancas
		{-812.9285, 1513.2213, 20.5255, REGULAR},//LasBarrancas
		{-789.8806, 1508.9523, 23.0531, REGULAR},//LasBarrancas
		{-781.4223, 1504.4660, 29.0341, REGULAR},//LasBarrancas
		{-792.0734, 1503.3671, 26.4771, REGULAR},//LasBarrancas
		{-783.5465, 1518.8212, 27.0429, REGULAR},//LasBarrancas
		{-794.5970, 1526.8830, 27.1140, REGULAR},//LasBarrancas
		{-790.6901, 1551.8300, 27.1171, REGULAR},//LasBarrancas
		{-783.3665, 1559.8826, 27.1171, REGULAR},//LasBarrancas
		{-765.6910, 1570.9373, 26.9693, REGULAR},//LasBarrancas
		{-748.2930, 1595.4309, 27.1171, REGULAR},//LasBarrancas
		{-739.3101, 1603.8411, 27.1171, REGULAR},//LasBarrancas
		{-742.1945, 1623.4086, 27.1171, REGULAR},//LasBarrancas
		{-742.5883, 1603.2462, 29.5703, REGULAR},//LasBarrancas
		{-729.6301, 1535.1173, 40.2243, REGULAR},//LasBarrancas
		{-722.9685, 1544.8811, 39.4604, REGULAR},//LasBarrancas
		{-733.3125, 1554.5263, 39.7419, REGULAR},//LasBarrancas
		{-787.9016, 1561.5847, 27.1171, REGULAR},//LasBarrancas
		{-790.2465, 1552.3557, 27.1171, REGULAR},//LasBarrancas
		{-817.1196, 1557.4248, 27.1171, REGULAR},//LasBarrancas
		{-824.9155, 1572.8522, 27.1171, REGULAR},//LasBarrancas
		{-827.0989, 1556.5327, 27.1171, REGULAR},//LasBarrancas
		{-820.2383, 1556.8380, 30.6651, REGULAR},//LasBarrancas
		{-856.0488, 1559.6896, 24.2277, REGULAR},//LasBarrancas
		{-867.2157, 1537.4476, 22.5870, REGULAR},//LasBarrancas
		{-867.4689, 1524.3425, 22.5870, REGULAR},//LasBarrancas
		{-860.7660, 1514.9385, 22.5870, REGULAR},//LasBarrancas
		{-858.4703, 1537.1169, 22.5870, REGULAR},//LasBarrancas
		{-883.2144, 1562.6164, 25.9113, REGULAR},//LasBarrancas
		{-880.3121, 1549.0533, 25.9140, REGULAR},//LasBarrancas
		{-880.7677, 1536.9726, 25.9140, REGULAR},//LasBarrancas
		{-905.4520, 1515.8345, 26.3168, REGULAR},//LasBarrancas
		{-1319.1342, 2540.7915, 87.6206, REGULAR},//AldeaMalvada
		{-1333.9453, 2526.0307, 87.0468, REGULAR},//AldeaMalvada
		{-1336.7545, 2509.9020, 87.0213, REGULAR},//AldeaMalvada
		{-1335.1018, 2493.6567, 87.0468, REGULAR},//AldeaMalvada
		{-1331.0821, 2489.3576, 87.0468, REGULAR},//AldeaMalvada
		{-1317.4104, 2500.0000, 87.0420, REGULAR},//AldeaMalvada
		{-1313.0289, 2525.5781, 87.5150, REGULAR},//AldeaMalvada
		{-1299.3422, 2536.5949, 87.7421, REGULAR},//AldeaMalvada
		{-1285.9600, 2541.7500, 86.7976, REGULAR},//AldeaMalvada
		{-1312.1374, 2511.7409, 87.0420, REGULAR},//AldeaMalvada
		{-1327.1241, 2487.0891, 87.0468, REGULAR},//AldeaMalvada
		{-897.1773, 1975.0209, 60.6306, WAREHOUSE},//ShermanDam
		{-864.6166, 1995.3682, 60.1875, WAREHOUSE},//ShermanDam
		{-787.2794, 2101.2980, 60.3828, WAREHOUSE},//ShermanDam
		{-780.2859, 2125.0520, 60.3828, WAREHOUSE},//ShermanDam
		{-644.6754, 2148.7624, 60.3828, WAREHOUSE},//ShermanDam
		{-652.1885, 2133.4096, 60.3828, WAREHOUSE},//ShermanDam
		{-619.7211, 2045.5307, 60.3828, WAREHOUSE},//ShermanDam
		{-613.3627, 2030.1831, 60.3828, WAREHOUSE},//ShermanDam
		{-2186.7299, 2410.3435, 4.9711, REGULAR},//Bayside
		{-2189.3608, 2412.4870, 5.1562, REGULAR},//Bayside
		{-2184.7290, 2419.5612, 5.0476, REGULAR},//Bayside
		{-2213.2272, 2421.4548, 2.4940, REGULAR},//Bayside
		{-2209.2407, 2415.8708, 2.4841, REGULAR},//Bayside
		{-2230.3417, 2401.4589, 2.4843, REGULAR},//Bayside
		{-2218.7299, 2376.9157, 4.9737, REGULAR},//Bayside
		{-2225.3967, 2361.1503, 4.9874, REGULAR},//Bayside
		{-2217.0024, 2337.5717, 4.9843, REGULAR},//Bayside
		{-2213.4963, 2322.8737, 4.9843, REGULAR},//Bayside
		{-2225.7482, 2314.9006, 4.9917, REGULAR},//Bayside
		{-2237.2165, 2316.3303, 7.5468, REGULAR},//Bayside
		{-2231.1767, 2334.9189, 7.5468, REGULAR},//Bayside
		{-2242.5700, 2359.3833, 4.9810, REGULAR},//Bayside
		{-2256.2783, 2375.7368, 5.0023, REGULAR},//Bayside
		{-2245.3244, 2377.5341, 5.0062, REGULAR},//Bayside
		{-2257.7412, 2381.4377, 4.9504, REGULAR},//Bayside
		{-2237.8020, 2471.0891, 4.9843, REGULAR},//Bayside
		{-2239.4050, 2459.9802, 4.9810, REGULAR},//Bayside
		{-2233.3916, 2443.7819, 2.4864, REGULAR},//Bayside
		{-2241.4006, 2448.0007, 2.4737, REGULAR},//Bayside
		{-2302.8957, 2334.7124, 4.9843, REGULAR},//Bayside
		{-2302.3786, 2299.8354, 4.9843, REGULAR},//Bayside
		{-2294.0095, 2287.2480, 4.9811, REGULAR},//Bayside
		{-2296.5510, 2254.3669, 4.9843, REGULAR},//Bayside
		{-2290.9138, 2235.0461, 4.9821, REGULAR},//Bayside
		{-2331.1142, 2307.4846, 3.5000, REGULAR},//Bayside
		{-2330.8176, 2294.4279, 3.5000, REGULAR},//Bayside
		{-2338.2980, 2293.0639, 4.9843, REGULAR},//Bayside
		{-2387.6254, 2327.7216, 4.9843, REGULAR},//Bayside
		{-2436.3200, 2306.3388, 4.9843, REGULAR},//Bayside
		{-2432.7580, 2291.0827, 4.9843, REGULAR},//Bayside
		{-2432.2910, 2259.5668, 4.9843, REGULAR},//Bayside
		{-2443.3168, 2240.0876, 4.8437, REGULAR},//Bayside
		{-2386.9460, 2216.0258, 4.9843, REGULAR},//Bayside
		{-2374.1591, 2215.6132, 4.9843, REGULAR},//Bayside
		{-2387.3486, 2213.9768, 4.9843, REGULAR},//Bayside
		{-2455.5205, 2263.9504, 4.9849, REGULAR},//Bayside
		{-2455.2114, 2287.4331, 4.9807, REGULAR},//Bayside
		{-2456.5666, 2312.4521, 4.9843, REGULAR},//Bayside
		{-2483.7961, 2294.1735, 4.9843, REGULAR},//Bayside
		{-2501.6604, 2284.3244, 4.9843, REGULAR},//Bayside
		{-2504.5256, 2274.4816, 4.9843, REGULAR},//Bayside
		{-2522.4145, 2290.1010, 4.9843, REGULAR},//Bayside
		{-2529.4313, 2294.7229, 4.9843, REGULAR},//Bayside
		{-2547.4733, 2303.5681, 4.9843, REGULAR},//Bayside
		{-2551.6872, 2319.1062, 4.9750, REGULAR},//Bayside
		{-2537.9777, 2320.6259, 4.9843, REGULAR},//Bayside
		{-2518.2778, 2339.0957, 4.9843, REGULAR},//Bayside
		{-2503.7866, 2365.7309, 4.9805, REGULAR},//Bayside
		{-2522.3999, 2353.8564, 4.9838, REGULAR},//Bayside
		{-2544.1501, 2353.7023, 4.9843, REGULAR},//Bayside
		{-2543.9936, 2367.9250, 4.9897, REGULAR},//Bayside
		{-2526.8518, 2369.6718, 4.9958, REGULAR},//Bayside
		{-2546.2526, 2339.9428, 4.9794, REGULAR},//Bayside
		{-2583.0812, 2367.5144, 9.7071, REGULAR},//Bayside
		{-2583.3601, 2354.2333, 9.7334, REGULAR},//Bayside
		{-2598.9633, 2349.8742, 9.0251, REGULAR},//Bayside
		{-2600.3811, 2359.6926, 9.0000, REGULAR},//Bayside
		{-2580.2707, 2314.1923, 5.9576, REGULAR},//Bayside
		{-2583.0451, 2318.1503, 5.9476, REGULAR},//Bayside
		{-2601.8566, 2303.5551, 7.5068, REGULAR},//Bayside
		{-2588.4262, 2287.9907, 6.9860, REGULAR},//Bayside
		{-2609.2143, 2270.8527, 8.2109, REGULAR},//Bayside
		{-2625.9519, 2285.6506, 8.3013, REGULAR},//Bayside
		{-2626.6145, 2294.1477, 8.3066, REGULAR},//Bayside
		{-2625.9562, 2308.2346, 8.3013, REGULAR},//Bayside
		{-2625.7177, 2316.4665, 8.2994, REGULAR},//Bayside
		{-2632.6140, 2349.6462, 8.4851, REGULAR},//Bayside
		{-2639.4277, 2346.6320, 8.4936, REGULAR},//Bayside
		{-2639.5031, 2334.6623, 8.5471, REGULAR},//Bayside
		{-2626.3356, 2360.7797, 8.9273, REGULAR},//Bayside
		{-2633.9091, 2375.5395, 9.0000, REGULAR},//Bayside
		{-2637.4309, 2384.6918, 11.1910, REGULAR},//Bayside
		{-2625.6064, 2395.2099, 11.1114, REGULAR},//Bayside
		{-2631.7658, 2402.5739, 11.3354, REGULAR},//Bayside
		{-2622.6552, 2412.6828, 13.7764, REGULAR},//Bayside
		{-2495.9387, 2359.8220, 14.1182, REGULAR},//Bayside
		{-2496.5620, 2343.6586, 14.1182, REGULAR},//Bayside
		{-2491.5402, 2360.1567, 10.2759, REGULAR},//Bayside
		{-2479.1745, 2349.1398, 8.0229, REGULAR},//Bayside
		{-2453.9965, 2349.2551, 5.2524, REGULAR},//Bayside
		{-2440.1621, 2344.9270, 4.9687, REGULAR},//Bayside
		{-2435.2866, 2353.7910, 4.9609, REGULAR},//Bayside
		{-2423.8847, 2348.8391, 4.9687, REGULAR},//Bayside
		{-2429.3286, 2368.6499, 5.1996, REGULAR},//Bayside
		{-2380.7609, 2389.4804, 9.0727, REGULAR},//Bayside
		{-2344.4062, 2440.0812, 7.3046, REGULAR},//Bayside
		{-2379.7917, 2439.7563, 9.2349, REGULAR},//Bayside
		{-2385.4218, 2443.6247, 9.2333, REGULAR},//Bayside
		{-2394.6696, 2412.7639, 8.8849, REGULAR},//Bayside
		{-2399.4177, 2403.6252, 8.9296, REGULAR},//Bayside
		{-2422.4919, 2410.6337, 13.1439, REGULAR},//Bayside
		{-2423.2856, 2399.9658, 13.0980, REGULAR},//Bayside
		{-2427.7736, 2445.3815, 13.5354, REGULAR},//Bayside
		{-2438.7238, 2448.0935, 13.7921, REGULAR},//Bayside
		{-2447.3459, 2449.5266, 14.8937, REGULAR},//Bayside
		{-2470.2731, 2446.8503, 16.2051, REGULAR},//Bayside
		{-2479.6357, 2447.0988, 17.0735, REGULAR},//Bayside
		{-2491.0986, 2460.5742, 17.2074, REGULAR},//Bayside
		{-2480.0136, 2480.9211, 17.8400, REGULAR},//Bayside
		{-2465.8662, 2488.9326, 16.8963, REGULAR},//Bayside
		{-2472.1848, 2505.9011, 17.6251, REGULAR},//Bayside
		{-2462.6301, 2512.4187, 18.2553, REGULAR},//Bayside
		{-2443.6357, 2511.1037, 15.3039, REGULAR},//Bayside
		{-2398.8149, 2474.7507, 10.2639, REGULAR},//Bayside
	//San Fierro
		{-1620.9212, 1414.7185, 7.1842, WAREHOUSE}, //Pier
		{-1639.6843, 1425.2152, 7.1875,WAREHOUSE}, //Pier
		{-1645.3466, 1403.9510, 9.8046,WAREHOUSE}, //Pier
		{-1642.2239, 1392.6870, 9.8046,WAREHOUSE}, //Pier
		{-1659.3173, 1383.0517, 9.8046,WAREHOUSE}, //Pier
		{-1663.7623, 1398.2263, 9.7971,WAREHOUSE}, //Pier
		{-1677.6530, 1400.4361, 12.2031,WAREHOUSE}, //Pier
		{-1684.2116, 1370.5545, 9.8046,WAREHOUSE}, //Pier
		{-1694.0095, 1364.1590, 9.8046,WAREHOUSE}, //Pier
		{-1682.7247, 1351.2299, 9.8046,WAREHOUSE}, //Pier
		{-1682.7087, 1364.1701, 7.1796,WAREHOUSE}, //Pier
		{-1668.9061, 1348.4858, 7.1796,WAREHOUSE}, //Pier
		{-1700.3460, 1367.9318, 7.1796,WAREHOUSE}, //Pier
		{-2650.5776, 1373.2685, 7.1870,WAREHOUSE}, //Pier
		{-2650.0861, 1373.3188, 20.7265,WAREHOUSE}, //Pier
		{-2617.2011, 1410.0172, 7.1103,WAREHOUSE}, //Pier
		{-2679.3120, 1374.6402, 7.0960,WAREHOUSE}, //Pier
		{-2686.4311, 1415.0936, 7.1015,WAREHOUSE}, //Pier
		{-2648.0368, 1444.5858, 7.1215,WAREHOUSE}, //Pier
		{-2475.6250, 1395.7286, 7.1875,WAREHOUSE}, //Pier
		{-2429.4328, 1383.9960, 7.0468,WAREHOUSE}, //Pier
		{-2205.0351, 1351.5319, 7.1840,WAREHOUSE}, //Pier
		{-2078.3020, 1419.3580, 7.1006,WAREHOUSE}, //Pier
		{-2074.6159, 1413.2459, 7.1015,WAREHOUSE}, //Pier
		{-2067.3195, 1428.1458, 7.1006,WAREHOUSE}, //Pier
		{-2069.1398, 1408.8332, 7.1006,WAREHOUSE}, //Pier
		{-2010.9885, 1343.1704, 7.1530,WAREHOUSE}, //Pier
		{-1994.1695, 1389.7370, 7.1817,WAREHOUSE}, //Pier
		{-1972.9975, 1377.0233, 7.1841,WAREHOUSE}, //Pier
		{-1943.6066, 1391.8232, 7.1807,WAREHOUSE}, //Pier
		{-1918.0203, 1375.7712, 7.1841,WAREHOUSE}, //Pier
		{-1867.7384, 1420.7011, 7.1829,WAREHOUSE}, //Pier
		{-1881.2236, 1441.3656, 7.1822,WAREHOUSE}, //Pier
		{-1916.0238, 1473.7508, 7.1839,WAREHOUSE}, //Pier
		{-1956.9222, 1500.9427, 7.1875,WAREHOUSE}, //Pier
		{-1956.5039, 1495.5056, 7.1875,WAREHOUSE}, //Pier
		{-1801.7039, 1417.2603, 10.1145,WAREHOUSE}, //Pier
		{-1781.0510, 1414.6024, 10.2846,WAREHOUSE}, //Pier
		{-1762.2708, 1421.6591, 12.1839,WAREHOUSE}, //Pier
		{-1746.9548, 1428.6065, 10.8586,WAREHOUSE}, //Pier
		{-1639.7188, 1305.2059, 7.0257,WAREHOUSE}, //Pier
		{-1581.6004, 1292.3536, 7.1737,WAREHOUSE}, //Pier
		{-1500.8380, 1295.7225, 4.9822,WAREHOUSE}, //Pier
		{-1499.6577, 1276.5117, 7.1719,WAREHOUSE}, //Pier
		{-1546.0242, 1267.6080, 6.9921,WAREHOUSE}, //Pier
		{-1598.6654, 1242.7241, 7.1786,WAREHOUSE}, //Pier
		{-1557.4350, 1155.3104, 7.1875,WAREHOUSE}, //Pier
		{-1567.5067, 1121.6391, 7.1875,WAREHOUSE}, //Pier
		{-1537.8413, 1109.5095, 7.1875,WAREHOUSE}, //Pier
		{-1494.1677, 1127.2996, 7.1845,WAREHOUSE}, //Pier
		{-1487.0891, 1139.2657, 7.1845,WAREHOUSE}, //Pier
		{-1463.3275, 1136.7039, 7.1875,WAREHOUSE}, //Pier
		{-1483.0516, 1073.5844, 7.1845,WAREHOUSE}, //Pier
		{-1464.4129, 1079.5104, 7.1875,WAREHOUSE}, //Pier
		{-1494.8781, 1081.6066, 7.1875,WAREHOUSE}, //Pier
		{-1509.0898, 1026.5880, 7.1875,WAREHOUSE}, //Pier
		{-1495.6500, 994.4010, 7.1875,WAREHOUSE}, //Pier
		{-1462.7618, 1004.3724, 7.1875,WAREHOUSE}, //Pier
		{-1436.1932, 970.2739, 7.1875,WAREHOUSE}, //Pier
		{-1445.6824, 926.2833, 7.1875,WAREHOUSE}, //Pier
		{-1429.2882, 864.5908, 7.1875,WAREHOUSE}, //Pier
		{-1477.2271, 833.2797, 7.1875,WAREHOUSE}, //Pier
		{-1517.2941, 814.0418, 7.1875,WAREHOUSE}, //Pier
		{-2639.0578, 635.4556, 14.4531,HOSPITAL}, //Pier
		{-2653.2631, 638.2199, 14.4531,HOSPITAL}, //Pier
		{-2657.7250, 632.6672, 14.4531,HOSPITAL}, //Pier
		{-2645.5566, 632.1350, 14.4545,HOSPITAL}, //Pier
		{-2684.7182, 636.0294, 14.4531,HOSPITAL}, //Pier
		{-2680.5200, 614.8224, 14.4531,HOSPITAL}, //Pier
		{-2665.9865, 615.7811, 14.4545,HOSPITAL}, //Pier
		{-2655.5170, 604.7302, 14.4531,HOSPITAL}, //Pier
		{-2641.4748, 607.3833, 14.4531,HOSPITAL}, //Pier
		{-2626.3576, 603.4681, 14.4531,HOSPITAL}, //Pier
		{-2590.9489, 623.6483, 14.4591,HOSPITAL}, //Pier
		{-2584.3579, 650.1132, 14.4531,HOSPITAL}, //Pier
		{-2572.3557, 654.8102, 14.4531,HOSPITAL}, //Pier
		{-2566.4648, 643.5058, 14.4591,HOSPITAL}, //Pier
		{-2549.1838, 653.9290, 14.4591,HOSPITAL}, //Pier
		{-2598.0551, 607.0929, 15.6267,HOSPITAL}, //Pier
		{-2613.0747, 584.1152, 15.6250,HOSPITAL}, //Pier
		{-2042.7869, 140.0142, 28.8359, GAS}, //GarageDoherty
		{-2050.3120, 148.4412, 28.8359, GAS}, //GarageDoherty
		{-2060.6989, 162.6502, 28.8359, GAS}, //GarageDoherty
		{-2054.0114, 187.1128, 29.4308, GAS}, //GarageDoherty
		{-2020.4605, 171.6208, 28.5240, GAS}, //GarageDoherty
		{-2032.1372, 156.6508, 29.0461, GAS}, //GarageDoherty
		{-2025.7489, 145.0986, 28.8359, GAS}, //GarageDoherty
		{-2015.7248, 155.0166, 27.6875, GAS}, //GarageDoherty
		{-1693.3461, 400.2355, 7.1796, GAS}, //Essence
		{-1704.6369, 403.4992, 7.1796, GAS}, //Essence
		{-1709.5938, 404.5190, 7.4189, GAS}, //Essence
		{-1716.4571, 391.0236, 7.1796, GAS}, //Essence
		{-1678.3350, 427.3223, 7.1796, GAS}, //Essence
		{-1664.1804, 426.0299, 7.1796, GAS}, //Essence
		{-1664.4757, 452.1534, 7.1796, GAS}, //Essence
		{-2413.8693, 1007.0186, 50.3906, GAS}, //Essence
		{-2425.5698, 1000.6738, 50.3906, GAS}, //Essence
		{-2431.5703, 1038.4781, 50.3906, GAS}, //Essence
		{-2407.4628, 967.2319, 45.2968, GAS}, //Essence
		{-2413.3671, 976.9246, 45.2968, GAS}, //Essence
		{-2418.9289, 988.1569, 45.2968, GAS}, //Essence
		{-2427.4514, 991.0850, 45.2968, GAS}, //Essence
		{-2449.5019, 972.6026, 45.2968, GAS}, //Essence
		{-2432.3723, 954.7687, 45.2968, GAS}, //Essence
		{-2626.3859, 209.3407, 4.6028, GUNSHOP}, //Ammunation
		{-2617.9631, 209.4333, 4.8287, GUNSHOP}, //Ammunation
		{-2624.9904, 213.3578, 4.4951, GUNSHOP}, //Ammunation
		{-2610.6936, 207.6441, 5.5391, GUNSHOP}, //Ammunation
		{-2601.2619, 201.2879, 5.4622, GUNSHOP}, //Ammunation
		{-2591.7607, 207.0933, 7.5058, GUNSHOP}, //Ammunation
		{-2587.7878, 199.2804, 5.5843, GUNSHOP}, //Ammunation
		{-2615.0063, 195.1627, 4.4782, GUNSHOP}, //Ammunation
		{-2611.2866, 188.5971, 4.3203, GUNSHOP}, //Ammunation
		{-2620.3757, 185.9631, 4.3418, GUNSHOP}, //Ammunation
		{-2626.9350, 182.6282, 4.3410, GUNSHOP}, //Ammunation
		{-2633.5715, 181.7796, 4.3408, GUNSHOP}, //Ammunation
		{-2639.9858, 185.4959, 4.3316, GUNSHOP}, //Ammunation
		{-2631.3649, 210.8587, 4.4758, GUNSHOP}, //Ammunation
		{-2622.3132, 210.1499, 4.7128, GUNSHOP}, //Ammunation
		{-1963.7958, 307.2109, 35.4739, GAS}, //Garage
		{-1954.6440, 302.0443, 35.4687, GAS}, //Garage
		{-1951.6600, 281.0103, 35.4687, GAS}, //Garage
		{-1946.0391, 271.1849, 35.4739, GAS}, //Garage
		{-1952.0493, 265.5869, 35.4687, GAS}, //Garage
		{-1953.0321, 256.5757, 35.4739, GAS}, //Garage
		{-1947.6143, 259.9293, 35.4687, GAS}, //Garage
		{-1958.7849, 258.9367, 35.4687, GAS}, //Garage
		{-1945.8574, 274.3023, 41.0470, GAS}, //Garage
		{-1952.1361, 293.0125, 41.0470, GAS}, //Garage
		{-1957.2736, 299.8148, 41.0470, GAS}, //Garage
		{-1957.8944, 260.9559, 41.0470, GAS}, //Garage
		{-1933.5754, 273.1654, 41.0468, GAS}, //Garage
		{-1921.4879, 283.0885, 41.0468, GAS}, //Garage
		{-1917.0230, 277.6726, 41.0468, GAS}, //Garage
		{-1927.6976, 243.1398, 41.0390, GAS}, //Garage
		{-1986.1104, 252.0126, 35.1718, GAS}, //Garage
		{-1969.0772, 274.7077, 35.1718, GAS}, //Garage
		{-1987.6569, 299.4972, 35.1793, GAS}, //Garage
		{-1732.3101, 214.5241, 3.5546, WAREHOUSE}, //Docks
		{-1745.5238, 202.4082, 3.5546, WAREHOUSE}, //Docks
		{-1747.6873, 186.2460, 3.5495, WAREHOUSE}, //Docks
		{-1734.0631, 160.9494, 3.5546, WAREHOUSE}, //Docks
		{-1725.0854, 165.6062, 3.5546, WAREHOUSE}, //Docks
		{-1717.7716, 133.2309, 3.5546, WAREHOUSE}, //Docks
		{-1733.4829, 125.2439, 3.5546, WAREHOUSE}, //Docks
		{-1707.1938, 99.3403, 3.5546, WAREHOUSE}, //Docks
		{-1668.2841, 107.8711, 3.5546, WAREHOUSE}, //Docks
		{-1610.7457, 156.0074, 3.5546, WAREHOUSE}, //Docks
		{-1591.2314, 149.8096, 3.7169, WAREHOUSE}, //Docks
		{-1530.8155, 148.4432, 3.5546, WAREHOUSE}, //Docks
		{-1542.5756, 127.3928, 3.5546, WAREHOUSE}, //Docks
		{-1569.7316, 89.6410, 3.5546, WAREHOUSE}, //Docks
		{-1624.5428, 58.4955, 3.5494, WAREHOUSE}, //Docks
		{-1646.2178, 59.8568, 3.5546, WAREHOUSE}, //Docks
		{-1654.1146, 56.7726, 3.5546, WAREHOUSE}, //Docks
		{-1671.9327, 32.7311, 3.5546, WAREHOUSE}, //Docks
		{-1650.2048, -21.2900, 3.5546, WAREHOUSE}, //Docks
		{-1641.5167, -33.5811, 3.8062, WAREHOUSE}, //Docks
		{-1627.1981, -43.4616, 3.5546, WAREHOUSE}, //Docks
		{-1648.0312, -71.0890, 3.5720, WAREHOUSE}, //Docks
		{-1659.3818, -92.4465, 3.5574, WAREHOUSE}, //Docks
		{-1675.5197, -100.6767, 3.5690, WAREHOUSE}, //Docks
		{-1693.3223, -103.3578, 3.5546, WAREHOUSE}, //Docks
		{-1724.4519, -89.7989, 3.5546, WAREHOUSE}, //Docks
		{-1730.1121, -120.2651, 3.5546, WAREHOUSE}, //Docks
		{-1733.1967, -49.2533, 3.5546, WAREHOUSE}, //Docks
		{-2666.2731, 1398.6497, 55.8125, WAREHOUSE}, //GantBridge
		{-2681.7907, 1411.0040, 55.8125, WAREHOUSE}, //GantBridge
		{-2690.3400, 1374.1862, 55.8166, WAREHOUSE}, //GantBridge
		{-2697.5288, 1337.1872, 55.8125, WAREHOUSE}, //GantBridge
		{-2681.7294, 1301.5827, 55.6640, WAREHOUSE}, //GantBridge
		{-2675.1926, 1284.6048, 55.4296, WAREHOUSE}, //GantBridge
		{-2672.4414, 1283.5538, 56.4843, WAREHOUSE}, //GantBridge
		{-2667.6030, 1271.3402, 55.4296, WAREHOUSE}, //GantBridge
		{-2677.5437, 1265.8408, 55.4296, WAREHOUSE}, //GantBridge
		{-2686.3974, 1263.1445, 56.7037, WAREHOUSE}, //GantBridge
		{-2695.2810, 1261.5218, 55.4296, WAREHOUSE}, //GantBridge
		{-2688.6354, 1234.8691, 55.4296, WAREHOUSE}, //GantBridge
		{-2679.6755, 1215.7097, 55.4296, WAREHOUSE}, //GantBridge
		{-2666.5598, 1208.6531, 55.4296, WAREHOUSE}, //GantBridge
		{-2663.5700, 1176.8743, 55.4296, WAREHOUSE}, //GantBridge
		{-2641.8520, 1168.3229, 55.4296, WAREHOUSE}, //GantBridge
		{-2603.0639, 1122.5981, 56.6844, WAREHOUSE}, //GantBridge
		{-2574.0180, 1119.6779, 55.5353, WAREHOUSE}, //GantBridge
		{-2547.3359, 1094.2800, 55.5781, WAREHOUSE}, //GantBridge
		{-2502.9887, 1110.4096, 55.5781, WAREHOUSE}, //GantBridge
		{-2566.4082, 1148.0457, 55.7265, REGULAR}, //Maison
		{-2544.0795, 1138.0676, 55.7265, REGULAR}, //Maison
		{-2490.8774, 1137.3549, 55.7265, REGULAR}, //Maison
		{-2424.5366, 1138.1917, 55.7265, REGULAR}, //Maison
		{-2512.8454, 1053.9932, 64.6778, REGULAR}, //Maison
		{-2514.2709, 1038.0914, 70.4623, REGULAR}, //Maison
		{-2514.0341, 1007.1045, 78.3369, REGULAR}, //Maison
		{-2512.3818, 975.8487, 77.0758, REGULAR}, //Maison
		{-2541.4611, 967.5836, 73.5574, REGULAR}, //Maison
		{-2540.8125, 955.8527, 69.3526, REGULAR}, //Maison
		{-2541.2500, 939.3526, 64.0000, REGULAR}, //Maison
		{-2515.6921, 926.7878, 65.1168, REGULAR}, //Maison
		{-2541.8544, 922.2330, 67.0937, REGULAR}, //Maison
		{-2551.8647, 917.8235, 64.9843, REGULAR}, //Maison
		{-2572.9226, 918.8795, 64.9843, REGULAR}, //Maison
		{-2579.6938, 929.0469, 64.9843, REGULAR}, //Maison
		{-2570.9245, 934.5906, 64.9843, REGULAR}, //Maison
		{-2552.1940, 941.4462, 64.8574, REGULAR}, //Maison
		{-2587.6015, 978.6648, 78.2734, REGULAR}, //Maison
		{-2576.0065, 980.0681, 78.2659, REGULAR}, //Maison
		{-2554.6687, 982.3389, 78.2734, REGULAR}, //Maison
		{-2587.5305, 897.8301, 64.9912, REGULAR}, //Maison
		{-2573.5126, 897.5324, 64.9843, REGULAR}, //Maison
		{-2563.1748, 898.0048, 64.9843, REGULAR}, //Maison
		{-2544.5832, 896.6396, 64.9843, REGULAR}, //Maison
		{-2571.4643, 875.7822, 61.7353, REGULAR}, //Maison
		{-2552.6572, 887.9419, 64.9874, REGULAR}, //Maison
		{-2566.6755, 860.6959, 58.4680, REGULAR}, //Maison
		{-2566.9655, 850.8793, 54.8781, REGULAR}, //Maison
		{-2581.3159, 829.0675, 50.0296, REGULAR}, //Maison
		{-2588.2189, 848.3034, 56.8321, REGULAR}, //Maison
		{-2589.4162, 838.5916, 57.9484, REGULAR}, //Maison
		{-2590.0632, 831.2832, 59.2419, REGULAR}, //Maison
		{-2591.0483, 821.6968, 57.1552, REGULAR}, //Maison
		{-2571.4641, 826.2111, 59.4887, REGULAR}, //Maison
		{-2549.8225, 823.7453, 59.4777, REGULAR}, //Maison
		{-2551.9965, 818.9154, 49.9910, REGULAR}, //Maison
		{-2565.6201, 819.1997, 49.9910, REGULAR}, //Maison
		{-2577.5964, 819.1662, 49.9843, REGULAR}, //Maison
		{-2588.0549, 818.8748, 49.9843, REGULAR}, //Maison
		{-2593.4128, 834.7727, 52.0937, REGULAR}, //Maison
		{-2594.6394, 841.2821, 50.3764, REGULAR}, //Maison
		{-2594.7907, 852.1066, 53.5725, REGULAR}, //Maison
		{-2594.7329, 861.8621, 55.6130, REGULAR}, //Maison
		{-2596.5600, 874.9500, 59.5772, REGULAR}, //Maison
		{-2595.3203, 885.6553, 62.8552, REGULAR}, //Maison
		{-2624.2893, 894.1635, 64.9893, REGULAR}, //Maison
		{-2630.9970, 877.8991, 64.0749, REGULAR}, //Maison
		{-2642.9099, 847.2924, 62.5871, REGULAR}, //Maison
		{-2635.6889, 827.0255, 49.9843, REGULAR}, //Maison
		{-2649.1479, 827.6787, 49.9843, REGULAR}, //Maison
		{-2666.4040, 828.6040, 49.9843, REGULAR}, //Maison
		{-2677.7465, 837.1488, 49.9843, REGULAR}, //Maison
		{-2706.3596, 836.7048, 49.9843, REGULAR}, //Maison
		{-2708.4289, 805.2634, 49.9843, REGULAR}, //Maison
		{-2726.7753, 803.8391, 53.0156, REGULAR}, //Maison
		{-2693.1669, 790.6313, 49.9765, REGULAR}, //Maison
		{-2640.8286, 742.7619, 27.9609, REGULAR}, //Maison
		{-2741.1323, 796.1339, 53.2346, REGULAR}, //Maison
		{-2740.8439, 783.0556, 54.3828, REGULAR}, //Maison
		{-2740.5122, 770.0288, 54.3828, REGULAR}, //Maison
		{-2736.1831, 720.8275, 41.2734, REGULAR}, //Maison
		{-2695.4816, 720.3958, 34.7824, REGULAR}, //Maison
		{-2674.6840, 720.5155, 28.3967, REGULAR}, //Maison
		{-2664.0981, 719.6042, 27.9339, REGULAR}, //Maison
		{-2622.9855, 734.2860, 28.3864, REGULAR}, //Maison
		{-2594.1875, 745.7796, 31.9116, REGULAR}, //Maison
		{-2619.8554, 756.4407, 35.3569, REGULAR}, //Maison
		{-2618.6411, 772.5629, 41.6383, REGULAR}, //Maison
		{-2538.7727, 842.3099, 50.5543, REGULAR}, //Maison
		{-2470.2504, 918.6212, 63.1159, REGULAR}, //Maison
		{-2450.0410, 920.3952, 58.0725, REGULAR}, //Maison
		{-2431.6921, 920.0496, 50.5781, REGULAR}, //Maison
		{-2400.2946, 941.6606, 45.4320, REGULAR}, //Maison
		{-2400.8823, 932.0630, 45.6240, REGULAR}, //Maison
		{-2373.1035, 864.3475, 43.3157, REGULAR}, //Maison
		{-2360.4199, 817.1488, 37.8983, REGULAR}, //Maison
		{-2327.2836, 818.8399, 44.2023, REGULAR}, //Maison
		{-2321.4987, 797.3651, 45.3069, REGULAR}, //Maison
		{-2343.8400, 798.1287, 41.2194, REGULAR}, //Maison
		{-2298.5664, 797.4638, 49.2887, REGULAR}, //Maison
		{-2280.8234, 861.6150, 66.7827, REGULAR}, //Maison
		{-2234.8000, 853.6668, 64.3694, REGULAR}, //Maison
		{-2234.6113, 874.3541, 66.6420, REGULAR}, //Maison
		{-2234.2072, 892.1998, 66.6527, REGULAR}, //Maison
		{-2269.8005, 906.8262, 66.5000, REGULAR}, //Maison
		{-2278.5573, 944.8909, 66.6484, REGULAR}, //Maison
		{-2240.3088, 975.6445, 69.8024, REGULAR}, //Maison
		{-2240.3894, 994.1364, 77.2224, REGULAR}, //Maison
		{-2280.8510, 999.6605, 79.3165, REGULAR}, //Maison
		{-2281.2436, 1014.6704, 84.0125, REGULAR}, //Maison
		{-2277.6960, 1040.5460, 83.8437, REGULAR}, //Maison
		{-2240.2424, 1043.1425, 83.8499, REGULAR}, //Maison
		{-2239.7409, 1055.8267, 82.8249, REGULAR}, //Maison
		{-2240.3483, 1074.8454, 80.9029, REGULAR}, //Maison
		{-2281.7145, 1090.8469, 80.3176, REGULAR}, //Maison
		{-2279.7558, 1117.4034, 74.8572, REGULAR}, //Maison
		{-2280.6525, 1135.7247, 67.3112, REGULAR}, //Maison
		{-2238.8186, 1153.9765, 59.6484, REGULAR}, //Maison
		{-2239.3015, 1142.0218, 64.7419, REGULAR}, //Maison
		{-2238.4187, 1121.0097, 73.1684, REGULAR}, //Maison
		{-2229.0686, 1105.6883, 80.0078, REGULAR}, //Maison
		{-2223.3666, 1081.6418, 80.0078, REGULAR}, //Maison
		{-2205.3867, 1080.2819, 80.0130, REGULAR}, //Maison
		{-2193.7749, 1105.8485, 80.0008, REGULAR}, //Maison
		{-2170.6745, 1082.6834, 80.0078, REGULAR}, //Maison
		{-2158.1184, 1121.6207, 72.8024, REGULAR}, //Maison
		{-2158.1945, 1153.9819, 59.3369, REGULAR}, //Maison
		{-2128.4426, 1091.9479, 80.0078, REGULAR}, //Maison
		{-2127.1298, 1069.6343, 80.0078, REGULAR}, //Maison
		{-2126.6547, 1032.0603, 80.0078, REGULAR}, //Maison
		{-2128.0729, 979.7395, 80.0078, REGULAR}, //Maison
		{-2107.2778, 976.0201, 71.5156, REGULAR}, //Maison
		{-2108.6245, 901.6023, 76.7109, REGULAR}, //Maison
		{-2099.9663, 899.2152, 76.7109, REGULAR}, //Maison
		{-2082.9536, 903.4885, 64.3592, REGULAR}, //Maison
		{-2075.4941, 899.2117, 64.1328, REGULAR}, //Maison
		{-2075.6564, 972.6779, 62.9218, REGULAR}, //Maison
		{-2066.5539, 974.8669, 62.9218, REGULAR}, //Maison
		{-2036.0372, 954.0183, 48.8605, REGULAR}, //Maison
		{-2027.0456, 952.0651, 47.2733, REGULAR}, //Maison
		{-2047.9836, 899.7009, 53.4171, REGULAR}, //Maison
		{-2033.6628, 902.3546, 50.3063, REGULAR}, //Maison
		{-1931.2301, 1188.8873, 45.4453, REGULAR}, //Maison
		{-1955.4752, 1188.7308, 45.4453, REGULAR}, //Maison
		{-1995.1273, 1189.3812, 45.4453, REGULAR}, //Maison
		{-1940.1784, 1194.6103, 45.4321, REGULAR}, //Maison
		{-1923.7429, 1188.8819, 45.4453, REGULAR}, //Maison
		{-1913.0593, 1188.8466, 45.4453, REGULAR}, //Maison
		{-1901.4090, 1222.1850, 33.7314, REGULAR}, //Maison
		{-1874.5019, 1147.6511, 45.4453, REGULAR}, //Maison
		{-1874.2645, 1121.8431, 45.4453, REGULAR}, //Maison
		{-1856.5750, 1112.7735, 45.4453, REGULAR}, //Maison
		{-1815.2680, 1112.8262, 45.4453, REGULAR}, //Maison
		{-1813.5030, 1143.0131, 45.4501, REGULAR}, //Maison
		{-1781.0678, 1138.3510, 38.6012, REGULAR}, //Maison
		{-1727.5021, 1160.0748, 30.0745, REGULAR}, //Maison
		{-1727.3076, 1139.7769, 38.2710, REGULAR}, //Maison
		{-1707.3928, 1087.5511, 45.2506, REGULAR}, //Maison
		{-1700.1981, 1023.0335, 45.2109, REGULAR}, //Maison
		{-1696.8905, 1011.4139, 45.2109, REGULAR}, //Maison
		{-1729.4611, 1023.8483, 44.9067, REGULAR}, //Maison
		{-1649.5979, 941.1842, 20.1713, REGULAR}, //Maison
		{-1630.4973, 895.9644, 10.1902, REGULAR}, //Maison
		{-1629.6539, 880.4262, 8.9287, REGULAR}, //Maison
		{-1614.4521, 899.6973, 9.2265, REGULAR}, //Maison
		{-1695.3706, 863.2089, 24.8906, REGULAR}, //Maison
		{-1693.6120, 831.2707, 24.8849, REGULAR}, //Maison
		{-1702.9610, 804.5728, 24.8906, REGULAR}, //Maison
		{-1704.6666, 783.8938, 24.8906, REGULAR}, //Maison
		{-1703.9013, 767.2442, 24.8906, REGULAR}, //Maison
		{-1654.4627, 746.7232, 16.9073, REGULAR}, //Maison
		{-1832.2587, 712.5303, 37.6520, REGULAR}, //Maison
		{-1833.1848, 696.5585, 33.3281, REGULAR}, //Maison
		{-1829.7136, 674.8234, 30.4312, REGULAR}, //Maison
		{-1957.0573, 720.9160, 46.5625, REGULAR}, //Maison
		{-2025.5992, 744.6303, 48.1459, REGULAR}, //Maison
		{-2041.2890, 744.4243, 54.4530, REGULAR}, //Maison
		{-2066.4680, 745.2578, 64.9941, REGULAR}, //Maison
		{-2067.9450, 758.4747, 71.8906, REGULAR}, //Maison
		{-2048.6853, 758.6093, 64.1562, REGULAR}, //Maison
		{-2050.1372, 756.9922, 64.1562, REGULAR}, //Maison
		{-2051.0683, 783.6854, 64.1562, REGULAR}, //Maison
		{-2049.1364, 785.8008, 64.1562, REGULAR}, //Maison
		{-2093.8908, 798.9606, 69.6906, REGULAR}, //Maison
		{-2096.8415, 819.4283, 69.5625, REGULAR}, //Maison
		{-2122.6184, 819.6679, 69.5625, REGULAR}, //Maison
		{-2172.3049, 819.4797, 63.1944, REGULAR}, //Maison
		{-2174.2248, 797.5318, 62.4191, REGULAR}, //Maison
		{-2191.4533, 797.3084, 55.5773, REGULAR}, //Maison
		{-2212.1730, 798.4168, 49.4392, REGULAR}, //Maison
		{-2244.8518, 778.6348, 49.4453, REGULAR}, //Maison
		{-2236.2929, 740.1400, 49.4140, REGULAR}, //Maison
		{-2212.3425, 723.1038, 49.4140, REGULAR}, //Maison
		{-2200.6223, 722.5308, 51.7848, REGULAR}, //Maison
		{-2186.8200, 742.5565, 57.4113, REGULAR}, //Maison
		{-2175.1372, 742.6682, 62.0300, REGULAR}, //Maison
		{-2245.1494, 702.9471, 49.4453, REGULAR}, //Maison
		{-2252.6535, 687.9130, 49.2968, REGULAR}, //Maison
		{-2262.5156, 686.9271, 49.2968, REGULAR}, //Maison
		{-2275.8664, 688.3844, 49.4453, REGULAR}, //Maison
		{-2307.4035, 678.8900, 44.7766, REGULAR}, //Maison
		{-2308.1245, 657.9994, 44.6086, REGULAR}, //Maison
		{-2339.3999, 658.1364, 38.5366, REGULAR}, //Maison
		{-2360.1174, 657.6412, 35.1718, REGULAR}, //Maison
		{-2374.6386, 641.2443, 35.1718, REGULAR}, //Maison
		{-2375.1811, 602.0384, 30.0395, REGULAR}, //Maison
		{-2339.6564, 578.6559, 27.6445, REGULAR}, //Maison
		{-2325.6687, 579.5568, 30.1867, REGULAR}, //Maisonz
		{-2306.7834, 578.7684, 33.6096, REGULAR}, //Maisonz
		{-2289.9077, 581.4578, 35.1671, REGULAR}, //Maisonz
		{-2374.0615, 691.2832, 35.1640, REGULAR}, //Maisonz
		{-2371.2756, 709.5670, 35.1640, REGULAR}, //Maison
		{-2370.2678, 721.7906, 36.0786, REGULAR}, //Maison
		{-2370.6088, 741.6833, 35.0382, REGULAR}, //Maison
		{-2371.5595, 750.9750, 34.9775, REGULAR}, //Maison
		{-2432.4797, 815.0660, 37.5051, REGULAR}, //Maison
		{-2486.3815, 820.3829, 38.3828, REGULAR}, //Maison
		{-2502.0935, 820.9833, 44.6532, REGULAR}, //Maison
		{-2514.9797, 830.7453, 50.0002, REGULAR}, //Maison
		{-2724.2880, 816.4866, 52.6209, REGULAR}, //Maison
		{-2739.7280, 856.9833, 62.9952, REGULAR}, //Maison
		{-2898.8403, 1168.6477, 13.0184, REGULAR}, //Maison
		{-2902.8598, 1154.3477, 13.6815, REGULAR}, //Maison
		{-2903.4057, 1123.3569, 27.0703, REGULAR}, //Maison
		{-2903.4643, 1109.9265, 27.0703, REGULAR}, //Maison
		{-2896.4790, 1102.4160, 27.2414, REGULAR}, //Maison
		{-2909.8156, 1081.3466, 32.1328, REGULAR}, //Maison
		{-2898.2858, 1071.5186, 32.0790, REGULAR}, //Maison
		{-2896.7333, 1056.7794, 32.2775, REGULAR}, //Maison
		{-2902.0378, 1038.0783, 36.8281, REGULAR}, //Maison
		{-2895.4553, 1029.2760, 36.3343, REGULAR}, //Maison
		{-2895.0126, 1019.5330, 36.8281, REGULAR}, //Maison
		{-2887.1657, 1002.8215, 40.7187, REGULAR}, //Maison
		{-2863.9753, 983.8668, 41.6673, REGULAR}, //Maison
		{-2867.1069, 949.2637, 43.9140, REGULAR}, //Maison
		{-2853.2858, 954.1895, 44.0546, REGULAR}, //Maison
		{-2841.6496, 912.7076, 44.0546, REGULAR}, //Maison
		{-2836.8996, 875.2344, 44.0546, REGULAR}, //Maison
		{-2836.2165, 863.0886, 44.0546, REGULAR}, //Maison
		{-2861.8823, 829.6601, 39.6564, REGULAR}, //Maison
		{-2883.4008, 750.7986, 29.2994, REGULAR}, //Maison
		{-2865.2839, 687.2294, 23.4608, REGULAR}, //Maison
		{-2797.5512, 442.2831, 4.4371, REGULAR}, //Maison
		{-2787.3637, 435.3139, 4.4926, REGULAR}, //Maison
		{-2769.1657, 430.5534, 4.4861, REGULAR}, //Maison
		{-2716.8242, 395.8679, 4.6640, REGULAR}, //Maison
		{-2706.0021, 376.1159, 4.9684, REGULAR}, //Maison
		{-2717.8471, 331.2223, 4.1796, REGULAR}, //Maison
		{-2712.0659, 301.1044, 4.3359, REGULAR}, //Maison
		{-2728.7517, 280.3197, 4.6178, REGULAR}, //Maison
		{-2773.4609, 281.0961, 6.4450, REGULAR}, //Maison
		{-2800.6782, 259.9761, 7.1796, REGULAR}, //Maison
		{-2798.2148, 226.8805, 7.1384, REGULAR}, //Maison
		{-2797.5109, 213.7274, 7.1875, REGULAR}, //Maison
		{-2791.9343, 189.6150, 7.8515, REGULAR}, //Maison
		{-2783.3139, 170.5713, 7.8051, REGULAR}, //Maison
		{-2770.4450, 186.9083, 7.1951, REGULAR}, //Maison
		{-2774.1870, 194.5335, 7.1875, REGULAR}, //Maison
		{-2768.1970, 219.1173, 7.1796, REGULAR}, //Maison
		{-2750.9455, 202.3003, 7.0349, REGULAR}, //Maison
		{-2772.4089, 144.6954, 7.1875, REGULAR}, //Maison
		{-2778.7758, 134.9367, 7.1865, REGULAR}, //Maison
		{-2773.4321, 123.5930, 7.1883, REGULAR}, //Maison
		{-2760.8532, 86.5671, 6.9529, REGULAR}, //Maison
		{-2751.8601, 76.4471, 7.1875, REGULAR}, //Maison
		{-2734.1423, 77.8169, 4.3359, REGULAR}, //Maison
		{-2712.9538, 97.4571, 4.3359, REGULAR}, //Maison
		{-2694.7189, 100.9928, 4.3359, REGULAR}, //Maison
		{-2689.4665, 112.9462, 4.0859, REGULAR}, //Maison
		{-2693.4333, 123.9857, 4.3435, REGULAR}, //Maison
		{-2690.2053, 173.2763, 4.3359, REGULAR}, //Maison
		{-2659.7937, 179.2549, 4.3365, REGULAR}, //Maison
		{-2641.7324, 183.5119, 4.3280, REGULAR}, //Maison
		{-2601.3298, 137.3499, 4.1796, REGULAR}, //Maison
		{-2616.6647, 124.6866, 4.3436, REGULAR}, //Maison
		{-2616.2578, 100.8078, 4.3359, REGULAR}, //Maison
		{-2615.6862, 54.4731, 4.3359, REGULAR}, //Maison
		{-2638.8093, 12.4940, 6.1328, REGULAR}, //Maison
		{-2630.6687, 1.5096, 6.1328, REGULAR}, //Maison
		{-2624.9462, -9.4207, 6.1328, REGULAR}, //Maison
		{-2644.3271, -31.7352, 6.1328, REGULAR}, //Maison
		{-2718.3435, -39.4210, 4.3432, REGULAR}, //Maison
		{-2717.8479, -54.7452, 4.3359, REGULAR}, //Maison
		{-2694.1347, -86.4137, 4.3359, REGULAR}, //Maison
		{-2717.1901, -104.1180, 4.3359, REGULAR}, //Maison
		{-2717.8374, -111.3447, 4.3359, REGULAR}, //Maison
		{-2715.3825, -128.0242, 4.3281, REGULAR}, //Maison
		{-2693.8786, -140.7095, 4.3359, REGULAR}, //Maison
		{-2694.7692, -178.3211, 4.3359, REGULAR}, //Maison
		{-2617.7128, -192.2372, 4.3434, REGULAR}, //Maison
		{-2613.6818, -177.3645, 4.3359, REGULAR}, //Maison
		{-2596.4360, -160.6422, 4.3281, REGULAR}, //Maison
		{-2578.4025, -130.6094, 5.9125, REGULAR}, //Maison
		{-2499.3874, -113.7374, 25.4705, REGULAR}, //Maison
		{-2495.5434, -103.8636, 25.6171, REGULAR}, //Maison
		{-2507.1594, -87.9667, 25.6171, REGULAR}, //Maison
		{-2510.6018, -63.2391, 25.6171, REGULAR}, //Maison
		{-2495.0231, -47.3462, 25.7029, REGULAR}, //Maison
		{-2511.9792, -27.1014, 25.6171, REGULAR}, //Maison
		{-2470.2121, -76.9601, 30.8847, REGULAR}, //Maison
		{-2447.4123, -89.2713, 34.1460, REGULAR}, //Maison
		{-2433.3908, -63.9944, 35.1561, REGULAR}, //Maison
		{-2358.2885, -44.3625, 35.3125, REGULAR}, //Maison
		{-2355.9528, -19.9568, 35.3125, REGULAR}, //Maison
		{-2347.7407, 9.4034, 35.3125, REGULAR}, //Maison
		{-2366.1406, 49.4937, 35.3125, REGULAR}, //Maison
		{-2400.9621, 63.4246, 35.2232, REGULAR}, //Maison
		{-2424.8186, 72.9899, 35.0234, REGULAR}, //Maison
		{-2455.7846, 101.7436, 35.1718, REGULAR}, //Maison
		{-2435.9729, 134.2740, 34.9531, REGULAR}, //Maison
		{-2452.5397, 131.1261, 35.1718, REGULAR}, //Maison
		{-2440.2485, 158.5914, 35.1328, REGULAR}, //Maison
		{-2416.0090, 315.7755, 35.1718, REGULAR}, //Maison
		{-2421.9797, 331.6991, 35.6273, REGULAR}, //Maison
		{-2242.2988, 187.2147, 35.3203, REGULAR}, //Maison
		{-2247.4426, 161.8136, 35.1718, REGULAR}, //Maison
		{-2243.0668, 141.1347, 35.3203, REGULAR}, //Maison
		{-2239.6608, 115.2163, 35.3203, REGULAR}, //Maison
		{-2275.5783, 42.6430, 35.3125, REGULAR}, //Maison
		{-2263.6398, 32.6824, 35.3203, REGULAR}, //Maison
		{-2266.2844, 6.1929, 35.3203, REGULAR}, //Maison
		{-2274.0878, -1.0849, 35.3203, REGULAR}, //Maison
		{-2268.1267, -18.1643, 35.3203, REGULAR}, //Maison
		{-2269.1936, -37.9544, 35.3203, REGULAR}, //Maison
		{-2244.9057, -53.5425, 35.3203, REGULAR}, //Maison
		{-2238.9291, -62.3484, 35.3203, REGULAR}, //Maison
		{-2231.7883, -76.4996, 35.3203, REGULAR}, //Maison
		{-2206.9562, -77.2534, 35.3203, REGULAR} //Maison
	};
	x = aRandomItemPos[spawnid][xPos];
	y = aRandomItemPos[spawnid][yPos];
	z = aRandomItemPos[spawnid][zPos];
	neutral = aRandomItemPos[spawnid][dNeutral];
	gun = aRandomItemPos[spawnid][dGun];
	vehicle = aRandomItemPos[spawnid][dVehicle];
	medic = aRandomItemPos[spawnid][dMedic];
	clothe = aRandomItemPos[spawnid][dClothe];
	bag = aRandomItemPos[spawnid][dBag];
	level = aRandomItemPos[spawnid][dLevel];
}
forward InitializeSpawnObject(spawnid, Pointer:objectid);
public InitializeSpawnObject(spawnid, Pointer:objectid)
{
	if(spawnid >= 0 && spawnid < MAX_ITEM_SPAWNS)
	{
		if(objectid != MEM_NULLPTR)
		{
			dSpawnObjects[spawnid] = objectid;
			dItemsHistory[dHistory] = spawnid;
			if(++ dHistory >= MAX_SPAWNED_ITEMS) dHistory = 0;
		}
		else dSpawnObjects[spawnid] = MEM_NULLPTR;
	}
}

public CreateRandomItem()
{
	static dReplace = 0;

	new dDestroyed = -1;
	new dSpawned = CallRemoteFunction("GetSpawnedObjects", "");
	if(dSpawned >= MAX_SPAWNED_ITEMS)
	{
		/*printf("%d objects spawned", dSpawned);
		for(new i = 0; i < MAX_SPAWNED_ITEMS; i++)
		{
			printf("dItemsHistory[%d] = %d", i, dItemsHistory[i]);
		}*/
	    //printf("1");
		do
		{
			if(++ dReplace >= MAX_SPAWNED_ITEMS) dReplace = 0;
		}
	    while(dItemsHistory[dReplace] == -1);
	    //---
	    if(dSpawnObjects[dItemsHistory[dReplace]] != MEM_NULLPTR)
			CallRemoteFunction("DestroyItem", "x", _:dSpawnObjects[dItemsHistory[dReplace]]);

	    dSpawnObjects[dItemsHistory[dReplace]] = MEM_NULLPTR;
	    dItemsHistory[dReplace] = -1;
		if(++ dReplace >= MAX_SPAWNED_ITEMS) dReplace = 0;
	    dDestroyed = dReplace;
	}
	//---
	//printf("2");
	new dSlotID = -1;//Variable pour trouver un slot où y'a po d'objet
	for(new i = 0; i < MAX_ITEM_SPAWNS; i ++)
	{
		if(dSpawnObjects[i] != MEM_NULLPTR && CallRemoteFunction("GetObjectID", "x", _:dSpawnObjects[i]) == 0) dSpawnObjects[i] = MEM_NULLPTR;
		if(dSpawnObjects[i] == MEM_NULLPTR)
		{
			dSlotID = i;//Si on a trouvé au moins une position libre, c'bon
			break;
		}
	}
	//printf("3");
	//---
	if(dSlotID == -1) return 1;//Si on a pas trouvé de position libre, on interrompt*/
	//---
	//printf("4");
    new dRandomPos;
 	do dRandomPos = random(MAX_ITEM_SPAWNS);//On prend des positions au hasard...
	while(dSpawnObjects[dRandomPos] != MEM_NULLPTR);//Jusqu'à ce qu'on en trouve une qui soit libre là !
	//---
	//printf("5");
	new Float:x, Float:y, Float:z, neutral, gun, vehicle, medic, clothe, bag, level;
	GetItemSpawnPos(dRandomPos, x, y, z, neutral, gun, vehicle, medic, clothe, bag, level);
	/*if(Pointer:CallRemoteFunction("GetItemWithinDistance", "ffff", x, y, z, 3.0) != MEM_NULLPTR)
		return 1;*/
    new dItemID = PickRandomItem(neutral, gun, vehicle, medic, clothe, bag, level);
	//printf("6");
	new Pointer:pt = Pointer:CallRemoteFunction("CreateItem", "dfffbddd", dItemID, x, y, z, true, -1, CallRemoteFunction("GetObjectDefaultExtraVal", "d", dItemID), dRandomPos);
	dSpawnObjects[dRandomPos] = pt;
	//---
	//printf("7");
	if(dDestroyed == -1)
	{
		dItemsHistory[dHistory] = dRandomPos;
		dHistory ++;
	}
	else
	{
		dItemsHistory[dDestroyed] = dRandomPos;
	}
	return 1;
}

public PickRandomItem(neutral, gun, vehicle, medic, clothes, bag, level)//Fonction pour choisir un item à spawn en fonction de l'endroit
{
	#if defined ADVANCED_ITEM_SPAWN
	new dChances[100] = {-1, ...};
	for(new i = 0; i < neutral; i ++)
	{
	    new dRand = 0;
		do dRand = random(100);
		while(dChances[dRand] != -1);
		dChances[dRand] = NEUTRAL_SPAWN;
	}
	//---
	for(new i = 0; i < gun; i ++)
	{
	    new dRand = 0;
		do dRand = random(100);
		while(dChances[dRand] != -1);
		dChances[dRand] = GUN_SPAWN;
	}
	//---
	for(new i = 0; i < vehicle; i ++)
	{
	    new dRand = 0;
		do dRand = random(100);
		while(dChances[dRand] != -1);
		dChances[dRand] = VEHICLE_SPAWN;
	}
	//---
	for(new i = 0; i < medic; i ++)
	{
	    new dRand = 0;
		do dRand = random(100);
		while(dChances[dRand] != -1);
		dChances[dRand] = MEDIC_SPAWN;
	}
	//---
	for(new i = 0; i < clothes; i ++)
	{
	    new dRand = 0;
		do dRand = random(100);
		while(dChances[dRand] != -1);
		dChances[dRand] = CLOTHE_SPAWN;
	}
	//---
	for(new i = 0; i < bag; i ++)
	{
	    new dRand = 0;
		do dRand = random(100);
		while(dChances[dRand] != -1);
		dChances[dRand] = BAG_SPAWN;
	}
	return GetRandomItem(dChances[random(100)], level);

	#else

	new dRandomCategory = random(100);
	if(neutral > dRandomCategory >= 0) return GetRandomItem(NEUTRAL_SPAWN, level);
	else if(neutral + gun > dRandomCategory >= neutral) return GetRandomItem(GUN_SPAWN, level);
	else if(neutral + gun + vehicle > dRandomCategory >= neutral + gun) return GetRandomItem(VEHICLE_SPAWN, level);
	else if(neutral + gun + vehicle + medic > dRandomCategory >= neutral + gun + vehicle) return GetRandomItem(MEDIC_SPAWN, level);
	else if(neutral + gun + vehicle + medic + clothes > dRandomCategory >= neutral + gun + vehicle + medic) return GetRandomItem(CLOTHE_SPAWN, level);
	else if(neutral + gun + vehicle + medic + clothes + bag > dRandomCategory >= neutral + gun + vehicle + medic + clothes) return GetRandomItem(BAG_SPAWN, level);

	#endif

	return 0;
}

GetRandomItem(category, level)
{
	switch(category)
	{
	    case NEUTRAL_SPAWN:
	    {
			if(random(100) > 30)//Bouffe
			{
			    if(level == LEVEL_1)
			    {
			        new dItemList[] =
			        {
			            66, 67, 70, 72, 73, 74, 81, 110
			        };
		        	return dItemList[random(sizeof(dItemList))];
	        	}
			    else if(level >= LEVEL_2)
			    {
			        new dItemList[] =
			        {
			            109, 118, 119, 120, 121
			        };
		        	return dItemList[random(sizeof(dItemList))];
	        	}
			}
			else//Autres objets normaux
			{
			    if(level == LEVEL_1)
			    {
			        new dItemList[] =
			        {
               			1, 31, 40, 71, 80, 91, 103, 107, 113, 115
			        };
		        	return dItemList[random(sizeof(dItemList))];
				}
			}
	    }
	    case GUN_SPAWN:
	    {
	        if(random(100) > 50)//Armes
	        {
			    if(level == LEVEL_1)
			    {
			        new dItemList[] =
			        {
			            3, 4, 5, 8, 9, 10, 11, 14, 20, 21, 23
			        };
			        return dItemList[random(sizeof(dItemList))];
				}
			    else if(level == LEVEL_2)
			    {
			        new dItemList[] =
			        {
			            6, 7, 12, 13, 16, 18, 22, 133
			        };
			        return dItemList[random(sizeof(dItemList))];
				}
			    else if(level == LEVEL_3)
			    {
			        new dItemList[] =
			        {
			            12, 15, 19, 133, 134
			        };
			        return dItemList[random(sizeof(dItemList))];
				}
			    else if(level == LEVEL_4)
			    {
			        new dItemList[] =
			        {
			            12, 15, 19, 24, 134
			        };
			        return dItemList[random(sizeof(dItemList))];
				}
			}
			else //Munitions
			{
			    new dItemList[] =
				{
					25, 26, 27, 28, 29
				};
			    return dItemList[random(sizeof(dItemList))];
			}
	    }
		case VEHICLE_SPAWN:
		{
	        if(level == LEVEL_1)
			{
			    new dItemList[] =
		        {
		            8, 30, 31, 63, 64
		        };
		        return dItemList[random(sizeof(dItemList))];
			}
		}
		case MEDIC_SPAWN:
		{
	        if(level == LEVEL_1)
			{
   				new dItemList[] =
		        {
		            36, 37, 38, 39
		        };
		        return dItemList[random(sizeof(dItemList))];
			}
	        else if(level >= LEVEL_2)
			{
   				new dItemList[] =
		        {
		            2, 36, 37, 38, 39
		        };
		        return dItemList[random(sizeof(dItemList))];
			}
		}
		case CLOTHE_SPAWN:
		{
	        if(level >= LEVEL_1)
			{
   				new dItemList[] =
		        {
		            41, 42, 43, 44, 45, 46, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 65, 75, 76, 77, 78, 79, 85, 101
		        };
		        return dItemList[random(sizeof(dItemList))];
			}
		}
		case BAG_SPAWN:
		{
	        if(level == LEVEL_1)
			{
		        new dItemList[] =
		        {
		        	32, 34, 34, 32, 33, 32, 33, 32, 33, 35, 32, 33, 32
		        };
		        return dItemList[random(sizeof(dItemList))];
			}
	        else if(level == LEVEL_2)
			{
		        new dItemList[] =
		        {
		        	33, 34, 34, 33, 32, 33, 32, 33, 35, 35, 33, 32, 33
		        };
		        return dItemList[random(sizeof(dItemList))];
			}
	        else if(level == LEVEL_3)
			{
		        new dItemList[] =
		        {
		        	33, 35, 35, 34, 34, 33, 34, 33, 34, 35, 33, 34, 33
		        };
		        return dItemList[random(sizeof(dItemList))];
			}
	        else if(level == LEVEL_4)//Différentes catégories d'objets à trouver
			{
		        new dItemList[] =
		        {
		        	34, 33, 33, 34, 35, 35, 34, 35, 34, 33, 34, 34, 35
		        };
		        return dItemList[random(sizeof(dItemList))];
			}
		}
	}
	return 0;
}
