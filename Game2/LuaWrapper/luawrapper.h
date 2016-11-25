#ifndef LUAWRAPPER_H
#define LUAWRAPPER_H

#include "../Strings/stringhash.h"
#include "../Lua/lua.hpp"
#include "../Strings/string.h"
#include "../Streams/streams.h"
#include "../Containers/containers.h"
#include "../Containers/exceptions.h"


using namespace coon;

enum LuaTypes { LW_INTEGER, LW_NUMBER, LW_STRING, LW_TABLE, LW_FUNCTION,
                LW_NIL, LW_BOOLEAN };


class LuaObject
{
private:
    Map<String, Pointer<LuaObject>> fields;
    Pointer<void> pValue = Pointer<void>(NULL);
    LuaTypes type = LW_TABLE;
public:
    LuaObject(double d) { setNumber(d); }
    LuaObject(String s) { setString(s); }
    LuaObject() {}
    LuaObject(const LuaObject& obj) { fields = obj.fields; pValue = obj.pValue; type = obj.type; }
    LuaObject(int i) { setInteger(i); }
    LuaObject(bool b) {}
    
    static LuaObject fromFunction(int i) {
        LuaObject res = LuaObject(i);
        res.type = LW_FUNCTION;
        return res;
    }
    
    static LuaObject fromNil() {
        LuaObject res = LuaObject();
        res.type = LW_NIL;
        return res;
    }
    
    void clear() { fields.clear(); pValue = Pointer<void>(NULL); }

    void* getValue() { return pValue.get(); }
    double* getNumber() { return (double*)pValue.get(); }
    String* getString() { return (String*)pValue.get(); }
    int* getInteger() { return (int*)pValue.get(); }
    bool* getBoolean() { return (bool*)pValue.get(); }
    int getFuncIdx() { return *(int*)pValue.get(); }
    
    void set(Pointer<void> ptr) { pValue = ptr; }
    bool isNull() { return getValue() == NULL; }
    LuaTypes getType() { return type; }
    
    void setNumber(double d) { set(Pointer<double>(new double(d))); type = LW_NUMBER; }
    void setString(String s) { set(Pointer<String>(new String(s))); type = LW_STRING; }
    void setInteger(int i) { set(Pointer<int>(new int(i))); type = LW_INTEGER; }
    void setBoolean(bool b) { set(Pointer<bool>(new bool(b))); type = LW_BOOLEAN; }

    void setField(String name, LuaObject value) { fields[name] = Pointer<LuaObject>(new LuaObject(value)); }
    LuaObject getField(String name) { if (fields.count(name) > 0) return *(fields[name]); else return LuaObject(); }
    Map<String, Pointer<LuaObject>>& getFields() { return fields; }
};


class LuaWrapper;


class LuaFunction
{
private:
    lua_State* lua;
    Map<String, LuaTypes> signature;
    Vector<String> names;
    LuaWrapper* wrapper;
    int count = 0;
protected:
    LuaObject args;
    void returnNumber(double res) { lua_pushnumber(lua, res); count++; }
    void returnString(const String& res) { lua_pushstring(lua, res.toStdString().c_str()); count++; }
public:
    LuaFunction(LuaWrapper* wrapper) { this->wrapper = wrapper; }
    
    void init();
    int call(lua_State* lua);
    
    LuaWrapper* getWrapper() { return wrapper; }

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
public:
    void pushGlobalObject(LuaObject& obj, int stack_idx = 0);
    void pullGlobalObject(LuaObject& obj, int stack_idx = 0);
    void leaveFunctionBinding(int idx);
    void pushValue(LuaObject& value);
    LuaObject pullValue(int val_idx = -1);

    LuaWrapper();
    ~LuaWrapper() { lua_close(lua); }
    
    LuaObject callLuaFunction(LuaObject function, Vector<LuaObject> args,
                              int* luaErr = NULL);

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


std::ostream& operator<<(std::ostream& stream, LuaObject& obj);

#endif // LUAWRAPPER_H
