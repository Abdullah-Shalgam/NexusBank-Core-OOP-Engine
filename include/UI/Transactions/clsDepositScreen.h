#pragma once
#include <iostream>
#include "clsClientScreenBase.h"
#include "clsBankClient.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsDepositScreen : protected clsClientScreenBase
{
private:
    clsDepositScreen() : clsClientScreenBase(122) {}

    void _Show()
    {
        _ResetTheScreen();
        _DrawScreenHeader("DEPOSIT SCREEN", "Deposit Money Into Client Account");

        clsBankClient Client = _GetExistingClient("  [>] Enter Account Number: ");
        _PrintClientCard(Client, "CLIENT ACCOUNT DETAILS");

        cout << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << "  [+] TRANSACTION DETAILS:"
             << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        double Amount = InputValidateLib::ReadDblPositiveNumber("  [>] Please enter deposit amount: ");

        string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) + "\n  [?] Are you sure you want to deposit (" + _FormatBalance(Amount) + ") into this account? (y/n): " + UtilLib::GetColor(UtilLib::enColor::Reset);

        char Answer = InputValidateLib::getYesNoAnswer(PromptMsg);

        if (tolower(Answer) == 'y')
        {
            cout << "\n";
            _ShowLoadingSpinner("Processing deposit transaction & updating database...");

            if (Client.Deposit(Amount))
            {
                cout << "\n\n";
                _PrintAnimatedSuccess("  [!] Deposit Transaction Completed Successfully!", 20);

                cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                cout << UtilLib::GetColor(UtilLib::enColor::BrightGreen)
                     << "  [V] New Account Balance Is : " << _FormatBalance(Client.GetAccountBalance()) << "\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);

                PrintTransactionReceipt(Client, "Deposit", Amount);
            }
            else
            {
                _ShowWarningMessage("ERROR: Deposit operation failed! Unable to save transaction to database.");
            }
        }
        else
        {
            cout << UtilLib::GetColor(UtilLib::enColor::Yellow)
                 << "\n  [i] Deposit transaction cancelled. Account balance remains unchanged.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
        }
    }

public:
    static void ShowDepositScreen()
    {
        clsDepositScreen DepositScreen;
        DepositScreen._Show();
    }
};