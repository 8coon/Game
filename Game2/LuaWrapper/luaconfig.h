#ifndef LUACONFIG_H
#define LUACONFIG_H

#include "../Strings/stringhash.h"
#include "../Containers/containers.h"
#include "../Strings/string.h"
#include "../LuaWrapper/luawrapper.h"
#include "../common.h"

using namespace coon;


#define TRUE "TRUE"
#define FALSE "FALSE"

#define X_RESOLUTION "X_RESOLUTION"
#define Y_RESOLUTION "Y_RESOLUTION"
#define FPS "FPS"
#define FULLSCREEN "FULLSCREEN"
#define VSYNC "VSYNC"

#define CONFIG_FILE "Scripts/config.lua"


class LuaConfig: protected LuaWrapper
{
protected:
    void save();
public:
    LuaConfig();

    Vector2di getResolution();
    int getFPS();
    bool getFullscreen();
    bool getVSync();

    void setResolution(Vector2di resolution);
    void setFPS(int fps);
    void setFullscreen(bool fullscreen);
    void setVSync(bool vsync);
};

#endif // LUACONFIG_H
