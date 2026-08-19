#pragma once

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include "Global.h"
#include "clsScreen.h"
#include "clsUser.h"
#include "UtilLib.h"
#include "MyStringLib.h"

using namespace std;

class clsLogoutScreen : protected clsScreen
{
private:
    clsLogoutScreen() : clsScreen(122) {}

    static void _ResetUserSession()
    {
        // 1. Clear Active Session Object in RAM
        Global::CurrentUser = clsUser::GetEmptyUserObject();

        // 2. Clear Session Time
        Global::CurrentUserLoginTime = "";
    }

    void _Show()
    {
        _ResetTheScreen();

        // Store user info in local variables before resetting the active session
        string UserFullName = Global::CurrentUser.GetFullName();
        string UserName = Global::CurrentUser.GetUserName();

        // 1. Header
        _DrawScreenHeader("LOGOUT SESSION", "Closing Active User Session Safely");

        // 2. Farewell Card
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan);
        UtilLib::PrintHeaderLine('=', _ScreenWidth);
        cout << UtilLib::GetColor(UtilLib::enColor::Reset);

        _PrintCenteredLine("GOODBYE, " + MyStringLib::CapitalizeAllString(UserFullName) + "!", UtilLib::enColor::BrightYellow);
        _PrintCenteredLine("User Account [" + UserName + "] is logging out...", UtilLib::enColor::Cyan);

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan);
        UtilLib::PrintHeaderLine('-', _ScreenWidth);
        cout << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n\n";

        // 3. System termination animation & state cleanup
        _ShowProgressBar("Saving system state & terminating session...");

        _ResetUserSession();

        cout << "\n";
        _PrintAnimatedSuccess("  [+] Session closed successfully. Returning to Login Screen...", 15);

        // Seamless UI transition delay
        this_thread::sleep_for(chrono::milliseconds(1200));
    }

public:
    static void ShowLogoutScreen()
    {
        clsLogoutScreen Screen;
        Screen._Show();
    }
};