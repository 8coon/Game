#include "snlandscapenode.h"


float MSTexHeightmap::getHeightAt(const int x, const int y)
{
    int nx = x + getSizeX() / 2;
    int ny = y + getSizeY() / 2;
    if ((nx > 0) && (ny > 0) && (nx < getSizeX()) && (ny < getSizeY())) {
        float r = 0;
        r += texture->getRawData()[ny * getSizeX() + nx].channels.r / 255.0;
        r += texture->getRawData()[ny * getSizeX() + nx].channels.g / 255.0;
        r += texture->getRawData()[ny * getSizeX() + nx].channels.b / 255.0;
        return (r / 3.0f) * maxHeight;
    }
    return 0.0f;
}


SNLandscapeNode::SNLandscapeNode(const String& name, MSLandscape* landscape)
        : ISceneNode(name)
{
    this->landscape = Pointer<MSLandscape>(landscape);
    
    int j = 0;
    int x = -landscape->getSizeX() / 2; int canX = x;
    int z = -landscape->getSizeY() / 2; int canZ = z;
    
    float texX = 0.0f;
    float texY = 0.0f;
    
    for (int i = 0; i < landscape->getSizeX() * landscape->getSizeY() * 4; i++) {
        switch (j) {
            case 0: break;
            case 1: x++; break;
            case 2: x--; z++; break;
            case 3: x++; break;
        }
        
        float height = landscape->getHeightAt(x, z);
        vertices.push_back(Vector3df(x, height, z));
        normals.push_back(Vector3df(0.0f, 1.0f, 0.0f));
            
        if (j == 3) {
            canX++;
            
            if (canX == landscape->getSizeX() / 2) {
                canZ++;
                canX = -landscape->getSizeX() / 2;
            }
            
            x = canX;
            z = canZ;
            
            indices.push_back(i + 0 - 3);
            indices.push_back(i + 1 - 3);
            indices.push_back(i + 2 - 3);
            indices.push_back(i + 1 - 3);
            indices.push_back(i + 2 - 3);
            indices.push_back(i + 3 - 3);
                
            /*texCoords.push_back(Vector2df(1.0f, 1.0f));
            texCoords.push_back(Vector2df(0.0f, 1.0f));
            texCoords.push_back(Vector2df(1.0f, 0.0f));
            texCoords.push_back(Vector2df(0.0f, 0.0f));*/
            
            float scale = landscape->getTexScale();
            texCoords.push_back(Vector2df(texX + scale, texY + scale));
            texCoords.push_back(Vector2df(texX,         texY + scale));
            texCoords.push_back(Vector2df(texX + scale, texY        ));
            texCoords.push_back(Vector2df(texX,         texY        ));
            
            texX += scale;
            if (texX > 1.0f) {
                texX = 0.0f;
                texY += scale;
            }
                
            j = -1;
        }
            
        j++;
    }
    
    bakedVertices  = new GLfloat[vertices.size()  * 3];
    bakedNormals   = new GLfloat[normals.size()   * 3];
    bakedTexCoords = new GLfloat[texCoords.size() * 2];
    bakedIndices   = new  GLuint[indices.size()      ];
    
    for (int i = 0; i < ((int) vertices.size()) * 3; i += 3) {
        bakedVertices [i    ] = vertices [i / 3].x;
        bakedVertices [i + 1] = vertices [i / 3].y;
        bakedVertices [i + 2] = vertices [i / 3].z;
        
        bakedNormals  [i    ] = normals  [i / 3].x;
        bakedNormals  [i + 1] = normals  [i / 3].y;
        bakedNormals  [i + 2] = normals  [i / 3].z;
    }
    
    for (int i = 0; i < ((int) texCoords.size()) * 2; i += 2) {
        bakedTexCoords[i    ] = texCoords[i / 2].x;
        bakedTexCoords[i + 1] = texCoords[i / 2].y;
    }
    
    for (int i = 0; i < (int) indices.size(); i++) {
        bakedIndices[i] = indices[i];
    }
    
    lenVertices = (int) vertices.size() * 3;
    lenIndices = (int) indices.size();
    
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();
}


SNLandscapeNode::~SNLandscapeNode()
{
    if (this->bakedNormals != NULL) delete[] bakedNormals;
    if (this->bakedIndices != NULL) delete[] bakedIndices;
    if (this->bakedVertices != NULL) delete[] bakedVertices;
    if (this->bakedTexCoords != NULL) delete[] bakedTexCoords;
}


void SNLandscapeNode::render(GLContext *context)
{
    //glEnableClientState(GL_NORMAL_ARRAY);
    //glNormalPointer(GL_FLOAT, 0, bakedNormals);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, bakedVertices);
    
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, bakedTexCoords);
    
    glDrawElements(GL_TRIANGLES, lenIndices, GL_UNSIGNED_INT, bakedIndices);
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
}
