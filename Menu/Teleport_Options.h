#pragma once

void TeleportOptions();
bool warp_local_player_into_vehicle(VEHICLE_INDEX target_veh);
void TeleportLocalPlayerOnFoot(float x, float y, float z, float h);
bool TeleportLocalPlayerInVehicle(float x, float y, float z, float h);
bool TeleportNetworkPlayerOnFoot(const PLAYER_INDEX playerIndex, float x, float y, float z, float h, bool checkTeleported);
bool TeleportNetworkPlayerToCoordinate(PLAYER_INDEX playerIndex, float x, float y, float z, float h);

void TeleportNetworkPlayerToMe(PLAYER_INDEX player, int unused);
void TeleportToNetworkPlayer(PLAYER_INDEX playerIndex, int unused); //moved variables to top to look cleaner and renamed some variables so they make more sense
void TeleportIntoNetworkPlayersVehicle(PLAYER_INDEX playerIndex, int unused); //needs optimising but works for now.
