/**********************************************\
*
*  Andrey A. Ugolnik
*  Tiny Orange, OOO
*  http://www.tinyorange.com
*  andrey@ugolnik.info
*
\**********************************************/

#include <iostream>
#include <stdio.h>
#include "inifile.h"

static int GetFileSize(const char* name)
{
    FILE* f = fopen(name, "rb");
    fseek(f, 0, SEEK_END);
    const int size = ftell(f);
    fclose(f);
    return size;
}

int main()
{
    const char* file_name = "test.ini";

    CIniFile ini;
    for(int i = 0; i < 1000; i++)
    {
        if(ini.ReadIni(file_name))
        {
            printf("Reading INI-file, size %d\n", GetFileSize(file_name));
            printf("%d, %d, %d, %d\n",
                    ini.GetInt("Options", "W", 999),
                    ini.GetInt("Options", "Mute", 0),
                    ini.GetInt("Options", "VolumeMusic", 0),
                    ini.GetInt("Options", "VolumeEffects", 0));

            ini.SetInt("Options", "W", 10000);
            ini.SetInt("Options", "Mute", 40);
            ini.SetInt("Options", "VolumeMusic", 50);
            ini.SetInt("Options", "VolumeEffects", 60);
            //ini.SetInt("Options2", "VolumeEffects2", 100);
            ini.SetInt("Options", "unknown_key", 888);
            printf("%d, %d, %d, %d, %d, %d\n",
                    ini.GetInt("Options", "Windowed", 0),
                    ini.GetInt("Options", "Mute", 0),
                    ini.GetInt("Options", "VolumeMusic", 0),
                    ini.GetInt("Options", "VolumeEffects", 0),
                    ini.GetInt("Options2", "VolumeEffects2", 0),
                    ini.GetInt("Options", "W", 0));


            ini.WriteIni(file_name);
            printf("Writing INI-file, size %d\n", GetFileSize(file_name));
        }
    }
    return 0;
}

