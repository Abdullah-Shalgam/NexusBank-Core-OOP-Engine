#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include "clsScreen.h"
#include "clsUser.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsUserScreenBase : protected clsScreen
{
protected:
    clsUserScreenBase(short ScreenWidth = 122) : clsScreen(ScreenWidth) {}

    clsUser _GetExistingUser(string PromptMessage = "  [>] Enter Username: ")
    {
        cout << UtilLib::GetColor(UtilLib::enColor::BrightYellow) << "  [?] USER SEARCH:" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
        string UserName = InputValidateLib::ReadLimitedText(12, PromptMessage);

        while (!clsUser::IsUserExist(UserName))
        {
            cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                 << "  [!] Username [" << UserName << "] was not found! Please try again.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
            UserName = InputValidateLib::ReadLimitedText(12, PromptMessage);
        }

        return clsUser::Find(UserName);
    }

    bool _IsFullAccess(int Permissions)
    {
        if (Permissions == clsUser::enMainMenuPermissions::eAll)
            return true;

        int AllPermissionsMask = clsUser::enMainMenuPermissions::pListClients |
                                 clsUser::enMainMenuPermissions::pAddNewClient |
                                 clsUser::enMainMenuPermissions::pDeleteClient |
                                 clsUser::enMainMenuPermissions::pUpdateClients |
                                 clsUser::enMainMenuPermissions::pFindClient |
                                 clsUser::enMainMenuPermissions::pTransactions |
                                 clsUser::enMainMenuPermissions::pManageUsers |
                                 clsUser::enMainMenuPermissions::pLoginRegister |
                                 clsUser::enMainMenuPermissions::pCurrencyExchange;

        return (Permissions == AllPermissionsMask);
    }

    string _GetPermissionsText(int Permissions)
    {
        if (_IsFullAccess(Permissions))
            return "Full Access";

        string PermText = "";

        if (Permissions & clsUser::enMainMenuPermissions::pListClients)
            PermText += "List, ";
        if (Permissions & clsUser::enMainMenuPermissions::pAddNewClient)
            PermText += "Add, ";
        if (Permissions & clsUser::enMainMenuPermissions::pDeleteClient)
            PermText += "Delete, ";
        if (Permissions & clsUser::enMainMenuPermissions::pUpdateClients)
            PermText += "Update, ";
        if (Permissions & clsUser::enMainMenuPermissions::pFindClient)
            PermText += "Find, ";
        if (Permissions & clsUser::enMainMenuPermissions::pTransactions)
            PermText += "Transactions, ";
        if (Permissions & clsUser::enMainMenuPermissions::pManageUsers)
            PermText += "Manage Users, ";
        if (Permissions & clsUser::enMainMenuPermissions::pLoginRegister)
            PermText += "Login Register, ";
        if (Permissions & clsUser::enMainMenuPermissions::pCurrencyExchange)
            PermText += "Currency, ";

        if (!PermText.empty())
            PermText = PermText.substr(0, PermText.length() - 2);
        else
            PermText = "No Access";

        return PermText;
    }

    int _ReadPermissionsToSet()
    {
        int Permissions = 0;

        string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) +
                           "  [?] Do you want to give full access? (y/n): " +
                           UtilLib::GetColor(UtilLib::enColor::Reset);

        char Answer = InputValidateLib::getYesNoAnswer(PromptMsg);

        if (tolower(Answer) == 'y')
        {
            return clsUser::enMainMenuPermissions::eAll;
        }

        cout << "\n"
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << "  [?] SELECT SPECIFIC PERMISSIONS:" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        if (tolower(InputValidateLib::getYesNoAnswer("  [>] Show Client List? (y/n): ")) == 'y')
            Permissions |= clsUser::enMainMenuPermissions::pListClients;

        if (tolower(InputValidateLib::getYesNoAnswer("  [>] Add New Client? (y/n): ")) == 'y')
            Permissions |= clsUser::enMainMenuPermissions::pAddNewClient;

        if (tolower(InputValidateLib::getYesNoAnswer("  [>] Delete Client? (y/n): ")) == 'y')
            Permissions |= clsUser::enMainMenuPermissions::pDeleteClient;

        if (tolower(InputValidateLib::getYesNoAnswer("  [>] Update Client? (y/n): ")) == 'y')
            Permissions |= clsUser::enMainMenuPermissions::pUpdateClients;

        if (tolower(InputValidateLib::getYesNoAnswer("  [>] Find Client? (y/n): ")) == 'y')
            Permissions |= clsUser::enMainMenuPermissions::pFindClient;

        if (tolower(InputValidateLib::getYesNoAnswer("  [>] Transactions Menu? (y/n): ")) == 'y')
            Permissions |= clsUser::enMainMenuPermissions::pTransactions;

        if (tolower(InputValidateLib::getYesNoAnswer("  [>] Manage Users Menu? (y/n): ")) == 'y')
            Permissions |= clsUser::enMainMenuPermissions::pManageUsers;

        if (tolower(InputValidateLib::getYesNoAnswer("  [>] Currency Exchange? (y/n): ")) == 'y')
            Permissions |= clsUser::enMainMenuPermissions::pCurrencyExchange;

        if (tolower(InputValidateLib::getYesNoAnswer("  [>] Login Register Audit Log? (y/n): ")) == 'y')
            Permissions |= clsUser::enMainMenuPermissions::pLoginRegister;

        if (_IsFullAccess(Permissions))
            return clsUser::enMainMenuPermissions::eAll;

        return Permissions;
    }

    void _PrintUserCard(const clsUser &User, string CardTitle = "USER CARD DETAILS")
    {
        const int BoxWidth = 59;
        const int LeftMargin = (_ScreenWidth - BoxWidth) / 2;
        const string Indent(LeftMargin, ' ');
        const string TopBottomLine = string(BoxWidth, '=');
        const string MiddleSeparator = "|" + string(BoxWidth - 2, '-') + "|";

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

        // Header Title
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

        PrintCardRow("Username", User.GetUserName(), UtilLib::enColor::BrightCyan);
        PrintCardRow("Full Name", User.GetFullName());
        PrintCardRow("Email", User.GetEmail());
        PrintCardRow("Phone", User.GetPhone());
        PrintCardRow("Password", User.GetPassword(), UtilLib::enColor::Yellow);

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << MiddleSeparator << endl;

        string PermHeaderTitle = "GRANTED SYSTEM PERMISSIONS";
        int PermHeaderLeft = (InnerWidth - (int)PermHeaderTitle.length()) / 2;
        int PermHeaderRight = InnerWidth - (int)PermHeaderTitle.length() - PermHeaderLeft;
        string FormattedPermHeader = string(PermHeaderLeft, ' ') + PermHeaderTitle + string(PermHeaderRight, ' ');

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow) << FormattedPermHeader
             << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |" << endl;

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << MiddleSeparator << endl;

        struct stPermissionItem
        {
            int Flag;
            string Name;
        };

        vector<stPermissionItem> vPermissions = {
            {clsUser::enMainMenuPermissions::pListClients, "Show Client List"},
            {clsUser::enMainMenuPermissions::pAddNewClient, "Add New Client"},
            {clsUser::enMainMenuPermissions::pDeleteClient, "Delete Client"},
            {clsUser::enMainMenuPermissions::pUpdateClients, "Update Client"},
            {clsUser::enMainMenuPermissions::pFindClient, "Find Client"},
            {clsUser::enMainMenuPermissions::pTransactions, "Transactions Menu"},
            {clsUser::enMainMenuPermissions::pManageUsers, "Manage Users Menu"},
            {clsUser::enMainMenuPermissions::pLoginRegister, "Login Register"}};

        int UserPerm = User.GetPermissions();
        bool HasFullAccess = _IsFullAccess(User.GetPermissions());

        auto PrintPermRow = [&](string Tag, string Text, UtilLib::enColor TagColor, UtilLib::enColor TextColor)
        {
            cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << "| "
                 << "  " << UtilLib::GetColor(TagColor) << Tag << " "
                 << UtilLib::GetColor(TextColor) << left << setw(49) << Text
                 << UtilLib::GetColor(UtilLib::enColor::Cyan) << " |" << endl;
        };

        if (HasFullAccess)
        {
            PrintPermRow("[*]", "FULL ACCESS (All System Permissions)", UtilLib::enColor::BrightYellow, UtilLib::enColor::BrightGreen);
        }

        for (const auto &Perm : vPermissions)
        {
            bool IsGranted = HasFullAccess || (UserPerm & Perm.Flag);
            if (IsGranted)
            {
                PrintPermRow("[+]", Perm.Name, UtilLib::enColor::BrightGreen, UtilLib::enColor::Green);
            }
            else
            {
                PrintPermRow("[-]", Perm.Name, UtilLib::enColor::BrightRed, UtilLib::enColor::DarkGray);
            }
        }

        cout << Indent << UtilLib::GetColor(UtilLib::enColor::Cyan) << TopBottomLine << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n\n";
    }
};