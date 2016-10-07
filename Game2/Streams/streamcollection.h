#ifndef STREAMCOLLECTION_H
#define STREAMCOLLECTION_H

#include "../Streams/streams.h"
#include "../Strings/string.h"
#include "../Strings/stringhash.h"

#define SC_MAGIC "SC 8coon "


using namespace coon;


class StreamCollection
{
private:
    String fileName;
    Map<String, List<char>> records;
public:
    StreamCollection(const String& fileName);

    void restore();
    void flush();
    void createRecord(const String& key) { records[key] = List<char>(); }
    void putRecord(const String& key, const char* data, int len);
    void putRecord(const String& key, const char data) { records[key].push_back(data); }
    List<char>& getRecord(const String& key) { return records[key]; }
    int getRecordLen(const String& key) { return records[key].size(); }
    void fillBuffer(const String& key, char* buffer, int len);
};

#endif // STREAMCOLLECTION_H
