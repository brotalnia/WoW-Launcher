typedef int bool;
#define true 1
#define false 0

#include "version.h"
#include <windows.h>
#include "resource.h"
#include "memset.h"

#if EXPANSION > VANILLA
#include "direxists.h"
#endif

/* declare windows procedures */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure2(HWND, UINT, WPARAM, LPARAM);

int rboption = 6; /* which radio button is currently selected */

/* default servers */
#if EXPANSION == CATA
char Server1Name[256] = "Atlantiss";
char Server1Realm[256] = "play.atlantiss.eu";
char Server1Website[256] = "http://atlantiss.eu";
char Server2Name[256] = "Monster-WoW";
char Server2Realm[256] = "logon.monster-wow.com";
char Server2Website[256] = "https://www.monster-wow.com";
char Server3Name[256] = "WoW Mortal";
char Server3Realm[256] = "logon.wowmortal.com";
char Server3Website[256] = "http://wowmortal.com";
char Server4Name[256] = "WoW Circle";
char Server4Realm[256] = "logon.wowcircle.com";
char Server4Website[256] = "http://www.wowcircle.com";
char Server5Name[256] = "Apollo (Twinstar)";
char Server5Realm[256] = "login.apollo-wow.com";
char Server5Website[256] = "http://www.apollo-wow.com/";
#elif EXPANSION == WOTLK
char Server1Name[256] = "Eternal-WoW";
char Server1Realm[256] = "logon.eternal-wow.com";
char Server1Website[256] = "http://eternal-wow.com";
char Server2Name[256] = "Gamer-District";
char Server2Realm[256] = "wow.gamer-district.org";
char Server2Website[256] = "https://www.gamer-district.org";
char Server3Name[256] = "Land of Elves";
char Server3Realm[256] = "landofelves.net";
char Server3Website[256] = "https://landofelves.net";
char Server4Name[256] = "WoW Circle";
char Server4Realm[256] = "logon.wowcircle.com";
char Server4Website[256] = "http://www.wowcircle.com";
char Server5Name[256] = "Warmane";
char Server5Realm[256] = "logon.warmane.com";
char Server5Website[256] = "https://www.warmane.com";
#elif EXPANSION == TBC
char Server1Name[256] = "Smolderforge";
char Server1Realm[256] = "connect.smolderforge.com";
char Server1Website[256] = "https://www.smolderforge.com";
char Server2Name[256] = "Nightbane";
char Server2Realm[256] = "logon.sunwell.pl";
char Server2Website[256] = "https://sunwell.pl/";
char Server3Name[256] = "Vengeance of Azeroth";
char Server3Realm[256] = "logon.vengeancewow.com";
char Server3Website[256] = "https://www.vengeancewow.com/";
char Server4Name[256] = "ExcaliburWoW";
char Server4Realm[256] = "exwow-serv.exnw.com";
char Server4Website[256] = "https://www.excalibur.ws/";
char Server5Name[256] = "Warmane";
char Server5Realm[256] = "logon.warmane.com";
char Server5Website[256] = "https://www.warmane.com/";
#else
char Server1Name[256] = "VanillaGaming";
char Server1Realm[256] = "Logon.vanillagaming.org";
char Server1Website[256] = "http://www.vanillagaming.org";
char Server2Name[256] = "RetroWoW";
char Server2Realm[256] = "logon.retro-wow.com";
char Server2Website[256] = "http://retro-wow.com/";
char Server3Name[256] = "KRONOS";
char Server3Realm[256] = "wow.twinstar.cz";
char Server3Website[256] = "http://www.kronos-wow.com";
char Server4Name[256] = "Symmetry";
char Server4Realm[256] = "logon.symmetrywow.com";
char Server4Website[256] = "https://symmetrywow.com";
char Server5Name[256] = "Lights Hope";
char Server5Realm[256] = "logon.lightshope.org";
char Server5Website[256] = "https://lightshope.org/";
#endif

char cachePath[MAX_PATH];
char ownPath[MAX_PATH];

HWND hwnd1; /* handle for the main window */
HWND hwnd2; /* handle for the server list window */

/* handles for all the different controls */
HWND txtRealmlist, rbtnNoChange, rbtnLocalhost, rbtnServer1, rbtnServer2, rbtnServer3, rbtnServer4, rbtnServer5, rbtnSetTo, lblRealmlist, btnStart, btnOpenDirectory, btnWebsite, btnCache, btnExit, lblName1, lblName2, lblName3, lblName4, lblName5, lblRealm1, lblRealm2, lblRealm3, lblRealm4, lblRealm5, lblWebsite1, lblWebsite2, lblWebsite3, lblWebsite4, lblWebsite5, txtName1, txtName2, txtName3, txtName4, txtName5, txtRealm1, txtRealm2, txtRealm3, txtRealm4, txtRealm5, txtWebsite1, txtWebsite2, txtWebsite3, txtWebsite4, txtWebsite5, btnSave;

#if EXPANSION > TBC
HWND txtLanguage, lblLanguage;
#endif

/* window class names */
char szClassName[ ] = "MainWindow";
char szClassName2[ ] = "ServerList";

/* main window controls */
#define ID_BTN_START 1
#define ID_BTN_OPEN_DIRECTORY 2
#define ID_BTN_WEBSITE 3
#define ID_BTN_CACHE 4
#define ID_BTN_EXIT 5
#define ID_RBTN_NO_CHANGE 6
#define ID_RBTN_LOCALHOST 7
#define ID_RBTN_SERVER1 8
#define ID_RBTN_SERVER2 9
#define ID_RBTN_SERVER3 10
#define ID_RBTN_SERVER4 11
#define ID_RBTN_SERVER5 12
#define ID_RBTN_SET_TO 13
#define ID_TXT_REALMLIST 14
#define ID_LBL_REALMLIST 15
#define ID_TXT_LANGUAGE 32
/* second window controls */
#define ID_TXT_NAME1 16
#define ID_TXT_NAME2 17
#define ID_TXT_NAME3 18
#define ID_TXT_NAME4 19
#define ID_TXT_NAME5 20
#define ID_TXT_REALM1 21
#define ID_TXT_REALM2 22
#define ID_TXT_REALM3 23
#define ID_TXT_REALM4 24
#define ID_TXT_REALM5 25
#define ID_TXT_WEBSITE1 26
#define ID_TXT_WEBSITE2 27
#define ID_TXT_WEBSITE3 28
#define ID_TXT_WEBSITE4 29
#define ID_TXT_WEBSITE5 30
#define ID_BTN_SAVE 31

int GetWindowShowState()
{
    STARTUPINFO sui;

    GetStartupInfo(&sui);

    return (sui.dwFlags & STARTF_USESHOWWINDOW)
                ? sui.wShowWindow
                : SW_SHOWDEFAULT;
}

int APIENTRY WinMainCRTStartup (void)
{
    int iRet;
    iRet = WinMain(GetModuleHandle(NULL), NULL, NULL, GetWindowShowState());
    ExitProcess(iRet);
    return iRet;
}

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the main window class */
    WNDCLASSEX wincl2;       /* Data structure for the server list window class */
    
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Set icon and mouse-pointer */
    wincl.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON_BIG));
    wincl.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON_SMALL));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Set background color */
    wincl.hbrBackground = CreateSolidBrush(RGB(236, 233, 216));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd1 = CreateWindowEx (
           0,                   /* Extended possibilities for variation */
           szClassName,         /* Classname */
           #if EXPANSION == CATA
           "WoW Launcher by brotalnia for Cataclysm",     /* Title Text */
           #elif EXPANSION == WOTLK
           "WoW Launcher by brotalnia for WotLK",         /* Title Text */
           #elif EXPANSION == TBC
           "WoW Launcher by brotalnia for TBC",           /* Title Text */
           #else
           "WoW Launcher by brotalnia for Vanilla",       /* Title Text */
           #endif
           WS_SYSMENU | WS_MINIMIZEBOX, /* Window style */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           383,                 /* The programs width */
           254,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd1, nFunsterStil);

    ZeroMemory(&wincl2,sizeof(WNDCLASSEX));
    
    /* The Window structure */
    wincl2.hInstance=hThisInstance;
    wincl2.lpszClassName=szClassName2;
    wincl2.lpfnWndProc=WindowProcedure2;
    wincl2.style=CS_HREDRAW|CS_VREDRAW;
    wincl2.cbSize=sizeof(WNDCLASSEX);
    
    /* Set icon and mouse-pointer */
    wincl2.hIcon=LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON_CONFIG));
    wincl2.hIconSm=LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON_CONFIG));
    wincl2.hCursor= LoadCursor (NULL, IDC_ARROW);
    wincl2.lpszMenuName=NULL;              /* No menu */
    wincl2.cbClsExtra=0;                   /* No extra bytes after the window class */
    wincl2.cbWndExtra=0;                   /* structure or the window instance */
    /* Set background color */
    wincl2.hbrBackground=CreateSolidBrush(RGB(236, 233, 216));
    
    /* Register the window class, and if it fails throw an error */
    if(!RegisterClassEx(&wincl2))
    {
        MessageBox(NULL, "Window class creation failed for window 2","Window Class Failed", MB_OK | MB_ICONERROR);
    }

    /* The class is registered, let's create the window*/
    hwnd2 = CreateWindowEx (
           0,                   /* Extended possibilities for variation */
           szClassName2,         /* Classname */
           "Server List Configuration",   /* Title Text */
           WS_OVERLAPPED | WS_SYSMENU, /* Window style */
           (GetSystemMetrics(SM_CXSCREEN) - 603) / 2,       /* Middle of the screen */
           (GetSystemMetrics(SM_CYSCREEN) - 313) / 2,
           603,                 /* The programs width */
           313,                 /* and height in pixels */
           hwnd1,               /* The window is a child-window of the main window */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
           

    if(!hwnd2)
    {
        MessageBox(NULL, "Window creation failed", "Window creation failed", MB_OK | MB_ICONERROR);
    }

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        if(!IsDialogMessage(hwnd2, &messages)) /* allows selecting controls with tab */
        {
            /* Translate virtual-key messages into character messages */
            TranslateMessage(&messages);
            /* Send message to WindowProcedure */
            DispatchMessage(&messages);
        }
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void WriteRealmlist(const char* realm)
{
    HANDLE hFile;
    char realmFinal[256] = "set realmlist ";
    /* realmlist file is in the Data\<locale>\ folder in wotlk */
#if EXPANSION > TBC
    char realmPath[MAX_PATH] = "Data\\";
    char locale[5];
    GetDlgItemText(hwnd1, ID_TXT_LANGUAGE, locale, 5);
    lstrcat(realmPath, locale);
    lstrcat(realmPath, "\\realmlist.wtf");
#else
    char realmPath[MAX_PATH] = "realmlist.wtf";
#endif

    SetFileAttributes(realmPath, FILE_ATTRIBUTE_NORMAL);
    lstrcat(realmFinal, realm);
    
    /* need to set patch list for cataclysm */
#if EXPANSION > WOTLK
    lstrcat(realmFinal, "\r\nset patchlist ");
    lstrcat(realmFinal, realm);
#endif
    
    hFile = CreateFile(realmPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        long unsigned int bytesWritten = 0;
        WriteFile(hFile, realmFinal, lstrlen(realmFinal), &bytesWritten, NULL);
        CloseHandle(hFile);        
    }
}

const char* FindSubString(const char* src, const char* sub)
{
    int i;
    
	if (*sub == '\0')
		return src;
    
	for (i = 0; i < lstrlen(src); i++)
	{
		if (*(src + i) == *sub) 
		{
			const char* ptr = FindSubString(src + i + 1, sub + 1);
			return (ptr) ? ptr - 1 : 0;
		}
	}

	return 0;
}

int ReadTextLine(char* data, int dataSize, char* line)
{
    int i;
    for (i = 0; i < dataSize; i++)
    {
        if (data[i] == '\r' || data[i] == '\n' || data[i] == '\0')
        {
            line[i] = '\0';
            return i+1;
        }
        line[i] = data[i];
    }
    return i;
}

void ReadServersList(const char* fileName)
{
    char data[512];
    char line[512];
    DWORD position = 0;
    DWORD fileSize = -1;
    
    HANDLE hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
        return;
    
    if(!ReadFile(hFile, (LPVOID*)&data, 512, &fileSize, NULL))
        return;
    
    if (fileSize <= 0)
    {
        CloseHandle(hFile);
        return;
    }
    
    if (data[fileSize] != '\0')
        data[fileSize] = '\0';
    
    while (position < fileSize)
    {
        int lineSize = ReadTextLine(data + position, fileSize - position, line);
        position += lineSize;
        
        if (lineSize == 0)
            break;

        if(FindSubString(line, "Server1Name"))
        {
            lstrcpyn(Server1Name, line+12, lineSize-12);
        }
        else if (FindSubString(line, "Server1Realm"))
        {
            lstrcpyn(Server1Realm, line+13, lineSize-13);
        }
        else if (FindSubString(line, "Server1Website"))
        {
            lstrcpyn(Server1Website, line+15, lineSize-15);
        }
        else if(FindSubString(line, "Server2Name"))
        {
            lstrcpyn(Server2Name, line+12, lineSize-12);
        }
        else if (FindSubString(line, "Server2Realm"))
        {
            lstrcpyn(Server2Realm, line+13, lineSize-13);
        }
        else if (FindSubString(line, "Server2Website"))
        {
            lstrcpyn(Server2Website, line+15, lineSize-15);
        }
        else if(FindSubString(line, "Server3Name"))
        {
            lstrcpyn(Server3Name, line+12, lineSize-12);
        }
        else if (FindSubString(line, "Server3Realm"))
        {
            lstrcpyn(Server3Realm, line+13, lineSize-13);
        }
        else if (FindSubString(line, "Server3Website"))
        {
            lstrcpyn(Server3Website, line+15, lineSize-15);
        }
        else if(FindSubString(line, "Server4Name"))
        {
            lstrcpyn(Server4Name, line+12, lineSize-12);
        }
        else if (FindSubString(line, "Server4Realm"))
        {
            lstrcpyn(Server4Realm, line+13, lineSize-13);
        }
        else if (FindSubString(line, "Server4Website"))
        {
            lstrcpyn(Server4Website, line+15, lineSize-15);
        }
        else if(FindSubString(line, "Server5Name"))
        {
            lstrcpyn(Server5Name, line+12, lineSize-12);
        }
        else if (FindSubString(line, "Server5Realm"))
        {
            lstrcpyn(Server5Realm, line+13, lineSize-13);
        }
        else if (FindSubString(line, "Server5Website"))
        {
            lstrcpyn(Server5Website, line+15, lineSize-14);
        }
    }
   CloseHandle(hFile);
}

void WriteServersList(const char* fileName)
{
    HANDLE hFile;
    char configData[1024];
    lstrcpy(configData, "Server1Name=");
    lstrcat(configData, Server1Name);
    lstrcat(configData, "\r\nServer1Realm=");
    lstrcat(configData, Server1Realm);
    lstrcat(configData, "\r\nServer1Website=");
    lstrcat(configData, Server1Website);
    lstrcat(configData, "\r\nServer2Name=");
    lstrcat(configData, Server2Name);
    lstrcat(configData, "\r\nServer2Realm=");
    lstrcat(configData, Server2Realm);
    lstrcat(configData, "\r\nServer2Website=");
    lstrcat(configData, Server2Website);
    lstrcat(configData, "\r\nServer3Name=");
    lstrcat(configData, Server3Name);
    lstrcat(configData, "\r\nServer3Realm=");
    lstrcat(configData, Server3Realm);
    lstrcat(configData, "\r\nServer3Website=");
    lstrcat(configData, Server3Website);
    lstrcat(configData, "\r\nServer4Name=");
    lstrcat(configData, Server4Name);
    lstrcat(configData, "\r\nServer4Realm=");
    lstrcat(configData, Server4Realm);
    lstrcat(configData, "\r\nServer4Website=");
    lstrcat(configData, Server4Website);
    lstrcat(configData, "\r\nServer5Name=");
    lstrcat(configData, Server5Name);
    lstrcat(configData, "\r\nServer5Realm=");
    lstrcat(configData, Server5Realm);
    lstrcat(configData, "\r\nServer5Website=");
    lstrcat(configData, Server5Website);
    
    hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        long unsigned int bytesWritten = 0;
        WriteFile(hFile, configData, lstrlen(configData), &bytesWritten, NULL);
        CloseHandle(hFile);        
    }
}

#if EXPANSION > VANILLA
const char* GetLocale()
{
    char realmDir[MAX_PATH];
    
    lstrcpy(realmDir, ownPath);
    lstrcat(realmDir,"\\Data\\enGB");
    if (DirExists(realmDir))
    {
        return "enGB";
    }
    lstrcpy(realmDir, ownPath);
    lstrcat(realmDir,"\\Data\\enUS");
    if (DirExists(realmDir))
    {
        return "enUS";
    }
    lstrcpy(realmDir, ownPath);
    lstrcat(realmDir,"\\Data\\esES");
    if (DirExists(realmDir))
    {
        return "esES";
    }
    lstrcpy(realmDir, ownPath);
    lstrcat(realmDir,"\\Data\\esMX");
    if (DirExists(realmDir))
    {
        return "esMX";
    }
    lstrcpy(realmDir, ownPath);
    lstrcat(realmDir,"\\Data\\ruRU");
    if (DirExists(realmDir))
    {
        return "ruRU";
    }
    lstrcpy(realmDir, ownPath);
    lstrcat(realmDir,"\\Data\\frFR");
    if (DirExists(realmDir))
    {
        return "frFR";
    }
    lstrcpy(realmDir, ownPath);
    lstrcat(realmDir,"\\Data\\deDE");
    if (DirExists(realmDir))
    {
        return "deDE";
    }
    lstrcpy(realmDir, ownPath);
    lstrcat(realmDir,"\\Data\\zhCN");
    if (DirExists(realmDir))
    {
        return "zhCN";
    }
    lstrcpy(realmDir, ownPath);
    lstrcat(realmDir,"\\Data\\zhTW");
    if (DirExists(realmDir))
    {
        return "zhTW";
    }
    lstrcpy(realmDir, ownPath);
    lstrcat(realmDir,"\\Data\\koKR");
    if (DirExists(realmDir))
    {
        return "koKR";
    }
    
    return "enGB";
    
}
#endif

void FindCacheDir(void)
{
#if EXPANSION > TBC
    char locale[5];
#endif
    lstrcpy(cachePath, ownPath);
#if EXPANSION == VANILLA
    lstrcat(cachePath, "\\WDB");
#elif EXPANSION == TBC
    /* cache directory is localized in tbc */
    lstrcat(cachePath, "\\Cache\\WDB\\");
    lstrcat(cachePath, GetLocale()); 
#else
    /* take locale from text box for wotlk or newer */
    lstrcat(cachePath, "\\Cache\\WDB\\");
    GetDlgItemText(hwnd1, ID_TXT_LANGUAGE, locale, 5);
    lstrcat(cachePath, locale);
#endif
}

void ClearCache(void)
{
    HANDLE hFind;
    WIN32_FIND_DATA data;
    char filePath[MAX_PATH];
    char cacheSearchString[MAX_PATH];
    
#if EXPANSION > TBC
    FindCacheDir();
#endif
    
    lstrcpy(cacheSearchString, cachePath);
    lstrcat(cacheSearchString, "\\");
    lstrcat(cacheSearchString, "*.wdb");
    
    hFind = FindFirstFile(cacheSearchString, &data);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            /* build the path for each file in the folder */
            lstrcpy(filePath, cachePath);
            lstrcat(filePath, "\\");
            lstrcat(filePath, data.cFileName);
            DeleteFile(filePath);
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }
}

void SetSelectedServer(int rbtn_id)
{
    if(IsDlgButtonChecked(hwnd1, rbtn_id) == BST_CHECKED)
    {
        /* disable textbox next to Set To radio button if it is not selected */
        EnableWindow(txtRealmlist, FALSE);
        rboption = rbtn_id;
    }
}

void StartGame(HWND hwnd)
{
    int success;
    bool retry;
    Sleep(500);
    if (GetModuleHandle(NULL) != NULL)
    {
        do
        {
            retry = false;
            success = (int) ShellExecute(hwnd, "open", "Wow.exe", NULL, NULL, SW_SHOWDEFAULT);
            if (success < 32) /* check if game has launched successfully */
            {
                if (MessageBox(hwnd, "Unable to launch World of Warcraft.", "WoW Launcher", MB_RETRYCANCEL | MB_DEFBUTTON2 | MB_ICONERROR) == IDRETRY)
                    retry = true;
            }
            else
            {
                ExitProcess(0); /* close program once game has launched */
            }
        } while (retry);
    }
}

HBRUSH hbrBkgnd = NULL; /* used for the label colors */
HFONT hFont = NULL; /* used for the label fonts */
HPEN hPenOld;
HPEN hLinePen;
COLORREF qLineColor;

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    HINSTANCE hInstance;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
        {
            /* find current working directory */
            GetCurrentDirectory((sizeof(ownPath)), &ownPath);
            
            /* find cache directory now for tbc and vanilla */
            #if EXPANSION < WOTLK
            FindCacheDir();
            #endif
    
            /* creating labels and setting their font */
            lblRealmlist = CreateWindow ("STATIC", " Realmlist:", WS_VISIBLE | WS_CHILD | SS_NOTIFY, 5, 3, 84, 20, hwnd, (HMENU) ID_LBL_REALMLIST, NULL, NULL);
            hFont=CreateFont (20, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Microsoft Sans Serif");
            SendMessage (lblRealmlist, WM_SETFONT, (WPARAM)hFont, TRUE);
            #if EXPANSION > TBC
            lblLanguage = CreateWindow ("STATIC", "Language:", WS_VISIBLE | WS_CHILD, 215, 195, 50, 13, hwnd, NULL, NULL, NULL);
            hFont=CreateFont (14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Microsoft Sans Serif");
            SendMessage (lblLanguage, WM_SETFONT, (WPARAM)hFont, TRUE);
            #endif
            
            /* creating the buttons and setting their font */
            btnStart = CreateWindow ("BUTTON", "Start", WS_VISIBLE |WS_CHILD, 243, 12, 100, 28, hwnd, (HMENU) ID_BTN_START, NULL, NULL);
            SendMessage(btnStart,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            btnOpenDirectory = CreateWindow ("BUTTON", "Open Directory", WS_VISIBLE |WS_CHILD, 243, 46, 100, 28, hwnd, (HMENU) ID_BTN_OPEN_DIRECTORY, NULL, NULL);
            SendMessage(btnOpenDirectory,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            btnWebsite = CreateWindow ("BUTTON", "Website", WS_VISIBLE |WS_CHILD, 243, 80, 100, 28, hwnd, (HMENU) ID_BTN_WEBSITE, NULL, NULL);
            SendMessage(btnWebsite,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            btnCache = CreateWindow ("BUTTON", "Cache", WS_VISIBLE |WS_CHILD, 243, 114, 100, 28, hwnd, (HMENU) ID_BTN_CACHE, NULL, NULL);
            SendMessage(btnCache,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            btnExit = CreateWindow ("BUTTON", "Exit", WS_VISIBLE |WS_CHILD, 243, 148, 100, 28, hwnd, (HMENU) ID_BTN_EXIT, NULL, NULL);
            SendMessage(btnExit,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            
            /* load saved servers list */
            ReadServersList("launcher.cfg");
            
            /* creating the radio buttons and setting their font */
            hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
            rbtnNoChange = CreateWindowEx(0,  "BUTTON",  "No Change",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 31,   140,  17,  hwnd,  (HMENU) ID_RBTN_NO_CHANGE,  hInstance,  NULL);
            SendMessage(rbtnNoChange,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            SendDlgItemMessage(hwnd, ID_RBTN_NO_CHANGE, BM_SETCHECK, 1, 0);
            rbtnLocalhost = CreateWindowEx(0,  "BUTTON",  "Localhost",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 54,   140,  17,  hwnd,  (HMENU) ID_RBTN_LOCALHOST,  hInstance,  NULL);
            SendMessage(rbtnLocalhost,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnServer1 = CreateWindowEx(0,  "BUTTON",  Server1Name,  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 77,   140,  17,  hwnd,  (HMENU) ID_RBTN_SERVER1,  hInstance,  NULL);
            SendMessage(rbtnServer1,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnServer2 = CreateWindowEx(0,  "BUTTON",  Server2Name,  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 100,   140,  17,  hwnd,  (HMENU) ID_RBTN_SERVER2,  hInstance,  NULL);
            SendMessage(rbtnServer2,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnServer3 = CreateWindowEx(0,  "BUTTON",  Server3Name,  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 123,   140,  17,  hwnd,  (HMENU) ID_RBTN_SERVER3,  hInstance,  NULL);
            SendMessage(rbtnServer3,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnServer4 = CreateWindowEx(0,  "BUTTON",  Server4Name,  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 146,   140,  17,  hwnd,  (HMENU) ID_RBTN_SERVER4,  hInstance,  NULL);
            SendMessage(rbtnServer4,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnServer5 = CreateWindowEx(0,  "BUTTON",  Server5Name,  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 169,   140,  17,  hwnd,  (HMENU) ID_RBTN_SERVER5,  hInstance,  NULL);
            SendMessage(rbtnServer5,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnSetTo = CreateWindowEx(0,  "BUTTON",  "Set to:",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 195,   52,  17,  hwnd,  (HMENU) ID_RBTN_SET_TO,  hInstance,  NULL);
            SendMessage(rbtnSetTo,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            
            /* creating textboxes */
            txtRealmlist = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 65, 192, 130, 20, hwnd, (HMENU) ID_TXT_REALMLIST, NULL, NULL);
            SendMessage(txtRealmlist,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            EnableWindow(txtRealmlist, FALSE);
            #if EXPANSION > TBC
            txtLanguage = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(GetLocale()), WS_CHILD | WS_VISIBLE, 274, 192, 92, 20, hwnd, (HMENU) ID_TXT_LANGUAGE, NULL, NULL);
            SendMessage(txtLanguage,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            #endif
            
            break;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case ID_BTN_START: /* start button */
                {
                    switch (rboption) /* check which radio button is selected */
                    {
                        case ID_RBTN_NO_CHANGE:
                            break;
                        case ID_RBTN_LOCALHOST:
                        {
                            WriteRealmlist("127.0.0.1\n");
                            break;
                        }
                        case ID_RBTN_SERVER1:
                        {
                            WriteRealmlist(Server1Realm);
                            break;
                        }
                        case ID_RBTN_SERVER2:
                        {
                            WriteRealmlist(Server2Realm);
                            break;
                        }
                        case ID_RBTN_SERVER3:
                        {
                            WriteRealmlist(Server3Realm);
                            break;
                        }
                        case ID_RBTN_SERVER4:
                        {
                            WriteRealmlist(Server4Realm);
                            break;
                        }
                        case ID_RBTN_SERVER5:
                        {
                            WriteRealmlist(Server5Realm);
                            break;
                        }
                        case ID_RBTN_SET_TO:
                        {
                            char realmlist[256];
                            GetDlgItemText(hwnd, ID_TXT_REALMLIST, realmlist, 256);
                            WriteRealmlist(realmlist);
                            break;
                        }
                    }
                    StartGame(hwnd);
                    break;
                }
                case ID_BTN_OPEN_DIRECTORY: /* open directory button */
                {
                    if (GetModuleHandle(NULL) != NULL)
                        ShellExecute(NULL, "open", ownPath, NULL, NULL, SW_SHOWDEFAULT);
                    break;
                }
                case ID_BTN_WEBSITE: /* website button */
                {
                    switch (rboption)
                    {
                        case ID_RBTN_LOCALHOST:
                            ShellExecute(NULL, "open", "http://127.0.0.1", NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        case ID_RBTN_SERVER1:
                            ShellExecute(NULL, "open", Server1Website, NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        case ID_RBTN_SERVER2:
                            ShellExecute(NULL, "open", Server2Website, NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        case ID_RBTN_SERVER3:
                            ShellExecute(NULL, "open", Server3Website, NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        case ID_RBTN_SERVER4:
                            ShellExecute(NULL, "open", Server4Website, NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        case ID_RBTN_SERVER5:
                            ShellExecute(NULL, "open", Server5Website, NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        default:
                            /* no change or set to */
                            MessageBox(hwnd, "Select a server to view its website.","WoW Launcher", MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION);
                            break;
                    }
                    break;
                }
                case ID_BTN_CACHE: /* clear cache button */
                {
                    if (MessageBox(hwnd, "Do you want to delete the cache?", "WoW Launcher", MB_YESNO | MB_DEFBUTTON1 | MB_ICONQUESTION)==IDYES)
                        ClearCache();
                    break;
                }
                case ID_BTN_EXIT: /* exit button */
                {
                    ExitProcess(0);
                    break;
                }
                case ID_RBTN_NO_CHANGE: /* no change radio button */
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            SetSelectedServer(ID_RBTN_NO_CHANGE);
                            break; 
                        }
                    }
                    break;
                }
                case ID_RBTN_LOCALHOST: /* localhost radio button */
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            SetSelectedServer(ID_RBTN_LOCALHOST);
                            break;
                        }
                    }
                    break;
                }
                case ID_RBTN_SERVER1: /* server1 radio button */
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            SetSelectedServer(ID_RBTN_SERVER1);
                            break; 
                        }
                    }
                    break;
                }
                case ID_RBTN_SERVER2: /* server2 radio button */
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            SetSelectedServer(ID_RBTN_SERVER2);
                            break; 
                        }
                    }
                    break;
                }
                case ID_RBTN_SERVER3: /* server3 radio button */
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            SetSelectedServer(ID_RBTN_SERVER3);
                            break;
                        }
                    }
                    break;
                }
                case ID_RBTN_SERVER4: /* server4 radio button */
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            SetSelectedServer(ID_RBTN_SERVER4);
                            break;
                        }
                    }
                    break;
                }
                case ID_RBTN_SERVER5: /* server5 radio button */
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            SetSelectedServer(ID_RBTN_SERVER5);
                            break;
                        }
                    }
                    break;
                }
                case ID_RBTN_SET_TO: /* set to radio button */
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            if(IsDlgButtonChecked(hwnd, ID_RBTN_SET_TO) == BST_CHECKED)
                            {
                                EnableWindow(txtRealmlist, TRUE); /* enable the textbox */
                                rboption = ID_RBTN_SET_TO; /* storing selected server */
                            }
                            break;
                        }
                    }
                    break;
                }
                case ID_LBL_REALMLIST: /* realmlist label */
                {
                    /* check for double click */
                    if(HIWORD(wParam)==STN_DBLCLK)
                    {
                        /* show the server list window */
                        ShowWindow(hwnd2,SW_SHOWNORMAL);
                        EnableWindow(hwnd1, false);
                    }
                    break;
                }
            }
            break;
        } /* end of case WM_COMMAND */
        case WM_CTLCOLORSTATIC:
        {
            /* setting the label colors */
            HDC hdcStatic = (HDC) wParam;
            SetTextColor(hdcStatic, RGB(0,0,0));
            SetBkColor(hdcStatic, RGB(236, 233, 216));

            if (hbrBkgnd == NULL)
            {
                hbrBkgnd = CreateSolidBrush(RGB(236, 233, 216));
            }
            return (INT_PTR)hbrBkgnd;
        }
        case WM_PAINT:
        {
            hdc = BeginPaint(hwnd, &ps);

            /* Drawing the black separator line in the middle */
            qLineColor = RGB(0, 0, 0);
            hLinePen = CreatePen(PS_SOLID, 5, qLineColor);
            hPenOld = (HPEN)SelectObject(hdc, hLinePen);

            MoveToEx(hdc, 204, 0, NULL);
            LineTo(hdc, 204, 230);

            SelectObject(hdc, hPenOld);
            DeleteObject(hLinePen);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK WindowProcedure2(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
        {
            /* creating labels */
            hFont=CreateFont (14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Microsoft Sans Serif");
            lblName1 = CreateWindow ("STATIC", "Name 1", WS_VISIBLE | WS_CHILD, 15, 10, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblName1, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblName2 = CreateWindow ("STATIC", "Name 2", WS_VISIBLE | WS_CHILD, 15, 55, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblName2, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblName3 = CreateWindow ("STATIC", "Name 3", WS_VISIBLE | WS_CHILD, 15, 100, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblName3, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblName4 = CreateWindow ("STATIC", "Name 4", WS_VISIBLE | WS_CHILD, 15, 145, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblName4, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblName5 = CreateWindow ("STATIC", "Name 5", WS_VISIBLE | WS_CHILD, 15, 190, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblName5, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblRealm1 = CreateWindow ("STATIC", "Realmlist 1", WS_VISIBLE | WS_CHILD, 150, 10, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblRealm1, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblRealm2 = CreateWindow ("STATIC", "Realmlist 2", WS_VISIBLE | WS_CHILD, 150, 55, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblRealm2, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblRealm3 = CreateWindow ("STATIC", "Realmlist 3", WS_VISIBLE | WS_CHILD, 150, 100, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblRealm3, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblRealm4 = CreateWindow ("STATIC", "Realmlist 4", WS_VISIBLE | WS_CHILD, 150, 145, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblRealm4, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblRealm5 = CreateWindow ("STATIC", "Realmlist 5", WS_VISIBLE | WS_CHILD, 150, 190, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblRealm5, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblWebsite1 = CreateWindow ("STATIC", "Website 1", WS_VISIBLE | WS_CHILD, 377, 10, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblWebsite1, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblWebsite2 = CreateWindow ("STATIC", "Website 2", WS_VISIBLE | WS_CHILD, 377, 55, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblWebsite2, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblWebsite3 = CreateWindow ("STATIC", "Website 3", WS_VISIBLE | WS_CHILD, 377, 100, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblWebsite3, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblWebsite4 = CreateWindow ("STATIC", "Website 4", WS_VISIBLE | WS_CHILD, 377, 145, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblWebsite4, WM_SETFONT, (WPARAM) hFont, TRUE);
            lblWebsite5 = CreateWindow ("STATIC", "Website 5", WS_VISIBLE | WS_CHILD, 377, 190, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblWebsite5, WM_SETFONT, (WPARAM) hFont, TRUE);
            
            /* creating textboxes */
            txtName1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server1Name), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 12, 28, 120, 20, hwnd, (HMENU) ID_TXT_NAME1, NULL, NULL);
            SendMessage(txtName1,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtName2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server2Name), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 12, 73, 120, 20, hwnd, (HMENU) ID_TXT_NAME2, NULL, NULL);
            SendMessage(txtName2,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtName3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server3Name), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 12, 118, 120, 20, hwnd, (HMENU) ID_TXT_NAME3, NULL, NULL);
            SendMessage(txtName3,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtName4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server4Name), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 12, 163, 120, 20, hwnd, (HMENU) ID_TXT_NAME4, NULL, NULL);
            SendMessage(txtName4,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtName5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server5Name), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 12, 208, 120, 20, hwnd, (HMENU) ID_TXT_NAME5, NULL, NULL);
            SendMessage(txtName5,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtRealm1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server1Realm), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 147, 28, 210, 20, hwnd, (HMENU) ID_TXT_REALM1, NULL, NULL);
            SendMessage(txtRealm1,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtRealm2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server2Realm), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 147, 73, 210, 20, hwnd, (HMENU) ID_TXT_REALM2, NULL, NULL);
            SendMessage(txtRealm2,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtRealm3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server3Realm), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 147, 118, 210, 20, hwnd, (HMENU) ID_TXT_REALM3, NULL, NULL);
            SendMessage(txtRealm3,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtRealm4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server4Realm), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 147, 163, 210, 20, hwnd, (HMENU) ID_TXT_REALM4, NULL, NULL);
            SendMessage(txtRealm4,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtRealm5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server5Realm), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 147, 208, 210, 20, hwnd, (HMENU) ID_TXT_REALM5, NULL, NULL);
            SendMessage(txtRealm5,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtWebsite1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server1Website), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 374, 28, 210, 20, hwnd, (HMENU) ID_TXT_WEBSITE1, NULL, NULL);
            SendMessage(txtWebsite1,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtWebsite2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server2Website), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 374, 73, 210, 20, hwnd, (HMENU) ID_TXT_WEBSITE2, NULL, NULL);
            SendMessage(txtWebsite2,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtWebsite3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server3Website), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 374, 118, 210, 20, hwnd, (HMENU) ID_TXT_WEBSITE3, NULL, NULL);
            SendMessage(txtWebsite3,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtWebsite4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server4Website), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 374, 163, 210, 20, hwnd, (HMENU) ID_TXT_WEBSITE4, NULL, NULL);
            SendMessage(txtWebsite4,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtWebsite5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server5Website), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 374, 208, 210, 20, hwnd, (HMENU) ID_TXT_WEBSITE5, NULL, NULL);
            SendMessage(txtWebsite5,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            
            /* creating buttons */
            btnSave = CreateWindow ("BUTTON", "Save", WS_VISIBLE |WS_CHILD, 490, 240, 94, 30, hwnd, (HMENU) ID_BTN_SAVE, NULL, NULL);
            SendMessage(btnSave,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            
            break;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case ID_BTN_SAVE: /* save button */
                {
                    /* get new server data */
                    GetDlgItemText(hwnd, ID_TXT_NAME1, Server1Name, 256);
                    GetDlgItemText(hwnd, ID_TXT_NAME2, Server2Name, 256);
                    GetDlgItemText(hwnd, ID_TXT_NAME3, Server3Name, 256);
                    GetDlgItemText(hwnd, ID_TXT_NAME4, Server4Name, 256);
                    GetDlgItemText(hwnd, ID_TXT_NAME5, Server5Name, 256);
                    GetDlgItemText(hwnd, ID_TXT_REALM1, Server1Realm, 256);
                    GetDlgItemText(hwnd, ID_TXT_REALM2, Server2Realm, 256);
                    GetDlgItemText(hwnd, ID_TXT_REALM3, Server3Realm, 256);
                    GetDlgItemText(hwnd, ID_TXT_REALM4, Server4Realm, 256);
                    GetDlgItemText(hwnd, ID_TXT_REALM5, Server5Realm, 256);
                    GetDlgItemText(hwnd, ID_TXT_WEBSITE1, Server1Website, 256);
                    GetDlgItemText(hwnd, ID_TXT_WEBSITE2, Server2Website, 256);
                    GetDlgItemText(hwnd, ID_TXT_WEBSITE3, Server3Website, 256);
                    GetDlgItemText(hwnd, ID_TXT_WEBSITE4, Server4Website, 256);
                    GetDlgItemText(hwnd, ID_TXT_WEBSITE5, Server5Website, 256);
                    /* update names on main window */
                    SetDlgItemText(hwnd1, ID_RBTN_SERVER1, Server1Name);
                    SetDlgItemText(hwnd1, ID_RBTN_SERVER2, Server2Name);
                    SetDlgItemText(hwnd1, ID_RBTN_SERVER3, Server3Name);
                    SetDlgItemText(hwnd1, ID_RBTN_SERVER4, Server4Name);
                    SetDlgItemText(hwnd1, ID_RBTN_SERVER5, Server5Name);
                    /* saving to file */
                    WriteServersList("launcher.cfg");
                    /* hide the configuration window */
                    ShowWindow(hwnd,SW_HIDE);
                    EnableWindow(hwnd1, true);
                    SetForegroundWindow(hwnd1);
                    
                    break;
                }
            }
            break;
        }
        case WM_CTLCOLORSTATIC:
        {
            /* setting the label colors */
            HDC hdcStatic = (HDC) wParam;
            SetTextColor(hdcStatic, RGB(0,0,0));
            SetBkColor(hdcStatic, RGB(236, 233, 216));

            if (hbrBkgnd == NULL)
            {
                hbrBkgnd = CreateSolidBrush(RGB(236, 233, 216));
            }
            return (INT_PTR)hbrBkgnd;
        }
        case WM_CLOSE:
        {
            /* hide window when pressing close button instead of closing it */
            ShowWindow(hwnd,SW_HIDE);
            EnableWindow(hwnd1, true);
            SetForegroundWindow(hwnd1);
            return true;
        }
        break;
    }

    return DefWindowProc(hwnd,msg,wParam,lParam);
}

