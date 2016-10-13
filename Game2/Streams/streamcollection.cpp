#include "streamcollection.h"


StreamCollection::StreamCollection(const String& fileName)
{
    this->fileName = fileName;
}


void StreamCollection::flush()
{
    FileStream fs;
    fs.open(fileName.toStdString(), FileStream::binary | FileStream::out);
    this->AbstractStreamCollection::flush(fs);
    fs.close();
}


void StreamCollection::restore()
{
    FileStream fs;
    fs.open(fileName.toStdString(), FileStream::binary | FileStream::in);
    this->AbstractStreamCollection::restore(fs);
    fs.close();
}



AbstractStreamCollection::AbstractStreamCollection()
{
}


void AbstractStreamCollection::flush(IOStream& fs)
{
    int32_t buffer;

    const char magic[] = SC_MAGIC;
    fs.write(magic, sizeof(magic));

    buffer = (int) records.size();
    fs.write((char*)(&buffer), sizeof(buffer));

    buffer = 0;
    int labelPos = (int) fs.tellp();
    fs.write((char*)(&buffer), sizeof(buffer));

    Map<String, int> offsets;
    for (const auto& pair: records) {
        offsets[pair.first] = (int) fs.tellp();
        for (const char& ch: pair.second) fs.write(&ch, 1);
    }

    buffer = (int) fs.tellp();
    fs.seekp(labelPos, FileStream::beg);
    fs.write((char*)(&buffer), sizeof(buffer));
    fs.seekp(buffer, FileStream::beg);

    for (const auto& pair: offsets) {
        const char* key = pair.first.toStdString().c_str();
        buffer = (int) strlen(key);
        fs.write((char*)(&buffer), sizeof(buffer));
        fs.write(key, sizeof(char) * buffer);

        buffer = pair.second;
        fs.write((char*)(&buffer), sizeof(buffer));

        buffer = (int) records[pair.first].size();
        fs.write((char*)(&buffer), sizeof(buffer));
    }
}


void AbstractStreamCollection::restore(IOStream& fs)
{
    records.clear();
    int32_t buffer;

    const char magic[] = SC_MAGIC;
    char* read = new char[100];
    memset(read, 0, 100);

    fs.read(read, sizeof(magic));
    if (strcmp(magic, read) != 0) {
        std::cout << "AbstractStreamCollection::restore(): corrupt stream \"" <<
                "" << "\"" << std::endl;
        delete[] read;
        return;
    }
    delete[] read;

    fs.read((char*)(&buffer), sizeof(buffer));
    int size = buffer;

    fs.read((char*)(&buffer), sizeof(buffer));
    fs.seekg(buffer, FileStream::beg);

    for (int i = 0; i < size; i++) {
        fs.read((char*)(&buffer), sizeof(buffer));
        char* key = new char[buffer];
        memset(key, 0, buffer);
        fs.read(key, sizeof(char) * buffer);

        String strKey(key);
        records[strKey] = List<char>();
        delete[] key;

        fs.read((char*)(&buffer), sizeof(buffer));
        int offset = buffer;

        fs.read((char*)(&buffer), sizeof(buffer));
        int len = buffer;

        int lastPos = (int) fs.tellg();
        fs.seekg(offset, FileStream::beg);

        for (int i = 0; i < len; i++) {
            char ch;
            fs.read(&ch, 1);
            records[strKey].push_back(ch);
        }

        fs.seekg(lastPos, FileStream::beg);
    }
}


void AbstractStreamCollection::putRecord(const String &key, const char *data, int len)
{
    for (int i = 0; i < len; i++) putRecord(key, data[i]);
}


void AbstractStreamCollection::fillBuffer(const String &key, char *buffer, int len)
{
    List<char>& list = getRecord(key);
    int i = 0;

    for (const char& ch: list) {
        buffer[i] = ch;
        i++;
        if (i > len) break;
    }
}
