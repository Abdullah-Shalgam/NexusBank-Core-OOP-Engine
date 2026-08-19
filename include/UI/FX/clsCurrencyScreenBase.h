#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include "clsScreen.h"
#include "clsCurrency.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsCurrencyScreenBase : protected clsScreen
{
protected:
    clsCurrencyScreenBase(short ScreenWidth = 122) : clsScreen(ScreenWidth) {}

    static string _FormatRate(double Rate)
    {
        ostringstream ss;
        ss << fixed << setprecision(2) << Rate;
        return "$ " + ss.str();
    }

    void _PrintCurrencyCard(const clsCurrency &Currency, string CardTitle = "CURRENCY DETAILS")
    {
        const int BoxWidth = 59;
        const int LeftMargin = (_ScreenWidth - BoxWidth) / 2;
        const string Indent(LeftMargin, ' ');
        const string TopBottomLine = string(BoxWidth, '=');

        const int InnerWidth = BoxWidth - 4;
        int TitleLen = (int)CardTitle.length();

        int LeftSpaces = (InnerWidth - TitleLen) / 2;
        if (LeftSpaces < 0)
            LeftSpaces = 0;

        int RightSpaces = InnerWidth - TitleLen - LeftSpaces;
        if (RightSpaces < 0)
            RightSpaces = 0;

        string FormattedTitle = string(LeftSpaces, ' ') + CardTitle + string(RightSpaces, ' ');

        cout << "\n";
        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << TopBottomLine << endl;

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow);
        UtilLib::TypeWriterText(FormattedTitle, 8);
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |" << endl;

        cout << Indent << TopBottomLine << endl;

        auto PrintCardRow = [&](string Key, string Value, UtilLib::enColor ValColor = UtilLib::enColor::Reset)
        {
            ostringstream ssKey, ssVal;
            ssKey << left << setw(18) << Key;
            ssVal << left << setw(35) << UtilLib::Truncate(Value, 35);

            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
                 << UtilLib::GetColor(UtilLib::enColor::Yellow);
            UtilLib::TypeWriterText(ssKey.str(), 1);

            cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << ": "
                 << UtilLib::GetColor(ValColor);
            UtilLib::TypeWriterText(ssVal.str(), 1);

            cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |" << endl;
        };

        PrintCardRow("Country", Currency.GetCountry(), UtilLib::enColor::BrightCyan);
        PrintCardRow("Currency Code", Currency.GetCurrencyCode(), UtilLib::enColor::BrightYellow);
        PrintCardRow("Currency Name", Currency.GetCurrencyName());
        PrintCardRow("Rate(1$) =", _FormatRate(Currency.GetRate()), UtilLib::enColor::BrightGreen);

        cout << Indent << TopBottomLine << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n\n";
    }

    clsCurrency _GetExistingCurrency(bool IsOnlyCurrencyCode = false)
    {
        short Choice = 1;

        if (!IsOnlyCurrencyCode)
        {
            string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) +
                               "  [?] Find By: [1] Code or [2] Country: " +
                               UtilLib::GetColor(UtilLib::enColor::Reset);

            Choice = InputValidateLib::ReadShortNumberInRange(1, 2, PromptMsg, "  [!] Invalid Choice, Enter 1 or 2: ");
        }

        clsCurrency Currency = clsCurrency::GetEmptyCurrencyObject();

        if (Choice == 1)
        {
            cout << "\n";
            string CurrencyCode = InputValidateLib::ReadLimitedText(3, "  [>] Please Enter Currency Code: ");
            Currency = clsCurrency::FindByCode(CurrencyCode);

            while (Currency.IsEmpty())
            {
                cout << "\n"
                     << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                     << "  [!] Currency Code [" << CurrencyCode << "] was not found! Please try again.\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);

                CurrencyCode = InputValidateLib::ReadLimitedText(3, "  [>] Please Enter Currency Code: ");
                Currency = clsCurrency::FindByCode(CurrencyCode);
            }
        }

        else
        {
            cout << "\n";
            string Country = InputValidateLib::ReadText("  [>] Please Enter Country Name: ");
            Currency = clsCurrency::FindByCountry(Country);

            while (Currency.IsEmpty())
            {
                cout << "\n"
                     << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                     << "  [!] Currency for Country [" << Country << "] was not found! Please try again.\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);

                Country = InputValidateLib::ReadText("  [>] Please Enter Country Name: ");
                Currency = clsCurrency::FindByCountry(Country);
            }
        }

        return Currency;
    }
};