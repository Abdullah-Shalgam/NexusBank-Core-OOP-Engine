#pragma once

#include <iostream>
#include <iomanip>
#include <conio.h>

#include "clsScreen.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

// Screens
#include "clsDepositScreen.h"
#include "clsWithdrawScreen.h"
#include "clsTotalBalancesScreen.h"
#include "clsTransferScreen.h"
#include "clsTransferLogScreen.h"

using namespace std;

class clsTransactionsScreen : protected clsScreen
{
private:
    enum class enTransactionsMenuOptions
    {
        eDeposit = 1,
        eWithdraw = 2,
        eShowTotalBalance = 3,
        eTransfer = 4,
        eTransferLog = 5,
        eShowMainMenu = 6
    };

    clsTransactionsScreen() : clsScreen(122) {}

    // ----------------------------------------------------------
    // Navigation & Screen Dispatchers
    // ----------------------------------------------------------

    void _GoBackToTransactionsMenu()
    {
        cout << "\n\n"
             << UtilLib::ColorText("  [>] Press any key to return to Transactions Menu...", UtilLib::enColor::Yellow);
        _getch();
        _Show();
    }

    void _ShowDepositScreen()
    {
        _ShowProgressBar("Initializing Deposit Module & Loading Components...");
        clsDepositScreen::ShowDepositScreen();
    }

    void _ShowWithdrawScreen()
    {
        _ShowProgressBar("Initializing Withdraw Module & Loading Components...");
        clsWithdrawScreen::ShowWithdrawScreen();
    }

    void _ShowTotalBalancesScreen()
    {
        _ShowProgressBar("Calculating System Balances & Loading Database...");
        clsTotalBalancesScreen::ShowTotalBalancesScreen();
    }

    void _ShowTransferScreen()
    {
        _ShowProgressBar("Initializing Transfer Module & Loading Components...");
        clsTransferScreen::ShowTransferScreen();
    }

    void _ShowTransferLogScreen()
    {
        _ShowProgressBar("Fetching Transfer Transaction History & Audit Logs...");
        clsTransferLogScreen::ShowTransferLogScreen();
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
        PrintCenteredTitle("TRANSACTIONS MAIN DASHBOARD");
        PrintFrameLine('=');

        PrintSectionHeader("-- FINANCIAL TRANSACTION SERVICES --", UtilLib::enColor::BrightGreen);
        PrintOption("1", "Deposit");
        PrintOption("2", "Withdraw");
        PrintOption("3", "Total Balances");
        PrintOption("4", "Transfer");
        PrintOption("5", "Transfer Log");

        PrintSeparator('-');

        PrintOption("6", "Main Menu", UtilLib::enColor::BrightRed);

        PrintFrameLine('=');
        cout << "\n";
    }

    // ----------------------------------------------------------
    // Main Command Processing Loop
    // ----------------------------------------------------------

    void _PerformTransactionsMenuOption(enTransactionsMenuOptions TransactionsMenuOption)
    {
        switch (TransactionsMenuOption)
        {
        case enTransactionsMenuOptions::eDeposit:
            _ResetTheScreen();
            _ShowDepositScreen();
            _GoBackToTransactionsMenu();
            break;

        case enTransactionsMenuOptions::eWithdraw:
            _ResetTheScreen();
            _ShowWithdrawScreen();
            _GoBackToTransactionsMenu();
            break;

        case enTransactionsMenuOptions::eShowTotalBalance:
            _ResetTheScreen();
            _ShowTotalBalancesScreen();
            _GoBackToTransactionsMenu();
            break;

        case enTransactionsMenuOptions::eTransfer:
            _ResetTheScreen();
            _ShowTransferScreen();
            _GoBackToTransactionsMenu();
            break;

        case enTransactionsMenuOptions::eTransferLog:
            _ResetTheScreen();
            _ShowTransferLogScreen();
            _GoBackToTransactionsMenu();
            break;

        case enTransactionsMenuOptions::eShowMainMenu:
            break;
        }
    }

    void _Show()
    {
        if (!_CheckAccessRights(clsUser::enMainMenuPermissions::pTransactions))
            return;

        _ResetTheScreen();
        _DrawScreenHeader("TRANSACTIONS DASHBOARD", "Manage Financial Operations & Balances");
        _DrawMenuBox();

        string Prompt = "  [?] Choose Option [1 to 6]: ";
        short Choice = InputValidateLib::ReadShortNumberInRange(1, 6, Prompt, "  [!] Invalid Option! Enter Number between 1 and 6: ");
        _PerformTransactionsMenuOption((enTransactionsMenuOptions)Choice);
    }

public:
    static void ShowTransactionsMenu()
    {
        clsTransactionsScreen TransactionsScreen;
        TransactionsScreen._Show();
    }
};