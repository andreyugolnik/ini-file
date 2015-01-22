/**********************************************\
*
*  Andrey A. Ugolnik
*  Tiny Orange, OOO
*  http://www.tinyorange.com
*  andrey@ugolnik.info
*
\**********************************************/

#include "inifile.h"
#include <fstream>

CIniFile::CIniFile()
{
}

CIniFile::~CIniFile()
{
    ClearSections();
}

void CIniFile::TrimSpaces(std::string& str)
{
    // Trim Both leading and trailing spaces
    size_t startpos = str.find_first_not_of(" \t\r\n");
    size_t endpos = str.find_last_not_of(" \t\r\n");

    // if all spaces or empty return an empty string
    if((std::string::npos == startpos) || (std::string::npos == endpos))
    {
        str = "";
    }
    else
    {
        str = str.substr(startpos, endpos - startpos + 1);
    }
}

std::vector<std::string> CIniFile::SepString(std::string input, const char &seperator)
{
    std::vector<std::string> result;
    size_t pos = 0;
    while(pos < input.size())
    {
        if(input[pos] == seperator)
        {
            const std::string found = input.substr(0, pos);
            result.push_back(found);
            input = input.substr(pos + 1, input.size() - pos);
            pos = 0;
        }
        pos++;
    }
    result.push_back(input);
    return result;
}

void CIniFile::ParseLine(const std::string &s)
{
    int len = s.length();
    if(len >= 3 && s.at(0) == '[' && s.at(len - 1) == ']')
    {
        std::map<std::string, CIniSection*>::iterator pos = m_sections.find(s);
        if(pos == m_sections.end())
        { // create new section
            //printf("SECTION '%s' created.\n", s.c_str());
            m_sections[s] = new CIniSection();
            m_sLastSection = s;
        }
    }
    else if(!m_sLastSection.empty())
    {
        std::map<std::string, CIniSection*>::iterator pos = m_sections.find(m_sLastSection);
        if(pos != m_sections.end())
        { // fill section
            //printf("  Parsing line '%s'\n", s.c_str());
            std::vector<std::string> pair = SepString(s, '=');
            if(pair.size() == 2)
            {
                TrimSpaces(pair[0]);
                TrimSpaces(pair[1]);
                pos->second->SetKey(pair[0], pair[1]);
            }
            else
            {
                pos->second->SetKey(pair[0], "");
            }
        }
        else
        {
            printf("Section '%s' not found!\n", m_sLastSection.c_str());
        }
    }
}

bool CIniFile::ReadIni(const char* pchName)
{
    ClearSections();

    std::ifstream is(pchName);
    std::vector<std::string> lines;
    for(std::string line; std::getline(is, line); )
    {
        lines.push_back(line);
    }
    is.close();

    std::vector<std::string>::const_iterator it = lines.begin();
    for( ; it != lines.end(); it++)
    {
        ParseLine((*it));
    }

    return true;
}

bool CIniFile::WriteIni(const char* pchName)
{
    std::ofstream os(pchName);
    std::ostream_iterator<std::string> oo(os);

    std::map<std::string, CIniSection*>::const_iterator it = m_sections.begin();
    for( ; it != m_sections.end(); it++)
    {
        oo = (*it).first + "\n";
        (*it).second->WriteKey(oo);
    }

    os.close();

    return true;
}

const char* CIniFile::GetString(const char* section, const char* name, const char* def_val)
{
    std::string sec("[");
    sec += section;
    sec += "]";
    std::map<std::string, CIniSection*>::const_iterator it = m_sections.find(sec);
    if(it != m_sections.end())
    {
        const char* pdata;
        if(true == (*it).second->GetKey(name, &pdata))
        {
            return pdata;
        }
    }
    return def_val;
}

void CIniFile::SetString(const char* section, const char* name, const char* value)
{
    std::string sec("[");
    sec += section;
    sec += "]";
    std::map<std::string, CIniSection*>::const_iterator it = m_sections.find(sec);
    if(it != m_sections.end())
    {
        (*it).second->SetKey(name, value);
    }
    else
    {
        m_sections[sec] = new CIniSection();
        SetString(section, name, value);
    }
}

int CIniFile::GetInt(const char* section, const char* name, const int def_val)
{
    char buff[20];
    snprintf(buff, sizeof(buff), "%d", def_val);
    return atoi(GetString(section, name, buff));
}

void CIniFile::SetInt(const char* section, const char* name, const int value)
{
    char buff[20];
    snprintf(buff, sizeof(buff), "%d", value);
    SetString(section, name, buff);
}

float CIniFile::GetFloat(const char* section, const char* name, const float def_val)
{
    char buff[20];
    snprintf(buff, sizeof(buff), "%g", def_val);
    return atof(GetString(section, name, buff));
}

void CIniFile::SetFloat(const char* section, const char* name, const float value)
{
    char buff[20];
    snprintf(buff, sizeof(buff), "%g", value);
    SetString(section, name, buff);
}

void CIniFile::ClearSections()
{
    std::map<std::string, CIniSection*>::iterator it = m_sections.begin();
    for( ; it != m_sections.end(); ++it)
    {
        delete (*it).second;
    }

    m_sections.clear();
}

