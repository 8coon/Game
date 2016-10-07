#include "luawrapper.h"

LuaWrapper::LuaWrapper()
{
    lua = luaL_newstate();
    luaopen_base(lua);
}


void LuaWrapper::pushGlobalObject(LuaObject &obj)
{
    for (auto& pair: obj.getFields()) {
        switch (pair.second->getType()) {
            case LW_NUMBER:
                lua_pushnumber(lua, *(pair.second->getNumber()));
                lua_setglobal(lua, pair.first.toStdString().c_str());
                break;
            case LW_STRING:
                lua_pushstring(lua, pair.second->getString()->toStdString().c_str());
                lua_setglobal(lua, pair.first.toStdString().c_str());
                break;
            default: break;
        }
    }
}


void LuaWrapper::pullGlobalObject(LuaObject &obj)
{
    for (auto& pair: obj.getFields()) {
        switch (pair.second->getType()) {
            case LW_NUMBER:
                lua_getglobal(lua, pair.first.toStdString().c_str());
                obj.setField(pair.first, LuaObject(lua_tonumber(lua, -1)));
                break;
            case LW_STRING:
                lua_getglobal(lua, pair.first.toStdString().c_str());
                obj.setField(pair.first, LuaObject(lua_tostring(lua, -1)));
                break;
            default: break;
        }
    }
}


void LuaFunction::init()
{
    String s = getSignature();
    Vector<String> pairs = s.split(",");

    for (String value: pairs) {
        value.replace("[%s]+", " ", true);
        if (value.startsWith(" ")) value = value.substring(1);
        Vector<String> pair = value.split(" ");
        signature[pair[1]] = (pair[0] == "number")?(LW_NUMBER):(LW_STRING);
        names.push_back(pair[1]);
    }
}


int LuaFunction::call(lua_State *lua)
{
    int i = 1;

    for (const String& name: names) {
        i++;

        switch (signature[name]) {
            case LW_NUMBER:
                args.setField(name, LuaObject(lua_tonumber(lua, i)));
                break;
            case LW_STRING:
                args.setField(name, LuaObject(lua_tostring(lua, i)));
                break;
            default: break;
        }
    }

    execute();
    int c = count;
    count = 0;
    return c;
}


LuaWrapper* currentLuaWrapper(LuaWrapper* wrapper, bool pop)
{
    static Stack<LuaWrapper*> wrappers;
    if (pop) wrappers.pop(); else {
        if (wrapper != NULL) wrappers.push(wrapper); else {
            if (wrappers.size() > 0) return wrappers.top();
        }
    }
    return NULL;
}


int luaFunctionCaller(lua_State* lua)
{
    if (lua != NULL) {
        String func(lua_tostring(lua, 1));

        #ifdef LUA_LOG
            std::cout << "called " << func << " on " << currentLuaWrapper() << std::endl;
        #endif

        if (currentLuaWrapper()->hasFunction(func)) {
            return currentLuaWrapper()->getFunction(func)->call(currentLuaWrapper()->getLua());
        } else {
            std::cout << "Failed to call " << func << std::endl;
        }

        return 0;
    } else {
        #ifdef LUA_LOG
            std::cout << "called NULL" << std::endl;
        #endif
        return 0;
    }
}


String LuaWrapper::loadFile(const String &fileName, Map<String, bool> loaded)
{
    TextFileReader in(fileName);
    if (in.error()) return String();
    return loadFile(&in, loaded);
}


String LuaWrapper::loadFile(TextReader *in, Map<String, bool> loaded)
{
    StringBuilder builder;
    bool first = true;

    while (!in->eof()) {
        String rawline = (first)?("#include \"Scripts/system.lua\""):(in->readLine());
        first = false;

        if (rawline.size() > 0) {
            String line = rawline;
            line.replace("[%s]+", "", true);

            if (line.startsWith("#include")) {
                line.replace("#include\"", "");
                line.replace("\"", "");

                if (loaded.count(line) < 1) {
                    loaded[line] = true;
                    builder.append(loadFile(line, loaded));
                    builder.append(String("\n"));
                }
            } else {
                builder.append(rawline);
                builder.append(String("\n"));
            }
        } else {
            builder.append(String("\n"));
        }
    }

    return builder.toString();
}


int LuaWrapper::execute(TextReader *in, TextWriter *out)
{
    clear();

    pushGlobalObject(root);
    currentLuaWrapper(this);

    String buff = loadFile(in);
    int error = luaL_loadstring(lua, buff.toStdString().c_str()) || lua_pcall(lua, 0, LUA_MULTRET, 0);
    pullGlobalObject(root);
    currentLuaWrapper(NULL, true);

    if (error != 0) std::cout << "Failed to run lua script: " << lua_tostring(lua, -1) << std::endl;
    return error;
}


void LuaWrapper::clear()
{
    lua_close(lua);
    lua = luaL_newstate();
    luaopen_base(lua);
    luaopen_string(lua);
    luaopen_math(lua);
    luaopen_table(lua);

    lua_pushcfunction(lua, luaFunctionCaller);
    lua_setglobal(lua, "internalCall");
}
