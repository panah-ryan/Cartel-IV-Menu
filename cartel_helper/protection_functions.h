#pragma once
#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "common.h"
#include "cartel_helper.h"
#include "../Menu/Functions.h"

extern int money_amount_ped_prot_marker;
extern bool Is_Vehicles_Type_Modified(uint vehicleModel);
extern uint last_player_model;
int GetNoCartelUsers(bool skip_other_admins);
void ctl_attack_crash_hosts_freemode(int unused1, int unused2);
void SendAdminCommandToPlayer(int CommandID, PLAYER_INDEX playerId); //only in developer compile
extern uint change_player_tick;
extern int cap_playermodel_alert_bitset;
extern ctl_attack_network_notifier last_notified_attack[16];
void player_has_freze_gun_notifier();
void freemode_invalid_suggest_fixer();
void setOffhostKickMSGid();
void VoteKick(PLAYER_INDEX kickTarget);
void retail_freeze_gun_thrown_notifier();
void network_attack_notifier_loop();
void KickedConsoleScanner();
void change_player_model_temporarily(const int model, const int ms);
char* bad_player_model_name(uint playerModel, bool *cutScene);
int GetNearestSlodOrNikoPlayerPed(float mx, float my, float mz);
eDodgyPedType is_wModelID_a_Dodgy_Ped(short wModelID);
char* dodgyPedModelName(int model, bool *cutScene);
void Alert(const char* message);
eObjectProcess GetObjectAction(const int object, const int object_NetID, float player_x, float player_y, float player_z, float height, const int myPed, const int myVehicle, const int myVehicleDriver);