#pragma once
#include <iostream>
#include "clsUserScreenBase.h"
#include "clsUser.h"
#include "InputValidateLib.h"
#include "UtilLib.h"
#include "MyStringLib.h"

using namespace std;

class clsDeleteUserScreen : protected clsUserScreenBase
{
private:
    clsDeleteUserScreen() : clsUserScreenBase(122) {}

    void _Show()
    {
        _ResetTheScreen();
        _DrawScreenHeader("DELETE USER DASHBOARD", "Remove Existing User Record Permanently");

        clsUser User = _GetExistingUser("  [>] Enter Username: ");

        if (MyStringLib::LowerAllString(User.GetUserName()) == "admin")
        {
            _ShowWarningMessage("ERROR: You cannot delete the [Admin] user account!");
            return;
        }

        if (User.GetUserName() == Global::CurrentUser.GetUserName())
        {
            _ShowWarningMessage("ERROR: You cannot delete the currently logged-in user account!");
            return;
        }

        _PrintUserCard(User, "USER CARD DETAILS TO DELETE");

        string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightRed) +
                           UtilLib::GetColor(UtilLib::enColor::Bold) +
                           "  [!] WARNING: Are you sure you want to delete this user permanently? (Y/N): " +
                           UtilLib::GetColor(UtilLib::enColor::Reset);

        char Answer = InputValidateLib::getYesNoAnswer(PromptMsg);

        if (tolower(Answer) == 'y')
        {
            if (!_ConfirmUserPassword("PERMANENTLY DELETE SYSTEM USER"))
            {
                return;
            }

            cout << "\n";
            _ShowProgressBar("Deleting user record permanently from database...");

            if (User.Delete())
            {
                cout << "\n\n";
                _PrintAnimatedSuccess("  [!] User Record Deleted Permanently!", 20);

                _PrintUserCard(User, "USER CARD DETAILS AFTER DELETION");
            }
            else
            {
                _ShowWarningMessage("ERROR: Failed to delete user record!");
            }
        }
        else
        {
            cout << UtilLib::GetColor(UtilLib::enColor::Yellow)
                 << "\n  [i] Deletion operation cancelled. User record remains safe.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
        }
    }

public:
    static void ShowDeleteUser()
    {
        clsDeleteUserScreen DeleteUserScreen;
        DeleteUserScreen._Show();
    }
};