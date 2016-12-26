/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "Ini.h"

int main(int argc, const char* argv[])
{
    if (argc == 3)
    {
        cFile inFile;
        if (inFile.open(argv[1]))
        {
            cIni ini;

            {
                ini.read(&inFile);

                printf("Reading INI-file: %s, size %u\n", argv[1], (unsigned)inFile.size());
                printf("%s, %s, %s, %s\n",
                       ini.getString("Options", "W"),
                       ini.getString("Options", "Mute"),
                       ini.getString("Options", "VolumeMusic"),
                       ini.getString("Options", "VolumeEffects"));
            }

            {
                ini.setString("new section", "key", "value");
                ini.setString("new section", "key with spaces", "value");
                ini.setString("new section", "value_with_spaces", "some value with spaces");
            }

            cFile outFile;
            if (outFile.open(argv[2], "wb"))
            {
                printf("%s, %s, %s, %s, %s, %s\n",
                       ini.getString("Options", "Windowed"),
                       ini.getString("Options", "Mute"),
                       ini.getString("Options", "VolumeMusic"),
                       ini.getString("Options", "VolumeEffects"),
                       ini.getString("Options2", "VolumeEffects2"),
                       ini.getString("Options", "W"));


                ini.save(&outFile);
                printf("Writing INI-file: %s, size %u\n", argv[2], outFile.size());
            }
        }
    }

    return 0;
}
