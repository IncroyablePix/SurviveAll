/*-------------------------------------------------------------------------------//
\\  - Aide sur le serveur -
//
\\  	Fonctions globales:
//          DestroyMapping(playerid)
\\          CheckForPlayersToGates()
\\
//
//      Fonctions locales:
\\          CreateGates()
//          CreateMappingObject(mappartid)
\\          DestroyGates()
//          DestroyStaticMapping()
\\          ChangeGateState(gateid, bool:open)
//          PreRemoveBuildings()
\\
//-------------------------------------------------------------------------------*/

#include <a_samp>
#include <[SA]Defines.inc>
#include <[SA]Functions.inc>

//---VARIABLES
static tFondDialogue[3] = {Text:INVALID_TEXT_DRAW, ...};
/*
AIDE:

1- Ramasser un objet:
	Vous pouvez ramasser un objet, un lit, une tente sur le sol.
	Appuyez sur H pour ramasser quelque chose � c�t� de vous.
	S'il y a plusieurs objets � c�t� de vous, vous pourrez choisir.

2- Utiliser un objet:
	Si vous avez un objet dans la main, appuyez sur N pour l'utiliser.
	Sinon, ouvrez votre sac � dos avec Y et double-cliquez sur l'objet.

3- Le sac � dos:
	Appuyez sur Y pour ouvrir votre sac � dos.
	Pour changer deux objets de place, cliquez cons�cutivement sur eux.
	Pour utiliser ou jeter un objet, double-cliquez sur celui-ci.
	Vous pouvez trouver de meilleurs sac � dos ou en acheter !
	Si vous mourrez, vous perdrez tout le contenu de votre sac.

4- Faim:
	Si vous ne mangez pas, vous allez mourir de faim.
	Vous pouvez trouver, acheter ou faire vous-m�me � manger.
	Vous pouvez chasser, p�cher ou cultiver !

5- Soif:
	Si vous ne buvez pas, vous allez mourir de soif.
	Vous pouvez trouver ou collecter vous-m�me � boire.
	Vous pouvez fabriquer ou trouver un collecteur d'eau !

6- Sommeil:
	Si vous ne dormez pas, vous allez vous affaiblir.
	Vous pouvez trouver ou acheter un lit.
	Vous ne pouvez poser de lits que dans les maisons ou dans les tentes.
	Vous pouvez aussi prendre une pillule de caf�ine pour vous r�veiller.

7- Temp�rature:
	Si vous passez votre temps dans l'eau, votre temp�rature baissera.
	Si votre temp�rature de surface atteint 20�, vous mourrez de froid.
	En courrant ou en restant pr�s du feu, vous vous r�chaufferez.

8- R�cup�rateurs d'eau:
	Vous pouvez fabriquer ou voler un collecteur d'eau.
	Il se remplira quand il pleuvra, et vous pourrez remplir vos bouteilles.
	Pour le fabriquer, trouvez un ing�nieur.

9- Coffres forts:
	Pour stocker vos objets les plus pr�cieux, vous aurez besoin d'un coffre.
	Vous pouvez fabriquer un coffre chez un ing�nieur.
	Utilisez un coffre pour le poser, vous devrez mettre un code � 4 chiffres.
	Appuyez sur C pour l'ouvrir, Y pour regarder dedans.
	
10- V�hicules:
	Pour aller plus vite, vous pourrez trouver des v�hicules.
	Pour qu'un v�hicule roule, il doit avoir un moteur et de l'essence.
	Vous pourrez trouver un moteur pr�s des garages.
	Vous pourrez trouver de l'essence un peu partout ou dans les stations.
	Vous pourrez r�parer votre v�hicule avec une cl� anglaise.
	
11- Coffre de v�hicule:
	Un v�hicule a un coffre dans lequel vous pourrez ranger des objets.
	Appuyez sur Y � l'int�rieur du v�hicule pour regarder.
	
12- Culture:
	Si vous trouvez des graines, vous pourrez planter dans la terre.
	Les plantes poussent le jour, et plus vite s'il pleut.
	Une plante donne des fruits.
	Vous pourrez couper avec une tron�onneuse si vous voulez du bois.
	
13- Magasins:
	Vous pourrez trouver des magasins dans les camps de survivants.
	Vous pourrez vendre des objets en double-cliquant dessus dans votre sac.
	Vous gagnerez de l'or que vous pourrez d�penser pour d'autres objets.
	Certains objets ne peuvent �tre trouv�s nulle part ailleurs.
	
14- Ing�nieurs:
	Vous pourrez trouver des ing�nieurs dans les camps de survivants.
	Vous pourrez d�monter des objets en double-cliquant dessus dans votre sac.
	Vous obtiendrez du bois, du fer ou autres.
	Vous pourrez aussi fabriquer de nouveaux objets uniques !
	
15- Armes:
	Pour survivre, chasser ou vous battre, vous aurez besoin d'armes.
	Vous pourrez en trouver/acheter un peu partout ou en fabriquer chez un ing�nieur.
	Pour une arme � feu, vous aurez besoin de munitions.
	Si vous mourrez, vous perdrez toutes vos armes.
	
16- Cuisine:
	Trouvez, achetez ou fabriquez une casserole pour cuisiner.
	Utilisez-la pr�s d'un feu.
	S�lectionnez une recette dont vous avez les ingr�dients et cuisinez !
	La nourriture issue de la cuisine est plus nourrissante que tout !
*/

//---FONCTIONS
forward ShowPlayerHelp(playerid, helpid, language);//Fonction pour afficher une partie de l'aide.

//---VARIABLES
