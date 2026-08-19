#pragma once
#include <iostream>
#include "clsClientScreenBase.h"
#include "clsBankClient.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsUpdateClientScreen : protected clsClientScreenBase
{
private:
    enum enUpdateFields
    {
        eNone = 0,           // 0
        eFirstName = 1 << 0, // 1
        eLastName = 1 << 1,  // 2
        eEmail = 1 << 2,     // 4
        ePhone = 1 << 3,     // 8
        ePinCode = 1 << 4,   // 16
        eBalance = 1 << 5,   // 32
        eAll = 63            // 63
    };

    clsUpdateClientScreen() : clsClientScreenBase(122) {}

    void _DrawFieldsMenu()
    {
        cout << "\n"
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow) << "  [+] SELECT FIELDS TO UPDATE:" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        auto PrintOption = [](string Num, string FieldName)
        {
            cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  | "
                 << UtilLib::GetColor(UtilLib::enColor::Yellow) << "[" << Num << "] "
                 << UtilLib::GetColor(UtilLib::enColor::Reset) << left << setw(46) << FieldName
                 << UtilLib::GetColor(UtilLib::enColor::Cyan) << "|\n";
        };

        PrintOption("1", "First Name");
        PrintOption("2", "Last Name");
        PrintOption("3", "Email Address");
        PrintOption("4", "Phone Number");
        PrintOption("5", "PIN Code");
        PrintOption("6", "Account Balance");
        PrintOption("7", "Update ALL Fields");
        PrintOption("8", "Done Selecting & Proceed");

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
    }

    unsigned short _ReadUpdateMask()
    {
        _DrawFieldsMenu();

        unsigned short UpdateMask = enUpdateFields::eNone;
        short Choice = 0;

        while (true)
        {
            Choice = InputValidateLib::ReadShortNumberInRange(1, 8,
                                                              "  [?] Choose field to update [1-8]: ",
                                                              "  [!] Invalid choice! Enter between 1 and 8: ");

            if (Choice == 8)
            {
                if (UpdateMask == enUpdateFields::eNone)
                {
                    cout << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                         << "  [!] No fields selected! Please select at least one field or choice [7].\n"
                         << UtilLib::GetColor(UtilLib::enColor::Reset);
                    continue;
                }
                break;
            }

            if (Choice == 7)
            {
                UpdateMask = enUpdateFields::eAll;
                cout << UtilLib::GetColor(UtilLib::enColor::BrightGreen) << "  [V] All fields selected for update!\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                break;
            }

            switch (Choice)
            {
            case 1:
                UpdateMask |= enUpdateFields::eFirstName;
                cout << UtilLib::GetColor(UtilLib::enColor::Green) << "  [+] First Name added.\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                break;
            case 2:
                UpdateMask |= enUpdateFields::eLastName;
                cout << UtilLib::GetColor(UtilLib::enColor::Green) << "  [+] Last Name added.\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                break;
            case 3:
                UpdateMask |= enUpdateFields::eEmail;
                cout << UtilLib::GetColor(UtilLib::enColor::Green) << "  [+] Email Address added.\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                break;
            case 4:
                UpdateMask |= enUpdateFields::ePhone;
                cout << UtilLib::GetColor(UtilLib::enColor::Green) << "  [+] Phone Number added.\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                break;
            case 5:
                UpdateMask |= enUpdateFields::ePinCode;
                cout << UtilLib::GetColor(UtilLib::enColor::Green) << "  [+] PIN Code added.\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                break;
            case 6:
                UpdateMask |= enUpdateFields::eBalance;
                cout << UtilLib::GetColor(UtilLib::enColor::Green) << "  [+] Balance added.\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                break;
            }
        }
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        return UpdateMask;
    }

    void _ReadClientInfoByMask(clsBankClient &Client, unsigned short Mask)
    {
        cout << "\n"
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow) << "  [+] ENTER NEW DATA FOR SELECTED FIELDS:" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        if (Mask & enUpdateFields::eFirstName)
        {
            Client.SetFirstName(InputValidateLib::ReadLimitedText(25, "  [>] Enter New First Name   : "));
        }

        if (Mask & enUpdateFields::eLastName)
        {
            Client.SetLastName(InputValidateLib::ReadLimitedText(25, "  [>] Enter New Last Name    : "));
        }

        if (Mask & enUpdateFields::eEmail)
        {
            Client.SetEmail(InputValidateLib::ReadLimitedText(28, "  [>] Enter New Email Address: "));
        }

        if (Mask & enUpdateFields::ePhone)
        {
            Client.SetPhone(InputValidateLib::ReadLimitedText(10, "  [>] Enter New Phone Number : "));
        }

        if (Mask & enUpdateFields::ePinCode)
        {
            Client.SetPinCode(InputValidateLib::GetValidPIN("  [>] Enter New PIN Code     : "));
        }

        if (Mask & enUpdateFields::eBalance)
        {
            Client.SetAccountBalance(InputValidateLib::ReadDblPositiveNumber("  [>] Enter New Balance      : "));
        }

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
    }

    bool _Show()
    {
        if (!_CheckAccessRights(clsUser::enMainMenuPermissions::pUpdateClients))
            return false;

        _ResetTheScreen();
        _DrawScreenHeader("UPDATE CLIENT DASHBOARD", "Modify Existing Client Details Selectively");

        clsBankClient Client = _GetExistingClient("  [>] Enter Account Number: ");
        _PrintClientCard(Client, "CURRENT CLIENT CARD DETAILS");

        string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) + "  [?] Are you sure you want to update this client? (y/n): " + UtilLib::GetColor(UtilLib::enColor::Reset);

        char Answer = InputValidateLib::getYesNoAnswer(PromptMsg);

        if (tolower(Answer) == 'y')
        {
            unsigned short UpdateMask = _ReadUpdateMask();

            _ReadClientInfoByMask(Client, UpdateMask);

            if (!_ConfirmUserPassword("UPDATE SENSITIVE CLIENT DATA"))
            {
                return true;
            }

            cout << "\n";
            _ShowProgressBar("Updating client record in database...");

            clsBankClient::enSaveResults SaveResult = Client.Save();

            switch (SaveResult)
            {
            case clsBankClient::enSaveResults::svSucceeded:
                cout << "\n\n";
                _PrintAnimatedSuccess("  [!] Client Record Updated Successfully!", 20);
                _PrintClientCard(Client, "UPDATED CLIENT CARD DETAILS");
                break;

            case clsBankClient::enSaveResults::svFailedEmptyObject:
                _ShowWarningMessage("ERROR: Update failed because the client object is empty!");
                break;

            default:
                break;
            }
        else
        {
            cout << UtilLib::GetColor(UtilLib::enColor::Yellow)
                 << "\n  [i] Update operation cancelled. Client record remains unchanged.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
        }

        return true;
    }

public:
    static bool ShowUpdateClient()
    {
        clsUpdateClientScreen UpdateClientScreen;
        return UpdateClientScreen._Show();
    }
};