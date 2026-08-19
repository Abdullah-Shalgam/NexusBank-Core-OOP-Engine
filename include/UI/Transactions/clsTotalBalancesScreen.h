#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include "clsClientScreenBase.h"
#include "clsBankClient.h"
#include "UtilLib.h"

using namespace std;

class clsTotalBalancesScreen : protected clsClientScreenBase
{
private:
    clsTotalBalancesScreen() : clsClientScreenBase(122) {}

    void _PrintTableHeader()
    {
        _PrintFullWidthLine('=');

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(25) << "Account Number"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(50) << "Client Name"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(37) << "Balance"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |"
             << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;

        _PrintFullWidthLine('=');
        cout << endl;
    }

    void _PrintClientRecordLine(const clsBankClient &Client)
    {
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan)
             << left << setw(25) << UtilLib::Truncate(Client.GetAccountNumber(), 25)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << left << setw(50) << UtilLib::Truncate(Client.GetFullName(), 50)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightGreen)
             << left << setw(37) << UtilLib::Truncate(_FormatBalance(Client.GetAccountBalance()), 37)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |"
             << UtilLib::GetColor(UtilLib::enColor::Reset);
    }

    void _PrintFooterSummary(size_t TotalClients, double TotalBalances)
    {
        _PrintFullWidthLine('=');
        cout << endl;

        string SummaryText = "Total Clients: " + to_string(TotalClients) + "  |  Total System Balances: " + _FormatBalance(TotalBalances);
        _PrintCenteredLineWithBorders(SummaryText, UtilLib::enColor::BrightYellow);

        clsCurrency SysCurrency = clsCurrency::GetSystemCurrency();

        double ConvertedTotal = clsCurrency::ConvertFromBaseUSD(TotalBalances);

        string TextInWords = "( " + UtilLib::ConvertNumToTextWithDecimals(ConvertedTotal, SysCurrency.GetCurrencyCode(), "Cents") + " )";
        _PrintCenteredLineWithBorders(TextInWords, UtilLib::enColor::BrightGreen);

        cout << "\n";
        _PrintFullWidthLine('=');
        cout << endl;
    }

    void _Show()
    {
        vector<clsBankClient> vClients = clsBankClient::GetClientsList();

        string Title = "TOTAL BALANCES SCREEN";
        string SubTitle = "Financial Overview & System Balances Summary";

        _ResetTheScreen();
        _DrawScreenHeader(Title, SubTitle);

        _PrintTableHeader();

        if (vClients.empty())
        {
            _PrintCenteredLineWithBorders("No Clients Available In the System!", UtilLib::enColor::BrightRed);
            cout << "\n";
            _PrintFullWidthLine('=');
            cout << endl;
        }
        else
        {
            for (const clsBankClient &Client : vClients)
            {
                _PrintClientRecordLine(Client);
                cout << endl;
            }

            double TotalBalances = clsBankClient::GetTotalBalances();
            _PrintFooterSummary(vClients.size(), TotalBalances);
        }
    }

public:
    static void ShowTotalBalancesScreen()
    {
        clsTotalBalancesScreen TotalBalancesScreen;
        TotalBalancesScreen._Show();
    }
};