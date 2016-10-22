#ifndef COMMON_H
#define COMMON_H

#define PROJECT_TITLE "Project World"
#define PROJECT_VERSION "0.0.1 alpha"

#include "SDL2/SDL.h"
#include <iostream>
#include <OpenGL/gl.h>

#define RECT2SDL(r, R) r.x = R.x; r.y = R.y; r.w = R.w; r.h = R.h


struct RGBA
{
    int r;
    int g;
    int b;
    int a;
    RGBA(int _r = 0, int _g = 0, int _b = 0, int _a = 0) { r = _r; g = _g; b = _b; a = _a; }
};


struct RGBA32_channels {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};


union RGBA32
{
    unsigned int value;
    RGBA32_channels channels;
    
    RGBA32(unsigned int value = 0) { this->value = value; }
};


struct Point
{
    int x;
    int y;
    Point(int _x = 0, int _y = 0) { x = _x; y = _y; }
};


struct Rect
{
    int x;
    int y;
    int w;
    int h;
    Rect(int _x = 0, int _y = 0, int _w = 0, int _h = 0) { x = _x; y = _y; w = _w; h = _h; }
    Rect(Point pos, Point size);
    bool intersectsWith(const Rect& a);
    bool intersectsWith(int x, int y) { return intersectsWith(Rect(x, y, 1, 1)); }
    void clip(const Rect& form);
};


struct Vector2di
{
    int x;
    int y;
    Vector2di(int x = 0, int y = 0) { this->x = x; this->y = y; }
    Vector2di(const Vector2di& vec): Vector2di(vec.x, vec.y) {}
};


struct Vector2df
{
    float x;
    float y;
    Vector2df(float x = 0, float y = 0) { this->x = x; this->y = y; }
    Vector2df(const Vector2df& vec): Vector2df(vec.x, vec.y) {}
    
    Vector2df& add(const Vector2df& vec)
    { x += vec.x; y += vec.y; return *this; }
    Vector2df& sub(const Vector2df& vec)
    { x -= vec.x; y -= vec.y; return *this; }
    Vector2df& mul(const float num)
    { x *= num; y *= num; return *this; }
    Vector2df dup() { return Vector2df(*this); }
};


struct Vector3di
{
    int x;
    int y;
    int z;
    Vector3di(int x = 0, int y = 0, int z = 0) { this->x = x; this->y = y; this->z = z; }
    Vector3di(const Vector3di& vec): Vector3di(vec.x, vec.y, vec.z) {}
};


struct Vector3df
{
    float x;
    float y;
    float z;
    Vector3df(float x = 0, float y = 0, float z = 0) { this->x = x; this->y = y; this->z = z; }
    Vector3df(const Vector3df& vec): Vector3df(vec.x, vec.y, vec.z) {}
    
    Vector3df& add(const Vector3df& vec)
        { x += vec.x; y += vec.y; z += vec.z; return *this; }
    Vector3df& sub(const Vector3df& vec)
        { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
    Vector3df& mul(const float num)
        { x *= num; y *= num; z *= num; return *this; }
    Vector3df dup() { return Vector3df(*this); }
};


std::ostream& operator<<(std::ostream& s, const Point& r);
std::ostream& operator<<(std::ostream& s, const Rect& r);
std::ostream& operator<<(std::ostream& s, const Vector2di& r);
std::ostream& operator<<(std::ostream& s, const Vector2df& r);
std::ostream& operator<<(std::ostream& s, const Vector3di& r);
std::ostream& operator<<(std::ostream& s, const Vector3df& r);
std::ostream& operator<<(std::ostream& s, const RGBA& r);
int getTime();


void RGBA2GLfloatv(const RGBA& rgba, GLfloat* floats, int len);


#endif // COMMON_H
