#include <dirent.h>
#include "resource.h"
#include "namestrip.h"
#include "direxists.h"
#include <fstream>
#include <cstring>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure2(HWND, UINT, WPARAM, LPARAM);

int rboption = 6; //which radio button is currently selected

/* Set starting coordinates for the server list window to be center of screen */
const int ScreenX = (GetSystemMetrics(SM_CXSCREEN) - 603) / 2;
const int ScreenY = (GetSystemMetrics(SM_CYSCREEN) - 313) / 2;

//default servers
char Server1Name[MAX_PATH] = "Eternal-WoW";
char Server1Realm[MAX_PATH] = "logon.eternal-wow.com";
char Server1Website[MAX_PATH] = "http://eternal-wow.com";
char Server2Name[MAX_PATH] = "Gamer-District";
char Server2Realm[MAX_PATH] = "wow.gamer-district.org";
char Server2Website[MAX_PATH] = "https://www.gamer-district.org";
char Server3Name[MAX_PATH] = "Land of Elves";
char Server3Realm[MAX_PATH] = "landofelves.net";
char Server3Website[MAX_PATH] = "https://landofelves.net";
char Server4Name[MAX_PATH] = "WoW Circle";
char Server4Realm[MAX_PATH] = "logon.wowcircle.com";
char Server4Website[MAX_PATH] = "http://www.wowcircle.com";
char Server5Name[MAX_PATH] = "Warmane (Molten)";
char Server5Realm[MAX_PATH] = "logon.warmane.com";
char Server5Website[MAX_PATH] = "https://www.warmane.com";

HWND hwnd1;               /* This is the handle for the main window */
HWND hwnd2;               /* This is the handle for the server list window */

/* Handles for all the different controls */
HWND txtRealmlist, txtLanguage, rbtnNoChange, rbtnLocalhost, rbtnServer1, rbtnServer2, rbtnServer3, rbtnServer4, rbtnServer5, rbtnSetTo, lblRealmlist, lblLanguage, btnStart, btnOpenDirectory, btnWebsite, btnCache, btnExit, lblName1, lblName2, lblName3, lblName4, lblName5, lblRealm1, lblRealm2, lblRealm3, lblRealm4, lblRealm5, lblWebsite1, lblWebsite2, lblWebsite3, lblWebsite4, lblWebsite5, txtName1, txtName2, txtName3, txtName4, txtName5, txtRealm1, txtRealm2, txtRealm3, txtRealm4, txtRealm5, txtWebsite1, txtWebsite2, txtWebsite3, txtWebsite4, txtWebsite5, btnSave;

/*  Make the class name into a global variable  */
char szClassName[ ] = "MainWindow";
char szClassName2[ ] = "ServerList";

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the window class */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Set icon and mouse-pointer */
    wincl.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_MYICON));
    wincl.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_MYICON));
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
           "WoW Launcher by brotalnia for WotLK",       /* Title Text */
           WS_SYSMENU | WS_MINIMIZEBOX, /* window style */
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
    
    /* Server List Window */
    WNDCLASSEX wincl2;       /* Data structure for the windowclass */
    
    ZeroMemory(&wincl2,sizeof(WNDCLASSEX));
    
    /* The Window structure */
    wincl2.hInstance=hThisInstance;
    wincl2.lpszClassName=szClassName2;
    wincl2.lpfnWndProc=WindowProcedure2;
    wincl2.style=CS_HREDRAW|CS_VREDRAW;
    wincl2.cbSize=sizeof(WNDCLASSEX);
    
    /* Set icon and mouse-pointer */
    wincl2.hIcon=LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_MYICOC));
    wincl2.hIconSm=LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_MYICOC));
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
           WS_OVERLAPPED | WS_SYSMENU, /* window style */
           ScreenX,       /* Windows decides the position */
           ScreenY,       /* where the window ends up on the screen */
           603,                 /* The programs width */
           313,                 /* and height in pixels */
           hwnd1,               /* The window is a child-window of the main window */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
           

    if(!hwnd2)
    {
        MessageBox(NULL, "Window creation failed","Window creation failed", MB_OK | MB_ICONERROR);
    }

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        if(!IsDialogMessage(hwnd2, &messages)) // allows selecting controls with tab
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

/* we need the locale for the realmlist and cache directory */
const char* GetLocale()
{
    char ownPth[MAX_PATH];
    GetModuleFileName(GetModuleHandle(NULL),ownPth, (sizeof(ownPth))); 
    path_strip_filename(ownPth);
    
    char realmDir[MAX_PATH];
    strcpy(realmDir, ownPth);
    std::strcat(realmDir,"\\Data\\enGB");
    if (dirExists(realmDir))
    {
        return "enGB";
    }
    strcpy(realmDir, ownPth);
    std::strcat(realmDir,"\\Data\\enUS");
    if (dirExists(realmDir))
    {
        return "enUS";
    }
    strcpy(realmDir, ownPth);
    std::strcat(realmDir,"\\Data\\esES");
    if (dirExists(realmDir))
    {
        return "esES";
    }
    strcpy(realmDir, ownPth);
    std::strcat(realmDir,"\\Data\\esMX");
    if (dirExists(realmDir))
    {
        return "esMX";
    }
    strcpy(realmDir, ownPth);
    std::strcat(realmDir,"\\Data\\ruRU");
    if (dirExists(realmDir))
    {
        return "ruRU";
    }
    strcpy(realmDir, ownPth);
    std::strcat(realmDir,"\\Data\\frFR");
    if (dirExists(realmDir))
    {
        return "frFR";
    }
    strcpy(realmDir, ownPth);
    std::strcat(realmDir,"\\Data\\deDE");
    if (dirExists(realmDir))
    {
        return "deDE";
    }
    strcpy(realmDir, ownPth);
    std::strcat(realmDir,"\\Data\\zhCN");
    if (dirExists(realmDir))
    {
        return "zhCN";
    }
    strcpy(realmDir, ownPth);
    std::strcat(realmDir,"\\Data\\zhTW");
    if (dirExists(realmDir))
    {
        return "zhTW";
    }
    strcpy(realmDir, ownPth);
    std::strcat(realmDir,"\\Data\\koKR");
    if (dirExists(realmDir))
    {
        return "koKR";
    }
    
    return "enGB";
    
}

void WriteRealmlist(const char* realm)
{
    char locale[5];
    GetDlgItemText(hwnd1, 32, locale, 5);
    char realmPath[MAX_PATH] = "Data\\";
    std::strcat(realmPath,locale);
    std::strcat(realmPath,"\\realmlist.wtf");
    SetFileAttributes(realmPath, FILE_ATTRIBUTE_NORMAL);
    char realmfinal[254] = "set realmlist ";
    std::strcat(realmfinal,realm);
    std::ofstream myfile;
    myfile.open (realmPath);
    myfile << realmfinal;
    myfile.close();
}

static HBRUSH hbrBkgnd = NULL; //used for the label colors
HFONT hFont = NULL; //used for the label fonts

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
        {
            //creating labels and setting their font
            lblRealmlist = CreateWindow ("STATIC", " Realmlist:", WS_VISIBLE | WS_CHILD | SS_NOTIFY, 5, 3, 84, 20, hwnd, (HMENU) 15, NULL, NULL);
            hFont=CreateFont (20, 0, 0, 0, 700, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Microsoft Sans Serif");
            SendMessage (lblRealmlist, WM_SETFONT, WPARAM (hFont), TRUE);
            lblLanguage = CreateWindow ("STATIC", "Language:", WS_VISIBLE | WS_CHILD, 215, 195, 50, 13, hwnd, NULL, NULL, NULL);
            hFont=CreateFont (14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Microsoft Sans Serif");
            SendMessage (lblLanguage, WM_SETFONT, WPARAM (hFont), TRUE);
            //creating the buttons and setting their font
            btnStart = CreateWindow ("BUTTON", "Start", WS_VISIBLE |WS_CHILD, 243, 12, 100, 28, hwnd, (HMENU) 1, NULL, NULL);
            SendMessage(btnStart,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            btnOpenDirectory = CreateWindow ("BUTTON", "Open Directory", WS_VISIBLE |WS_CHILD, 243, 46, 100, 28, hwnd, (HMENU) 2, NULL, NULL);
            SendMessage(btnOpenDirectory,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            btnWebsite = CreateWindow ("BUTTON", "Website", WS_VISIBLE |WS_CHILD, 243, 80, 100, 28, hwnd, (HMENU) 3, NULL, NULL);
            SendMessage(btnWebsite,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            btnCache = CreateWindow ("BUTTON", "Cache", WS_VISIBLE |WS_CHILD, 243, 114, 100, 28, hwnd, (HMENU) 4, NULL, NULL);
            SendMessage(btnCache,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            btnExit = CreateWindow ("BUTTON", "Exit", WS_VISIBLE |WS_CHILD, 243, 148, 100, 28, hwnd, (HMENU) 5, NULL, NULL);
            SendMessage(btnExit,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            //read saved server list
            std::ifstream in("launcher.cfg");
            if(in)
            {
                char str[255];
                while(in.getline(str,255))
                {
                    if(strstr(str,"Server1Name"))
                    {
                        strncpy(Server1Name, str+12, strlen(str)-11);
                    }
                    else if (strstr(str,"Server1Realm"))
                    {
                        strncpy(Server1Realm, str+13, strlen(str)-12);
                    }
                    else if (strstr(str,"Server1Website"))
                    {
                        strncpy(Server1Website, str+15, strlen(str)-14);
                    }
                    else if(strstr(str,"Server2Name"))
                    {
                        strncpy(Server2Name, str+12, strlen(str)-11);
                    }
                    else if (strstr(str,"Server2Realm"))
                    {
                        strncpy(Server2Realm, str+13, strlen(str)-12);
                    }
                    else if (strstr(str,"Server2Website"))
                    {
                        strncpy(Server2Website, str+15, strlen(str)-14);
                    }
                    else if(strstr(str,"Server3Name"))
                    {
                        strncpy(Server3Name, str+12, strlen(str)-11);
                    }
                    else if (strstr(str,"Server3Realm"))
                    {
                        strncpy(Server3Realm, str+13, strlen(str)-12);
                    }
                    else if (strstr(str,"Server3Website"))
                    {
                        strncpy(Server3Website, str+15, strlen(str)-14);
                    }
                    else if(strstr(str,"Server4Name"))
                    {
                        strncpy(Server4Name, str+12, strlen(str)-11);
                    }
                    else if (strstr(str,"Server4Realm"))
                    {
                        strncpy(Server4Realm, str+13, strlen(str)-12);
                    }
                    else if (strstr(str,"Server4Website"))
                    {
                        strncpy(Server4Website, str+15, strlen(str)-14);
                    }
                    else if(strstr(str,"Server5Name"))
                    {
                        strncpy(Server5Name, str+12, strlen(str)-11);
                    }
                    else if (strstr(str,"Server5Realm"))
                    {
                        strncpy(Server5Realm, str+13, strlen(str)-12);
                    }
                    else if (strstr(str,"Server5Website"))
                    {
                        strncpy(Server5Website, str+15, strlen(str)-14);
                    }
                }
                in.close();
            }
            //creating the radio buttons and setting their font
            HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
            rbtnNoChange = CreateWindowEx(0,  "BUTTON",  "No Change",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 31,   140,  17,  hwnd,  (HMENU) 6,  hInstance,  NULL);
            SendMessage(rbtnNoChange,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            SendDlgItemMessage(hwnd, 6, BM_SETCHECK, 1, 0);
            rbtnLocalhost = CreateWindowEx(0,  "BUTTON",  "Localhost",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 54,   140,  17,  hwnd,  (HMENU) 7,  hInstance,  NULL);
            SendMessage(rbtnLocalhost,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnServer1 = CreateWindowEx(0,  "BUTTON",  Server1Name,  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 77,   140,  17,  hwnd,  (HMENU) 8,  hInstance,  NULL);
            SendMessage(rbtnServer1,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnServer2 = CreateWindowEx(0,  "BUTTON",  Server2Name,  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 100,   140,  17,  hwnd,  (HMENU) 9,  hInstance,  NULL);
            SendMessage(rbtnServer2,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnServer3 = CreateWindowEx(0,  "BUTTON",  Server3Name,  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 123,   140,  17,  hwnd,  (HMENU) 10,  hInstance,  NULL);
            SendMessage(rbtnServer3,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnServer4 = CreateWindowEx(0,  "BUTTON",  Server4Name,  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 146,   140,  17,  hwnd,  (HMENU) 11,  hInstance,  NULL);
            SendMessage(rbtnServer4,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnServer5 = CreateWindowEx(0,  "BUTTON",  Server5Name,  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 169,   140,  17,  hwnd,  (HMENU) 12,  hInstance,  NULL);
            SendMessage(rbtnServer5,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            rbtnSetTo = CreateWindowEx(0,  "BUTTON",  "Set to:",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 195,   52,  17,  hwnd,  (HMENU) 13,  hInstance,  NULL);
            SendMessage(rbtnSetTo,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            //creating textboxes
            txtRealmlist = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 65, 192, 130, 20, hwnd, (HMENU) 14, NULL, NULL);
            SendMessage(txtRealmlist,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            EnableWindow(txtRealmlist, FALSE); //Set To textbox is disabled by default
            txtLanguage = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(GetLocale()), WS_CHILD | WS_VISIBLE, 274, 192, 92, 20, hwnd, (HMENU) 32, NULL, NULL);
            SendMessage(txtLanguage,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            
            break;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case 1: //start button
                {
                    switch (rboption) //check which radio button is selected
                    {
                        case 6:
                            //no change
                            break;
                        case 7:
                        {
                            //localhost
                            WriteRealmlist("127.0.0.1\n");
                            break;
                        }
                        case 8:
                        {
                            //server1
                            WriteRealmlist(Server1Realm);
                            break;
                        }
                        case 9:
                        {
                            //server2
                            WriteRealmlist(Server2Realm);
                            break;
                        }
                        case 10:
                        {
                            //server3
                            WriteRealmlist(Server3Realm);
                            break;
                        }
                        case 11:
                        {
                            //server4
                            WriteRealmlist(Server4Realm);
                            break;
                        }
                        case 12:
                        {
                            //server5
                            WriteRealmlist(Server5Realm);
                            break;
                        }
                        case 13:
                        {
                            //set to
                            char setrealm[254];
                            GetDlgItemText(hwnd, 14, setrealm, 254);
                            WriteRealmlist(setrealm);
                            break;
                        }
                    }
                    Sleep(1000);
                    char ownPth[MAX_PATH]; 
                    if (GetModuleHandle(NULL) != NULL)
                    {
                        // When passing NULL to GetModuleHandle, it returns handle of exe itself
                        GetModuleFileName(GetModuleHandle(NULL),ownPth, (sizeof(ownPth))); 
                        path_strip_filename(ownPth);
                        std::strcat(ownPth,"\\Wow.exe");
                        // Use above module handle to get the path using GetModuleFileName()
                        bool erretry = false;
                        do
                        {
                            erretry = false;
                            int success = (int) ShellExecute(hwnd, "open", ownPth, NULL, NULL, SW_SHOWDEFAULT);
                            if (success < 32) //check if game has launched successfully
                            {
                                int errorchoice = MessageBox(hwnd, "Unable to launch World of Warcraft.","WoW Launcher", MB_RETRYCANCEL | MB_DEFBUTTON2 | MB_ICONERROR);
                                if (errorchoice==4)
                                {
                                    erretry = true;
                                }
                            }
                            else
                            {
                                exit(0); //close program once game has launched
                            }
                        } while (erretry);
                    }
                    break;
                }
                case 2: //open directory button
                {
                    char ownPth[MAX_PATH]; 
                    if (GetModuleHandle(NULL) != NULL)
                    {
                        // When passing NULL to GetModuleHandle, it returns handle of exe itself
                        GetModuleFileName(GetModuleHandle(NULL),ownPth, (sizeof(ownPth))); 
                        path_strip_filename(ownPth);
                        // Use above module handle to get the path using GetModuleFileName()
                        ShellExecute(NULL, "open", ownPth, NULL, NULL, SW_SHOWDEFAULT);
                    }
                    break;
                }
                case 3: //website button
                {
                    switch (rboption)
                    {
                        case 6:
                        case 13:
                            //no change or set to
                            MessageBox(hwnd, "Select a server to view its website.","WoW Launcher", MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION);
                            break;
                        case 7:
                            //localhost
                            ShellExecute(NULL, "open", "http://127.0.0.1", NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        case 8:
                            //server1
                            ShellExecute(NULL, "open", Server1Website, NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        case 9:
                            //server2
                            ShellExecute(NULL, "open", Server2Website, NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        case 10:
                            //server3
                            ShellExecute(NULL, "open", Server3Website, NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        case 11:
                            //server4
                            ShellExecute(NULL, "open", Server4Website, NULL, NULL, SW_SHOWDEFAULT);
                            break;
                        case 12:
                            //server5
                            ShellExecute(NULL, "open", Server5Website, NULL, NULL, SW_SHOWDEFAULT);
                            break;
                    }
                    break;
                }
                case 4: //clear cache button
                {
                    if (MessageBox(hwnd, "Do you want to delete the cache?","WoW Launcher", MB_YESNO | MB_DEFBUTTON1 | MB_ICONQUESTION)==IDYES)
                    {
                        char filePath[MAX_PATH]; 
                        GetModuleFileName(GetModuleHandle(NULL),filePath, (sizeof(filePath))); 
                        path_strip_filename(filePath);
                        char locale[5];
                        GetDlgItemText(hwnd, 32, locale, 5);
                        std::strcat(filePath,"\\Cache\\WDB\\");
                        std::strcat(filePath,locale);
                        DIR *theFolder = opendir(filePath);
                        struct dirent *next_file;
                        char filepath[256];

                        // reading all files in folder
                        while ( (next_file = readdir(theFolder)) != NULL )
                        {
                            // check if it is a wdb file
                            if(strstr(next_file->d_name,".wdb"))
                            {
                                // build the path for each file in the folder
                                sprintf(filepath, "%s\\%s", filePath, next_file->d_name);
                                remove(filepath);
                            }
                        }
                        closedir(theFolder);
                    }
                    break;
                }
                case 5: //exit button
                {
                    exit(0);
                    break;
                }
                case 6: //no change radio button
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            if(IsDlgButtonChecked(hwnd, 6) == BST_CHECKED)
                            {
                                EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                                rboption = 6; //storing selected server
                            }
                            break; 
                        }
                    }
                    break;
                }
                case 7: //localhost radio button
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            if(IsDlgButtonChecked(hwnd, 7) == BST_CHECKED)
                            {
                                EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                                rboption = 7; //storing selected server
                            }
                            break;
                        }
                    }
                    break;
                }
                case 8: //server1 radio button
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            if(IsDlgButtonChecked(hwnd, 8) == BST_CHECKED)
                            {
                                EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                                rboption = 8; //storing selected server
                            }
                            break; 
                        }
                    }
                    break;
                }
                case 9: //server2 radio button
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            if(IsDlgButtonChecked(hwnd, 9) == BST_CHECKED)
                            {
                                EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                                rboption = 9; //storing selected server
                            }
                            break; 
                        }
                    }
                    break;
                }
                case 10: //server3 radio button
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            if(IsDlgButtonChecked(hwnd, 10) == BST_CHECKED)
                            {
                                EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                                rboption = 10; //storing selected server
                            }
                            break;
                        }
                    }
                    break;
                }
                case 11: //server4 radio button
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            if(IsDlgButtonChecked(hwnd, 11) == BST_CHECKED)
                            {
                                EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                                rboption = 11; //storing selected server
                            }
                            break;
                        }
                    }
                    break;
                }
                case 12: //server5 radio button
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            if(IsDlgButtonChecked(hwnd, 12) == BST_CHECKED)
                            {
                                EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                                rboption = 12; //storing selected server
                            }
                            break;
                        }
                    }
                    break;
                }
                case 13: //set to radio button
                {
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            if(IsDlgButtonChecked(hwnd, 13) == BST_CHECKED)
                            {
                                EnableWindow(txtRealmlist, TRUE); //disable textbox next to Set To radio button if it is not selected
                                rboption = 13; //storing selected server
                            }
                            break;
                        }
                    }
                    break;
                }
                case 15: //realmlist label
                {
                    //check for double click
                    if(HIWORD(wParam)==STN_DBLCLK)
                    {
                        //show the server list window
                        ShowWindow(hwnd2,SW_SHOWNORMAL);
                        EnableWindow(hwnd1, false);
                    }
                    break;
                }
            }
            break;
        } // end of case WM_COMMAND
        case WM_CTLCOLORSTATIC:
        {
            //setting the label colors
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

            HPEN hPenOld;

            // Drawing the black separator line in the middle
            HPEN hLinePen;
            COLORREF qLineColor;
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
            //creating labels
            hFont=CreateFont (14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Microsoft Sans Serif");
            lblName1 = CreateWindow ("STATIC", "Name 1", WS_VISIBLE | WS_CHILD, 15, 10, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblName1, WM_SETFONT, WPARAM (hFont), TRUE);
            lblName2 = CreateWindow ("STATIC", "Name 2", WS_VISIBLE | WS_CHILD, 15, 55, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblName2, WM_SETFONT, WPARAM (hFont), TRUE);
            lblName3 = CreateWindow ("STATIC", "Name 3", WS_VISIBLE | WS_CHILD, 15, 100, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblName3, WM_SETFONT, WPARAM (hFont), TRUE);
            lblName4 = CreateWindow ("STATIC", "Name 4", WS_VISIBLE | WS_CHILD, 15, 145, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblName4, WM_SETFONT, WPARAM (hFont), TRUE);
            lblName5 = CreateWindow ("STATIC", "Name 5", WS_VISIBLE | WS_CHILD, 15, 190, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblName5, WM_SETFONT, WPARAM (hFont), TRUE);
            lblRealm1 = CreateWindow ("STATIC", "Realmlist 1", WS_VISIBLE | WS_CHILD, 150, 10, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblRealm1, WM_SETFONT, WPARAM (hFont), TRUE);
            lblRealm2 = CreateWindow ("STATIC", "Realmlist 2", WS_VISIBLE | WS_CHILD, 150, 55, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblRealm2, WM_SETFONT, WPARAM (hFont), TRUE);
            lblRealm3 = CreateWindow ("STATIC", "Realmlist 3", WS_VISIBLE | WS_CHILD, 150, 100, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblRealm3, WM_SETFONT, WPARAM (hFont), TRUE);
            lblRealm4 = CreateWindow ("STATIC", "Realmlist 4", WS_VISIBLE | WS_CHILD, 150, 145, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblRealm4, WM_SETFONT, WPARAM (hFont), TRUE);
            lblRealm5 = CreateWindow ("STATIC", "Realmlist 5", WS_VISIBLE | WS_CHILD, 150, 190, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblRealm5, WM_SETFONT, WPARAM (hFont), TRUE);
            lblWebsite1 = CreateWindow ("STATIC", "Website 1", WS_VISIBLE | WS_CHILD, 377, 10, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblWebsite1, WM_SETFONT, WPARAM (hFont), TRUE);
            lblWebsite2 = CreateWindow ("STATIC", "Website 2", WS_VISIBLE | WS_CHILD, 377, 55, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblWebsite2, WM_SETFONT, WPARAM (hFont), TRUE);
            lblWebsite3 = CreateWindow ("STATIC", "Website 3", WS_VISIBLE | WS_CHILD, 377, 100, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblWebsite3, WM_SETFONT, WPARAM (hFont), TRUE);
            lblWebsite4 = CreateWindow ("STATIC", "Website 4", WS_VISIBLE | WS_CHILD, 377, 145, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblWebsite4, WM_SETFONT, WPARAM (hFont), TRUE);
            lblWebsite5 = CreateWindow ("STATIC", "Website 5", WS_VISIBLE | WS_CHILD, 377, 190, 70, 13, hwnd, NULL, NULL, NULL);
            SendMessage (lblWebsite5, WM_SETFONT, WPARAM (hFont), TRUE);
            //creating textboxes
            txtName1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server1Name), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 12, 28, 120, 20, hwnd, (HMENU) 16, NULL, NULL);
            SendMessage(txtName1,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtName2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server2Name), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 12, 73, 120, 20, hwnd, (HMENU) 17, NULL, NULL);
            SendMessage(txtName2,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtName3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server3Name), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 12, 118, 120, 20, hwnd, (HMENU) 18, NULL, NULL);
            SendMessage(txtName3,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtName4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server4Name), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 12, 163, 120, 20, hwnd, (HMENU) 19, NULL, NULL);
            SendMessage(txtName4,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtName5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server5Name), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 12, 208, 120, 20, hwnd, (HMENU) 20, NULL, NULL);
            SendMessage(txtName5,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtRealm1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server1Realm), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 147, 28, 210, 20, hwnd, (HMENU) 21, NULL, NULL);
            SendMessage(txtRealm1,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtRealm2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server2Realm), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 147, 73, 210, 20, hwnd, (HMENU) 22, NULL, NULL);
            SendMessage(txtRealm2,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtRealm3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server3Realm), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 147, 118, 210, 20, hwnd, (HMENU) 23, NULL, NULL);
            SendMessage(txtRealm3,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtRealm4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server4Realm), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 147, 163, 210, 20, hwnd, (HMENU) 24, NULL, NULL);
            SendMessage(txtRealm4,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtRealm5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server5Realm), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 147, 208, 210, 20, hwnd, (HMENU) 25, NULL, NULL);
            SendMessage(txtRealm5,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtWebsite1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server1Website), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 374, 28, 210, 20, hwnd, (HMENU) 26, NULL, NULL);
            SendMessage(txtWebsite1,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtWebsite2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server2Website), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 374, 73, 210, 20, hwnd, (HMENU) 27, NULL, NULL);
            SendMessage(txtWebsite2,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtWebsite3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server3Website), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 374, 118, 210, 20, hwnd, (HMENU) 28, NULL, NULL);
            SendMessage(txtWebsite3,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtWebsite4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server4Website), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 374, 163, 210, 20, hwnd, (HMENU) 29, NULL, NULL);
            SendMessage(txtWebsite4,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            txtWebsite5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(Server5Website), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 374, 208, 210, 20, hwnd, (HMENU) 30, NULL, NULL);
            SendMessage(txtWebsite5,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            //creating buttons
            btnSave = CreateWindow ("BUTTON", "Save", WS_VISIBLE |WS_CHILD, 490, 240, 94, 30, hwnd, (HMENU) 31, NULL, NULL);
            SendMessage(btnSave,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
            
            break;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case 31: //save button
                {
                    //get new server data
                    GetDlgItemText(hwnd, 16, Server1Name, 254);
                    GetDlgItemText(hwnd, 17, Server2Name, 254);
                    GetDlgItemText(hwnd, 18, Server3Name, 254);
                    GetDlgItemText(hwnd, 19, Server4Name, 254);
                    GetDlgItemText(hwnd, 20, Server5Name, 254);
                    GetDlgItemText(hwnd, 21, Server1Realm, 254);
                    GetDlgItemText(hwnd, 22, Server2Realm, 254);
                    GetDlgItemText(hwnd, 23, Server3Realm, 254);
                    GetDlgItemText(hwnd, 24, Server4Realm, 254);
                    GetDlgItemText(hwnd, 25, Server5Realm, 254);
                    GetDlgItemText(hwnd, 26, Server1Website, 254);
                    GetDlgItemText(hwnd, 27, Server2Website, 254);
                    GetDlgItemText(hwnd, 28, Server3Website, 254);
                    GetDlgItemText(hwnd, 29, Server4Website, 254);
                    GetDlgItemText(hwnd, 30, Server5Website, 254);
                    //update names on main window
                    SetDlgItemText(hwnd1, 8, Server1Name);
                    SetDlgItemText(hwnd1, 9, Server2Name);
                    SetDlgItemText(hwnd1, 10, Server3Name);
                    SetDlgItemText(hwnd1, 11, Server4Name);
                    SetDlgItemText(hwnd1, 12, Server5Name);
                    //saving to file
                    char configData[1024];
                    sprintf(configData, "Server1Name=%s\nServer1Realm=%s\nServer1Website=%s\nServer2Name=%s\nServer2Realm=%s\nServer2Website=%s\nServer3Name=%s\nServer3Realm=%s\nServer3Website=%s\nServer4Name=%s\nServer4Realm=%s\nServer4Website=%s\nServer5Name=%s\nServer5Realm=%s\nServer5Website=%s", Server1Name, Server1Realm, Server1Website, Server2Name, Server2Realm, Server2Website, Server3Name, Server3Realm, Server3Website, Server4Name, Server4Realm, Server4Website, Server5Name, Server5Realm, Server5Website);
                    std::ofstream myfile;
                    myfile.open ("launcher.cfg");
                    myfile << configData;
                    myfile.close();
                    //hide the configuration window
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
            //setting the label colors
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
            //hide window when pressing close button instead of closing it
            ShowWindow(hwnd,SW_HIDE);
            EnableWindow(hwnd1, true);
            SetForegroundWindow(hwnd1);
            return true;
        }
        break;
    }

    return DefWindowProc(hwnd,msg,wParam,lParam);
}
