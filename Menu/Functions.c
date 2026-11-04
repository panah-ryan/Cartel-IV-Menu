#include "stdafx.h"
#include "Utils.h"
#include "Functions.h"
#include "Player_Options.h"
#include "Vehicle_Options.h"
#include "MenuUiHandling.h"
#include "Ped_Options.h"
#include "Network_Options.h"
#include "Network_Functions.h"
#include "Admin_Options.h"
#include "World_Options.h"
#include "Menu_Options.h"
#include "Chat_Options.h"
#include "ObjectVehicles_Options.h"
#include "Security.h"
//#include "common.h"

stickyFreeMemory *freeMem;
ctl_memory_sco *ctlMem;
//Spawner_Objects_Array *spawnerMem;
SpawnerObject *spawnerMem;
CAMERA_INDEX scriptedCam = 0;
#define DeclaredKeys 41
int max_spawner_objs = 70; //#deifne MAX_SPAWNER_OBJECTS max_spawner_objs //70 by default for retail, increased if XEX detected.
extern TextureDict browser;

//MAX_AUTH_USERS = 50 FUNCTIONS.h
void load_cartel_memory_with_auths()
{

	ctlMem->ObfuscatedIdentifiers[0].m_midObfuscatedLower = hidden_number0();
	ctlMem->ObfuscatedIdentifiers[0].m_xuidObfuscatedUpper = hidden_number0();  //never use array index 0
#ifdef AdminOptions
	//ObfuscatedIdentifiers[1].m_midObfuscatedLower = 2039011167; //Nath RGH current MID (will change cuz prolly on a kicklist ty azl) //working
	ctlMem->ObfuscatedIdentifiers[1].m_xuidObfuscatedUpper = 1792498931; //Dank Functions XUID obfuscated (Unique half). 
	ctlMem->ObfuscatedIdentifiers[1].m_midObfuscatedLower = 571770667; //Nath spare RGH KV (Admin Key). //hopefully doesn't get banned when the other does.
	ctlMem->ObfuscatedIdentifiers[2].m_midObfuscatedLower = -2071052949; //Nath xk3y
	ctlMem->ObfuscatedIdentifiers[2].m_xuidObfuscatedUpper = ctlMem->ObfuscatedIdentifiers[1].m_xuidObfuscatedUpper; //Dank Functions XUID obfuscated (Unique half).
	ctlMem->ObfuscatedIdentifiers[3].m_midObfuscatedLower = -1891902913;	//Ryan
	ctlMem->ObfuscatedIdentifiers[3].m_xuidObfuscatedUpper = -2076611116; //Wannabe Haxor
	//ObfuscatedIdentifiers[6].m_midObfuscatedLower = 335296187; //Stormsnout //Corona fucked RGH
	//ObfuscatedIdentifiers[6].m_xuidObfuscatedUpper = 1393870803; //Stormanout (CORM3Y)
	ctlMem->ObfuscatedIdentifiers[4].m_midObfuscatedLower = 1690982907; //Trinity, idk how the fuck he has a new KV. from the supposed same file.
	//ObfuscatedIdentifiers[4].m_midObfuscatedLower = -697791685;//Stormsnout
	ctlMem->ObfuscatedIdentifiers[4].m_xuidObfuscatedUpper = 1393870803; //Stormanout (CORM3Y);

	ctlMem->ObfuscatedIdentifiers[5].m_midObfuscatedLower = 653903807; //Dank 0Fuse current KV.
	ctlMem->ObfuscatedIdentifiers[5].m_xuidObfuscatedUpper = ctlMem->ObfuscatedIdentifiers[1].m_xuidObfuscatedUpper;
#else
#ifdef Moderator
	ctlMem->ObfuscatedIdentifiers[1].m_midObfuscatedLower = -1372890229;//Nova
	ctlMem->ObfuscatedIdentifiers[1].m_xuidObfuscatedUpper = -1435322753; //Nova XeMVP /TESTER

	ctlMem->ObfuscatedIdentifiers[2].m_midObfuscatedLower = 605094879; //imstuartb current MID //working /TESTER
	ctlMem->ObfuscatedIdentifiers[2].m_xuidObfuscatedUpper = 1488262064; //imstuartb XUID obfuscated (Unique half).

	ctlMem->ObfuscatedIdentifiers[3].m_midObfuscatedLower = 1506513979;	//PPRX RGH
	ctlMem->ObfuscatedIdentifiers[3].m_xuidObfuscatedUpper = 1526835160; //PPRX Meow XDK /TESTER

	ctlMem->ObfuscatedIdentifiers[4].m_midObfuscatedLower = 787380683; //Clerk RGH 
	ctlMem->ObfuscatedIdentifiers[4].m_xuidObfuscatedUpper = 1857080120; //ClerkIV /naths friend/TESTER

	ctlMem->ObfuscatedIdentifiers[5].m_midObfuscatedLower = -357348724; //Wrong functions / Luke
	ctlMem->ObfuscatedIdentifiers[5].m_xuidObfuscatedUpper = 106891330; //Wrong functions / Luke.

	ctlMem->ObfuscatedIdentifiers[6].m_midObfuscatedLower = 1634757419;  //Actionz IV paid Ryan 05/12/2024 Cashapp
	ctlMem->ObfuscatedIdentifiers[6].m_xuidObfuscatedUpper = 1972366685; //Actionz IV paid Ryan 05/12/2024 Cashapp //granted mod by nathan 22/01/2025

	ctlMem->ObfuscatedIdentifiers[7].m_midObfuscatedLower = 1450857003; //TarsaLLime61
	ctlMem->ObfuscatedIdentifiers[7].m_xuidObfuscatedUpper = -177099388; //TarsaLLime61

	ctlMem->ObfuscatedIdentifiers[8].m_midObfuscatedLower = 959067;		//ImFoxxyy RGH
	ctlMem->ObfuscatedIdentifiers[8].m_xuidObfuscatedUpper = 1465924181; //Mars6477 (Im Foxxyy)
	ctlMem->ObfuscatedIdentifiers[9].m_midObfuscatedLower = 1541259487; //Jordosky (Naths pal).
	ctlMem->ObfuscatedIdentifiers[9].m_xuidObfuscatedUpper = 1006139311; //TATTOOEDMENACE6

	ctlMem->ObfuscatedIdentifiers[10].m_midObfuscatedLower = -1892270613; //eatmyassfodolla 2 (slim)
	ctlMem->ObfuscatedIdentifiers[10].m_xuidObfuscatedUpper = 1409718019; //eatmyassfodolla 2 (slim) 

	ctlMem->ObfuscatedIdentifiers[11].m_xuidObfuscatedUpper = 115304294; //TBPing //115304294  
	ctlMem->ObfuscatedIdentifiers[11].m_midObfuscatedLower = -83383764;
#else

	//ctlMem->ObfuscatedIdentifiers[1].m_midObfuscatedLower = 1288547724; //J0rdan XDK (Nath friend/his birthday).
	//ctlMem->ObfuscatedIdentifiers[1].m_xuidObfuscatedUpper = -1846983305; //J0rdan XDK (Nath friend/his birthday).
	ctlMem->ObfuscatedIdentifiers[1].m_midObfuscatedLower = 1937485931; //AudiRS32951 / HUFF (Discord ID: 394441324876660737) (14/12/2025)
	ctlMem->ObfuscatedIdentifiers[1].m_xuidObfuscatedUpper = 2078708321;

	ctlMem->ObfuscatedIdentifiers[2].m_midObfuscatedLower = 1635561023;  //IMMORT4L Xx //Nath friend //probs remove.
	ctlMem->ObfuscatedIdentifiers[2].m_xuidObfuscatedUpper = -193403298; //IMMORT4L Xx  nath friend
	ctlMem->ObfuscatedIdentifiers[3].m_midObfuscatedLower = 275885868;   //eatmyassfodolla / DoYouHaveXBL 69 (Paid) //26/11/2024 //buyer no.1
	ctlMem->ObfuscatedIdentifiers[3].m_xuidObfuscatedUpper = 1409718019; //eatmyassfodolla / DoYouHaveXBL 69 (Paid) //26/11/2024 //buyer no.1 //paid nath/split with ryan but fees fucked us, ryan now gets 2 (1 as a buffer)
	//ctlMem->ObfuscatedIdentifiers[4].m_midObfuscatedLower = 1476883851;  //lordxlr/ YoungDSL Paid nath 02/11/2024 Paypal
	//ctlMem->ObfuscatedIdentifiers[4].m_xuidObfuscatedUpper = 885240164;  //lordxlr/ YoungDSL Paid nath 02/11/2024 Paypal //idk wtf i was supposed to do.
	ctlMem->ObfuscatedIdentifiers[4].m_midObfuscatedLower = -2125684293; //SynxDev / fkSynx (paid Ryan 19/12/2024 Cashapp)
	ctlMem->ObfuscatedIdentifiers[4].m_xuidObfuscatedUpper = -155781203; //SynxDev / fkSynx (paid Ryan 19/12/2024 Cashapp)

	ctlMem->ObfuscatedIdentifiers[5].m_midObfuscatedLower = 1938928795;   //GHOSTFACE SHADOW / ShadowOffiziell (Paid nath 30/12/2024 Paypal).
	ctlMem->ObfuscatedIdentifiers[5].m_xuidObfuscatedUpper = 593514842;  //GHOSTFACE SHADOW / ShadowOffiziell (Paid nath 30/12/2024 Paypal).
	ctlMem->ObfuscatedIdentifiers[6].m_midObfuscatedLower = -939019393; //Lewis487 / UtterSafe642243 (Paid Ryan 25/01/2025 amazon).
	//ctlMem->ObfuscatedIdentifiers[7].m_xuidObfuscatedUpper = 1312608401;  /// UtterSafe642243 (Paid Ryan 25/01/2025 amazon). /achievement test account
	ctlMem->ObfuscatedIdentifiers[6].m_xuidObfuscatedUpper = 1644835362; //Lewis684 /changed from Lweis487 / UtterSafe642243 (Paid Ryan 25/01/2025 amazon). /achievement test account
	//ctlMem->ObfuscatedIdentifiers[7].m_xuidObfuscatedUpper = -1847556820;  //Lewis487 / UtterSafe642243 (Paid Ryan 25/01/2025 amazon). /achievement test account
	///////////////ctlMem->ObfuscatedIdentifiers[8].m_midObfuscatedLower = 1896724447;  //SHARED KV GDIT //Shockwave / SysDEX (Paid Nath 02/02/2025 paypal) / could be getting shared who knows.
	ctlMem->ObfuscatedIdentifiers[7].m_midObfuscatedLower = 390179276; //Shockwave / SysDEX (Paid Nath 02/02/2025 paypal) / could be getting shared who knows.
	ctlMem->ObfuscatedIdentifiers[7].m_xuidObfuscatedUpper = -1195527588; //SysDEX
	//ctlMem->ObfuscatedIdentifiers[8].m_midObfuscatedLower = 690668251; //rosssssssssieboyl/Ross / Discord. //Paid nath 10/02/2025 Paypal).
	//ctlMem->ObfuscatedIdentifiers[8].m_xuidObfuscatedUpper = -229879760;  //Aryieta - CHANGED FROM Aryietu 

	//ctlMem->ObfuscatedIdentifiers[8].m_midObfuscatedLower = 866649471; //Ross (Paid for third re-auth)
	//ctlMem->ObfuscatedIdentifiers[8].m_xuidObfuscatedUpper = -1990812688; //iehla (GT changed from x Aryieta /same XUID)

	ctlMem->ObfuscatedIdentifiers[8].m_midObfuscatedLower = -1061210401; //Ross (4th and final)
	ctlMem->ObfuscatedIdentifiers[8].m_xuidObfuscatedUpper = -470739467; //Ross


	ctlMem->ObfuscatedIdentifiers[9].m_midObfuscatedLower = -764393856; //bullshit fucking with azl
	ctlMem->ObfuscatedIdentifiers[9].m_xuidObfuscatedUpper = 65754555; //bullshit fucking with azl //hash for "fucked" string
	//ctlMem->ObfuscatedIdentifiers[9].m_midObfuscatedLower = ctlMem->ObfuscatedIdentifiers[8].m_midObfuscatedLower; //rosssssssssieboyl / Ross / Discord.
	//ctlMem->ObfuscatedIdentifiers[9].m_xuidObfuscatedUpper = -1990812688; //x Aryieta

	//RYAN NOW REQUIRES 2 ORDERS TO RESTORE "1 ahead" POSITION
	ctlMem->ObfuscatedIdentifiers[10].m_midObfuscatedLower = -1024037377; //IGON17 / Paid Ryan 27/02/2025 - Cashapp
	ctlMem->ObfuscatedIdentifiers[10].m_xuidObfuscatedUpper = 379936736; //IGON17 / Paid Ryan 27/02/2025 - Cashapp

	ctlMem->ObfuscatedIdentifiers[11].m_midObfuscatedLower = -1538485877; //KV Ban 1998008891; //Lewis x2 - Paid for SOMBRA - yHolyz
	ctlMem->ObfuscatedIdentifiers[11].m_xuidObfuscatedUpper = -884471224; //Lewis x2 - Paid for SOMBRA - yHolyz
	//Position reset - Ryan requires 2 again to get ahead.
	//ctlMem->ObfuscatedIdentifiers[12].m_midObfuscatedLower = 964683391; //BANNED x2 //SirSmokeALot - Paid Ryan 07/03/2025 - Assuming Cashapp
	//ctlMem->ObfuscatedIdentifiers[12].m_midObfuscatedLower = 431558975; //BANNED SirSmokeALot - Paid Ryan 07/03/2025 - Assuming Cashapp
	//ctlMem->ObfuscatedIdentifiers[12].m_midObfuscatedLower = -167540449; //BANNED x3
	ctlMem->ObfuscatedIdentifiers[12].m_midObfuscatedLower = -2100567589; //SirSmokeAlot
	ctlMem->ObfuscatedIdentifiers[12].m_xuidObfuscatedUpper = 1484643763; //SirSmokeALot - Paid Ryan 07/03/2025 - Assuming Cashapp
	ctlMem->ObfuscatedIdentifiers[13].m_midObfuscatedLower = -1122882049; //Yzz71 Discord - Nath 16/03/2025
	ctlMem->ObfuscatedIdentifiers[13].m_xuidObfuscatedUpper = -1311016209; //Yzz71 / L33xs - Nath 16/03/2025
	ctlMem->ObfuscatedIdentifiers[14].m_midObfuscatedLower = 1627516683; //Recur Pain - Ryan 02/04/2025
	ctlMem->ObfuscatedIdentifiers[14].m_xuidObfuscatedUpper = 898132237; //Recut Pain

	ctlMem->ObfuscatedIdentifiers[15].m_midObfuscatedLower = 1211113579; //AUTH 2 (used free re-auth was: -1165532549;) //Capo - Ryan
	ctlMem->ObfuscatedIdentifiers[15].m_xuidObfuscatedUpper = 701039991; //ImBoutaBuss7532  (Capo)
	//Fair n Square.
	ctlMem->ObfuscatedIdentifiers[16].m_midObfuscatedLower = 1201785947; //radiation snake
	ctlMem->ObfuscatedIdentifiers[16].m_xuidObfuscatedUpper = 1234377122; //radiation snake - Nathan

	ctlMem->ObfuscatedIdentifiers[17].m_midObfuscatedLower = 1092121163; //LE - Nath (ex-bitch)
	ctlMem->ObfuscatedIdentifiers[17].m_xuidObfuscatedUpper = -1618003094; //LE / Xt9opL4kt1yX

	ctlMem->ObfuscatedIdentifiers[18].m_midObfuscatedLower = -2016344065; // Tunksy - remove guys a user
	ctlMem->ObfuscatedIdentifiers[18].m_xuidObfuscatedUpper = -907505394; //Tunksy XeTMOOU /naths friend //testing for cipher //was no 1 but took the piss with auth.
	//uae66 - Nath (ex-bitch) //PAID TWICE 1x FOR BROTHER THAT HAS NO DISOCRD, (Not to be confused with LE) 
	//ctlMem->ObfuscatedIdentifiers[19].m_midObfuscatedLower = 521717311;   //UAE6666 //No KV Mode
	ctlMem->ObfuscatedIdentifiers[19].m_midObfuscatedLower = -521321525;   //UAE6666
	ctlMem->ObfuscatedIdentifiers[19].m_xuidObfuscatedUpper = 1285504908;


	//[Cartel - Script] ctlMem->ObfuscatedIdentifiers[x].m_midObfuscatedLower = -377491349
	//ctlMem->ObfuscatedIdentifiers[20].m_midObfuscatedLower = -1068594913; //UAE666 x2 (QxMxO) //No KV Mode
	ctlMem->ObfuscatedIdentifiers[20].m_midObfuscatedLower = -377491349; //UAE666 x2 (QxMxO)
	ctlMem->ObfuscatedIdentifiers[20].m_xuidObfuscatedUpper = 1328372779;


	//SECOND SLOT PAID TWICE - uae66 - Nath (ex-bitch)
	//Ryan requires 3 again to get ahead.
	ctlMem->ObfuscatedIdentifiers[21].m_midObfuscatedLower = 2089404319; //m1urka (ivnutter) Nath 30/05/2025
	ctlMem->ObfuscatedIdentifiers[21].m_xuidObfuscatedUpper = 445150062;

	ctlMem->ObfuscatedIdentifiers[22].m_midObfuscatedLower = ctlMem->ObfuscatedIdentifiers[21].m_midObfuscatedLower; //ivnutter second auth 27/Aug/25 (no more free auths).
	//ctlMem->ObfuscatedIdentifiers[22].m_xuidObfuscatedUpper = -1132384535; //Esy Wrk (ivnutter)
	ctlMem->ObfuscatedIdentifiers[22].m_xuidObfuscatedUpper = 969351350; //W3trz

	//PING - Ryan Amazon June 20th, 2025.
	// [Cartel - Script] ctlMem->ObfuscatedIdentifiers[x].m_xuidObfuscatedUpper = 115304294
	//ctlMem->ObfuscatedIdentifiers[23].m_xuidObfuscatedUpper = 115304294; //TBPing
	//ctlMem->ObfuscatedIdentifiers[23].m_midObfuscatedLower = -83383764;
	//2 more for ryan to get level i think ---------------------------------------------------------------------------------------------------
	ctlMem->ObfuscatedIdentifiers[23].m_midObfuscatedLower = -1372678389; //E2RY / Izzy (Discord ID: 1140649947025395792) Giveaway winner (13/12/2025)
	ctlMem->ObfuscatedIdentifiers[23].m_xuidObfuscatedUpper = 571228063;

	//below doesn't apply as been trying to auth this dude forever and he isn't paying. (Old friend)
	ctlMem->ObfuscatedIdentifiers[24].m_midObfuscatedLower = 1657288091; //FreshOnRGH (FBModderFresh naths bud, been trying to auth forever).
	ctlMem->ObfuscatedIdentifiers[24].m_xuidObfuscatedUpper = -1451288914; //FreshONRGH

	ctlMem->ObfuscatedIdentifiers[25].m_midObfuscatedLower = -1674716545; //Koltz / "JESUS IS KING" (nathan amazon)
	ctlMem->ObfuscatedIdentifiers[25].m_xuidObfuscatedUpper = 515448681; //JESUS IS KING


	ctlMem->ObfuscatedIdentifiers[26].m_midObfuscatedLower = 640841083; //WarmedWhistle00 - Nath (Discord = gfizz3758 (GFizz))
	ctlMem->ObfuscatedIdentifiers[26].m_xuidObfuscatedUpper = 1478728866; ///WarmedWhistle00

	ctlMem->ObfuscatedIdentifiers[27].m_midObfuscatedLower = 495568203; //Kernel Aim - Ryan (Discord = just_the_basic69)
	ctlMem->ObfuscatedIdentifiers[27].m_xuidObfuscatedUpper = -237952185;

	ctlMem->ObfuscatedIdentifiers[28].m_midObfuscatedLower = -770342581; //Optus IV
	ctlMem->ObfuscatedIdentifiers[28].m_xuidObfuscatedUpper = -327590742; //Optus IV

	ctlMem->ObfuscatedIdentifiers[29].m_midObfuscatedLower = 766377483; //ziehla (dude decided to use his NAND MID to join me instead of his KV?) - Maybe tryna get both authed but doesn't work like that.
	ctlMem->ObfuscatedIdentifiers[29].m_xuidObfuscatedUpper = -1990812688; //ziehla (Ross x2)

	ctlMem->ObfuscatedIdentifiers[30].m_midObfuscatedLower = 1345154796;
	ctlMem->ObfuscatedIdentifiers[30].m_xuidObfuscatedUpper = ctlMem->ObfuscatedIdentifiers[10].m_xuidObfuscatedUpper; //IGON17 / Second agreed console auth. //379936736;

	ctlMem->ObfuscatedIdentifiers[31].m_midObfuscatedLower = -1695136993; //Lidl Xb360 - Discord ID: 595427535966175243 (ExZoAlpacino (Lidl Xb360) Nath 20/11/25)
	ctlMem->ObfuscatedIdentifiers[31].m_xuidObfuscatedUpper = -1601881881; //Lidl Xb360

	ctlMem->ObfuscatedIdentifiers[32].m_midObfuscatedLower = 1874272427; //Sice - Discord ID: 698262002614927471 (saint - Nath 21/11/25)
	ctlMem->ObfuscatedIdentifiers[32].m_xuidObfuscatedUpper = 1287662244;

	ctlMem->ObfuscatedIdentifiers[33].m_midObfuscatedLower = 826102220; //TiberiusLares - Discord ID: 143552230082084864 (Joey - Nath (Keeping storm sweet then Ryans turn) - 25/11/2025
	ctlMem->ObfuscatedIdentifiers[33].m_xuidObfuscatedUpper = 648881303; 

	ctlMem->ObfuscatedIdentifiers[34].m_midObfuscatedLower = 1389506251; //Token7s
	ctlMem->ObfuscatedIdentifiers[34].m_xuidObfuscatedUpper = 1054708230; //Token7s

	ctlMem->ObfuscatedIdentifiers[35].m_midObfuscatedLower = -1267878293; //CaliCa1iCali (RGH Winner from/Lweis) - Discord ID: 286910770770477056 - (Tootankhamunッ) 30/11/25 (Giveaway RGH Winner)
	ctlMem->ObfuscatedIdentifiers[35].m_xuidObfuscatedUpper = 596541936;

	ctlMem->ObfuscatedIdentifiers[36].m_midObfuscatedLower = -1171282821; //"AKA Bea5tly" - Discord ID: 673384791843799080 - (DJ85) 30/11/25 Nath - Getting Ryan voucher) Se7enSins: "AKA Bea5tly" same as gt
	ctlMem->ObfuscatedIdentifiers[36].m_xuidObfuscatedUpper = -1993875833;

	ctlMem->ObfuscatedIdentifiers[37].m_midObfuscatedLower = 164405599; //LordDarkness540 - Discord ID: 506182118490243072 (I'm vengance) - Nath 01/12/2025 - Se7enSins: Thunderous2013
	ctlMem->ObfuscatedIdentifiers[37].m_xuidObfuscatedUpper = 509860794;

	ctlMem->ObfuscatedIdentifiers[38].m_midObfuscatedLower = -1956297601; //Nnn3953 - Discord ID: 1431334673027567707 (Niko Bellic/nikobellic0426) - Nath 15/12/2025 
	ctlMem->ObfuscatedIdentifiers[38].m_xuidObfuscatedUpper = 794105579;

	ctlMem->ObfuscatedIdentifiers[39].m_midObfuscatedLower = -732591029; //f2v v zr0 - Discord ID: 659106074040991756 (f2z) - Second Menu giveaway winner
	ctlMem->ObfuscatedIdentifiers[39].m_xuidObfuscatedUpper = -306063577;

	ctlMem->ObfuscatedIdentifiers[40].m_midObfuscatedLower = 138975167; //"The GOAT"s buddy 
	ctlMem->ObfuscatedIdentifiers[40].m_xuidObfuscatedUpper = -1318285017;

	ctlMem->ObfuscatedIdentifiers[41].m_midObfuscatedLower = -1481832033; //Bean ivi - Discord ID: 495432531634683904 (Sean) - Ryan 30/12/25
	ctlMem->ObfuscatedIdentifiers[41].m_xuidObfuscatedUpper = 1849522553;

	ctlMem->ObfuscatedIdentifiers[42].m_midObfuscatedLower = -366399969; //Fuzznox - Discord ID: 1120085641636614254 (Havoc Staff) - Nath 31/12/2025 (Aaand now writing this it's 2026 wooo bang boom what a racket).
	ctlMem->ObfuscatedIdentifiers[42].m_xuidObfuscatedUpper = -1154057096;

	ctlMem->ObfuscatedIdentifiers[43].m_midObfuscatedLower = 1959565899; //xPlaugeuisx - Discord ID: 768932066146123807 (wyatt6074) - Ryan 01/01/2026
	ctlMem->ObfuscatedIdentifiers[43].m_xuidObfuscatedUpper = -2010761732;

	ctlMem->ObfuscatedIdentifiers[44].m_midObfuscatedLower = 328523999; //Whssa - Discord ID: 747319948359106652 (Kohh) - Nath 01/01/2026
	ctlMem->ObfuscatedIdentifiers[44].m_xuidObfuscatedUpper = 372793147;

	ctlMem->ObfuscatedIdentifiers[45].m_midObfuscatedLower = 1789476863; //COBRARGH30 - Discord ID: 635222353101455381 (White0pzz) - Ryan 05/01/2026
	ctlMem->ObfuscatedIdentifiers[45].m_xuidObfuscatedUpper = -1895411510;

	ctlMem->ObfuscatedIdentifiers[46].m_midObfuscatedLower = -2118840789; //Hondak20Swap - Discord ID: 614178335207063552 (W arp) - Nath 09/01/2026
	ctlMem->ObfuscatedIdentifiers[46].m_xuidObfuscatedUpper = -2078968431;

	ctlMem->ObfuscatedIdentifiers[47].m_midObfuscatedLower = -1488095445; // Rob Torzilli - Discord ID: 664641262913323040 (Smosny) - Ryan 10/01/2026
	ctlMem->ObfuscatedIdentifiers[47].m_xuidObfuscatedUpper = 1472600284; 

	ctlMem->ObfuscatedIdentifiers[48].m_midObfuscatedLower = -1954059909; //AbsurdSpy4689 - Discord ID: 1456424554670067724 (Moving forward) - Nath 11/01/2026
	ctlMem->ObfuscatedIdentifiers[48].m_xuidObfuscatedUpper = -892688061;

	ctlMem->ObfuscatedIdentifiers[49].m_midObfuscatedLower = 127651675; //Outlaws8411 - Discord ID: 1045173237056798850 (cheathra) - Nath 24/01/2026 (Paypal: Chase Thrash)
	ctlMem->ObfuscatedIdentifiers[49].m_xuidObfuscatedUpper = -1714762027;

	//ctlMem->ObfuscatedIdentifiers[50].m_midObfuscatedLower = -754134405; //banned
	ctlMem->ObfuscatedIdentifiers[50].m_midObfuscatedLower = 1541961196;
	ctlMem->ObfuscatedIdentifiers[50].m_xuidObfuscatedUpper = -1678513066; //DivineCave8759 - Discord ID: 721879269059330098 (Dylan-L / dylanl9376) - Pulled from PlayerDB and converted PAINFULLY by programming Cartel-Bot - Nath 28/Jan/2026) (Paypal: Dylan Lynch)

	ctlMem->ObfuscatedIdentifiers[51].m_midObfuscatedLower = 37880635; //vi UAE iv - //!3zf (nwe9r) Discord ID: 839577679656910859 - (Ryan)
	ctlMem->ObfuscatedIdentifiers[51].m_xuidObfuscatedUpper = -1184698088;

	ctlMem->ObfuscatedIdentifiers[52].m_midObfuscatedLower = -917207617;
	ctlMem->ObfuscatedIdentifiers[52].m_xuidObfuscatedUpper = 1743278492; //SYREN X W4LF - Discord ID: 853356940288589864 (🌸SyrenWolf | LaylaV2🌸 / syren_wolf_laya_v2) - Nath PayPal: KaiDvon Durham

	
	//STILL PENDING ctlMem->ObfuscatedIdentifiers[50].m_midObfuscatedLower = 1456719451; //Lastereolife - Discord ID: 1423426495300374633 (Doni) - 
	//STILL PENDING ctlMem->ObfuscatedIdentifiers[50].m_xuidObfuscatedUpper = 1364224934;


	//ctlMem->ObfuscatedIdentifiers[38].m_midObfuscatedLower = -1892270613; //eatmyassfodolla 2 (slim) //moved to moderator. 
	//ctlMem->ObfuscatedIdentifiers[38].m_xuidObfuscatedUpper = ctlMem->ObfuscatedIdentifiers[3].m_xuidObfuscatedUpper; //eatmyassfodolla2

	//testing
	//ctlMem->ObfuscatedIdentifiers[40].m_midObfuscatedLower = 571770667; //Nath spare RGH KV (Admin Key). //hopefully doesn't get banned when the other does.
	//ctlMem->ObfuscatedIdentifiers[40].m_xuidObfuscatedUpper = 1792498931; //Dank Functions XUID obfuscated (Unique half). 
    ctlMem->ObfuscatedIdentifiers[53].m_midObfuscatedLower = 1919642395; //tag here - Discord ID: 885828340924158033 (Professional / outspend)
    ctlMem->ObfuscatedIdentifiers[53].m_xuidObfuscatedUpper = 1273180509; //Added for dankfunctions by Cartel-Serv bot 2026-02-08 13:47:31 UTC.
#endif
#endif
}

void save_data_to_gamesave(int unused1, int unused2)
{
#ifdef Developer
	PRINTSTRING("Ssaving  data to gamesave\n");
	PRINTSTRING("Start: save_data_to_gamesave(), ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES == ");
	PRINTINT(IS_BIT_SET(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES));
	PRINTNL();
#endif
	saveData_storage gameSave;
	gameSave.m_weaponBits = ctlMem->m_bit_sets.m_weapons;
	gameSave.m_playerBits = ctlMem->m_bit_sets.m_player;
	bitReset(&gameSave.m_playerBits, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED);
	gameSave.m_vehicleBits = ctlMem->m_bit_sets.m_vehicle;
	bitReset(&gameSave.m_vehicleBits, SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED);
	bitReset(&gameSave.m_playerBits, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED);
	gameSave.m_menuPreferencesBits = ctlMem->m_bit_sets.m_menuPreferences;
	bitReset(&gameSave.m_menuPreferencesBits, PREF_SETTING_ACCEPTED_CHAT_RULES); //might get rid of this
	gameSave.m_stickyBits = *(int*)stickyBitSet;


	gameSave.SBYT[SBYT_TIME_HOUR] = (byte)time_weather_data.m_timeHour;
	gameSave.SBYT[SBYT_TIME_MINUTE] = (byte)time_weather_data.m_timeMinute;
	gameSave.SBYT[SBYT_WEATHER] = (byte)time_weather_data.m_weather;
	gameSave.SBYT[SBYT_SAVE_DATA_VERSION] = SAVE_DATA_VERSION_BYTE;
	gameSave.SBYT[SBYT_MENU_BUTTON_HOLD] = (byte)Container.Ui.OpenButton1;
	gameSave.SBYT[SBYT_MENU_BUTTON_PRESS] = (byte)Container.Ui.OpenButton2;
	gameSave.SBYT[SBYT_HEADER_COL_1] = (byte)Container.Ui.HeaderColor.r;
	gameSave.SBYT[SBYT_HEADER_COL_2] = (byte)Container.Ui.HeaderColor.g;
	gameSave.SBYT[SBYT_HEADER_COL_3] = (byte)Container.Ui.HeaderColor.b;
	gameSave.SBYT[SBYT_HEADER_ALPHA] = (byte)Container.Ui.HeaderColor.a;
	gameSave.SBYT[SBYT_HEADERTXT_COL_1] = (byte)Container.Ui.HeaderTextColor.r;
	gameSave.SBYT[SBYT_HEADERTXT_COL_2] = (byte)Container.Ui.HeaderTextColor.g;
	gameSave.SBYT[SBYT_HEADERTXT_COL_3] = (byte)Container.Ui.HeaderTextColor.b;
	gameSave.SBYT[SBYT_HEADERTXT_ALPHA] = (byte)Container.Ui.HeaderTextColor.a;
	gameSave.SBYT[SBYT_SUBTITLE_COL_1] = (byte)Container.Ui.SubtitleColor.r;
	gameSave.SBYT[SBYT_SUBTITLE_COL_2] = (byte)Container.Ui.SubtitleColor.g;
	gameSave.SBYT[SBYT_SUBTITLE_COL_3] = (byte)Container.Ui.SubtitleColor.b;
	gameSave.SBYT[SBYT_SUBTITLE_ALPHA] = (byte)Container.Ui.SubtitleColor.a;
	gameSave.SBYT[SBYT_TEXT_COL_1] = (byte)Container.Ui.TextColor.r;
	gameSave.SBYT[SBYT_TEXT_COL_2] = (byte)Container.Ui.TextColor.g;
	gameSave.SBYT[SBYT_TEXT_COL_3] = (byte)Container.Ui.TextColor.b;
	gameSave.SBYT[SBYT_TEXT_ALPHA] = (byte)Container.Ui.TextColor.a;
	gameSave.SBYT[SBYT_BACKGROUND_COL_1] = (byte)Container.Ui.BackgroundColor.r;
	gameSave.SBYT[SBYT_BACKGROUND_COL_2] = (byte)Container.Ui.BackgroundColor.g;
	gameSave.SBYT[SBYT_BACKGROUND_COL_3] = (byte)Container.Ui.BackgroundColor.b;
	gameSave.SBYT[SBYT_BACKGROUND_ALPHA] = (byte)Container.Ui.BackgroundColor.a;
	gameSave.SBYT[SBYT_SEL_TEXT_COL_1] = (byte)Container.Ui.SelectedTextColor.r;
	gameSave.SBYT[SBYT_SEL_TEXT_COL_2] = (byte)Container.Ui.SelectedTextColor.g;
	gameSave.SBYT[SBYT_SEL_TEXT_COL_3] = (byte)Container.Ui.SelectedTextColor.b;
	gameSave.SBYT[SBYT_SEL_TEXT_ALPHA] = (byte)Container.Ui.SelectedTextColor.a;
	gameSave.SBYT[SBYT_SCROLLBAR_COL_1] = (byte)Container.Ui.ScrollBarColor.r;
	gameSave.SBYT[SBYT_SCROLLBAR_COL_2] = (byte)Container.Ui.ScrollBarColor.g;
	gameSave.SBYT[SBYT_SCROLLBAR_COL_3] = (byte)Container.Ui.ScrollBarColor.b;
	gameSave.SBYT[SBYT_SCROLLBAR_ALPHA] = (byte)Container.Ui.ScrollBarColor.a;

	gameSave.SBYT[SBYT_CHAT_MSG_DISPLAY_SECONDS] = (byte)ctlMem->m_chat_msg_display_time_seconds;

	byte* HealthColor = (byte*)GetHealthHUDColourAddress();
	gameSave.SBYT[SBYT_HEALTH_COL_1] = HealthColor[0];
	gameSave.SBYT[SBYT_HEALTH_COL_2] = HealthColor[1];
	gameSave.SBYT[SBYT_HEALTH_COL_3] = HealthColor[2];

	byte* ArmorColor = (byte*)GetArmourHUDColourAddress();
	gameSave.SBYT[SBYT_ARMOUR_COL_1] = ArmorColor[0];
	gameSave.SBYT[SBYT_ARMOUR_COL_2] = ArmorColor[1];
	gameSave.SBYT[SBYT_ARMOUR_COL_3] = ArmorColor[2];

	byte* PauseColor = (byte*)0x82FB30BD; //this is the one we set / gets copied over when user sets
	gameSave.SBYT[SBYT_PAUSE_TXT_COL_1] = PauseColor[0];
	gameSave.SBYT[SBYT_PAUSE_TXT_COL_2] = PauseColor[1];
	gameSave.SBYT[SBYT_PAUSE_TXT_COL_3] = PauseColor[2];

	gameSave.menuPos = Container.Ui.DrawPos;

	gameSave.m_savedVehicleModel = freeMem->m_savedVehicleModel;
	gameSave.m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits = freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits; //first 10 = upgrades, windows = 4, doors = 4, tyres = 6
	for (int i = 0; i < 4; i++)
		gameSave.m_savedVehicleColours[i] = freeMem->m_savedVehicleColours[i];

	memcpy((int*)GAMESAVE_LOCATION, &gameSave, sizeof(saveData_storage)); //copy data into gamesave
	memcpy(freeMem->m_saved_data_copy, (int*)GAMESAVE_LOCATION, sizeof(saveData_storage)); //copy data to freeMem so we can grab in MP.
	//SET_MISSION_FLAG(false);
	DO_AUTO_SAVE(); //don't need to print as help text comes up
	//SET_MISSION_FLAG(true);

	if (IS_PLAYER_ONLINE())
	{
		CopyGamesaveToOnlineScores();
		//UploadScores(); //called in above function
	}
	else
		freeMem->m_carryover_mode = SAVE_CARRY_TO_MP;

#ifdef Developer
	PRINTSTRING("End: save_data_to_gamesave(), ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES == ");
	PRINTINT(IS_BIT_SET(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES));
	PRINTNL();
#endif
}

/*
void read_data_from_gamesave(int unused1, int unused2) //assumes the save has been loaded (usually is automatically).
{
#ifdef Developer
	PRINTSTRING("reading data from gamesave\n");
	PRINTSTRING("Start: read_data_from_gamesave(), ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES == ");
	PRINTINT(IS_BIT_SET(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES));
	PRINTNL();
#endif
	saveData_storage gameSave;
	memcpy(&gameSave, (int*)GAMESAVE_LOCATION, sizeof(saveData_storage));
	if (gameSave.SBYT[SBYT_SAVE_DATA_VERSION] != SAVE_DATA_VERSION_BYTE)
	{
		printf("~r~CTL: ~w~Saved data is incompatible with current version, re-save to overwrite.");
		return;
	}

	bool MIDsecurityBit = bitTest(ctlMem->m_bit_sets.m_vehicle, SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED);
	bool XUIDsecurityBit = bitTest(ctlMem->m_bit_sets.m_player, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED);

	if (bitTest(gameSave.m_stickyBits, STICK_ADVERTISE_MENU))
		bitSet((int*)stickyBitSet, STICK_ADVERTISE_MENU);

	bool StoreXUIDFuncCalled = bitTest(ctlMem->m_bit_sets.m_player, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED);

	ctlMem->m_bit_sets.m_player = gameSave.m_playerBits;
	ctlMem->m_bit_sets.m_weapons = gameSave.m_weaponBits;
	ctlMem->m_bit_sets.m_vehicle = gameSave.m_vehicleBits;

	if(bitTest(ctlMem->m_bit_sets.m_vehicle, local_autoflip))
		SET_BIT(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (myPlayerIndex + 16));
	//ADMIN_SECURITY_BIT_GET_XUID_FROM_FREE_MEM_CALLED;
	//ADMIN_SECURITY_BIT_MACHINE_ID_MATCHED
	//bool MIDMatchedAdminSet = bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SECURITY_BIT_MACHINE_ID_MATCHED);

	bitReset(&ctlMem->m_bit_sets.m_player, inChauffeurSubmenu);
	bitReset(&ctlMem->m_bit_sets.m_player, sharedIsSelectedPlayerMuted);
	if (MIDsecurityBit)
		bitSet(&ctlMem->m_bit_sets.m_vehicle, SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED);

	if (XUIDsecurityBit)
		bitSet(&ctlMem->m_bit_sets.m_player, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED);

	ctlMem->m_bit_sets.m_menuPreferences = gameSave.m_menuPreferencesBits;

	if (bitTest(gameSave.m_stickyBits, STICK_USE_CARTEL_SPAWN))
		bitSet((int*)stickyBitSet, STICK_USE_CARTEL_SPAWN);

	//DISABLE_PLAYER_AUTO_VEHICLE_EXIT(myPlayerPed, IS_BIT_SET(ctlMem->m_bit_sets.m_vehicle, blockVehicleEjectHijack)); //kudos stuart
	SWITCH_AMBIENT_PLANES(!IS_BIT_SET(ctlMem->m_bit_sets.m_vehicle, disabledRunwayPlanes));

	time_weather_data.m_timeHour = (int)gameSave.SBYT[SBYT_TIME_HOUR];
	time_weather_data.m_timeMinute = (int)gameSave.SBYT[SBYT_TIME_MINUTE];
	time_weather_data.m_weather = (uint)gameSave.SBYT[SBYT_WEATHER];
	Container.Ui.OpenButton1 = gameSave.SBYT[SBYT_MENU_BUTTON_HOLD];
	Container.Ui.OpenButton2 = gameSave.SBYT[SBYT_MENU_BUTTON_PRESS];
	Container.Ui.HeaderColor.r = gameSave.SBYT[SBYT_HEADER_COL_1];
	Container.Ui.HeaderColor.g = gameSave.SBYT[SBYT_HEADER_COL_2];
	Container.Ui.HeaderColor.b = gameSave.SBYT[SBYT_HEADER_COL_3];
	Container.Ui.HeaderColor.a = gameSave.SBYT[SBYT_HEADER_ALPHA];
	Container.Ui.HeaderTextColor.r = gameSave.SBYT[SBYT_HEADERTXT_COL_1];
	Container.Ui.HeaderTextColor.g = gameSave.SBYT[SBYT_HEADERTXT_COL_2];
	Container.Ui.HeaderTextColor.b = gameSave.SBYT[SBYT_HEADERTXT_COL_3];
	Container.Ui.HeaderTextColor.a = gameSave.SBYT[SBYT_HEADERTXT_ALPHA];
	Container.Ui.SubtitleColor.r = gameSave.SBYT[SBYT_SUBTITLE_COL_1];
	Container.Ui.SubtitleColor.g = gameSave.SBYT[SBYT_SUBTITLE_COL_2];
	Container.Ui.SubtitleColor.b = gameSave.SBYT[SBYT_SUBTITLE_COL_3];
	Container.Ui.SubtitleColor.a = gameSave.SBYT[SBYT_SUBTITLE_ALPHA];
	Container.Ui.TextColor.r = gameSave.SBYT[SBYT_TEXT_COL_1];
	Container.Ui.TextColor.g = gameSave.SBYT[SBYT_TEXT_COL_2];
	Container.Ui.TextColor.b = gameSave.SBYT[SBYT_TEXT_COL_3];
	Container.Ui.TextColor.a = gameSave.SBYT[SBYT_TEXT_ALPHA];
	Container.Ui.BackgroundColor.r = gameSave.SBYT[SBYT_BACKGROUND_COL_1];
	Container.Ui.BackgroundColor.g = gameSave.SBYT[SBYT_BACKGROUND_COL_2];
	Container.Ui.BackgroundColor.b = gameSave.SBYT[SBYT_BACKGROUND_COL_3];
	Container.Ui.BackgroundColor.a = gameSave.SBYT[SBYT_BACKGROUND_ALPHA];
	Container.Ui.SelectedTextColor.r = gameSave.SBYT[SBYT_SEL_TEXT_COL_1];
	Container.Ui.SelectedTextColor.g = gameSave.SBYT[SBYT_SEL_TEXT_COL_2];
	Container.Ui.SelectedTextColor.b = gameSave.SBYT[SBYT_SEL_TEXT_COL_3];
	Container.Ui.SelectedTextColor.a = gameSave.SBYT[SBYT_SEL_TEXT_ALPHA];
	Container.Ui.ScrollBarColor.r = gameSave.SBYT[SBYT_SCROLLBAR_COL_1];
	Container.Ui.ScrollBarColor.g = gameSave.SBYT[SBYT_SCROLLBAR_COL_2];
	Container.Ui.ScrollBarColor.b = gameSave.SBYT[SBYT_SCROLLBAR_COL_3];
	Container.Ui.ScrollBarColor.a = gameSave.SBYT[SBYT_SCROLLBAR_ALPHA];

	freeMem->m_savedVehicleModel = gameSave.m_savedVehicleModel;
	freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits = gameSave.m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits; //first 10 = upgrades, windows = 4, doors = 4, tyres = 6
	for (int i = 0; i < 4; i++)
		freeMem->m_savedVehicleColours[i] = gameSave.m_savedVehicleColours[i];

	ctlMem->m_chat_msg_display_time_seconds = gameSave.SBYT[SBYT_CHAT_MSG_DISPLAY_SECONDS];

	byte* HealthColor = (byte*)GetHealthHUDColourAddress();
	HealthColor[0] = gameSave.SBYT[SBYT_HEALTH_COL_1];
	HealthColor[1] = gameSave.SBYT[SBYT_HEALTH_COL_2];
	HealthColor[2] = gameSave.SBYT[SBYT_HEALTH_COL_3];

	byte* ArmorColor = (byte*)GetArmourHUDColourAddress();
	ArmorColor[0] = gameSave.SBYT[SBYT_ARMOUR_COL_1];
	ArmorColor[1] = gameSave.SBYT[SBYT_ARMOUR_COL_2];
	ArmorColor[2] = gameSave.SBYT[SBYT_ARMOUR_COL_3];

	byte* PauseColor = (byte*)0x82FB30BD; //this is the one we set
	PauseColor[0] = gameSave.SBYT[SBYT_PAUSE_TXT_COL_1];
	PauseColor[1] = gameSave.SBYT[SBYT_PAUSE_TXT_COL_2];
	PauseColor[2] = gameSave.SBYT[SBYT_PAUSE_TXT_COL_3];
	setPauseColours(0, 0); //carries these over to the other bytes for pause txt colour

	Container.Ui.DrawPos = gameSave.menuPos;

	if (StoreXUIDFuncCalled)
		bitSet(&ctlMem->m_bit_sets.m_player, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED);

	update_menu_open_controls_string();

	ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
	printf("~r~CTL: ~w~Settings loaded.");

	if (!bitTest(*(int*)stickyBitSet, STICK_XEX_PRESENT))
		bitReset(&ctlMem->m_bit_sets.m_player, syncTimeToRealWorld);

	if (!NetworkGame && GET_BITS_IN_RANGE(ctlMem->m_bit_sets.m_player, attachedToWalker, printedNewDickheadIdentifiers))
	{
		saveData_storage data = *(saveData_storage*)GAMESAVE_LOCATION;
		bitReset(&ctlMem->m_bit_sets.m_player, attachedToWalker); //might not need this
		bitReset(&ctlMem->m_bit_sets.m_player, printedNewDickheadIdentifiers); //might not need this
		bitReset(&ctlMem->m_bit_sets.m_player, networkRank10All); //probably keep this as is, newtwork bitset is mainly runtime bits / shit user shouldn't really be wanting to change or save.
		bitReset(&data.m_playerBits, attachedToWalker);
		bitReset(&data.m_playerBits, printedNewDickheadIdentifiers);
		bitReset(&data.m_playerBits, networkRank10All); //probably keep this as is, newtwork bitset is mainly runtime bits / shit user shouldn't really be wanting to change or save.
		DO_AUTO_SAVE(); 
	}
#ifdef Developer
	PRINTSTRING("End: read_data_from_gamesave(), ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES == ");
	PRINTINT(IS_BIT_SET(ctlMem->m_bit_sets.m_menuPreferences, PREF_HIDE_PLAYER_NAMES));
	PRINTNL();
#endif
}

*/

////115-140 //25 = savedata storage / carryover /scores
void CopyGamesaveToOnlineScores() //ideally we just came from SP.
{
#ifdef Developer
	PRINTSTRING("Copying gamesave to online scores\n");
#endif
	unsigned int score_data[25];
	memcpy(score_data, freeMem->m_saved_data_copy, 100); 
	const uint base_score = (myCurrentEpisode == 0 ? 115 : 90);
	for (int i = 0; i < 25; i++) //only do the amount of bytes we need to
	{
		SET_ONLINE_SCORE(base_score + i, score_data[i]);
	}
	//printf("~r~CTL: ~w~Singleplayer settings copied to GTA IV servers.");
	UploadScores();
	//read_bits_from_online_scores(0, 0); //needs to run a bit later in MP //can probs remove.
}

void CopyOnlineScoreDataToGamesave() 
{
#ifdef Developer
	PRINTSTRING("Copying online score data to gamesave\n");
#endif
	const uint base_score = (myCurrentEpisode == 0 ? 115 : 90);
	unsigned int score_data[25];
	for (int i = 0; i < 25; i++) //only do the amount of bytes we need to
		score_data[i] = GET_ONLINE_SCORE(base_score + i);
	
	memcpy((int*)GAMESAVE_LOCATION, score_data, 100);
	//*(saveData_storage*)GAMESAVE_LOCATION = *(saveData_storage*)score_data;
	//read_data_from_gamesave(0, 0);
	dynamic_read_data_from_saved(true, 0);
	DO_AUTO_SAVE();
	printf("~r~CTL: ~w~Mutiplayer settings copied to singleplayer gamesave.");
}

void CarryOverSaveSettings()
{ 
	if (NetworkGame)
	{ 
		if (freeMem->m_carryover_mode == SAVE_CARRY_TO_MP) //SP -> MP //Carry data to multiplayer
		{
			CopyGamesaveToOnlineScores(); //copy data to scores
			dynamic_read_data_from_saved(0, 0); //read from scores
			//read_bits_from_online_scores(0, 0);
			freeMem->m_carryover_mode = SAVE_CARRY_NONE; //clear carryover
			//online loading triggered in register broadcast vars function Admin_Options.c
		}
	}
	else
	{
		if (freeMem->m_carryover_mode == SAVE_CARRY_TO_SP) //MP -> SP //carry data to singleplayer
		{
			CopyOnlineScoreDataToGamesave(); //can read these in SP if we been online so meh.
			freeMem->m_carryover_mode = SAVE_CARRY_NONE;
		}
		else 
		{
			saveData_storage* data = (saveData_storage*)GAMESAVE_LOCATION;
			if (data->SBYT[SBYT_SAVE_DATA_VERSION] == SAVE_DATA_VERSION_BYTE && bitTest(data->m_menuPreferencesBits, PREF_SP_AUTOLOAD))
			{
				//read_data_from_gamesave(0, 0);
				dynamic_read_data_from_saved(0, 0); //read from gamesave
			}
		}
	}
	////115-140 //25 = savedata storage / carryover /scores
	//freeMem->saved_data_carryover[sizeof(saveData_storage)];
	//need 100bytes of freemem address that won't clear.
}

//can archive this soon
//char ctlMem->m_char_buffers.keyboard_input_buffer[MAX_KEYBOARD_INPUT_SIZE];// = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
bool GrabKeyboardInputString(int max_size, int min_size, bool upper_case, bool block_spaces, bool block_special_chars, bool is_hex, bool clear_buffer) //global ctlMem->m_char_buffers.keyboard_input_buffer
{
	//By Nathan Muthafuckaaaaaaa
	if (clear_buffer)
	{
		for (int i = 0; i < MAX_KEYBOARD_INPUT_SIZE; i++)
			ctlMem->m_char_buffers.keyboard_input_buffer[i] = 0;
	}

	int Keys[DeclaredKeys] = {
			KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,
			KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
			KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U,
			KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
			KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6,
			KEY_7, KEY_8, KEY_9, KEY_FORWARDSLASH, KEY_COMMA, KEY_PERIOD, KEY_BACKSLASH, KEY_APOSTROPHE };

	char* KeyCharactersLower[DeclaredKeys] = {
		"a", "b", "c", "d", "e", "f", "g",
		"h", "i", "j", "k", "l", "m", "n",
		"o", "p", "q", "r", "s", "t", "u",
		"v", "w", "x", "y", "z",
		"0", "1", "2", "3", "4", "5", "6",
		"7", "8", "9", "/", ",", ".", "\"", "'" };

	char* KeyCharactersUpper[DeclaredKeys] = {
		"A", "B", "C", "D", "E", "F", "G",
		"H", "I", "J", "K", "L", "M", "N",
		"O", "P", "Q", "R", "S", "T", "U",
		"V", "W", "X", "Y", "Z",
		")", "!", "@", "#", "$", "%", "^", //@ on this line needs to be "
		"&", "*", "(", "?", "<", ">", "|" , "@" };

	if (upper_case)
	{
		for (int i = 0; i < 26; i++) //set all letters to uppercase
			KeyCharactersLower[i] = KeyCharactersUpper[i];
	}

	if (block_special_chars)
	{
		for (int i = 26; i < 37; i++) //force uppercase numbers to be numbers
			KeyCharactersUpper[i] = KeyCharactersLower[i];

		for (int i = 36; i < DeclaredKeys; i++) //set all other characters to 0
		{
			KeyCharactersLower[i] = "0";
			KeyCharactersUpper[i] = "0";
		}
	}

	while (!IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_ENTER))
	{
		int BackSpaceTick = 0;

		WAIT(0);

		
			if (NetworkGame)
			{
				//can't do network loop yet as it does interior loop and sets the character..
				//NetworkLoopedOptions(); //stop people sinking if drive/walk water goin
				AdminClientLoop(); //stop people evading admin cmds.
			}
			

		while (IS_GAME_KEYBOARD_KEY_PRESSED(KEY_BACKSPACE)) //picks up keyboard and chatpad.. //I think some inputs will require using IS_KEYBOARD not IS_GAME_KEYBOARD
		{
			WAIT(0);

			if (NetworkGame)
			{
				//can't do network loop yet as it does interior loop and sets the character..
				//NetworkLoopedOptions(); //stop people sinking if drive/walk water goin
				AdminClientLoop(); //stop people evading admin cmds.
			}

			ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);

			if (is_hex)
				printf("~r~CTL TEXT: ~w~0x%s", ctlMem->m_char_buffers.keyboard_input_buffer);
			else
				printf("~r~CTL TEXT: ~w~%s", ctlMem->m_char_buffers.keyboard_input_buffer);

			if (!IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_BACKSPACE)) //usually true for like a second after it's pressed i think?
			{
				if (BackSpaceTick < GetTickCount())
				{
					for (int i = (MAX_KEYBOARD_INPUT_SIZE - 1); i > -1; i--)
					{
						if (ctlMem->m_char_buffers.keyboard_input_buffer[i] != 0)
						{
							ctlMem->m_char_buffers.keyboard_input_buffer[i] = 0;
							break;
						}
					}

					BackSpaceTick = (GetTickCount() + 150);
				}
			}
		}

		if (!block_spaces && IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_SPACE))
		{
			for (int i = (MAX_KEYBOARD_INPUT_SIZE - 1); i > -1; i--)
			{
				if (ctlMem->m_char_buffers.keyboard_input_buffer[i] != 0)
				{
					ctlMem->m_char_buffers.keyboard_input_buffer[i + 1] = ' ';
					break;
				}
			}
		}

		bool shiftHeld = (IS_GAME_KEYBOARD_KEY_PRESSED(KEY_LEFT_SHIFT) || IS_GAME_KEYBOARD_KEY_PRESSED(KEY_RIGHT_SHIFT)); //Picks up both don't worry
		for (int i = 0; i < DeclaredKeys; i++)
		{
			if (IS_GAME_KEYBOARD_KEY_JUST_PRESSED(Keys[i]) || IS_KEYBOARD_KEY_JUST_PRESSED(Keys[i])) //Chatpad and Keyboard
			{
				if (ctlMem->m_char_buffers.keyboard_input_buffer[0] == 0 && clear_buffer)
					strcpy(ctlMem->m_char_buffers.keyboard_input_buffer, (shiftHeld ? KeyCharactersUpper[i] : KeyCharactersLower[i]), 96);
					//strcpy_s(ctlMem->m_char_buffers.keyboard_input_buffer, (shiftHeld ? KeyCharactersUpper[i] : KeyCharactersLower[i]));
				else
				{
					if (GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer) < max_size)
						stradd(ctlMem->m_char_buffers.keyboard_input_buffer, (shiftHeld ? KeyCharactersUpper[i] : KeyCharactersLower[i]), 96);
						//stradd_s(ctlMem->m_char_buffers.keyboard_input_buffer, (shiftHeld ? KeyCharactersUpper[i] : KeyCharactersLower[i]));
				}
			}
		}

		ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);

		if (is_hex)
			printf("~r~CTL TEXT: ~w~0x%s", ctlMem->m_char_buffers.keyboard_input_buffer);
		else
			printf("~r~CTL TEXT: ~w~%s", ctlMem->m_char_buffers.keyboard_input_buffer);

		//should make it flash _ using the string before ctlMem->m_char_buffers.keyboard_input_buffer whils ctlMem->m_char_buffers.keyboard_input_buffer is empty.
		if (IS_BUTTON_JUST_PRESSED(0, BUTTON_B) || IS_GAME_KEYBOARD_KEY_PRESSED(KEY_ESC))
		{
			ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
			printf("~r~CTL TEXT: ~w~You pressed ~PAD_B~ or escape, cancelled.");
			return false;
		}
	}

	//Should we allow people to join as nothing?? hmmmm....
	//if developer
	//return (ctlMem->m_char_buffers.keyboard_input_buffer[0] != 0);

	return (ctlMem->m_char_buffers.keyboard_input_buffer[0] != 0 && ctlMem->m_char_buffers.keyboard_input_buffer[0] != ' ' && (GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer) >= min_size)); //true if something has been typed.

	//return true; //!IS_STRING_NULL(ctlMem->m_char_buffers.keyboard_input_buffer);
}


#pragma region storms_keyboard
//char made_by_storm[27];
void decrypt_keyboard_creator() {
	int key[] = {
		0x3C, 0x42, 0x3E, 0x3C, 0x49,
		0x3E, 0x3C, 0x54, 0x3E, 0x3C,
		0x43, 0x3E, 0x3C, 0x48, 0x3E
	};

	int encrypted_storm[] = {
		0x11, 0x62, 0x1E, 0x71, 0x28, 0x5A, 0x59, 0x74,
		0x5C, 0x45, 0x63, 0x46, 0x6F, 0x3C, 0x51, 0x4E,
		0x2F, 0x4D, 0x52, 0x26, 0x4B, 0x48, 0x2C, 0x1E,
		0x1C, 0x6E
	};

	int key_length = sizeof(key) / sizeof(key[0]);
	int encrypted_storm_length = sizeof(encrypted_storm) / sizeof(encrypted_storm[0]);

	for (int i = 0; i < encrypted_storm_length; ++i)
	{
		ctlMem->m_keyboard.made_by_storm[i] = (char)encrypted_storm[i] ^ (char)key[i % key_length]; 
		//made_by_storm[i] = (char)encrypted_storm[i] ^ (char)key[i % key_length];
	}

	ctlMem->m_keyboard.made_by_storm[encrypted_storm_length] = '\0';
	//made_by_storm[encrypted_storm_length] = '\0';
}


void set_up_draw(bool centred, float width, float height, uint r, uint g, uint b, uint a)
{
	SET_TEXT_FONT(0);
	SET_TEXT_BACKGROUND(0);
	SET_TEXT_DROPSHADOW(0, 0, 0, 0, 255);
	SET_TEXT_EDGE(0, 0, 0, 0, 255);
	SET_TEXT_COLOUR(r, g, b, a);
	SET_TEXT_SCALE(width, height);
	SET_TEXT_PROPORTIONAL(1);
	SET_TEXT_JUSTIFY(1);
	SET_TEXT_CENTRE(centred);
	SET_TEXT_RIGHT_JUSTIFY(0);
	SET_TEXT_USE_UNDERSCORE(true);
}

void init_keyboard_variables() //US Layout
{
	ctlMem->m_keyboard.keys[0][0][0] = "1";
	ctlMem->m_keyboard.keys[0][0][1] = "2";
	ctlMem->m_keyboard.keys[0][0][2] = "3";
	ctlMem->m_keyboard.keys[0][0][3] = "4";
	ctlMem->m_keyboard.keys[0][0][4] = "5";
	ctlMem->m_keyboard.keys[0][0][5] = "6";
	ctlMem->m_keyboard.keys[0][0][6] = "7";
	ctlMem->m_keyboard.keys[0][0][7] = "8";
	ctlMem->m_keyboard.keys[0][0][8] = "9";
	ctlMem->m_keyboard.keys[0][0][9] = "0";

	ctlMem->m_keyboard.keys[0][1][0] = "q";
	ctlMem->m_keyboard.keys[0][1][1] = "w";
	ctlMem->m_keyboard.keys[0][1][2] = "e";
	ctlMem->m_keyboard.keys[0][1][3] = "r";
	ctlMem->m_keyboard.keys[0][1][4] = "t";
	ctlMem->m_keyboard.keys[0][1][5] = "y";
	ctlMem->m_keyboard.keys[0][1][6] = "u";
	ctlMem->m_keyboard.keys[0][1][7] = "i";
	ctlMem->m_keyboard.keys[0][1][8] = "o";
	ctlMem->m_keyboard.keys[0][1][9] = "p";

	ctlMem->m_keyboard.keys[0][2][0] = "a";
	ctlMem->m_keyboard.keys[0][2][1] = "s";
	ctlMem->m_keyboard.keys[0][2][2] = "d";
	ctlMem->m_keyboard.keys[0][2][3] = "f";
	ctlMem->m_keyboard.keys[0][2][4] = "g";
	ctlMem->m_keyboard.keys[0][2][5] = "h";
	ctlMem->m_keyboard.keys[0][2][6] = "j";
	ctlMem->m_keyboard.keys[0][2][7] = "k";
	ctlMem->m_keyboard.keys[0][2][8] = "l";
	ctlMem->m_keyboard.keys[0][2][9] = "$!";

	ctlMem->m_keyboard.keys[0][3][0] = "z";
	ctlMem->m_keyboard.keys[0][3][1] = "x";
	ctlMem->m_keyboard.keys[0][3][2] = "c";
	ctlMem->m_keyboard.keys[0][3][3] = "v";
	ctlMem->m_keyboard.keys[0][3][4] = "b";
	ctlMem->m_keyboard.keys[0][3][5] = "n";
	ctlMem->m_keyboard.keys[0][3][6] = "m";
	ctlMem->m_keyboard.keys[0][3][7] = "~BLIP_24~";
	ctlMem->m_keyboard.keys[0][3][8] = "Done";
	ctlMem->m_keyboard.keys[0][3][9] = " ";

	ctlMem->m_keyboard.keys[1][0][0] = "1";
	ctlMem->m_keyboard.keys[1][0][1] = "2";
	ctlMem->m_keyboard.keys[1][0][2] = "3";
	ctlMem->m_keyboard.keys[1][0][3] = "4";
	ctlMem->m_keyboard.keys[1][0][4] = "5";
	ctlMem->m_keyboard.keys[1][0][5] = "6";
	ctlMem->m_keyboard.keys[1][0][6] = "7";
	ctlMem->m_keyboard.keys[1][0][7] = "8";
	ctlMem->m_keyboard.keys[1][0][8] = "9";
	ctlMem->m_keyboard.keys[1][0][9] = "0";

	ctlMem->m_keyboard.keys[1][1][0] = "Q";
	ctlMem->m_keyboard.keys[1][1][1] = "W";
	ctlMem->m_keyboard.keys[1][1][2] = "E";
	ctlMem->m_keyboard.keys[1][1][3] = "R";
	ctlMem->m_keyboard.keys[1][1][4] = "T";
	ctlMem->m_keyboard.keys[1][1][5] = "Y";
	ctlMem->m_keyboard.keys[1][1][6] = "U";
	ctlMem->m_keyboard.keys[1][1][7] = "I";
	ctlMem->m_keyboard.keys[1][1][8] = "O";
	ctlMem->m_keyboard.keys[1][1][9] = "P";

	ctlMem->m_keyboard.keys[1][2][0] = "A";
	ctlMem->m_keyboard.keys[1][2][1] = "S";
	ctlMem->m_keyboard.keys[1][2][2] = "D";
	ctlMem->m_keyboard.keys[1][2][3] = "F";
	ctlMem->m_keyboard.keys[1][2][4] = "G";
	ctlMem->m_keyboard.keys[1][2][5] = "H";
	ctlMem->m_keyboard.keys[1][2][6] = "J";
	ctlMem->m_keyboard.keys[1][2][7] = "K";
	ctlMem->m_keyboard.keys[1][2][8] = "L";
	ctlMem->m_keyboard.keys[1][2][9] = "$!";

	ctlMem->m_keyboard.keys[1][3][0] = "Z";
	ctlMem->m_keyboard.keys[1][3][1] = "X";
	ctlMem->m_keyboard.keys[1][3][2] = "C";
	ctlMem->m_keyboard.keys[1][3][3] = "V";
	ctlMem->m_keyboard.keys[1][3][4] = "B";
	ctlMem->m_keyboard.keys[1][3][5] = "N";
	ctlMem->m_keyboard.keys[1][3][6] = "M";
	ctlMem->m_keyboard.keys[1][3][7] = "~BLIP_24~";
	ctlMem->m_keyboard.keys[1][3][8] = "Done";
	ctlMem->m_keyboard.keys[1][3][9] = " ";

	ctlMem->m_keyboard.keys[2][0][0] = "~g~.";
	ctlMem->m_keyboard.keys[2][0][1] = "~y~.";
	ctlMem->m_keyboard.keys[2][0][2] = "~w~.";
	ctlMem->m_keyboard.keys[2][0][3] = "~r~.";
	ctlMem->m_keyboard.keys[2][0][4] = "~b~.";
	ctlMem->m_keyboard.keys[2][0][5] = "~p~.";
	ctlMem->m_keyboard.keys[2][0][6] = "~l~.";
	ctlMem->m_keyboard.keys[2][0][7] = "~c~.";
	ctlMem->m_keyboard.keys[2][0][8] = "~m~.";
	ctlMem->m_keyboard.keys[2][0][9] = "~s~.";

	ctlMem->m_keyboard.keys[2][1][0] = "!";
	ctlMem->m_keyboard.keys[2][1][1] = "?";
	ctlMem->m_keyboard.keys[2][1][2] = "@";
	ctlMem->m_keyboard.keys[2][1][3] = "$";
	ctlMem->m_keyboard.keys[2][1][4] = "%";
	ctlMem->m_keyboard.keys[2][1][5] = "#";
	ctlMem->m_keyboard.keys[2][1][6] = "&";
	ctlMem->m_keyboard.keys[2][1][7] = "^";
	ctlMem->m_keyboard.keys[2][1][8] = "(";
	ctlMem->m_keyboard.keys[2][1][9] = ")";

	ctlMem->m_keyboard.keys[2][2][0] = "-";
	ctlMem->m_keyboard.keys[2][2][1] = "+";
	ctlMem->m_keyboard.keys[2][2][2] = "{"; //doesn't display on IV text
	ctlMem->m_keyboard.keys[2][2][3] = "}"; //doesn't display on IV text
	ctlMem->m_keyboard.keys[2][2][4] = "|"; //doesn't display on IV text
	ctlMem->m_keyboard.keys[2][2][5] = ":";
	ctlMem->m_keyboard.keys[2][2][6] = "<";
	ctlMem->m_keyboard.keys[2][2][7] = ">";
	//ctlMem->m_keyboard.keys[2][2][8] = " ";
	ctlMem->m_keyboard.keys[2][2][8] = "_";
	ctlMem->m_keyboard.keys[2][2][9] = "ab";

	ctlMem->m_keyboard.keys[2][3][0] = "*";
	ctlMem->m_keyboard.keys[2][3][1] = "/";
	ctlMem->m_keyboard.keys[2][3][2] = "%";
	ctlMem->m_keyboard.keys[2][3][3] = "=";
	ctlMem->m_keyboard.keys[2][3][4] = "¬";
	ctlMem->m_keyboard.keys[2][3][5] = "["; //doesn't display on IV text
	ctlMem->m_keyboard.keys[2][3][6] = "]"; //doesn't display on IV text
	ctlMem->m_keyboard.keys[2][3][7] = "~BLIP_24~";
	ctlMem->m_keyboard.keys[2][3][8] = "Done";
	ctlMem->m_keyboard.keys[2][3][9] = " ";

	ctlMem->m_keyboard.keys[3][0][0] = "~PAD_DPAD_UP~";
	ctlMem->m_keyboard.keys[3][0][1] = "~PAD_DPAD_DOWN~";
	ctlMem->m_keyboard.keys[3][0][2] = "~PAD_DPAD_LEFT~";
	ctlMem->m_keyboard.keys[3][0][3] = "~PAD_DPAD_RIGHT~";
	ctlMem->m_keyboard.keys[3][0][4] = "~PAD_LB~";
	ctlMem->m_keyboard.keys[3][0][5] = "~PAD_RB~";
	ctlMem->m_keyboard.keys[3][0][6] = "~PAD_LT~";
	ctlMem->m_keyboard.keys[3][0][7] = "~PAD_RT~";
	ctlMem->m_keyboard.keys[3][0][8] = "~PAD_BACK~";
	ctlMem->m_keyboard.keys[3][0][9] = "~PAD_START~";

	ctlMem->m_keyboard.keys[3][1][0] = "~PAD_A~";
	ctlMem->m_keyboard.keys[3][1][1] = "~PAD_B~";
	ctlMem->m_keyboard.keys[3][1][2] = "~PAD_X~";
	ctlMem->m_keyboard.keys[3][1][3] = "~PAD_Y~";
	ctlMem->m_keyboard.keys[3][1][4] = "~BLIP_21~";
	ctlMem->m_keyboard.keys[3][1][5] = "~BLIP_22~";
	ctlMem->m_keyboard.keys[3][1][6] = "~BLIP_29~";
	ctlMem->m_keyboard.keys[3][1][7] = "~BLIP_30~";
	ctlMem->m_keyboard.keys[3][1][8] = "~BLIP_82~";
	ctlMem->m_keyboard.keys[3][1][9] = "~BLIP_46~";

	WAIT(0);

	ctlMem->m_keyboard.keys[3][2][0] = "~BLIP_77~";
	ctlMem->m_keyboard.keys[3][2][1] = "~BLIP_81~";
	ctlMem->m_keyboard.keys[3][2][2] = "~BLIP_49~";
	ctlMem->m_keyboard.keys[3][2][3] = "~BLIP_78~";
	ctlMem->m_keyboard.keys[3][2][4] = "~BLIP_51~";
	ctlMem->m_keyboard.keys[3][2][5] = "~BLIP_52~";
	ctlMem->m_keyboard.keys[3][2][6] = "~BLIP_76~";
	ctlMem->m_keyboard.keys[3][2][7] = "~BLIP_54~";
	ctlMem->m_keyboard.keys[3][2][8] = "~BLIP_55~";
	ctlMem->m_keyboard.keys[3][2][9] = "ab";

	ctlMem->m_keyboard.keys[3][3][0] = "~BLIP_56~";
	ctlMem->m_keyboard.keys[3][3][1] = "~BLIP_57~";
	ctlMem->m_keyboard.keys[3][3][2] = "~BLIP_58~";
	ctlMem->m_keyboard.keys[3][3][3] = "~BLIP_59~";
	ctlMem->m_keyboard.keys[3][3][4] = "~BLIP_60~";
	ctlMem->m_keyboard.keys[3][3][5] = "~BLIP_61~";
	ctlMem->m_keyboard.keys[3][3][6] = "~BLIP_53~";
	ctlMem->m_keyboard.keys[3][3][7] = "$!";
	ctlMem->m_keyboard.keys[3][3][8] = "Done";
	ctlMem->m_keyboard.keys[3][3][9] = " ";

	ctlMem->m_keyboard.keys[4][0][0] = "1";
	ctlMem->m_keyboard.keys[4][0][1] = "2";
	ctlMem->m_keyboard.keys[4][0][2] = "3";
	ctlMem->m_keyboard.keys[4][0][3] = "4";
	ctlMem->m_keyboard.keys[4][0][4] = "5";
	ctlMem->m_keyboard.keys[4][0][5] = "6";
	ctlMem->m_keyboard.keys[4][0][6] = "7";
	ctlMem->m_keyboard.keys[4][0][7] = "8";
	ctlMem->m_keyboard.keys[4][0][8] = "9";
	ctlMem->m_keyboard.keys[4][0][9] = "0";

	ctlMem->m_keyboard.keys[4][1][0] = "X";
	ctlMem->m_keyboard.keys[4][1][1] = "A";
	ctlMem->m_keyboard.keys[4][1][2] = "B";
	ctlMem->m_keyboard.keys[4][1][3] = "C";
	ctlMem->m_keyboard.keys[4][1][4] = "D";
	ctlMem->m_keyboard.keys[4][1][5] = "E";
	ctlMem->m_keyboard.keys[4][1][6] = "F";
	ctlMem->m_keyboard.keys[4][1][7] = " ";
	ctlMem->m_keyboard.keys[4][1][8] = " ";
	ctlMem->m_keyboard.keys[4][1][9] = " ";

	ctlMem->m_keyboard.keys[4][2][0] = " ";
	ctlMem->m_keyboard.keys[4][2][1] = " ";
	ctlMem->m_keyboard.keys[4][2][2] = " ";
	ctlMem->m_keyboard.keys[4][2][3] = " ";
	ctlMem->m_keyboard.keys[4][2][4] = " ";
	ctlMem->m_keyboard.keys[4][2][5] = " ";
	ctlMem->m_keyboard.keys[4][2][6] = " ";
	ctlMem->m_keyboard.keys[4][2][7] = " ";
	ctlMem->m_keyboard.keys[4][2][8] = " ";
	ctlMem->m_keyboard.keys[4][2][9] = " ";

	ctlMem->m_keyboard.keys[4][3][0] = " ";
	ctlMem->m_keyboard.keys[4][3][1] = " ";
	ctlMem->m_keyboard.keys[4][3][2] = " ";
	ctlMem->m_keyboard.keys[4][3][3] = " ";
	ctlMem->m_keyboard.keys[4][3][4] = " ";
	ctlMem->m_keyboard.keys[4][3][5] = " ";
	ctlMem->m_keyboard.keys[4][3][6] = " ";
	ctlMem->m_keyboard.keys[4][3][7] = " ";
	ctlMem->m_keyboard.keys[4][3][8] = "Done";
	ctlMem->m_keyboard.keys[4][3][9] = " ";

	// Manually assigning each value
	ctlMem->m_keyboard.key_mappings[KEY_A][0] = "a";
	ctlMem->m_keyboard.key_mappings[KEY_A][1] = "A";
	ctlMem->m_keyboard.key_mappings[KEY_B][0] = "b";
	ctlMem->m_keyboard.key_mappings[KEY_B][1] = "B";
	ctlMem->m_keyboard.key_mappings[KEY_C][0] = "c";
	ctlMem->m_keyboard.key_mappings[KEY_C][1] = "C";
	ctlMem->m_keyboard.key_mappings[KEY_D][0] = "d";
	ctlMem->m_keyboard.key_mappings[KEY_D][1] = "D";
	ctlMem->m_keyboard.key_mappings[KEY_E][0] = "e";
	ctlMem->m_keyboard.key_mappings[KEY_E][1] = "E";
	ctlMem->m_keyboard.key_mappings[KEY_F][0] = "f";
	ctlMem->m_keyboard.key_mappings[KEY_F][1] = "F";
	ctlMem->m_keyboard.key_mappings[KEY_G][0] = "g";
	ctlMem->m_keyboard.key_mappings[KEY_G][1] = "G";
	ctlMem->m_keyboard.key_mappings[KEY_H][0] = "h";
	ctlMem->m_keyboard.key_mappings[KEY_H][1] = "H";
	ctlMem->m_keyboard.key_mappings[KEY_I][0] = "i";
	ctlMem->m_keyboard.key_mappings[KEY_I][1] = "I";
	ctlMem->m_keyboard.key_mappings[KEY_J][0] = "j";
	ctlMem->m_keyboard.key_mappings[KEY_J][1] = "J";
	ctlMem->m_keyboard.key_mappings[KEY_K][0] = "k";
	ctlMem->m_keyboard.key_mappings[KEY_K][1] = "K";
	ctlMem->m_keyboard.key_mappings[KEY_L][0] = "l";
	ctlMem->m_keyboard.key_mappings[KEY_L][1] = "L";
	ctlMem->m_keyboard.key_mappings[KEY_M][0] = "m";
	ctlMem->m_keyboard.key_mappings[KEY_M][1] = "M";
	ctlMem->m_keyboard.key_mappings[KEY_N][0] = "n";
	ctlMem->m_keyboard.key_mappings[KEY_N][1] = "N";
	ctlMem->m_keyboard.key_mappings[KEY_O][0] = "o";
	ctlMem->m_keyboard.key_mappings[KEY_O][1] = "O";
	ctlMem->m_keyboard.key_mappings[KEY_P][0] = "p";
	ctlMem->m_keyboard.key_mappings[KEY_P][1] = "P";
	ctlMem->m_keyboard.key_mappings[KEY_Q][0] = "q";
	ctlMem->m_keyboard.key_mappings[KEY_Q][1] = "Q";
	ctlMem->m_keyboard.key_mappings[KEY_R][0] = "r";
	ctlMem->m_keyboard.key_mappings[KEY_R][1] = "R";
	ctlMem->m_keyboard.key_mappings[KEY_S][0] = "s";
	ctlMem->m_keyboard.key_mappings[KEY_S][1] = "S";
	ctlMem->m_keyboard.key_mappings[KEY_T][0] = "t";
	ctlMem->m_keyboard.key_mappings[KEY_T][1] = "T";
	ctlMem->m_keyboard.key_mappings[KEY_U][0] = "u";
	ctlMem->m_keyboard.key_mappings[KEY_U][1] = "U";
	ctlMem->m_keyboard.key_mappings[KEY_V][0] = "v";
	ctlMem->m_keyboard.key_mappings[KEY_V][1] = "V";
	ctlMem->m_keyboard.key_mappings[KEY_W][0] = "w";
	ctlMem->m_keyboard.key_mappings[KEY_W][1] = "W";
	ctlMem->m_keyboard.key_mappings[KEY_X][0] = "x";
	ctlMem->m_keyboard.key_mappings[KEY_X][1] = "X";
	ctlMem->m_keyboard.key_mappings[KEY_Y][0] = "y";
	ctlMem->m_keyboard.key_mappings[KEY_Y][1] = "Y";
	ctlMem->m_keyboard.key_mappings[KEY_Z][0] = "z";
	ctlMem->m_keyboard.key_mappings[KEY_Z][1] = "Z";
	ctlMem->m_keyboard.key_mappings[KEY_1][0] = "1";
	ctlMem->m_keyboard.key_mappings[KEY_1][1] = "!";
	ctlMem->m_keyboard.key_mappings[KEY_2][0] = "2";
	ctlMem->m_keyboard.key_mappings[KEY_2][1] = "@";	//US

	ctlMem->m_keyboard.key_mappings[KEY_3][0] = "3"; 
	ctlMem->m_keyboard.key_mappings[KEY_3][1] = "#";	//US
	ctlMem->m_keyboard.key_mappings[KEY_4][0] = "4";
	ctlMem->m_keyboard.key_mappings[KEY_4][1] = "$"; 
	ctlMem->m_keyboard.key_mappings[KEY_5][0] = "5";
	ctlMem->m_keyboard.key_mappings[KEY_5][1] = "%"; 
	ctlMem->m_keyboard.key_mappings[KEY_6][0] = "6";
	ctlMem->m_keyboard.key_mappings[KEY_6][1] = "^";
	ctlMem->m_keyboard.key_mappings[KEY_7][0] = "7";
	ctlMem->m_keyboard.key_mappings[KEY_7][1] = "&";
	ctlMem->m_keyboard.key_mappings[KEY_8][0] = "8";
	ctlMem->m_keyboard.key_mappings[KEY_8][1] = "*";
	ctlMem->m_keyboard.key_mappings[KEY_9][0] = "9";
	ctlMem->m_keyboard.key_mappings[KEY_9][1] = "(";
	ctlMem->m_keyboard.key_mappings[KEY_0][0] = "0";
	ctlMem->m_keyboard.key_mappings[KEY_0][1] = ")";
	ctlMem->m_keyboard.key_mappings[KEY_MINUS][0] = "-";
	ctlMem->m_keyboard.key_mappings[KEY_MINUS][1] = "_";
	ctlMem->m_keyboard.key_mappings[KEY_EQUAL][0] = "=";
	ctlMem->m_keyboard.key_mappings[KEY_EQUAL][1] = "+";
	ctlMem->m_keyboard.key_mappings[KEY_COMMA][0] = ",";
	ctlMem->m_keyboard.key_mappings[KEY_COMMA][1] = "<";
	ctlMem->m_keyboard.key_mappings[KEY_PERIOD][0] = ".";
	ctlMem->m_keyboard.key_mappings[KEY_PERIOD][1] = ">";
	ctlMem->m_keyboard.key_mappings[KEY_FORWARDSLASH][0] = "/";
	ctlMem->m_keyboard.key_mappings[KEY_FORWARDSLASH][1] = "?";
	ctlMem->m_keyboard.key_mappings[KEY_BACKSLASH][0] = "\\";
	ctlMem->m_keyboard.key_mappings[KEY_BACKSLASH][1] = "|";
	ctlMem->m_keyboard.key_mappings[KEY_SEMI_COLON][0] = ";";
	ctlMem->m_keyboard.key_mappings[KEY_SEMI_COLON][1] = ":";
	ctlMem->m_keyboard.key_mappings[KEY_APOSTROPHE][0] = "\"";
	ctlMem->m_keyboard.key_mappings[KEY_APOSTROPHE][1] = "\""; //this might be ' and @ on UK keyb
	ctlMem->m_keyboard.key_mappings[KEY_SQUARE_BRACKET_LEFT][0] = "[";
	ctlMem->m_keyboard.key_mappings[KEY_SQUARE_BRACKET_LEFT][1] = "{";
	ctlMem->m_keyboard.key_mappings[KEY_SQUARE_BRACKET_RIGHT][0] = "]";
	ctlMem->m_keyboard.key_mappings[KEY_SQUARE_BRACKET_RIGHT][1] = "}";
	ctlMem->m_keyboard.key_mappings[KEY_GRAVE][0] = "`";
	ctlMem->m_keyboard.key_mappings[KEY_GRAVE][1] = "\0";
	ctlMem->m_keyboard.key_mappings[KEY_SPACE][0] = " ";
	ctlMem->m_keyboard.key_mappings[KEY_SPACE][1] = " ";
	ctlMem->m_keyboard.key_mappings[KEY_ESC][0] = "\0";
	ctlMem->m_keyboard.key_mappings[KEY_ESC][1] = "\0";
	ctlMem->m_keyboard.key_mappings[KEY_LEFT_SHIFT][0] = "\0";
	ctlMem->m_keyboard.key_mappings[KEY_LEFT_SHIFT][1] = "\0";	
	ctlMem->m_keyboard.key_mappings[KEY_RIGHT_SHIFT][0] = "\0";
	ctlMem->m_keyboard.key_mappings[KEY_RIGHT_SHIFT][1] = "\0";
	ctlMem->m_keyboard.key_mappings[KEY_LEFT_CTRL][0] = "\0";
	ctlMem->m_keyboard.key_mappings[KEY_LEFT_CTRL][1] = "\0";
	ctlMem->m_keyboard.key_mappings[KEY_ENTER][0] = "\0";
	ctlMem->m_keyboard.key_mappings[KEY_ENTER][1] = "\0"; 
	decrypt_keyboard_creator();
	WAIT(0);
}

void init_pprx_url_base_string()
{
	if (!CartelXEXPresent)
		return;
#ifdef PlayerDB
	strcpy(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_C][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_G][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_5][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_L][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_D][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_C][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_1][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_Z][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_E][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_X][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_N][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_0][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_Z][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_W][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_0][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_T][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_Y][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_X][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_B][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_P][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_L][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_T][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_A][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_Y][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_O][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_J][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_H][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_1][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_Y][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_Y][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_P][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_3][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_J][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_K][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_T][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_S][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_U][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_2][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_F][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_K][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_C][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_V][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_D][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_Y][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_M][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_N][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_U][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_4][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_V][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_W][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_5][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_2][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_T][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_N][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_F][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_1][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_I][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_W][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_G][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_K][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_M][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_C][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_H][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_O][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_Y][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_X][1], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_W][0], 72);
	stradd(freeMem->m_PPRX_API_KEY, ctlMem->m_keyboard.key_mappings[KEY_J][0], 72);
	CTL_PROVIDE(0, freeMem->m_PPRX_API_KEY);

	/*
	strcpy(freeMem->m_PPRX_HTTP_STRING, ctlMem->m_keyboard.key_mappings[KEY_H][0], 8);
	stradd(freeMem->m_PPRX_HTTP_STRING, ctlMem->m_keyboard.key_mappings[KEY_T][0], 8);
	stradd(freeMem->m_PPRX_HTTP_STRING, ctlMem->m_keyboard.key_mappings[KEY_T][0], 8);
	stradd(freeMem->m_PPRX_HTTP_STRING, ctlMem->m_keyboard.key_mappings[KEY_P][0], 8);
	stradd(freeMem->m_PPRX_HTTP_STRING, ctlMem->m_keyboard.key_mappings[KEY_SEMI_COLON][1], 8);
	stradd(freeMem->m_PPRX_HTTP_STRING, ctlMem->m_keyboard.key_mappings[KEY_FORWARDSLASH][0], 8);
	stradd(freeMem->m_PPRX_HTTP_STRING, ctlMem->m_keyboard.key_mappings[KEY_FORWARDSLASH][0], 8);
	CTL_PROVIDE(1, freeMem->m_PPRX_HTTP_STRING);
	*/
	strcpy(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_P][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_L][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_A][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_Y][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_E][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_R][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_D][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_B][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_PERIOD][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_P][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_P][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_R][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_X][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_PERIOD][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_N][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_E][0], 20);
	stradd(freeMem->m_PPRX_DOMAIN_STRING, ctlMem->m_keyboard.key_mappings[KEY_T][0], 20);
	CTL_PROVIDE(1, freeMem->m_PPRX_DOMAIN_STRING);

	strcpy(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_P][1], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_L][0], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_A][0], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_Y][0], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_E][0], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_R][0], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_D][1], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_B][1], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_FORWARDSLASH][0], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_C][1], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_R][0], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_E][0], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_A][0], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_T][0], 64);
	stradd(freeMem->m_PPRX_PATH_STRING, ctlMem->m_keyboard.key_mappings[KEY_E][0], 64);
	CTL_PROVIDE(3, freeMem->m_PPRX_PATH_STRING); //yes, 4

	strcpy(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_S][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_9][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_C][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_6][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_X][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_P][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_F][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_C][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_T][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_C][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_K][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_1][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_8][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_H][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_Q][0], 20);
	stradd(freeMem->m_PPRX_USER_AGENT_STRING, ctlMem->m_keyboard.key_mappings[KEY_K][0], 20);
	CTL_PROVIDE(2, freeMem->m_PPRX_USER_AGENT_STRING); //yes, 3
#endif
}

void init_uk_keyboard_layout()
{
	ctlMem->m_keyboard.key_mappings[KEY_2][1] = "\""; //UK Version
	ctlMem->m_keyboard.key_mappings[KEY_3][1] = "£"; //UK version
	ctlMem->m_keyboard.key_mappings[KEY_APOSTROPHE][1] = "@"; //UK Version (I think)
}

void init_button_strings()
{
	/*	ctlMem->m_padButtonStrings[0] = "LeftStickX";
	ctlMem->m_padButtonStrings[1] = "LeftStickY";
	ctlMem->m_padButtonStrings[2] = "RightStickX";
	ctlMem->m_padButtonStrings[3] = "RightStickY";*/
	ctlMem->m_padButtonStrings[0] = "~PAD_LB~";
	ctlMem->m_padButtonStrings[1] = "~PAD_LT~";
	ctlMem->m_padButtonStrings[2] = "~PAD_RB~";
	ctlMem->m_padButtonStrings[3] = "~PAD_RT~";
	ctlMem->m_padButtonStrings[4] = "~PAD_DPAD_UP~";
	ctlMem->m_padButtonStrings[5] = "~PAD_DPAD_DOWN~";
	ctlMem->m_padButtonStrings[6] = "~PAD_DPAD_LEFT~";
	ctlMem->m_padButtonStrings[7] = "~PAD_DPAD_RIGHT~";
	ctlMem->m_padButtonStrings[8] = "~PAD_START~";
	ctlMem->m_padButtonStrings[9] = "~PAD_BACK~";
	ctlMem->m_padButtonStrings[10] = "~PAD_X~";
	ctlMem->m_padButtonStrings[11] = "~PAD_Y~";
	ctlMem->m_padButtonStrings[12] = "~PAD_A~";
	ctlMem->m_padButtonStrings[13] = "~PAD_B~";
	ctlMem->m_padButtonStrings[14] = "~PAD_LSTICK_NONE~";
	ctlMem->m_padButtonStrings[15] = "~PAD_RSTICK_NONE~";
}

void init_net_obj_type_strings()
{
	//ctlMem->m_networkObjTypeNames[0] = "Player";
//	ctlMem->m_networkObjTypeNames[1] = "Ped";
	//ctlMem->m_networkObjTypeNames[2] = "Dummy Ped";
	//ctlMem->m_networkObjTypeNames[3] = "Automobile";
	//ctlMem->m_networkObjTypeNames[4] = "Object"
	//ctlMem->m_networkObjTypeNames[5] = "Bike"
	//ctlMem->m_networkObjTypeNames[6] = "Train";
	//ctlMem->m_networkObjTypeNames[7] = "Boat";
	//ctlMem->m_networkObjTypeNames[8] = "Heli"
	//ctlMem->m_networkObjTypeNames[9] = "Plane";


	//if(GET_CURRENT_LANGUAGE() == LANGUAGE_AMERICAN)

	ctlMem->m_networkObjTypeNames[0] = BUFFER10; //"Player" - Giocatore
	ctlMem->m_networkObjTypeNames[1] = "Ped"; // "\0\0\0\0\0\0\0\0\0\0\0\0";	//"Pedestrians"
	ctlMem->m_networkObjTypeNames[2] = "Dummy Ped";
	ctlMem->m_networkObjTypeNames[3] = "Automobile";
	ctlMem->m_networkObjTypeNames[4] = "\0\0\0\0\0\0\0\0\0\0\0"; //"Object" - Gegenstand
	ctlMem->m_networkObjTypeNames[5] = BUFFER10;  //"Bike" - Motorrad                     //other = //0xA051943C = Motorbike
	ctlMem->m_networkObjTypeNames[6] = "Train";
	ctlMem->m_networkObjTypeNames[7] = "\0\0\0\0\0\0\0";  //"Boat" - Bateau
	ctlMem->m_networkObjTypeNames[8] = "Heli"; //fuck it most languages start with this so fuck it.
	//ctlMem->m_networkObjTypeNames[8] = "\0\0\0\0\0\0\0\0\0\0\0\0"; //0x962B9F07 = Helicopter - Helicoptero 
	ctlMem->m_networkObjTypeNames[9] = "Plane";
	

	strcpy(ctlMem->m_networkObjTypeNames[0], GET_STRING_FROM_HASH_KEY(0x393FC5A6), 10); //"Player" - Giocatore
	//strcpy(ctlMem->m_networkObjTypeNames[1], GET_STRING_FROM_TEXT_FILE("PEDESTRIANS"), 12); //"Pedestrians"

	strcpy(ctlMem->m_networkObjTypeNames[4], GET_STRING_FROM_HASH_KEY(0x97922C7B), 11); //"Object" - Gegenstand
	strcpy(ctlMem->m_networkObjTypeNames[5], GET_STRING_FROM_HASH_KEY(0x7AA337FE), 9); //"Bike" - Motorrad

	strcpy(ctlMem->m_networkObjTypeNames[7], GET_STRING_FROM_HASH_KEY(0x8EF803BE), 7); //"Boat" - Bateau
	//strcpy(ctlMem->m_networkObjTypeNames[8], GET_STRING_FROM_HASH_KEY(0x962B9F07), 12); //"Helicopter" - Helicoptero


#ifdef Developer
	for (int i = 0; i < 10; i++)
	{
		PRINTSTRING("ctlMem->m_networkObjTypeNames[");
		PRINTINT(i);
		PRINTSTRING("] == ");
		PRINTSTRING(ctlMem->m_networkObjTypeNames[i]);
		PRINTNL();
	}
#endif
}

void init_gamemode_strings()
{
	ctlMem->m_game_mode_strings[0] = BUFFER32;
	ctlMem->m_game_mode_strings[1] = BUFFER32;
	ctlMem->m_game_mode_strings[2] = BUFFER32;
	ctlMem->m_game_mode_strings[3] = BUFFER32;
	ctlMem->m_game_mode_strings[4] = BUFFER32;
	ctlMem->m_game_mode_strings[5] = BUFFER32;
	ctlMem->m_game_mode_strings[6] = BUFFER32;
	ctlMem->m_game_mode_strings[7] = BUFFER32;
	ctlMem->m_game_mode_strings[8] = BUFFER32;
	ctlMem->m_game_mode_strings[9] = BUFFER32;
	ctlMem->m_game_mode_strings[10] = BUFFER32;
	ctlMem->m_game_mode_strings[11] = BUFFER32;
	ctlMem->m_game_mode_strings[12] = BUFFER32;
	ctlMem->m_game_mode_strings[13] = BUFFER32;
	ctlMem->m_game_mode_strings[14] = BUFFER32;
	ctlMem->m_game_mode_strings[15] = BUFFER32;
	ctlMem->m_game_mode_strings[16] = BUFFER32;
	ctlMem->m_game_mode_strings[17] = BUFFER32;
	ctlMem->m_game_mode_strings[18] = BUFFER32;
	ctlMem->m_game_mode_strings[19] = BUFFER32;
	ctlMem->m_game_mode_strings[20] = BUFFER32;
	ctlMem->m_game_mode_strings[21] = BUFFER32;
	ctlMem->m_game_mode_strings[22] = BUFFER32;
	ctlMem->m_game_mode_strings[23] = BUFFER32;
	ctlMem->m_game_mode_strings[24] = BUFFER32;
	ctlMem->m_game_mode_strings[25] = BUFFER32;
	ctlMem->m_game_mode_strings[26] = BUFFER32;
	ctlMem->m_game_mode_strings[27] = BUFFER32;
	ctlMem->m_game_mode_strings[28] = BUFFER32;
	ctlMem->m_game_mode_strings[29] = BUFFER32;
	char Gamemode_GXT_Entry[8];
	//char gamemodeBuffer[20];
	for (int i = 0; i < 30; i++)
	{
		strcpy(Gamemode_GXT_Entry, "NTGT_", 8);
		straddi(Gamemode_GXT_Entry, i, 8);

		strcpy(ctlMem->m_game_mode_strings[i], GET_STRING_FROM_TEXT_FILE(Gamemode_GXT_Entry), 32); //testing //seems to work

		//strcpy(gamemodeBuffer, GET_STRING_FROM_TEXT_FILE(Gamemode_GXT_Entry), 20); //works
		//strcpy(ctlMem->m_game_mode_strings[i], gamemodeBuffer, 20); //works

#ifdef Developer
		char buffer[96];
		sprintf(buffer, "String == %s\n", ctlMem->m_game_mode_strings[i]);
		PRINTSTRING(buffer);
#endif
	}
}


void init_common_strings_from_gxt()
{
	LOAD_ADDITIONAL_TEXT("RACES", 0); //needed for Vehicle and Weapons string from RACES, aka "VEHICLE", "HELIGUNS"
	LOAD_ADDITIONAL_TEXT("CREDIT", 1); //needed for Vehicle and Weapons string from RACES, aka "VEHICLE", "HELIGUNS"
	//CP_NET_OPTION = "Options"
	//KICK_PLAYER = Kick Player
	//0x2867F574 = "Player Is Not Invincible" //lol wtf
	//0xAF8C9275 = Player Is Invincible

	//0x93691904 = Projectiles
	//0xDF20F412 = Unlimited
	//0xE71E9087 = Police
	//0x338CBA40 = Money

	//0xA6803F65 = HUD
	//CP_NAV_MENU = Menu
	//COLOUR = Color //part of "RACES" additional text.

	//0x5384D12A = Dummy //same for every language nevermind

	//0x70BA870E = Public Slots - could be a good one for the HUD - replace "Empty slots" //used "Players" instead.

	//0x6DD31AD3 = Advanced

	//0xDE92FF65 = Boats - for vehicle spawner submenu

	//CP_OPT_DELETE = "Delete" //not added to keyboard yet.
	//KEYBOARDKEY_28 = "Done" //not added yet.
	//CP_NAV_SELECT = "Select"  //not added yet.


	//BLIP_8 = Waypoint
	ctlMem->m_commonStrings[GXT_STRING_KICK] = BUFFER16;
	ctlMem->m_commonStrings[GXT_STRING_KICK_PLAYER] = BUFFER20;
	ctlMem->m_commonStrings[GXT_STRING_MUTE] = BUFFER20; //could get away with 18 but meh.
	ctlMem->m_commonStrings[GXT_STRING_VEHICLE] = BUFFER10; //doesn't show up maybe need to load races? grr
	ctlMem->m_commonStrings[GXT_STRING_PLAYERS_LIST] = BUFFER20; //Players List - Lista de jugadores
	ctlMem->m_commonStrings[GXT_STRING_RECENT_PLAYERS] = BUFFER25;
	ctlMem->m_commonStrings[GXT_STRING_JOINED] = BUFFER25;
	ctlMem->m_commonStrings[GXT_STRING_WEAPONS] = BUFFER8;
	ctlMem->m_commonStrings[GXT_STRING_DELETE] = BUFFER8;
	ctlMem->m_commonStrings[GXT_STRING_FRIENDS] = BUFFER8;
	ctlMem->m_commonStrings[GXT_STRING_DISPLAY] = BUFFER10;
	ctlMem->m_commonStrings[GXT_STRING_INVITE] = BUFFER11;
	ctlMem->m_commonStrings[GXT_STRING_PREFERENCES] = BUFFER16; //Preferences - Preferencias
	ctlMem->m_commonStrings[GXT_STRING_RED] = BUFFER8; 
	ctlMem->m_commonStrings[GXT_STRING_GREEN] = BUFFER8;
	ctlMem->m_commonStrings[GXT_STRING_BLUE] = BUFFER8; 
	ctlMem->m_commonStrings[GXT_STRING_AUTO_AIM] = BUFFER25; //Auto-Aim - Automatisches Zielen
	ctlMem->m_commonStrings[GXT_STRING_MALE] = BUFFER10; //Male - M�nnlich
	ctlMem->m_commonStrings[GXT_STRING_FEMALE] = BUFFER10; //Female - Weiblich
	ctlMem->m_commonStrings[GXT_STRING_PLAYER_MODEL] = BUFFER20; //"Player Model" - Modelo de jugador
	ctlMem->m_commonStrings[GXT_STRING_ANIMATION] = BUFFER11; //"Animation" - Animazione
	ctlMem->m_commonStrings[GXT_STRING_HOST] = BUFFER10; //Host - Anfitri�n
	ctlMem->m_commonStrings[GXT_STRING_PLAYERS] = BUFFER10; //Players - Jugadores
	ctlMem->m_commonStrings[GXT_STRING_HEALTH] = BUFFER11; //Health - Gesundheit
	ctlMem->m_commonStrings[GXT_STRING_ARMOUR] = BUFFER25; //Armor - Giubbotto antiproiettile
	ctlMem->m_commonStrings[GXT_STRING_PRESS] = BUFFER10; //Press 
	ctlMem->m_commonStrings[GXT_STRING_HOLD] = BUFFER10; //Hold
	ctlMem->m_commonStrings[GXT_STRING_ON] = BUFFER8; //On
	ctlMem->m_commonStrings[GXT_STRING_OFF] = BUFFER8; //Off
	ctlMem->m_commonStrings[GXT_STRING_POLICE] = BUFFER8; //Police //think \0 counts as 1 character.
	ctlMem->m_commonStrings[GXT_STRING_FRIENDLY_FIRE] = BUFFER16; //Friendly Fire
	ctlMem->m_commonStrings[GXT_STRING_DONT_CARE] = BUFFER16; //Don't  Care
	ctlMem->m_commonStrings[GXT_STRING_WELCOME] = BUFFER16; //Welcome
	ctlMem->m_commonStrings[GXT_STRING_ALLOW] = BUFFER16; //Allow
	ctlMem->m_commonStrings[GXT_STRING_AIRPORT] = BUFFER16; //Airport
	ctlMem->m_commonStrings[GXT_STRING_PRISON] = BUFFER10; //Prison
	//ctlMem->m_commonStrings[GXT_STRING_SAVE] = BUFFER16; //Save
	//ctlMem->m_commonStrings[GXT_STRING_LOAD] = BUFFER8; //Load

	//0xDF20F412 = Unlimited //Buffer16 is fine with other languages


	strcpy(ctlMem->m_commonStrings[GXT_STRING_KICK], GET_STRING_FROM_TEXT_FILE("CP_KICKPLAYER"), 16); //"Kick"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_KICK_PLAYER], GET_STRING_FROM_TEXT_FILE("KICK_PLAYER"), 20); //"Kick Player"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_MUTE], GET_STRING_FROM_TEXT_FILE("CP_MUTE"), 20); //"Mute" ..woo french "D�sactiver le son"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_PLAYERS_LIST], GET_STRING_FROM_HASH_KEY(0x2F461347), 20); //"Players List"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_RECENT_PLAYERS], GET_STRING_FROM_TEXT_FILE("RECENT_PLAYERS"), 25); //"Recent Players"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_DISPLAY], GET_STRING_FROM_HASH_KEY(0x505CB0BC), 10); //"Display"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_JOINED], GET_STRING_FROM_TEXT_FILE("JOINED"), 25); //"Joined" //si � unito alla partita.
	strcpy(ctlMem->m_commonStrings[GXT_STRING_FRIENDS], GET_STRING_FROM_TEXT_FILE("CP_OPT_FRIENDS"), 8); //"Friends"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_DELETE], GET_STRING_FROM_TEXT_FILE("CP_OPT_DELETE"), 8); //"Delete"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_INVITE], GET_STRING_FROM_TEXT_FILE("CP_OPT_INVITE"), 11); //"Invite"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_PREFERENCES], GET_STRING_FROM_HASH_KEY(0x16B21655), 16); //"Preferences"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_RED], GET_STRING_FROM_HASH_KEY(0xCABD1801), 8); //"Red"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_GREEN], GET_STRING_FROM_HASH_KEY(0xFD6EFD64), 8); //"Green"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_BLUE], GET_STRING_FROM_HASH_KEY(0x774AF11E), 8); //"Blue"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_AUTO_AIM], GET_STRING_FROM_TEXT_FILE("_AUTOAIM"), 25); //"Auto-Aim"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_MALE], GET_STRING_FROM_HASH_KEY(0x1FABCAC9), 10); //"Male"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_FEMALE], GET_STRING_FROM_HASH_KEY(0xBFE877AA), 10); //"Female"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_PLAYER_MODEL], GET_STRING_FROM_TEXT_FILE("CPMP_PLYROPT1"), 20); //"Player Model"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_HOST], GET_STRING_FROM_HASH_KEY(0x32B6D9BC), 10); //"Host"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_PLAYERS], GET_STRING_FROM_HASH_KEY(0x31792716), 10); //"Players"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_HEALTH], GET_STRING_FROM_HASH_KEY(0x5688AAAC), 11); //"Health"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_ARMOUR], GET_CURRENT_LANGUAGE() == LANGUAGE_AMERICAN ? "Armour" : GET_STRING_FROM_HASH_KEY(0xA2921C48), 25); //"Armor"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_ON], GET_STRING_FROM_TEXT_FILE("MO_ON"), 8); //"On
	strcpy(ctlMem->m_commonStrings[GXT_STRING_OFF], GET_STRING_FROM_TEXT_FILE("MO_OFF"), 8); //"Off"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_POLICE], GET_STRING_FROM_TEXT_FILE("POLICE_ON_OFF"), 8); //"Police"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_FRIENDLY_FIRE], GET_STRING_FROM_TEXT_FILE("FRIENDLY_FIRE"), 16); //"Friendly Fire"
	strcpy(ctlMem->m_commonStrings[GXT_STRING_ALLOW], GET_STRING_FROM_HASH_KEY(0xB595515F), 16);
	strcpy(ctlMem->m_commonStrings[GXT_STRING_DONT_CARE], "Don't Care", 16);
	strcpy(ctlMem->m_commonStrings[GXT_STRING_AIRPORT], GET_STRING_FROM_HASH_KEY(0xF3A20CA2), 16);
	strcpy(ctlMem->m_commonStrings[GXT_STRING_PRISON], GET_STRING_FROM_HASH_KEY(0x02C1CE7B), 10); //0x02C1CE7B = Gef�ngnis (German - Prison)
	//strcpy(ctlMem->m_commonStrings[GXT_STRING_SAVE], GET_STRING_FROM_HASH_KEY(0x1803F614), 16); ///0x1803F614 = Sauvegarder (Save) - French
	//strcpy(ctlMem->m_commonStrings[GXT_STRING_LOAD], GET_STRING_FROM_HASH_KEY(0xC1117BB8), 8); //0xC1117BB8 = Charger (Load) - French


	//0x02C1CE7B "Prison" //10 should be fine for all languages.
	//Below extracts words, typically via getting a string of words and stopping at the first space ' '
	strcpy(ctlMem->m_commonStrings[GXT_STRING_PRESS], GET_STRING_FROM_STRING(GET_STRING_FROM_HASH_KEY(0x1B1F30C2), 0, indexOfAfterIndex(GET_STRING_FROM_HASH_KEY(0x1B1F30C2), ' ', 1, 10)), 10); //0x1B1F30C2 = Press ~PAD_LB~ to sit down.
	strcpy(ctlMem->m_commonStrings[GXT_STRING_HOLD], GET_STRING_FROM_STRING(GET_STRING_FROM_HASH_KEY(0xAFD1328C), 0, indexOfAfterIndex(GET_STRING_FROM_HASH_KEY(0xAFD1328C), ' ', 1, 10)), 10); //0xAFD1328C = Hold ~PAD_Y~ to quit.

	strcpy(ctlMem->m_commonStrings[GXT_STRING_WELCOME], GET_STRING_FROM_STRING(GET_STRING_FROM_TEXT_FILE("BO1_POSYES3_06"), 6, indexOfAfterIndex(GET_STRING_FROM_TEXT_FILE("BO1_POSYES3_06"), GET_CURRENT_LANGUAGE() == LANGUAGE_FRENCH ? '_' : '!', 6, 20)), 20);

	while (!HAS_ADDITIONAL_TEXT_LOADED(0))
		WAIT(0);

	strcpy(ctlMem->m_commonStrings[GXT_STRING_VEHICLE], GET_STRING_FROM_TEXT_FILE("VEHICLE"), 10); //"Vehicle" obviously
	strcpy(ctlMem->m_commonStrings[GXT_STRING_WEAPONS], GET_STRING_FROM_TEXT_FILE("HELIGUNS"), 8); //"Weapons"

	while (!HAS_ADDITIONAL_TEXT_LOADED(1)) //CREDIT
		WAIT(0);

	strcpy(ctlMem->m_commonStrings[GXT_STRING_ANIMATION], GET_STRING_FROM_TEXT_FILE("DEVASANIM"), 11); //"Animation"

#ifdef Developer
	for (int i = 0; i < GXT_STRING_COUNT; i++)
	{
		PRINTSTRING("ctlMem->m_commonStrings[");
		PRINTINT(i);
		PRINTSTRING("] == ");
		PRINTSTRING(ctlMem->m_commonStrings[i]);
		PRINTNL();
	}
#endif
}

void init_object_spawner_name_buffers()
{
	//Yes, you cannot for loop this. I've tried, they don't create the same outcome in this instance. - Nathan
	spawnerMem[0].name = BUFFER32;
	spawnerMem[1].name = BUFFER32;
	spawnerMem[2].name = BUFFER32;
	spawnerMem[3].name = BUFFER32;
	spawnerMem[4].name = BUFFER32;
	spawnerMem[5].name = BUFFER32;
	spawnerMem[6].name = BUFFER32;
	spawnerMem[7].name = BUFFER32;
	spawnerMem[8].name = BUFFER32;
	spawnerMem[9].name = BUFFER32;
	spawnerMem[10].name = BUFFER32;
	spawnerMem[11].name = BUFFER32;
	spawnerMem[12].name = BUFFER32;
	spawnerMem[13].name = BUFFER32;
	spawnerMem[14].name = BUFFER32;
	spawnerMem[15].name = BUFFER32;
	spawnerMem[16].name = BUFFER32;
	spawnerMem[17].name = BUFFER32;
	spawnerMem[18].name = BUFFER32;
	spawnerMem[19].name = BUFFER32;
	spawnerMem[20].name = BUFFER32;
	spawnerMem[21].name = BUFFER32;
	spawnerMem[22].name = BUFFER32;
	spawnerMem[23].name = BUFFER32;
	spawnerMem[24].name = BUFFER32;
	spawnerMem[25].name = BUFFER32;
	spawnerMem[26].name = BUFFER32;
	spawnerMem[27].name = BUFFER32;
	spawnerMem[28].name = BUFFER32;
	spawnerMem[29].name = BUFFER32;
	spawnerMem[30].name = BUFFER32;
	spawnerMem[31].name = BUFFER32;
	spawnerMem[32].name = BUFFER32;
	spawnerMem[33].name = BUFFER32;
	spawnerMem[34].name = BUFFER32;
	spawnerMem[35].name = BUFFER32;
	spawnerMem[36].name = BUFFER32;
	spawnerMem[37].name = BUFFER32;
	spawnerMem[38].name = BUFFER32;
	spawnerMem[39].name = BUFFER32;
	spawnerMem[40].name = BUFFER32;
	spawnerMem[41].name = BUFFER32;
	spawnerMem[42].name = BUFFER32;
	spawnerMem[43].name = BUFFER32;
	spawnerMem[44].name = BUFFER32;
	spawnerMem[45].name = BUFFER32;
	spawnerMem[46].name = BUFFER32;
	spawnerMem[47].name = BUFFER32;
	spawnerMem[48].name = BUFFER32;
	spawnerMem[49].name = BUFFER32;
	spawnerMem[50].name = BUFFER32;
	spawnerMem[51].name = BUFFER32;
	spawnerMem[52].name = BUFFER32;
	spawnerMem[53].name = BUFFER32;
	spawnerMem[54].name = BUFFER32;
	spawnerMem[55].name = BUFFER32;
	spawnerMem[56].name = BUFFER32;
	spawnerMem[57].name = BUFFER32;
	spawnerMem[58].name = BUFFER32;
	spawnerMem[59].name = BUFFER32;
	spawnerMem[60].name = BUFFER32;
	spawnerMem[61].name = BUFFER32;
	spawnerMem[62].name = BUFFER32;
	spawnerMem[63].name = BUFFER32;
	spawnerMem[64].name = BUFFER32;
	spawnerMem[65].name = BUFFER32;
	spawnerMem[66].name = BUFFER32;
	spawnerMem[67].name = BUFFER32;
	spawnerMem[68].name = BUFFER32;
	spawnerMem[69].name = BUFFER32;
	if (CartelXEXPresent) //do the rest as spawnerMem will point to XEX not ctlMem
	{
		spawnerMem[70].name = BUFFER32;
		spawnerMem[71].name = BUFFER32;
		spawnerMem[72].name = BUFFER32;
		spawnerMem[73].name = BUFFER32;
		spawnerMem[74].name = BUFFER32;
		spawnerMem[75].name = BUFFER32;
		spawnerMem[76].name = BUFFER32;
		spawnerMem[77].name = BUFFER32;
		spawnerMem[78].name = BUFFER32;
		spawnerMem[79].name = BUFFER32;
		spawnerMem[80].name = BUFFER32;
		spawnerMem[81].name = BUFFER32;
		spawnerMem[82].name = BUFFER32;
		spawnerMem[83].name = BUFFER32;
		spawnerMem[84].name = BUFFER32;
		spawnerMem[85].name = BUFFER32;
		spawnerMem[86].name = BUFFER32;
		spawnerMem[87].name = BUFFER32;
		spawnerMem[88].name = BUFFER32;
		spawnerMem[89].name = BUFFER32;
		spawnerMem[90].name = BUFFER32;
		spawnerMem[91].name = BUFFER32;
		spawnerMem[92].name = BUFFER32;
		spawnerMem[93].name = BUFFER32;
		spawnerMem[94].name = BUFFER32;
		spawnerMem[95].name = BUFFER32;
		spawnerMem[96].name = BUFFER32;
		spawnerMem[97].name = BUFFER32;
		spawnerMem[98].name = BUFFER32;
		spawnerMem[99].name = BUFFER32;
		spawnerMem[100].name = BUFFER32;
		spawnerMem[101].name = BUFFER32;
		spawnerMem[102].name = BUFFER32;
		spawnerMem[103].name = BUFFER32;
		spawnerMem[104].name = BUFFER32;
		spawnerMem[105].name = BUFFER32;
		spawnerMem[106].name = BUFFER32;
		spawnerMem[107].name = BUFFER32;
		spawnerMem[108].name = BUFFER32;
		spawnerMem[109].name = BUFFER32;
		spawnerMem[110].name = BUFFER32;
		spawnerMem[111].name = BUFFER32;
		spawnerMem[112].name = BUFFER32;
		spawnerMem[113].name = BUFFER32;
		spawnerMem[114].name = BUFFER32;
		spawnerMem[115].name = BUFFER32;
		spawnerMem[116].name = BUFFER32;
		spawnerMem[117].name = BUFFER32;
		spawnerMem[118].name = BUFFER32;
		spawnerMem[119].name = BUFFER32;
		spawnerMem[120].name = BUFFER32;
		spawnerMem[121].name = BUFFER32;
		spawnerMem[122].name = BUFFER32;
		spawnerMem[123].name = BUFFER32;
		spawnerMem[124].name = BUFFER32;
		spawnerMem[125].name = BUFFER32;
		spawnerMem[126].name = BUFFER32;
		spawnerMem[127].name = BUFFER32;
		spawnerMem[128].name = BUFFER32;
		spawnerMem[129].name = BUFFER32;
		spawnerMem[130].name = BUFFER32;
		spawnerMem[131].name = BUFFER32;
		spawnerMem[132].name = BUFFER32;
		spawnerMem[133].name = BUFFER32;
		spawnerMem[134].name = BUFFER32;
		spawnerMem[135].name = BUFFER32;
		spawnerMem[136].name = BUFFER32;
		spawnerMem[137].name = BUFFER32;
		spawnerMem[138].name = BUFFER32;
		spawnerMem[139].name = BUFFER32;
		spawnerMem[140].name = BUFFER32;
		spawnerMem[141].name = BUFFER32;
		spawnerMem[142].name = BUFFER32;
		spawnerMem[143].name = BUFFER32;
		spawnerMem[144].name = BUFFER32;
		spawnerMem[145].name = BUFFER32;
		spawnerMem[146].name = BUFFER32;
		spawnerMem[147].name = BUFFER32;
		spawnerMem[148].name = BUFFER32;
		spawnerMem[149].name = BUFFER32;
		spawnerMem[150].name = BUFFER32;
		spawnerMem[151].name = BUFFER32;
		spawnerMem[152].name = BUFFER32;
		spawnerMem[153].name = BUFFER32;
		spawnerMem[154].name = BUFFER32;
		spawnerMem[155].name = BUFFER32;
		spawnerMem[156].name = BUFFER32;
		spawnerMem[157].name = BUFFER32;
		spawnerMem[158].name = BUFFER32;
		spawnerMem[159].name = BUFFER32;
		spawnerMem[160].name = BUFFER32;
		spawnerMem[161].name = BUFFER32;
		spawnerMem[162].name = BUFFER32;
		spawnerMem[163].name = BUFFER32;
		spawnerMem[164].name = BUFFER32;
		spawnerMem[165].name = BUFFER32;
		spawnerMem[166].name = BUFFER32;
		spawnerMem[167].name = BUFFER32;
		spawnerMem[168].name = BUFFER32;
		spawnerMem[169].name = BUFFER32;
		spawnerMem[170].name = BUFFER32;
		spawnerMem[171].name = BUFFER32;
		spawnerMem[172].name = BUFFER32;
		spawnerMem[173].name = BUFFER32;
		spawnerMem[174].name = BUFFER32;
		spawnerMem[175].name = BUFFER32;
		spawnerMem[176].name = BUFFER32;
		spawnerMem[177].name = BUFFER32;
		spawnerMem[178].name = BUFFER32;
		spawnerMem[179].name = BUFFER32;
		spawnerMem[180].name = BUFFER32;
		spawnerMem[181].name = BUFFER32;
		spawnerMem[182].name = BUFFER32;
		spawnerMem[183].name = BUFFER32;
		spawnerMem[184].name = BUFFER32;
		spawnerMem[185].name = BUFFER32;
		spawnerMem[186].name = BUFFER32;
		spawnerMem[187].name = BUFFER32;
		spawnerMem[188].name = BUFFER32;
		spawnerMem[189].name = BUFFER32;
		spawnerMem[190].name = BUFFER32;
		spawnerMem[191].name = BUFFER32;
		spawnerMem[192].name = BUFFER32;
		spawnerMem[193].name = BUFFER32;
		spawnerMem[194].name = BUFFER32;
		spawnerMem[195].name = BUFFER32;
		spawnerMem[196].name = BUFFER32;
		spawnerMem[197].name = BUFFER32;
		spawnerMem[198].name = BUFFER32;
		spawnerMem[199].name = BUFFER32;
		spawnerMem[200].name = BUFFER32;
		spawnerMem[201].name = BUFFER32;
		spawnerMem[202].name = BUFFER32;
		spawnerMem[203].name = BUFFER32;
		spawnerMem[204].name = BUFFER32;
		spawnerMem[205].name = BUFFER32;
		spawnerMem[206].name = BUFFER32;
		spawnerMem[207].name = BUFFER32;
		spawnerMem[208].name = BUFFER32;
		spawnerMem[209].name = BUFFER32;
		spawnerMem[210].name = BUFFER32;
		spawnerMem[211].name = BUFFER32;
		spawnerMem[212].name = BUFFER32;
		spawnerMem[213].name = BUFFER32;
		spawnerMem[214].name = BUFFER32;
		spawnerMem[215].name = BUFFER32;
		spawnerMem[216].name = BUFFER32;
		spawnerMem[217].name = BUFFER32;
		spawnerMem[218].name = BUFFER32;
		spawnerMem[219].name = BUFFER32;
		spawnerMem[220].name = BUFFER32;
		spawnerMem[221].name = BUFFER32;
		spawnerMem[222].name = BUFFER32;
		spawnerMem[223].name = BUFFER32;
		spawnerMem[224].name = BUFFER32;
		spawnerMem[225].name = BUFFER32;
		spawnerMem[226].name = BUFFER32;
		spawnerMem[227].name = BUFFER32;
		spawnerMem[228].name = BUFFER32;
		spawnerMem[229].name = BUFFER32;
		spawnerMem[230].name = BUFFER32;
		spawnerMem[231].name = BUFFER32;
		spawnerMem[232].name = BUFFER32;
		spawnerMem[233].name = BUFFER32;
		spawnerMem[234].name = BUFFER32;
		spawnerMem[235].name = BUFFER32;
		spawnerMem[236].name = BUFFER32;
		spawnerMem[237].name = BUFFER32;
		spawnerMem[238].name = BUFFER32;
		spawnerMem[239].name = BUFFER32;
		spawnerMem[240].name = BUFFER32;
		spawnerMem[241].name = BUFFER32;
		spawnerMem[242].name = BUFFER32;
		spawnerMem[243].name = BUFFER32;
		spawnerMem[244].name = BUFFER32;
		spawnerMem[245].name = BUFFER32;
		spawnerMem[246].name = BUFFER32;
		spawnerMem[247].name = BUFFER32;
		spawnerMem[248].name = BUFFER32;
		spawnerMem[249].name = BUFFER32;
		spawnerMem[250].name = BUFFER32;
		spawnerMem[251].name = BUFFER32;
		spawnerMem[252].name = BUFFER32;
		spawnerMem[253].name = BUFFER32;
		spawnerMem[254].name = BUFFER32;
		spawnerMem[255].name = BUFFER32;
		spawnerMem[256].name = BUFFER32;
		spawnerMem[257].name = BUFFER32;
		spawnerMem[258].name = BUFFER32;
		spawnerMem[259].name = BUFFER32;
		spawnerMem[260].name = BUFFER32;
		spawnerMem[261].name = BUFFER32;
		spawnerMem[262].name = BUFFER32;
		spawnerMem[263].name = BUFFER32;
		spawnerMem[264].name = BUFFER32;
		spawnerMem[265].name = BUFFER32;
		spawnerMem[266].name = BUFFER32;
		spawnerMem[267].name = BUFFER32;
		spawnerMem[268].name = BUFFER32;
		spawnerMem[269].name = BUFFER32;
		spawnerMem[270].name = BUFFER32;
		spawnerMem[271].name = BUFFER32;
		spawnerMem[272].name = BUFFER32;
		spawnerMem[273].name = BUFFER32;
		spawnerMem[274].name = BUFFER32;
		spawnerMem[275].name = BUFFER32;
		spawnerMem[276].name = BUFFER32;
		spawnerMem[277].name = BUFFER32;
		spawnerMem[278].name = BUFFER32;
		spawnerMem[279].name = BUFFER32;
		spawnerMem[280].name = BUFFER32;
		spawnerMem[281].name = BUFFER32;
		spawnerMem[282].name = BUFFER32;
		spawnerMem[283].name = BUFFER32;
		spawnerMem[284].name = BUFFER32;
		spawnerMem[285].name = BUFFER32;
		spawnerMem[286].name = BUFFER32;
		spawnerMem[287].name = BUFFER32;
		spawnerMem[288].name = BUFFER32;
		spawnerMem[289].name = BUFFER32;
		spawnerMem[290].name = BUFFER32;
		spawnerMem[291].name = BUFFER32;
		spawnerMem[292].name = BUFFER32;
		spawnerMem[293].name = BUFFER32;
		spawnerMem[294].name = BUFFER32;
		spawnerMem[295].name = BUFFER32;
		spawnerMem[296].name = BUFFER32;
		spawnerMem[297].name = BUFFER32;
		spawnerMem[298].name = BUFFER32;
		spawnerMem[299].name = BUFFER32;
	}
}

void update_menu_open_controls_string()
{
	//"Hold %s + Press %s"
	sprintf(ctlMem->m_assembled_strings[ASSEMBLED_STR_PRESS_AND_HOLD_MENU_OPEN], "%s %s + %s %s", ctlMem->m_commonStrings[GXT_STRING_HOLD], ctlMem->m_padButtonStrings[Container.Ui.OpenButton1 - 4], ctlMem->m_commonStrings[GXT_STRING_PRESS], ctlMem->m_padButtonStrings[Container.Ui.OpenButton2 - 4]);
	sprintf(ctlMem->m_assembled_strings[ASSEMBLED_STR_AUTO_AIM_PLAYERS], "%s (%s)", ctlMem->m_commonStrings[GXT_STRING_AUTO_AIM], ctlMem->m_commonStrings[GXT_STRING_PLAYERS]);
	sprintf(ctlMem->m_assembled_strings[ASSEMBLED_STR_AUTO_AIM_PEDS], "%s (%s)", ctlMem->m_commonStrings[GXT_STRING_AUTO_AIM], ctlMem->m_networkObjTypeNames[NET_OBJ_TYPE_PED]);
}

void init_strings_assembled_from_gxt()
{
	ctlMem->m_assembled_strings[ASSEMBLED_STR_PRESS_AND_HOLD_MENU_OPEN] = BUFFER64;
	ctlMem->m_assembled_strings[ASSEMBLED_STR_AUTO_AIM_PLAYERS] = BUFFER64;
	ctlMem->m_assembled_strings[ASSEMBLED_STR_AUTO_AIM_PEDS] = BUFFER64;
	//ASSEMBLED_STR_AUTO_AIM_PLAYERS,SSEMBLED_STR_AUTO_AIM_PEDS,


	//InitMenuDraw(); //load default controls.. //moved to startup so we can do check on menu title.
	update_menu_open_controls_string();


#ifdef Developer
	for (int i = 0; i < ASSEMBLED_STR_COUNT; i++)
	{
		PRINTSTRING("ctlMem->m_assembled_strings[");
		PRINTINT(i);
		PRINTSTRING("] == ");
		PRINTSTRING(ctlMem->m_assembled_strings[i]);
		PRINTNL();
	}
#endif
}

bool are_original_vehicles_safe()
{
	// 🚗 Cars
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_ADMIRAL)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_AIRTUG)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_AMBULANCE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_BANSHEE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_BENSON)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_BIFF)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_BLISTA)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_BOBCAT)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_BOXVILLE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_BUCCANEER)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_BURRITO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_BURRITO2)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_BUS)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_CABBY)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_CAVALCADE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_CHAVOS)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_COGNOSCENTI)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_COMET)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_COQUETTE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_DF8)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_DILETTANTE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_DUKES)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_EMPEROR)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_EMPEROR2)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_ESPERANTO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_FACTION)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_FBI)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_FELTZER)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_FEROCI)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_FIRETRUK)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_FLATBED)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_FORTUNE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_FORKLIFT)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_FUTO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_FXT)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_HABANERO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_HAKUMAI)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_HUNTLEY)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_INFERNUS)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_INGOT)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_INTRUDER)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_LANDSTALKER)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_LOKUS)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_MANANA)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_MARBELLA)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_MERIT)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_MINIVAN)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_MOONBEAM)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_MRTASTY)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_MULE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_NOOSE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_ORACLE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PACKER)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PATRIOT)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PERENNIAL)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PEYOTE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PHANTOM)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PINNACLE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PMP600)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_POLICE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_POLICE2)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_POLPATRIOT)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PONY)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PREMIER)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PRES)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_PRIMO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_RANCHER)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_REBLA)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_RIPLEY)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_ROMERO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_RUINER)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_SABRE)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_SABRE2)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_SABREGT)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_SCHAFTER)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_SENTINEL)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_SOLAIR)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_SPEEDO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_STALION)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_STRETCH)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_SULTAN)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_SULTANRS)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_SUPERGT)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_TAXI)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_TAXI2)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_TRASH)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_TURISMO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_URANUS)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_VIGERO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_VIGERO2)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_VINCENT)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_VIRGO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_VOODOO)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_WASHINGTON)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_WILLARD)) return false;
	if (!IS_THIS_MODEL_A_CAR(VEHICLE_YANKEE)) return false;

	// 🏍️ Bikes
	if (!IS_THIS_MODEL_A_BIKE(VEHICLE_BOBBER)) return false;
	if (!IS_THIS_MODEL_A_BIKE(VEHICLE_FAGGIO)) return false;
	if (!IS_THIS_MODEL_A_BIKE(VEHICLE_HELLFURY)) return false;
	if (!IS_THIS_MODEL_A_BIKE(VEHICLE_NRG900)) return false;
	if (!IS_THIS_MODEL_A_BIKE(VEHICLE_PCJ)) return false;
	if (!IS_THIS_MODEL_A_BIKE(VEHICLE_SANCHEZ)) return false;
	if (!IS_THIS_MODEL_A_BIKE(VEHICLE_ZOMBIEB)) return false;

	// 🚁 Helicopters
	if (!IS_THIS_MODEL_A_HELI(VEHICLE_ANNIHILATOR)) return false;
	if (!IS_THIS_MODEL_A_HELI(VEHICLE_MAVERICK)) return false;
	if (!IS_THIS_MODEL_A_HELI(VEHICLE_POLMAV)) return false;
	if (!IS_THIS_MODEL_A_HELI(VEHICLE_TOURMAV)) return false;

	// 🚤 Boats
	if (!IS_THIS_MODEL_A_BOAT(VEHICLE_DINGHY)) return false;
	if (!IS_THIS_MODEL_A_BOAT(VEHICLE_JETMAX)) return false;
	if (!IS_THIS_MODEL_A_BOAT(VEHICLE_MARQUIS)) return false;
	if (!IS_THIS_MODEL_A_BOAT(VEHICLE_PREDATOR)) return false;
	if (!IS_THIS_MODEL_A_BOAT(VEHICLE_REEFER)) return false;
	if (!IS_THIS_MODEL_A_BOAT(VEHICLE_SQUALO)) return false;
	if (!IS_THIS_MODEL_A_BOAT(VEHICLE_TUGA)) return false;
	if (!IS_THIS_MODEL_A_BOAT(VEHICLE_TROPIC)) return false;

	return true;
}

void check_for_freeze_vehicles()
{
	if (are_original_vehicles_safe())
		bitSet(&ctlMem->m_bit_sets.m_debug, DEBUG_OG_VEHICLES_ARE_MP_SAFE);
}

void setup_military_missile_vars()
{
	ctlMem->StormMissileVars.bullshit = 8;
	ctlMem->StormMissileVars.bullshit2 = 8;
	ctlMem->StormMissileVars.missile_burn_time = 20.0f;
	ctlMem->StormMissileVars.missile_climb_time = 5.0f;
	ctlMem->StormMissileVars.missile_proximity_fuse = 1.0f;
	ctlMem->StormMissileVars.missile_velocity = 5.0f;
	ctlMem->StormMissileVars.missile_flame_trail = true;
	ctlMem->StormMissileVars.missile_equip_with_cam = false;
	ctlMem->StormMissileVars.missile_cam_offset = 1.5f;
	//ctlMem->StormMissileVars.txdPolice;
	//ctlMem->StormMissileVars.spaceHud;
	ctlMem->StormMissileVars.lastTarget = 0;
	ctlMem->StormMissileVars.lockStartTime = 0;
	ctlMem->StormMissileVars.fired_pos.x = 0;
	ctlMem->StormMissileVars.fired_pos.y = 0;
	ctlMem->StormMissileVars.fired_pos.z = 0;
	ctlMem->StormMissileVars.WarheadTypeStrings[0] = "Grenade";
	ctlMem->StormMissileVars.WarheadTypeStrings[1] = "Rocket";
	ctlMem->StormMissileVars.WarheadTypeStrings[2] = "Ship Destroy";
	ctlMem->StormMissileVars.WarheadTypeStrings[3] = "Truck";
	ctlMem->StormMissileVars.MissileChassisStrings[0] = "CJ_BRIAZER";
	ctlMem->StormMissileVars.MissileChassisStrings[1] = "CJ_ROCKET";
	ctlMem->StormMissileVars.MissileChassisStrings[2] = "CJ_HIPPO_BIN";
	ctlMem->StormMissileVars.LinkModeStrings[0] = "Off";
	ctlMem->StormMissileVars.LinkModeStrings[1] = "Stealth";
	ctlMem->StormMissileVars.LinkModeStrings[2] = "Kamikaze";
	ctlMem->StormMissileVars.WarheadTypes[0] = EXPLOSION_GRENADE;
	ctlMem->StormMissileVars.WarheadTypes[1] = EXPLOSION_ROCKET;
	ctlMem->StormMissileVars.WarheadTypes[2] = EXPLOSION_SHIP_DESTROY;
	ctlMem->StormMissileVars.WarheadTypes[3] = EXPLOSION_TRUCK;
	ctlMem->StormMissileVars.MissileChassisInts[0] = OBJECT_CJ_BRIAZER;
	ctlMem->StormMissileVars.MissileChassisInts[1] = OBJECT_CJ_ROCKET;
	ctlMem->StormMissileVars.MissileChassisInts[2] = OBJECT_CJ_HIPPO_BIN;
	ctlMem->StormMissileVars.selectedMissileChassis = 0;
	ctlMem->StormMissileVars.selectedExplosionType = 0;
	ctlMem->StormMissileVars.selectedLinkMode = 1;
}

void init_char_buffers()
{
	ctlMem->m_char_buffers.keyboard_input_buffer = BUFFER128;
	ctlMem->m_char_buffers.player_name_backup[0] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[1] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[2] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[3] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[4] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[5] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[6] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[7] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[8] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[9] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[10] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[11] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[12] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[13] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[14] = BUFFER16;
	ctlMem->m_char_buffers.player_name_backup[15] = BUFFER16;
	//char joining_players_bar_buffer[96]; //this is used in a diff script so we do this to avoid any possible funky business.
}

void init_ctl_mem()
{
	init_char_buffers();
	load_cartel_memory_with_auths();
	setup_military_missile_vars();
	ctlMem->m_chauffeur.speed = 15.0f; //could byte this and TO_FLOAT it but meh.
	ctlMem->m_follow_driver_data.exit_vehicle_input_pressed_timeout_tick = (GetTickCount() + 4500);
	ctlMem->m_follow_driver_data.SeatIndex = 1337; //invalid
	ctlMem->m_retail_prot.m_model_prot_mode = MODEL_PROT_LOBBY;
	ctlMem->m_retail_prot.m_obj_prot_mode = OBJ_PROT_MODE_BOTH;
	ctlMem->m_padButtonsMandatoryDrawAlpha = 255; //decrease by 1 till 0 and then kill it if user has turned off.
	init_button_strings();
	init_net_obj_type_strings();
	init_gamemode_strings();
	init_common_strings_from_gxt();
	init_strings_assembled_from_gxt();
	ctlMem->m_chat_msg_display_time_seconds = 10; //default.
	ctlMem->m_language = GET_CURRENT_LANGUAGE();

	strcpy(ctlMem->m_netDisplay.hostName, "LOADING..", 32);
	strcpy(ctlMem->m_netDisplay.frameRateString, "FPS: ~w~0", 12);
	sprintf(ctlMem->m_netDisplay.emptySlots_360UserCountString, "Empty slots: ~w~%i", (NETWORK_GET_MAX_SLOTS() - 1)); //updates when player counts change.
	sprintf(ctlMem->m_netDisplay.playerCountString, "%s: ~w~1", ctlMem->m_commonStrings[GXT_STRING_PLAYERS], 19);
	strcpy(ctlMem->m_netDisplay.moddingString, "Modding: ~s~1", 17);
	init_keyboard_variables(); //make sure this is below all the other init functions concerning ctlMem strings.

	WAIT(0);
	//sizeof(ctl_network_display);

	ctlMem->m_bit_sets.m_network = (1 << NETWORK_IGNORE_COMPLAINTS) |
		(1 << NETWORK_SCO_PROT_GROUP_HACK) | //put checks in each to ensure they check for xex presence.
		(1 << NETWORK_SCO_PROT_TIDLE_CRASH) |
		(1 << NETWORK_PROT_FLIP_MODEL_OBJECT_ATTACHED) |
		(1 << NETWORK_SCO_PROT_SUPERLOD) |
		(1 << NETWORK_SCO_PROT_CUTSCENE) |
		(1 << NETWORK_PROT_AUTO_KICK);

	//if (!NETWORK_IS_GAME_RANKED()) //maybe 
	ctlMem->m_bit_sets.m_player = (1 << playerGodMode) |
		(1 << neverWanted);

	ctlMem->m_bit_sets.m_weapons = (1 << unlimitedAmmo); // |
		//(1 << forceAutoAimPlayers);

	ctlMem->m_bit_sets.m_obj_spawner = (1 << PREF_OBJ_SPAWNER_INVINCIBLE) |
		(1 << PREF_OBJ_SPAWNER_COLLISION) |
		(1 << PREF_OBJ_SPAWNER_FROZEN);

	ctlMem->m_bit_sets.m_menuPreferences = (1 << PREF_TOP_DRAW_JOINING_GHOSTS_PLAYERS) |
		(1 << PREF_BOTTOM_DRAW_FPS_PLAYERS_360USERS) |
		(1 << PREF_MINIMAP_DRAW_TALKING_PLAYERS) |
		(1 << PREF_DRAW_OPEN_CONTROLS) |
		(1 << PREF_REPORT_ATTACKS_TO_OTHERS) |
		(1 << PREF_LISTEN_FOR_NETUSER_ATTACKS) |
#ifdef Moderator
		(1 << PREF_HOST_AUTO_KICK_ATTACKERS_AGAINST_FRIENDS) |
		(1 << PREF_VOTE_KICK_FRIENDS_ATTACKER) |
		(1 << PREF_VOTE_KICK_ATTACKERS) |
		(1 << PREF_AUTO_VOTE_KICK_DICKHEADS) | //could prob do with checking we meet the threshhold.
#else
		(1 << PREF_HOST_AUTO_KICK_ATTACKERS_AGAINST_FRIENDS) |
		(1 << PREF_VOTE_KICK_FRIENDS_ATTACKER) |
		(1 << PREF_VOTE_KICK_ATTACKERS) |
#endif
		(1 << PREF_ADMIN_ONLY_OFFHOST_ATTACKERS) |
		(1 << PREF_ADMIN_ONLY_OFFHOST_ATTACKS_AGAINST_FRIENDS) | //code only compiled in admin/developer build so meh fuck it.
		(1 << PREF_SET_GHOSTS_ACTIVE) |
		(1 << PREF_LISTEN_FOR_NETUSER_ATTACKS_FRIENDS_ONLY);

	ctlMem->m_bit_sets.m_vehicle = (1 << spawnVehiclesInvincible) |
		(1 << spawnedVehiclesRequireBreakIn) |
		(1 << vehicleInvincibleLoop) |
		(1 << deleteLastFunnyVehObjects) |
		(1 << deleteLastNeonVehObjects) |
		(1 << vehicleAlwaysClean) |
		(1 << chauffeurStuckChecks) |
		(1 << bypassVehicleDoorLocks) |
#ifdef Developer
		(1 << blockVehicleNetMigration) |
		(1 << autoWarpDriversNewVehicle) |
#else
		(1 << blockVehicleNetMigration) |
#endif
		(1 << local_autoflip);

	if (CartelXEXPresent)
	{
		//CTL_SET_CLONE_SPAM_LOCKOUT_COUNT
		int *protect_bits = CTL_GET_EVENT_BITSET();
		*protect_bits = (1 << REQUEST_CONTROL_EVENT) |
			(1 << GIVE_CONTROL_EVENT) |
			(1 << OBJECT_ID_FREED_EVENT) |
			(1 << WEAPON_DAMAGE_EVENT) |
			(1 << REQUEST_PICKUP_EVENT) |
			(1 << GAME_CLOCK_AND_WEATHER_EVENT) |
			(1 << RESURRECTED_LOCAL_PLAYER_EVENT) | //was causing anims to de-sync when players respawn so re-enabled it.
			(1 << VEHICLE_COMPONENT_CONTROL_EVENT) | //might have to get rid in future
			(1 << REQUEST_FIRE_EVENT) |
			(1 << START_FIRE_EVENT) |
			(1 << REQUEST_EXPLOSION_EVENT) |
			(1 << START_EXPLOSION_EVENT) |
			(1 << START_PROJECTILE_EVENT) |
			(1 << SESSION_SETTINGS_CHANGED_EVENT) |
			(1 << ALTER_WANTED_LEVEL_EVENT) |
			(1 << CREATE_PICKUP_EVENT) |
			(1 << CHANGE_RADIO_STATION_EVENT) |
			(1 << UPDATE_GPS_EVENT) |
			(1 << MARK_AS_NO_LONGER_NEEDED_EVENT) |
			(1 << PLAYER_TAUNT_EVENT) |
			//(1 << DOOR_BREAK_EVENT) |  //lets people remove your vehicle doors lol
			(1 << HOST_VARIABLES_VERIFY_EVENT);
	}
}

void init_free_mem()
{ //we don't memset this to 0 so it will be necessary to 0 some values.
	freeMem->m_timed_ramps->m_deletionDelaySeconds = 10;
	freeMem->m_timed_ramps->m_rampSpawnAmount = 1;
	freeMem->m_walk_on_air.m_tick = 0;
	init_pprx_url_base_string();
	//generate random float to set object health locally (so we can skip already dealt with laglights/objects).
}

bool start_cartel_helper()
{
	if (!DOES_SCRIPT_EXIST("cartel_helper"))
		return false;

	if (GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT("cartel_helper") > 0) //to account for menu restarts.. 
	{
		TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cartel_helper");
		WAIT(10);
	}

	REQUEST_SCRIPT("cartel_helper");
	while (!HAS_SCRIPT_LOADED("cartel_helper"))
		WAIT(0);

	//THREADID thread = START_NEW_SCRIPT_WITH_ARGS("cartel_helper", (uint*)&ctlMem, 1, 1024); //sorted
	freeMem->m_CartelHelper_ThreadId = START_NEW_SCRIPT_WITH_ARGS("cartel_helper", (uint*)&ctlMem, 1, 1024); 
	MARK_SCRIPT_AS_NO_LONGER_NEEDED("cartel_helper");

	//return thread;
	return freeMem->m_CartelHelper_ThreadId;
}

void make_player_settings_legit(int un1, int un2) 
{
	bitReset(&ctlMem->m_bit_sets.m_player, neverWanted);
	rigNeverWantedGlobal(0, 0);

	bitReset(&ctlMem->m_bit_sets.m_player, playerGodMode);
	SET_PLAYER_INVINCIBLE(myPlayerIndex, false);
	SET_CHAR_PROOFS(myPlayerPed, false, false, false, false, false); //not found in any network scripts so this is fine.

	bitReset(&ctlMem->m_bit_sets.m_player, Invisibility);
	SET_CHAR_VISIBLE(myPlayerPed, true);

	bitReset(&ctlMem->m_bit_sets.m_vehicle, disabledRunwayPlanes);
	SWITCH_AMBIENT_PLANES(true);

	bitReset(&ctlMem->m_bit_sets.m_vehicle, vehicleInvincibleLoop);
	bitReset(&ctlMem->m_bit_sets.m_vehicle, spawnVehiclesInvincible); //someone will do this undoubtedly even though they shouldn't.
	bitReset(&ctlMem->m_bit_sets.m_vehicle, bypassVehicleDoorLocks);
	bitReset(&ctlMem->m_bit_sets.m_vehicle, vehicleAlwaysClean);
	bitReset(&ctlMem->m_bit_sets.m_vehicle, autoWarpDriversNewVehicle); //meh normal players shouldn't be re-spawning vehicles anyway, but just incase..
	bitReset(&ctlMem->m_bit_sets.m_vehicle, storm_flying_car);
	bitReset(&ctlMem->m_bit_sets.m_weapons, unlimitedAmmo);
	bitReset(&ctlMem->m_bit_sets.m_weapons, unlimitedClip);
	bitReset(&ctlMem->m_bit_sets.m_weapons, forceNeverTargettedVehicle);

	bitReset(&ctlMem->m_bit_sets.m_weapons, forceNeverTargettedPed);
	SET_CHAR_NEVER_TARGETTED(myPlayerPed, false);

	bitReset(&ctlMem->m_bit_sets.m_weapons, gravityGun);
	//bitReset(&ctlMem->m_bit_sets.m_weapons, gravityGunExplodeMode); //not used unless gravityGun bit is set

	DISABLE_PLAYER_AUTO_VEHICLE_EXIT(myPlayerIndex, false); //not called by any script oddly lol
	printf("~r~CTL: ~w~Legit mode set, godmode etc disabled.");
}

void dynamic_read_data_from_saved(int block_autoSave, int un2) //if IS_NETWORK_SESSION it reads from scores, else gamesave
{
	saveData_storage* saveData;
	unsigned int score_data[25]; //no way around this unless we get the base address of the scores.

	if (NetworkGame)
	{  //I'm sure I could just do saveData = (saveData_storage*)BaseScoreAddress, I bet these differ between OG and EFLC though so meh, this is better than two functions with duplicate logic for reading/loading - Nath
		const uint base_score = (myCurrentEpisode == 0 ? 115 : 90);
		for (int i = 0; i < 25; i++)
			score_data[i] = GET_ONLINE_SCORE(base_score + i);

		saveData = (saveData_storage*)score_data; //works fine with automatic loading.
	}
	else
		saveData = (saveData_storage*)GAMESAVE_LOCATION;
		//memcpy(&saveData, (int*)GAMESAVE_LOCATION, sizeof(saveData_storage));

	if (saveData->SBYT[SBYT_SAVE_DATA_VERSION] == SAVE_DATA_VERSION_BYTE)
	{
		//bool MIDMatchedAdminSet = bitTest(ctlMem->m_bit_sets.m_admin, ADMIN_SECURITY_BIT_MACHINE_ID_MATCHED); //not touched by this func.
		bool StoreXUIDFuncCalled = bitTest(ctlMem->m_bit_sets.m_player, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED);
		bool MIDsecurityBit = bitTest(ctlMem->m_bit_sets.m_vehicle, SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED);
		bool XUIDsecurityBit = bitTest(ctlMem->m_bit_sets.m_player, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED);
		ctlMem->m_bit_sets.m_player = saveData->m_playerBits;
		ctlMem->m_bit_sets.m_vehicle = saveData->m_vehicleBits;
		ctlMem->m_bit_sets.m_weapons = saveData->m_weaponBits;

		SET_PLAYER_INVINCIBLE(myPlayerIndex, bitTest(ctlMem->m_bit_sets.m_player, playerGodMode));
		//SET_CHAR_VISIBLE(myPlayerPed, !bitTest(ctlMem->m_bit_sets.m_player, Invisibility)); //nah they too dumb for this
		bitReset(&ctlMem->m_bit_sets.m_player, Invisibility);

		SET_CHAR_NEVER_TARGETTED(myPlayerPed, bitTest(ctlMem->m_bit_sets.m_weapons, forceNeverTargettedPed));

		bitReset(&ctlMem->m_bit_sets.m_player, inChauffeurSubmenu);
		bitReset(&ctlMem->m_bit_sets.m_player, sharedIsSelectedPlayerMuted);

		if (bitTest(ctlMem->m_bit_sets.m_vehicle, local_autoflip))
			SET_BIT(&ctlMem->m_bit_sets.m_moneyDrop_vehicleAutoFlip, (myPlayerIndex + 16));

		if (MIDsecurityBit)
			bitSet(&ctlMem->m_bit_sets.m_vehicle, SECURITY_BIT_VEHICLESET_OBFUSCATE_MACHINE_ID_FUNCTION_CALLED);

		onlineScoreBitSet = GET_ONLINE_SCORE(cartel_online_bitset_score_id); //LEAVE AS IS
		delete_distance = GET_ONLINE_SCORE(cartel_online_score_id_delete_distance); //LEAVE AS IS

		bool xuidStored = bitTest(*(int*)stickyBitSet, STICK_XUID_STORED);

		if (XUIDsecurityBit)
			bitSet(&ctlMem->m_bit_sets.m_player, SECURITY_BIT_OBFUSCATE_XUID_FUNCTION_CALLED); //intentionally ordered annoyingly.

		*(int*)stickyBitSet = saveData->m_stickyBits;

		if (xuidStored)
			bitSet((int*)stickyBitSet, STICK_XUID_STORED);
		if (CartelXEXPresent) //we set this on script startup so it won't get overwritten here.
			bitSet((int*)stickyBitSet, STICK_XEX_PRESENT); //keep original value.

		time_weather_data.m_timeHour = saveData->SBYT[SBYT_TIME_HOUR];
		time_weather_data.m_timeMinute = saveData->SBYT[SBYT_TIME_MINUTE];
		time_weather_data.m_weather = saveData->SBYT[SBYT_WEATHER];
		Container.Ui.OpenButton1 = saveData->SBYT[SBYT_MENU_BUTTON_HOLD];
		Container.Ui.OpenButton2 = saveData->SBYT[SBYT_MENU_BUTTON_PRESS];
		Container.Ui.HeaderColor.r = saveData->SBYT[SBYT_HEADER_COL_1];
		Container.Ui.HeaderColor.g = saveData->SBYT[SBYT_HEADER_COL_2];
		Container.Ui.HeaderColor.b = saveData->SBYT[SBYT_HEADER_COL_3];
		Container.Ui.HeaderColor.a = saveData->SBYT[SBYT_HEADER_ALPHA];
		Container.Ui.HeaderTextColor.r = saveData->SBYT[SBYT_HEADERTXT_COL_1];
		Container.Ui.HeaderTextColor.g = saveData->SBYT[SBYT_HEADERTXT_COL_2];
		Container.Ui.HeaderTextColor.b = saveData->SBYT[SBYT_HEADERTXT_COL_3];
		Container.Ui.HeaderTextColor.a = saveData->SBYT[SBYT_HEADERTXT_ALPHA];
		Container.Ui.SubtitleColor.r = saveData->SBYT[SBYT_SUBTITLE_COL_1];
		Container.Ui.SubtitleColor.g = saveData->SBYT[SBYT_SUBTITLE_COL_2];
		Container.Ui.SubtitleColor.b = saveData->SBYT[SBYT_SUBTITLE_COL_3];
		Container.Ui.SubtitleColor.a = saveData->SBYT[SBYT_SUBTITLE_ALPHA];
		Container.Ui.TextColor.r = saveData->SBYT[SBYT_TEXT_COL_1];
		Container.Ui.TextColor.g = saveData->SBYT[SBYT_TEXT_COL_2];
		Container.Ui.TextColor.b = saveData->SBYT[SBYT_TEXT_COL_3];
		Container.Ui.TextColor.a = saveData->SBYT[SBYT_TEXT_ALPHA];
		Container.Ui.BackgroundColor.r = saveData->SBYT[SBYT_BACKGROUND_COL_1];
		Container.Ui.BackgroundColor.g = saveData->SBYT[SBYT_BACKGROUND_COL_2];
		Container.Ui.BackgroundColor.b = saveData->SBYT[SBYT_BACKGROUND_COL_3];
		Container.Ui.BackgroundColor.a = saveData->SBYT[SBYT_BACKGROUND_ALPHA];
		Container.Ui.SelectedTextColor.r = saveData->SBYT[SBYT_SEL_TEXT_COL_1];
		Container.Ui.SelectedTextColor.g = saveData->SBYT[SBYT_SEL_TEXT_COL_2];
		Container.Ui.SelectedTextColor.b = saveData->SBYT[SBYT_SEL_TEXT_COL_3];
		Container.Ui.SelectedTextColor.a = saveData->SBYT[SBYT_SEL_TEXT_ALPHA];
		Container.Ui.ScrollBarColor.r = saveData->SBYT[SBYT_SCROLLBAR_COL_1];
		Container.Ui.ScrollBarColor.g = saveData->SBYT[SBYT_SCROLLBAR_COL_2];
		Container.Ui.ScrollBarColor.b = saveData->SBYT[SBYT_SCROLLBAR_COL_3];
		Container.Ui.ScrollBarColor.a = saveData->SBYT[SBYT_SCROLLBAR_ALPHA];

		ctlMem->m_chat_msg_display_time_seconds = saveData->SBYT[SBYT_CHAT_MSG_DISPLAY_SECONDS];

		byte* HealthColor = (byte*)GetHealthHUDColourAddress();
		HealthColor[0] = saveData->SBYT[SBYT_HEALTH_COL_1];
		HealthColor[1] = saveData->SBYT[SBYT_HEALTH_COL_2];
		HealthColor[2] = saveData->SBYT[SBYT_HEALTH_COL_3];

		byte* ArmorColor = (byte*)GetArmourHUDColourAddress();
		ArmorColor[0] = saveData->SBYT[SBYT_ARMOUR_COL_1];
		ArmorColor[1] = saveData->SBYT[SBYT_ARMOUR_COL_2];
		ArmorColor[2] = saveData->SBYT[SBYT_ARMOUR_COL_3];

		byte* PauseColor = (byte*)0x82FB30BD; //this is the one we set
		PauseColor[0] = saveData->SBYT[SBYT_PAUSE_TXT_COL_1];
		PauseColor[1] = saveData->SBYT[SBYT_PAUSE_TXT_COL_2];
		PauseColor[2] = saveData->SBYT[SBYT_PAUSE_TXT_COL_3];
		setPauseColours(0, 0); //carries these over to the other bytes for pause txt colour

		Container.Ui.DrawPos = saveData->menuPos;

		freeMem->m_savedVehicleModel = saveData->m_savedVehicleModel;
		freeMem->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits = saveData->m_UpgradesOn_WindowsSmashed_DoorsBroken_TyresBurst_bits; //first 10 = upgrades, windows = 4, doors = 4, tyres = 6

		for (int i = 0; i < 4; i++)
			freeMem->m_savedVehicleColours[i] = saveData->m_savedVehicleColours[i];

		ctlMem->m_bit_sets.m_menuPreferences = saveData->m_menuPreferencesBits;

		SWITCH_AMBIENT_PLANES(!bitTest(ctlMem->m_bit_sets.m_vehicle, disabledRunwayPlanes)); //should remain compatible.
		if (StoreXUIDFuncCalled)
			bitSet(&ctlMem->m_bit_sets.m_player, SECURITY_BIT_STORE_XUID_FUNCTION_CALLED);

		//DISABLE_PLAYER_AUTO_VEHICLE_EXIT(myPlayerPed, IS_BIT_SET(ctlMem->m_bit_sets.m_vehicle, blockVehicleEjectHijack)); //kudos stuart
		update_menu_open_controls_string();
		load_cartel_chat();
		rigNeverWantedGlobal(0, 0);

		if (!CartelXEXPresent)
			bitReset(&ctlMem->m_bit_sets.m_player, syncTimeToRealWorld);

		ALLOW_LOCKON_TO_RANDOM_PEDS(myPlayerIndex, bitTest(ctlMem->m_bit_sets.m_weapons, forceAutoAimPeds));
		//ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS(false);
		if (NetworkGame)
		{
			printf("~r~CTL: ~w~Settings loaded from GTA IV servers.");
			if ((NETWORK_GET_GAME_MODE() == GAME_MODE_FREE_MODE || NETWORK_GET_GAME_MODE() == GAME_MODE_PARTY_MODE) && !NETWORK_IS_GAME_RANKED())
			{
				if (bitTest(ctlMem->m_bit_sets.m_weapons, forceAutoAimPlayers))
				{
					DISABLE_PLAYER_LOCKON(myPlayerIndex, false);
					ALLOW_LOCKON_TO_FRIENDLY_PLAYERS(myPlayerIndex, true);
				}
				DISABLE_PLAYER_AUTO_VEHICLE_EXIT(myPlayerPed, bitTest(ctlMem->m_bit_sets.m_vehicle, blockVehicleEjectHijack));
			}
			else
				make_player_settings_legit(0, 0); //should prob move to register broadcast variables or whatever... they won't necessarily have auto-load enabled.
		}
		else
		{
			printf("~r~CTL: ~w~Settings loaded from gamesave.");
			// if (!NetworkGame && GET_BITS_IN_RANGE(ctlMem->m_bit_sets.m_player, attachedToWalker, printedNewDickheadIdentifiers))
			if (GET_BITS_IN_RANGE(ctlMem->m_bit_sets.m_player, attachedToWalker, printedNewDickheadIdentifiers))
			{
				bitReset(&ctlMem->m_bit_sets.m_player, attachedToWalker); //might not need this
				bitReset(&ctlMem->m_bit_sets.m_player, printedNewDickheadIdentifiers); //might not need this
				bitReset(&ctlMem->m_bit_sets.m_player, networkRank10All); //probably keep this as is, newtwork bitset is mainly runtime bits / shit user shouldn't really be wanting to change or save.
				bitReset(&saveData->m_playerBits, attachedToWalker);
				bitReset(&saveData->m_playerBits, printedNewDickheadIdentifiers);
				bitReset(&saveData->m_playerBits, networkRank10All); //probably keep this as is, newtwork bitset is mainly runtime bits / shit user shouldn't really be wanting to change or save.
				if(!block_autoSave)
					DO_AUTO_SAVE();
			}
		}
	}
	else
		printf("~r~CTL: ~w~Saved data is incompatible with current version, re-save to overwrite.");
}

/*
void read_saved_data(int unused0, int unused1)
{
	if (NetworkGame)
		read_bits_from_online_scores(0, 0);
	else
		read_data_from_gamesave(0, 0);
}
*/
void save_data(int unused0, int unused1)
{
	if (NetworkGame)
		save_bits_to_online_scores(0, 0);
	else
		save_data_to_gamesave(0, 0);
}

void draw_menu_open_inputs()
{ 
	bool userHasEnabled = bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_DRAW_OPEN_CONTROLS);
	if (ctlMem->m_padButtonsMandatoryDrawAlpha > 0)
		ctlMem->m_padButtonsMandatoryDrawAlpha--;
	
	if (userHasEnabled || ctlMem->m_padButtonsMandatoryDrawAlpha > 0)
	{
		//sprintf(buffer, "Hold %s + Press %s", ctlMem->m_padButtonStrings[Container.Ui.OpenButton1 - 4], ctlMem->m_padButtonStrings[Container.Ui.OpenButton2 - 4]);
		if(!userHasEnabled)
			SET_TEXT_COLOUR(255, 255, 255, ctlMem->m_padButtonsMandatoryDrawAlpha);

		if (!NetworkGame || !bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_BOTTOM_DRAW_FPS_PLAYERS_360USERS))
		{
			SET_TEXT_SCALE(0.2250f, 0.4500f);
			DISPLAY_TEXT_WITH_LITERAL_STRING(0.0550f, 0.9575f, "STRING", ctlMem->m_assembled_strings[ASSEMBLED_STR_PRESS_AND_HOLD_MENU_OPEN]);
		}
		else
		{
			SET_TEXT_SCALE(0.16f, 0.32f);
			SET_TEXT_DROPSHADOW(false, 0, 0, 0, 0); //needa work with these..
			DISPLAY_TEXT_WITH_LITERAL_STRING(0.135f, 0.935f, "STRING", ctlMem->m_assembled_strings[ASSEMBLED_STR_PRESS_AND_HOLD_MENU_OPEN]);
		}
	}
}



//INFO: title is what you see at the top of the keyboard.
//use maxlength with hexmode.
//editString is a string you can use to pre-fill the buffer.
//fadeMs = experimental. 0 = wont have any fade.
//posX = 0.5 posY = 0.5 = center of screen.
bool showAndGetKeyboardInput(const char *title, bool useHexMode, const char *editString, int maxLength, int fadeMs, float posX, float posY, int r, int g, int b)
{
	//decrypt_keyboard_creator();
	strcpy(ctlMem->m_char_buffers.keyboard_input_buffer, (GET_LENGTH_OF_LITERAL_STRING(editString) < MAX_KEYBOARD_INPUT_SIZE ? editString : ""), MAX_KEYBOARD_INPUT_SIZE);
	maxLength = (maxLength > MAX_KEYBOARD_INPUT_SIZE) ? MAX_KEYBOARD_INPUT_SIZE : maxLength;

	const float startKeyboardX = posX - 0.2205f;
	const float startKeyboardY = posY - 0.19f;
	const float keyboardSpacingX = 0.046f;
	const float keyboardSpacingY = 0.08f;
	const int rows = 4;
	const int cols = 10;

	const float selectorX = startKeyboardX + 0.015f;
	const float selectorY = startKeyboardY + 0.02f;

	int selectedRow = 0;
	int selectedCol = 0;
	int selectedPage = useHexMode == false ? 0 : 4;

	float scale = 1.0f;
	int fader = 0;

	//TextureDict broswer = LOAD_TXD("browser"); //we use this already for the menu so //extern TextureDict browser; at the top of this file.
	Texture keyboardTxd = GET_TEXTURE(browser, "keyboard");
	Texture selectedTxd = GET_TEXTURE(browser, "keyboardbuttonhover");
	Texture doubleSelectedTxd = GET_TEXTURE(browser, "keyboardbuttondoubleon");

	const float rowOffset[4] = { 0.0f, 0.02f, 0.02f, 0.02f };

	/*
	char* keys[5][4][10] = {
		{
			{"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"},
			{"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"},
			{"a", "s", "d", "f", "g", "h", "j", "k", "l", "$!"},
			{"z", "x", "c", "v", "b", "n", "m", "~BLIP_24~", "Done", " "}
		},
		{
			{"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"},
			{"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"},
			{"A", "S", "D", "F", "G", "H", "J", "K", "L", "$!"},
			{"Z", "X", "C", "V", "B", "N", "M", "~BLIP_24~", "Done", " "}
		},
		{
			{"~g~.", "~y~.", "~w~.", "~r~.", "~b~.", "~p~.", "~l~.", "~c~.", "~m~.", "~s~."},
			{"!", "?", "@", "$", "%", "#", "&", "^", "(", ")"},
			{"-", "+", "{", "}", "|", ":", "<", ">", " ", "ab"},
			{"*", "/", "%", "=", "¬", "[", "]", "~BLIP_24~", "Done", " "},
		},
		{
			{"~PAD_UP~", "~PAD_DPAD_DOWN~", "~PAD_DPAD_LEFT~", "~PAD_DPAD_RIGHT~", "~PAD_LB~", "~PAD_RB~", "~PAD_LT~", "~PAD_RT~", "~PAD_BACK~", "~PAD_START~"},
			{"~PAD_A~", "~PAD_B~", "~PAD_X~", "~PAD_Y~", "~BLIP_21~", "~BLIP_22~", "~BLIP_29~", "~BLIP_30~", "~BLIP_82~", "~BLIP_46~"},
			{"~BLIP_77~", "~BLIP_81~", "~BLIP_49~", "~BLIP_78~", "~BLIP_51~", "~BLIP_52~", "~BLIP_76~", "~BLIP_54~", "~BLIP_55~", "ab"},
			{"~BLIP_56~", "~BLIP_57~", "~BLIP_58~", "~BLIP_59~", "~BLIP_60~", "~BLIP_61~", "~BLIP_53~", "$!", "Done", " "},
		},
		{
			{"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"},
			{"X", "A", "B", "C", "D", "E", "F", " ", " ", " "},
			{" ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
			{" ", " ", " ", " ", " ", " ", " ", " ", "Done", " "}
		}
	};
	*/

	PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");

	size_t input_length;
	while (!IS_BUTTON_JUST_PRESSED(0, BUTTON_START) && !IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_ENTER))
	{
		WAIT(0);
		if(IS_BUTTON_JUST_PRESSED(0,BUTTON_Y))
			BlockYVehicleExitJustPressed();

		if (IS_BUTTON_JUST_PRESSED(0, BUTTON_B) || IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_ESC))
			return 0;

		if (NetworkGame)
			AdminClientLoop(); //stop people evading admin cmds.

		if (IS_BUTTON_JUST_PRESSED(0, STICK_LEFT))
			SET_CHAR_DUCKING(myPlayerPed, !IS_CHAR_DUCKING(myPlayerPed)); //we don't grab myPlayerPed during keyboard hurrr...

		if (fader < fadeMs)
		{
			fader++;
			scale = (float)fader / fadeMs;
		}

		input_length = GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer);

		DRAW_SPRITE(keyboardTxd, posX, posY, 0.5f*scale, 0.5f*scale, 0.0f, r, g, b, 255);
		set_up_draw(true, 0.3f*scale, 0.3f*scale, 255, 255, 255, 200);
		DISPLAY_TEXT_WITH_LITERAL_STRING(posX, startKeyboardY + 0.06f, "STRING", ctlMem->m_keyboard.made_by_storm);
		if (title != NULL && GET_LENGTH_OF_LITERAL_STRING(title) > 0)
		{
			set_up_draw(true, 0.45f*scale, 0.45f*scale, 255, 255, 255, 200);
			DISPLAY_TEXT_WITH_LITERAL_STRING(posX, startKeyboardY - 0.06f, "STRING", title);
		}

		for (int row = 0; row < rows; row++)
		{
			for (int col = 0; col < cols; col++)
			{
				const float x = startKeyboardX + (col * keyboardSpacingX) + 0.013f;
				const float y = startKeyboardY + (row * keyboardSpacingY) + rowOffset[row] + 0.011f;
				char *keyToDisplay = ctlMem->m_keyboard.keys[selectedPage][row][col];
				bool isDoneString = COMPARE_STRING(keyToDisplay, "Done");
				if (row == selectedRow && col == selectedCol)
				{
					if (isDoneString)
						DRAW_SPRITE(doubleSelectedTxd, selectorX + (col * keyboardSpacingX) + keyboardSpacingX * 0.5f, selectorY + (row * keyboardSpacingY) + rowOffset[row], 0.1f*scale, 0.08f*scale, 0.0f, 255, 255, 255, 250);
					else
						DRAW_SPRITE(selectedTxd, selectorX + (col * keyboardSpacingX), selectorY + (row * keyboardSpacingY) + rowOffset[row], 0.05f*scale, 0.08f*scale, 0.0f, 255, 255, 255, 250);
				}
				set_up_draw(!isDoneString, 0.4f*scale, 0.4f*scale, 255, 255, 255, 255);
				DISPLAY_TEXT_WITH_LITERAL_STRING(x, y, "STRING", keyToDisplay);
			}
		}

		if (input_length == 0)
		{
			set_up_draw(false, 0.35*scale, 0.35*scale, 255, 255, 255, 255);
			DISPLAY_TEXT_WITH_LITERAL_STRING(startKeyboardX + (0 * keyboardSpacingX), startKeyboardY + rows * keyboardSpacingY + 0.02f, "STRING", "~PAD_DPAD_ALL~ = Move");
			USE_PREVIOUS_FONT_SETTINGS();
			DISPLAY_TEXT_WITH_LITERAL_STRING(startKeyboardX + (0 * keyboardSpacingX), startKeyboardY + rows * keyboardSpacingY + keyboardSpacingY * 0.75f, "STRING", "~PAD_A~ = Select");
			USE_PREVIOUS_FONT_SETTINGS();
			DISPLAY_TEXT_WITH_LITERAL_STRING(startKeyboardX + (3.3 * keyboardSpacingX), startKeyboardY + rows * keyboardSpacingY + 0.02f, "STRING", "~PAD_X~ = Delete");
			USE_PREVIOUS_FONT_SETTINGS();
			DISPLAY_TEXT_WITH_LITERAL_STRING(startKeyboardX + (3.3 * keyboardSpacingX), startKeyboardY + rows * keyboardSpacingY + keyboardSpacingY * 0.75f, "STRING", "~PAD_LSTICK_ALL~ = Capitals");
			USE_PREVIOUS_FONT_SETTINGS();
			DISPLAY_TEXT_WITH_LITERAL_STRING(startKeyboardX + (7.3 * keyboardSpacingX), startKeyboardY + rows * keyboardSpacingY + 0.02f, "STRING", "~PAD_Y~ = Space");
			USE_PREVIOUS_FONT_SETTINGS();
			DISPLAY_TEXT_WITH_LITERAL_STRING(startKeyboardX + (7.3 * keyboardSpacingX), startKeyboardY + rows * keyboardSpacingY + keyboardSpacingY * 0.75f, "STRING", "~PAD_START~ = Finish");
		}
		else
		{
			const float size = input_length < 40 ? 0.35f : 0.2f;
			set_up_draw(false, size*scale, size*scale, 255, 255, 255, 255);
			DISPLAY_TEXT_WITH_LITERAL_STRING(startKeyboardX, startKeyboardY + (rows * keyboardSpacingY) + 0.04f, "STRING", ctlMem->m_char_buffers.keyboard_input_buffer);
			char* anzahl = BUFFER16;
			strcpy(anzahl, "(", 16);
			straddi(anzahl, input_length, 16);
			stradd(anzahl, " / ", 16);
			straddi(anzahl, maxLength, 16);
			stradd(anzahl, ")", 16);
			set_up_draw(true, 0.3f*scale, 0.3f*scale, 255, 255, 255, 255);
			DISPLAY_TEXT_WITH_LITERAL_STRING(startKeyboardX + ((cols - 1.2f) * keyboardSpacingX), startKeyboardY + (rows * keyboardSpacingY + 0.02f), "STRING", anzahl);
		}

		if (IS_BUTTON_JUST_PRESSED(0, STICK_LEFT) || IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_NUMPAD_9))
		{
			if (selectedPage < 2)
				selectedPage = (selectedPage + 1) % 2;
		}
		else if (IS_BUTTON_JUST_PRESSED(0, DPAD_UP) || IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_NUMPAD_8))
		{
			PLAY_AUDIO_EVENT("RADIO_RETUNE_BEEP");
			selectedRow = (selectedRow - 1 + rows) % rows;
			if (selectedCol == cols - 1 && selectedRow == rows - 1)
				selectedCol--;
		}
		else if (IS_BUTTON_JUST_PRESSED(0, DPAD_DOWN) || IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_NUMPAD_2))
		{
			PLAY_AUDIO_EVENT("RADIO_RETUNE_BEEP");
			if (selectedCol == cols - 1 && selectedRow + 1 == rows - 1)
				selectedCol--;

			selectedRow = (selectedRow + 1) % rows;
		}
		else if (IS_BUTTON_JUST_PRESSED(0, DPAD_LEFT) || IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_NUMPAD_4))
		{
			PLAY_AUDIO_EVENT("RADIO_RETUNE_BEEP");
			selectedCol = (selectedCol - 1 + cols) % cols;
			if (selectedCol == cols - 1 && selectedRow == rows - 1)
				selectedCol--;
		}
		else if (IS_BUTTON_JUST_PRESSED(0, DPAD_RIGHT) || IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_NUMPAD_6)) 
		{
			PLAY_AUDIO_EVENT("RADIO_RETUNE_BEEP");
			if (selectedCol + 1 == cols - 1 && selectedRow == rows - 1) selectedCol++;
				selectedCol = (selectedCol + 1) % cols;
		}
		else if (IS_BUTTON_JUST_PRESSED(0, BUTTON_A))
		{
			const char* selectedKey = ctlMem->m_keyboard.keys[selectedPage][selectedRow][selectedCol];
			if (useHexMode && COMPARE_STRING(selectedKey, " ")) 
				continue;

			if (COMPARE_STRING(selectedKey, "$!"))
			{
				PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_SERVER_OPTION_CHANGE");
				selectedPage = 2;
			}
			else if (COMPARE_STRING(selectedKey, "ab")) //think there is a bug here somewhere,
			{
				PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_SERVER_OPTION_CHANGE");
				selectedPage = 0;
			}
			else if (COMPARE_STRING(selectedKey, "~BLIP_24~"))
			{
				PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_SERVER_OPTION_CHANGE");
				selectedPage = 3;
			}
			else if (COMPARE_STRING(selectedKey, "Done")) {
				break;
			}
			else
			{
				size_t len = input_length;
				if (GET_LENGTH_OF_LITERAL_STRING(selectedKey) == 4 && selectedKey[3] == '.')
					selectedKey = GET_STRING_FROM_STRING(selectedKey, 0, 3);

				if (input_length + GET_LENGTH_OF_LITERAL_STRING(selectedKey) <= maxLength)
				{
					PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_SERVER_HIGHLIGHT");
					stradd(ctlMem->m_char_buffers.keyboard_input_buffer, selectedKey, MAX_KEYBOARD_INPUT_SIZE);
				}
			}
		}
		else if (IS_BUTTON_JUST_PRESSED(0, BUTTON_X) || IS_GAME_KEYBOARD_KEY_JUST_PRESSED(KEY_BACKSPACE)) 
		{
			//ideally add detection for holding and delete continously when so
			size_t len = input_length;
			if (len > 0) 
			{
				if (ctlMem->m_char_buffers.keyboard_input_buffer[len - 1] == '~')
				{
					do 
					{
						len--;
					} while (len > 0 && ctlMem->m_char_buffers.keyboard_input_buffer[len - 1] != '~');
				}
				strcpy(ctlMem->m_char_buffers.keyboard_input_buffer, GET_STRING_FROM_STRING(ctlMem->m_char_buffers.keyboard_input_buffer, 0, len - 1), MAX_KEYBOARD_INPUT_SIZE);
			}
			else PLAY_AUDIO_EVENT("BODY_ARMOUR_BUY");
		}
		else if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y)) 
			stradd(ctlMem->m_char_buffers.keyboard_input_buffer, " ", MAX_KEYBOARD_INPUT_SIZE);
		else 
		{
			input_length = GET_LENGTH_OF_LITERAL_STRING(ctlMem->m_char_buffers.keyboard_input_buffer);
			if (input_length == maxLength) 
				continue;

			bool isHoldingShift = (IS_GAME_KEYBOARD_KEY_PRESSED(KEY_LEFT_SHIFT) || IS_GAME_KEYBOARD_KEY_PRESSED(KEY_RIGHT_SHIFT));

			for (int key = KEY_ESC; key <= KEY_NUMPAD_PERIOD; key++) 
			{
				if (IS_GAME_KEYBOARD_KEY_JUST_PRESSED(key)) 
				{
					/*
					char *keyMappings[84][2] = {
						[KEY_A] = { "a", "A" },[KEY_B] = { "b", "B" },[KEY_C] = { "c", "C" },[KEY_D] = { "d", "D" },
						[KEY_E] = { "e", "E" },[KEY_F] = { "f", "F" },[KEY_G] = { "g", "G" },[KEY_H] = { "h", "H" },
						[KEY_I] = { "i", "I" },[KEY_J] = { "j", "J" },[KEY_K] = { "k", "K" },[KEY_L] = { "l", "L" },
						[KEY_M] = { "m", "M" },[KEY_N] = { "n", "N" },[KEY_O] = { "o", "O" },[KEY_P] = { "p", "P" },
						[KEY_Q] = { "q", "Q" },[KEY_R] = { "r", "R" },[KEY_S] = { "s", "S" },[KEY_T] = { "t", "T" },
						[KEY_U] = { "u", "U" },[KEY_V] = { "v", "V" },[KEY_W] = { "w", "W" },[KEY_X] = { "x", "X" },
						[KEY_Y] = { "y", "Y" },[KEY_Z] = { "z", "Z" },

						[KEY_1] = { "1", "!" },[KEY_2] = { "2", "@" },[KEY_3] = { "3", "#" },[KEY_4] = { "4", "$" },
						[KEY_5] = { "5", "%" },[KEY_6] = { "6", "^" },[KEY_7] = { "7", "&" },[KEY_8] = { "8", "*" },
						[KEY_9] = { "9", "(" },[KEY_0] = { "0", ")" },

						[KEY_MINUS] = { "-", "_" },[KEY_EQUAL] = { "=", "+" },[KEY_COMMA] = { ",", "<" },
						[KEY_PERIOD] = { ".", ">" },[KEY_FORWARDSLASH] = { "/", "?" },[KEY_BACKSLASH] = { "\\", "|" },
						[KEY_SEMI_COLON] = { ";", ":" },[KEY_APOSTROPHE] = { "\"", "\"" },
						[KEY_SQUARE_BRACKET_LEFT] = { "[", "{" },[KEY_SQUARE_BRACKET_RIGHT] = { "]", "}" },
						[KEY_GRAVE] = { "`", "\0" },[KEY_SPACE] = { " ", " " },[KEY_ESC] = { "\0", "\0" }
					};
					*/

					//char *character = ctlMem->m_keyboard.key_mappings[key][isHoldingShift ? 1 : 0];
					char *character = ctlMem->m_keyboard.key_mappings[key][isHoldingShift];
					
					//if (character[0] != '\0') 
					if (character[0] && character[0] != '\0')  //ssafety anti-null test
					{
						for (int layout = 0; layout < 5; layout++) 
						{
							if (!useHexMode && layout == 3) 
								continue;
							for (int row = 0; row < rows; row++) 
							{
								for (int col = 0; col < cols; col++) 
								{
									const char *keyStr = ctlMem->m_keyboard.keys[layout][row][col];
									if (GET_LENGTH_OF_LITERAL_STRING(keyStr) != 1) 
										continue;

									if (character[0] == keyStr[0]) 
									{
										stradd(ctlMem->m_char_buffers.keyboard_input_buffer, character, MAX_KEYBOARD_INPUT_SIZE);
										selectedPage = layout;
										selectedRow = row;
										selectedCol = col;
										goto exit_loop;
									}
								}
							}
						}
					exit_loop:
						break;
					}
				}
			}
		}
	}

	ctlMem->m_keyboard.start_pressed_tick = (GetTickCount() + 1000); //caught in helper

	PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_UNREADY");

	RELEASE_TEXTURE(keyboardTxd);
	RELEASE_TEXTURE(selectedTxd);
	RELEASE_TEXTURE(doubleSelectedTxd);

	//uint invalid = 2211124811;
	//if (GET_HASH_KEY(input) == invalid) return "invalid";
	
	return input_length > 0;
}
#pragma endregion

int HexToIntNath(const char* hex)
{
	int result = 0;

	for (int i = 0; hex[i] != '\0'; i++)
	{
		result *= 16;

		if (hex[i] >= '0' && hex[i] <= '9')
			result += hex[i] - '0';
		else if (hex[i] >= 'a' && hex[i] <= 'f')
			result += hex[i] - 'a' + 10;
		else if (hex[i] >= 'A' && hex[i] <= 'F')
			result += hex[i] - 'A' + 10;
	}

	return result;
}

int get_chat_rules_acceptance() 
{
	TextureDict policeComputer = LOAD_TXD("policecomputer");
	Texture disclaimer = GET_TEXTURE(policeComputer, "kbentry");
	bool DrawFPS360UsersEtc_helper = bitTest(ctlMem->m_bit_sets.m_menuPreferences, PREF_BOTTOM_DRAW_FPS_PLAYERS_360USERS);
	bitReset(&ctlMem->m_bit_sets.m_menuPreferences, PREF_BOTTOM_DRAW_FPS_PLAYERS_360USERS);
	int value = 0;
	while (true) 
	{
		WAIT(0);

		AdminClientLoop();

		DRAW_SPRITE(disclaimer, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 255, 255, 255, 255);
		DRAW_RECT(0.52f, 0.6f, 0.83f, 0.5f, 242, 185, 30, 255);

		set_up_draw(true, 0.7f, 0.7f, 190, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.5f, 0.235f, "STRING", "~BLIP_60~   Cartel Chat Policies   ~BLIP_60~");

		set_up_draw(false, 0.5f, 0.5f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.11f, 0.37f, "STRING", "~BLIP_60~");
		set_up_draw(false, 0.35f, 0.35f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.15f, 0.37f, "STRING", "You are the host of the chat system and fully responsible for your actions within it.");

		set_up_draw(false, 0.5f, 0.5f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.11f, 0.5f, "STRING", "~BLIP_60~");
		set_up_draw(false, 0.35f, 0.35f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.15f, 0.5f, "STRING", "The same rules that apply to Xbox chat also apply here.");

		set_up_draw(false, 0.5f, 0.5f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.11f, 0.63f, "STRING", "~BLIP_60~");
		set_up_draw(false, 0.35f, 0.35f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.15f, 0.63f, "STRING", "If you encounter any illegal or inappropriate behavior");
		set_up_draw(false, 0.35f, 0.35f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.15f, 0.65f, "STRING", "such as harassment, hate speech, or discrimination,");
		set_up_draw(false, 0.35f, 0.35f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.15f, 0.67f, "STRING", "you are encouraged to mute or block the offending user.");

		set_up_draw(false, 0.5f, 0.5f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.11f, 0.78f, "STRING", "~BLIP_60~");
		set_up_draw(false, 0.35f, 0.35f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.15f, 0.78f, "STRING", "The creator of this mod is not liable for any actions");
		set_up_draw(false, 0.35f, 0.35f, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.15f, 0.80f, "STRING", "taken by users or harm resulting from the use of this mod.");

		set_up_draw(true, 0.5f, 0.5f, 190, 0, 0, 255);
		SET_TEXT_EDGE(1, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.35f, 0.94f, "STRING", "~PAD_Y~  =  Decline");
		set_up_draw(true, 0.5f, 0.5f, 190, 0, 0, 255);
		SET_TEXT_EDGE(1, 0, 0, 0, 255);
		DISPLAY_TEXT_WITH_LITERAL_STRING(0.65f, 0.94f, "STRING", "~PAD_X~  =  Accept");
		if (IS_BUTTON_JUST_PRESSED(0, BUTTON_X))
		{
			value = 1;
			break;
		}
		else if (IS_BUTTON_JUST_PRESSED(0, BUTTON_Y))
		{
			value = 2;
			break;
		}
	}

	RELEASE_TEXTURE(disclaimer); //untested

	if(DrawFPS360UsersEtc_helper)
		bitSet(&ctlMem->m_bit_sets.m_menuPreferences, PREF_BOTTOM_DRAW_FPS_PLAYERS_360USERS);

	return value;
}

void CreateByteSet(uint *ByteSet, uint b0, uint b1, uint b2, uint b3)
{
	*ByteSet = ((b3 & 0xFF) * 16777216) + ((b2 & 0xFF) * 65536) + ((b1 & 0xFF) * 256) + (b0 & 0xFF);
}

void GetByteSetH2(uint ByteSet, uint *b0, uint *b1, uint *b2, uint *b3)
{
	*b0 = ByteSet & 0xFF;
	*b1 = (ByteSet >> 8) & 0xFF;
	*b2 = (ByteSet >> 16) & 0xFF;
	*b3 = (ByteSet >> 24) & 0xFF;
}

/*
PED_INDEX GetPlayerPed()
{
	//return myPlayerPed;
	PED_INDEX myPed; 
	GET_PLAYER_CHAR(myPlayerIndex, &myPed); //Could replace myPlayerPed with a variable but ultimately this crashes us if users switch models with another menu.
	return myPed;
}
*/

VEHICLE_INDEX GetPlayerVeh() //GOING TO REMOVE //auto-boat is causing handle to change mid-loop and null-pointer crash, we might have to use this.
{
	VEHICLE_INDEX myVeh; //= INVALID_HANDLE;

	//if(IS_CHAR_IN_ANY_CAR(myPlayerPed))
	GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);

	return myVeh;
}

void DeleteChar(PED_INDEX *ped)
{
	NETWORK_INDEX NetworkId;
	GET_NETWORK_ID_FROM_PED(*ped, &NetworkId);
	ReleaseControlOfNetworkId(NetworkId);
	DELETE_CHAR(ped);
}

void DeleteCar(VEHICLE_INDEX *vehicle)
{
	DeleteObjectsOnVehicle(*vehicle);
	NETWORK_INDEX NetworkId;
	GET_NETWORK_ID_FROM_VEHICLE(*vehicle, &NetworkId);
	ReleaseControlOfNetworkId(NetworkId);
	DELETE_CAR(vehicle);
}

void DeleteObject(OBJECT_INDEX *object)
{
	NETWORK_INDEX NetworkId;
	GET_NETWORK_ID_FROM_OBJECT(*object, &NetworkId);
	ReleaseControlOfNetworkId(NetworkId);
	DELETE_OBJECT(object);
}

int indexOfAfterIndex(const char* str, char c, int startIndex, int size)
{
	for (int i = (startIndex + 1); i < size; i++)
	{
		if (str[i] == c)
			return i;
	}

	return -1;
}


/*
printf("~fuck~YO"); //doesn't crash displays YO
printf("~sup~sup"); //doesn't crash displays sup
printf("~~sup");  //doesn't crash dusplays sup
printf("~ ~test"); //doesn't crash dusplays test
printf("r~ ~rtest"); //doesn't crash displays rrtest
printf("~"); //crashes */
bool IsStrValid(const char* str)
{
	int len = GET_LENGTH_OF_LITERAL_STRING(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '~')
		{
			int index = indexOfAfterIndex(str, '~', i, len); //Find the next tilde after the current one
			if (index == -1)
				return false; //No matching closing '~' found, invalid string

			//Extract the string between the two tilde symbols
			int tagLength = index - i - 1;  // Length of the tag (excluding the '~'s)
			if (tagLength == 0) 
				return false; // Reject case like "~" with no tag between the '~'s
			
			i = index; //Move the index to the position after the valid tag
		}
	}
	return true; 
}

//extern Weapon myWeapons[8], myAmmo[8], currentWep;
/* Backup
void GiveWeaponsToPed(Ped ped, int Type, bool includeArmour)
{
	REMOVE_ALL_CHAR_WEAPONS(ped);

	if (includeArmour)
	{
		if (!HAS_CHAR_GOT_WEAPON(ped, WEAPON_ARMOUR))
			GIVE_WEAPON_TO_CHAR(ped, WEAPON_ARMOUR, 1, false);

		ADD_ARMOUR_TO_CHAR(ped, 999);
	}

	GIVE_WEAPON_TO_CHAR(ped, WEAPON_UNARMED, 1, false);
	if (Type == 0)
	{ //Advanced
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_GRENADE, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_RLAUNCHER, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_MP5, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_M4, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_BARETTA, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_SNIPERRIFLE, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_DEAGLE, AMMO_MAX, false);
		SET_CURRENT_CHAR_WEAPON(ped, WEAPON_M4, true);
	}
	if (Type == 1)
	{ //Poor
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_MOLOTOV, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_RLAUNCHER, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_MICRO_UZI, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_AK47, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_SHOTGUN, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_M40A1, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_PISTOL, AMMO_MAX, false);
		SET_CURRENT_CHAR_WEAPON(ped, WEAPON_AK47, true);
	}
	if (Type == 2)
	{ //Advanced TBoGT
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_EPISODIC_9, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_EPISODIC_10, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_EPISODIC_13, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_EPISODIC_14, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_EPISODIC_15, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_EPISODIC_1, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_EPISODIC_16, AMMO_MAX, false);
		GIVE_WEAPON_TO_CHAR(ped, WEAPON_KNIFE, 1, false);
		SET_CURRENT_CHAR_WEAPON(ped, WEAPON_EPISODIC_14, true);
	}
}
*/

void BlockYVehicleExitJustPressed() //only runs if we're in a car
{
	if (IS_CHAR_IN_ANY_CAR(myPlayerPed))
	{
		VEHICLE_INDEX myVeh;
		GET_CAR_CHAR_IS_USING(myPlayerPed, &myVeh);
		WARP_CHAR_FROM_CAR_TO_CAR(myPlayerPed, myVeh, GetPedSeatIndex(myPlayerPed));
	}
	//else, clear char tasks or something?
}

void GiveWeaponIfNotSpecified(PED_INDEX ped, Weapon giveWeapon, int ammo, bool forceInHand, Weapon specifiedWeapon)
{
	if (giveWeapon != specifiedWeapon)
		GIVE_WEAPON_TO_CHAR(ped, giveWeapon, ammo, forceInHand);
}

void GiveWeaponsToPed(PED_INDEX ped, CTL_WEAPON_TYPES type, bool armor)
{
	WEAPON_TYPE currentWeapon;

	//maybe check here to see if char has armour, if they do, give it back to them..
	GET_CURRENT_CHAR_WEAPON(ped, &currentWeapon);
	REMOVE_ALL_CHAR_WEAPONS(ped);

	//store their armour if not gonna give it? to restore..
	if (armor)
	{
		if (!HAS_CHAR_GOT_WEAPON(ped, WEAPON_ARMOUR))
			GIVE_WEAPON_TO_CHAR(ped, WEAPON_ARMOUR, 1, false);

		ADD_ARMOUR_TO_CHAR(ped, 999);
	}

	GiveWeaponIfNotSpecified(ped, WEAPON_UNARMED, 1, false, currentWeapon);

	if (type == WEAPON_TYPE_ADVANCED)
	{
		GiveWeaponIfNotSpecified(ped, WEAPON_GRENADE, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_RLAUNCHER, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_MP5, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_M4, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_BARETTA, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_SNIPERRIFLE, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_DEAGLE, AMMO_MAX, false, currentWeapon);
		//SET_CURRENT_CHAR_WEAPON(ped, WEAPON_M4, true);
	}

	if (type == WEAPON_TYPE_POOR)
	{
		GiveWeaponIfNotSpecified(ped, WEAPON_MOLOTOV, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_RLAUNCHER, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_MICRO_UZI, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_AK47, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_SHOTGUN, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_M40A1, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_PISTOL, AMMO_MAX, false, currentWeapon);
		//SET_CURRENT_CHAR_WEAPON(ped, WEAPON_AK47, true);
	}

	if (type == WEAPON_TYPE_TBOGT)
	{
		GiveWeaponIfNotSpecified(ped, WEAPON_EPISODIC_9, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_EPISODIC_10, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_EPISODIC_13, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_EPISODIC_14, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_EPISODIC_15, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_EPISODIC_1, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_EPISODIC_16, AMMO_MAX, false, currentWeapon);
		GiveWeaponIfNotSpecified(ped, WEAPON_KNIFE, 1, false, currentWeapon);
	}

	GIVE_WEAPON_TO_CHAR(ped, currentWeapon, AMMO_MAX, false);
}

CPlayerInfo* GetPlayerPointer(PLAYER_INDEX playerIndex)
{
	CPlayerInfo** playerArray = (CPlayerInfo**)0x82C01C70;
	return playerArray[playerIndex];
}

bool IsPlayerAMultiplayerModel(PLAYER_INDEX playerIndex)
{
	if (!IsPlayerValid(playerIndex))
		return true; //avoid false flags.

	PED_INDEX tmpPed;
	uint tmpModel;

	if (PLAYER_HAS_CHAR(playerIndex))
	{
		GET_PLAYER_CHAR(playerIndex, &tmpPed);
		GET_CHAR_MODEL(tmpPed, &tmpModel);
		return (tmpModel == PED_M_Y_MULTIPLAYER || tmpModel == PED_F_Y_MULTIPLAYER);
	}

	return false;
}

bool IsPlayerInvincible(PLAYER_INDEX playerIndex)
{
	if (!IsPlayerValid(playerIndex) || !PLAYER_HAS_CHAR(playerIndex))
		return false;

	if (IS_PLAYER_DEAD(playerIndex)) //players are invincible while dead (part of respawning process) - Nath
		return false;

	if (myCurrentEpisode == EPISODE_TBOGT) //not ideal but we will just have to skip if people are wearing parachute (not sure this will actually skip the entirety of the duration they have godmode - TEST REQUIRED: 20th January 2026) //seems to work
	{
		PED_INDEX playerPed;
		GET_PLAYER_CHAR(playerIndex, &playerPed);
		if (GET_CHAR_DRAWABLE_VARIATION(playerPed, PED_COMPONENT_SUS2) == 1) //parachute is worn
			return false;
	}
	return GetPlayerPointer(playerIndex)->m_playerPed->m_flags & 0xFF0000;
}

bool GetHostByte(void) //works
{
	return *(byte*)0x830FB0EB;
}

void SetHostByte(bool set) //works
{
	if (!NetworkGame) 
		return;

	*(byte*)0x830FB0EB = set;
}

void SetTellNetResponseEnabled(bool enable)
{
	if (!enable && CartelXEXPresent)
		return; //Message ID needs to come through so xex can detect it. (Detoured so we won't crash.).

//	int rand;
//	GENERATE_RANDOM_INT_IN_RANGE(65536, 9999999, &rand);
	if (enable)
		*(int*)0x8318EBC4 = 0x81;
	else
		*(int*)0x8318EBC4 = 65536; //max value for READING packet = 65535
}

void fix_tell_net_loop() //stops user receiving blackscreen events.
{
	//we wait for loading screen to fade currently, if that changes we will need additional checks here
	//or we risk disabling the TELL_NET response too early and being unable to join.
	SetTellNetResponseEnabled((GET_HOST_ID() == myPlayerIndex) || (NETWORK_LEAVE_GAME_PENDING() || DOES_GAME_CODE_WANT_TO_LEAVE_NETWORK_SESSION()));
}

/* //BACKUP!!
void TeleportNetworkPlayerOnFoot(const int playerIndex, float x, float y, float z, float h, bool checkTeleported)
{
	Player myPlayerID = myPlayerIndex;
	SetHostByte(true);		
	RESURRECT_NETWORK_PLAYER(playerIndex, x, y, (z - 1.0f), h); //z - 1.0 as it sets +1.0
	SetHostByte(GET_HOST_ID() == myPlayerID);

	if (playerIndex == myPlayerID || !checkTeleported) //we don't needa run detection on ourselves.
		return;
	//Safety checks..
	if (IsPlayerValid(playerIndex) && PLAYER_HAS_CHAR(playerIndex))
	{
		int tick = (GetTickCount() + 1100);
		Ped teleportPed;
		float curX, curY, curZ, dist;
		char* playersName = NetworkPlayerGetNameWithColour(playerIndex);
		while (tick > GetTickCount())
		{
			WAIT(0);
			GET_PLAYER_CHAR(playerIndex, &teleportPed);
			GET_CHAR_COORDINATES(teleportPed, &curX, &curY, &curZ);
			GET_DISTANCE_BETWEEN_COORDS_3D(curX, curY, curZ, x, y, z, &dist);
			if (dist < 15.0f)
			{
				float freshDist;
				WAIT(100);//they will rubber-band if using retail "Warp-back" protection.
				GET_CHAR_COORDINATES(teleportPed, &curX, &curY, &curZ);
				GET_DISTANCE_BETWEEN_COORDS_3D(curX, curY, curZ, x, y, z, &freshDist);
				if (freshDist < 15.0f)
					return; //Teleport Successful.
			}
		} //while loop
		printf("~r~CTL: ~w~Looks like %s ~w~has teleport protection.", playersName); //returns if successful so will only print if failed to teleport player.
	}
	
}
*/

void CreateVehicleWithPed(int model, float x, float y, float z)
{
	NETWORK_INDEX netId;
	PED_INDEX ped;

	if (!SpawnVehicle(model, x, y, z, 0.0f, &freeMem->m_map_mod.vehicle, false, true))
		return;

	REQUEST_MODEL(0x90442A67);
	SET_CAR_ENGINE_ON(freeMem->m_map_mod.vehicle, true, true);

	//SET_HELI_BLADES_FULL_SPEED(myMapVehicle); //creates un-needed lag, heli blades will be at full speed in 2-3 seconds.
	SET_CAR_PROOFS(freeMem->m_map_mod.vehicle, true, true, true, true, true);
	while (!HAS_MODEL_LOADED(0x90442A67))
		WAIT(0);

	CREATE_CHAR_INSIDE_CAR(freeMem->m_map_mod.vehicle, 25, 0x90442A67, &ped);
	MARK_MODEL_AS_NO_LONGER_NEEDED(0x90442A67);
	SET_CHAR_INVINCIBLE(ped, true);
	GET_NETWORK_ID_FROM_PED(ped, &netId);
	SET_NETWORK_ID_CAN_MIGRATE(netId, false);
}

void InteriorScenesScanLoop()
{
	float mX, mY, mZ;
	INTERIOR_INSTANCE_INDEX myInterior;

	/*if (IS_INTERIOR_SCENE())
{
	GET_GAME_VIEWPORT_ID(&uVar1);
	GET_KEY_FOR_VIEWPORT_IN_ROOM(uVar1, &uVar2);
	ADD_OBJECT_TO_INTERIOR_ROOM_BY_KEY(uVar0, uVar2);
}*/

	if (IS_INTERIOR_SCENE())
	{
		//uint key;
		//GET_KEY_FOR_CHAR_IN_ROOM(myPlayerPed, &key);
		//SET_ROOM_FOR_CHAR_BY_KEY(myPlayerPed, key);

		GET_CHAR_COORDINATES(myPlayerPed, &mX, &mY, &mZ);
		GET_INTERIOR_AT_COORDS(mX, mY, mZ, &myInterior);

		//extern _native void LOAD_SCENE(float x, float y, float z)l
		//extern _native void LOAD_SCENE_FOR_ROOM_BY_KEY(int interior, uint roomhash)l
		//extern _native void REQUEST_INTERIOR_MODELS(uint model, const char* interiorName)l
		//extern _native void ACTIVATE_INTERIOR(Interior interior, bool unknownTrue)l
			//extern _native void GET_INTERIOR_AT_COORDS(float x, float y, float z, Interior * pInterior)l
	}
}

bool SpawnVehicle(uint model, float x, float y, float z, float h, VEHICLE_INDEX *spawnedVehicle, bool netMigratable, bool onAllMachines)
{
	if (!IS_MODEL_IN_CDIMAGE(model))
	{
		printf("~r~CTL: ~w~Model %i is missing from your image files.", model);
		return false;
	}

	if (NetworkGame && Is_Vehicles_Type_Modified(model))
	{
		printf("~r~CTL: ~y~FREEZE CAR SPAWN BLOCKED ~w~Don't be a ~r~dick, ~w~lists exist for a reason.");
		return false;
	}

	REQUEST_MODEL(model);
	NETWORK_INDEX vehNetId;
	INTERIOR_INSTANCE_INDEX spawnInterior;
	GET_INTERIOR_AT_COORDS(x, y, z, &spawnInterior);
	while (!HAS_MODEL_LOADED(model)) 
		WAIT(0);

#ifdef Developer
	bool canRegister = CAN_REGISTER_MISSION_VEHICLE();
	PRINTSTRING("SpawnVehicle(), CAN_REGISTER_MISSION_VEHICLE() == ");
	PRINTINT(canRegister);
	PRINTNL();
	if (!canRegister)
		printf("~r~CTL DEV: ~w~CAN_REGISTER_MISSION_VEHICLE() == FALSE");
#endif
	//DELETE_MISSION

	CREATE_CAR(model, x, y, z, &*spawnedVehicle, onAllMachines);
	if (spawnInterior != 0)
	{
		SET_ROOM_FOR_CAR_BY_KEY(*spawnedVehicle, spawnInterior);
		//printf("spawnedVehicle interior == %i", spawnInterior);
	}

	MARK_MODEL_AS_NO_LONGER_NEEDED(model);
	SET_CAR_HEADING(*spawnedVehicle, h);
//	SET_CAR_AS_MISSION_CAR(*spawnedVehicle); //testing

	if (bitTest(ctlMem->m_bit_sets.m_vehicle, spawnVehiclesInvincible))
	{
		SET_CAR_CAN_BE_DAMAGED(*spawnedVehicle, false);
		SET_CAR_CAN_BE_VISIBLY_DAMAGED(*spawnedVehicle, false);
		if (!IS_THIS_MODEL_A_BOAT(model)) //auto-boat is fucked without this
		{
			SET_CAR_WATERTIGHT(*spawnedVehicle, true);
			SET_CAN_BURST_CAR_TYRES(*spawnedVehicle, false);
			SET_CAR_STRONG(*spawnedVehicle, true); //should help with doors coming off, stop car breaking alone doesn't work.
			STOP_CAR_BREAKING(*spawnedVehicle, true);
		}
		SET_CAR_PROOFS(*spawnedVehicle, true, true, true, true, true);
		SET_VEHICLE_DIRT_LEVEL(*spawnedVehicle, 0.0f);
		//IS_CAR_DOOR_DAMAGED
	//	SET_CAN_RESPRAY_CAR(*spawnedVehicle, true);

	}

	if (bitTest(ctlMem->m_bit_sets.m_vehicle, spawnedVehiclesRequireBreakIn) && !IS_THIS_MODEL_A_BOAT(model)) //doesn't work on boats properly.
		LOCK_CAR_DOORS(*spawnedVehicle, 7); //Locked car but player can break in.. (Cartel menu signature :P), make sure this is on by default.

	if (NetworkGame && onAllMachines)
	{
		GET_NETWORK_ID_FROM_VEHICLE(*spawnedVehicle, &vehNetId);
		SET_NETWORK_ID_CAN_MIGRATE(vehNetId, netMigratable);
	}

	//TESTING
	//MARK_CAR_AS_CONVOY_CAR() //lol wtf
	//MARK_CAR_AS_NO_LONGED_NEEDED(&*spawnedVehicle);

	return true;
}

int DeleteObjectsOnPed(PED_INDEX ped) //returns number of deleted objects
{
	OBJECT_INDEX attachedObj;
	int attachmentsDetected = 0;
	int attachmentsDeleted = 0;

	if (!DOES_CHAR_EXIST(ped))
		return 0;

	for (int z = 0; z < 3; z++) //request control and wait try again (aim is to make this take around one second)
	{
		for (int i = 0; i < 6000; i++)
		{
			if (!DOES_OBJECT_EXIST_WITH_NETWORK_ID(i))
				continue;

			GET_OBJECT_FROM_NETWORK_ID(i, &attachedObj);

			if (!IS_OBJECT_ATTACHED(attachedObj))
				continue; //might not need this not sure

			if (GET_PED_OBJECT_IS_ATTACHED_TO(attachedObj) != ped)
				continue; //might be fine with just this but aint gonna chance it.

			attachmentsDetected++; //yes above canMigrate because the entity may be able too soon.. (game is weird with its timings).

			//if (!CanObjectMigrate(attachedObj)) continue; //not tested for this case use..

			if (HAS_CONTROL_OF_NETWORK_ID(i))
			{
				DeleteObject(&attachedObj);
				attachmentsDeleted++;
			}
			else
				REQUEST_CONTROL_OF_NETWORK_ID(i);
		}

		if (attachmentsDetected == 0)
			return 0; //don't wait or keep trying if there's nothin attached to the ped..

		WAIT(200);
	}
	return attachmentsDeleted;
}

int DeleteObjectsOnVehicle(VEHICLE_INDEX vehicle) //returns number of deleted objects
{
	OBJECT_INDEX attachedObj;
	int attachmentsDetected = 0;
	int attachmentsDeleted = 0;

	if (!DOES_VEHICLE_EXIST(vehicle))
		return 0;

	for (int z = 0; z < 3; z++) //request control and wait try again (aim is to make this take around one second)
	{
		for (int i = 0; i < 6000; i++)
		{
			if (!DOES_OBJECT_EXIST_WITH_NETWORK_ID(i)) 
				continue;

			GET_OBJECT_FROM_NETWORK_ID(i, &attachedObj);

			if (!IS_OBJECT_ATTACHED(attachedObj)) 
				continue; //might not need this not sure

			if (GET_CAR_OBJECT_IS_ATTACHED_TO(attachedObj) != vehicle) 
				continue; //might be fine with just this but aint gonna chance it.

			attachmentsDetected++; //yes above canMigrate because the entity may be able too soon.. (game is weird with its timings).

			//if (!CanObjectMigrate(attachedObj)) continue; //not tested for this case use..

			if (HAS_CONTROL_OF_NETWORK_ID(i))
			{
				DeleteObject(&attachedObj);
				attachmentsDeleted++;
			}
			else
				REQUEST_CONTROL_OF_NETWORK_ID(i);
		}

		if (attachmentsDetected == 0)
			return 0; //don't wait or keep trying if there's nothin attached to the car..
		
		WAIT(200);
	}

	SET_CAR_VISIBLE(vehicle, true); //incase funny carz.. //Set visible locally.
	return attachmentsDeleted;
}

bool CloneVehicle(VEHICLE_INDEX targetVeh, float x, float y, float z, float h, VEHICLE_INDEX* clonedVehicle)
{
	uint targetVehModel; 
	int vehColours[4];
	float dirtLvl;
	
	GET_CAR_MODEL(targetVeh, &targetVehModel);

	if(!SpawnVehicle((int)targetVehModel, x, y, z, h, &*clonedVehicle, true, true)) 
		return false; //no need for checks as model is rendered therefor present in ISO

	if (!DOES_VEHICLE_EXIST(targetVeh))
	{
		printf("~r~CTL: ~w~Target vehicle for cloning no longer exists, bailing to avoid freeze.");
		return false; //should make this a bool and return false.
	}

	if (targetVeh == myObjectCar)
		printf("~r~CTL: ~w~Target vehicle is my funny vehicle, objects not cloned.");

	GET_CAR_COLOURS(targetVeh, &vehColours[0], &vehColours[1]);
	GET_EXTRA_CAR_COLOURS(targetVeh, &vehColours[2], &vehColours[3]);
	GET_VEHICLE_DIRT_LEVEL(targetVeh, &dirtLvl);

	for (int i = 0; i < 10; i++)
		TURN_OFF_VEHICLE_EXTRA(*clonedVehicle, i, !IS_VEHICLE_EXTRA_TURNED_ON(targetVeh, i));
	
	CHANGE_CAR_COLOUR(*clonedVehicle, vehColours[0], vehColours[1]);
	SET_EXTRA_CAR_COLOURS(*clonedVehicle, vehColours[2], vehColours[3]);
	SET_VEHICLE_DIRT_LEVEL(*clonedVehicle, dirtLvl);
	SWITCH_CAR_SIREN(*clonedVehicle, IS_CAR_SIREN_ON(targetVeh));

	for (int i = 0; i < 6; i++)
	{
		if (IS_CAR_TYRE_BURST(targetVeh, i))
			BURST_CAR_TYRE(*clonedVehicle, i);
	}

	for (int i = 0; i < 4; i++)
	{	//it looks like only 0 - 3 are valid (Ryan)
		if (!IS_VEH_WINDOW_INTACT(targetVeh, i)) 
			SMASH_CAR_WINDOW(*clonedVehicle, i);

		if (IS_CAR_DOOR_DAMAGED(targetVeh, i))
			BREAK_CAR_DOOR(*clonedVehicle, i, true);
	}


	if (IsVehicleScorched(targetVeh))
		SET_VEHICLE_RENDER_SCORCHED(*clonedVehicle, true);

	return true;
}

/*
bool SpaceInNetObjectPool(uint amount)
{
	POOL* NetObjPool = *(POOL**)0x8318E950; //Network object pool
	return ((NetObjPool->MaxHandle - NetObjPool->HandlesInPool) >= amount);
}

void CreateObject(int model, float x, float y, float z, float h, Object* object, bool lockNetControl)
{
	if (IS_MODEL_IN_CDIMAGE(model))
	{
		REQUEST_MODEL(model);
		while (!HAS_MODEL_LOADED(model)) WAIT(0);
		bool poolSpace = SpaceInNetObjectPool(1);
		CREATE_OBJECT_NO_OFFSET(model, x, y, z, &*object, poolSpace); 
		if (!poolSpace)
		{
			printf("NO POOL SPACE");
			//SET_OBJECT_ALPHA(*object, 150);
			SET_OBJECT_RENDER_SCORCHED(*object, true);
		}
		//SET_OBJECT_ALPHA
		while (!DOES_OBJECT_EXIST(*object)) WAIT(0);
		FREEZE_OBJECT_POSITION(*object, true);
		SET_OBJECT_HEADING(*object, h);
		SET_OBJECT_INVINCIBLE(*object, true);
		SET_OBJECT_HEALTH(*object, 165.0f); //stops detaching by my protection if local player has control.
		MARK_MODEL_AS_NO_LONGER_NEEDED(model);
		if (poolSpace && NetworkGame && lockNetControl)
		{
			int netId;
			GET_NETWORK_ID_FROM_OBJECT(*object, &netId);
			SET_NETWORK_ID_CAN_MIGRATE(netId, false);
		}
	}
	else
	{
		printf("~r~CTL: ~w~Model is missing from your image files.");
	}
}
*/

network_s* GetNetworkDataFromObject(uint pool, uint object)
{
	pool_s* pPool = (pool_s*)*(int*)pool;
	void* class = (void*)((pPool->size * (object >> 8)) + (int)pPool->pool);

	if (class == NULL)
		return nullptr;

	return (network_s*)*(int*)((int)class + 0x68);
}

bool CanNetworkIdMigrate(uint pool_address, uint object)
{
	network_s* data = GetNetworkDataFromObject(pool_address, object);

	if (data == nullptr)
		return false;

	return !(data->idFlags & 0x100);
}

bool IsVehicleScorched(VEHICLE_INDEX vehicle)
{
	pool_s* pPool = (pool_s*)*(int*)0x82FB0700;
	vehicle_s* class = (vehicle_s*)((pPool->size * (vehicle >> 8)) + (int)pPool->pool);

	if (class == NULL)
		return false;

	return class->renderFlags & 0x8000;
}

uint GetHandleFromPool(CPool* pool, uint Pointer_Address)
{
	uint index = (Pointer_Address - pool->m_firstPointer) / pool->m_structSize;
	return (index * 0x100) + *(byte*)(pool->m_bools + index);
}

void GetOffsetFromCamera(float offset, float* x, float* y, float* z, float* heading) //Buffeting's
{
	CAMERA_INDEX gameCamera;
	float camRotX, camRotY, camRotZ, camX, camY, camZ, camTangent;

	*x = *y = *z = 0.0;
	GET_GAME_CAM(&gameCamera);

	if (IS_CAM_ACTIVE(gameCamera))
	{
		GET_CAM_ROT(gameCamera, &camRotX, &camRotY, &camRotZ);
		*heading = camRotZ; //not present in original func
		GET_CAM_POS(gameCamera, &camX, &camY, &camZ);
		*x = camX - (SIN(camRotZ) * (offset * COS(camRotX)));
		*y = camY + (COS(camRotZ) * (offset * COS(camRotX)));
		*z = camZ + (SIN(camRotX) * offset);
	}
}

void SetCarColorForIndex(int index, Color* color)
{
	byte* colorData = (byte*)(0x83032078 + (index << 2));

	colorData[1] = color->r;
	colorData[2] = color->g;
	colorData[3] = color->b;
	colorData[0] = color->a;
}

Color GetCarColorForIndex(int index)
{
	byte* colorData = (byte*)(0x83032078 + (index << 2));
	return Color(colorData[1], colorData[2], colorData[3], colorData[0]);
}

/*
bool IsPlayerValid_old(PLAYER_INDEX player)
{
	return IsPlayerValid(player);
	int r, g, b;

	GET_PLAYER_RGB_COLOUR(player, &r, &g, &b);
	return ((((r | g | b) != 0x00) && !HAS_NETWORK_PLAYER_LEFT_GAME(player)) || (GET_HOST_ID() == player)); //try without host check?
}
*/

char* CWeaponInfoManager_GetInfo(WEAPON_TYPE weapon_index)
{
	if (weapon_index >= 60)
		return (char*)0x82CA8AB0;
	return (char*)((weapon_index * 0x110) + 0x82CB8AB0); //Xbox only.
}

void fix_freeze_guns_in_memory()
{
	char* rocket_pointer = CWeaponInfoManager_GetInfo(WEAPON_ROCKET); //this should fix freeze gun properly on retail.
	*(int*)(rocket_pointer + 0x20) |= 0xC005; //CAN_AIM - CAN_FREE_AIM - ANIM_RELOAD - ANIM_CROUCH_FIRE
	*(int*)(rocket_pointer + 0x28) = 0x1D; //grenade anim group

	if (myCurrentEpisode == EPISODE_TBOGT)
	{
		char* e2_rocket_pointer = CWeaponInfoManager_GetInfo(WEAPON_EPISODIC_18); //this should fix freeze gun properly on retail.
		*(int*)(e2_rocket_pointer + 0x20) |= 0xC005; //CAN_AIM - CAN_FREE_AIM - ANIM_RELOAD - ANIM_CROUCH_FIRE
		*(int*)(e2_rocket_pointer + 0x28) = 0x1D; //grenade anim group
	}
}

bool netObject_IsPendingOwnerChange(uint net_object)
{
	return getByte((void*)(net_object + 0x10)) != 0xFF;
}

uint CNetGamePlayer_GetLocalPlayer(uint net_interface)
{
	if ((getByte((void*)(net_interface + 0x1F40)) & 0x40) == 0)
		return NULL;

	return netObject_IsPendingOwnerChange(net_interface + 0x40) ? net_interface + 0x40 : NULL;
}

uint CNetGamePlayer_GetNetPlayer(uint net_interface, PLAYER_INDEX playerIndex)
{
	//if (playerIndex != -1 && (getByte((void*)(net_interface + 0x1F40)) & 0x40) != 0 && playerIndex >= 0 && playerIndex < 16) //we won't pass invalid indexes in sco
	if ((getByte((void*)(net_interface + 0x1F40)) & 0x40) != 0)
	{
		uint local_player = CNetGamePlayer_GetLocalPlayer(net_interface);
		if (local_player == NULL || getByte((void*)(local_player + 0x10)) != playerIndex)
			return *(uint*)(((playerIndex + 0x16B) << 2) + net_interface);
		return local_player;
	}

	return NULL;
}

/*
uint CNetGamePlayer_GetNetPlayer(uint net_interface, PLAYER_INDEX player)
{
	if (player != -1 && (getByte((void*)(net_interface + 0x1F40)) & 0x40) != 0 && player >= 0 && player < 16)
	{
		uint local_player = CNetGamePlayer_GetLocalPlayer(net_interface);
		if (local_player == NULL || getByte((void*)(local_player + 0x10)) != player)
			return *(uint*)(((player + 0x16B) << 2) + net_interface);
		return local_player;
	}

	return NULL;
}
*/
bool IsPlayerValid(PLAYER_INDEX playerIndex)
{
	if (playerIndex == myPlayerIndex)
		return true;

	uint net_player = CNetGamePlayer_GetNetPlayer(0x83109C88, playerIndex);
	return net_player && netObject_IsPendingOwnerChange(net_player);
}

bool safeDetachLocalPlayer() //Detaches from Walk-On-Air & Vehicles.
{
	if (bitTest(ctlMem->m_bit_sets.m_player, attachedToWalker))
	{
		//saves us having to store object or actively check we're attached to it.
		bitReset(&ctlMem->m_bit_sets.m_player, attachedToWalker);
		CLEAR_CHAR_TASKS_IMMEDIATELY(myPlayerPed);
		DETACH_PED(myPlayerPed, true);
		WAIT(100);
		return true;
	}
	else if (IS_PED_ATTACHED_TO_ANY_CAR(myPlayerPed))
	{
		DETACH_PED(myPlayerPed, true);
		//detachLocalPlayerFromVehicleOrWalker(); //no fucking idea why this crashes when called here, it doesn't elsewhere.
		WAIT(100);
		return true;
	}
	return false;
}