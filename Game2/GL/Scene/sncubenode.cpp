#include "sncubenode.h"



void SNCubeNode::render(GLContext* context)
{
    GLfloat vertices[] = {
        // Back side
        -0.5, -0.5,  0.5,
         0.5, -0.5,  0.5,
         0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        
        // Right side
         0.5, -0.5,  0.5,
         0.5, -0.5, -0.5,
         0.5,  0.5, -0.5,
         0.5,  0.5,  0.5,
        
        // Left side
        -0.5, -0.5,  0.5,
        -0.5, -0.5, -0.5,
        -0.5,  0.5, -0.5,
        -0.5,  0.5,  0.5,
        
        // Front side
        -0.5, -0.5, -0.5,
         0.5, -0.5, -0.5,
         0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5,
        
        // Bottom side
        -0.5, -0.5,  0.5,
         0.5, -0.5,  0.5,
         0.5, -0.5, -0.5,
        -0.5, -0.5, -0.5,
        
        // Top side
        -0.5,  0.5,  0.5,
         0.5,  0.5,  0.5,
         0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5,
    };
    
    GLfloat texCoords[] = {
        // Back side
        0, 1,
        1, 1,
        1, 0,
        0, 0,
        
        // Right side
        0, 1,
        1, 1,
        1, 0,
        0, 0,
        
        // Left side
        1, 1,
        0, 1,
        0, 0,
        1, 0,
        
        // Front side
        1, 1,
        0, 1,
        0, 0,
        1, 0,
        
        // Bottom side
        1, 1,
        0, 1,
        0, 0,
        1, 0,
        
        // Top side
        1, 1,
        0, 1,
        0, 0,
        1, 0,
    };
    
    GLubyte indices[] = {
        // Back size
        0,  1,  2,
        0,  2,  3,
        
        // Right side
        4,  5,  6,
        4,  6,  7,
        
        // Left side
        8,  9,  10,
        8,  10, 11,
        
        // Front side
        12, 13, 14,
        12, 14, 15,
        
        // Bottom side
        16, 17, 18,
        16, 18, 19,
        
        // Top side
        20, 21, 22,
        20, 22, 23,
    };
    
    GLfloat normals[] = {
        // Back side
         0,  0,  1,
         0,  0,  1,
         0,  0,  1,
         0,  0,  1,
        
        // Right side
         1,  0,  0,
         1,  0,  0,
         1,  0,  0,
         1,  0,  0,
        
        // Left side
        -1,  0,  0,
        -1,  0,  0,
        -1,  0,  0,
        -1,  0,  0,
        
        // Front side
         0,  0, -1,
         0,  0, -1,
         0,  0, -1,
         0,  0, -1,
        
        // Bottom side
         0, -1,  0,
         0, -1,  0,
         0, -1,  0,
         0, -1,  0,
        
        // Top side
         0,  1,  0,
         0,  1,  0,
         0,  1,  0,
         0,  1,  0,
    };
    
    /*for (int i = 0; i < 3*24; i++) {
        normals[i] *= -1;
    }*/
    
    GLfloat tangents[6*6];
    GLfloat bitangents[6*6];
    int j = 0;
    
    for (int i = 0; i < 6*6; i += 9) {
        Vector3df v0(vertices[i+0], vertices[i+1], vertices[i+2]);
        Vector3df v1(vertices[i+3], vertices[i+4], vertices[i+5]);
        Vector3df v2(vertices[i+6], vertices[i+7], vertices[i+8]);
        
        Vector2df uv0(texCoords[j+0], texCoords[j+1]);
        Vector2df uv1(texCoords[j+2], texCoords[j+3]);
        Vector2df uv2(texCoords[j+4], texCoords[j+5]);
        j += 6;
        
        Vector3df deltaPos1 = v1.dup().sub(v0);
        Vector3df deltaPos2 = v2.dup().sub(v0);
        Vector2df deltaUV1 = uv1.dup().sub(uv0);
        Vector2df deltaUV2 = uv2.dup().sub(uv0);
        
        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        
        Vector3df tangent =
            deltaPos1.dup().mul(deltaUV2.y).sub(deltaPos2.dup().mul(deltaUV1.y));
        tangent.mul(r);
        
        Vector3df bitangent =
        deltaPos2.dup().mul(deltaUV1.x).sub(deltaPos1.dup().mul(deltaUV2.x));
        bitangent.mul(r);
        
        for (int k = 0; k < 9; k += 3) {
            tangents[i+k+0] = tangent.x;
            tangents[i+k+1] = tangent.y;
            tangents[i+k+2] = tangent.z;
            
            bitangents[i+k+0] = bitangent.x;
            bitangents[i+k+1] = bitangent.y;
            bitangents[i+k+2] = bitangent.z;
        }
    }
    

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    
    glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_BYTE, indices);
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
