#pragma once
#include <iostream>
#include "clsCurrencyScreenBase.h"
#include "clsCurrency.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsUpdateCurrencyRateScreen : protected clsCurrencyScreenBase
{
private:
    clsUpdateCurrencyRateScreen() : clsCurrencyScreenBase(122) {}

    double _ReadNewRate()
    {
        string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) +
                           "  [>] Enter New Exchange Rate (1$ = ?): " +
                           UtilLib::GetColor(UtilLib::enColor::Reset);

        return InputValidateLib::ReadDblPositiveNumber(PromptMsg, "  [!] Invalid Rate! Please enter a valid positive number: ");
    }

    void _Show()
    {
        _ResetTheScreen();
        _DrawScreenHeader("UPDATE CURRENCY RATE DASHBOARD", "Modify Exchange Rates for Existing Currencies");

        clsCurrency Currency = _GetExistingCurrency(true);
        _PrintCurrencyCard(Currency, "CURRENT CURRENCY DETAILS");

        if (Currency.GetCurrencyCode() == "USD")
        {
            cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                 << "  [!] You cannot update the rate for US Dollar (USD). Base currency rate is fixed (1$ = 1$).\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
            return;
        }

        string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) +
                           "  [?] Are you sure you want to update the exchange rate for this currency? (y/n): " +
                           UtilLib::GetColor(UtilLib::enColor::Reset);

        char Answer = InputValidateLib::getYesNoAnswer(PromptMsg);

        if (tolower(Answer) == 'y')
        {
            cout << "\n";
            double NewRate = _ReadNewRate();

            if (!_ConfirmUserPassword("MODIFY Currency CREDENTIALS & PERMISSIONS"))
            {
                return;
            }

            cout << "\n";
            _ShowProgressBar("Updating currency exchange rate in database...");

            Currency.UpdateRate(NewRate);

            cout << "\n\n";
            _PrintAnimatedSuccess("  [!] Currency Rate Updated Successfully!", 20);

            _PrintCurrencyCard(Currency, "UPDATED CURRENCY DETAILS");
        }
        else
        {
            cout << UtilLib::GetColor(UtilLib::enColor::Yellow)
                 << "\n  [i] Update operation cancelled. Currency rate remains unchanged.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
        }
    }

public:
    static void ShowUpdateCurrencyRate()
    {
        clsUpdateCurrencyRateScreen UpdateCurrencyScreen;
        UpdateCurrencyScreen._Show();
    }
};