
#include "glcontext.h"


GLContext::GLContext(GLWindow* window)
{
    this->window = window;
    this->context = SDL_GL_CreateContext(window->getWindow());
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);
    glShadeModel(GL_SMOOTH);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glColor4f(1.0, 1.0, 1.0, 1.0);
    
    SDL_GL_SetSwapInterval(1);
    
#ifdef OSX
    glewExperimental = GL_TRUE;
    glewInit();
#endif
}


void GLContext::drawTexture(UITexture *texture, Rect src, Rect dst)
{
    SDL_GL_BindTexture(texture->getTexture(), NULL, NULL);
    drawTextureBinded(src, dst, texture->getWidth(), texture->getHeight());
    SDL_GL_UnbindTexture(texture->getTexture());
}


void GLContext::drawTexture(GLuint texture, Rect src, Rect dst, float w, float h)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    drawTextureBinded(src, dst, w, h);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void GLContext::drawTextureBinded(Rect src, Rect dst, float w, float h)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    Point res = window->getResolution();
    pushMatrix();
    //glPushAttrib(GL_CURRENT_BIT);
    
    glOrtho(0, res.x, res.y, 0, 0, 1000);
    
    
    GLfloat vertices[] = {
        (float) (dst.x),         (float) (dst.y),         0,
        (float) (dst.x + dst.w), (float) (dst.y),         0,
        (float) (dst.x + dst.w), (float) (dst.y + dst.h), 0,
        (float) (dst.x),         (float) (dst.y + dst.h), 0,
    };
    
    GLfloat texCoords[] = {
        (float) (src.x / w),           (float) (src.y / h),
        (float) ((src.x + src.w) / w), (float) (src.y / h),
        (float) ((src.x + src.w) / w), (float) ((src.y + src.h) / h),
        (float) (src.x / w),           (float) ((src.y + src.h) / h),
    };
    
    GLubyte indices[] = {
        0, 1, 2,
        0, 2, 3,
    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_LIGHTING);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
    
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    //glPopAttrib();
    popMatrix();
}


void GLContext::pushMatrix(const GLMatrix matrix, const bool clear)
{
    switch (matrix) {
        case GLM_MODELVIEW:
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            MVMatrixStackSize++;
            break;
        case GLM_PROJECTION:
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            PRMatrixStackSize++;
            break;
        case GLM_BOTH:
            pushMatrix(GLM_PROJECTION, clear);
            pushMatrix(GLM_MODELVIEW, clear);
            return;
    }
    
    if (clear) glLoadIdentity();
}


void GLContext::popMatrix(const GLMatrix matrix)
{
    switch (matrix) {
        case GLM_MODELVIEW:
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
            MVMatrixStackSize--;
            break;
        case GLM_PROJECTION:
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            PRMatrixStackSize--;
            break;
        case GLM_BOTH:
            popMatrix(GLM_PROJECTION);
            popMatrix(GLM_MODELVIEW);
            break;
    }
}


void GLContext::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lastLightIndex = -1;
    
    //pushMatrix();
    
    float w = window->getResolution().x;
    float h = window->getResolution().y;
    //float r = w / h;
    glViewport(0, 0, window->getResolution().x, window->getResolution().y);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    GLfloat zNear = 0.1f;
    GLfloat zFar = 255.0f;
    GLfloat aspect = w / h;
    GLfloat fOV = 45;
    GLfloat fH = tan((float) fOV / 360.0f * 3.14159f) * zNear;
    GLfloat fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluPerspective(60.0, w / h, 0.0, 1000.0);
 
    //glScalef(0.5, 0.5, 0.5);
    glEnable(GL_DEPTH_TEST);
}


void GLContext::renderPresent()
{
    //popMatrix();
    SDL_GL_SwapWindow(this->window->getWindow());
}








