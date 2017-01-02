//
//  2dgame.hpp
//  Game2
//
//  Created by Сергей Пешков on 02.01.17.
//  Copyright © 2017 8coon. All rights reserved.
//

#ifndef _dgame_hpp
#define _dgame_hpp


#include <iostream>
#include "common.h"
#include "../../Containers/containers.h"
#include "../../Strings/string.h"
#include "../../Strings/stringhash.h"
#include "../../Streams/streams.h"
#include "../../LuaWrapper/luaconfig.h"
#include "../../LuaWrapper/luawrapper.h"
#include "../../GL/glwindow.h"
#include "../../GL/UI/uitexture.h"
#include "../../GL/UI/uiskin.h"
#include "../../GL/UI/Widgets/uiscene.h"
#include "../../GL/glcontext.h"
#include "../../GL/gltexture.h"


using namespace coon;


class C2DGame
{
private:
    Pointer<LuaConfig> config;
    Pointer<GLWindow> window;
    Pointer<LuaWrapper> lua;
public:
    C2DGame(String mainFile);
    ~C2DGame() {}
    
    LuaConfig* getConfig() { return config.get(); }
    GLWindow* getWindow() { return window.get(); }
};



#endif /* _dgame_hpp */
