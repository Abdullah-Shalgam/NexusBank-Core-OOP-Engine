#pragma once
#include <iostream>
#include "clsCurrencyScreenBase.h"
#include "clsCurrency.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsFindCurrencyScreen : protected clsCurrencyScreenBase
{
private:
    clsFindCurrencyScreen() : clsCurrencyScreenBase(122) {}

    void _Show()
    {
        _ResetTheScreen();
        _DrawScreenHeader("FIND CURRENCY DASHBOARD", "Search & Display Existing Currency Record");

        clsCurrency Currency = _GetExistingCurrency();

        if (!Currency.IsEmpty())
        {
            cout << "\n";
            _ShowProgressBar("Fetching Currency record from database...");

            cout << "\n\n";
            _PrintAnimatedSuccess("  [!] Currency Record Found & Retrieved Successfully!", 20);

            _PrintCurrencyCard(Currency, "CURRENCY CARD DETAILS");
        }
    }

public:
    static void ShowFindCurrency()
    {
        clsFindCurrencyScreen FindCurrencyScreen;
        FindCurrencyScreen._Show();
    }
};