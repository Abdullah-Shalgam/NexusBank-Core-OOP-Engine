#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include "clsScreen.h"
#include "clsBankClient.h"
#include "clsCurrency.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsClientScreenBase : protected clsScreen
{
protected:
    clsClientScreenBase(short ScreenWidth = 122) : clsScreen(ScreenWidth) {}

    static string _FormatBalance(double BalanceInUSD)
    {
        return clsCurrency::FormatSystemAmount(BalanceInUSD);
    }

    static void PrintTransactionReceipt(const clsBankClient &Client, string TransactionType, double Amount)
    {
        string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) +
                           "\n  [?] Would you like to print an official transaction receipt? (y/n): " +
                           UtilLib::GetColor(UtilLib::enColor::Reset);

        char Answer = InputValidateLib::getYesNoAnswer(PromptMsg);
        if (tolower(Answer) != 'y')
            return;

        cout << "\n";
        UtilLib::ShowSpinner("Generating & Formatting Official Receipt...", 2);

        cout << "\n\n";

        const short InnerWidth = 52;
        string BorderLine = string(InnerWidth + 4, '=');
        string Indent = "  ";

        string RawTitle = "BANK TRANSACTION RECEIPT";
        short TitlePadding = (InnerWidth - (short)RawTitle.length()) / 2;

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << BorderLine << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << string(TitlePadding, ' ')
             << UtilLib::GetColor(UtilLib::enColor::Bold)
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << RawTitle
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << string(InnerWidth - TitlePadding - (short)RawTitle.length(), ' ')
             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |" << endl;

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << BorderLine << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        auto PrintReceiptRow = [&](string Key, string Value, UtilLib::enColor ValColor = UtilLib::enColor::Reset)
        {
            const short KeyWidth = 18;
            const short ValWidth = 32;

            string FormattedKey = UtilLib::Truncate(Key, KeyWidth);
            string FormattedVal = UtilLib::Truncate(Value, ValWidth);

            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
                 << UtilLib::GetColor(UtilLib::enColor::Yellow);
            UtilLib::TypeWriterText(FormattedKey + string(KeyWidth - (short)FormattedKey.length(), ' '), 1);

            cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << ": "
                 << UtilLib::GetColor(ValColor);
            UtilLib::TypeWriterText(FormattedVal + string(ValWidth - (short)FormattedVal.length(), ' '), 1);

            cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |" << endl;
        };

        PrintReceiptRow("Date / Time", UtilLib::GetSystemDateTime(), UtilLib::enColor::BrightCyan);
        PrintReceiptRow("Account Number", Client.GetAccountNumber(), UtilLib::enColor::BrightCyan);
        PrintReceiptRow("Client Name", Client.GetFullName());
        PrintReceiptRow("Transaction Type", TransactionType, UtilLib::enColor::Magenta);
        PrintReceiptRow("Amount", _FormatBalance(Amount), UtilLib::enColor::BrightGreen);
        PrintReceiptRow("Updated Balance", _FormatBalance(Client.GetAccountBalance()), UtilLib::enColor::BrightYellow);

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << BorderLine << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        string RawThanks = "Thank you for using our Banking System!";
        short ThanksPadding = (InnerWidth - (short)RawThanks.length()) / 2;

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << string(ThanksPadding, ' ')
             << UtilLib::GetColor(UtilLib::enColor::BrightGreen)
             << RawThanks
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << string(InnerWidth - ThanksPadding - (short)RawThanks.length(), ' ')
             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |" << endl;

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << BorderLine << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n\n";
    }

    clsBankClient _GetExistingClient(string PromptMessage = "  [>] Enter Account Number: ")
    {
        cout << UtilLib::GetColor(UtilLib::enColor::BrightYellow) << "  [?] ACCOUNT SEARCH:" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
        string AccountNumber = InputValidateLib::ReadLimitedText(10, PromptMessage);

        while (!clsBankClient::IsClientExist(AccountNumber))
        {
            cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                 << "  [!] Account Number [" << AccountNumber << "] was not found! Please try again.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
            AccountNumber = InputValidateLib::ReadLimitedText(10, PromptMessage);
        }

        return clsBankClient::Find(AccountNumber);
    }

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
        PrintCardRow("Email", Client.GetEmail());
        PrintCardRow("Phone", Client.GetPhone());
        PrintCardRow("PIN Code", Client.GetPinCode(), UtilLib::enColor::Yellow);
        PrintCardRow("Account Balance", _FormatBalance(Client.GetAccountBalance()), UtilLib::enColor::BrightGreen);

        cout << Indent << TopBottomLine << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n\n";
    }
};