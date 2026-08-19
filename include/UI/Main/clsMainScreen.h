#pragma once

#include <iostream>
#include <iomanip>
#include <conio.h>

#include "clsScreen.h"
#include "InputValidateLib.h"
#include "UtilLib.h"
#include "Global.h"

// Screens
#include "clsClientsListScreen.h"
#include "clsAddNewClientScreen.h"
#include "clsDeleteClientScreen.h"
#include "clsUpdateClientScreen.h"
#include "clsFindClientScreen.h"
#include "clsTransactionsScreen.h"
#include "clsManageUsersScreen.h"
#include "clsCurrencyExchangeMainScreen.h"
#include "clsLoginRegisterScreen.h"
#include "clsLogoutScreen.h"

using namespace std;

class clsMainScreen : protected clsScreen
{
private:
    enum class enMainMenuOptions
    {
        eListClients = 1,
        eAddNewClient = 2,
        eDeleteClient = 3,
        eUpdateClient = 4,
        eFindClient = 5,
        eShowTransactionsMenu = 6,
        eManageUsers = 7,
        eCurrncyExchange = 8,
        eLoginRegister = 9,
        eLogout = 10
    };

    clsMainScreen() : clsScreen(122) {}

    // ----------------------------------------------------------
    // Navigation & Screen Dispatchers
    // ----------------------------------------------------------

    void _GoBackToMainMenu()
    {
        cout << "\n\n"
             << UtilLib::ColorText("  [>] Press any key to return to Main Menu...", UtilLib::enColor::Yellow);
        _getch();
        _Show();
    }

    bool _ShowAllClientsScreen()
    {
        _ShowProgressBar("Fetching client records from database...");
        return clsClientsListScreen::ShowClientsList();
    }

    bool _ShowAddNewClientsScreen()
    {
        _ShowProgressBar("Initializing client registration system...");
        return clsAddNewClientScreen::ShowAddNewClient();
    }

    bool _ShowDeleteClientScreen()
    {
        _ShowProgressBar("Initializing client removal system...");
        return clsDeleteClientScreen::ShowDeleteClient();
    }

    bool _ShowUpdateClientScreen()
    {
        _ShowProgressBar("Initializing client update module...");
        return clsUpdateClientScreen::ShowUpdateClient();
    }

    bool _ShowFindClientScreen()
    {
        _ShowProgressBar("Initializing client search module...");
        return clsFindClientScreen::ShowFindClient();
    }

    void _ShowTransactionsMenu()
    {
        clsTransactionsScreen::ShowTransactionsMenu();
        _Show();
    }

    void _ShowManageUsersMenu()
    {
        clsManageUsersScreen::ShowManageUsersMenu();
        _Show();
    }

    void _ShowCurrencyExchangeMainScreen()
    {
        clsCurrencyExchangeMainScreen::ShowCurrencyExchangeMenu();
        _Show();
    }

    bool _ShowLoginRegisterScreen()
    {
        _ShowProgressBar("Opening Login Register Audit Log...");
        return clsLoginRegisterScreen::ShowLoginRegister();
    }

    void _ShowLogoutScreen()
    {
        clsLogoutScreen::ShowLogoutScreen();
    }

    // ----------------------------------------------------------
    // User Interface Components
    // ----------------------------------------------------------

    void _DrawMenuBox()
    {
        const int BoxWidth = 66;
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

        auto PrintOption = [&](string Num, string Label, clsUser::enMainMenuPermissions PermFlag, UtilLib::enColor NumColor = UtilLib::enColor::Yellow, bool ShowBadge = true)
        {
            bool HasAccess = (PermFlag == (clsUser::enMainMenuPermissions)0) ||
                             Global::CurrentUser.CheckAccessPermission(PermFlag);

            int IndentInside = 2;
            int RightMargin = 2;
            string NumTag = "[" + Num + "]";

            string StatusBadge = ShowBadge ? (HasAccess ? "[+ ALLOWED]" : "[- LOCKED]") : "";

            UtilLib::enColor TagColor = HasAccess ? NumColor : UtilLib::enColor::DarkGray;
            UtilLib::enColor TextColor = HasAccess ? UtilLib::enColor::Reset : UtilLib::enColor::DarkGray;
            UtilLib::enColor BadgeColor = HasAccess ? UtilLib::enColor::BrightGreen : UtilLib::enColor::Red;

            int BadgeLen = ShowBadge ? ((int)StatusBadge.length() + RightMargin) : 0;
            int ContentLength = IndentInside + (int)NumTag.length() + 1 + (int)Label.length() + BadgeLen;
            int RightPad = InnerWidth - ContentLength;
            if (RightPad < 0)
                RightPad = 0;

            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|"
                 << string(IndentInside, ' ')
                 << UtilLib::GetColor(TagColor) << NumTag << " "
                 << UtilLib::GetColor(TextColor) << Label
                 << string(RightPad, ' ');

            if (ShowBadge)
            {
                cout << UtilLib::GetColor(BadgeColor) << StatusBadge
                     << string(RightMargin, ' ');
            }

            cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|"
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
        PrintCenteredTitle("SYSTEM MAIN DASHBOARD");
        PrintFrameLine('=');

        // Section 1: Client Management
        PrintSectionHeader("-- CLIENT MANAGEMENT SERVICES --", UtilLib::enColor::BrightGreen);
        PrintOption("1", "Show Client List", clsUser::enMainMenuPermissions::pListClients);
        PrintOption("2", "Add New Client", clsUser::enMainMenuPermissions::pAddNewClient);
        PrintOption("3", "Delete Client", clsUser::enMainMenuPermissions::pDeleteClient);
        PrintOption("4", "Update Client Info", clsUser::enMainMenuPermissions::pUpdateClients);
        PrintOption("5", "Find Client", clsUser::enMainMenuPermissions::pFindClient);

        PrintSeparator('-');

        // Section 2: Operations, Audit & Currency
        PrintSectionHeader("-- BANK OPERATIONS & AUDIT --", UtilLib::enColor::BrightYellow);
        PrintOption("6", "Transactions Menu", clsUser::enMainMenuPermissions::pTransactions);
        PrintOption("7", "Manage Users Menu", clsUser::enMainMenuPermissions::pManageUsers);
        PrintOption("8", "Currency Exchange", clsUser::enMainMenuPermissions::pCurrencyExchange);
        PrintOption("9", "Login Register Log", clsUser::enMainMenuPermissions::pLoginRegister);

        PrintSeparator('-');

        // Section 3: Session
        PrintOption("10", "Logout & Exit Session", (clsUser::enMainMenuPermissions)0, UtilLib::enColor::BrightRed, false);

        PrintFrameLine('=');
        cout << "\n";
    }

    // ----------------------------------------------------------
    // Main Command Processing Loop
    // ----------------------------------------------------------

    void _PerformMainMenuOption(enMainMenuOptions MainMenuOption)
    {
        switch (MainMenuOption)
        {
        case enMainMenuOptions::eListClients:
            _ResetTheScreen();
            if (_ShowAllClientsScreen())
                _GoBackToMainMenu();
            break;

        case enMainMenuOptions::eAddNewClient:
            _ResetTheScreen();
            if (_ShowAddNewClientsScreen())
                _GoBackToMainMenu();
            break;

        case enMainMenuOptions::eDeleteClient:
            _ResetTheScreen();
            if (_ShowDeleteClientScreen())
                _GoBackToMainMenu();
            break;

        case enMainMenuOptions::eUpdateClient:
            _ResetTheScreen();
            if (_ShowUpdateClientScreen())
                _GoBackToMainMenu();
            break;

        case enMainMenuOptions::eFindClient:
            _ResetTheScreen();
            if (_ShowFindClientScreen())
                _GoBackToMainMenu();
            break;

        case enMainMenuOptions::eShowTransactionsMenu:
            _ResetTheScreen();
            _ShowTransactionsMenu();
            break;

        case enMainMenuOptions::eManageUsers:
            _ResetTheScreen();
            _ShowManageUsersMenu();
            break;

        case enMainMenuOptions::eCurrncyExchange:
            _ResetTheScreen();
            _ShowCurrencyExchangeMainScreen();
            break;

        case enMainMenuOptions::eLoginRegister:
            _ResetTheScreen();
            if (_ShowLoginRegisterScreen())
                _GoBackToMainMenu();
            break;

        case enMainMenuOptions::eLogout:
            _ShowLogoutScreen();
            break;
        }
    }

    void _Show()
    {
        _ResetTheScreen();
        _DrawScreenHeader("MAIN DASHBOARD", "Select an option from the menu below");
        _DrawMenuBox();

        string Prompt = "  [?] Choose Option [1 to 10]: ";
        short Choice = InputValidateLib::ReadShortNumberInRange(1, 10, Prompt, "  [!] Invalid Option! Enter Number between 1 and 10: ");
        _PerformMainMenuOption((enMainMenuOptions)Choice);
    }

public:
    static void ShowMainMenu()
    {
        clsMainScreen MainScreen;
        MainScreen._Show();
    }
};