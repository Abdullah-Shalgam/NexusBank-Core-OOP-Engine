#pragma once

#include "Global.h"

#include <iostream>
#include <string>
#include <string_view>
#include <ctime>
#include <iomanip>
#include <conio.h>

#include "clsUser.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

/**
 * @class clsScreen
 * @brief Base abstract UI template providing common header rendering, auth checks, and screen layouts.
 */
class clsScreen
{
protected:
    short _ScreenWidth;

    explicit clsScreen(short ScreenWidth = 122) noexcept
        : _ScreenWidth(ScreenWidth)
    {
    }

    virtual ~clsScreen() = default;

    void _DrawStatusBar() const
    {
        const std::string LoggedInTime = Global::CurrentUserLoginTime;
        const std::string DateStr = LoggedInTime.empty() ? "" : "Login Time: " + LoggedInTime;

        const std::string UserName = Global::CurrentUser.GetUserName();
        const std::string UserStr = UserName.empty() ? "" : "User: " + UserName;

        std::cout << UtilLib::GetColor(UtilLib::enColor::Cyan);
        UtilLib::PrintHeaderLine('=', _ScreenWidth);
        std::cout << UtilLib::GetColor(UtilLib::enColor::Reset);

        const std::string LeftPart = DateStr.empty() ? "" : " " + DateStr;
        const std::string RightPart = UserStr.empty() ? "" : UserStr + " ";

        const int LeftLen = static_cast<int>(LeftPart.length());
        const int RightLen = static_cast<int>(RightPart.length());

        std::cout << UtilLib::GetColor(UtilLib::enColor::Yellow);

        if (LeftLen > 0 || RightLen > 0)
        {
            int Spaces = _ScreenWidth - LeftLen - RightLen;
            if (Spaces < 0)
                Spaces = 1;

            std::cout << LeftPart << std::string(static_cast<size_t>(Spaces), ' ') << RightPart << "\n";
        }
        else
        {
            const std::string SystemInfo = std::string(Global::AppName) + " " + std::string(Global::AppVersion);
            int LeftPadding = (_ScreenWidth - static_cast<int>(SystemInfo.length())) / 2;
            if (LeftPadding < 0)
                LeftPadding = 0;

            std::cout << std::string(static_cast<size_t>(LeftPadding), ' ') << SystemInfo << "\n";
        }

        std::cout << UtilLib::GetColor(UtilLib::enColor::Reset);
        std::cout << UtilLib::GetColor(UtilLib::enColor::Cyan);
        UtilLib::PrintHeaderLine('-', _ScreenWidth);
        std::cout << UtilLib::GetColor(UtilLib::enColor::Reset);
    }

    void _PrintCenteredLine(const std::string &Text, UtilLib::enColor TextColor = UtilLib::enColor::Yellow) const
    {
        const int TextLength = static_cast<int>(Text.length());
        int LeftPadding = (_ScreenWidth - TextLength) / 2;
        if (LeftPadding < 0)
            LeftPadding = 0;

        std::cout << std::string(static_cast<size_t>(LeftPadding), ' ')
                  << UtilLib::GetColor(TextColor) << Text << UtilLib::GetColor(UtilLib::enColor::Reset)
                  << "\n";
    }

    void _PrintFullWidthLine(char LineChar = '=', UtilLib::enColor Color = UtilLib::enColor::Cyan) const
    {
        std::cout << UtilLib::GetColor(Color);
        UtilLib::PrintHeaderLine(LineChar, _ScreenWidth);
        std::cout << UtilLib::GetColor(UtilLib::enColor::Reset);
    }

    void _PrintCenteredLineWithBorders(const std::string &Text, UtilLib::enColor TextColor = UtilLib::enColor::Reset) const
    {
        const int InsideWidth = _ScreenWidth - 2;
        std::string DisplayText = (static_cast<int>(Text.length()) > InsideWidth) 
                                  ? Text.substr(0, static_cast<size_t>(InsideWidth - 3)) + "..." 
                                  : Text;
        const int TextLen = static_cast<int>(DisplayText.length());

        int LeftSpaces = (InsideWidth - TextLen) / 2;
        if (LeftSpaces < 0)
            LeftSpaces = 0;

        int RightSpaces = InsideWidth - TextLen - LeftSpaces;
        if (RightSpaces < 0)
            RightSpaces = 0;

        std::cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|"
                  << std::string(static_cast<size_t>(LeftSpaces), ' ')
                  << UtilLib::ColorText(DisplayText, TextColor)
                  << std::string(static_cast<size_t>(RightSpaces), ' ')
                  << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|\n"
                  << UtilLib::GetColor(UtilLib::enColor::Reset);
    }

    bool _ConfirmUserPassword(const std::string &ActionDescription = "CONFIRM SENSITIVE OPERATION", short MaxAttempts = 3) const
    {
        short FailedAttempts = 0;

        while (FailedAttempts < MaxAttempts)
        {
            std::cout << "\n"
                      << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
                      << "  [!] SECURITY CHECK REQUIRED: [" << ActionDescription << "]\n"
                      << UtilLib::GetColor(UtilLib::enColor::Reset);

            const std::string Prompt = "  [?] Enter your current password to authorize: ";
            const std::string EnteredPassword = InputValidateLib::ReadPassword(Prompt);

            if (Global::CurrentUser.VerifyPassword(EnteredPassword))
            {
                std::cout << UtilLib::ColorText("  [+] Identity verified successfully!\n\n", UtilLib::enColor::BrightGreen);
                return true;
            }

            FailedAttempts++;
            const short RemainingAttempts = static_cast<short>(MaxAttempts - FailedAttempts);
            std::cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                      << "  [!] WARNING: Invalid password! You have (" << RemainingAttempts << ") attempt(s) remaining.\n"
                      << UtilLib::GetColor(UtilLib::enColor::Reset);
        }

        std::cout << "\n";
        _PrintFullWidthLine('=', UtilLib::enColor::BrightRed);
        _PrintCenteredLine("  [!] SECURITY ALERT: Authorization Failed! Operation Aborted.", UtilLib::enColor::BrightRed);
        _PrintFullWidthLine('=', UtilLib::enColor::BrightRed);
        std::cout << "\n";

        return false;
    }

    bool _CheckAccessRights(clsUser::enMainMenuPermissions Permission) const
    {
        if (!Global::CurrentUser.CheckAccessPermission(Permission))
        {
            _ResetTheScreen();
            _DrawScreenHeader("ACCESS DENIED", "You do not have permission to perform this action");

            _PrintFullWidthLine('=', UtilLib::enColor::BrightRed);
            _PrintCenteredLine("  [!] ACCESS DENIED! Please contact your administrator.", UtilLib::enColor::BrightRed);
            _PrintFullWidthLine('=', UtilLib::enColor::BrightRed);

            std::cout << "\n\n"
                      << UtilLib::ColorText("  [>] Press any key to return to Main Menu...", UtilLib::enColor::Yellow);
            _getch();
            return false;
        }
        return true;
    }

    void _DrawScreenHeader(const std::string &Title, const std::string &SubTitle = "") const
    {
        _DrawStatusBar();

        std::cout << "\n";
        _PrintCenteredLine(Title, UtilLib::enColor::BrightYellow);

        if (!SubTitle.empty())
        {
            _PrintCenteredLine(SubTitle, UtilLib::enColor::Yellow);
        }

        std::cout << "\n" << UtilLib::GetColor(UtilLib::enColor::Cyan);
        UtilLib::PrintHeaderLine('=', _ScreenWidth);
        std::cout << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n\n";
    }

    void _ShowWarningMessage(const std::string &Message = "WARNING: Invalid Input!") const
    {
        std::cout << "\n";
        UtilLib::PrintCentered("", _ScreenWidth);
        UtilLib::FlashWarningText(Message);
        std::cout << "\n\n";
    }

    void _ShowLoadingSpinner(const std::string &Message = "Processing, please wait...", int Cycles = 3) const
    {
        UtilLib::ShowSpinner(Message, Cycles);
    }

    void _ShowProgressBar(const std::string &Message = "Loading data, please wait...") const
    {
        UtilLib::ShowProgressBar(Message);
    }

    void _PrintAnimatedSuccess(const std::string &Text, int SpeedInMs = 20) const
    {
        std::cout << UtilLib::GetColor(UtilLib::enColor::BrightGreen);
        UtilLib::TypeWriterText(Text, SpeedInMs);
        std::cout << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
    }

    static void _ResetTheScreen()
    {
        UtilLib::ResetTheScreen();
    }
};