#include "ObjectSpawnerUI.h"
#include "..\..\ESP.h"

static void InitText() 
{
    SET_TEXT_FONT(0);
    SET_TEXT_SCALE(0.3f, 0.3f);
    SET_TEXT_COLOUR(255, 255, 255, 255);
    SET_TEXT_WRAP(0.0f, 1.0f);
    SET_TEXT_CENTRE(0);
    SET_TEXT_BACKGROUND(0);
    SET_TEXT_PROPORTIONAL(1);
}

static void DrawTextOnScreen(float posX, float posY, char* text) {
    
    InitText();
    DISPLAY_TEXT_WITH_LITERAL_STRING(posX, posY, "STRING", text);
}

void DrawControls(bool isAlternativeMode)
{
    float baseX = 0.03f;
    float baseY = 0.55f;
    float lineHeight = 0.025f;
    float bgWidth = 0.28f;
    float bgHeight = 0.27f;
    float y = baseY;
    float paddingX = 0.01f;

    DRAW_RECT(baseX + bgWidth / 2, baseY + bgHeight / 2, bgWidth, bgHeight, 0, 0, 0, 150);

    y += lineHeight / 2;
    DrawTextOnScreen(baseX + paddingX / 2, y, "~p~Controls Overview");
    y += lineHeight * 1.2;

    if (!isAlternativeMode)
    {
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_BACK~ ~w~Toggle alternative controlls");
        y += lineHeight;
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_LSTICK_ALL~ ~w~Move Object");
        y += lineHeight;
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_RSTICK_ALL~ ~w~Rotate Camera");
        y += lineHeight;
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_LB~/~PAD_RB~ ~w~Rotate Yaw");
        y += lineHeight;
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_DPAD_ALL~ ~w~Rotate Pitch / Roll");
        y += lineHeight;
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_X~ ~w~Speed Mode");
        y += lineHeight;
    }
    else
    {
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_BACK~ ~w~Toggle normal controlls");
        y += lineHeight;
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_RSTICK_UPDOWN~ ~w~Zoom");
        y += lineHeight;
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_DPAD_LEFTRIGHT~ ~w~Previous/Next Object");
        y += lineHeight;
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_LSTICK_NONE~ ~w~Attach to Player");
        y += lineHeight;
        DrawTextOnScreen(baseX + paddingX, y, "~PAD_RSTICK_NONE~ ~w~Snap to Ground");
        y += lineHeight;
    }

    DrawTextOnScreen(baseX + paddingX, y, "~PAD_A~ ~w~Clone");
    y += lineHeight;
    DrawTextOnScreen(baseX + paddingX, y, "~PAD_Y~ ~w~Delete");
    y += lineHeight;
    DrawTextOnScreen(baseX + paddingX, y, "~PAD_B~ ~w~Exit");
    y += lineHeight;
}

void DrawObjectInformation(SpawnerObject *object)
{
    if (!DOES_OBJECT_EXIST(object->o))
        return;

    char buffer[128];
    vector3 pos, rot;
    quaternion q;
    GET_OBJECT_COORDINATES(object->o, &pos.x, &pos.y, &pos.z);
    GET_OBJECT_QUATERNION(object->o, &q.x, &q.y, &q.z, &q.w);
    rot = quaternion_to_euler(q);
    uint model;
    GET_OBJECT_MODEL(object->o, &model);

    int current_time;
    GET_GAME_TIMER(&current_time);
    int timeDiff = current_time - object->creationTime;
    int totalSeconds = (int)(timeDiff / 1000);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds / 60) % 60;
    int seconds = totalSeconds % 60;
   

    // UI
    float baseX = 0.03f;
    float baseY = 0.1f;
    float lineHeight = 0.025f;
    float bgWidth = 0.28f;
    float bgHeight = 0.345f;
    float y = baseY;
    float paddingX = 0.01f;

    DRAW_RECT(baseX + bgWidth / 2, baseY + bgHeight / 2, bgWidth, bgHeight, 0, 0, 0, 150);

    y += lineHeight / 2;
    sprintf(buffer, "~w~Object Attributes");
    DrawTextOnScreen(baseX + paddingX / 2, y, buffer);
    y += lineHeight;
    sprintf(buffer, "~p~Name: ~w~%s", object->name);
    DrawTextOnScreen(baseX + paddingX, y, buffer);
    y += lineHeight;
    sprintf(buffer, "~p~Hash: ~w~%i", model);
    DrawTextOnScreen(baseX + paddingX, y, buffer);
    y += lineHeight;
    sprintf(buffer, "~p~Age: ~w~%i:%i:%i", hours, minutes, seconds);
    DrawTextOnScreen(baseX + paddingX, y, buffer);
    y += lineHeight;
    DrawTextOnScreen(baseX + paddingX / 2, y, "~w~Object Offsets");
    y += lineHeight;
    sprintf(buffer, "~p~X: ~w~%f", pos.x);
    DrawTextOnScreen(baseX + paddingX, y, buffer);
    y += lineHeight;
    sprintf(buffer, "~p~Y: ~w~%f", pos.y);
    DrawTextOnScreen(baseX + paddingX, y, buffer);
    y += lineHeight;
    sprintf(buffer, "~p~Z: ~w~%f", pos.z);
    DrawTextOnScreen(baseX + paddingX, y, buffer);
    y += lineHeight;
    DrawTextOnScreen(baseX + paddingX / 2, y, "~w~Object Rotations");
    y += lineHeight;
    sprintf(buffer, "~p~Pitch: ~w~%f", rot.x);
    DrawTextOnScreen(baseX + paddingX, y, buffer);
    y += lineHeight;
    sprintf(buffer, "~p~Roll: ~w~%f", rot.y);
    DrawTextOnScreen(baseX + paddingX, y, buffer);
    y += lineHeight;
    sprintf(buffer, "~p~Yaw: ~w~%f", rot.z);
    DrawTextOnScreen(baseX + paddingX, y, buffer);
    y += lineHeight;
    if (IS_OBJECT_ATTACHED(object->o)) 
        DrawTextOnScreen(baseX + paddingX, y, "~p~is attached: ~w~yes");
    else 
        DrawTextOnScreen(baseX + paddingX, y, "~p~is attached: ~w~no");
}

void DrawObjectAxes(Object obj)
{
    if (!DOES_OBJECT_EXIST(obj)) return;

    uint model;
    GET_OBJECT_MODEL(obj, &model);

    vector3 minDim, maxDim;
    GET_MODEL_DIMENSIONS(model, &minDim, &maxDim);

    float lenX = 5.0f; //+ (maxDim.x - minDim.x);
    float lenY = 5.0f; //+ (maxDim.y - minDim.y);
    float lenZ = 5.0f; //+ (maxDim.z - minDim.z);

    vector3 origin;
    GET_OBJECT_COORDINATES(obj, &origin.x, &origin.y, &origin.z);

    vector3 endX, endY, endZ;
    GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(obj, lenX, 0.0f, 0.0f, &endX.x, &endX.y, &endX.z);
    GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(obj, 0.0f, lenY, 0.0f, &endY.x, &endY.y, &endY.z);
    GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS(obj, 0.0f, 0.0f, lenZ, &endZ.x, &endZ.y, &endZ.z);

    DrawInterpolatedCylinderLine(origin, endX, 3, 0.2f, 255, 0, 0, 255);
    DrawInterpolatedCylinderLine(origin, endY, 3, 0.2f, 0, 255, 0, 255);
    DrawInterpolatedCylinderLine(origin, endZ, 3, 0.2f, 0, 0, 255, 255);
}