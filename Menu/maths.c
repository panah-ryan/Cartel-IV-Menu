#include "maths.h"

float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

/*vector3 vector3_add(vector3 a, vector3 b) {
    return (vector3){ a.x + b.x, a.y + b.y, a.z + b.z };
}
vector3 vector3_sub(vector3 a, vector3 b) {
    return (vector3){ a.x - b.x, a.y - b.y, a.z - b.z };
}
float vector3_dot(vector3 a, vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}*/

vector3 vector3_scale(vector3 v, float scale) {
    return (vector3) { v.x* scale, v.y* scale, v.z* scale };
}

vector3 vector3_cross(vector3 a, vector3 b) {
    return (vector3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float vector3_length(vector3 v) {
    return SQRT(v.x * v.x + v.y * v.y + v.z * v.z);
}

float vector3_distance(vector3 a, vector3 b) {
    return vector3_length(vector3Sub(a, b));
}


vector3 vector3_lerp(vector3 a, vector3 b, float t)
{
    vector3 result;
    result.x = lerp(a.x, b.x, t);
    result.y = lerp(a.y, b.y, t);
    result.z = lerp(a.z, b.z, t);
    return result;
}

vector3 vector3_normalize(vector3 v) {
    float length = SQRT(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0.0f) return (vector3){0.0f, 0.0f, 0.0f};
    return (vector3){ v.x / length, v.y / length, v.z / length };
}

vector3 rotate_vector_by_input(vector3 dir, vector3 delta)
{
    vector3 forward = vector3_normalize(dir);
    vector3 right = vector3_normalize(vector3_cross((vector3) { 0, 0, 1 }, forward));
    vector3 up = vector3_normalize(vector3_cross(forward, right));

    quaternion qPitch = quaternion_from_axis_angle(right, delta.x);
    quaternion qYaw = quaternion_from_axis_angle(up, delta.y);

    quaternion qCombined = quaternion_multiply(qYaw, qPitch);
    return quaternion_rotate_vector(qCombined, forward);
}

quaternion CreateFromAxisAngle(vector3 axis, float angleDegrees)
{
    float halfAngle = angleDegrees * 0.5f;
    float s = SIN(halfAngle);

    quaternion q = {
        axis.x * s,
        axis.y * s,
        axis.z * s,
        COS(halfAngle)
    };

    return quaternion_normalized(q);
}

quaternion quaternion_from_axis_angle(vector3 axis, float angleRad) {
    float halfAngle = angleRad * 0.5f;
    float s = SIN(halfAngle);
    return quaternion_normalized((quaternion){
        axis.x * s,
        axis.y * s,
        axis.z * s,
        COS(halfAngle)
    });
}

quaternion quaternion_multiply(quaternion a, quaternion b) {
    return (quaternion){
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    };
}

quaternion quaternion_lerp(quaternion a, quaternion b, float t) {
    quaternion result;
    result.x = lerp(a.x, b.x, t);
    result.y = lerp(a.y, b.y, t);
    result.z = lerp(a.z, b.z, t);
    result.w = lerp(a.w, b.w, t);
    return quaternion_normalized(result);
}

quaternion quaternion_normalized(quaternion q) {
    float mag = SQRT(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    if (mag == 0.0f) return (quaternion){0.0f, 0.0f, 0.0f, 1.0f};
    return (quaternion){ q.x / mag, q.y / mag, q.z / mag, q.w / mag };
}

vector3 quaternion_rotate_vector(quaternion q, vector3 v) {
    quaternion qVec = { v.x, v.y, v.z, 0.0f };
    quaternion qConj = { -q.x, -q.y, -q.z, q.w };
    quaternion result = quaternion_multiply(quaternion_multiply(q, qVec), qConj);
    return (vector3){ result.x, result.y, result.z };
}

vector3 quaternion_to_euler(quaternion q)
{
    vector3 euler;

    float sinPitch = 2.0f * (q.w * q.x + q.y * q.z);
    float cosPitch = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    euler.x = ATAN2(sinPitch, cosPitch);

    float sinYaw = 2.0f * (q.w * q.y - q.z * q.x);
    sinYaw = clamp(sinYaw, -1.0f, 1.0f);
    euler.y = ASIN(sinYaw);

    float sinRoll = 2.0f * (q.w * q.z + q.x * q.y);
    float cosRoll = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    euler.z = ATAN2(sinRoll, cosRoll);

    return euler;
}

quaternion euler_to_quaternion(vector3 euler)
{
    float cy = COS(euler.y * 0.5f);
    float sy = SIN(euler.y * 0.5f);
    float cp = COS(euler.x * 0.5f); 
    float sp = SIN(euler.x * 0.5f);
    float cr = COS(euler.z * 0.5f); 
    float sr = SIN(euler.z * 0.5f);

    quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return quaternion_normalized(q);
}