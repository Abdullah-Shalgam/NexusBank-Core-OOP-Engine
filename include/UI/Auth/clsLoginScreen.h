#pragma once

#include "Global.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

#include "clsScreen.h"
#include "clsUser.h"
#include "clsMainScreen.h"
#include "UtilLib.h"
#include "InputValidateLib.h"

using namespace std;

class clsLoginScreen : protected clsScreen
{
private:
    clsLoginScreen() : clsScreen(122) {}

    // ----------------------------------------------------------
    // UI Components & Visual Frames
    // ----------------------------------------------------------

    void _DrawLoginBox()
    {
        const int BoxWidth = 64;
        const int InnerWidth = BoxWidth - 2;
        const int LeftMargin = (_ScreenWidth - BoxWidth) / 2;
        const string Indent(LeftMargin > 0 ? LeftMargin : 0, ' ');

        auto PrintFrameLine = [&](char FillChar = '=')
        {
            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan)
                 << string(BoxWidth, FillChar)
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;
        };

        auto PrintCenteredTitle = [&](string Text, UtilLib::enColor TextColor = UtilLib::enColor::BrightYellow)
        {
            int TextLen = (int)Text.length();
            int Padding = InnerWidth - TextLen;
            int LeftPad = Padding / 2;
            int RightPad = Padding - LeftPad;

            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|"
                 << string(LeftPad, ' ')
                 << UtilLib::GetColor(TextColor) << Text
                 << UtilLib::GetColor(UtilLib::enColor::Cyan) << string(RightPad, ' ') << "|"
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << endl;
        };

        PrintFrameLine('=');
        PrintCenteredTitle("SYSTEM SECURITY AUTHENTICATION", UtilLib::enColor::BrightYellow);
        PrintCenteredTitle("Please Enter Authorized Credentials To Access", UtilLib::enColor::Yellow);
        PrintFrameLine('=');
        cout << "\n";
    }

    void _ShowSystemLockedScreen()
    {
        _ResetTheScreen();

        cout << "\n\n";
        _PrintFullWidthLine('=', UtilLib::enColor::BrightRed);
        _PrintCenteredLine("SYSTEM LOCKED / ACCESS DENIED", UtilLib::enColor::BrightRed);
        _PrintFullWidthLine('=', UtilLib::enColor::BrightRed);
        cout << "\n";

        _PrintCenteredLine("[!] You have entered invalid credentials 3 times consecutively.", UtilLib::enColor::Yellow);
        _PrintCenteredLine("[!] Security Policy Triggered: Session terminated immediately.", UtilLib::enColor::Yellow);
        cout << "\n\n";

        for (int i = 5; i > 0; i--)
        {
            string TimerMsg = "  [!] Emergency System Shutdown In: " + to_string(i) + " Second(s)...";
            cout << "\r" << string(25, ' ')
                 << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                 << UtilLib::GetColor(UtilLib::enColor::Bold)
                 << TimerMsg
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << flush;

            this_thread::sleep_for(chrono::seconds(1));
        }

        cout << "\n\n";
        _PrintFullWidthLine('=', UtilLib::enColor::BrightRed);
        cout << "\n";
    }

    // ----------------------------------------------------------
    // Main Login Logic
    // ----------------------------------------------------------

    bool _Login()
    {
        bool LoginFailed = false;
        short FailedAttempts = 0;

        while (true)
        {
            _ResetTheScreen();
            _DrawScreenHeader("AUTHENTICATION GATEWAY", "Secure Core Banking Suite Access");

            if (LoginFailed)
            {
                short RemainingAttempts = Global::MaxLoginAttempts - FailedAttempts;

                cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                     << "  [!] INVALID USERNAME OR PASSWORD!\n"
                     << "  [!] WARNING: You have (" << RemainingAttempts << ") attempt(s) left before system lockout!\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------\n\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
            }

            _DrawLoginBox();

            string UserName = InputValidateLib::ReadLimitedText(15, "  [>] Enter Username : ");
            string Password = InputValidateLib::ReadPassword("  [>] Enter Password : ");

            cout << "\n";
            _ShowLoadingSpinner("Authenticating credentials with database...", 2);

            Global::CurrentUser = clsUser::Find(UserName, Password);
            LoginFailed = Global::CurrentUser.IsEmpty();

            if (!LoginFailed)
            {
                cout << "\n";
                _ShowProgressBar("Initializing user session & loading security profile...");

                cout << "\n\n";
                string WelcomeMsg = "  [V] Access Granted! Welcome back, " + Global::CurrentUser.GetFullName() + " (" + Global::CurrentUser.GetUserName() + ")";
                _PrintAnimatedSuccess(WelcomeMsg, 15);

                _ShowLoadingSpinner("Redirecting to System Main Dashboard...", 2);

                FailedAttempts = 0;
                LoginFailed = false;

                Global::CurrentUserLoginTime = UtilLib::GetSystemDateTime();
                Global::CurrentUser.RegisterLogin();

                clsMainScreen::ShowMainMenu();
            }
            else
            {
                FailedAttempts++;

                if (FailedAttempts >= Global::MaxLoginAttempts)
                {
                    _ShowSystemLockedScreen();
                    return false;
                }
            }
        }
    }

public:
    static void ShowLoginScreen()
    {
        clsLoginScreen LoginScreen;
        LoginScreen._Login();
    }
};