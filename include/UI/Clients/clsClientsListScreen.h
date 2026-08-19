#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include "clsClientScreenBase.h"
#include "clsBankClient.h"
#include "UtilLib.h"

using namespace std;

class clsClientsListScreen : protected clsClientScreenBase
{
private:
    clsClientsListScreen() : clsClientScreenBase(122) {}

    void _PrintTableHeader()
    {
        _PrintFullWidthLine('=');

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(14) << "Acc Number"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(25) << "Client Name"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(10) << "Phone"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(27) << "Email"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(10) << "Pin Code"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(17) << "Balance"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |"
             << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;

        _PrintFullWidthLine('=');
        cout << endl;
    }

    void _PrintClientRecordLine(const clsBankClient &Client)
    {
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan)
             << left << setw(14) << UtilLib::Truncate(Client.GetAccountNumber(), 14)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << left << setw(25) << UtilLib::Truncate(Client.GetFullName(), 25)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << left << setw(10) << UtilLib::Truncate(Client.GetPhone(), 12)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << left << setw(27) << UtilLib::Truncate(Client.GetEmail(), 26)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(10) << UtilLib::Truncate(Client.GetPinCode(), 10)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightGreen)
             << left << setw(17) << UtilLib::Truncate(_FormatBalance(Client.GetAccountBalance()), 15)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |"
             << UtilLib::GetColor(UtilLib::enColor::Reset);
    }

    void _PrintFooterSummary(size_t TotalClients, double TotalBalance)
    {
        _PrintFullWidthLine('=');
        cout << endl;

        string SummaryText = "Total Clients: " + to_string(TotalClients) + "  |  Total System Balances: " + _FormatBalance(TotalBalance);
        _PrintCenteredLineWithBorders(SummaryText, UtilLib::enColor::BrightYellow);

        cout << "\n";
        _PrintFullWidthLine('=');
        cout << endl;
    }

    bool _Show()
    {
        if (!_CheckAccessRights(clsUser::enMainMenuPermissions::pListClients))
            return false;

        vector<clsBankClient> vClients = clsBankClient::GetClientsList();

        string Title = "CLIENTS LIST DASHBOARD";
        string SubTitle = "Active Client Records & Financial Overview";

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
            double TotalBalance = 0;
            for (const clsBankClient &Client : vClients)
            {
                TotalBalance += Client.GetAccountBalance();
                _PrintClientRecordLine(Client);
                cout << endl;
            }
            _PrintFooterSummary(vClients.size(), TotalBalance);
        }

        return true;
    }

public:
    static bool ShowClientsList()
    {
        clsClientsListScreen ClientListScreen;
        return ClientListScreen._Show();
    }
};