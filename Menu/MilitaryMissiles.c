#include "MilitaryMissiles.h"
#include "Utils.h"
//IS_PED_HOLDING_AN_OBJECT
//GIVE_PED_PICKUP_OBJECT(Ped ped, Object obj, bool flag)l
//HAS_OBJECT_BEEN_DAMAGED_BY_CAR(Object obj, Vehicle vehicle)l
//IS_VEHICLE_TOUCHING_OBJECT
//HAS_OBJECT_BEEN_DAMAGED(Object obj)l
//GET_OBJECT_PED_IS_HOLDING(Ped ped)l
//ANCHOR_OBJECT(Object obj, bool anchor, uint flags)l
//CONNECT_LODS(Object obj0, Object obj1)l
//ENABLE_DISABLED_ATTRACTORS_ON_OBJECT
//SET_OBJECT_PROOFS
//SET_OBJECT_PHYSICS_PARAMS

//#define MAX_MISSILES 3 //Functions.h
#define LOCK_TIME 2000
//typedef struct military_missile_vars Functions.h
//void setup_military_missile_vars() Functions.c
/*
int bullshit = 8;
int bullshit2 = 8;

float missile_burn_time = 20.0f;
float missile_climb_time = 5.0f;
float missile_proximity_fuse = 1.0f;
float missile_velocity = 5.0f;
bool missile_flame_trail = true;
bool missile_equip_with_cam = false;
float missile_cam_offset = 1.5f;
TextureDict txdPolice;
TextureDict spaceHud;

static Vehicle lastTarget = 0;
static int lockStartTime = 0;
static vector3 fired_pos;

char* WarheadTypeStrings[4] = { "Grenade", "Rocket", "Ship Destroy", "Truck" };
char* MissileChassisStrings[4] = { "CJ_BRIAZER", "CJ_ROCKET", "CJ_HIPPO_BIN" };
char* LinkModeStrings[3] = { "Off", "Stealth", "Kamikaze" };
EXPLOSION_TAG WarheadTypes[4] = { EXPLOSION_GRENADE, EXPLOSION_ROCKET, EXPLOSION_SHIP_DESTROY, EXPLOSION_TRUCK };
uint MissileChassisInts[3] = { OBJECT_CJ_BRIAZER, OBJECT_CJ_ROCKET, OBJECT_CJ_HIPPO_BIN };
int selectedMissileChassis = 0;
int selectedExplosionType = 0;
int selectedLinkMode = 1;
*/
/*
typedef struct
{
    Object missile;
    Vehicle target;
    MissileMode mode;
    int firedTime;
    vector3 helperVector;
    EXPLOSION_TAG missile_warhead_type;
    int burn_time;
    int climb_time;
    float proximity_fuse;
    float velocity;
    bool flame_trail;
    bool equiped_with_cam;
    float cam_offset;
    int link_mode;
} MilitaryMissile;

MilitaryMissile militaryMissile[MAX_MISSILES];
*/
static bool WorldToScreen(vector3 coords, vector2* out)
{
    int viewport;
    int screenx, screeny;
    float screenX, screenY;

    GET_GAME_VIEWPORT_ID(&viewport);
    GET_SCREEN_RESOLUTION(&screenx, &screeny);
    GET_VIEWPORT_POSITION_OF_COORD(coords.x, coords.y, coords.z, viewport, &screenX, &screenY);

    if (screenX < 0 || screenX > screenx || screenY < 0 || screenY > screeny)
        return false;

    out->x = screenX / (float)screenx;
    out->y = screenY / (float)screeny;

    return true;
}

void MissilePrefillTarget()
{
    Vehicle target;
    vector3 myPos;
    float targetDistance;
    int targetId;
    GET_CHAR_COORDINATES(myPlayerPed, &myPos.x, &myPos.y, &myPos.z);
    for (int i = 0; i < MAX_MISSILES; i++)
    {
        if (!DOES_OBJECT_EXIST(ctlMem->militaryMissile[i].missile)) continue;
        if (GET_OBJECT_PED_IS_HOLDING(myPlayerPed) != ctlMem->militaryMissile[i].missile) continue;
        for (int i = 0; i < 16; i++)
        {
            if (!IS_NETWORK_PLAYER_ACTIVE(i)) continue;
            Ped network_player;
            GET_PLAYER_CHAR(i, &network_player);
            if (!DOES_CHAR_EXIST(network_player)) continue;
            if (!IS_CHAR_IN_ANY_CAR(network_player)) continue;
            Vehicle car;
            GET_CAR_CHAR_IS_USING(network_player, &car);
            if (!DOES_VEHICLE_EXIST(car)) continue;
            vector3 pos;
            float distance;
            GET_CAR_COORDINATES(car, &pos.x, &pos.y, &pos.z);
            GET_DISTANCE_BETWEEN_COORDS_3D(pos.x, pos.y, pos.z, myPos.x, myPos.y, myPos.z, &distance);
            if (!DOES_VEHICLE_EXIST(target) || distance < targetDistance)
            {
                target = car;
                targetDistance = distance;
                targetId = i;
            }
        }
        if (DOES_VEHICLE_EXIST(target))
        {
            ctlMem->militaryMissile[i].target = target;
            printf("Missile´s target was set to: %s", GET_PLAYER_NAME(targetId));
            return;
        }
        else  print("~r~Could not find any possible targets", 1500);
    }
    print("~r~You do not carry a missile to prefill", 1500);
}

int GetMissileFlightTime(StormMilitaryMissile* m)
{
    int timeAlive;
    GET_GAME_TIMER(&timeAlive);
    return timeAlive - m->firedTime;
}

bool ShouldDetonateMissile(StormMilitaryMissile* m)
{
    if (!DOES_OBJECT_EXIST(m->missile) || GetMissileFlightTime(m) > m->burn_time || HAS_OBJECT_COLLIDED_WITH_ANYTHING(m->missile)
        || HAS_OBJECT_BEEN_DAMAGED(m->missile) || (m->mode == MISSILE_TV && IS_BUTTON_JUST_PRESSED(0, BUTTON_Y)))
            return true;

    if (DOES_VEHICLE_EXIST(m->target) && (m->mode == MISSILE_HEATSEEKER || m->mode == MISSILE_HOMING))
    {
        vector3 targetPos, missilePos;
        float distance;
        GET_CAR_COORDINATES(m->target, &targetPos.x, &targetPos.y, &targetPos.z);
        GET_OBJECT_COORDINATES(m->missile, &missilePos.x, &missilePos.y, &missilePos.z);
        GET_DISTANCE_BETWEEN_COORDS_3D(targetPos.x, targetPos.y, targetPos.z, missilePos.x, missilePos.y, missilePos.z, &distance);

        return distance < m->proximity_fuse;
    }
    return false;
}
   

int FindFreeMissileSlot()
{
    for (int i = 0; i < MAX_MISSILES; i++)
    {
        if (!DOES_OBJECT_EXIST(ctlMem->militaryMissile[i].missile))
            return i;
    }
    return -1;
}

void CreateAndGiveMilitaryMissile(MissileMode mode, int unk2)
{
    printf("Given missile with param %i", (int)mode);

    int slot = FindFreeMissileSlot();
    if (slot == -1)
    {
        print("Military Missile: ~r~maximum missiles in air.", 1200);
        return;
    }

    vector3 myPos;
    GET_CHAR_COORDINATES(myPlayerPed, &myPos.x, &myPos.y, &myPos.z);

    uint model = ctlMem->StormMissileVars.MissileChassisInts[ctlMem->StormMissileVars.selectedMissileChassis];

    REQUEST_MODEL(model);
    while (!HAS_MODEL_LOADED(model)) WAIT(0);
    CREATE_OBJECT(model, myPos.x, myPos.y, myPos.z, &ctlMem->militaryMissile[slot].missile, true);
    REQUEST_COLLISION_FOR_MODEL(model); //?? tf does this even do. Models are models not entities how does this even work - Nath
    MARK_MODEL_AS_NO_LONGER_NEEDED(model);
    if (!DOES_OBJECT_EXIST(ctlMem->militaryMissile[slot].missile)) return;
    int id;
    GET_NETWORK_ID_FROM_OBJECT(ctlMem->militaryMissile[slot].missile, &id);
    SET_NETWORK_ID_CAN_MIGRATE(id, false);

    ctlMem->militaryMissile[slot].mode = mode;
    ctlMem->militaryMissile[slot].firedTime = 0;
    ctlMem->militaryMissile[slot].helperVector = (vector3){0.0f, 0.0f, 0.0f};
    ctlMem->militaryMissile[slot].target = null;
    ctlMem->militaryMissile[slot].burn_time = (int)(ctlMem->StormMissileVars.missile_burn_time * 1000);
    ctlMem->militaryMissile[slot].climb_time = (int)(ctlMem->StormMissileVars.missile_climb_time * 1000);
    ctlMem->militaryMissile[slot].missile_warhead_type = ctlMem->StormMissileVars.WarheadTypes[ctlMem->StormMissileVars.selectedExplosionType];
    ctlMem->militaryMissile[slot].velocity = ctlMem->StormMissileVars.missile_velocity;
    ctlMem->militaryMissile[slot].proximity_fuse = ctlMem->StormMissileVars.missile_proximity_fuse;
    ctlMem->militaryMissile[slot].equiped_with_cam = IS_BIT_SET(ctlMem->StormMissileVars.bullshit, ctlMem->StormMissileVars.missile_equip_with_cam) || mode == MISSILE_TV;
    ctlMem->militaryMissile[slot].cam_offset = ctlMem->StormMissileVars.missile_cam_offset;
    ctlMem->militaryMissile[slot].flame_trail = true;//IS_BIT_SET(bullshit2, missile_flame_trail);
    ctlMem->militaryMissile[slot].link_mode = ctlMem->StormMissileVars.selectedLinkMode;


    ALLOW_PLAYER_TO_CARRY_NON_MISSION_OBJECTS(myPlayerIndex, true);
    SET_OBJECT_AS_STEALABLE(ctlMem->militaryMissile[slot].missile, true);
    SET_OBJECT_INVINCIBLE(ctlMem->militaryMissile[slot].missile, false);
    SET_OBJECT_DYNAMIC(ctlMem->militaryMissile[slot].missile, true);
    SET_OBJECT_COLLISION(ctlMem->militaryMissile[slot].missile, true);
    SET_OBJECT_RECORDS_COLLISIONS(ctlMem->militaryMissile[slot].missile, true);
    //SET_OBJECT_EXISTS_ON_ALL_MACHINES(ctlMem->militaryMissile[slot].missile, true); //no need for this, 
    SET_OBJECT_PROOFS(ctlMem->militaryMissile[slot].missile, false, true, false, false, false);
    SET_OBJECT_CCD(ctlMem->militaryMissile[slot].missile, true);
    MAKE_OBJECT_TARGETTABLE(ctlMem->militaryMissile[slot].missile, true);

    GIVE_PED_PICKUP_OBJECT(myPlayerPed, ctlMem->militaryMissile[slot].missile, true);
}

void SlideObjectInDirection(Object obj, vector3 direction, float speed, bool collision)
{
    if (!DOES_OBJECT_EXIST(obj)) return;

    vector3 pos;
    GET_OBJECT_COORDINATES(obj, &pos.x, &pos.y, &pos.z);

    vector3 vel = vector3_scale(vector3_normalize(direction), speed);
    vector3 target = vector3Add(pos, vel);

    APPLY_FORCE_TO_OBJECT(obj, 1, vel.x, vel.y, vel.z, 0, 0, 0, 1, false, true, true);
}

void LockOn(StormMilitaryMissile* m)
{
    Vehicle bestVeh = 0;
    float bestDist = 0.03f;

    for (int i = 0; i < 9999; i++)
    {
        if (!DOES_VEHICLE_EXIST_WITH_NETWORK_ID(i)) continue;

        Vehicle veh;
        GET_VEHICLE_FROM_NETWORK_ID(i, &veh);
        if (!DOES_VEHICLE_EXIST(veh)) continue;

        vector3 vehPos;
        GET_CAR_COORDINATES(veh, &vehPos.x, &vehPos.y, &vehPos.z);

        vector2 screen;
        if (!WorldToScreen(vehPos, &screen)) continue;

        float dx = screen.x - 0.5f;
        float dy = screen.y - 0.5f;
        float dist = SQRT(dx * dx + dy * dy);

        uint model;
        GET_CAR_MODEL(veh, &model);

        Ped driver;
        GET_DRIVER_OF_CAR(veh, &driver);
        bool hasPlayer = DOES_CHAR_EXIST(driver) && IS_THIS_PED_A_PLAYER(driver);

        if (IS_THIS_MODEL_A_HELI(model) || hasPlayer)
            dist *= 0.5f;

        if (dist < bestDist)
        {
            bestDist = dist;
            bestVeh = veh;
        }
    }

    int now;
    GET_GAME_TIMER(&now);

    if (DOES_VEHICLE_EXIST(bestVeh))
    {
        if (bestVeh != ctlMem->StormMissileVars.lastTarget || ctlMem->StormMissileVars.lockStartTime == 0)
        {
            PLAY_SOUND_FRONTEND(-1, "NAV_UP"); 
            ctlMem->StormMissileVars.lastTarget = bestVeh;
            ctlMem->StormMissileVars.lockStartTime = now;
        }

        int lockingSinceTime = now - ctlMem->StormMissileVars.lockStartTime;

        if (lockingSinceTime >= LOCK_TIME)
        {
            m->target = bestVeh;

            float ax, ay, az;
            GET_CAR_COORDINATES(bestVeh, &ax, &ay, &az);
            float offsets[] = { -3, -1, 1, 3 };
            for (int i = 0; i < 4; i++)
            {
                float dx = offsets[i];
                DRAW_CHECKPOINT(ax + dx, ay, az, 1.0f, 255, 0, 0);
                DRAW_CHECKPOINT(ax, ay + dx, az, 1.0f, 255, 0, 0);
                DRAW_CHECKPOINT(ax, ay, az + dx, 1.0f, 255, 0, 0);
                PLAY_SOUND_FRONTEND(-1, "FRONTEND_GAME_PHONE_DIAL_HANG_UP");
            }
        }
        else
        {
            
            float progress = (float)(now - ctlMem->StormMissileVars.lockStartTime) / (float)LOCK_TIME;
            int interval = (int)(200.0f * POW(1.0f - progress, 2));
            if ((now % interval) < 10) 
                PLAY_SOUND_FRONTEND(-1, "FRONTEND_GAME_PHONE_DIAL_HANG_UP");
          
            float ax, ay, az;
            GET_CAR_COORDINATES(bestVeh, &ax, &ay, &az);
            float offsets[] = { -3, -1, 1, 3 };
            for (int i = 0; i < 4; i++)
            {
                float dx = offsets[i];
                DRAW_CHECKPOINT(ax + dx, ay, az, 1.0f, 0, 0, 255);
                DRAW_CHECKPOINT(ax, ay + dx, az, 1.0f, 0, 0, 255);
                DRAW_CHECKPOINT(ax, ay, az + dx, 1.0f, 0, 0, 255);
            }
        }
    }
    else
    {
        ctlMem->StormMissileVars.lastTarget = 0;
        ctlMem->StormMissileVars.lockStartTime = 0;
    }
}

void PerformHoming(StormMilitaryMissile* m)
{
    if (!DOES_OBJECT_EXIST(m->missile) || !DOES_VEHICLE_EXIST(m->target))
        return;

    vector3 missilePos, missileVel;
    vector3 targetPos, targetVel;

    GET_OBJECT_COORDINATES(m->missile, &missilePos.x, &missilePos.y, &missilePos.z);
    GET_CAR_COORDINATES(m->target, &targetPos.x, &targetPos.y, &targetPos.z);

    DRAW_CHECKPOINT_WITH_ALPHA(targetPos.x, targetPos.y, targetPos.z, 2.0f, 255, 0, 0, 200);

    GET_OBJECT_VELOCITY(m->missile, &missileVel.x, &missileVel.y, &missileVel.z);
    GET_CAR_SPEED_VECTOR(m->target, &targetVel, true);
    vector3 toTarget = vector3Sub(targetPos, missilePos);
    float distance = vector3_length(toTarget);
    float missileSpeed = vector3_length(missileVel);

    if (missileSpeed < 1.0f) missileSpeed = 1.0f;

    float timeToIntercept = distance / missileSpeed;
    vector3 futureTargetPos = vector3Add(targetPos, vector3_scale(targetVel, timeToIntercept));
    vector3 los = vector3Sub(futureTargetPos, missilePos);
    vector3 losDir = vector3_normalize(los);

    if (vector3_length(m->helperVector) < 0.001f)
    {
        m->helperVector = losDir;
        return;
    }

    float frameTime;
    GET_FRAME_TIME(&frameTime);
    float dt = frameTime > 0.001f ? frameTime : 0.001f;

    vector3 deltaLOS = vector3Sub(losDir, m->helperVector);
    vector3 losRate = vector3_scale(deltaLOS, 1.0f / dt);

    vector3 missileDir = vector3_normalize(missileVel);
    if (vector3_length(missileDir) < 0.001f)
        missileDir = m->helperVector;

    vector3 accelCmd = vector3_cross(losRate, missileDir);

    float navConstMin = 1.5f;
    float navConstMax = 4.0f;
    float distMin = 25.0f;
    float distMax = 300.0f;

    float t = clamp((distance - distMin) / (distMax - distMin), 0.0f, 1.0f);
    float nav_dynamic = lerp(navConstMax, navConstMin, t); 

    vector3 steering = vector3_scale(accelCmd, nav_dynamic);
    vector3 desiredDirection = vector3_normalize(vector3Add(losDir, steering));

    SlideObjectInDirection(m->missile, desiredDirection, m->velocity, true);

    m->helperVector = losDir;
}

void PerformHeatSeeker(StormMilitaryMissile* m)
{
    vector3 missilePos, targetPos;
    GET_OBJECT_COORDINATES(m->missile, &missilePos.x, &missilePos.y, &missilePos.z);
    GET_CAR_COORDINATES(m->target, &targetPos.x, &targetPos.y, &targetPos.z);

    vector3 toTarget = vector3Sub(targetPos, missilePos);
    vector3 direction = vector3_normalize(toTarget);
    SlideObjectInDirection(m->missile, direction, m->velocity, true);
}

void PerformTV(StormMilitaryMissile* m)
{
    vector3 missilePos;
    GET_OBJECT_COORDINATES(m->missile, &missilePos.x, &missilePos.y, &missilePos.z);

    vector3 direction = m->helperVector;

    if (vector3_length(direction) < 0.001f)
    {
        vector3 missileVel;
        GET_OBJECT_VELOCITY(m->missile, &missileVel.x, &missileVel.y, &missileVel.z);
        direction = vector3_normalize(missileVel);

        if (vector3_length(direction) < 0.001f)
            direction = (vector3){ 0.0f, 1.0f, 0.0f };
    }

    int pos[4];
    GET_POSITION_OF_ANALOGUE_STICKS(0, &pos[0], &pos[1], &pos[2], &pos[3]);

    float inputX = pos[0];
    float inputY = pos[1];

    float deadzone = 0.1f;
    if (ABSF(inputX) < deadzone) inputX = 0.0f;
    if (ABSF(inputY) < deadzone) inputY = 0.0f;

    float sensitivity = 0.05f;

    vector3 inputRotation = { -inputY * sensitivity, -inputX * sensitivity, 0.0f };

    vector3 targetDirection = rotate_vector_by_input(direction, inputRotation);
    targetDirection = vector3_normalize(targetDirection);

    float steeringStrength = 0.5f; 
    direction = vector3Add(
        vector3_scale(direction, 1.0f - steeringStrength),
        vector3_scale(targetDirection, steeringStrength)
    );
    direction = vector3_normalize(direction);

    float speed = m->velocity;
    if (IS_BUTTON_PRESSED(0, BUTTON_LT))
    {
        speed /= 2;
    }
    else if (IS_BUTTON_PRESSED(0, BUTTON_RT))
    {
        speed *= 2;
    }
    SlideObjectInDirection(m->missile, direction, speed, true);

    m->helperVector = direction;
}

void PerformBomb(StormMilitaryMissile* m)
{

}

void MissileIntelligence(StormMilitaryMissile* m)
{
    if (IS_CHAR_ON_FIRE(myPlayerPed))
    {
        EXTINGUISH_CHAR_FIRE(myPlayerPed);
    }

    // on hand
    if (GET_OBJECT_PED_IS_HOLDING(myPlayerPed) == m->missile && IS_BUTTON_PRESSED(0, BUTTON_LT))
    {
        LockOn(m);
        print("lockon active", 100);
        return;
    }

    // just fired
    if (GET_OBJECT_PED_IS_HOLDING(myPlayerPed) != m->missile && m->firedTime == 0)
    {
        //txdPolice = LOAD_TXD("policecomputer");
        //spaceHud = LOAD_TXD("space_hud");
        if (!DOES_VEHICLE_EXIST(m->target) && (m->mode == MISSILE_HEATSEEKER || m->mode == MISSILE_HOMING))
        {
            m->mode = MISSILE_BOMB;
        }
        STOP_SOUND(-1);
        RELEASE_SOUND_ID(-1);
        START_PTFX_ON_OBJ("weap_rocket_player", m->missile, 0, 0, 0, 0, 3, 3, 1);
        GET_CHAR_COORDINATES(myPlayerPed, &ctlMem->StormMissileVars.fired_pos.x, &ctlMem->StormMissileVars.fired_pos.y, &ctlMem->StormMissileVars.fired_pos.z);
        // PLAY_SOUND_FROM_OBJECT(-1, "GENERAL_WEAPONS_ROCKET_THRUST", m->missile);
        GET_GAME_TIMER(&m->firedTime);
        if (m->equiped_with_cam || m->mode == MISSILE_TV)
        {
            CREATE_CAM(14, &scriptedCam);
            SET_CAM_ACTIVE(scriptedCam, true);
            SET_CAM_PROPAGATE(scriptedCam, true);
            ACTIVATE_SCRIPTED_CAMS(true, true);
        }
        if (m->link_mode > 0)
        {
            ATTACH_PED_TO_OBJECT(myPlayerPed, m->missile, BONE_ROOT, 0.0f, 0.0f, 1.5f, 0.0f, -180.0f, false, false);
            SET_PLAYER_CONTROL(myPlayerIndex, m->link_mode != 1);
            SET_CHAR_INVINCIBLE(myPlayerPed, m->link_mode != 2);
        }
    }

    // in air
    if (m->firedTime != 0)
    {
        vector3 missilePos, missileVel, myPos;
        int timeAlive = GetMissileFlightTime(m);
        GET_OBJECT_COORDINATES(m->missile, &missilePos.x, &missilePos.y, &missilePos.z);
        GET_OBJECT_VELOCITY(m->missile, &missileVel.x, &missileVel.y, &missileVel.z);
        //REQUEST_COLLISION_AT_POSN(missilePos.x, missilePos.y, missilePos.z);
        if (m->equiped_with_cam)
        {
            vector3 dir = vector3_normalize(missileVel);
            vector3 camOffset = vector3_scale(dir, m->cam_offset);
            vector3 camPos = vector3Add(missilePos, camOffset);
            SET_CAM_POS(scriptedCam, camPos.x, camPos.y, camPos.z);
            vector3 lookAt = vector3Add(missilePos, vector3_scale(dir, 12.0f));
            POINT_CAM_AT_COORD(scriptedCam, lookAt.x, lookAt.y, lookAt.z);
        }

        if (m->flame_trail && timeAlive % 1 == 0)
        {
            ADD_EXPLOSION(missilePos.x, missilePos.y, missilePos.z, EXPLOSION_MOLOTOV, 1.0f, true, false, 1.0f);
        }

        if (ShouldDetonateMissile(m))
        {
            ADD_EXPLOSION(missilePos.x, missilePos.y, missilePos.z, m->missile_warhead_type, 1.0f, true, false, 0.0f);
            if (DOES_CAM_EXIST(scriptedCam))
            {
                SET_CAM_ACTIVE(scriptedCam, false);
                SET_CAM_PROPAGATE(scriptedCam, false);
                ACTIVATE_SCRIPTED_CAMS(false, false);
                UNATTACH_CAM(scriptedCam);
                DESTROY_CAM(scriptedCam);
                while (DOES_CAM_EXIST(scriptedCam))
                    WAIT(0);
                scriptedCam = INVALID_HANDLE;
            }
            DELETE_OBJECT(&m->missile);
            m->missile = 0;
            if (m->link_mode == 1)
            {
                SET_CHAR_COORDINATES(myPlayerPed, ctlMem->StormMissileVars.fired_pos.x, ctlMem->StormMissileVars.fired_pos.y, ctlMem->StormMissileVars.fired_pos.z);
                SET_CHAR_INVINCIBLE(myPlayerPed, false);
            }
            DETACH_PED(myPlayerPed, false);
            SET_PLAYER_CONTROL(myPlayerIndex, true);
            return;
        } 

        if (GetMissileFlightTime(m) < m->climb_time)
        {
            if (missilePos.z > 1700.0f)
            {
                m->climb_time = 0;
                printf("Missile: ~y~Maximum height reached.");
                return;
            }
            SlideObjectInDirection(m->missile, (vector3) { 0.0f, 0.0f, 1.0f }, m->velocity, false);
            return;
        }

        switch (m->mode)
        {
            case MISSILE_HEATSEEKER:
                PerformHeatSeeker(m);
                break;
            case MISSILE_HOMING:
                PerformHoming(m);
                break;
            case MISSILE_TV:
                PerformTV(m);
                break;
            case MISSILE_BOMB:
                PerformBomb(m);
                break;
        }
    }
}


void MissileLoop()
{
    for (int i = 0; i < MAX_MISSILES; i++)
    {
        if (!DOES_OBJECT_EXIST(ctlMem->militaryMissile[i].missile)) continue;
        MissileIntelligence(&ctlMem->militaryMissile[i]);
    }
}