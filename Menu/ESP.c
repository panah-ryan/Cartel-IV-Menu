#include "stdafx.h"
#include "MenuUiHandling.h"
#include "Player_Options.h"
#include "Functions.h"
#include "maths.h"

void NewDrawRect(float x, float y, float width, float height, RGBA color)
{
	int screenx, screeny;

	GET_SCREEN_RESOLUTION(&screenx, &screeny);
	DRAW_RECT(x / screenx, y / screeny, width / screenx, height / screeny, color.r, color.g, color.b, color.a);

	//DrawRect(x / screenx, y / screeny, width / screenx, height / screeny, color);
}

bool WorldToScreen(vector3 coords, vector2* out)
{
	int viewport;
	int screenx, screeny;

	GET_GAME_VIEWPORT_ID(&viewport);
	GET_SCREEN_RESOLUTION(&screenx, &screeny);
	GET_VIEWPORT_POSITION_OF_COORD(coords.x, coords.y, coords.z, viewport, &out->x, &out->y);
	return out->x <= screenx && out->x >= 0 && out->y <= screeny && out->y >= 0;
}

void DrawLine(float x1, float y1, float x2, float y2, float width, RGBA color)
{
	int screenx, screeny;

	GET_SCREEN_RESOLUTION(&screenx, &screeny);
	float h1 = y2 - y1;
	float l1 = x2 - x1;
	float l2 = SQRT((l1 * l1) + ((h1 * h1) * 3));
	float y = y1 + (h1 / 2);
	float angle = ATAN(h1 / l1);

	//DrawSprite(white, (x1 + ((x2 - x1) / 2)) / screenx, (y1 + (h1 / 2)) / screeny, l2 / screenx, width / screeny, angle, color);
	DRAW_SPRITE(GET_TEXTURE(GET_TXD("hud"), "radar_objective"), (x1 + ((x2 - x1) / 2)) / screenx, (y1 + (h1 / 2)) / screeny, l2 / screenx, width / screeny, angle, color.r, color.g, color.b, color.a);

	//DrawSprite(GET_TEXTURE(GET_TXD("hud"), "radar_objective"), (x1 + ((x2 - x1) / 2)) / screenx, (y1 + (h1 / 2)) / screeny, l2 / screenx, width / screeny, angle, color);
}

void DrawBoneLine(int ped, int bone1, int bone2, RGBA color)
{
	vector3 orig1, orig2;
	vector2 loc1, loc2;

	GET_PED_BONE_POSITION(ped, bone1, 0.0f, 0.0f, 0.0f, &orig1);
	GET_PED_BONE_POSITION(ped, bone2, 0.0f, 0.0f, 0.0f, &orig2);

	if (WorldToScreen(orig1, &loc1) && WorldToScreen(orig2, &loc2))
		DrawLine(loc1.x, loc1.y, loc2.x, loc2.y, 2.25f, color);
}

void DrawPlayerBones(int ped, RGBA color)
{
	DrawBoneLine(ped, BONE_HEAD, BONE_NECK, color);
	DrawBoneLine(ped, BONE_LEFT_UPPERARM, BONE_LEFT_FOREARM, color);
	DrawBoneLine(ped, BONE_LEFT_FOREARM, BONE_LEFT_HAND, color);
	DrawBoneLine(ped, BONE_RIGHT_UPPERARM, BONE_RIGHT_FOREARM, color);
	DrawBoneLine(ped, BONE_RIGHT_FOREARM, BONE_RIGHT_HAND, color);
	DrawBoneLine(ped, BONE_NECK, BONE_ROOT, color);
	DrawBoneLine(ped, BONE_ROOT, BONE_LEFT_THIGH, color);
	DrawBoneLine(ped, BONE_LEFT_THIGH, BONE_LEFT_CALF, color);
	DrawBoneLine(ped, BONE_LEFT_CALF, BONE_LEFT_FOOT, color);
	DrawBoneLine(ped, BONE_ROOT, BONE_RIGHT_THIGH, color);
	DrawBoneLine(ped, BONE_RIGHT_THIGH, BONE_RIGHT_CALF, color);
	DrawBoneLine(ped, BONE_RIGHT_CALF, BONE_RIGHT_FOOT, color);
}

void Make3DBox(int ped, RGBA color)
{
	vector3 min, max;
	vector3 origin;
	uint model;
	vector2 smin, smax, crnr1, crnr2, crnr3, crnr4, crnr5, crnr6;

	GET_CHAR_MODEL(ped, &model);
	GET_MODEL_DIMENSIONS(model, &min, &max);
	GET_CHAR_COORDINATES(ped, &origin.x, &origin.y, &origin.z);

	min = vector3Add(min, origin);
	max = vector3Add(max, origin);

	vector3 _1 = { max.x, min.y, min.z };
	vector3 _2 = { max.x, min.y, max.z };
	vector3 _3 = { min.x, min.y, max.z };
	vector3 _4 = { min.x, max.y, max.z };
	vector3 _5 = { min.x, max.y, min.z };
	vector3 _6 = { max.x, max.y, min.z };

	if (WorldToScreen(min, &smin) && WorldToScreen(max, &smax) && WorldToScreen(_1, &crnr1)
		&& WorldToScreen(_2, &crnr2) && WorldToScreen(_3, &crnr3) && WorldToScreen(_4, &crnr4)
		&& WorldToScreen(_5, &crnr5) && WorldToScreen(_6, &crnr6))
	{
		// From min to 2, 4 and 6 
		DrawLine(smin.x, smin.y, crnr1.x, crnr1.y, 1.0f, color);
		DrawLine(smin.x, smin.y, crnr3.x, crnr3.y, 1.0f, color);
		DrawLine(smin.x, smin.y, crnr5.x, crnr5.y, 1.0f, color);

		// From max to 5, 7 and 3 
		DrawLine(smax.x, smax.y, crnr4.x, crnr4.y, 1.0f, color);
		DrawLine(smax.x, smax.y, crnr6.x, crnr6.y, 1.0f, color);
		DrawLine(smax.x, smax.y, crnr2.x, crnr2.y, 1.0f, color);

		// From 2 to 7 and 3 
		DrawLine(crnr1.x, crnr1.y, crnr6.x, crnr6.y, 1.0f, color);
		DrawLine(crnr1.x, crnr1.y, crnr2.x, crnr2.y, 1.0f, color);

		// From 4 to 5 and 3 
		DrawLine(crnr3.x, crnr3.y, crnr4.x, crnr4.y, 1.0f, color);
		DrawLine(crnr3.x, crnr3.y, crnr2.x, crnr2.y, 1.0f, color);

		// From 6 to 5 and 7 
		DrawLine(crnr5.x, crnr5.y, crnr4.x, crnr4.y, 1.0f, color);
		DrawLine(crnr5.x, crnr5.y, crnr6.x, crnr6.y, 1.0f, color);
	}
}

void DrawPlayerBoxes(int playerIndex)
{
	PED_INDEX ped;
	vector2 headLoc, feetLoc;
	vector3 feetLeft, feetRight, feetAlpha, head = { 0.0f, 0.0f, 0.0f };
	int x, y;
	int viewport;
	int screenx, screeny;
	RGBA color = { 255, 255, 255, 255 };
	RGBA redColor = { 255, 0, 0, 255 };

	GET_PLAYER_CHAR(playerIndex, &ped);

	if (ped != NULL)
	{
		GET_GAME_VIEWPORT_ID(&viewport);
		GET_PED_BONE_POSITION(ped, BONE_LEFT_FOOT, 0.0f, 0.0f, 0.0f, &feetLeft);
		GET_PED_BONE_POSITION(ped, BONE_RIGHT_FOOT, 0.0f, 0.0f, 0.0f, &feetRight);
		feetAlpha.x = (feetLeft.x + feetRight.x) / 2;
		feetAlpha.y = (feetLeft.y + feetRight.y) / 2;
		feetAlpha.z = (feetLeft.z + feetRight.z) / 2;
		GET_PED_BONE_POSITION(ped, BONE_HEAD, 0.0f, 0.0f, 0.0f, &head);

		if (WorldToScreen(head, &headLoc) && WorldToScreen(feetAlpha, &feetLoc))
		{
			GET_SCREEN_RESOLUTION(&screenx, &screeny);
			float height = feetLoc.y - (headLoc.y - 50.0f);
			float width = height / 1.95f;

			//NewDrawRect(feetLoc.x, feetLoc.y, width, 1.0f, color);
			//NewDrawRect(feetLoc.x, feetLoc.y - height, width, 1.0f, color);
			//NewDrawRect(feetLoc.x - (width / 2), feetLoc.y - (height / 2), 1.0f, height, color);
			//NewDrawRect(feetLoc.x + (width / 2), feetLoc.y - (height / 2), 1.0f, height, color);
		//	DRAW_WINDOW_TEXT
			//Color playerColour = { 0, 0, 0, 255 };
			//GET_PLAYER_RGB_COLOUR(playerIndex, &playerColour.r, playerColour.g, playerColour.b);
			//if (!IS_CHAR_VISIBLE(ped))

			//DrawText(GET_PLAYER_NAME(playerIndex), Container.Ui.TextFont, headLoc.x / screenx, (headLoc.y - 70.0f) / screeny, 0.15f, 0.3f, Container.Ui.HeaderColor, 0, 1);

			//RGBA redColor = { 255, 0, 0, 255 };
			//RGBA playerColourRgba = { playerColour.r, playerColour.g, playerColour.b, 255 };

			if(bitTest(ctlMem->m_bit_sets.m_player, playerESPDrawLines))
				DrawLine(feetLoc.x, feetLoc.y, screenx / 2, screeny, 1.0f, redColor);

			//DrawPlayerBones(ped, playerColourRgba);

			DrawPlayerBones(ped, color);
			if(bitTest(ctlMem->m_bit_sets.m_player, playerESPDrawBoxes))
				Make3DBox(ped, color);
		}

		//NewDrawRect(headLoc.x, )
		//NewDrawRect(feetLoc.x - (width / 2), feetLoc.y, width, 1.0f, color);
		//NewDrawRect(feetLoc.x - (width / 2), feetLoc.y - height, 1.0f, height, color);
		//NewDrawRect(feetLoc.x + (width / 2), feetLoc.y - height, 1.0f, height + 1.0f, color);
		//DrawRect(headLoc.x, headLoc.y - 0.05f, 0.1f, 0.001f, color);
		//DrawRect(headLoc.x - (0.1f / 2), headLoc.y + 0.2f, 0.001f, 0.5f, color);
	}
}

void DrawInterpolatedCylinderLine(vector3 start, vector3 end, int segmentsPerMeter, float thickness, int r, int g, int b, int a)
{
	float totalDistance = SQRT(
		(end.x - start.x) * (end.x - start.x) +
		(end.y - start.y) * (end.y - start.y) +
		(end.z - start.z) * (end.z - start.z)
	);

	int segments = (int)(totalDistance * segmentsPerMeter);
	if (segments < 1)
		segments = 1;

	for (int i = 0; i <= segments; i++)
	{
		float t = (float)i / (float)segments;
		vector3 point = vector3_lerp(start, end, t);
		DRAW_CHECKPOINT_WITH_ALPHA(point.x, point.y, point.z, thickness, r, g, b, a);
	}
}

//testDict = GET_TXD("hud");
//white = GET_TEXTURE(testDict, "radar_objective");
//if need global/laggy do white = GET_TEXTURE(GET_TXT("hud"), "radar_objective");