#pragma once

#include <iostream>
#include <vector>
#include <iomanip>
#include "clsCurrencyScreenBase.h"
#include "clsCurrency.h"
#include "UtilLib.h"

using namespace std;

class clsCurrenciesListScreen : protected clsCurrencyScreenBase
{
private:
    clsCurrenciesListScreen() : clsCurrencyScreenBase(122) {}

    void _PrintTableHeader()
    {
        _PrintFullWidthLine('=');

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(33) << "Country"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(8) << "Code"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(45) << "Currency Name"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(23) << "Rate(1$)"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |"
             << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;

        _PrintFullWidthLine('=');
        cout << endl;
    }

    void _PrintCurrencyRecordLine(const clsCurrency &Currency)
    {
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << left << setw(33) << UtilLib::Truncate(Currency.GetCountry(), 33)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan)
             << left << setw(8) << UtilLib::Truncate(Currency.GetCurrencyCode(), 8)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << left << setw(45) << UtilLib::Truncate(Currency.GetCurrencyName(), 45)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightGreen)
             << left << setw(23) << UtilLib::Truncate(_FormatRate(Currency.GetRate()), 23)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |"
             << UtilLib::GetColor(UtilLib::enColor::Reset);
    }

    void _PrintFooterSummary(size_t TotalCurrencies)
    {
        _PrintFullWidthLine('=');
        cout << endl;

        string SummaryText = "Total Currencies Available: " + to_string(TotalCurrencies);
        _PrintCenteredLineWithBorders(SummaryText, UtilLib::enColor::BrightYellow);

        cout << "\n";
        _PrintFullWidthLine('=');
        cout << endl;
    }

    void _Show()
    {
        vector<clsCurrency> vCurrencies = clsCurrency::GetCurrenciesList();

        string Title = "CURRENCIES LIST DASHBOARD";
        string SubTitle = "Active Currencies & Exchange Rates Overview";

        _ResetTheScreen();
        _DrawScreenHeader(Title, SubTitle);

        _PrintTableHeader();

        if (vCurrencies.empty())
        {
            _PrintCenteredLineWithBorders("No Currencies Available In the System!", UtilLib::enColor::BrightRed);
            cout << "\n";
            _PrintFullWidthLine('=');
            cout << endl;
        }

        else
        {
            for (const clsCurrency &Currency : vCurrencies)
            {
                _PrintCurrencyRecordLine(Currency);
                cout << endl;
            }
            _PrintFooterSummary(vCurrencies.size());
        }
    }

public:
    static void ShowCurrenciesList()
    {
        clsCurrenciesListScreen CurrenciesListScreen;
        CurrenciesListScreen._Show();
    }
};