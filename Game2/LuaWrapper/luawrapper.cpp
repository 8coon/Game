#include "luawrapper.h"

LuaWrapper::LuaWrapper()
{
    lua = luaL_newstate();
    luaopen_base(lua);
}


LuaObject LuaWrapper::callLuaFunction(LuaObject function,
                                      Vector<LuaObject> args, int* luaErr)
{
    leaveFunctionBinding(*(function.getInteger()));
    
    for (LuaObject& arg: args) {
        pushValue(arg);
    }
    
    int err = lua_pcall(lua, (int) args.size(), 1, 0);
    if (luaErr != NULL) *luaErr = err;
    
    return pullValue();
}


void LuaWrapper::pushValue(LuaObject& value)
{
    switch (value.getType()) {
        case LW_NIL:
            lua_pushnil(lua);
            break;
            
        case LW_BOOLEAN: {
            int b = 0;
            if (value.getBoolean()) b = 1;
            lua_pushboolean(lua, b);
            
            } break;
            
        case LW_TABLE: {
            lua_newtable(lua);
            int tbl_idx = lua_gettop(lua);
            pushGlobalObject(value, tbl_idx);
            
            } break;
            
        case LW_NUMBER:
            lua_pushnumber(lua, *(value.getNumber()));
            break;
            
        case LW_INTEGER:
            lua_pushinteger(lua, *(value.getInteger()));
            break;
            
        case LW_STRING:
            lua_pushstring(lua, value.getString()->toStdString().c_str());
            break;
            
        case LW_FUNCTION:
            leaveFunctionBinding(*(value.getInteger()));
            break;
            
        default: break;
    }
}


void LuaWrapper::leaveFunctionBinding(int idx)
{
    lua_getglobal(lua, "has_function_binding");
    lua_pushinteger(lua, idx);
    
    if (lua_pcall(lua, 1, 1, 0) != 0)
        throw RuntimeException("Failed to check Lua function binging!");
    
    int has_binding = (int) lua_toboolean(lua, -1);
    lua_pop(lua, -1);
    
    if (has_binding != 0) {
        lua_getglobal(lua, "get_function_binding");
        lua_pushinteger(lua, idx);
        
        if (lua_pcall(lua, 1, 1, 0) != 0)
            throw RuntimeException("Failed to get Lua function binging!");
    } else {
        lua_pushnil(lua);  // Something should always be on top of the stack
    }
}


void LuaWrapper::pushGlobalObject(LuaObject &obj, int idx)
{
    bool should_pop = false;
    
    if (idx == 0) {
        lua_pushglobaltable(lua);
        idx = (int) lua_gettop(lua);
        should_pop = true;
    }
    
    for (auto& pair: obj.getFields()) {
        const char* key = pair.first.toStdString().c_str();
        
        pushValue(*(pair.second.get()));
        lua_setfield(lua, idx, key);
    }
    
    if (should_pop) lua_pop(lua, -1);
}


LuaObject LuaWrapper::pullValue(int val_idx)
{
    switch (lua_type(lua, val_idx)) {
        case LUA_TNIL:
            return LuaObject::fromNil();
            
        case LUA_TBOOLEAN: {
            int val = lua_toboolean(lua, val_idx);
            
            if (val != 0) {
                return LuaObject(true);
            } else {
                return LuaObject(false);
            }
            
        }
            
        case LUA_TNUMBER: {
            LuaObject val = LuaObject(lua_tonumber(lua, val_idx));
            
            if (lua_isinteger(lua, val_idx)) {
                val = LuaObject((int) lua_tointeger(lua, val_idx));
            }
            return val;
            
        }
            
        case LUA_TSTRING: {
            const char* s = lua_tostring(lua, val_idx);
            return LuaObject(String(s));
        }
            
        case LUA_TTABLE: {
            LuaObject val = LuaObject();
            pullGlobalObject(val, val_idx);
            lua_pushnil(lua);
            return val;
            
        }
            
        case LUA_TFUNCTION: {
            lua_getglobal(lua, "create_function_binding");
            lua_pushvalue(lua, val_idx);
            
            if (lua_pcall(lua, 1, 1, 0) != 0)
                throw RuntimeException("Failed to create Lua function binging!");
            
            int func_idx = (int) lua_tointeger(lua, -1);
            LuaObject val = LuaObject::fromFunction(func_idx);
            return val;
            
        }
            
            /* ToDo: Do this stuff probably maybe */
        case LUA_TUSERDATA: return LuaObject::fromNil();
        case LUA_TLIGHTUSERDATA: return LuaObject::fromNil();
        case LUA_TTHREAD: return LuaObject::fromNil();
            
        default:
            return LuaObject::fromNil();
    }
}


void LuaWrapper::pullGlobalObject(LuaObject &obj, int idx)
{
    bool should_pop = false;
    
    if (idx == 0) {
        lua_pushglobaltable(lua);
        idx = (int) lua_gettop(lua);
        should_pop = true;
    }
    
    obj.clear();
    lua_pushnil(lua);
    
    while (lua_next(lua, idx) != 0) {
        // Key is at index - 2 and value is at index - 1
        
        int key_idx = lua_gettop(lua) - 1;
        int val_idx = key_idx + 1;
        
        lua_pushvalue(lua, key_idx);
        String key = lua_tostring(lua, -1);
        std::cout << key << std::endl;
        lua_pop(lua, -1);
        
        obj.setField(key, pullValue(val_idx));
        lua_pop(lua, -1);
    }
    
    if (should_pop) lua_pop(lua, -1);
}


/*void LuaWrapper::pullGlobalObject__(LuaObject &obj, int idx)
{
    for (auto& pair: obj.getFields()) {
        switch (pair.second->getType()) {
            case LW_NUMBER:
                lua_getfield(lua, idx, pair.first.toStdString().c_str());
                obj.setField(pair.first, LuaObject(lua_tonumber(lua, -1)));
                break;
            case LW_STRING:
                lua_getfield(lua, idx, pair.first.toStdString().c_str());
                obj.setField(pair.first, LuaObject(lua_tostring(lua, -1)));
                break;
            case LW_INTEGER:
                lua_getfield(lua, idx, pair.first.toStdString().c_str());
                obj.setField(pair.first,
                             LuaObject((int)lua_tointeger(lua, -1)));
                break;
                
            case LW_FUNCTION: {
                
                lua_getglobal(lua, "create_function_binding");
                lua_getfield(lua, idx, pair.first.toStdString().c_str());
                
                if (lua_pcall(lua, 1, 1, 0) != 0)
                    throw RuntimeException("Failed to create Lua function binging!");
                
                int func_idx = (int) lua_tointeger(lua, -1);
                obj.setField(pair.first, LuaObject::fromFunction(func_idx));
                lua_pop(lua, -1);
                
                } break;
                
            case LW_TABLE: {
                
                lua_getfield(lua, idx, pair.first.toStdString().c_str());
                int table_idx = lua_gettop(lua);
            
                LuaObject obj;// = LuaObject::fromTable();
                lua_pushnil(lua);  // First key
                
                while (lua_next(lua, table_idx) != 0) {
                    // Key is at index - 2 and value is at index - 1
                    
                }
                
                
                } break;
        
            default: break;
        }
    }
}*/


void LuaFunction::init()
{
    String s = getSignature();
    Vector<String> pairs = s.split(",");

    for (String value: pairs) {
        value.replace("[%s]+", " ", true);
        if (value.startsWith(" ")) value = value.substring(1);
        Vector<String> pair = value.split(" ");
        //signature[pair[1]] = (pair[0] == "number")?(LW_NUMBER):(LW_STRING);
        
        signature[pair[1]] = LW_STRING;
        
        if (pair[0] == "number") {
            signature[pair[1]] = LW_NUMBER;
        } else if ((pair[0] == "int") || (pair[0] == "integer")) {
            signature[pair[1]] = LW_INTEGER;
        } else if (pair[0] == "string") {
            signature[pair[1]] = LW_STRING;
        } else if ((pair[0] == "table") || (pair[0] == "object")) {
            signature[pair[1]] = LW_TABLE;
        } else if (pair[0] == "function") {
            signature[pair[1]] = LW_FUNCTION;
        }
        
        names.push_back(pair[1]);
    }
}


int LuaFunction::call(lua_State *lua)
{
    int i = lua_gettop(lua) - (int) names.size();

    for (const String& name: names) {
        i++;
        
        LuaObject val = getWrapper()->pullValue(i);
        args.setField(name, val);
        
        #ifdef LUA_LOG
            std::cout << name << " = " << val << std::endl;
        #endif
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
    
    if (error != 0) {
        std::cout << "Failed to run lua script: " << lua_tostring(lua, -1) << std::endl;
        return error;
    }
    
    for (auto& pair: root.getFields()) {
        lua_getglobal(lua, pair.first.toStdString().c_str());
        LuaObject val = pullValue(lua_gettop(lua));
        root.setField(pair.first, val);
        
        #ifdef LUA_LOG
            std::cout << pair.first << " = " << val << std::endl;
        #endif
        
        lua_pop(lua, -1);
    }
    
    //pullGlobalObject(root);
    currentLuaWrapper(NULL, true);
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



std::ostream& operator<<(std::ostream& stream, LuaObject& obj)
{
    switch (obj.getType()) {
        case LW_NIL: stream << "NULL"; break;
        case LW_INTEGER: stream << *(obj.getInteger()); break;
        case LW_TABLE: stream << "[Table@" << *(obj.getInteger()); break;
        case LW_NUMBER: stream << *(obj.getNumber()); break;
        case LW_STRING: stream << '"' << *(obj.getString()) << '"'; break;
        case LW_BOOLEAN: stream << ((*obj.getBoolean())?("true"):("false")); break;
        case LW_FUNCTION: stream << "[Function@" << *(obj.getInteger()); break;
        default: break;
    }
    
    return stream;
}

