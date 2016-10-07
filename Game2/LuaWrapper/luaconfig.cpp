#include "luaconfig.h"


LuaConfig::LuaConfig(): LuaWrapper()
{
    getRoot().setField(X_RESOLUTION, LuaObject(640));
    getRoot().setField(Y_RESOLUTION, LuaObject(480));
    getRoot().setField(FPS, LuaObject(60));
    getRoot().setField(FULLSCREEN, LuaObject(FALSE));
    getRoot().setField(VSYNC, LuaObject(TRUE));

    TextFileReader reader(CONFIG_FILE);
    execute(&reader, NULL);
}


void LuaConfig::save()
{
    TextFileWriter writer(CONFIG_FILE);

    for (auto& pair: getRoot().getFields()) {
        StringBuilder builder;
        builder.append(pair.first);
        builder.append(" = ");

        switch (pair.second->getType()) {
            case LW_NUMBER:
                builder.append(String::fromDouble(*(pair.second->getNumber())));
                break;
            case LW_STRING:
                builder.append("\"");
                builder.append(String(*(pair.second->getString())));
                builder.append("\"");
                break;
            default: break;
        }

        writer.writeLine(builder.toString());
    }
}


Vector2di LuaConfig::getResolution()
{
    return Vector2di(*(getRoot().getField(X_RESOLUTION).getNumber()),
                     *(getRoot().getField(Y_RESOLUTION).getNumber()));
}


void LuaConfig::setResolution(Vector2di resolution)
{
    getRoot().setField(X_RESOLUTION, LuaObject(resolution.x));
    getRoot().setField(Y_RESOLUTION, LuaObject(resolution.y));
    save();
}


int LuaConfig::getFPS()
{
    return *(getRoot().getField(FPS).getNumber());
}


void LuaConfig::setFPS(int fps)
{
    getRoot().setField(FPS, LuaObject(fps));
    save();
}


bool LuaConfig::getFullscreen()
{
    return (*(getRoot().getField(FULLSCREEN).getString())).equals(TRUE);
}


void LuaConfig::setFullscreen(bool fullscreen)
{
    getRoot().setField(FULLSCREEN, LuaObject((fullscreen)?(TRUE):(FALSE)));
    save();
}


bool LuaConfig::getVSync()
{
    return (*(getRoot().getField(VSYNC).getString())).equals(TRUE);
}


void LuaConfig::setVSync(bool vsync)
{
    getRoot().setField(VSYNC, LuaObject((vsync)?(TRUE):(FALSE)));
    save();
}
