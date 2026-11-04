#pragma once
#include "stdafx.h"

float clamp(float value, float min, float max);
float lerp(float a, float b, float t);

//vector3 vector3_add(vector3 a, vector3 b); //use intrinsic function vector3Add instead
vector3 vector3_scale(vector3 v, float scale);
//float vector3_dot(vector3 a, vector3 b); //use intrinsic functiuon vector3Dot instead
vector3 vector3_lerp(vector3 a, vector3 b, float t);
vector3 vector3_normalize(vector3 v);
//vector3 vector3_sub(vector3 a, vector3 b); //use intrinsic function vector3Sub instead
vector3 vector3_cross(vector3 a, vector3 b);
float vector3_length(vector3 v);
float vector3_distance(vector3 a, vector3 b);
vector3 rotate_vector_by_input(vector3 dir, vector3 delta);

quaternion quaternion_from_axis_angle(vector3 axis, float angleRad);
quaternion quaternion_multiply(quaternion a, quaternion b);
quaternion quaternion_normalized(quaternion q);
vector3 quaternion_rotate_vector(quaternion q, vector3 v);
vector3 quaternion_to_euler(quaternion q);
quaternion quaternion_lerp(quaternion a, quaternion b, float t);
quaternion CreateFromAxisAngle(vector3 axis, float angleDegrees);
quaternion euler_to_quaternion(vector3 euler);
