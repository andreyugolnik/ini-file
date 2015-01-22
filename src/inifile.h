/**********************************************\
*
*  Andrey A. Ugolnik
*  Tiny Orange, OOO
*  http://www.tinyorange.com
*  andrey@ugolnik.info
*
\**********************************************/

#ifndef INIFILE_H_P7A5X0QM
#define INIFILE_H_P7A5X0QM

#include <string>
#include <vector>
#include <map>
#include <iterator>

class CIniSection
{
public:
    CIniSection() {}
    virtual ~CIniSection() {}

    void SetKey(const std::string& key, const std::string& data)
    {
        //printf("    Key: '%s', value: '%s'\n", key.c_str(), data.c_str());
        if(!key.empty())
        {
            std::vector<sLine>::iterator it = m_vecSection.begin();
            for( ; it != m_vecSection.end(); ++it)
            {
                if((*it).key == key)
                {
                    (*it).data = data;
                    return;
                }
            }
            sLine str = { key, data };
            m_vecSection.push_back(str);
        }
    }

    bool GetKey(const char* key, const char** data) const
    {
        std::vector<sLine>::const_iterator it = m_vecSection.begin();
        for( ; it != m_vecSection.end(); ++it)
        {
            if((*it).key == key)
            {
                *data = (*it).data.c_str();
                return true;
            }
        }
        return false;
    }

    void WriteKey(std::ostream_iterator<std::string>& oo)
    {
        std::vector<sLine>::const_iterator it = m_vecSection.begin();
        for( ; it != m_vecSection.end(); ++it)
        {
            oo = (*it).key;
            if(!(*it).data.empty())
            {
                oo = " = " + (*it).data;
            }
            oo = "\n";
        }
        oo = "\n";
    }

private:
    struct sLine
    {
        std::string key;
        std::string data;
    };
    std::vector<sLine> m_vecSection;
};





class CIniFile
{
public:
    CIniFile();
    virtual ~CIniFile();

    bool ReadIni(const char* pchName);
    bool WriteIni(const char* pchName);

    const char *GetString(const char* section, const char* name, const char* def_val);
    int GetInt(const char* section, const char* name, const int def_val);
    float GetFloat(const char* section, const char* name, const float def_val);

    void SetString(const char* section, const char* name, const char* value);
    void SetInt(const char* section, const char* name, const int value);
    void SetFloat(const char* section, const char* name, const float value);

private:
    std::map<std::string, CIniSection*> m_sections;
    std::string m_sLastSection;

    void ClearSections();
    void ParseLine(const std::string& s);
    void TrimSpaces(std::string& str);
    std::vector<std::string> SepString(std::string input, const char& seperator);
};

#endif /* end of include guard: INIFILE_H_P7A5X0QM */

