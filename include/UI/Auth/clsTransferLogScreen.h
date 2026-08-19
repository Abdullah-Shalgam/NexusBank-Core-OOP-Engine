#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include "clsClientScreenBase.h"
#include "clsBankClient.h"
#include "UtilLib.h"

using namespace std;

class clsTransferLogScreen : protected clsClientScreenBase
{
private:
    clsTransferLogScreen() : clsClientScreenBase(122) {}

    void _PrintTableHeader()
    {
        _PrintFullWidthLine('=', UtilLib::enColor::Cyan);

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(21) << "Date / Time"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(8) << "s.Acc"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(8) << "d.Acc"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(14) << "Amount"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(14) << "s.Bal"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(14) << "d.Bal"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Bold) << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(21) << "User Name"

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |"
             << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;

        _PrintFullWidthLine('=', UtilLib::enColor::Cyan);
    }

    void _PrintTransferLogRecordLine(const clsBankClient::stTransferLogRecord &Record)
    {
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << left << setw(21) << UtilLib::Truncate(Record.DateTime, 21)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan)
             << left << setw(8) << UtilLib::Truncate(Record.SenderAccountNumber, 8)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan)
             << left << setw(8) << UtilLib::Truncate(Record.DestinationAccountNumber, 8)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightGreen)
             << left << setw(14) << UtilLib::Truncate(_FormatBalance(Record.Amount), 14)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Yellow)
             << left << setw(14) << UtilLib::Truncate(_FormatBalance(Record.SenderBalanceAfter), 14)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Yellow)
             << left << setw(14) << UtilLib::Truncate(_FormatBalance(Record.DestinationBalanceAfter), 14)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << left << setw(21) << UtilLib::Truncate(Record.UserName, 21)

             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |"
             << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;
    }

    void _Show()
    {
        _ResetTheScreen();
        vector<clsBankClient::stTransferLogRecord> vRecords = clsBankClient::GetTransferLogList();

        string Title = "TRANSFER LOG AUDIT SCREEN";
        string SubTitle = "Transfer Log List (" + to_string(vRecords.size()) + ") Record(s)";

        _DrawScreenHeader(Title, SubTitle);

        _PrintTableHeader();

        if (vRecords.empty())
        {
            _PrintCenteredLineWithBorders("NO TRANSFER LOG RECORDS FOUND IN THE SYSTEM!", UtilLib::enColor::BrightRed);
        }
        else
        {
            for (const auto &Record : vRecords)
            {
                _PrintTransferLogRecordLine(Record);
            }
        }

        _PrintFullWidthLine('=', UtilLib::enColor::Cyan);
        cout << endl;
    }

public:
    static void ShowTransferLogScreen()
    {
        clsTransferLogScreen TransferLogScreen;
        TransferLogScreen._Show();
    }
};