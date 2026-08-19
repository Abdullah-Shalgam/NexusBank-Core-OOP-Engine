#pragma once

#include <iostream>
#include <iomanip>
#include <conio.h>

#include "clsScreen.h"
#include "InputValidateLib.h"
#include "UtilLib.h"
#include "Global.h"

// Screens
#include "clsCurrenciesListScreen.h"
#include "clsFindCurrencyScreen.h"
#include "clsUpdateCurrencyRateScreen.h"
#include "clsCurrencyCalculatorScreen.h"
#include "clsSetSystemCurrencyScreen.h"

using namespace std;

class clsCurrencyExchangeMainScreen : protected clsScreen
{
private:
    enum class enCurrencyExchangeOptions
    {
        eListCurrencies = 1,
        eFindCurrency = 2,
        eUpdateRate = 3,
        eCurrencyCalculator = 4,
        eSetSystemCurrency = 5,
        eMainMenu = 6
    };

    clsCurrencyExchangeMainScreen() : clsScreen(122) {}

    void _GoBackToCurrencyExchangeMenu()
    {
        cout << "\n\n"
             << UtilLib::ColorText("  [>] Press any key to return to Currency Exchange Menu...", UtilLib::enColor::Yellow);
        _getch();
        _Show();
    }

    void _ShowCurrenciesListScreen()
    {
        _ShowProgressBar("Fetching currencies list from database...");
        clsCurrenciesListScreen::ShowCurrenciesList();
    }

    void _ShowFindCurrencyScreen()
    {
        _ShowProgressBar("Initializing currency search module...");
        clsFindCurrencyScreen::ShowFindCurrency();
    }

    void _ShowUpdateRateScreen()
    {
        _ShowProgressBar("Initializing rate update module...");
        clsUpdateCurrencyRateScreen::ShowUpdateCurrencyRate();
    }

    void _ShowCurrencyCalculatorScreen()
    {
        _ShowProgressBar("Initializing currency calculator...");
        clsCurrencyCalculatorScreen::ShowCurrencyCalculator();
    }

    void _ShowSetSystemCurrencyScreen()
    {
        _ShowProgressBar("Initializing system currency manager...");
        clsSetSystemCurrencyScreen::ShowSetSystemCurrency();
    }

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
            int IndentInside = 2;
            string NumTag = "[" + Num + "]";

            int ContentLength = IndentInside + (int)NumTag.length() + 1 + (int)Label.length();
            int RightPad = InnerWidth - ContentLength;
            if (RightPad < 0)
                RightPad = 0;

            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|"
                 << string(IndentInside, ' ')
                 << UtilLib::GetColor(NumColor) << NumTag << " "
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << Label
                 << string(RightPad, ' ')
                 << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|"
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
        PrintCenteredTitle("CURRENCY EXCHANGE DASHBOARD");
        PrintFrameLine('=');

        PrintSectionHeader("-- FOREIGN EXCHANGE SERVICES --", UtilLib::enColor::BrightGreen);
        PrintOption("1", "List Currencies");
        PrintOption("2", "Find Currency");
        PrintOption("3", "Update Rate");
        PrintOption("4", "Currency Calculator");
        PrintOption("5", "Set System Currency", UtilLib::enColor::BrightCyan);

        PrintSeparator('-');

        PrintOption("6", "Main Menu", UtilLib::enColor::BrightRed);

        PrintFrameLine('=');
        cout << "\n";
    }

    void _PerformCurrencyExchangeOption(enCurrencyExchangeOptions CurrencyExchangeOption)
    {
        switch (CurrencyExchangeOption)
        {
        case enCurrencyExchangeOptions::eListCurrencies:
            _ResetTheScreen();
            _ShowCurrenciesListScreen();
            _GoBackToCurrencyExchangeMenu();
            break;

        case enCurrencyExchangeOptions::eFindCurrency:
            _ResetTheScreen();
            _ShowFindCurrencyScreen();
            _GoBackToCurrencyExchangeMenu();
            break;

        case enCurrencyExchangeOptions::eUpdateRate:
            _ResetTheScreen();
            _ShowUpdateRateScreen();
            _GoBackToCurrencyExchangeMenu();
            break;

        case enCurrencyExchangeOptions::eCurrencyCalculator:
            _ResetTheScreen();
            _ShowCurrencyCalculatorScreen();
            _GoBackToCurrencyExchangeMenu();
            break;

        case enCurrencyExchangeOptions::eSetSystemCurrency:
            _ResetTheScreen();
            _ShowSetSystemCurrencyScreen();
            _GoBackToCurrencyExchangeMenu();
            break;

        case enCurrencyExchangeOptions::eMainMenu:
            break;
        }
    }

    void _Show()
    {
        if (!_CheckAccessRights(clsUser::enMainMenuPermissions::pCurrencyExchange))
            return;

        _ResetTheScreen();
        _DrawScreenHeader("CURRENCY EXCHANGE MENU", "Select a currency service from the menu below");
        _DrawMenuBox();

        string Prompt = "  [?] Choose Option [1 to 6]: ";
        short Choice = InputValidateLib::ReadShortNumberInRange(1, 6, Prompt, "  [!] Invalid Option! Enter Number between 1 and 6: ");
        _PerformCurrencyExchangeOption((enCurrencyExchangeOptions)Choice);
    }

public:
    static void ShowCurrencyExchangeMenu()
    {
        clsCurrencyExchangeMainScreen CurrencyExchangeScreen;
        CurrencyExchangeScreen._Show();
    }
};