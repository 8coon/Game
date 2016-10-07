#include "uifont.h"

UIFont::UIFont(SDL_Renderer* renderer, const String& fileName)
{
    StreamCollection collection(fileName);
    collection.restore();
    texture = new UITexture(renderer, &collection, "IMAGE");

    int len = collection.getRecordLen("DESCR");
    char* buffer = new char[len + 2];
    memset(buffer, 0, len + 2);
    collection.fillBuffer("DESCR", buffer, len);
    String descr(buffer);
    delete[] buffer;

    StringReader reader(descr.toStdString());
    LuaWrapper lua;
    lua.addFunction("font_setLetter", new UIFont_setLetter(this));
    lua.execute(&reader, NULL);
}


void UIFont_setLetter::execute()
{
    Rect pos;
    pos.x = *(args.getField("x").getNumber());
    pos.y = *(args.getField("y").getNumber());
    pos.w = *(args.getField("width").getNumber());
    pos.h = *(args.getField("height").getNumber());

    font->setLetter(*(args.getField("letter").getString()), pos);
}


Rect UIFont::measureText(const String &text)
{
    bool tag = false;
    StringBuilder final;

    for (const Char& ch: text) {
        if (!tag) {
            if (ch.equals(String("&").at(0))) {
                tag = true;
            } else {
                final.append(ch);
            }
        } else {
            // ch is our tag letter
            tag = false;
        }
    }

    String s = final.toString();
    int width = 0;
    int height = 0;
    for (const Char& ch: s) {
        Rect rect = getLetter(String(ch));
        width += rect.w;
        if (rect.h > height) height = rect.h;
    }

    return Rect(0, 0, width, height);
}




