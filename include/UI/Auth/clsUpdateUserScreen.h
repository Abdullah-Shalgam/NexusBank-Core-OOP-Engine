#pragma once
#include <iostream>
#include "clsUserScreenBase.h"
#include "clsUser.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsUpdateUserScreen : protected clsUserScreenBase
{
private:
    enum enUpdateFields
    {
        eNone = 0,             // 0
        eFirstName = 1 << 0,   // 1
        eLastName = 1 << 1,    // 2
        eEmail = 1 << 2,       // 4
        ePhone = 1 << 3,       // 8
        ePassword = 1 << 4,    // 16
        ePermissions = 1 << 5, // 32
        eAll = 63              // 63
    };

    clsUpdateUserScreen() : clsUserScreenBase(122) {}

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
        PrintOption("5", "Password");
        PrintOption("6", "Permissions");
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
                UpdateMask |= enUpdateFields::ePassword;
                cout << UtilLib::GetColor(UtilLib::enColor::Green) << "  [+] Password added.\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                break;
            case 6:
                UpdateMask |= enUpdateFields::ePermissions;
                cout << UtilLib::GetColor(UtilLib::enColor::Green) << "  [+] Permissions added.\n"
                     << UtilLib::GetColor(UtilLib::enColor::Reset);
                break;
            }
        }
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        return UpdateMask;
    }

    void _ReadUserInfoByMask(clsUser &User, unsigned short Mask)
    {
        cout << "\n"
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow) << "  [+] ENTER NEW DATA FOR SELECTED FIELDS:" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";

        if (Mask & enUpdateFields::eFirstName)
        {
            User.SetFirstName(InputValidateLib::ReadLimitedText(25, "  [>] Enter New First Name   : "));
        }

        if (Mask & enUpdateFields::eLastName)
        {
            User.SetLastName(InputValidateLib::ReadLimitedText(25, "  [>] Enter New Last Name    : "));
        }

        if (Mask & enUpdateFields::eEmail)
        {
            User.SetEmail(InputValidateLib::ReadLimitedText(28, "  [>] Enter New Email Address: "));
        }

        if (Mask & enUpdateFields::ePhone)
        {
            User.SetPhone(InputValidateLib::ReadLimitedText(10, "  [>] Enter New Phone Number : "));
        }

        if (Mask & enUpdateFields::ePassword)
        {
            User.SetPassword(InputValidateLib::ReadLimitedText(10, "  [>] Enter New Password     : "));
        }

        if (Mask & enUpdateFields::ePermissions)
        {
            cout << "\n";
            User.SetPermissions(_ReadPermissionsToSet());
        }

        cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------" << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n";
    }

    void _Show()
    {
        _ResetTheScreen();
        _DrawScreenHeader("UPDATE USER DASHBOARD", "Modify Existing User Details Selectively");

        clsUser User = _GetExistingUser("  [>] Enter Username: ");
        _PrintUserCard(User, "CURRENT USER CARD DETAILS");

        string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) + "  [?] Are you sure you want to update this user? (y/n): " + UtilLib::GetColor(UtilLib::enColor::Reset);

        char Answer = InputValidateLib::getYesNoAnswer(PromptMsg);

        if (tolower(Answer) == 'y')
        {
            unsigned short UpdateMask = _ReadUpdateMask();

            _ReadUserInfoByMask(User, UpdateMask);

            if (!_ConfirmUserPassword("MODIFY USER CREDENTIALS & PERMISSIONS"))
            {
                return;
            }

            cout << "\n";
            _ShowProgressBar("Updating user record in database...");

            clsUser::enSaveResults SaveResult = User.Save();

            switch (SaveResult)
            {
            case clsUser::enSaveResults::svSucceeded:
                cout << "\n\n";
                _PrintAnimatedSuccess("  [!] User Record Updated Successfully!", 20);
                _PrintUserCard(User, "UPDATED USER CARD DETAILS");
                break;

            case clsUser::enSaveResults::svFailedEmptyObject:
                _ShowWarningMessage("ERROR: Update failed because the user object is empty!");
                break;

            default:
                break;
            }
        }
        else
        {
            cout << UtilLib::GetColor(UtilLib::enColor::Yellow)
                 << "\n  [i] Update operation cancelled. User record remains unchanged.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);
        }
    }

public:
    static void ShowUpdateUser()
    {
        clsUpdateUserScreen UpdateUserScreen;
        UpdateUserScreen._Show();
    }
};