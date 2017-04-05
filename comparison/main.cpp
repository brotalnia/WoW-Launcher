#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"
#include "namestrip.h"
#include "direxists.h"
#include <iostream>
#include <fstream>
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

int rboption = 6;
bool foundlocale = false;
HWND txtRealmlist, txtLanguage, rbtnNoChange, rbtnLocalhost, rbtnZhyper, rbtnGDistrict, rbtnLandofElves, rbtnCircle, rbtnMoltenWoW, rbtnSetTo, lblRealmlist, lblLanguage, btnStart, btnOpenDirectory, btnWebsite, btnAbout, btnExit;

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_MYICON));
    wincl.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_MYICON));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(236, 233, 216));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "WoW Launcher by brotalnia for WotLK",       /* Title Text */
           WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
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
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */
static HBRUSH hbrBkgnd = NULL; //used for the label colors
HFONT hFont = NULL; //used for the label fonts
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
	HDC hdc;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
             {
             //creating labels and settting their font
             lblRealmlist = CreateWindow ("STATIC", " Realmlist:", WS_VISIBLE | WS_CHILD, 5, 3, 84, 20, hwnd, NULL, NULL, NULL);
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
             btnAbout = CreateWindow ("BUTTON", "About", WS_VISIBLE |WS_CHILD, 243, 114, 100, 28, hwnd, (HMENU) 4, NULL, NULL);
             SendMessage(btnAbout,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             btnExit = CreateWindow ("BUTTON", "Exit", WS_VISIBLE |WS_CHILD, 243, 148, 100, 28, hwnd, (HMENU) 5, NULL, NULL);
             SendMessage(btnExit,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             //creating the radio buttons and setting their font
             HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
             rbtnNoChange = CreateWindowEx(0,  "BUTTON",  "No Change",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 31,   80,  17,  hwnd,  (HMENU) 6,  hInstance,  NULL);
             SendMessage(rbtnNoChange,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             SendDlgItemMessage(hwnd, 6, BM_SETCHECK, 1, 0);
             rbtnLocalhost = CreateWindowEx(0,  "BUTTON",  "Localhost",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 54,   71,  17,  hwnd,  (HMENU) 7,  hInstance,  NULL);
             SendMessage(rbtnLocalhost,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             rbtnZhyper = CreateWindowEx(0,  "BUTTON",  "Zhyper-Pwnage",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 77,   100,  17,  hwnd,  (HMENU) 8,  hInstance,  NULL);
             SendMessage(rbtnZhyper,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             rbtnGDistrict = CreateWindowEx(0,  "BUTTON",  "Gamer-District",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 100,   91,  17,  hwnd,  (HMENU) 9,  hInstance,  NULL);
             SendMessage(rbtnGDistrict,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             rbtnLandofElves = CreateWindowEx(0,  "BUTTON",  "Land of Elves",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 123,   90,  17,  hwnd,  (HMENU) 10,  hInstance,  NULL);
             SendMessage(rbtnLandofElves,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             rbtnCircle = CreateWindowEx(0,  "BUTTON",  "WoW Circle",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 146,   82,  17,  hwnd,  (HMENU) 11,  hInstance,  NULL);
             SendMessage(rbtnCircle,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             rbtnMoltenWoW = CreateWindowEx(0,  "BUTTON",  "Warmane (Molten)",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 169,   112,  17,  hwnd,  (HMENU) 12,  hInstance,  NULL);
             SendMessage(rbtnMoltenWoW,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             rbtnSetTo = CreateWindowEx(0,  "BUTTON",  "Set to:",  WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  12, 195,   52,  17,  hwnd,  (HMENU) 13,  hInstance,  NULL);
             SendMessage(rbtnSetTo,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             //creating textboxes
             txtRealmlist = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 65, 192, 130, 20, hwnd, (HMENU) 14, NULL, NULL);
             SendMessage(txtRealmlist,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             EnableWindow(txtRealmlist, FALSE); //Set To textbox disabled by default
             txtLanguage = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("enGB"), WS_CHILD | WS_VISIBLE, 274, 192, 92, 20, hwnd, (HMENU) 15, NULL, NULL);
             SendMessage(txtLanguage,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(1,0));
             //checking game locale and setting correct language in textbox 
             if (GetModuleHandle(NULL) != NULL)
              {
              if (!foundlocale)
              {
                char ownPth[MAX_PATH];
               // When passing NULL to GetModuleHandle, it returns handle of exe itself
                GetModuleFileName(GetModuleHandle(NULL),ownPth, (sizeof(ownPth))); 
                path_strip_filename(ownPth);
                // Use above module handle to get the path using GetModuleFileName()
                char* eudir = ownPth;
                std::strcat(eudir,"\\Data\\enGB\\");
                if (dirExists(eudir))
                {
                foundlocale=true; //no need to change txtLanguage value here cause it's enGB by default
                }
               }
              if (!foundlocale)
              {
                char ownPth[MAX_PATH];
               // When passing NULL to GetModuleHandle, it returns handle of exe itself
                GetModuleFileName(GetModuleHandle(NULL),ownPth, (sizeof(ownPth))); 
                path_strip_filename(ownPth);
                // Use above module handle to get the path using GetModuleFileName()
                char* usdir = ownPth;
                std::strcat(usdir,"\\Data\\enUS\\");
                if (dirExists(usdir))
                {
                SetDlgItemText(hwnd, 15, "enUS");
                foundlocale=true;
                }
               }
             if (!foundlocale)
              {
                char ownPth[MAX_PATH];
               // When passing NULL to GetModuleHandle, it returns handle of exe itself
                GetModuleFileName(GetModuleHandle(NULL),ownPth, (sizeof(ownPth))); 
                path_strip_filename(ownPth);
                // Use above module handle to get the path using GetModuleFileName()
                char* dedir = ownPth;
                std::strcat(dedir,"\\Data\\deDE\\");
                if (dirExists(dedir))
                {
                SetDlgItemText(hwnd, 15, "deDE");
                foundlocale=true;
                }
               }
             if (!foundlocale)
              {
                char ownPth[MAX_PATH];
               // When passing NULL to GetModuleHandle, it returns handle of exe itself
                GetModuleFileName(GetModuleHandle(NULL),ownPth, (sizeof(ownPth))); 
                path_strip_filename(ownPth);
                // Use above module handle to get the path using GetModuleFileName()
                char* frdir = ownPth;
                std::strcat(frdir,"\\Data\\frFR\\");
                if (dirExists(frdir))
                {
                SetDlgItemText(hwnd, 15, "frFR");
                foundlocale=true;
                }
               }
             if (!foundlocale)
              {
                char ownPth[MAX_PATH];
               // When passing NULL to GetModuleHandle, it returns handle of exe itself
                GetModuleFileName(GetModuleHandle(NULL),ownPth, (sizeof(ownPth))); 
                path_strip_filename(ownPth);
                // Use above module handle to get the path using GetModuleFileName()
                char* esdir = ownPth;
                std::strcat(esdir,"\\Data\\esES\\");
                if (dirExists(esdir))
                {
                SetDlgItemText(hwnd, 15, "esES");
                foundlocale=true;
                }
               }
             if (!foundlocale)
              {
                char ownPth[MAX_PATH];
               // When passing NULL to GetModuleHandle, it returns handle of exe itself
                GetModuleFileName(GetModuleHandle(NULL),ownPth, (sizeof(ownPth))); 
                path_strip_filename(ownPth);
                // Use above module handle to get the path using GetModuleFileName()
                char* rudir = ownPth;
                std::strcat(rudir,"\\Data\\ruRU\\");
                if (dirExists(rudir))
                {
                SetDlgItemText(hwnd, 15, "ruRU");
                foundlocale=true;
                }
               }
             }
             }
             break;
        case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1: //start button
             {
             char locale[254];
             GetDlgItemText(hwnd, 15, locale, 254);
             char realmPath[MAX_PATH] = "";
             std::strcat(realmPath,"Data\\");
             std::strcat(realmPath,locale);
             std::strcat(realmPath,"\\realmlist.wtf");
             SetFileAttributes(realmPath, FILE_ATTRIBUTE_NORMAL);
             switch (rboption)
             {
                    case 6:
                         //no change
                         break;
                    case 7:
                         {
                         std::ofstream myfile;
                         myfile.open (realmPath);
                         myfile << "set realmlist 127.0.0.1\n";
                         myfile.close();
                         }
                         break;
                    case 8:
                         {
                         std::ofstream myfile;
                         myfile.open (realmPath);
                         myfile << "set realmlist logon.zhyper-pwnage.com\n";
                         myfile.close();
                         }
                         break;
                    case 9:
                         {
                         std::ofstream myfile;
                         myfile.open (realmPath);
                         myfile << "set realmlist wow.gamer-district.org\n";
                         myfile.close();
                         }
                         break;
                    case 10:
                         {
                         std::ofstream myfile;
                         myfile.open (realmPath);
                         myfile << "set realmlist landofelves.net\n";
                         myfile.close();
                         }
                         break;
                    case 11:
                         {
                         std::ofstream myfile;
                         myfile.open (realmPath);
                         myfile << "set realmlist logon.wowcircle.com\n";
                         myfile.close();
                         }
                         break;
                    case 12:
                         {
                         std::ofstream myfile;
                         myfile.open (realmPath);
                         myfile << "set realmlist logon.warmane.com\n";
                         myfile.close();
                         }
                         break;
                    case 13:
                         {
                         char setrealm[254];
                         GetDlgItemText(hwnd, 14, setrealm, 254);
                         char realmfinal[254] = "set realmlist ";
                         std::strcat(realmfinal,setrealm);
                         std::ofstream myfile;
                         myfile.open (realmPath);
                         myfile << realmfinal;
                         myfile.close();
                         }
                         break;
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
                           int errorchoice = MessageBox(NULL, "Unable to launch World of Warcraft.","Error", MB_RETRYCANCEL | MB_DEFBUTTON2 | MB_ICONERROR);
                           if (errorchoice==4)
                              {
                               erretry = true;
                              }
                        }
                        else
                        {
                            exit(0); //close program once game has launched
                        }
                   }while(erretry);
              }
              }
             break;
        case 2: //open directory button
             char ownPtz[MAX_PATH]; 
             if (GetModuleHandle(NULL) != NULL)
              {
               // When passing NULL to GetModuleHandle, it returns handle of exe itself
                GetModuleFileName(GetModuleHandle(NULL),ownPtz, (sizeof(ownPtz))); 
                path_strip_filename(ownPtz);
                // Use above module handle to get the path using GetModuleFileName()
                ShellExecute(NULL, "open", ownPtz, NULL, NULL, SW_SHOWDEFAULT);
              }
             break;
        case 3: //website button
             switch (rboption)
             {
                    case 6:
                         MessageBox(NULL, "Select a server to view it's website.","WoW Launcher", MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION);
                         break;
                    case 7:
                         ShellExecute(NULL, "open", "http://127.0.0.1", NULL, NULL, SW_SHOWDEFAULT);
                         break;
                    case 8:
                         ShellExecute(NULL, "open", "http://zhyper-pwnage.com", NULL, NULL, SW_SHOWDEFAULT);
                         break;
                    case 9:
                         ShellExecute(NULL, "open", "http://www.gamer-district.org", NULL, NULL, SW_SHOWDEFAULT);
                         break;
                    case 10:
                         ShellExecute(NULL, "open", "http://www.landofelves.net", NULL, NULL, SW_SHOWDEFAULT);
                         break;
                    case 11:
                         ShellExecute(NULL, "open", "http://www.wowcircle.com", NULL, NULL, SW_SHOWDEFAULT);
                         break;
                    case 12:
                         ShellExecute(NULL, "open", "http://www.warmane.com", NULL, NULL, SW_SHOWDEFAULT);
                         break;
                    case 13:
                         MessageBox(NULL, "Select a server to view it's website.","WoW Launcher", MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION);
                         break;
             }
             break;
        case 4: //about button
             MessageBox(NULL, "Made by brotalnia\nTested on WotLK 3.3.5\nLast Change: 19 July 2015\nwww.youtube.com/brotalnia","About WoW Launcher", MB_OK | MB_DEFBUTTON1);
             break;
        case 5: //exit button
             exit(0);
             break;       
        case 6: //no change radio button
             switch (HIWORD(wParam))
             {
                 case BN_CLICKED:
                 if(IsDlgButtonChecked(hwnd, 6) == BST_CHECKED)
                 {
                 EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                 rboption = 6; //storing selected server
                 }
                 break; 
             }
        case 7: //localhost radio button
             switch (HIWORD(wParam))
             {
                 case BN_CLICKED:
                 if(IsDlgButtonChecked(hwnd, 7) == BST_CHECKED)
                 {
                 EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                 rboption = 7; //storing selected server
                 }
                 break; 
             }
        case 8: //zhyper radio button
             switch (HIWORD(wParam))
             {
                 case BN_CLICKED:
                 if(IsDlgButtonChecked(hwnd, 8) == BST_CHECKED)
                 {
                 EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                 rboption = 8; //storing selected server
                 }
                 break; 
             }
        case 9: //gamer-district radio button
             switch (HIWORD(wParam))
             {
                 case BN_CLICKED:
                 if(IsDlgButtonChecked(hwnd, 9) == BST_CHECKED)
                 {
                 EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                 rboption = 9; //storing selected server
                 }
                 break; 
             }
        case 10: //land of elves radio button
             switch (HIWORD(wParam))
             {
                 case BN_CLICKED:
                 if(IsDlgButtonChecked(hwnd, 10) == BST_CHECKED)
                 {
                 EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                 rboption = 10; //storing selected server
                 }
                 break; 
             }
        case 11: //wow circle radio button
             switch (HIWORD(wParam))
             {
                 case BN_CLICKED:
                 if(IsDlgButtonChecked(hwnd, 11) == BST_CHECKED)
                 {
                 EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                 rboption = 11; //storing selected server
                 }
                 break; 
             }
        case 12: //warmane radio button
             switch (HIWORD(wParam))
             {
                 case BN_CLICKED:
                 if(IsDlgButtonChecked(hwnd, 12) == BST_CHECKED)
                 {
                 EnableWindow(txtRealmlist, FALSE); //disable textbox next to Set To radio button if it is not selected
                 rboption = 12; //storing selected server
                 }
                 break; 
             }
        case 13: //set to radio button
             switch (HIWORD(wParam))
             {
                 case BN_CLICKED:
                 if(IsDlgButtonChecked(hwnd, 13) == BST_CHECKED)
                 {
                 EnableWindow(txtRealmlist, TRUE); //disable textbox next to Set To radio button if it is not selected
                 rboption = 13; //storing selected server
                 }
                 break; 
             }
        }
        break;
   case WM_CTLCOLORSTATIC:
        {
        HDC hdcStatic = (HDC) wParam; //setting the label colors
        SetTextColor(hdcStatic, RGB(0,0,0));
        SetBkColor(hdcStatic, RGB(236, 233, 216));

        if (hbrBkgnd == NULL)
        {
            hbrBkgnd = CreateSolidBrush(RGB(236, 233, 216));
        }
        return (INT_PTR)hbrBkgnd;
        }
        case WM_PAINT:
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
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
