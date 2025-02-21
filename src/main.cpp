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

int main(int argc, const char *argv[]) {
  if (argc == 3) {
    ini::cFile inFile;
    if (inFile.open(argv[1])) {
      ini::cIni ini;

      {
        ini.read(&inFile);

        printf("Reading INI-file: %s, size %u\n", argv[1],
               (unsigned)inFile.size());
        printf("  %s\n", ini.getString("first section", "key"));
        printf("  %s\n", ini.getString("first section", "another key"));
        printf("  %s\n", ini.getString("second section", "key"));
      }

      {
        ini.setString("first section", "key", "updated key");
        ini.setString("first section", "another key", "updated another key");
        ini.setString("second section", "key", "updated key");

        ini.setString("new section", "key", "value 0");
        ini.setString("new section", "key with spaces", "value 1");
        ini.setString("new section", "value_with_spaces",
                      "some value with spaces");
      }

      ini::cFile outFile;
      if (outFile.open(argv[2], "wb")) {
        ini.save(&outFile);
        printf("Writing INI-file: %s, size %u\n", argv[2], outFile.size());

        printf("  %s\n", ini.getString("first section", "key"));
        printf("  %s\n", ini.getString("first section", "another key"));
        printf("  %s\n", ini.getString("second section", "key"));

        printf("  %s\n", ini.getString("new section", "key"));
        printf("  %s\n", ini.getString("new section", "key with spaces"));
        printf("  %s\n", ini.getString("new section", "value_with_spaces"));
      }
    }
  } else {
    printf("Usage:\n");
    printf("  %s test.ini out.ini\n", argv[0]);
  }

  return 0;
}
