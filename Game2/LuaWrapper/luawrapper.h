#ifndef LUAWRAPPER_H
#define LUAWRAPPER_H

#include "../Strings/stringhash.h"
#include "../Lua/lua.hpp"
#include "../Strings/string.h"
#include "../Streams/streams.h"
#include "../Containers/containers.h"


using namespace coon;

enum LuaTypes { LW_NUMBER, LW_STRING, LW_OBJECT, LW_FUNCTION };


class LuaObject
{
private:
    Map<String, Pointer<LuaObject>> fields;
    Pointer<void> pValue = Pointer<void>(NULL);
    LuaTypes type;
public:
    LuaObject(double d) { setNumber(d); }
    LuaObject(String s) { setString(s); }
    LuaObject() {}
    LuaObject(const LuaObject& obj) { fields = obj.fields; pValue = obj.pValue; type = obj.type; }

    void* getValue() { return pValue.get(); }
    double* getNumber() { return (double*)pValue.get(); }
    String* getString() { return (String*)pValue.get(); }
    void set(Pointer<void> ptr) { pValue = ptr; }
    bool isNull() { return getValue() == NULL; }
    LuaTypes getType() { return type; }
    void setNumber(double d) { set(Pointer<double>(new double(d))); type = LW_NUMBER; }
    void setString(String s) { set(Pointer<String>(new String(s))); type = LW_STRING; }

    void setField(String name, LuaObject value) { fields[name] = Pointer<LuaObject>(new LuaObject(value)); }
    LuaObject getField(String name) { if (fields.count(name) > 0) return *(fields[name]); else return LuaObject(); }
    Map<String, Pointer<LuaObject>>& getFields() { return fields; }
};

class LuaFunction
{
private:
    lua_State* lua;
    Map<String, LuaTypes> signature;
    Vector<String> names;
    int count = 0;
protected:
    LuaObject args;
    void returnNumber(double res) { lua_pushnumber(lua, res); count++; }
    void returnString(const String& res) { lua_pushstring(lua, res.toStdString().c_str()); count++; }
public:
    void init();
    int call(lua_State* lua);

    virtual void execute() = 0;
    virtual String getSignature() = 0;
};


int luaFunctionCaller(lua_State* lua);


class LuaWrapper
{
private:
    lua_State* lua;
    LuaObject root;
    Map<String, Pointer<LuaFunction>> functions;
protected:
    void pushGlobalObject(LuaObject& obj);
    void pullGlobalObject(LuaObject& obj);
public:
    LuaWrapper();
    ~LuaWrapper() { lua_close(lua); }

    lua_State* getLua() { return lua; }
    LuaObject& getRoot() { return root; }
    int execute(TextReader* in, TextWriter* out);
    void registerFunction(Pointer<LuaFunction> function);
    String loadFile(const String& fileName, Map<String, bool> loaded = Map<String, bool>());
    String loadFile(TextReader* in, Map<String, bool> loaded = Map<String, bool>());
    bool hasFunction(const String& name) { return functions.count(name) > 0; }
    LuaFunction* getFunction(const String& name) { return functions[name].get(); }
    void addFunction(const String& name, LuaFunction* function)
        { functions[name] = Pointer<LuaFunction>(function); function->init(); }
    void clear();
    Map<String, Pointer<LuaFunction>>* getFunctions() { return &functions; }
};

LuaWrapper* currentLuaWrapper(LuaWrapper* wrapper = NULL, bool pop = false);


#endif // LUAWRAPPER_H
