#ifndef COMMON_H
#define COMMON_H

#define PROJECT_TITLE "Project World"
#define PROJECT_VERSION "0.0.1 alpha"

// =====[ Build Configuration ]=====

#define OSX
#define NO_SDL_GLEXT
//#define LUA_FUNC_TEST
#define LUA_LOG

// =====[ End Build Configuration ]=====


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
    
    float length() { return sqrtf(x*x + y*y + z*z); }
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


template<typename T, int N> struct MatrixNd
{
    T data[N*N];
    
    T get(int row, int col) { return data[col * N + row]; }
    void set(int row, int col, T val) { data[col * N + row] = val; }
    
    MatrixNd<T, N> dup() { return MatrixNd<T, N>(*this); }
    MatrixNd<T, N - 1> minor(int row, int col);
    void transpose();
    MatrixNd<T, N> inverse();
    void mul(float val)
            { for (int i = 0; i < N*N; i++) { data[i] *= val; }; }
    float def();
    Vector3df mul(const Vector3df& vec);
    
    MatrixNd<T, N>() {}
    MatrixNd<T, N>(T data[N*N]) { this->data = data; }
    //MatrixNd<T, N>(const MatrixNd<T, N>& mat): MatrixNd<T, N>(mat.data) {}
};


typedef MatrixNd<float, 4> Matrix4df;
typedef MatrixNd<int, 4> Matrix4di;
typedef MatrixNd<float, 3> Matrix3df;
typedef MatrixNd<int, 3> Matrix3di;
typedef MatrixNd<float, 2> Matrix2df;
typedef MatrixNd<int, 2> Matrix2di;


template<typename T, int N> void MatrixNd<T, N>::transpose()
{
    MatrixNd<T, N> trans;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            trans.set(j, i, get(i, j));
        }
    }
    
    for (int i = 0; i < N*N; i++) {
        data[i] = trans.data[i];
    }
}


template<typename T, int N> MatrixNd<T, N - 1>
MatrixNd<T, N>::minor(int row, int col)
{
    MatrixNd<T, N - 1> minor;
    int drow = 0;
    
    for (int mrow = 0; mrow < N; mrow++) {
        int dcol = 0;
        
        if (mrow != row) {
            for (int mcol = 0; mcol < N; mcol++) {
                if (mcol != col) {
                    minor.set(drow, dcol, get(mrow, mcol));
                    dcol++;
                }
            }
            
            drow++;
        }
    }
    
    return minor;
}

template<typename T> struct MatrixNd<T, 1>
{
    MatrixNd<T, 0> minor(int row, int col) { return NULL; }
    void set(int row, int col, T val) {}
    MatrixNd<T, 1> dup() { return MatrixNd<T, 1>(); }
    void transpose() {}
    MatrixNd<T, 0> inverse() { return NULL; }
    void mul(float val) {}
    float def() { return 0.0f; }
};


template<typename T, int N> float MatrixNd<T, N>::def()
{
    if (N < 1) return 0.0f;
    if (N == 1) return (float) data[0];
    if (N == 2) return get(0, 0) * get(1, 1) - get(0, 1) * get(1, 0);
    float sum = 0;
    
    for (int col = 0; col < N; col++) {
        MatrixNd<T, N - 1> minor = this->minor(0, col);
        
        float def = get(0, col) * minor.def();
        if (col % 2 == 1) def *= -1.0f;
        
        sum += def;
    }
    
    return sum;
}


template<typename T, int N> MatrixNd<T, N> MatrixNd<T, N>::inverse()
{
    MatrixNd<float, N> additions;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float addition = minor(i, j).def();
            if ((i + j) % 2 == 1) addition *= -1.0f;
            additions.set(i, j, addition);
        }
    }
    
    additions.transpose();
    additions.mul(1.0 / this->def());
    return additions;
}


template<typename T, int N> Vector3df
        MatrixNd<T, N>::mul(const Vector3df &vec)
{
    float w = 1.0f;
    Vector3df res;
    
    res.x = get(0,0)*vec.x + get(0,1)*vec.y + get(0,2)*vec.z + get(0,3)*w;
    res.y = get(1,0)*vec.x + get(1,1)*vec.y + get(1,2)*vec.z + get(1,3)*w;
    res.z = get(2,0)*vec.x + get(2,1)*vec.y + get(2,2)*vec.z + get(2,3)*w;
    
    return res;
}



#endif // COMMON_H
