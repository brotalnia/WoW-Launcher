#include <windows.h>

bool DirExists(const char* dirName_in)
{
    DWORD ftyp = GetFileAttributesA(dirName_in);
    if (ftyp == INVALID_FILE_ATTRIBUTES)
    {
        return false;  // something is wrong with your path!
    }

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    {
        return true;   // this is a directory!
    }
    else
    {
        return false;    // this is not a directory!
    }
}
