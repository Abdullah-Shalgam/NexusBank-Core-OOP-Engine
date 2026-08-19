#pragma once
#include <iostream>
#include "clsClientScreenBase.h"
#include "clsBankClient.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsAddNewClientScreen : protected clsClientScreenBase
{
private:
    clsAddNewClientScreen() : clsClientScreenBase(122) {}

    void _ReadClientInfo(clsBankClient &Client)
    {
        cout << "\n"
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow) << "  [+] ENTER CLIENT INFORMATION:" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        Client.SetFirstName(InputValidateLib::ReadLimitedText(25, "  [>] Enter First Name          : "));
        Client.SetLastName(InputValidateLib::ReadLimitedText(25, "  [>] Enter Last Name           : "));
        Client.SetEmail(InputValidateLib::ReadLimitedText(28, "  [>] Enter Email Address       : "));
        Client.SetPhone(InputValidateLib::ReadLimitedText(10, "  [>] Enter Phone Number        : "));
        Client.SetPinCode(InputValidateLib::GetValidPIN("  [>] Enter PIN Code (4 digits) : "));
        Client.SetAccountBalance(InputValidateLib::ReadDblPositiveNumber("  [>] Enter Initial Balance     : "));

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
    }

    bool _Show()
    {
        if (!_CheckAccessRights(clsUser::enMainMenuPermissions::pAddNewClient))
            return false;

        _ResetTheScreen();
        _DrawScreenHeader("ADD NEW CLIENT DASHBOARD", "Register New Bank Account Record");

        cout << UtilLib::GetColor(UtilLib::enColor::BrightYellow) << "  [?] ACCOUNT VERIFICATION:" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
        string AccountNumber = InputValidateLib::ReadLimitedText(10, "  [>] Enter Account Number: ");

        while (clsBankClient::IsClientExist(AccountNumber))
        {
            cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                 << "  [!] Account Number [" << AccountNumber << "] is already taken! Please try another.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
            AccountNumber = InputValidateLib::ReadLimitedText(10, "  [>] Enter New Account Number: ");
        }

        clsBankClient NewClient = clsBankClient::GetAddNewClientObject(AccountNumber);
        _ReadClientInfo(NewClient);

        if (!_ConfirmUserPassword("CREATE NEW CLIENT ACCOUNT REGISTRATION"))
        {
            return true;
        }

        cout << "\n";
        _ShowProgressBar("Saving new client to database...");

        clsBankClient::enSaveResults SaveResult = NewClient.Save();

        switch (SaveResult)
        {
        case clsBankClient::enSaveResults::svSucceeded:
            cout << "\n\n";
            _PrintAnimatedSuccess("  [!] Client Account Created Successfully!", 20);
            _PrintClientCard(NewClient, "NEW CLIENT CARD DETAILS");
            break;

        case clsBankClient::enSaveResults::svFailedEmptyObject:
            _ShowWarningMessage("ERROR: Save failed because the client object is empty!");
            break;

        case clsBankClient::enSaveResults::svFailedAccountNumberExists:
            _ShowWarningMessage("ERROR: Save failed because the Account Number already exists!");
            break;
        }

        return true;
    }

public:
    static bool ShowAddNewClient()
    {
        clsAddNewClientScreen AddNewClientScreen;
        return AddNewClientScreen._Show();
    }
};