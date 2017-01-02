function skin_loadTexture(file)
    return internalCall("skin_loadTexture", file)
end


function skin_loadFont(file)
    return internalCall("skin_loadFont", file)
end


function skin_setWidgetTexture(name, markup)
    return internalCall("skin_setWidgetTexture", name, markup.l, markup.t, markup.h,
            markup.w, markup.left, markup.top)
end


function font_setLetter(letter, x, y, width, height)
    return internalCall("font_setLetter", letter, x, y, width, height)
end


#include "Scripts/classes.lua"

