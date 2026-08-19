#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include "clsUserScreenBase.h"
#include "clsUser.h"
#include "UtilLib.h"

using namespace std;

class clsLoginRegisterScreen : protected clsUserScreenBase
{
private:
    clsLoginRegisterScreen() : clsUserScreenBase(122) {}

    void _PrintLoginRegisterRecordLine(const clsUser::stLoginRegisterRecord &Record)
    {
        string PermText = _GetPermissionsText(Record.Permissions);

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::Reset)
             << left << setw(21) << UtilLib::Truncate(Record.DateTime, 21)
             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan)
             << left << setw(27) << UtilLib::Truncate(Record.UserName, 27)
             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Yellow)
             << left << setw(18) << UtilLib::Truncate(Record.Password, 18)
             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::Reset);

        if (_IsFullAccess(Record.Permissions))
        {
            cout << UtilLib::GetColor(UtilLib::enColor::BrightGreen)
                 << left << setw(43) << UtilLib::Truncate(PermText, 43);
        }
        else
        {
            cout << UtilLib::GetColor(UtilLib::enColor::White)
                 << left << setw(43) << UtilLib::Truncate(PermText, 43);
        }

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |" << endl;
    }

    bool _Show()
    {
        if (!_CheckAccessRights(clsUser::enMainMenuPermissions::pLoginRegister))
            return false;

        _ResetTheScreen();
        vector<clsUser::stLoginRegisterRecord> vRecords = clsUser::GetLoginRegisterList();

        string Title = "LOGIN REGISTER AUDIT LOG";
        string SubTitle = "List Of All System Login Attempts (" + to_string(vRecords.size()) + ") Record(s)";

        _DrawScreenHeader(Title, SubTitle);

        _PrintFullWidthLine('=', UtilLib::enColor::Cyan);

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(21) << "Date / Time"
             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(27) << "Username"
             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(18) << "Password"
             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " | "
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(43) << "Granted Permissions"
             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |" << endl;

        _PrintFullWidthLine('=', UtilLib::enColor::Cyan);

        if (vRecords.empty())
        {
            _PrintCenteredLineWithBorders("NO LOGIN AUDIT RECORDS FOUND IN THE SYSTEM!", UtilLib::enColor::BrightRed);
        }
        else
        {
            for (const auto &Record : vRecords)
            {
                _PrintLoginRegisterRecordLine(Record);
            }
        }

        _PrintFullWidthLine('=', UtilLib::enColor::Cyan);
        cout << endl;

        return true;
    }

public:
    static bool ShowLoginRegister()
    {
        clsLoginRegisterScreen LoginRegisterScreen;
        return LoginRegisterScreen._Show();
    }
};