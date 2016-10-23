

#ifndef glcontext_hpp
#define glcontext_hpp

#include <stdio.h>
#include "../Containers/containers.h"
#include "../Strings/string.h"
#include "glwindow.h"

#include "../SDL2/SDL.h"
#include <OpenGL/gl.h>
#include <cmath>


using namespace coon;


class GLWindow;


enum GLMatrix { GLM_PROJECTION, GLM_MODELVIEW, GLM_BOTH };


class GLContext
{
private:
    SDL_GLContext context;
    GLWindow* window;
    int MVMatrixStackSize = 0;
    int PRMatrixStackSize = 0;
    int lastLightIndex = 0;
    int maxLightIndex = GL_MAX_LIGHTS;
public:
    GLContext(GLWindow* window);
    ~GLContext() { SDL_GL_DeleteContext(context); }
    
    void drawTexture(UITexture *texture, Rect src, Rect dst);
    void drawTexture(GLuint texture, Rect src, Rect dst, float w, float h);
    void drawTextureBinded(Rect src, Rect dst, float w, float h);
    void clear();
    void renderPresent();
    void pushMatrix(const GLMatrix matrix = GLM_BOTH, const bool clear = true);
    void popMatrix(const GLMatrix matrix = GLM_BOTH);
    
    int getMVMatrixStackSize() { return MVMatrixStackSize; }
    int getPRMatrixStackSize() { return PRMatrixStackSize; }
    
    void setRenderBuffer(int buffer, int texture)
        {
            glBindBuffer(GL_FRAMEBUFFER, (GLuint) buffer);
            glBindTexture(GL_TEXTURE_2D, (GLuint) texture);
        }
    
    int getRenderBuffer()
        { GLint n = 0; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &n); return n; }
    
    int getLastLightIndex() { return lastLightIndex; }
    bool incLastLightIndex()
            { lastLightIndex++; return lastLightIndex < maxLightIndex; }
    void decLastLightIndex()
        { lastLightIndex--; if (lastLightIndex < -1) lastLightIndex = -1; }
};


#endif /* glcontext_h */
