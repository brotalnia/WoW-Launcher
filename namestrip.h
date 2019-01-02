#include <windows.h>
#include <tchar.h>

void path_strip_filename(TCHAR *Path) {
    size_t Len = _tcslen(Path);
    if (Len==0) {return;};
    size_t Idx = Len-1;
    while (TRUE) {
        TCHAR Chr = Path[Idx];
        if (Chr==TEXT('\\')||Chr==TEXT('/')) {
            if (Idx==0||Path[Idx-1]==':') {Idx++;};
            break;
        } else if (Chr==TEXT(':')) {
            Idx++; break;
        } else {
            if (Idx==0) {break;} else {Idx--;};
        };
    };
    Path[Idx] = TEXT('\0');
};
