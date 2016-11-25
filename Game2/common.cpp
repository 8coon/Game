#include "common.h"


bool Rect::intersectsWith(const Rect &a)
{
    return ((x < a.x) && (y < a.y) && (x + w > a.x) && (y + h > a.y)) ||
            ((x > a.x) && (y > a.y) && (x < a.x + a.w) && (y < a.y + a.h));
}


std::ostream& operator<<(std::ostream& s, const Point& r)
{
    s << "Point: (x = " << r.x << ", y = " << r.y << ")";
    return s;
}


std::ostream& operator<<(std::ostream& s, const Rect& r)
{
    s << "Rect: (x = " << r.x << ", y = " << r.y << ", w = " << r.w << ", h = " << r.h << ")";
    return s;
}


std::ostream& operator<<(std::ostream& s, const Vector2di& r)
{
    s << "Vector2di: (x = " << r.x << ", y = " << r.y << ")";
    return s;
}


std::ostream& operator<<(std::ostream& s, const Vector2df& r)
{
    s << "Vector2df: (x = " << r.x << ", y = " << r.y << ")";
    return s;
}


std::ostream& operator<<(std::ostream& s, const Vector3di& r)
{
    s << "Vector3di: (x = " << r.x << ", y = " << r.y << ", z = " << r.z << ")";
    return s;
}


std::ostream& operator<<(std::ostream& s, const Vector3df& r)
{
    s << "Vector3df: (x = " << r.x << ", y = " << r.y << ", z = " << r.z << ")";
    return s;
}


std::ostream& operator<<(std::ostream& s, const RGBA& r)
{
    s << "RGBA: (r = " << r.r << ", g = " << r.g << ", b = " << r.b << ", a = " << r.a << ")";
    return s;
}


void Rect::clip(const Rect& form)
{
    /*if (x + w < form.x) w = 0;
    if (x < form.x) x = form.x;
    if (x > form.x + form.w) w = 0;
    if (x + w > form.x + form.w) w = form.x + form.w - x;

    if (y + h < form.y) h = 0;
    if (y < form.y) y = form.y;
    if (y > form.y + form.h) h = 0;
    if (y + h > form.y + form.h) h = form.y + form.h - y;*/
    //if (y < form.y) y = form.y;
}


int getTime()
{
    return SDL_GetTicks();
}



void RGBA2GLfloatv(const RGBA& rgba, GLfloat* floats, int len)
{
    GLfloat r = rgba.r / 255.0;
    GLfloat g = rgba.g / 255.0;
    GLfloat b = rgba.b / 255.0;
    GLfloat a = rgba.a / 255.0;
    
    if (len > 0) floats[0] = r;
    if (len > 1) floats[1] = g;
    if (len > 2) floats[2] = b;
    if (len > 3) floats[3] = a;
}


