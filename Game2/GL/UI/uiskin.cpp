#include "uiskin.h"


void UISkin_loadTexture::execute()
{
    skin->loadTexture(*(args.getField("fileName").getString()));
}


void UISkin_setWidgetTexture::execute()
{
    Rect pos;
    pos.x = *(args.getField("l").getInteger());
    pos.y = *(args.getField("t").getInteger());
    pos.h = *(args.getField("top").getInteger());
    pos.w = *(args.getField("left").getInteger());

    Point size;
    size.x = *(args.getField("w").getInteger());
    size.y = *(args.getField("h").getInteger());

    UIWidgetTexture texture(skin->getTexture(), pos, size);
    skin->setWidgetTexture(*(args.getField("name").getString()), texture);
}


void UISkin_loadFont::execute()
{
    skin->loadFont(*(args.getField("fileName").getString()));
}



UISkin::UISkin(SDL_Renderer* renderer, const String& fileName)
{
    LuaWrapper lua;
    TextFileReader reader(fileName);
    this->renderer = renderer;

    lua.addFunction("skin_loadTexture", new UISkin_loadTexture(this, &lua));
    lua.addFunction("skin_setWidgetTexture", new UISkin_setWidgetTexture(this, &lua));
    lua.addFunction("skin_loadFont", new UISkin_loadFont(this, &lua));

    lua.execute(&reader, NULL);
}


UIWidgetTexture UISkin::getWidgetTexture(const String &name)
{
    if (widgets.count(name) > 0) {
        return widgets[name];
    } else {
        std::cout << "Unable to find widget markup for " << name << std::endl;
        return UIWidgetTexture();
    }
}
