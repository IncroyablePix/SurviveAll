# Survive-All
## Abstract
SurviveAll is a bunch of **PAWN** scripts that (mostly) I wrote over 3 years. It contains the main script (gamemode) and its filterscripts for missions, mapping, object spawning, etc.

## Contributors
* IncroyablePix: Lead developper
* Jhonny: Supporting developper
* Draiike: Mapping of Los Santos, San Fierro & Las Venturas
* Azezas: Tester

## Progression
### Done
The project might not be "done" currently, I do not think that I will pursue it further. Its features contain : 
* Missions with a (poorly written) real scenario (~50?)
* A few bosses
* Hability to dynamically and personally build houses (from the place you chose to what-and-where furniture you want to set).
* Hability to create your very own village
* Auction houses
* CO-OP Missions
* ...wait and see.

It also contains:
* Farming system
* Complex crafting system
* Skills and levels
* (Very dumb) zombies
* Animals to hunt
* Fishing minigames
* Strange creatures roaming around
* Shops for exchanging with NPCs
* LOTS of NPCs
* Survivor camps with real interaction
* Long story short: blablabla

### To be done
* I have translated the whole script in english and french, I started the german and spannish versions as well, but had no time nor willing to go any further. The translation to portuguese was started at some point but the guy quit, as well as the italian version.
* The storyline is lacking some missions, the /filterscripts/[SA]Missions.pwn file contains my ideas and indications about how it was originally supposed to be.
* I was going to program a vehicle modification system (With rare loot, you could have been able to craft vehicle elements: plow, car-parachute, rockets for planes to be dropped).
* Even more bosses and missions!

You may obviously feel free to not give a crap about any of this and do whatever you want if you were willing to take this code on your own. 


## Dependencies
* "crashdetect" (optional)
* Incognito's "streamer"
* "ColAndreas"
* "FCNPC-DL"

## Installation
1. Download SA-MP 0.3DL (0.3.7 would work fine too, but several modifications shall be done => "Death" boss i.e.).
2. Download the dependencies
3. Create a ".cadb" file for ColAndreass [(here)](https://github.com/Pottus/ColAndreas) and place it in the /scriptfiles/colandreas folder
4. Extract the files to their respective folders.
5. Compile the following:
	- SurviveAll.pwn
	- [SA]Actors.pwn
	- [SA]Mapping.pwn
	- [SA]Missions.pwn
	- [SA]ObjectSpawner.pwn
	- [SA]Shops.pwn
6. Set your options into /pawno/include/[SA]Defines.inc
7. Configure /server.cfg:
	gamemode: SurviveAll
	filterscripts: (\0 => No need for the FS to be called here as they will be in the OnGameModeInit callback)
	plugins: crashdetect.(dll - so) streamer.(dll - so) ColAndreas.(dll - so) FCNPC-DL.(dll - so)
	maxplayers: Maximum players + 100 (Zombies) + 10 (PNJ humans) + 1 (Boss)
	maxnpc: 111
8. In /scriptfiles 'mkdir':
	=> Survive-All				//
		=> Admin			//
			=> IP			//
			=> Pseudos		//Nicknames in French, change it in the gamemode if you want to
			=> Punishments		//
		=> Comptes			//Accounts in French, change it as well
			=> Offline		//
		=> General			//	
8. Still in /scriptfiles:
	vim DB.ini
	Insert:
		```
		mysql_host
		mysql_user
		mysql_password
		mysql_database
		```
	(Replacing the mysql_* entries by the actual credentials)
9. Run the server

## Post Scriptum
1. At the very first launch, it will take hardly any time to get started, but at the closing or relaunching, the consequent files to be saved/loaded will make you wait for 1 - 5 minutes (depending on your HDD (or SSD))
2. Considering that I own absolutely no right on the music I used all along the cinematics, etc., I cannot include it here, figure something out by yourself or ask me directly.
3. Comments are mainly French, but should not be a problem for the hardcore coder you are!
