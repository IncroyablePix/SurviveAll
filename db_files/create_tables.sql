CREATE TABLE Plant(
    idplant INTEGER NOT NULL AUTO_INCREMENT,
    typeplant INTEGER NOT NULL DEFAULT 0,
    growtime INTEGER NOT NULL,
    xplant FLOAT NOT NULL,
    yplant FLOAT NOT NULL,
    zplant FLOAT NOT NULL,
    aplant FLOAT NOT NULL,

	CONSTRAINT plant_id         PRIMARY KEY(idplant)
);

CREATE TABLE Seat(
    idseat INTEGER NOT NULL AUTO_INCREMENT,
    model INTEGER NOT NULL,
    xseat FLOAT NOT NULL,
    yseat FLOAT NOT NULL,
    zseat FLOAT NOT NULL,
    aseat FLOAT NOT NULL,

	CONSTRAINT seat_id          PRIMARY KEY(idseat)
);

CREATE TABLE Shredder(
    idshredder INTEGER NOT NULL AUTO_INCREMENT,
    state INTEGER NOT NULL DEFAULT 0,
    xshredder FLOAT NOT NULL,
    yshredder FLOAT NOT NULL,
    zshredder FLOAT NOT NULL,
    ashredder FLOAT NOT NULL,

    CONSTRAINT shredder_id      PRIMARY KEY(idshredder)
);

CREATE TABLE Tank(
    idtank INTEGER NOT NULL AUTO_INCREMENT,
    amount INTEGER NOT NULL,
    xtank FLOAT NOT NULL,
    ytank FLOAT NOT NULL,
    ztank FLOAT NOT NULL,
    atank FLOAT NOT NULL,

    CONSTRAINT tank_id          PRIMARY KEY(idtank)
);

CREATE TABLE Tent(
    idtent INTEGER NOT NULL AUTO_INCREMENT,
    xtent FLOAT NOT NULL,
    ytent FLOAT NOT NULL,
    ztent FLOAT NOT NULL,
    atent FLOAT NOT NULL,

    CONSTRAINT tent_id          PRIMARY KEY(idtent)
);

CREATE TABLE Gold(
    idgold INTEGER NOT NULL AUTO_INCREMENT,
    amount INTEGER NOT NULL,
    xgold FLOAT NOT NULL,
    ygold FLOAT NOT NULL,
    zgold FLOAT NOT NULL,

    CONSTRAINT gold_id          PRIMARY KEY(idgold)
);

CREATE TABLE Fire(
    idfire INTEGER AUTO_INCREMENT,
    timefire INTEGER NOT NULL,
    xfire FLOAT NOT NULL,
    yfire FLOAT NOT NULL,
    zfire FLOAT NOT NULL,

    CONSTRAINT fire_id          PRIMARY KEY(idfire)
);

CREATE TABLE Collector(
    idcollector INTEGER AUTO_INCREMENT,
    water INTEGER NOT NULL,
    xcollector FLOAT NOT NULL,
    ycollector FLOAT NOT NULL,
    zcollector FLOAT NOT NULL,
    acollector FLOAT NOT NULL,

    CONSTRAINT collector_id     PRIMARY KEY(idcollector)
);

CREATE TABLE Furniture(
    idfurniture INTEGER AUTO_INCREMENT,
    model INTEGER NOT NULL,
    xfurniture FLOAT NOT NULL,
    yfurniture FLOAT NOT NULL,
    zfurniture FLOAT NOT NULL,
    rxfurniture FLOAT NOT NULL,
    ryfurniture FLOAT NOT NULL,
    rzfurniture FLOAT NOT NULL,

    CONSTRAINT furniture_id     PRIMARY KEY(idfurniture)
);

CREATE TABLE Environment(
    hours INTEGER NOT NULL,
    minutes INTEGER NOT NULL,
    daytime INTEGER NOT NULL,
    weather INTEGER NOT NULL,
    timeweather INTEGER NOT NULL
);

CREATE TABLE Gasstation(
    idstation INTEGER AUTO_INCREMENT,
    quantite INTEGER NOT NULL,
    xstation FLOAT NOT NULL,
    ystation FLOAT NOT NULL,
    zstation FLOAT NOT NULL,

    CONSTRAINT gastation_id     PRIMARY KEY(idstation),
    CONSTRAINT gasstation_notnegamount CHECK(quantite > 0)
);

CREATE TABLE Brasero(
    idbrasero INTEGER AUTO_INCREMENT,
    state INTEGER NOT NULL,
    xbrasero FLOAT NOT NULL,
    ybrasero FLOAT NOT NULL,
    zbrasero FLOAT NOT NULL,
    abrasero FLOAT NOT NULL,

    CONSTRAINT brasero_id   PRIMARY KEY(idbrasero)
);

CREATE TABLE Board(
    idboard INTEGER AUTO_INCREMENT,
    existsboard BOOLEAN NOT NULL DEFAULT FALSE,
    xboard FLOAT NOT NULL,
    yboard FLOAT NOT NULL,
    zboard FLOAT NOT NULL,
    aboard FLOAT NOT NULL,

    CONSTRAINT board_id     PRIMARY KEY(idboard)
);

CREATE TABLE Weapon(
    idweapon INTEGER AUTO_INCREMENT,
    ammo INTEGER NOT NULL,
    xweapon FLOAT NOT NULL,
    yweapon FLOAT NOT NULL,
    zweapon FLOAT NOT NULL,

    CONSTRAINT weapon_id    PRIMARY KEY(idweapon),
    CONSTRAINT weapon_notnegammo CHECK(ammo > 0)
);

CREATE TABLE Gunrack(
    idgunrack INTEGER AUTO_INCREMENT,
    xgunrack FLOAT NOT NULL,
    ygunrack FLOAT NOT NULL,
    zgunrack FLOAT NOT NULL,
    weapons VARCHAR(40) NOT NULL,

    CONSTRAINT gunrack_id   PRIMARY KEY(idgunrack)
);
CREATE TABLE AdminRanks (
    idrank INTEGER AUTO_INCREMENT,
    priority INTEGER NOT NULL,
    name VARCHAR(16) NOT NULL,
    
    CONSTRAINT rank_id PRIMARY KEY (idrank)
 );
CREATE TABLE Player(
    idplayer INTEGER AUTO_INCREMENT,
    username VARCHAR(25) NOT NULL,
    password VARCHAR(64) NOT NULL,
    salt VARCHAR(10) NOT NULL,

    registerdate DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    lastco DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,

    adminlevel INTEGER NOT NULL DEFAULT 0,
    idvip INTEGER NOT NULL DEFAULT 0,
    viptime INTEGER NOT NULL DEFAULT 0,
    banned DATETIME,
    language INTEGER NOT NULL DEFAULT 0,
    gold INTEGER NOT NULL DEFAULT 0,
    
    gametime INTEGER NOT NULL DEFAULT 0,
    bagtype INTEGER NOT NULL DEFAULT 0,
    
    hunger INTEGER NOT NULL DEFAULT 100,
    thirst INTEGER NOT NULL DEFAULT 100,
    sleep INTEGER NOT NULL DEFAULT 100,
    
    health INTEGER NOT NULL DEFAULT 1000,
    armour INTEGER NOT NULL DEFAULT 1000,
    
    x FLOAT NOT NULL DEFAULT 0.0,
    y FLOAT NOT NULL DEFAULT -250.0,
    z FLOAT NOT NULL DEFAULT 5.0,
    a FLOAT NOT NULL DEFAULT 0.0,
    
    legs BOOLEAN NOT NULL DEFAULT TRUE,
    bleed BOOLEAN NOT NULL DEFAULT FALSE,

    temperature INTEGER DEFAULT 370,

    infohat VARCHAR(90) NOT NULL,
    infoglasses VARCHAR(90) NOT NULL,
    infomask VARCHAR(90) NOT NULL,
    infobody VARCHAR(90) NOT NULL,
    infoweapon VARCHAR(54) NOT NULL,
    inventory VARCHAR(150) NOT NULL,

    level INTEGER NOT NULL DEFAULT 0,
    
    help1_16 INTEGER NOT NULL DEFAULT 0,
    help17_32 INTEGER NOT NULL DEFAULT 0,

    skills VARCHAR(27) NOT NULL,

    reggaeshark INTEGER NOT NULL DEFAULT 0,
    intro INTEGER NOT NULL DEFAULT 0,
    amy INTEGER NOT NULL DEFAULT 0,
    ken INTEGER NOT NULL DEFAULT 0,
    dpo INTEGER NOT NULL DEFAULT 0,

    killzombies INTEGER NOT NULL DEFAULT 0,
    killboss INTEGER NOT NULL DEFAULT 0,
    kills INTEGER NOT NULL DEFAULT 0,
    deaths INTEGER NOT NULL DEFAULT 0,

    goldtogive INTEGER NOT NULL DEFAULT 0,
    itemstogive VARCHAR(201) NOT NULL,

    CONSTRAINT player_id PRIMARY KEY(idplayer),
    CONSTRAINT admin_rank FOREIGN KEY(adminlevel) REFERENCES AdminRanks(idrank)
);

CREATE TABLE Object(
    idobject INTEGER AUTO_INCREMENT,
    idmodel INTEGER NOT NULL DEFAULT 19300,

    orotx FLOAT NOT NULL DEFAULT 0.0,
    oroty FLOAT NOT NULL DEFAULT 0.0,
    orotz FLOAT NOT NULL DEFAULT 0.0,
    ozoom FLOAT NOT NULL DEFAULT 0.0,

    ohoffsetx FLOAT NOT NULL DEFAULT 0.0,
    ohoffsety FLOAT NOT NULL DEFAULT 0.0,
    ohoffsetz FLOAT NOT NULL DEFAULT 0.0,
    ohrotx FLOAT NOT NULL DEFAULT 0.0,
    ohroty FLOAT NOT NULL DEFAULT 0.0,
    ohrotz FLOAT NOT NULL DEFAULT 0.0,
    ohoffzoom FLOAT NOT NULL DEFAULT 0.0,

    ogroundrotx FLOAT NOT NULL DEFAULT 0.0,
    ogroundroty FLOAT NOT NULL DEFAULT 0.0,
    ogroundrotz FLOAT NOT NULL DEFAULT 0.0,
    ogroundoffsetz FLOAT NOT NULL DEFAULT 0.0,

    sellprice INTEGER NOT NULL DEFAULT 0,
    typeobject INTEGER NOT NULL DEFAULT 0,

    heavy BOOLEAN NOT NULL DEFAULT FALSE,

    name_en VARCHAR(30) DEFAULT 'Nothing',
    name_fr VARCHAR(30) DEFAULT 'Rien',
    name_es VARCHAR(30) DEFAULT 'Nada',
    name_pg VARCHAR(30) DEFAULT 'Nada',
    name_it VARCHAR(30) DEFAULT 'Niente',
    name_de VARCHAR(30) DEFAULT 'Nichts',
    
    CONSTRAINT object_id PRIMARY KEY(idobject)
);

CREATE TABLE Bed(
    idbed INTEGER AUTO_INCREMENT,
    typebed INTEGER NOT NULL,
    xbed FLOAT NOT NULL,
    ybed FLOAT NOT NULL,
    zbed FLOAT NOT NULL,
    abed FLOAT NOT NULL,

    CONSTRAINT bed_id PRIMARY KEY(idbed)
);

CREATE TABLE PositionL(
    idbed INTEGER NOT NULL,
    idplayer INTEGER NOT NULL,
    dateposition DATETIME DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT position_id  PRIMARY KEY(idbed, idplayer),
    CONSTRAINT position_ref FOREIGN KEY(idbed) REFERENCES Bed(idbed),
                            FOREIGN KEY(idplayer) REFERENCES Player(idplayer)
);

CREATE TABLE Garage(
    idgarage INTEGER AUTO_INCREMENT,
    code CHAR(4) NOT NULL,
    xgarage FLOAT NOT NULL,
    ygarage FLOAT NOT NULL,
    zgarage FLOAT NOT NULL,
    agarage FLOAT NOT NULL,

    CONSTRAINT garage_id PRIMARY KEY(idgarage)
);

CREATE TABLE ConstructionG(
    idgarage INTEGER NOT NULL,
    idplayer INTEGER NOT NULL,
    dateconstruction DATETIME DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT constructiong_id     PRIMARY KEY(idgarage, idplayer),
    CONSTRAINT constructiong_ref    FOREIGN KEY(idgarage) REFERENCES Garage(idgarage),
                                    FOREIGN KEY(idplayer) REFERENCES Player(idplayer)
);

CREATE TABLE IP(
    ip VARCHAR(16) NOT NULL,
    banned BOOLEAN DEFAULT FALSE,

    CONSTRAINT ip_id 				PRIMARY KEY(ip)
);

CREATE TABLE Connection(
    ip VARCHAR(16) NOT NULL,
    idplayer INTEGER NOT NULL,
    dateconnection DATETIME DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT connection_id    PRIMARY KEY(ip, idplayer),
    CONSTRAINT connection_ref   FOREIGN KEY(ip) REFERENCES IP(ip),
                                FOREIGN KEY(idplayer) REFERENCES Player(idplayer)
);

CREATE TABLE House(
    idhouse INTEGER AUTO_INCREMENT,
    typehouse INTEGER NOT NULL,
    door BOOLEAN NOT NULL DEFAULT FALSE,
    dooropen BOOLEAN NOT NULL DEFAULT TRUE,
    code CHAR(4) NOT NULL,
    xhouse FLOAT NOT NULL,
    yhouse FLOAT NOT NULL,
    zhouse FLOAT NOT NULL,
    aHouse FLOAT NOT NULL,

    CONSTRAINT house_id PRIMARY KEY(idhouse)    
);

CREATE TABLE ConstructionH(
    idhouse INTEGER NOT NULL,
    idplayer INTEGER NOT NULL,

    CONSTRAINT constructionh_id     PRIMARY KEY(idhouse, idplayer),
    CONSTRAINT constructionh_ref    FOREIGN KEY(idhouse) REFERENCES House(idhouse),
                                    FOREIGN KEY(idplayer) REFERENCES Player(idplayer)
);

CREATE TABLE Item(
    iditem INTEGER AUTO_INCREMENT,
    idobject INTEGER NOT NULL DEFAULT 0,
    autospawn BOOLEAN NOT NULL DEFAULT FALSE,
    xitem FLOAT NOT NULL,
    yitem FLOAT NOT NULL,
    zitem FLOAT NOT NULL,

    CONSTRAINT item_id              PRIMARY KEY(iditem)
);

CREATE TABLE Representation(
    iditem INTEGER NOT NULL,
    idobject INTEGER NOT NULL,

    CONSTRAINT representation_id    PRIMARY KEY(iditem, idobject),
    CONSTRAINT representation_ref   FOREIGN KEY(iditem) REFERENCES Item(iditem),
                                    FOREIGN KEY(idobject) REFERENCES Object(idobject)
);

CREATE TABLE Auction(
    idauction INTEGER AUTO_INCREMENT,
    categorie INTEGER NOT NULL,

    idobject INTEGER NOT NULL DEFAULT 0,
    idplayer INTEGER NOT NULL,
    price INTEGER NOT NULL DEFAULT 0,
    dateauction DATETIME DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT auction_id           PRIMARY KEY(idauction),
    CONSTRAINT auction_ref          FOREIGN KEY(idplayer) REFERENCES Player(idplayer),
                                    FOREIGN KEY(idobject) REFERENCES Object(idobject)
);

CREATE TABLE Safe(
    idsafe INTEGER AUTO_INCREMENT,

    code CHAR(4),

    xsafe FLOAT NOT NULL,
    ysafe FLOAT NOT NULL,
    zsafe FLOAT NOT NULL,
    asafe FLOAT NOT NULL,

    content VARCHAR(50) NOT NULL,

    CONSTRAINT safe_id              PRIMARY KEY(idsafe)
);

CREATE TABLE Fridge(
    idfridge INTEGER AUTO_INCREMENT,

    xfridge FLOAT NOT NULL,
    yfridge FLOAT NOT NULL,
    zfridge FLOAT NOT NULL,
    afridge FLOAT NOT NULL,
    
    content VARCHAR(10) NOT NULL,

    CONSTRAINT fridge_id            PRIMARY KEY(idfridge)
);

CREATE TABLE Vehicles(
    idvehicle INTEGER AUTO_INCREMENT,
    model INTEGER NOT NULL,
    wheels INTEGER NOT NULL,
    fuel INTEGER NOT NULL DEFAULT 0,
    itemscapacity INTEGER NOT NULL,
    health FLOAT NOT NULL DEFAULT 1000.0,
    wheelsstate INTEGER NOT NULL,

    xveh FLOAT NOT NULL,
    yveh FLOAT NOT NULL,
    zveh FLOAT NOT NULL,
    aveh FLOAT NOT NULL,

    content VARCHAR(25) NOT NULL,

    col1 INTEGER NOT NULL DEFAULT 0,
    col2 INTEGER NOT NULL DEFAULT 0,

    engine BOOLEAN NOT NULL DEFAULT TRUE,

    CONSTRAINT vehicle_id           PRIMARY KEY(idvehicle)
);

CREATE TABLE Kills(
    idkill INTEGER AUTO_INCREMENT,

    idplayer INTEGER,
    idkilled INTEGER,
    datekill DATETIME DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT kill_id  	PRIMARY KEY(idkill),
    CONSTRAINT kill_ref 	FOREIGN KEY(idkilled) REFERENCES Player(idplayer),
                        	FOREIGN KEY(idplayer) REFERENCES Player(idplayer)
);