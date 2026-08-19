#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>

#include "Global.h"
#include "clsPerson.h"
#include "MyStringLib.h"
#include "UtilLib.h"

/**
 * @class clsUser
 * @brief Domain Entity representing system operators, authentication profiles, and RBAC permissions.
 */
class clsUser : public clsPerson
{
public:
    // -------------------------------------------------------------------------
    // Types, Structs & Enums
    // -------------------------------------------------------------------------
    struct stLoginRegisterRecord
    {
        std::string DateTime;
        std::string UserName;
        std::string Password;
        int Permissions = 0;
    };

    enum enMainMenuPermissions
    {
        eAll = -1,
        pListClients = 1,
        pAddNewClient = 2,
        pDeleteClient = 4,
        pUpdateClients = 8,
        pFindClient = 16,
        pTransactions = 32,
        pManageUsers = 64,
        pLoginRegister = 128,
        pCurrencyExchange = 256
    };

    enum class enMode
    {
        EmptyMode = 0,
        UpdateMode = 1,
        AddNewMode = 2
    };

    enum class enSaveResults
    {
        svFailedEmptyObject = 0,
        svSucceeded = 1,
        svFailedUserNameExists = 2
    };

    static clsUser GetEmptyUserObject()
    {
        return clsUser(enMode::EmptyMode, "", "", "", "", "", "", 0);
    }

private:
    enMode _Mode;
    std::string _UserName;
    std::string _Password;
    int _Permissions = 0;
    bool _MarkedForDelete = false;

    // Private Constructor
    clsUser(enMode Mode, std::string FirstName, std::string LastName,
            std::string Email, std::string Phone, std::string UserName,
            std::string Password, int Permissions)
        : clsPerson(std::move(FirstName), std::move(LastName), std::move(Email), std::move(Phone)),
          _Mode(Mode),
          _UserName(std::move(UserName)),
          _Password(std::move(Password)),
          _Permissions(Permissions)
    {
    }

    // -------------------------------------------------------------------------
    // Private Serialization & File I/O (Users)
    // -------------------------------------------------------------------------
    static clsUser _ConvertLineToUserObject(const std::string &Line, const std::string &Separator = "#//#")
    {
        const std::vector<std::string> vData = MyStringLib::SplitString(Line, Separator);
        if (vData.size() < 7)
            return GetEmptyUserObject();

        int Permissions = 0;
        try
        {
            Permissions = std::stoi(vData[6]);
        }
        catch (...)
        {
            Permissions = 0;
        }

        const std::string DecryptedPassword = UtilLib::DecryptText(vData[5], Global::EncryptionKey);

        return clsUser(enMode::UpdateMode, vData[0], vData[1], vData[2], vData[3], vData[4], DecryptedPassword, Permissions);
    }

    static std::string _ConvertUserObjectToLine(const clsUser &User, const std::string &Separator = "#//#")
    {
        const std::string EncryptedPassword = UtilLib::EncryptText(User.GetPassword(), Global::EncryptionKey);

        return User.GetFirstName() + Separator +
               User.GetLastName() + Separator +
               User.GetEmail() + Separator +
               User.GetPhone() + Separator +
               User.GetUserName() + Separator +
               EncryptedPassword + Separator +
               std::to_string(User.GetPermissions());
    }

    static std::vector<clsUser> _LoadUsersDataFromFile()
    {
        std::vector<clsUser> vUsers;
        std::fstream MyFile(Global::UsersFilePath, std::ios::in);

        if (MyFile.is_open())
        {
            std::string Line;
            while (std::getline(MyFile, Line))
            {
                if (!Line.empty())
                    vUsers.push_back(_ConvertLineToUserObject(Line));
            }
            MyFile.close();
        }
        return vUsers;
    }

    static void _SaveUsersDataToFile(const std::vector<clsUser> &vUsers)
    {
        std::fstream MyFile(Global::UsersFilePath, std::ios::out);
        if (MyFile.is_open())
        {
            for (const auto &User : vUsers)
            {
                if (!User._MarkedForDelete)
                {
                    MyFile << _ConvertUserObjectToLine(User) << "\n";
                }
            }
            MyFile.close();
        }
    }

    void _AddDataLineToFile(const std::string &DataLine)
    {
        std::fstream MyFile(Global::UsersFilePath, std::ios::out | std::ios::app);
        if (MyFile.is_open())
        {
            MyFile << DataLine << "\n";
            MyFile.close();
        }
    }

    void _Update()
    {
        std::vector<clsUser> vUsers = _LoadUsersDataFromFile();
        for (auto &U : vUsers)
        {
            if (U.GetUserName() == _UserName)
            {
                U = *this;
                break;
            }
        }
        _SaveUsersDataToFile(vUsers);
    }

    void _AddNew()
    {
        _AddDataLineToFile(_ConvertUserObjectToLine(*this));
    }

    bool _MarkForDelete(std::vector<clsUser> &vUsers)
    {
        for (auto &U : vUsers)
        {
            if (U.GetUserName() == _UserName)
            {
                U._MarkedForDelete = true;
                return true;
            }
        }
        return false;
    }

    // -------------------------------------------------------------------------
    // Private Serialization & File I/O (Login Register Log)
    // -------------------------------------------------------------------------
    std::string _PrepareLoginRecord(const std::string &Separator = "#//#") const
    {
        return UtilLib::GetSystemDateTime() + Separator +
               GetUserName() + Separator +
               UtilLib::EncryptText(GetPassword(), Global::EncryptionKey) + Separator +
               std::to_string(GetPermissions());
    }

    static stLoginRegisterRecord _ConvertLoginRegisterLineToRecord(const std::string &Line, const std::string &Separator = "#//#")
    {
        const std::vector<std::string> vData = MyStringLib::SplitString(Line, Separator);
        if (vData.size() < 4)
            return {};

        int Permissions = 0;
        try
        {
            Permissions = std::stoi(vData[3]);
        }
        catch (...)
        {
            Permissions = 0;
        }

        const std::string DecryptedPassword = UtilLib::DecryptText(vData[2], Global::EncryptionKey);

        return {vData[0], vData[1], DecryptedPassword, Permissions};
    }

    static std::vector<stLoginRegisterRecord> _LoadLoginRegisterDataFromFile()
    {
        std::vector<stLoginRegisterRecord> vRecords;
        std::fstream MyFile(Global::LoginRegisterFilePath, std::ios::in);

        if (MyFile.is_open())
        {
            std::string Line;
            while (std::getline(MyFile, Line))
            {
                if (!Line.empty())
                    vRecords.push_back(_ConvertLoginRegisterLineToRecord(Line));
            }
            MyFile.close();
        }
        return vRecords;
    }

public:
    // Status Queries & Validation
    bool IsEmpty() const noexcept { return (_Mode == enMode::EmptyMode); }
    bool IsMarkedForDelete() const noexcept { return _MarkedForDelete; }
    bool VerifyPassword(const std::string &InputPassword) const noexcept { return (_Password == InputPassword); }

    // Setters
    void SetUserName(std::string UserName) { _UserName = std::move(UserName); }
    void SetPassword(std::string Password) { _Password = std::move(Password); }
    void SetPermissions(int Permissions) noexcept { _Permissions = Permissions; }

    // Getters
    const std::string& GetUserName() const noexcept { return _UserName; }
    const std::string& GetPassword() const noexcept { return _Password; }
    int GetPermissions() const noexcept { return _Permissions; }

    // -------------------------------------------------------------------------
    // Static Lookup & User Retrieval
    // -------------------------------------------------------------------------
    static clsUser Find(const std::string &UserName)
    {
        const std::vector<clsUser> vUsers = _LoadUsersDataFromFile();
        for (const auto &User : vUsers)
        {
            if (User.GetUserName() == UserName)
                return User;
        }
        return GetEmptyUserObject();
    }

    static clsUser Find(const std::string &UserName, const std::string &Password)
    {
        clsUser User = Find(UserName);
        if (!User.IsEmpty() && User.GetPassword() == Password)
            return User;

        return GetEmptyUserObject();
    }

    static bool IsUserExist(const std::string &UserName)
    {
        return (!Find(UserName).IsEmpty());
    }

    static clsUser GetAddNewUserObject(std::string UserName)
    {
        return clsUser(enMode::AddNewMode, "", "", "", "", std::move(UserName), "", 0);
    }

    static std::vector<clsUser> GetUsersList()
    {
        return _LoadUsersDataFromFile();
    }

    static std::vector<stLoginRegisterRecord> GetLoginRegisterList()
    {
        return _LoadLoginRegisterDataFromFile();
    }

    // -------------------------------------------------------------------------
    // Management & Access Control
    // -------------------------------------------------------------------------
    enSaveResults Save()
    {
        switch (_Mode)
        {
        case enMode::EmptyMode:
            return enSaveResults::svFailedEmptyObject;

        case enMode::UpdateMode:
            _Update();
            return enSaveResults::svSucceeded;

        case enMode::AddNewMode:
            if (IsUserExist(_UserName))
            {
                return enSaveResults::svFailedUserNameExists;
            }
            _AddNew();
            _Mode = enMode::UpdateMode;
            return enSaveResults::svSucceeded;

        default:
            return enSaveResults::svFailedEmptyObject;
        }
    }

    bool Delete()
    {
        if (IsEmpty())
            return false;

        std::vector<clsUser> vUsers = _LoadUsersDataFromFile();
        if (_MarkForDelete(vUsers))
        {
            _SaveUsersDataToFile(vUsers);
            *this = GetEmptyUserObject();
            return true;
        }
        return false;
    }

    bool CheckAccessPermission(enMainMenuPermissions Permission) const noexcept
    {
        if (_Permissions == enMainMenuPermissions::eAll)
            return true;

        if ((Permission & _Permissions) == Permission)
            return true;

        return false;
    }

    void RegisterLogin() const
    {
        std::fstream MyFile(Global::LoginRegisterFilePath, std::ios::out | std::ios::app);
        if (MyFile.is_open())
        {
            MyFile << _PrepareLoginRecord() << "\n";
            MyFile.close();
        }
    }
};