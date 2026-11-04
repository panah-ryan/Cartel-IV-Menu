#pragma once

bool WorldToScreen(vector3 coords, vector2* out);
void DrawLine(float x1, float y1, float x2, float y2, float width, RGBA color);
void DrawPlayerBoxes(int playerIndex);
void DrawInterpolatedCylinderLine(vector3 start, vector3 end, int segmentsPerMeter, float thickness, int r, int g, int b, int a);