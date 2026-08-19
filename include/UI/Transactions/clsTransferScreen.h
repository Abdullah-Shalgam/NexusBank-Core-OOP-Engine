#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include "clsClientScreenBase.h"
#include "clsBankClient.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsTransferScreen : protected clsClientScreenBase
{
private:
    clsTransferScreen() : clsClientScreenBase(122) {}

    void _PrintClientCard(const clsBankClient &Client, string CardTitle = "CLIENT CARD DETAILS")
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

        PrintCardRow("Account Number", Client.GetAccountNumber(), UtilLib::enColor::BrightCyan);
        PrintCardRow("Full Name", Client.GetFullName());
        PrintCardRow("Account Balance", _FormatBalance(Client.GetAccountBalance()), UtilLib::enColor::BrightGreen);

        cout << Indent << TopBottomLine << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n\n";
    }

    void _PrintTransferSummaryBox(const clsBankClient &Sender, const clsBankClient &Recipient, double Amount)
    {
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan)
             << "  ========================================================\n"
             << "  |              TRANSFER TRANSACTION SUMMARY            |\n"
             << "  ========================================================\n"
             << UtilLib::GetColor(UtilLib::enColor::Reset);

        cout << UtilLib::GetColor(UtilLib::enColor::Yellow) << "   Sender (From)   : "
             << UtilLib::GetColor(UtilLib::enColor::Reset) << Sender.GetFullName()
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan) << " [" << Sender.GetAccountNumber() << "]\n";

        cout << UtilLib::GetColor(UtilLib::enColor::Yellow) << "   Recipient (To)  : "
             << UtilLib::GetColor(UtilLib::enColor::Reset) << Recipient.GetFullName()
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan) << " [" << Recipient.GetAccountNumber() << "]\n";

        cout << UtilLib::GetColor(UtilLib::enColor::Yellow) << "   Transfer Amount : "
             << UtilLib::GetColor(UtilLib::enColor::BrightGreen) << _FormatBalance(Amount) << "\n";

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan)
             << "  --------------------------------------------------------\n"
             << UtilLib::GetColor(UtilLib::enColor::Reset);
    }

    void _Show()
    {
        _ResetTheScreen();
        _DrawScreenHeader("MONEY TRANSFER MODULE", "Transfer funds securely between bank accounts");

        clsBankClient Sender = _GetExistingClient("  [>] Enter Account Number to Transfer From (Sender): ");
        _PrintClientCard(Sender, "SENDER ACCOUNT DETAILS (FROM)");

        clsBankClient Recipient = _GetExistingClient("  [>] Enter Account Number to Transfer To (Recipient): ");
        while (Sender.GetAccountNumber() == Recipient.GetAccountNumber())
        {
            cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                 << "  [!] Invalid Action: Sender and Recipient cannot be the same account!\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
            Recipient = _GetExistingClient("  [>] Enter a DIFFERENT Account Number to Transfer To: ");
        }
        _PrintClientCard(Recipient, "RECIPIENT ACCOUNT DETAILS (TO)");

        cout << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << "  [+] ENTER TRANSACTION AMOUNT:"
             << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        double Amount = InputValidateLib::ReadDblPositiveNumber("  [>] Enter Transfer Amount: ");

        while (Amount > Sender.GetAccountBalance() || Amount <= 0)
        {
            if (Amount <= 0)
            {
                cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                     << "  [!] Transfer amount must be greater than zero!\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
            }
            else
            {
                cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                     << "\n  [!] Insufficient Funds! Amount exceeds available balance.\n"
                     << "  [*] Maximum transferable amount: " << _FormatBalance(Sender.GetAccountBalance()) << "\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
            }
            Amount = InputValidateLib::ReadDblPositiveNumber("  [>] Enter a valid Transfer Amount: ");
        }

        cout << "\n";
        _PrintTransferSummaryBox(Sender, Recipient, Amount);

        string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) +
                           "  [?] Are you sure you want to execute this transfer? (y/n): " +
                           UtilLib::GetColor(UtilLib::enColor::Reset);

        char Answer = InputValidateLib::getYesNoAnswer(PromptMsg);

        if (tolower(Answer) == 'y')
        {
            if (!_ConfirmUserPassword("AUTHORIZE FINANCIAL TRANSFER"))
            {
                return;
            }

            cout << "\n";
            _ShowProgressBar("Processing financial transaction & updating client ledgers...");

            if (Sender.Transfer(Amount, Recipient, Global::CurrentUser.GetUserName()))
            {
                cout << "\n\n";
                _PrintAnimatedSuccess("  [V] Transfer Transaction Completed Successfully!", 20);

                cout << UtilLib::GetColor(UtilLib::enColor::Cyan)
                     << "\n  ========================================================\n"
                     << "  |               UPDATED ACCOUNT BALANCES               |\n"
                     << "  ========================================================\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);

                cout << UtilLib::GetColor(UtilLib::enColor::BrightGreen)
                     << "   [+] Sender    [" << Sender.GetAccountNumber() << "] New Balance : " << _FormatBalance(Sender.GetAccountBalance()) << "\n"
                     << "   [+] Recipient [" << Recipient.GetAccountNumber() << "] New Balance : " << _FormatBalance(Recipient.GetAccountBalance()) << "\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);

                cout << UtilLib::GetColor(UtilLib::enColor::Cyan)
                     << "  ========================================================\n\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
            }
            else
            {
                _ShowWarningMessage("ERROR: Transfer operation failed! Unable to save transaction to database.");
            }
        }
        else
        {
            cout << UtilLib::GetColor(UtilLib::enColor::Yellow)
                 << "\n  [i] Transfer transaction cancelled by user. Account balances remain unchanged.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
        }
    }

public:
    static void ShowTransferScreen()
    {
        clsTransferScreen TransferScreen;
        TransferScreen._Show();
    }
};