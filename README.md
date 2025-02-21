# Non-Destructive INI File Parser

A simple C++ class for reading and writing INI files.

## Usage
```cpp
    ini::cIni ini;

    // read from ini-file
    {
        ini::cFile file;
        if (file.open("input.ini"))
        {
            ini.read(&file);
        }
    }

    // get and set
    {
        // returns key value or nullptr
        const char* value = ini.getString("section_name", "key_name"));

        ini.setString("section_name", "key_name", "new key value");
    }

    // write to ini-file
    {
        ini::cFile file;
        if (file.open("output.ini", "wb"))
        {
            ini.save(&file);
        }
    }
```

***

```
Copyright © 2000-2017 Andrey A. Ugolnik. All Rights Reserved.
https://www.ugolnik.info
andrey@ugolnik.info
```
