#ifndef STREAMS_H
#define STREAMS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "../Strings/string.h"
#include "../Containers/containers.h"


namespace coon {
    using IStream = std::istream;
    using OStream = std::ostream;
    using FileStream = std::fstream;

    struct Char;
    class String;


    class Reader
    {
    public:
        virtual void read(char* buf, const int len) = 0;
        virtual bool eof() = 0;
        virtual void close() = 0;
    };

    class TextReader: public Reader
    {
    public:
        virtual Char readChar();
        virtual String readLine();
        virtual String readAll();
    };

    class Writer
    {
    public:
        virtual void write(const char* buf, const int len) = 0;
        virtual void close() = 0;
    };

    class TextWriter: public Writer
    {
    public:
        virtual void writeChar(const Char& ch);
        virtual void writeLine(const String& str);
    };


    class TextFileReader: public TextReader
    {
    private:
        bool err = false;
        FileStream fs;
    public:
        TextFileReader(const String& fileName);

        void read(char* buf, const int len);
        bool eof();
        void close();
        bool error() { return err; }
    };


    class TextFileWriter: public TextWriter
    {
    private:
        bool err = false;
        FileStream fs;
    public:
        TextFileWriter(const String& fileName);

        void write(const char *buf, const int len) { fs.write(buf, len); }
        void close() { fs.close(); }
        bool error() { return err; }
    };


    class StringReader: public TextReader
    {
    private:
        char* data = NULL;
        int offset = 0;
        int size = 0;
    public:
        StringReader(const std::string& str);
        ~StringReader() { delete data; }

        void read(char *buf, const int len);
        bool eof() { return offset >= size; }
        void close() {}
    };


    class StringWriter: public TextWriter
    {
    private:
        List<char> data;
    public:
        void write(const char *buf, const int len);
        void close() {}
        int getData(char** buffer);
    };
}


#endif // STREAMS_H
