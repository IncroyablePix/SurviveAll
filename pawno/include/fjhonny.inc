#if defined _included_fjhonny
    #endinput
#endif

#define _included_fjhonny

#include <a_samp>
/*
native SendClientMessageRadius(playerid, Float:range, color, const string[]); // Envoie un message dans un périmètre autour du joueur
native GetName(playerid); // Retourne le pseudo du joueur
native GetIp(playerid); // Retourne l'IP du joueur
native ReturnUser(text[], playerid = INVALID_PLAYER_ID); // Retourne l'ID du joueur en fonction d'une partie de pseudo / d'ID
native strtok(const string[], &index); // Décompose une chaîne de texte par espaces
native IsNumeric(const string[]); // Vérifie si une chaîne de texte est numérique (composée de nombre)
native strrest(const string[], &index); // Retourne le reste de la décomposition de chaine de texte
native ShowPlayerTextInfo(playerid, time, string[]); // Affiche un message en bas de l'écran (quêtes)
native bool:Success(pc); // Vérifie si il y a eu un succès avec un pourcentage entré
native ConvertEncoding(string[]); // Encode une chaîne de caractère (fixe les accents)
native MaxArray(array[]); // Retourne la plus grande valeur d'une case d'un tableau
native GameTextForPlayerEx(playerid, const string[], time, style); // GameTextForPlayer encodé
native GameTextForAllEx(const string[], time, style); // GameTextForAll encodé
native PlayerTextDrawSetStringEx(playerid, PlayerText: ptd, const string[]); // PlayerTextDrawSetString encodé
native TextDrawSetStringEx(Text: td, const string[]) // TextDrawSetString encodé
*/
stock GameTextForPlayerEx(playerid, string[], time, style)
{
	ConvertEncoding(string);
	GameTextForPlayer(playerid, string, time, style);
}
stock GameTextForAllEx(string[], time, style)
{
	ConvertEncoding(string);
	GameTextForAll(string, time, style);
}
stock PlayerTextDrawSetStringEx(playerid, PlayerText: ptd, string[])
{
	ConvertEncoding(string);
	PlayerTextDrawSetString(playerid, ptd, string);
}
stock TextDrawSetStringEx(playerid, Text: td, string[])
{
	ConvertEncoding(string);
	TextDrawSetString(td, string);
}

stock bool: Success(pc)
{
	new bool: result = false;
	if(pc >= 100) result = true;
	else
	{
	    new rand = random(100);
		for(new i; i < pc; i++)
		{
			if(i == rand)
			{
				result = true;
				break;
			}
		}
	}
	return result;
}
stock MaxArray(array[])
{
	new maxi;
	for(new i; i < strlen(array); i++)
	{
	    if(i == 0) maxi = array[i];
	    else if(array[i] > maxi) maxi = array[i];
	}
	return maxi;
}
stock ConvertEncoding(string[])
{
	static const
		scRealChars[256] =
		{
			  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
			 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
			 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
			 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
			 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
			 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
			 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
			112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
			128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
			144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
			160,  94, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
			124, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 175,
			128, 129, 130, 195, 131, 197, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
			208, 173, 142, 143, 144, 213, 145, 215, 216, 146, 147, 148, 149, 221, 222, 150,
			151, 152, 153, 227, 154, 229, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
			240, 174, 165, 166, 167, 245, 168, 247, 248, 169, 170, 171, 172, 253, 254, 255
		};
	if (ispacked(string))
	{
		for (new i = 0, len = strlen(string); i != len; ++i)
		{
			string{i} = scRealChars[string{i}];
		}
	}
	else
	{
		for (new i = 0, len = strlen(string), ch; i != len; ++i)
		{
			if (0 <= (ch = string[i]) < 256)
			{
				string[i] = scRealChars[ch];
			}
		}
	}
}

new bAlreadyText[MAX_PLAYERS];
new PlayerText:InfoText[MAX_PLAYERS];
new timerinfo[MAX_PLAYERS];
stock ShowPlayerTextInfo(playerid, time, string[])
	{
	    if(!bAlreadyText[playerid])
	        {
				InfoText[playerid] = CreatePlayerTextDraw(playerid, 330.000000, 340.000000, "");
				PlayerTextDrawSetStringEx(playerid, InfoText[playerid], string);
				PlayerTextDrawAlignment(playerid, InfoText[playerid], 2);
				PlayerTextDrawBackgroundColor(playerid, InfoText[playerid], 255);
				PlayerTextDrawFont(playerid, InfoText[playerid], 1);
				PlayerTextDrawLetterSize(playerid, InfoText[playerid], 0.400000, 1.000000);
				PlayerTextDrawColor(playerid, InfoText[playerid], -1);
				PlayerTextDrawSetOutline(playerid, InfoText[playerid], 0);
				PlayerTextDrawSetProportional(playerid, InfoText[playerid], 1);
				PlayerTextDrawSetShadow(playerid, InfoText[playerid], 1);
				PlayerTextDrawSetSelectable(playerid, InfoText[playerid], false);
	        }
		else
		    {
		        KillTimer(timerinfo[playerid]);
		        PlayerTextDrawSetString(playerid, InfoText[playerid], string);
		    }
        bAlreadyText[playerid] = true;
		PlayerTextDrawShow(playerid, InfoText[playerid]);
		if(time != -1) timerinfo[playerid] = SetTimerEx("HidePlayerTextInfo", time, false, "i", playerid);
	}
forward HidePlayerTextInfo(playerid);
public HidePlayerTextInfo(playerid)
	{
		PlayerTextDrawHide(playerid, InfoText[playerid]);
		PlayerTextDrawDestroy(playerid, InfoText[playerid]);
		InfoText[playerid] = PlayerText:INVALID_TEXT_DRAW;
        bAlreadyText[playerid] = false;
		KillTimer(timerinfo[playerid]);
		timerinfo[playerid] = -1;
	}

stock SendClientMessageRadius(playerid, Float:range, color, const string[])
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	for(new i; i <= GetPlayerPoolSize(); i++) if(IsPlayerInRangeOfPoint(i, range, x, y, z)) SendClientMessage(i, color, string);
	return 1;
}
stock GetName(playerid)
{
	new pname[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, pname, sizeof(pname));
	return pname;
}
stock GetIp(playerid)
{
	new ip[16];
	GetPlayerIp(playerid, ip, sizeof(ip));
	return ip;
}
stock strrest(const string[], &index)
	{
		new length = strlen(string);
		while ((index < length) && (string[index] <= ' '))
			{
				index++;
			}
		new offset = index;
		new result[128];
		while ((index < length) && ((index - offset) < (sizeof(result) - 1)))
			{
				result[index - offset] = string[index];
				index++;
			}
		result[index - offset] = EOS;
		return result;
	}

stock IsNumeric(const string[])
	{
		for (new i = 0, j = strlen(string); i < j; i++)
			{
				if (string[i] > '9' || string[i] < '0') return 0;
			}
		return 1;
	}
stock strtok(const string[], &index)
	{
		new length = strlen(string);
		while ((index < length) && (string[index] <= ' '))
			{
				index++;
			}
		new offset = index;
		new result[20];
		while ((index < length) && (string[index] > ' ') && ((index - offset) < (sizeof(result) - 1)))
			{
				result[index - offset] = string[index];
				index++;
			}
		result[index - offset] = EOS;
		return result;
	}

stock ReturnUser(text[], playerid = INVALID_PLAYER_ID)
	{
		new pos = 0;
		while (text[pos] < 0x21)
			{
				if (text[pos] == 0) return INVALID_PLAYER_ID;
				pos++;
			}
		new userid = INVALID_PLAYER_ID;
		if (IsNumeric(text[pos]))
			{
				userid = strval(text[pos]);
				if (userid >=0 && userid < MAX_PLAYERS)
					{
						if(!IsPlayerConnected(userid))
							{
								userid = INVALID_PLAYER_ID;
							}
						else
							{
								return userid;
							}
					}
			}
		new len = strlen(text[pos]);
		new count = 0;
		for (new i = 0; i < MAX_PLAYERS; i++)
			{
				if (IsPlayerConnected(i))
					{
						if (strcmp(GetName(i), text[pos], true, len) == 0)
							{
								if (len == strlen(GetName(i)))
									{
										return i;
									}
								else
									{
										count++;
										userid = i;
									}
							}
					}
			}
		if (count != 1)
			{
				if (playerid != INVALID_PLAYER_ID)
					{
						if (count)
							{
								SendClientMessage(playerid, 0xFF0000AA, "Multiple users found, please narrow earch");
							}
						else
							{
								SendClientMessage(playerid, 0xFF0000AA, "No matching user found");
							}
					}
				userid = INVALID_PLAYER_ID;
			}
		return userid;
	}
