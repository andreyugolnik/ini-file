/**********************************************\
*
*  Andrey A. Ugolnik
*  https://www.ugolnik.info
*  andrey@ugolnik.info
*
*  MIT License
*
\**********************************************/

#pragma once

#include <vector>

namespace ini
{
    class cFile
    {
    public:
        cFile();
        virtual ~cFile();

        virtual bool open(const char* name, const char* mode = "rb");
        virtual bool close();

        virtual unsigned read(void* buffer, unsigned size);
        virtual unsigned write(const void* buffer, unsigned size);

        virtual unsigned size() const
        {
            return m_size;
        }

        virtual unsigned offset() const
        {
            return m_offset;
        }

        virtual bool seek(unsigned offset, int whence);

    private:
        void* m_file = nullptr;
        unsigned m_size = 0;
        unsigned m_offset = 0;
    };

    class cSection;

    class cIni final
    {
    public:
        cIni();
        ~cIni();

        void clear();

        void read(cFile* file);
        void save(cFile* file);

        const char* getString(const char* section, const char* key) const;
        void setString(const char* section, const char* key, const char* value);

    public:
        using SectionList = std::vector<cSection*>;

    private:
        SectionList m_sections;
    };

} // namespace ini
