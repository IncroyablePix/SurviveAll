/*-------------------------------------------------------------------------------//
\\  - Acteurs statiques du serveur -
//
\\  	Fonctions globales:
//          GetPlayerNearCart(playerid)
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
#include <[SA]Defines.inc>
#include <[SA]Functions.inc>
#include <streamer.inc>
#undef PROFILING
#if defined PROFILING
#include <profiler.inc>
#endif

//---FONCTIONS
forward ClosePlayerShop(playerid);
forward ShowPlayerShop(playerid, shopid);
forward BuyPlayerItem(playerid);
forward GetPlayerShop(playerid);
forward NextShopItemForPlayer(playerid, bool:next);
forward GetPlayerNearShop(playerid);
forward GetPlayerNearCart(playerid);
forward GetPlayerNearAuctionHouse(playerid);

//---DEFINES
#define MAX_HOTDOGS                         (20)

//---MACROS

//---ENUMS
enum HotDog
{
	bool:bActive,
	aVendor,
	oCart,
	Float:xDog,
	Float:yDog,
	Float:zDog,
	Float:aDog
}

//---VARIABLES
new dHotDogStand[MAX_HOTDOGS][HotDog];
new dShop[MAX_PLAYERS][2];

//------------------------------------------------//

public OnFilterScriptInit()
{
	print("--------------------------------------");
	print("Survive-All shops - [Pix]");
	print("- Loaded");
	print("...");
	//------//
	for(new i = 0; i < MAX_PLAYERS; i ++)
	{
		dShop[i][0] = -1;
	}
	//---
	AddHotDogCart(0, 15.8002, -250.4329, 5.4296, 89.1784);
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
	print("Survive-All shops - [Pix]");
	print("- Unloaded");
	print("--------------------------------------\n");
	//------//
	for(new i = 0; i < MAX_HOTDOGS; i ++) DestroyHotDogCart(i);
	//------------------------//
	#if defined PROFILING
 	Profiler_Stop();
	#endif
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if(!IsPlayerNPC(playerid))
	{
		//---MAGASINS
		if(dShop[playerid][0] != -1)
		{
			dShop[playerid][0] = -1;
			dShop[playerid][1] = 0;
		}
	}
}

public GetPlayerNearCart(playerid)
{
	for(new i = 0; i < MAX_HOTDOGS; i ++) if(dHotDogStand[i][bActive] && IsPlayerInRangeOfPoint(playerid, 3.5, dHotDogStand[i][xDog], dHotDogStand[i][yDog], dHotDogStand[i][zDog])) return i;
	return -1;
}

public GetPlayerNearShop(playerid)
{
	if(IsPlayerInRangeOfPoint(playerid, 3.5, 1056.4539, -313.311, 73.9919)) return 0;//Boutique Hilltop Farm
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, -1954.3423, -2445.1948, 30.6870)) return 1;//Boutique Angel Pine
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, -1374.931, -2206.3049, 50.7849)) return 2;//Boutique dans les arbres
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, -1321.8129, 2504.871, 89.569)) return 3;//Boutique Aldea Malvada
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, 991.2984, 2442.5554, 10.8341)) return 4;//Boutique LV Nord
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, 2453.593, -1647.9379, 13.4739)) return 5;//Boutique Grove Street
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, 2.5493, -291.0689, 5.4297)) return 6;//Boutique Blueberry
	else if(IsPlayerInRangeOfPoint(playerid, 3.5, -2081.8059, 836.1129, 69.563)) return 7;//Boutique SF Residential
	else if(IsPlayerInRangeOfPoint(playerid, 3.5,-1638.2559, 1023.744, 69.828)) return 8;//Boutique SF Roofs
	return -1;
}

AddHotDogCart(cartid, Float:x, Float:y, Float:z, Float:angle)
{
	if(dHotDogStand[cartid][bActive]) return false;
	dHotDogStand[cartid][bActive] = true;
	dHotDogStand[cartid][aVendor] = CallRemoteFunction("CreateActorEx", "iffff", 168, x, y, z, angle);
    dHotDogStand[cartid][oCart] = CreateDynamicObject(1340, x + floatsin(-angle, degrees), y + floatcos(-angle, degrees), z + 0.1, 0.0, 0.0, angle + 90.0);
    dHotDogStand[cartid][xDog] = x;
    dHotDogStand[cartid][yDog] = y;
    dHotDogStand[cartid][zDog] = z;
    dHotDogStand[cartid][aDog] = angle;
    return true;
}

DestroyHotDogCart(cartid)
{
	if(!dHotDogStand[cartid][bActive]) return false;
    dHotDogStand[cartid][bActive] = false;
	DestroyDynamicObject(dHotDogStand[cartid][oCart]);
    dHotDogStand[cartid][oCart] = INVALID_OBJECT_ID;
	DestroyActor(dHotDogStand[cartid][aVendor]);
	dHotDogStand[cartid][aVendor] = INVALID_ACTOR_ID;
    dHotDogStand[cartid][xDog] = 0.0;
    dHotDogStand[cartid][yDog] = 0.0;
    dHotDogStand[cartid][zDog] = 0.0;
    dHotDogStand[cartid][aDog] = 0.0;
    return true;
}

GetShopItemPrice(shopid, itemid)
{
	switch(shopid)
	{
	    case 0://Hilltop Farm
	    {
	        switch(itemid)
	        {
	            case 1: return 15;//.222
	            case 2: return 10;//Couteau
	            case 3: return 70;//Coyote Pack
	            case 4: return 50;//Armure
	            case 5: return 10;//Wayfarers noires
	            case 6: return 20;//Casque militaire
	        }
		}
	    case 1://Camp Angel Pine
	    {
	        switch(itemid)
	        {
	            case 1: return 3;//Lait
	            case 2: return 250;//Canne à pêche
	            case 3: return 50;//Tronçonneuse
	            case 4: return 10;//9mm
	            case 5: return 3;//Bouteille vide
	            case 6: return 3;//Tomates
	        }
		}
	    case 2://Boutique du village des arbres
	    {
	        switch(itemid)
	        {
	            case 1: return 8;//Cagoule
	            case 2: return 20;//12 Gauge
	            case 3: return 40;//Uzi
	            case 4: return 45;//Alice Pack
	            case 5: return 15;//Bérêt
	            case 6: return 100;//Clé anglaise
	            case 7: return 45;//Lit miteux
	        }
		}
	    case 3://Boutique Aldea Malvada
	    {
	        switch(itemid)
	        {
	            case 1: return 15;//Poudre
	            case 2: return 20;//7.62
	            case 3: return 50;//Tente
	            case 4: return 40;//Country Rifle
	            case 5: return 10;//Aviators noires
	            case 6: return 20;//Masque à gaz
	        }
		}
	    case 4://Boutique LV Nord
	    {
	        switch(itemid)
	        {
	            case 1: return 15;//.50AE
	            case 2: return 40;//AK47
	            case 3: return 95;//Casserole
	            case 4: return 8;//Caféine
	            case 5: return 10;//Roue
	            case 6: return 35;//Chapeau de sorcière
	        }
		}
  		case 5://Boutique Grove Street
		{
		    switch(itemid)
		    {
		        case 1: return 20;//12 Gauge
		        case 2: return 25;//Batte
		        case 3: return 15;//.222
		        case 4: return 20;//7.62
		        case 5: return 50;//Desert Eagle
		        case 6: return 10;//Roue
		    }
		}
  		case 6://Boutique Blueberry
		{
		    switch(itemid)
		    {
		        case 1: return 15;//9mm
		        case 2: return 100;//Casserole
		        case 3: return 4;//Lait
		        case 4: return 15;//Sawn Off
		        case 5: return 60;//Lit miteux
		    }
		}
  		case 7://SF Residential
		{
		    switch(itemid)
		    {
		        case 1: return 60;//Raincoat
		        case 2: return 18;//Soupe
		        case 3: return 10;//Lait
		        case 4: return 20;//12 Gauge
		        case 5: return 10;//Molotov
		    }
		}
  		case 8://SF Roofs
		{
		    switch(itemid)
		    {
		        case 1: return 20;//Masque à gaz
		        case 2: return 100;//Coffre Fort
		        case 3: return 35;//Lampe
		        case 4: return 15;//Medikit
		        case 5: return 20;//Bombe
		        case 6: return 30;//Lit miteux
		    }
		}
	}
	return 0;
}

GetShopItemID(shopid, itemid)
{
	switch(shopid)
	{
	    case 0://Hilltop Farm
	    {
	        switch(itemid)
	        {
	            case 1: return 29;//.222
	            case 2: return 4;//Couteau
	            case 3: return 35;//Coyote Pack
	            case 4: return 83;//Armure
	            case 5: return 62;//Wayfarers noires
	            case 6: return 47;//Casque militaire
	        }
		}
	    case 1://Camp Angel Pine
	    {
	        switch(itemid)
	        {
	            case 1: return 70;//Lait
	            case 2: return 90;//Canne à pêche
	            case 3: return 7;//Tronçonneuse
	            case 4: return 26;//9mm
	            case 5: return 40;//Bouteille vide
	            case 6: return 74;//Tomates
	        }
		}
	    case 2://Boutique du village des arbres
	    {
	        switch(itemid)
	        {
	            case 1: return 101;//Cagoule
	            case 2: return 28;//12 Gauge
	            case 3: return 16;//Uzi
	            case 4: return 34;//Alice Pack
	            case 5: return 46;//Bérêt
	            case 6: return 96;//Clé anglaise
	            case 7: return 80;//Lit miteux
	        }
		}
	    case 3://Boutique Aldea Malvada
	    {
	        switch(itemid)
	        {
	            case 1: return 107;//Poudre
	            case 2: return 25;//7.62
	            case 3: return 1;//Tente
	            case 4: return 21;//Country Rifle
	            case 5: return 56;//Aviators noires
	            case 6: return 65;//Masque à gaz
	        }
		}
	    case 4://Boutique LV Nord
	    {
	        switch(itemid)
	        {
	            case 1: return 27;//.50AE
	            case 2: return 18;//AK47
	            case 3: return 102;//Casserole
	            case 4: return 39;//Caféine
	            case 5: return 63;//Roue
	            case 6: return 45;//Chapeau de sorcière
	        }
		}
		case 5://Boutique Grove Street
		{
		    switch(itemid)
		    {
		        case 1: return 28;//12 Gauge
		        case 2: return 5;//Batte
		        case 3: return 29;//.222
		        case 4: return 25;//7.62
		        case 5: return 12;//Desert Eagle
		        case 6: return 63;//Roue
		    }
		}
  		case 6://Boutique Blueberry
		{
		    switch(itemid)
		    {
		        case 1: return 26;//9mm
		        case 2: return 102;//Casserole
		        case 3: return 70;//Lait
		        case 4: return 4;//Sawn Off
		        case 5: return 80;//Lit miteux
		    }
		}
  		case 7://SF Residential
		{
		    switch(itemid)
		    {
		        case 1: return 135;//Raincoat
		        case 2: return 120;//Soupe
		        case 3: return 70;//Lait
		        case 4: return 28;//12 Gauge
		        case 5: return 9;//Molotov
		    }
		}
  		case 8://SF Roofs
		{
		    switch(itemid)
		    {
		        case 1: return 65;//Masque à gaz
		        case 2: return 97;//Coffre Fort
		        case 3: return 137;//Lampe
		        case 4: return 2;//Medikit
		        case 5: return 100;//Bombe
		        case 6: return 80;//Lit miteux
		    }
		}
	}
	return 0;
}

GetShopItemCameraInfos(shopid, itemid, &Float:x, &Float:y, &Float:z, &Float:fx, &Float:fy, &Float:fz)
{
	switch(shopid)
	{
	    case 0://Hilltop Farm
	    {
	        switch(itemid)
	        {
	            case 1://.222
	            {
	                x = 1055.2969;
	                y = -314.5769;
	                z = 73.8649;
	                fx = 1056.2482;
	                fy = -314.5884;
	                fz = 73.9083;
	            }
	            case 2://Couteau
	            {
	                x = 1055.404;
	                y = -314.127;
	                z = 74.0609;
	                fx = 1056.0025;
	                fy = -314.1476;
	                fz = 74.2308;
	            }
	            case 3://Coyote Pack
	            {
	                x = 1055.233;
	                y = -313.683;
	                z = 73.7379;
	                fx = 1056.0336;
	                fy = -313.5988;
	                fz = 74.2308;
	            }
	            case 4://Armure
	            {
	                x = 1055.357;
	                y = -312.9179;
	                z = 74.083;
	                fx = 1056.132;
	                fy = -312.7896;
	                fz = 74.4102;
	            }
	            case 5://Wayfarer noires
	            {
	                x = 1055.2769;
	                y = -312.44;
	                z = 73.8509;
	                fx = 1056.046;
	                fy = -312.3808;
	                fz = 74.151;
	            }
	            case 6://Casque militaire
	            {
	                x = 1055.3339;
	                y = -312.036;
	                z = 73.913;
	                fx = 1056.1021;
	                fy = -311.9626;
	                fz = 74.1432;
	            }
	        }
	    }
	    case 1://Camp Angel Pine
	    {
	        switch(itemid)
	        {
	            case 1://Lait
	            {
	                x = -1953.6562;
	                y = -2443.6865;
	                z = 30.4309;
	                fx = -1954.1778;
	                fy = -2444.5832;
	                fz = 31.0176;
	            }
	            case 2://Canne à pêche
	            {
	                x = -1952.8769;
	                y = -2443.3808;
	                z = 29.75;
	                fx = -1952.5589;
	                fy = -2445.2211;
	                fz = 31.2476;
	            }
	            case 3://Tronçonneuse
	            {
	                x = -1951.5751;
	                y = -2443.3886;
	                z = 29.9009;
	                fx = -1952.4816;
	                fy = -2443.958;
	                fz = 30.1258;
	            }
	            case 4://9mm
	            {
	                x = -1950.6279;
	                y = -2444.3564;
	                z = 29.8339;
	                fx = -1951.7137;
	                fy = -2444.5092;
	                fz = 30.1258;
	            }
	            case 5://Bouteille vide
	            {
	                x = -1950.6503;
	                y = -2445.2636;
	                z = 29.8649;
	                fx = -1951.3374;
	                fy = -2445.4023;
	                fz = 30.0688;
	            }
	            case 6://Tomates
	            {
	                x = -1950.0976;
	                y = -2447.1416;
	                z = 29.843;
	                fx = -1950.5902;
	                fy = -2447.1373;
	                fz = 29.9565;
	            }
	        }
	    }
	    case 2://Village dans les arbres
	    {
	        switch(itemid)
	        {
	            case 1://Cagoule
	            {
	                x = -1379.7519;
	                y = -2204.0908;
	                z = 52.3349;
	                fx = -1379.7711;
					fy = -2204.915;
					fz = 52.6113;
	            }
	            case 2://12 Gauge
	            {
	                x = -1379.0292;
	                y = -2205.3769;
	                z = 52.4259;
	                fx = -1379.359;
					fy = -2206.1464;
					fz = 52.6295;
	            }
	            case 3://Uzi
	            {
	                x = -1378.49218;
	                y = -2206.5322;
	                z = 52.471;
	                fx = -1378.5438;
					fy = -2207.3894;
					fz = 52.8524;
	            }
	            case 4://Alice Pack
	            {
	                x = -1376.8544;
	                y = -2206.2578;
	                z = 52.3989;
	                fx = -1376.864;
					fy = -2207.2871;
					fz = 53.2086;
	            }
	            case 5://Bérêt
	            {
	                x = -1375.5507;
	                y = -2205.5107;
	                z = 52.339;
	                fx = -1375.2498;
					fy = -2206.1547;
					fz = 52.4836;
	            }
	            case 6://Clé anglaise
	            {
	                x = -1374.2558;
	                y = -2204.5937;
	                z = 52.227;
	                fx = -1374.057;
					fy = -2204.8654;
					fz = 52.924;
	            }
	            case 7://Lit miteux
	            {
	                x = -1374.222;
	                y = -2202.0161;
	                z = 52.1549;
	                fx = -1373.9699;
					fy = -2203.8747;
					fz = 53.3647;
	            }
	        }
		}
	    case 3://Aldea Malvada
	    {
	        switch(itemid)
	        {
	            case 1://Poudre à canon
	            {
	                x = -1323.3979;
	                y = 2507.224;
	                z = 89.049;
	                fx = -1323.3881;
					fy = 2506.476;
					fz = 89.4395;
	            }
	            case 2://7.62
	            {
	                x = -1323.4859;
	                y = 2506.164;
	                z = 89.1699;
	                fx = -1323.7808;
					fy = 2505.5561;
					fz = 89.7694;
	            }
	            case 3://Tente
	            {
	                x = -1322.1259;
	                y = 2506.0991;
	                z = 89.1529;
	                fx = -1322.1571;
					fy = 2505.3117;
					fz = 89.6466;
	            }
	            case 4://Sniper
	            {
	                x = -1320.2569;
	                y = 2505.832;
	                z = 89.0449;
	                fx = -1320.3504;
					fy = 2505.2463;
					fz = 89.7023;
	            }
	            case 5://Aviators noires
	            {
	                x = -1320.1910;
	                y = 2505.083;
	                z = 89.106;
	                fx = -1321.1507;
					fy = 2505.0136;
					fz = 89.1564;
	            }
	            case 6://Masque à gaz
	            {
	                x = -1320.176;
	                y = 2504.5139;
	                z = 89.1389;
	                fx = -1320.7241;
					fy = 2504.2578;
					fz = 89.4719;
	            }
	        }
	    }
	    case 4://Boutique LV Nord
	    {
	        switch(itemid)
	        {
	            case 1://.50AE
	            {
	                x = 992.21997;
	                y = 2440.2929;
	                z = 11.237;
	                fx = 992.07;
					fy = 2441.8613;
					fz = 11.5478;
	            }
	            case 2://AK47
	            {
	                x = 991.1149;
	                y = 2440.1708;
	                z = 11.284;
	                fx = 991.0966;
					fy = 2441.8864;
					fz = 11.6358;
	            }
	            case 3://Casserole
	            {
	                x = 991.6572;
	                y = 2441.6514;
	                z = 10.346;
	                fx = 991.6754;
					fy = 2442.8273;
					fz = 11.1745;
	            }
	            case 4://Caféine
	            {
	                x = 991.0595;
	                y = 2441.5896;
	                z = 10.413;
	                fx = 991.1262;
					fy = 2442.8627;
					fz = 11.1744;
	            }
	            case 5://Roue
	            {
	                x = 989.5590;
	                y = 2440.8068;
	                z = 10.3339;
	                fx = 991.098;
					fy = 2441.5827;
					fz = 11.0057;
	            }
	            case 6://Chapeau de sorcière
	            {
	                x = 989.6191;
	                y = 2443.9404;
	                z = 10.3409;
	                fx = 990.656;
					fy = 2445.189;
					fz = 10.9727;
	            }
	        }
		}
	    case 5://Boutique Grove Street
	    {
	        switch(itemid)
	        {
	            case 1://12 Gauge
	            {
	                x = 2452.298;
	                y = -1646.676;
	                z = 13.489;
	                fx = 2452.3513;
					fy = -1647.6901;
					fz = 13.8345;
	            }
	            case 2://Batte
	            {
	                x = 2452.9528;
	                y = -1646.755;
	                z = 13.376;
	                fx = 2453.1010;
					fy = -1647.7213;
					fz = 13.8345;
	            }
	            case 3://.222
	            {
	                x = 2453.5849;
	                y = -1646.7189;
	                z = 13.338;
	                fx = 2453.6508;
					fy = -1647.7299;
					fz = 13.8345;
	            }
	            case 4://7.62
	            {
	                x = 2454.0258;
	                y = -1646.5539;
	                z = 13.3879;
	                fx = 2454.0;
					fy = -1647.7279;
					fz = 13.8345;
	            }
	            case 5://Desert Eagle
	            {
	                x = 2454.4409;
	                y = -1646.7989;
	                z = 13.284;
	                fx = 2454.5505;
					fy = -1647.7436;
					fz = 13.8345;
	            }
	            case 6://Roue
	            {
	                x = 2452.8569;
	                y = -1647.5229;
	                z = 12.9759;
	                fx = 2452.83;
					fy = -1648.773;
					fz = 13.3396;
	            }
	        }
		}
	    case 6://Boutique Blueberry
	    {
	        switch(itemid)
	        {
	            case 1://9mm
	            {
	                x = 4.899346;
					y = -290.179962;
					z = 3.321315;
	                fx = 0.551514;
	                fy = -290.100433;
	                fz = 5.789114;
	            }
	            case 2://Casserole
	            {
	                x = 4.950664;
					y = -290.844848;
					z = 3.441129;
	                fx = 0.542022;
	                fy = -290.619354;
	                fz = 5.789114;
	            }
	            case 3://Lait
	            {
	                x = 4.994320;
					y = -291.498046;
					z = 3.592125;
	                fx = 0.510322;
	                fy = -291.239196;
	                fz = 5.789114;
	            }
	            case 4://Sawn off
	            {
	                x = 4.959078;
					y = -292.088714;
					z = 3.611796;
	                fx = 0.459093;
	                fy = -291.991638;
	                fz = 5.789114;
	            }
	            case 5://Lit miteux
	            {
	                x = 1.455406;
					y = -291.845001;
					z = 5.222277;
	                fx = -2.528390;
	                fy = -294.652557;
	                fz = 6.338981;
	            }
	        }
		}
		case 7://SF Residential
		{
		    switch(itemid)
		    {
	            case 1://Gilet thermique
	            {
	                x = -2086.133544;
					y = 833.956787;
					z = 68.229675;
	                fx = -2081.723876;
					fy = 835.337341;
					fz = 70.139762;
	            }
	            case 2://Soupe aux poissons
	            {
	                x = -2086.507080;
	                y = 835.736816;
	                z = 67.402236;
	                fx = -2082.168945;
					fy = 835.602661;
					fz = 69.884826;
	            }
	            case 3://Lait
	            {
	                x = -2086.997314;
	                y = 835.888977;
	                z = 68.115226;
	                fx = -2082.273681;
					fy = 836.116821;
					fz = 69.738891;
	            }
	            case 4://12 gauge
	            {
	                x = -2086.454101;
	                y = 836.859497;
	                z = 67.926795;
	                fx = -2081.895996;
					fy = 836.601989;
					fz = 69.965835;
	            }
	            case 5://Molotov
	            {
	                x = -2086.688232;
	                y = 837.107177;
	                z = 68.164367;
	                fx = -2082.002441;
					fy = 837.153686;
					fz = 69.908180;
	            }
		    }
		}
		case 8://SF Roofs
		{
		    switch(itemid)
		    {
	            case 1://Masque à gaz
	            {
	                x = -1635.774902;
	                y = 1018.651184;
	                z = 67.238067;
	                fx = -1635.832763;
					fy = 1022.865478;
					fz = 69.928108;
	            }
	            case 2://Coffre fort
	            {
	                fx = -1636.690673;
					fy = 1024.069702;
					fz = 70.520233;
	                x = -1636.932739;
	                y = 1019.475036;
	                z = 68.563079;
	            }
	            case 3://Lampe
	            {
	                fx = -1638.362060;
					fy = 1024.454589;
					fz = 69.979003;
	                x = -1638.271362;
	                y = 1019.635864;
	                z = 68.647865;
	            }
	            case 4://Medikit
	            {
	                x = -1637.552001;
	                y = 1020.290405;
	                z = 68.244369;
	                fx = -1640.237426;
					fy = 1024.125244;
					fz = 70.000129;
	            }
	            case 5://Bombe
	            {
	                x = -1637.317260;
	                y = 1020.514282;
	                z = 68.685470;
	                fx = -1641.919799;
					fy = 1021.932495;
					fz = 70.028907;
	            }
		        case 6://Lit miteux
		        {
	                x = -1639.913696;
	                y = 1019.333679;
	                z = 69.204513;
	                fx = -1638.075561;
					fy = 1023.653808;
					fz = 70.924377;
		        }
		    }
		}
	}
}

public NextShopItemForPlayer(playerid, bool:next)
{
	new string[256];
	new dMaxShopItems[] = {6, 6, 7, 6, 6, 6, 5, 5, 6};
	if(next)
	{
		if(dShop[playerid][1] == dMaxShopItems[dShop[playerid][0]]) dShop[playerid][1] = 1;
    	else dShop[playerid][1] ++;
	}
	else if(!next)
	{
		if(dShop[playerid][1] == 1) dShop[playerid][1] = dMaxShopItems[dShop[playerid][0]];
		else dShop[playerid][1] --;
	}
	//---
	new dPrice = GetShopItemPrice(dShop[playerid][0], dShop[playerid][1]);
	new dItemID = GetShopItemID(dShop[playerid][0], dShop[playerid][1]);
	new sObjectName[30];
	new dLanguage = CallRemoteFunction("GetPlayerLanguage", "i", playerid);
	CallRemoteFunction("GetObjectName", "iii", playerid, dItemID, dLanguage);
	GetPVarString(playerid, "sShopName", sObjectName, 30);
	switch(dLanguage)
	{
	    case LANGUAGE_EN:
	    {
			if(CallRemoteFunction("HasPlayerGold", "ii", playerid, dPrice)) format(string, sizeof(string), "~g~%s~n~~w~Price: ~y~%.1fg of gold.~n~~n~~r~~k~~PED_JUMPING~ ~w~to buy.~n~~r~~<~ - ~>~ ~w~to switch.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~to leave.", sObjectName, floatdiv(dPrice, 10));
			else format(string, sizeof(string), "~r~%s~n~~w~Price: ~y~%.1fg of gold.~n~~n~~r~~<~ - ~>~ ~w~to switch.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~to leave.", sObjectName, floatdiv(dPrice, 10));
	    }
	    case LANGUAGE_FR:
	    {
			if(CallRemoteFunction("HasPlayerGold", "ii", playerid, dPrice)) format(string, sizeof(string), "~g~%s~n~~w~Prix: ~y~%.1fg d'or.~n~~n~~r~~k~~PED_JUMPING~ ~w~pour acheter.~n~~r~~<~ - ~>~ ~w~pour changer.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~pour quitter.", sObjectName, floatdiv(dPrice, 10));
			else format(string, sizeof(string), "~r~%s~n~~w~Prix: ~y~%.1fg d'or.~n~~n~~r~~<~ - ~>~ ~w~to changer.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~pour quitter.", sObjectName, floatdiv(dPrice, 10));
	    }
	    case LANGUAGE_ES:
	    {
			if(CallRemoteFunction("HasPlayerGold", "ii", playerid, dPrice)) format(string, sizeof(string), "~g~%s~n~~w~Precio: ~y~%.1fg de oro gold.~n~~n~~r~~k~~PED_JUMPING~ ~w~para comprar.~n~~r~~<~ - ~>~ ~w~para cambiar.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~para dejar.", sObjectName, floatdiv(dPrice, 10));
			else format(string, sizeof(string), "~r~%s~n~~w~Precio: ~y~%.1fg de oro.~n~~n~~r~~<~ - ~>~ ~w~to cambiar.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~para dejar.", sObjectName, floatdiv(dPrice, 10));
	    }
	    case LANGUAGE_PG:
	    {
			if(CallRemoteFunction("HasPlayerGold", "ii", playerid, dPrice)) format(string, sizeof(string), "~g~%s~n~~w~Preço: ~y~%.1fg de ouro.~n~~n~~r~~k~~PED_JUMPING~ ~w~para comprar.~n~~r~~<~ - ~>~ ~w~para mudar.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~para sair.", sObjectName, floatdiv(dPrice, 10));
			else format(string, sizeof(string), "~r~%s~n~~w~Preço: ~y~%.1fg de ouro.~n~~n~~r~~<~ - ~>~ ~w~para mudar.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~para sair.", sObjectName, floatdiv(dPrice, 10));
	    }
	    case LANGUAGE_IT:
	    {
			if(CallRemoteFunction("HasPlayerGold", "ii", playerid, dPrice)) format(string, sizeof(string), "~g~%s~n~~w~Prezzo: ~y~%.1fg di oro.~n~~n~~r~~k~~PED_JUMPING~ ~w~per comperar.~n~~r~~<~ - ~>~ ~w~per scambiare.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~per uscire.", sObjectName, floatdiv(dPrice, 10));
			else format(string, sizeof(string), "~r~%s~n~~w~Prezzo: ~y~%.1fg di oro gold.~n~~n~~r~~<~ - ~>~ ~w~per scambiare.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~per uscire", sObjectName, floatdiv(dPrice, 10));
	    }
	    case LANGUAGE_DE:
	    {
			if(CallRemoteFunction("HasPlayerGold", "ii", playerid, dPrice)) format(string, sizeof(string), "~g~%s~n~~w~Preis: ~y~%.1fg gold.~n~~n~~r~~k~~PED_JUMPING~ ~w~zum kaufen.~n~~r~~<~ - ~>~ ~w~zum Objekte wechseln.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~zum beenden.", sObjectName, floatdiv(dPrice, 10));
			else format(string, sizeof(string), "~r~%s~n~~w~Preis: ~y~%.1fg gold.~n~~n~~r~~<~ - ~>~ ~w~zum Objekte wechseln.~n~~r~~k~~VEHICLE_ENTER_EXIT~ ~w~zum beenden.", sObjectName, floatdiv(dPrice, 10));
	    }
	}
	//---
	new Float:x, Float:y, Float:z, Float:fx, Float:fy, Float:fz;
	GetShopItemCameraInfos(dShop[playerid][0], dShop[playerid][1], x, y, z, fx, fy, fz);
	//---
    CallRemoteFunction("ShowPlayerTextInfo", "iissssss", playerid, -1, string, string, string, string, string, string);
	SetPlayerCameraPos(playerid, fx, fy, fz);
	SetPlayerCameraLookAt(playerid, x, y, z, 1);
}

public GetPlayerShop(playerid)
{
	return (dShop[playerid][0] != -1) ? true : false;
}

public BuyPlayerItem(playerid)
{
    new dFreeSlot = CallRemoteFunction("GetPlayerNextFreeSlot", "i", playerid);
    if(dFreeSlot  == -1)
    {
		PlayerPlaySound(playerid, 1053, 0.0, 0.0, 0.0);
		CallRemoteFunction("SendClientMessageEx", "iissssss", playerid, ROUGE, "You cannot carry more items!", "Vous ne pouvez pas porter plus d'objets !", "¡No puede llevar más objetos!", "Portugais", "Italien", "Sie können nicht mehr Objekte tragen!");
		return 1;
    }
    new dPrice = GetShopItemPrice(dShop[playerid][0], dShop[playerid][1]);
    if(!CallRemoteFunction("HasPlayerGold", "ii", playerid, dPrice))
    {
		PlayerPlaySound(playerid, 1053, 0.0, 0.0, 0.0);
		CallRemoteFunction("SendClientMessageEx", "iissssss", playerid, ROUGE, "You don't have enough gold!", "Vous n'avez pas assez d'or !", "¡No tiene suficiente de oro!", "Portugais", "No avete abbastanza di oro.", "Sie haben nicht genug gold!");
		return 1;
    }
    //---
	PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
    CallRemoteFunction("GivePlayerGold", "ii", playerid, -dPrice);
    CallRemoteFunction("GivePlayerSlotObject", "iiii", playerid, GetShopItemID(dShop[playerid][0], dShop[playerid][1]), dFreeSlot);
    if(CallRemoteFunction("GetPlayerMission", "i", playerid) == MISSION_HAZING_HARVEST && dShop[playerid][0] == 6 && dShop[playerid][1] == 4)
    {
        if(dFreeSlot != 0) CallRemoteFunction("SwapPlayerObjects", "iii", playerid, 0, dFreeSlot);
		CallRemoteFunction("ShowPlayerTextInfo", "iissssss", playerid, 5000, "Now hit the ~k~~CONVERSATION_NO~ key to equip the weapon!", "Appuyez sur la touche ~k~~CONVERSATION_NO~ pour équiper l'arme !", "Espagnol", "Portugais", "Italien", "Allemand");
    }
    return 1;
}

public ShowPlayerShop(playerid, shopid)
{
	if(shopid == -1)
	{
	    return false;
	}
	else
	{
	    ClearAnimations(playerid, true);
	    dShop[playerid][0] = shopid;
	    dShop[playerid][1] = 0;
		TogglePlayerControllable(playerid, false);
	    NextShopItemForPlayer(playerid, true);
	    return true;
	}
}

public ClosePlayerShop(playerid)
{
    ClearAnimations(playerid, true);
	dShop[playerid][0] = -1;
	dShop[playerid][1] = 0;
	TogglePlayerControllable(playerid, true);
	SetCameraBehindPlayer(playerid);
	CallRemoteFunction("HidePlayerTextInfo", "i", playerid);
}

public GetPlayerNearAuctionHouse(playerid)
{
	if(IsPlayerInRangeOfPoint(playerid, 5.0, -1688.0220, 1011.0923, 76.1589)) return 0;//San Fierro
	if(IsPlayerInRangeOfPoint(playerid, 5.0, 2231.6819, 1090.6613, 40.7969)) return 1;//Las Venturas
	if(IsPlayerInRangeOfPoint(playerid, 5.0, -65.9688, -371.7368, 5.4297)) return 1;//Blueberry
	return -1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	new cmd[256];
	new string[128];
	new idx;
	cmd = strtok(cmdtext, idx);
	if(strcmp(cmd, "/hotdog", true) == 0)
	{
		new tmp[128];
		new cartid = 0;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /hotdog [SLOTID]");
 			return 1;
		}
		cartid = strval(tmp);
		new Float:x, Float:y, Float:z, Float:angle;
		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, angle);
		format(string, sizeof(string), "%.4f, %.4f, %.4f, %.4f", x, y, z, angle);
		SendClientMessage(playerid, 0xFFFFFFFF, string);
    	AddHotDogCart(cartid, x, y, z, angle);
		return 1;
   	}
	if(strcmp(cmd, "/destroydog", true) == 0)
	{
		new tmp[128];
		new cartid = 0;
		tmp = strtok(cmdtext, idx);
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "USAGE: /destroydog [SLOTID]");
 			return 1;
		}
		cartid = strval(tmp);
    	DestroyHotDogCart(cartid);
		return 1;
   	}
	return 0;
}
