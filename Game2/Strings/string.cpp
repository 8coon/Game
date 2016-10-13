#include "string.h"

using namespace coon;


CharHelper::CharHelper()
{
    latinCaps.min = String("A").at(0).a;
    latinCaps.max = String("Z").at(0).a;
    latinOrds.min = String("a").at(0).a;
    latinOrds.max = String("z").at(0).a;
    cyrlicCaps.min = String("А").at(0).a;
    cyrlicCaps.max = String("Я").at(0).a;
    cyrlicOrds.min = String("а").at(0).a;
    cyrlicOrds.max = String("я").at(0).a;
}


bool Char::isLatin() const
{
    return (((a >= charHelper.latinCaps.min) && (a <= charHelper.latinCaps.max)) ||
            ((a >= charHelper.latinOrds.min) && (a <= charHelper.latinOrds.max)));
}


bool Char::isCyrlic() const
{
    return (((a >= charHelper.cyrlicCaps.min) && (a <= charHelper.cyrlicCaps.max)) ||
            ((a >= charHelper.cyrlicOrds.min) && (a <= charHelper.cyrlicOrds.max)));
}


Char Char::toLower() const
{
    Char res(a);

    if (isLatin()) {
        if (a < charHelper.latinOrds.min)
            res.a += charHelper.latinOrds.min - charHelper.latinCaps.min;
    } else if (isCyrlic()) {
        if (a < charHelper.cyrlicOrds.min)
            res.a += charHelper.cyrlicOrds.min - charHelper.cyrlicCaps.min;
    }

    return res;
}


Char Char::toUpper() const
{
    Char res = toLower();

    if (res.isLatin()) {
        res.a -= charHelper.latinOrds.min - charHelper.latinCaps.min;
    } else if (res.isCyrlic()) {
        res.a -= charHelper.cyrlicOrds.min - charHelper.cyrlicCaps.min;
    }

    return res;
}


bool Char::equalsIgnoreCase(const Char& another) const
{
    return toLower().equals(another.toLower());
}


String::String()
{
}


String::String(Char ch)
{
    chars.push_back(ch);
}


String::String(const char *ch)
{
    if (ch != NULL) {
        int len = (int) strlen(ch);
        for (int i = 0; i < len; i++) {
            Char cha;

            if (ch[i] < 0) {
                cha = Char(*(short*)(&ch[i]));
                i++;
            } else {
                cha = Char(ch[i]);
            }

            if (cha.a != 0)
                chars.push_back(cha);
        }
    }
}


String::String(List<Char>& chars)
{
    this->chars.resize(chars.size());
    int i = 0;
    for (Char ch: chars) { this->chars[i] = ch; i++; }
}


void String::append(const Char ch)
{
    chars.push_back(ch);
}


void String::append(const String &str)
{
    for (Char ch: str) { append(ch); }
}


String String::substring(int start, int end)
{
    if (start < 0) start = (int) size();
    if (end < 0) end = (int) size();
    start %= size() + 1;
    end %= size() + 1;

    String res;
    for (int i = start; i < end; res.append(chars[i]), i++);
    return res;
}


void String::insert(int pos, const String &str)
{
    if (pos < 0) pos = (int) size();
    pos %= size() + 1;
    String beg = substring(0, pos);
    String end = substring(pos);

    StringBuilder builder;
    builder.append(beg);
    builder.append(str);
    builder.append(end);
    chars = builder.toString().chars;
}



void String::remove(int start, int end)
{
    if (start < 0) start = (int) size();
    if (end < 0) end = (int) size();
    start %= size() + 1;
    end %= size() + 1;

    String beg = substring(0, start);
    String stop = substring(end);

    StringBuilder builder;
    builder.append(beg);
    builder.append(stop);
    chars = builder.toString().chars;
}


int String::replace(const Char what, const Char with, int count)
{
    int replaced = 0;
    StringBuilder builder;

    for (Char ch: chars) {
        if (what.equals(ch) && ((replaced < count) || count < 0)) {
            builder.append(with);
            replaced++;
        } else {
            builder.append(ch);
        }
    }

    chars = builder.toString().chars;
    return replaced;
}


int String::replace(const String& what, const String& with, const bool regexp, const int count)
{
    int replaced = 0;

    if ((count >= 1) || (count < 0)) {
        int end;
        int start = find(what, 0, regexp, &end);
        if (start != -1) {
            String beg = substring(0, start);
            String stop = substring(end);
            replaced += 1 + stop.replace(what, with, regexp, count - 1);

            StringBuilder builder;
            builder.append(beg);
            builder.append(with);
            builder.append(stop);
            chars = builder.toString().chars;
        }
    }

    return replaced;
}


int String::find(const String &what, int offset, const bool regexp, int *end)
{
    if (offset > (int)size()) offset = 0;
    if (offset < 0) offset = (int)size();

    for (int i = offset; i < (int)size(); i++) {
        bool found = false;
        bool prevFound = false;

        if (!regexp) {
            for (int j = 0; j < (int)what.size(); j++) {
                if (!(at(i).equals(what.at(j)))) break;
                found = true;
            }

            if (found) {
                if (end != NULL) *end = i + (int) what.size();
                return i;
            }
        } else {
            Regexp re((regexp)?(what):(""));

            for (int j = i + 1; j <= (int)size(); j++) {
                String temp = substring(i, j);
                prevFound = found;
                found = re.matches(temp);
                if (found) {
                    if (end != NULL) *end = j;
                } else {
                    if (prevFound) break;
                }
            }
        }

        if ((prevFound) || (found)) {
            return i;
        }
    }

    return -1;
}


Vector<String> String::split(const String &delim, int count)
{
    int replaced = 0;
    Vector<String> result;
    StringBuilder line;

    for (int i = 0; i < (int)(size()); i++) {
        if (substring(i, i + (int) delim.size()).equals(delim)) {
            result.push_back(line.toString());
            i += delim.size() - 1;
            line.clear();
            replaced++;

            if (replaced == count) {
                result.push_back(substring(i));
                break;
            }
        } else {
            line.append(at(i));
        }
    }

    result.push_back(line.toString());
    return result;
}


bool String::equals(const String &another) const
{
    if (size() == another.size()) {
        for (int i = 0; i < (int)size(); i++) {
            if (!(at(i).equals(another.at(i)))) return false;
        }

        return true;
    }

    return false;
}


bool String::equalsIgnoreCase(const String &another) const
{
    if (size() == another.size()) {
        for (int i = 0; i < (int)size(); i++) {
            if (!(at(i).equalsIgnoreCase(another.at(i)))) return false;
        }

        return true;
    }

    return false;
}


void String::toLower()
{
    for (int i = 0; i < (int)size(); set(i, at(i).toLower()), i++);
}


void String::toUpper()
{
    for (int i = 0; i < (int)size(); set(i, at(i).toUpper()), i++);
}


std::string String::toStdString() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}


void String::reverse()
{
    StringBuilder builder;
    for (int i = (int)size() - 1; i >= 0; i--) builder.append(at(i));
    clear();
    chars = builder.toString().chars;
}


bool String::startsWith(String str) const
{
    StringBuilder builder;
    str.echo();
    builder.append(str);
    builder.append("*");

    Regexp re = builder.toString();
    return re.matches(*this);
}


bool String::endsWith(String str) const
{
    String s = *this;
    s.reverse();
    str.reverse();
    return s.startsWith(str);
}


void String::echo()
{
    replace("%", "%%");
    replace("[", "%[");
    replace("]", "%]");
    replace(".", "%.");
    replace("*", "%*");
}



String::String(Vector<Char> &chars)
{
    this->chars.resize(chars.size());
    int i = 0;
    for (Char ch: chars) { this->chars[i] = ch; i++; }
}


String StringBuilder::toString()
{
    Vector<Char> chars;
    chars.reserve(size());
    
    for (const String& str: strings) {
        for (const Char& ch: str) {
            chars.push_back(ch);
        }
    }
    
    return String(chars);
}


Regexp::Regexp(const String &str)
{
    bool escape = false;
    bool group_opened = false;
    StringBuilder builder;
    RegexpGroup current;

    for (int i = 0; i < (int)str.size(); i++) {
        if ((str.at(i).equals('%')) && (!escape)) {
            escape = true;
        } else {

            if (escape) {
                if (str.at(i).equals('w')) {
                    current.addRange(RegexpRange('A', 'Z'));
                    current.addRange(RegexpRange('a', 'z'));
                    current.addRange(RegexpRange(String("А").at(0).a, String("Я").at(0).a));
                    current.addRange(RegexpRange(String("а").at(0).a, String("я").at(0).a));
                } else if (str.at(i).equals('i')) {
                    current.addRange(RegexpRange('0', '9'));
                } else if (str.at(i).equals('s')) {
                    current.addRange(RegexpRange(' ', ' '));
                    current.addRange(RegexpRange('\t', '\t'));
                    current.addRange(RegexpRange('\r', '\r'));
                } else {
                    if (group_opened) {
                        current.addRange(RegexpRange(str.at(i).a, str.at(i).a));
                    } else {
                        builder.append(str.at(i));
                    }
                }

                escape = false;
            } else {
                if (str.at(i).equals('.')) {
                    RegexpGroup group;
                    group.addRange(RegexpRange());
                    group.one = true;
                    groups[(int) builder.size()] = group;
                    builder.append(String('\0'));

                } else if (str.at(i).equals('*')) {
                    RegexpGroup group;
                    group.addRange(RegexpRange());
                    group.one = false;
                    groups[(int) builder.size()] = group;
                    builder.append(String('\0'));

                } else if ((str.at(i).equals('[')) && (!group_opened)) {
                    group_opened = true;
                    current.list.clear();
                    current.one = true;

                } else if ((str.at(i).equals(']')) && (group_opened)) {
                    if ((i + 1 < (int)str.size()) && (str.at(i + 1).equals('+'))) {
                        i++;
                        current.one = false;
                    }
                    groups[(int) builder.size()] = current;
                    builder.append(String('\0'));
                    group_opened = false;

                } else if (group_opened) {
                    current.addRange(RegexpRange(str.at(i).a, str.at(i).a));

                } else {
                    builder.append(str.at(i));
                }
            }
        }
    }

    pattern = builder.toString();
}


bool Regexp::matches(const String &str)
{
    int strpos = 0;
    for (int i = 0; i < (int)pattern.size(); i++) {
        if (groups.count(i) > 0) {
            RegexpGroup group = groups[i];
            bool matching = true;

            while (matching) {
                matching = false;
                for (RegexpRange range: group.list) {
                    matching = ((str.at(strpos).a >= range.min) && (str.at(strpos).a <= range.max));
                    if (matching) break;
                }

                if ((group.one) && (!matching)) return false;

                strpos++;
                if (!matching) strpos--;
                if (strpos >= (int)str.size()) {
                    if (i + 1 >= (int)pattern.size()) {
                        REGEXP_DEBUG(4);
                        return matching;
                    } else {
                        REGEXP_DEBUG(5);
                        return false;
                    }
                }

                if (group.one) matching = false;
            }

        } else {
            if (!(str.at(strpos).equals(pattern.at(i)))) {
                REGEXP_DEBUG(3);
                return false;
            }
            strpos++;

            if (strpos == (int)str.size()) {
                REGEXP_DEBUG(2);
                return true;
            }
        }
    }

    REGEXP_DEBUG(1);
    return (strpos == (int)str.size());
}


std::ostream& coon::operator<<(std::ostream& stream, const String& str) {
    for (Char ch: str) {
        char cha[2];
        cha[0] = ((char*)(&ch.a))[0];
        cha[1] = ((char*)(&ch.a))[1];

        if (cha[0] != 0) {
            stream << cha[0];
            if (cha[0] < 0)
                stream << cha[1];
        }
    }
    return stream;
}


String coon::operator+(const String& lhs, const String& rhs)
{
    StringBuilder b;
    b.append(lhs);
    b.append(rhs);
    return b.toString();
}

