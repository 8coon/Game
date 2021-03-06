#ifndef STREAMCOLLECTION_H
#define STREAMCOLLECTION_H

#include "../Streams/streams.h"
#include "../Strings/string.h"
#include "../Strings/stringhash.h"

#define SC_MAGIC "SC 8coon "


using namespace coon;


class AbstractStreamCollection
{
private:
    Map<String, List<char>> records;
public:
    AbstractStreamCollection();

    void restore(IOStream& fs);
    void flush(IOStream& fs);
    void createRecord(const String& key) { records[key] = List<char>(); }
    void putRecord(const String& key, const char* data, int len);
    void putRecord(const String& key, const char data) { records[key].push_back(data); }
    List<char>& getRecord(const String& key) { return records[key]; }
    int getRecordLen(const String& key) { return (int) records[key].size(); }
    void fillBuffer(const String& key, char* buffer, int len);
};


class StreamCollection: public AbstractStreamCollection
{
private:
    String fileName;
public:
    StreamCollection(const String& fileName);
    
    void restore();
    void flush();
};


#endif // STREAMCOLLECTION_H
