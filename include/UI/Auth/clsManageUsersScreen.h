#pragma once

#include <iostream>
#include <iomanip>
#include <conio.h>

#include "clsScreen.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

// Screens
#include "clsUsersListScreen.h"
#include "clsAddNewUserScreen.h"
#include "clsDeleteUserScreen.h"
#include "clsUpdateUserScreen.h"
#include "clsFindUserScreen.h"

using namespace std;

class clsManageUsersScreen : protected clsScreen
{
private:
    enum class enManageUsersMenuOptions
    {
        eListUsers = 1,
        eAddNewUser = 2,
        eDeleteUser = 3,
        eUpdateUser = 4,
        eFindUser = 5,
        eShowMainMenu = 6
    };

    clsManageUsersScreen() : clsScreen(122) {}

    // ----------------------------------------------------------
    // Navigation & Screen Dispatchers
    // ----------------------------------------------------------

    void _GoBackToManageUsersMenu()
    {
        cout << "\n\n"
             << UtilLib::ColorText("  [>] Press any key to return to Manage Users Menu...", UtilLib::enColor::Yellow);
        _getch();
        _Show();
    }

    void _ShowListUsersScreen()
    {
        _ShowProgressBar("Fetching user records from database...");
        clsUsersListScreen::ShowUsersList();
    }

    void _ShowAddNewUserScreen()
    {
        _ShowProgressBar("Initializing user registration system...");
        clsAddNewUserScreen::ShowAddNewUser();
    }

    void _ShowDeleteUserScreen()
    {
        _ShowProgressBar("Initializing user removal system...");
        clsDeleteUserScreen::ShowDeleteUser();
    }

    void _ShowUpdateUserScreen()
    {
        _ShowProgressBar("Initializing user update module...");
        clsUpdateUserScreen::ShowUpdateUser();
    }

    void _ShowFindUserScreen()
    {
        _ShowProgressBar("Initializing user search module...");
        clsFindUserScreen::ShowFindUser();
    }

    // ----------------------------------------------------------
    // User Interface Components
    // ----------------------------------------------------------

    void _DrawMenuBox()
    {
        const int BoxWidth = 60;
        const int InnerWidth = BoxWidth - 2;
        const int LeftMargin = (_ScreenWidth - BoxWidth) / 2;
        const string Indent(LeftMargin > 0 ? LeftMargin : 0, ' ');

        auto PrintFrameLine = [&](char FillChar = '=')
        {
            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan)
                 << string(BoxWidth, FillChar)
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;
        };

        auto PrintCenteredTitle = [&](string Text, UtilLib::enColor TextColor = UtilLib::enColor::BrightYellow)
        {
            int TextLen = (int)Text.length();
            int Padding = InnerWidth - TextLen;
            int LeftPad = Padding / 2;
            int RightPad = Padding - LeftPad;

            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|"
                 << string(LeftPad, ' ')
                 << UtilLib::GetColor(TextColor) << Text
                 << UtilLib::GetColor(UtilLib::enColor::Cyan) << string(RightPad, ' ') << "|"
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;
        };

        auto PrintSectionHeader = [&](string Text, UtilLib::enColor TextColor = UtilLib::enColor::BrightGreen)
        {
            int LeftPad = 2;
            int RightPad = InnerWidth - LeftPad - (int)Text.length();
            if (RightPad < 0)
                RightPad = 0;

            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|"
                 << string(LeftPad, ' ')
                 << UtilLib::GetColor(TextColor) << Text
                 << UtilLib::GetColor(UtilLib::enColor::Cyan) << string(RightPad, ' ') << "|"
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;
        };

        auto PrintOption = [&](string Num, string Label, UtilLib::enColor NumColor = UtilLib::enColor::Yellow)
        {
            int IndentInside = 3;
            string NumTag = "[" + Num + "]";
            int UsedWidth = IndentInside + (int)NumTag.length() + 1 + (int)Label.length();
            int RightPad = InnerWidth - UsedWidth;
            if (RightPad < 0)
                RightPad = 0;

            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|"
                 << string(IndentInside, ' ')
                 << UtilLib::GetColor(NumColor) << NumTag
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << " "
                 << Label
                 << UtilLib::GetColor(UtilLib::enColor::Cyan) << string(RightPad, ' ') << "|"
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;
        };

        auto PrintSeparator = [&](char FillChar = '-')
        {
            int Pad = 1;
            int LineLen = InnerWidth - (Pad * 2);
            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|"
                 << string(Pad, ' ') << string(LineLen, FillChar) << string(Pad, ' ') << "|"
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;
        };

        PrintFrameLine('=');
        PrintCenteredTitle("MANAGE USERS DASHBOARD");
        PrintFrameLine('=');

        PrintSectionHeader("-- USER MANAGEMENT SERVICES --", UtilLib::enColor::BrightGreen);
        PrintOption("1", "List Users");
        PrintOption("2", "Add New User");
        PrintOption("3", "Delete User");
        PrintOption("4", "Update User");
        PrintOption("5", "Find User");

        PrintSeparator('-');

        PrintOption("6", "Main Menu", UtilLib::enColor::BrightRed);

        PrintFrameLine('=');
        cout << "\n";
    }

    // ----------------------------------------------------------
    // Main Command Processing Loop
    // ----------------------------------------------------------

    void _PerformManageUsersMenuOption(enManageUsersMenuOptions Option)
    {
        switch (Option)
        {
        case enManageUsersMenuOptions::eListUsers:
            _ResetTheScreen();
            _ShowListUsersScreen();
            _GoBackToManageUsersMenu();
            break;

        case enManageUsersMenuOptions::eAddNewUser:
            _ResetTheScreen();
            _ShowAddNewUserScreen();
            _GoBackToManageUsersMenu();
            break;

        case enManageUsersMenuOptions::eDeleteUser:
            _ResetTheScreen();
            _ShowDeleteUserScreen();
            _GoBackToManageUsersMenu();
            break;

        case enManageUsersMenuOptions::eUpdateUser:
            _ResetTheScreen();
            _ShowUpdateUserScreen();
            _GoBackToManageUsersMenu();
            break;

        case enManageUsersMenuOptions::eFindUser:
            _ResetTheScreen();
            _ShowFindUserScreen();
            _GoBackToManageUsersMenu();
            break;

        case enManageUsersMenuOptions::eShowMainMenu:
            break;
        }
    }

    void _Show()
    {
        if (!_CheckAccessRights(clsUser::enMainMenuPermissions::pManageUsers))
            return;

        _ResetTheScreen();
        _DrawScreenHeader("MANAGE USERS DASHBOARD", "Administer System Access & User Permissions");
        _DrawMenuBox();

        string Prompt = "  [?] Choose Option [1 to 7]: ";
        short Choice = InputValidateLib::ReadShortNumberInRange(1, 7, Prompt, "  [!] Invalid Option! Enter Number between 1 and 7: ");
        _PerformManageUsersMenuOption((enManageUsersMenuOptions)Choice);
    }

public:
    static void ShowManageUsersMenu()
    {
        clsManageUsersScreen ManageUsersScreen;
        ManageUsersScreen._Show();
    }
};