#pragma once
#include <iostream>
#include "clsClientScreenBase.h"
#include "clsBankClient.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsWithdrawScreen : protected clsClientScreenBase
{
private:
    clsWithdrawScreen() : clsClientScreenBase(122) {}

    void _Show()
    {
        _ResetTheScreen();
        _DrawScreenHeader("WITHDRAW SCREEN", "Withdraw Money From Client Account");

        clsBankClient Client = _GetExistingClient("  [>] Enter Account Number: ");
        _PrintClientCard(Client, "CLIENT ACCOUNT DETAILS");

        cout << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << "  [+] TRANSACTION DETAILS:"
             << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        double Amount = InputValidateLib::ReadDblPositiveNumber("  [>] Please enter withdraw amount: ");

        while (Amount > Client.GetAccountBalance())
        {
            cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                 << "\n  [!] Amount exceeds available balance! You can withdraw up to : "
                 << _FormatBalance(Client.GetAccountBalance()) << "\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);

            Amount = InputValidateLib::ReadDblPositiveNumber("  [>] Please enter another amount: ");
        }

        string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) + "\n  [?] Are you sure you want to withdraw (" + _FormatBalance(Amount) + ") from this account? (y/n): " + UtilLib::GetColor(UtilLib::enColor::Reset);

        char Answer = InputValidateLib::getYesNoAnswer(PromptMsg);

        if (tolower(Answer) == 'y')
        {
            cout << "\n";
            _ShowLoadingSpinner("Processing withdraw transaction & updating database...");

            if (Client.Withdraw(Amount))
            {
                cout << "\n\n";
                _PrintAnimatedSuccess("  [!] Withdraw Transaction Completed Successfully!", 20);

                cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                cout << UtilLib::GetColor(UtilLib::enColor::BrightGreen)
                     << "  [V] New Account Balance Is : " << _FormatBalance(Client.GetAccountBalance()) << "\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);

                PrintTransactionReceipt(Client, "Withdrawal", Amount);
            }
            else
            {
                _ShowWarningMessage("ERROR: Withdraw operation failed! Unable to save transaction to database.");
            }
        }
        else
        {
            cout << UtilLib::GetColor(UtilLib::enColor::Yellow)
                 << "\n  [i] Withdraw transaction cancelled. Account balance remains unchanged.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
        }
    }

public:
    static void ShowWithdrawScreen()
    {
        clsWithdrawScreen WithdrawScreen;
        WithdrawScreen._Show();
    }
};