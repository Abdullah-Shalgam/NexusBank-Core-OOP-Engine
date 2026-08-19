#pragma once
#include <iostream>
#include "clsUserScreenBase.h"
#include "clsUser.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsFindUserScreen : protected clsUserScreenBase
{
private:
    clsFindUserScreen() : clsUserScreenBase(122) {}

    void _Show()
    {
        _ResetTheScreen();
        _DrawScreenHeader("FIND USER DASHBOARD", "Search & Display Existing User Record");

        clsUser User = _GetExistingUser("  [>] Enter Username: ");

        cout << "\n";
        _ShowProgressBar("Fetching user record from database...");

        cout << "\n\n";
        _PrintAnimatedSuccess("  [!] User Record Found & Retrieved Successfully!", 20);

        _PrintUserCard(User, "USER CARD DETAILS");
    }

public:
    static void ShowFindUser()
    {
        clsFindUserScreen FindUserScreen;
        FindUserScreen._Show();
    }
};