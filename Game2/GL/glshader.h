#ifndef glshader_hpp
#define glshader_hpp

#include <stdio.h>

#include "UI/uitexture.h"
#include "../Strings/stringhash.h"
#include "../Containers/containers.h"
#include "../Strings/string.h"
#include "../Streams/streams.h"
#include "glwindow.h"

#include "../SDL2/SDL.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


using namespace coon;


enum GLShaderType { VERTEX_SHADER, FRAMGENT_SHADER, GEOMETRY_SHADER };


class GLShader
{
private:
    GLShaderType type;
    String name;
    String source;
    String log;
    
    GLuint shaderId;
private:
    void construct(const String& name, const String& source, GLShaderType type);
public:
    GLShader(const String& name, TextReader* reader);
    GLShader(const String& name, const String& source, GLShaderType type)
        { construct(name, source, type); }
    virtual ~GLShader();
    
    GLShaderType getType() { return type; }
    const String& getSource() { return source; }
    const String& getLog() { return log; }
    const String& getName() { return name; }
    GLuint getShaderId() { return shaderId; }
};


enum GLUniformType { UNIFORM_INT, UNIFORM_UINT, UNIFORM_FLOAT,
    UNIFORM_INT_ARRAY, UNIFORM_FLOAT_ARRAY, UNIFORM_VECTOR3DF,
};


class GLShaderUniform
{
public:
    GLUniformType type = UNIFORM_UINT;
    GLint index = -1;
    GLuint uintValue = 0;
    GLfloat floatValue = 0.0f;
    Vector3df vector3dfValue = Vector3df();
    Vector<GLint> intArray;
    Vector<GLfloat> floatArray;
    
    explicit GLShaderUniform(unsigned int val = 0)
        { type = UNIFORM_UINT; uintValue = val; }
    GLShaderUniform(float val, bool)
        { type = UNIFORM_FLOAT; floatValue = val; }
    GLShaderUniform(const Vector<GLint>& array)
        { type = UNIFORM_INT_ARRAY; intArray = array; }
    GLShaderUniform(const Vector<GLfloat>& array)
        { type = UNIFORM_FLOAT_ARRAY; floatArray = array; }
    GLShaderUniform(const Vector3df& val)
        { type = UNIFORM_VECTOR3DF; vector3dfValue = val; }
    virtual ~GLShaderUniform() {}
};


class GLShaderProgram
{
private:
    Vector<Pointer<GLShader>> shaders;
    Map<String, GLuint> attribs;
    Map<String, GLShaderUniform> uniforms;
    GLuint progId = 0;
public:
    GLShaderProgram();
    virtual ~GLShaderProgram();
    
    void link();
    
    void addShader(GLShader* sh) { shaders.push_back(Pointer<GLShader>(sh)); }
    void addAttrib(const String& name) { attribs[name] = 0; }
    void removeAttrib(const String& name) { attribs.erase(attribs.find(name)); }
    void setUniform(const String& name, GLShaderUniform def)
        { uniforms[name] = def; }
    void setUniformValue(const String& name, GLShaderUniform value);
    
    void beginUse();
    void endUse() { glUseProgram(0); }
};



#endif /* glshader_hpp */
