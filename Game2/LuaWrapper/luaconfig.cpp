#include "luaconfig.h"


LuaConfig::LuaConfig(): LuaWrapper()
{
    getRoot().setField(X_RESOLUTION, LuaObject(640));
    getRoot().setField(Y_RESOLUTION, LuaObject(480));
    getRoot().setField(FPS, LuaObject(60));
    getRoot().setField(FULLSCREEN, LuaObject(FALSE));
    getRoot().setField(VSYNC, LuaObject(TRUE));
    getRoot().setField(HIGHDPI, LuaObject(FALSE));

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
                builder.append(String::fromInt((int) *(pair.second->getNumber())));
                break;
                
            case LW_INTEGER:
                builder.append(String::fromInt((int) *(pair.second->getInteger())));
                break;
                
            case LW_BOOLEAN:
                builder.append(
                    *(pair.second->getBoolean())?("true"):("false"));
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
    return Vector2di(*(getRoot().getField(X_RESOLUTION).getInteger()),
                     *(getRoot().getField(Y_RESOLUTION).getInteger()));
}


void LuaConfig::setResolution(Vector2di resolution)
{
    getRoot().setField(X_RESOLUTION, LuaObject(resolution.x));
    getRoot().setField(Y_RESOLUTION, LuaObject(resolution.y));
    save();
}


int LuaConfig::getFPS()
{
    return *(getRoot().getField(FPS).getInteger());
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


bool LuaConfig::getHighDPI()
{
    // return (*(getRoot().getField(HIGHDPI).getString())).equals(TRUE);
    return false; // Сейчас сломано по вине SDL
}


void LuaConfig::setFullscreen(bool fullscreen)
{
    getRoot().setField(FULLSCREEN, LuaObject((fullscreen)?(TRUE):(FALSE)));
    save();
}


void LuaConfig::setHighDPI(bool highdpi)
{
    getRoot().setField(HIGHDPI, LuaObject((highdpi)?(TRUE):(FALSE)));
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
