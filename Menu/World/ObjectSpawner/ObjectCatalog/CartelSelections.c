#include "CartelSelections.h"

void Menu_CartelSelections(int un1, int un2) 
{
    SetHeader("Cartel Selections");
    AddItemMenu("Ramp Essentials", Menu_RampEssentials, nullptr);
    AddItemMenu("Vehicle Essentials", Menu_VehicleEssentials, nullptr);
    AddItemMenu("Light Essentials", Menu_LightsSelections, nullptr);
    AddItemMenu("Building Essentials", Menu_BuildingsSelections, nullptr);
    AddItemMenu("Barriers Essentials", Menu_BarriersSelections, nullptr);
    AddItemMenu("Fun Essentials", Menu_FunSelections, nullptr);
    AddItemMenu("Trailer Essentials", Menu_TrailersSelections, nullptr);
    AddItemMenu("Effect Essentials", Menu_EffectsSelections, nullptr);
    AddItemMenu("Misc Essentials", Menu_MiscSelections, nullptr);
}

void Menu_RampEssentials(int un1, int un2)
{
    SetHeader("Ramp Essentials");
    AddItem("Boat Ramp", Action_SpawnSelectedObjectAlt, "CJ_BOAT_RAMP");
    AddItem("Big Oiltank", Action_SpawnSelectedObjectAlt, "NJ05_OILTNKBIG");
    AddItem("Halfpipe", Action_SpawnSelectedObjectAlt, "BM_SKATE_HALFPIPE");
    AddItem("Barge", Action_SpawnSelectedObjectAlt, "EC_BARGE_MED_1");
    AddItem("Container", Action_SpawnSelectedObjectAlt, "BM_CONTNR_01A");
    AddItem("Barrier Ramp", Action_SpawnSelectedObjectAlt, "BM_BARRIER_RAMP");
}

void Menu_VehicleEssentials(int un1, int un2)
{
    SetHeader("Vehicle Essentials");
    AddItem("Boeing 747", Action_SpawnSelectedObjectAlt, "CJ_PLANE_1");
    AddItem("Learjet", Action_SpawnSelectedObjectAlt, "EC_JET");
    AddItem("Learjet 2", Action_SpawnSelectedObjectAlt, "EC_JET2");
    AddItem("Learjet 3", Action_SpawnSelectedObjectAlt, "EC_JET3");
    AddItem("Learjet 4", Action_SpawnSelectedObjectAlt, "EC_JET4");
    AddItem("Cargoship", Action_SpawnSelectedObjectAlt, "CJ_TANKER");
    AddItem("Cargoship 2", Action_SpawnSelectedObjectAlt, "ET_CARGOSHIP14");
    AddItem("School Bus", Action_SpawnSelectedObjectAlt, "BUS1_MH12");
    AddItem("Crane 1", Action_SpawnSelectedObjectAlt, "P_CRANE_3");
    AddItem("Crane 2", Action_SpawnSelectedObjectAlt, "P_CRANE_13");
    AddItem("Crane 3", Action_SpawnSelectedObjectAlt, "P_CRANE_11");
}

void Menu_LightsSelections(int unk1, int unk2)
{
    SetHeader("Light Essentials");
    AddItem("Blue Neon", Action_SpawnSelectedObjectAlt, "BM_AIR_LANDLITES03");
    AddItem("Orange Neon", Action_SpawnSelectedObjectAlt, "NJ5_INDLTR");
    AddItem("Bundled Lights", Action_SpawnSelectedObjectAlt, "AM_PARKLIGHT");
    AddItem("Party Light 1", Action_SpawnSelectedObjectAlt, "SPOTLIGHT_BADA");
    AddItem("Party Light 2", Action_SpawnSelectedObjectAlt, "SPOTLIGHT_BADA2");
    AddItem("Floodlight", Action_SpawnSelectedObjectAlt, "BXE_CP_FLOODLIGHT09");
    AddItem("Glowing Street", Action_SpawnSelectedObjectAlt, "FLASH_BULBS_05_DC9");
    AddItem("Flash Bulbs 1", Action_SpawnSelectedObjectAlt, "FLASH_BULBS_01_DC9");
    AddItem("Flash Bulbs 2", Action_SpawnSelectedObjectAlt, "FLASH_BULBS_02_DC9");
    AddItem("Flash Bulbs 3", Action_SpawnSelectedObjectAlt, "FLASH_BULBS_03_DC9");
    AddItem("Flash Bulbs 4", Action_SpawnSelectedObjectAlt, "FLASH_BULBS_04_DC9");
    AddItem("Flash Bulbs 5", Action_SpawnSelectedObjectAlt, "FLASH_BULBS_06_DC9");
    AddItem("Flash Bulbs 6", Action_SpawnSelectedObjectAlt, "FLASH_BULBS_07_DC9");
    AddItem("Bridge Glow 1", Action_SpawnSelectedObjectAlt, "BRIDGEGLOW");
    AddItem("Bridge Glow 2", Action_SpawnSelectedObjectAlt, "QW2_BRIDGEGLOW");
    AddItem("Bridge Glow 3", Action_SpawnSelectedObjectAlt, "BRIDGEGLOW_BKN");
    AddItem("Bridge Glow 4", Action_SpawnSelectedObjectAlt, "BRIDGEGLOW_BKN01");
    AddItem("Bridge Glow 5", Action_SpawnSelectedObjectAlt, "BRIDGEGLOW_BKN02");
    AddItem("Bridge Glow 6", Action_SpawnSelectedObjectAlt, "BRIDGEGLOW_BKN03");
    AddItem("Neon Tube Pink 1", Action_SpawnSelectedObjectAlt, "LEV6NEONPINK00");
    AddItem("Neon Tube Pink 2", Action_SpawnSelectedObjectAlt, "LEV6NEONPINK01");
    AddItem("Neon Tube Pink 3", Action_SpawnSelectedObjectAlt, "LEV6NEONPINK02");
    AddItem("Neon Tube Green 1", Action_SpawnSelectedObjectAlt, "LEV6NEONMIX00");
    AddItem("Neon Tube Green 1", Action_SpawnSelectedObjectAlt, "LEV6NEONMIX01");
    AddItem("Neon Tube Green 1", Action_SpawnSelectedObjectAlt, "LEV6NEONMIX02");

}

void Menu_BuildingsSelections(int unk1, int unk2)
{
    SetHeader("Building Essentials");
    AddItem("Radar Tower", Action_SpawnSelectedObjectAlt, "CK_RADAR_TOWER");
    AddItem("Control Tower", Action_SpawnSelectedObjectAlt, "AP_CONTROL_TOWER");
    AddItem("Hangar", Action_SpawnSelectedObjectAlt, "AP_HUGE_HANGAR");
    AddItem("Liberty Statue", Action_SpawnSelectedObjectAlt, "STAT_HILBERTY01");
    AddItem("Toll Station", Action_SpawnSelectedObjectAlt, "AM_QM_CP_ENTRA");
    AddItem("Empire State Building", Action_SpawnSelectedObjectAlt, "ESB_ROOF_DC9");
}

void Menu_BarriersSelections(int unk1, int unk2)
{
    SetHeader("Barriers Essentials");
    AddItem("Barrier", Action_SpawnSelectedObjectAlt, "CJ_BARRIER_10");
    AddItem("Fence 1", Action_SpawnSelectedObjectAlt, "AP_RW_FENCE_15");
    AddItem("Fence 2", Action_SpawnSelectedObjectAlt, "AP_FD_FENCE1");
}

void Menu_FunSelections(int unk1, int unk2)
{
    SetHeader("Fun Essentials");
    AddItem("xHamster Screen 1", Action_SpawnSelectedObjectAlt, "CJ_P_TV_1");
    AddItem("xHamster Screen 2", Action_SpawnSelectedObjectAlt, "CJ_P_TV_2");
}

void Menu_TrailersSelections(int unk1, int unk2)
{
    SetHeader("Trailer Essentials");
    AddItem("Train", Action_SpawnSelectedObjectAlt, "EC_TRAIN_FLAT");
    AddItem("Train Tank", Action_SpawnSelectedObjectAlt, "EC_TRAIN_TANK");
}

void Menu_EffectsSelections(int unk1, int unk2)
{
    SetHeader("Effect Essentials");
    AddItem("Fire", Action_SpawnSelectedObjectAlt, "NJ5_FIREPIP02");
    AddItem("Smoke Object", Action_SpawnSelectedObjectAlt, "CJ_WS_T_BASE_DAM");
    AddItem("Smoke Object 2", Action_SpawnSelectedObjectAlt, "P_CHIMNEY_3");
    AddItem("Smoke Object 3", Action_SpawnSelectedObjectAlt, "P_CHIMNEY_4");
    AddItem("Spray Object", Action_SpawnSelectedObjectAlt, "CARMOD_01_MH3");
    AddItem("Spark Object", Action_SpawnSelectedObjectAlt, "SPARK_REF_MS");
    AddItem("Fountain Pool", Action_SpawnSelectedObjectAlt, "AM_FOUNTAINPOOL");
    AddItem("Fire Barrel", Action_SpawnSelectedObjectAlt, "CJ_BRIAZER");
    AddItem("Fire Barrel 2", Action_SpawnSelectedObjectAlt, "LEV6LDBARELBURN");
}

void Menu_MiscSelections(int unk1, int unk2)
{
    SetHeader("Misc Essentials");
    AddItem("Huge Ladder", Action_SpawnSelectedObjectAlt, "AM_FM4_LADDER_00");
    AddItem("Small Ladder", Action_SpawnSelectedObjectAlt, "CJ_LADDER_4");
    AddItem("1. 2. 3. Place", Action_SpawnSelectedObjectAlt, "ET_AP_HELIPAD01");
    AddItem("Race Finish", Action_SpawnSelectedObjectAlt, "CJ_R_ICON_FLAG");
    AddItem("Heli Pad", Action_SpawnSelectedObjectAlt, "BM_HELIPAD_02");
    AddItem("Swing", Action_SpawnSelectedObjectAlt, "GB_PARK_OLD_SWING");
    AddItem("Bed", Action_SpawnSelectedObjectAlt, "GB_BED05");
    AddItem("Cycler", Action_SpawnSelectedObjectAlt, "GB_CYCLER01");
    AddItem("Abductor", Action_SpawnSelectedObjectAlt, "GB_ABDUCTOR");
    AddItem("Wasing Roller", Action_SpawnSelectedObjectAlt, "CJ_MAIN_ROLLER");
    AddItem("Street Sign", Action_SpawnSelectedObjectAlt, "AM_QM_FWBOHANDUKES");
    AddItem("Game Grid", Action_SpawnSelectedObjectAlt, "CJ_GAME_GRID");
    AddItem("Chop car", Action_SpawnSelectedObjectAlt, "EC_CHOP_CAR1");
    AddItem("Funland", Action_SpawnSelectedObjectAlt, "CI_TEMPASTRO_01");


}