#include "streams.h"

using namespace coon;


Char TextReader::readChar()
{
    char chars[2] = {0, 0};
    read(&(chars[0]), 1);
    if (chars[0] < 0) read(&(chars[1]), 1);
    return Char(*((short*)(chars)));
}


String TextReader::readLine()
{
    Char eol = String("\n").at(0);
    Char buf;
    StringBuilder builder;

    while ((!(buf.equals(eol))) && (!eof())) {
        buf = readChar();
        if (!(buf.equals(eol))) builder.append(buf);
    }

    String result = builder.toString();
    result.replace(Char('\0'), Char('\n'));
    return result;
}


String TextReader::readAll()
{
    StringBuilder builder;

    while (!eof()) {
        builder.append(readLine());
        builder.append(String("\n").at(0));
    }

    return builder.toString();
}


void TextWriter::writeChar(const Char &ch)
{
    char* chars = (char*)(&(ch.a));
    write(&(chars[0]), 1);
    if (chars[0] < 0) write(&(chars[1]), 1);
}


void TextWriter::writeLine(const String &str)
{
    for (Char ch: str) writeChar(ch);
    writeChar(String("\n").at(0));
}



TextFileReader::TextFileReader(const String &fileName)
{
    std::cout << fileName << std::endl;
    fs.open(fileName.toStdString());
    if (!(fs.is_open())) this->err = true;
}


void TextFileReader::close()
{
    fs.close();
}


bool TextFileReader::eof()
{
    return fs.eof() || this->error();
}


void TextFileReader::read(char* buf, const int len)
{
    fs.read(buf, len);
}


TextFileWriter::TextFileWriter(const String &fileName)
{
    try {
        fs.open(fileName.toStdString());
    } catch (std::ios_base::failure& e) {
        this->err = true;
    }
}


StringReader::StringReader(const std::string &str)
{
    data = new char[str.size() + 1];
    for (int i = 0; i < (int)str.size(); i++) {
        data[i] = str[i];
    }
    data[str.size()] = '\0';
    size = (int) str.size();
}


void StringReader::read(char *buf, const int len)
{
    for (int i = offset; i < ((offset + len > size)?(size):(offset + len)); i++)
        buf[i - offset] = data[i];
    offset += len;
}


void StringWriter::write(const char *buf, const int len)
{
    for (int i = 0; i < len; i++) data.push_back(buf[i]);
}


int StringWriter::getData(char **buffer)
{
    *buffer = new char[data.size()];
    int i = 0;

    for (const char& ch: data) {
        (*buffer)[i] = ch;
        i++;
    }

    return i;
}

