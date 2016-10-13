#ifndef STRING_H
#define STRING_H

#include <string.h>
#include <string>
#include <sstream>
#include "../Containers/containers.h"
#include "../Streams/streams.h"

#define REGEXP_DEBUG(A) /*(std::cout << "debug " << A << std::endl)*/ true


namespace coon {

struct Char {
    unsigned short a;
    Char(const short ch = '\0') { a = ch; }
    Char(const Char& ch) { a = ch.a; }
    char toChar() const { return a; }
    bool equals(const Char& another) const { return a == another.a; }
    bool equalsIgnoreCase(const Char& another) const;
    Char toLower() const;
    Char toUpper() const;
    bool isLatin() const;
    bool isCyrlic() const;
};

struct RegexpRange
{
    unsigned short min;
    unsigned short max;
    RegexpRange(unsigned short min=0, unsigned short max=USHRT_MAX) { this->min = min; this->max = max; }
};

typedef RegexpRange ShortRange;


class CharHelper
{
public:
    ShortRange latinCaps;
    ShortRange latinOrds;
    ShortRange cyrlicCaps;
    ShortRange cyrlicOrds;
    CharHelper();
};

static CharHelper charHelper;


class String
{
private:
    Vector<Char> chars;
public:
    String();
    String(const char* ch);
    String(Char ch);
    String(List<Char> &chars);
    String(Vector<Char> &chars);
    String(const String& str) { chars = str.chars; }

    static String fromInt(int n) { char b[255]; sprintf(b, "%d", n); return String(b); }
    static String fromDouble(double n) { char b[255]; sprintf(b, "%f", n); return String(b); }
    static String fromBool(bool b) { return ((b)?(String("True")):("False")); }

    int toInt() { return atoi(toStdString().c_str()); }
    double toDouble() { return atof(toStdString().c_str()); }
    bool toBool() { return equalsIgnoreCase("True"); }

    size_t size() const { return chars.size(); }
    void append(const Char ch);
    void append(const String& str);
    void insert(int pos, const String& str);
    void remove(int start, int end = -1);
    String substring(int start, int end = -1);
    int replace(const Char what, const Char with, int count = -1);
    int replace(const String& what, const String& with, const bool regexp = false, const int count = -1);
    int find(const String& what, int offset = 0, const bool regexp = false, int* end = NULL);
    Vector<String> split(const String& delim, int count = -1);

    Vector<Char>::const_iterator begin() const { return chars.begin(); }
    Vector<Char>::const_iterator end() const { return chars.end(); }

    bool isNull() { return size() == 0; }
    const Char& at(int index) const { return chars.at(index); }
    void set(int index, Char value) { chars[index] = value; }
    bool equals(const String& another) const;
    bool equalsIgnoreCase(const String &another) const;
    void toLower(); // Broken
    void toUpper(); // Broken
    std::string toStdString() const;
    bool startsWith(String str) const;
    bool endsWith(String) const;
    void reverse();
    void clear() { chars.clear(); }
    void echo();

    bool operator==(const String& another) const { return this->equals(another); }
};


/*class StringBuilder
{
private:
    List<Char> chars;
public:
    size_t size() const { return chars.size(); }
    void append(const Char ch) { chars.push_back(ch); }
    void append(const String& str);
    void clear() { chars.clear(); }

    String toString();
};*/
    
class StringBuilder
{
private:
    List<String> strings;
    int len = 0;
public:
    size_t size() const { return len; }
    void append(const Char ch) { strings.push_back(String(ch)); len += 1; }
    void append(const String& str) { strings.push_back(str); len += str.size(); }
    void clear() { strings.clear(); len = 0; }
    
    String toString();
};



String operator+(const String& lhs, const String& rhs);


struct RegexpGroup
{
    List<RegexpRange> list;
    bool one = true;
    void addRange(RegexpRange range) { list.push_back(range); }
};


class Regexp
{
private:
    Map<int, RegexpGroup> groups;
    String pattern;
public:
    Regexp(const String& str);
    bool matches(const String& str);
};



std::ostream& operator<<(std::ostream& stream, const String& str);

}
#endif // STRING_H
