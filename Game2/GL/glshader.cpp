#include "glshader.h"


GLShader::GLShader(const String& name, TextReader* reader)
{
    if (name.endsWith(".frag")) type = FRAMGENT_SHADER;
    if (name.endsWith(".vert")) type = VERTEX_SHADER;
    if (name.endsWith(".geom")) type = GEOMETRY_SHADER;
    
    source = reader->readAll();
    construct(name, source, type);
}


void GLShader::construct(const String& name, const String& source,
                         GLShaderType type)
{
    this->source = source;
    this->type = type;
    this->name = name;
    
    switch (type) {
        case FRAMGENT_SHADER: shaderId = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case VERTEX_SHADER: shaderId = glCreateShader(GL_VERTEX_SHADER);
            break;
        case GEOMETRY_SHADER: shaderId = glCreateShader(GL_GEOMETRY_SHADER_EXT);
            break;
    }
    
    GLchar* buf = new GLchar[source.size() + 1];
    buf[source.size()] = (GLchar) '\0';
    
    for (int i = 0; i < (int) source.size(); i++) {
        buf[i] = (GLchar) source.at(i).toChar();
    }
    
    glShaderSource((GLuint) shaderId, (GLsizei) 1, (GLchar* const*) &buf, NULL);
    glCompileShader(shaderId);
    delete[] buf;
    
    int infoLogLen = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLen);
    
    if (infoLogLen > 1) {
        int charsWritten = 0;
        char* rawLog = new char[infoLogLen];
        memset(rawLog, 0, infoLogLen * sizeof(char));
        
        glGetShaderInfoLog(shaderId, infoLogLen, &charsWritten, rawLog);
        log = String(rawLog);
        
        delete[] rawLog;
    }
    
    std::cout << "===== Shader log for " << name << " =====" << std::endl;
    std::cout << log << std::endl;
    std::cout << "===== End shader log =====" << std::endl;
}


GLShader::~GLShader()
{
}


void GLShaderProgram::setUniformValue(const String& name, GLShaderUniform value)
{
    switch(value.type) {
        case UNIFORM_UINT: uniforms[name].uintValue = value.uintValue; break;
        case UNIFORM_FLOAT: uniforms[name].floatValue = value.floatValue; break;
        default: break;
    }
}



GLShaderProgram::GLShaderProgram()
{
    progId = glCreateProgram();
}


GLShaderProgram::~GLShaderProgram()
{
    glUseProgram(0);
    glDeleteProgram(progId);
}


void GLShaderProgram::beginUse()
{
    setUniformValue("time", GLShaderUniform(SDL_GetTicks(), true));
    glUseProgram(progId);
    
    for (Pair<String, GLShaderUniform> uniform: uniforms) {
        switch (uniform.second.type) {
            case UNIFORM_INT:
                glUniform1i(uniform.second.index, uniform.second.uintValue);
                break;
            case UNIFORM_UINT:
                glUniform1i(uniform.second.index, uniform.second.uintValue);
                break;
            case UNIFORM_FLOAT:
                glUniform1f(uniform.second.index, uniform.second.floatValue);
                break;
        }
    }
}


void GLShaderProgram::link()
{
    for (Pointer<GLShader> shader: shaders) {
        glAttachShader(progId, shader->getShaderId());
    }
    
    glLinkProgram(progId);
    
    GLint ok = 0;
    glGetProgramiv(progId, GL_LINK_STATUS, &ok);
    
    if (!ok) {
        std::cout << "Couldn't attach shaders!" << std::endl;
        return;
    }
    
    for (Pair<String, GLuint> attrib: attribs) {
        const char* name = attrib.first.toStdString().c_str();
        attrib.second = glGetAttribLocation(progId, name);
        attribs[attrib.first] = attrib.second;
        
        if (attrib.second == -1) {
            std::cout << "Error binding shader attribute! ";
            std::cout << "No such attribute: " << name << std::endl;
        }
    }
    
    int count = 0;
    glGetProgramiv(progId, GL_ACTIVE_UNIFORMS, &count);
    std::cout << "Active uniforms: " << count << std::endl;
    
    uniforms["time"] = GLShaderUniform(SDL_GetTicks(), true);
    
    for (Pair<String, GLShaderUniform> uniform: uniforms) {
        const char* name = uniform.first.toStdString().c_str();
        uniform.second.index = glGetUniformLocation(progId, name);
        uniforms[uniform.first] = uniform.second;
        
        if (uniform.second.index == -1) {
            std::cout << "Error binding shader uniform! ";
            std::cout << "No such uniform: " << name << std::endl;
        }
    }
}













