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

//---FONCTIONS
forward ShowPlayerHelp(playerid, helpid, language);//Fonction pour afficher une partie de l'aide.

//---VARIABLES
