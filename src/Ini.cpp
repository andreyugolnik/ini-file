/**********************************************\
*
*  Andrey A. Ugolnik
*  https://www.ugolnik.info
*  andrey@ugolnik.info
*
*  MIT License
*
\**********************************************/

#include "Ini.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>

namespace ini
{
    cFile::cFile()
    {
    }

    cFile::~cFile()
    {
        close();
    }

    bool cFile::open(const char* name, const char* mode)
    {
        if (m_file != nullptr)
        {
            close();
        }

        m_file = fopen(name, mode);
        if (m_file != nullptr)
        {
            seek(0, SEEK_END);
            m_size = static_cast<unsigned>(ftell(static_cast<FILE*>(m_file)));
            seek(0, SEEK_SET);
        }

        return m_file != nullptr;
    }

    bool cFile::close()
    {
        const bool result = m_file != nullptr
            ? (fclose(static_cast<FILE*>(m_file)) == 0)
            : false;

        m_file = nullptr;
        m_size = 0;
        m_offset = 0;

        return result;
    }

    unsigned cFile::read(void* buffer, unsigned size)
    {
        auto count = static_cast<unsigned>(fread(buffer, 1, size, static_cast<FILE*>(m_file)));
        m_offset += count;
        return count;
    }

    unsigned cFile::write(const void* buffer, unsigned size)
    {
        auto count = static_cast<unsigned>(fwrite(buffer, 1, size, static_cast<FILE*>(m_file)));
        m_offset += count;

        if (m_offset > m_size)
        {
            m_size = m_offset;
        }

        return count;
    }

    bool cFile::seek(unsigned offset, int whence)
    {
        const bool result = fseek(static_cast<FILE*>(m_file), static_cast<long>(offset), whence) == 0;
        if (result)
        {
            switch (whence)
            {
            case SEEK_SET:
                m_offset = offset;
                break;

            case SEEK_CUR:
                m_offset += offset;
                break;

            case SEEK_END:
                m_offset -= offset;
                break;
            }

            if (m_offset > m_size)
            {
                m_size = m_offset;
            }
        }

        return result;
    }

    namespace
    {
        void TrimSpaces(std::string& str)
        {
            // Trim Both leading and trailing spaces
            auto startpos = str.find_first_not_of(" \t\r\n");
            auto endpos = str.find_last_not_of(" \t\r\n");

            // if all spaces or empty return an empty string
            if (std::string::npos == startpos || std::string::npos == endpos)
            {
                str = "";
            }
            else
            {
                str = str.substr(startpos, endpos - startpos + 1);
            }
        }

        std::vector<std::string> ReadLines(const std::vector<char>& in)
        {
            std::vector<std::string> result;
            size_t start = 0;
            for (size_t i = 0, size = in.size(); i < size; i++)
            {
                if (in[i] == '\n')
                {
                    result.push_back({ &in[start], i - start });
                    start = i + 1;
                }
            }

            return result;
        }

    } // namespace

    class cSection final
    {
    public:
        struct Entry
        {
            Entry()
                : type(Type::Empty)
            {
            }

            Entry(const std::string& c)
                : type(Type::Comment)
                , key(c)
            {
            }

            Entry(const std::string& k, const std::string& v)
                : type(Type::KeyValue)
                , key(k)
                , value(v)
            {
            }

            enum class Type
            {
                Empty,
                Comment,
                KeyValue,
            };

            Type type;
            std::string key;
            std::string value;
        };

        using Entries = std::vector<Entry>;

        cSection(const std::string& name)
            : m_name(name)
        {
        }

        const std::string& getName() const
        {
            return m_name;
        }

        void add(const std::string& data)
        {
            if (data.length())
            {
                if (data[0] != ';')
                {
                    auto pos = data.find('=');
                    if (pos != std::string::npos)
                    {
                        auto key = data.substr(0, pos - 1);
                        TrimSpaces(key);

                        auto value = data.substr(pos + 1, data.length() - pos);
                        TrimSpaces(value);

                        m_entries.push_back({ key, value });
                    }
                    else
                    {
                        auto key = data;
                        TrimSpaces(key);
                        m_entries.push_back({ key, "" });
                    }
                }
                else
                {
                    m_entries.push_back({ data });
                }
            }
            else
            {
                m_entries.push_back({});
            }
        }

        void setValue(const char* key, const char* value)
        {
            auto it = std::find_if(m_entries.begin(), m_entries.end(),
                                   [&key](const Entry& e) { return e.key == key; });

            if (value != nullptr)
            {
                if (it != m_entries.end())
                {
                    it->value = value;
                }
                else
                {
                    m_entries.push_back({ key, value });
                }
            }
            else
            {
                if (it != m_entries.end())
                {
                    m_entries.erase(it);
                }
            }
        }

        const char* getValue(const char* key) const
        {
            auto it = std::find_if(m_entries.begin(), m_entries.end(),
                                   [key](const Entry& e) { return e.key == key; });
            if (it != m_entries.end())
            {
                return it->value.c_str();
            }
            return nullptr;
        }

        const Entries& getEntries() const
        {
            return m_entries;
        }

    private:
        const std::string m_name;

    private:
        Entries m_entries;
    };

    cSection* Find(const char* section, const cIni::SectionList& sections)
    {
        for (auto s : sections)
        {
            if (s->getName() == section)
            {
                return s;
            }
        }

        return nullptr;
    }

    cSection* ParseLine(const std::string& s, cSection* currentSection,
                        cIni::SectionList& sections)
    {
        auto len = s.length();
        if (len > 2 && s[0] == '[' && s[len - 1] == ']')
        {
            auto name = s.substr(1, s.length() - 2);
            currentSection = Find(name.c_str(), sections);
            if (currentSection == nullptr)
            {
                currentSection = new cSection(name);
                sections.push_back(currentSection);
            }
        }
        else if (currentSection != nullptr)
        {
            currentSection->add(s);
        }

        return currentSection;
    }

    cIni::cIni()
    {
    }

    cIni::~cIni()
    {
        clear();
    }

    void cIni::read(cFile* file)
    {
        clear();

        auto size = file->size();
        std::vector<char> in(size);
        file->read(in.data(), size);

        auto lines = ini::ReadLines(in);

        cSection* currentSection = nullptr;
        for (auto& line : lines)
        {
            currentSection = ParseLine(line, currentSection, m_sections);
        }
    }

    void cIni::save(cFile* file)
    {
        constexpr auto delim = " = ";
        static const auto delimLen = strlen(delim);

        constexpr auto end = "\n";
        static const auto endLen = strlen(end);

        for (const auto section : m_sections)
        {
            const std::string name = "[" + section->getName() + "]\n";
            file->write(name.c_str(), name.length());

            auto& entries = section->getEntries();
            for (const auto& e : entries)
            {
                switch (e.type)
                {
                case cSection::Entry::Type::KeyValue:
                    file->write(e.key.c_str(), e.key.length());
                    file->write(delim, delimLen);
                    file->write(e.value.c_str(), e.value.length());
                    break;

                case cSection::Entry::Type::Comment:
                    file->write(e.key.c_str(), e.key.length());
                    break;

                case cSection::Entry::Type::Empty:
                    break;
                }

                file->write(end, endLen);
            }
        }
    }

    const char* cIni::getString(const char* section, const char* key) const
    {
        auto s = Find(section, m_sections);
        return s != nullptr
            ? s->getValue(key)
            : nullptr;
    }

    void cIni::setString(const char* section, const char* key, const char* value)
    {
        auto s = Find(section, m_sections);
        if (s != nullptr)
        {
            s->setValue(key, value);
        }
        else
        {
            s = new cSection(section);
            s->setValue(key, value);
            m_sections.push_back(s);
        }
    }

    void cIni::clear()
    {
        for (auto section : m_sections)
        {
            delete section;
        }
        m_sections.clear();
    }

} // namespace ini
