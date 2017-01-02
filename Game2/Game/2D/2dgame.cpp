//
//  2dgame.cpp
//  Game2
//
//  Created by Сергей Пешков on 02.01.17.
//  Copyright © 2017 8coon. All rights reserved.
//

#include "2dgame.h"


C2DGame::C2DGame(String mainFile)
{
    config = Pointer<LuaConfig>(new LuaConfig());
    window = Pointer<GLWindow>(new GLWindow(config.get()));
    lua = Pointer<LuaWrapper>(new LuaWrapper());
    
    TextFileReader reader(mainFile);
    lua->execute(&reader, NULL);
    
    reader.close();
}
