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
 * @class clsBankClient
 * @brief Core Domain Entity representing bank client accounts and ledger operations.
 */
class clsBankClient : public clsPerson
{
public:
    // -------------------------------------------------------------------------
    // Types, Structs & Enums
    // -------------------------------------------------------------------------
    struct stTransferLogRecord
    {
        std::string DateTime;
        std::string SenderAccountNumber;
        std::string DestinationAccountNumber;
        double Amount = 0.0;
        double SenderBalanceAfter = 0.0;
        double DestinationBalanceAfter = 0.0;
        std::string UserName;
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
        svFailedAccountNumberExists = 2
    };

private:
    enMode _Mode;
    std::string _AccountNumber;
    std::string _PinCode;
    double _AccountBalance = 0.0;
    bool _MarkedForDelete = false;

    // Private Constructor
    clsBankClient(enMode Mode, std::string FirstName, std::string LastName,
                  std::string Email, std::string Phone, std::string AccountNumber,
                  std::string PinCode, double AccountBalance)
        : clsPerson(std::move(FirstName), std::move(LastName), std::move(Email), std::move(Phone)),
          _Mode(Mode),
          _AccountNumber(std::move(AccountNumber)),
          _PinCode(std::move(PinCode)),
          _AccountBalance(AccountBalance)
    {
    }

    // -------------------------------------------------------------------------
    // Private Serialization & File I/O (Clients)
    // -------------------------------------------------------------------------
    static clsBankClient _ConvertLineToClientObject(const std::string &Line, const std::string &Separator = "#//#")
    {
        const std::vector<std::string> vData = MyStringLib::SplitString(Line, Separator);
        if (vData.size() < 7)
            return _GetEmptyClientObject();

        double Balance = 0.0;
        try
        {
            Balance = std::stod(vData[6]);
        }
        catch (...)
        {
            Balance = 0.0;
        }

        const std::string DecryptedPin = UtilLib::DecryptText(vData[5], Global::EncryptionKey);

        return clsBankClient(enMode::UpdateMode, vData[0], vData[1], vData[2], vData[3], vData[4], DecryptedPin, Balance);
    }

    static std::string _ConvertClientObjectToLine(const clsBankClient &Client, const std::string &Separator = "#//#")
    {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(6) << Client.GetAccountBalance();

        const std::string EncryptedPin = UtilLib::EncryptText(Client.GetPinCode(), Global::EncryptionKey);

        return Client.GetFirstName() + Separator +
               Client.GetLastName() + Separator +
               Client.GetEmail() + Separator +
               Client.GetPhone() + Separator +
               Client.GetAccountNumber() + Separator +
               EncryptedPin + Separator +
               ss.str();
    }

    static std::vector<clsBankClient> _LoadClientsDataFromFile()
    {
        std::vector<clsBankClient> vClients;
        std::fstream MyFile(Global::ClientsFilePath, std::ios::in);

        if (MyFile.is_open())
        {
            std::string Line;
            while (std::getline(MyFile, Line))
            {
                if (!Line.empty())
                    vClients.push_back(_ConvertLineToClientObject(Line));
            }
            MyFile.close();
        }
        return vClients;
    }

    static void _SaveClientsDataToFile(const std::vector<clsBankClient> &vClients)
    {
        std::fstream MyFile(Global::ClientsFilePath, std::ios::out);
        if (MyFile.is_open())
        {
            for (const auto &Client : vClients)
            {
                if (!Client._MarkedForDelete)
                {
                    MyFile << _ConvertClientObjectToLine(Client) << "\n";
                }
            }
            MyFile.close();
        }
    }

    void _AddDataLineToFile(const std::string &DataLine)
    {
        std::fstream MyFile(Global::ClientsFilePath, std::ios::out | std::ios::app);
        if (MyFile.is_open())
        {
            MyFile << DataLine << "\n";
            MyFile.close();
        }
    }

    void _Update()
    {
        std::vector<clsBankClient> vClients = _LoadClientsDataFromFile();
        for (auto &C : vClients)
        {
            if (C.GetAccountNumber() == GetAccountNumber())
            {
                C = *this;
                break;
            }
        }
        _SaveClientsDataToFile(vClients);
    }

    void _AddNew()
    {
        _AddDataLineToFile(_ConvertClientObjectToLine(*this));
    }

    static clsBankClient _GetEmptyClientObject()
    {
        return clsBankClient(enMode::EmptyMode, "", "", "", "", "", "", 0.0);
    }

    bool _MarkForDelete(std::vector<clsBankClient> &vClients)
    {
        for (auto &C : vClients)
        {
            if (C.GetAccountNumber() == _AccountNumber)
            {
                C._MarkedForDelete = true;
                return true;
            }
        }
        return false;
    }

    // -------------------------------------------------------------------------
    // Private Serialization & File I/O (Transfer Log)
    // -------------------------------------------------------------------------
    std::string _PrepareTransferLog(double Amount, const clsBankClient &DestinationClient,
                                   const std::string &UserName, const std::string &Separator = "#//#") const
    {
        std::ostringstream ssAmount, ssSenderBal, ssDestBal;
        ssAmount << std::fixed << std::setprecision(6) << Amount;
        ssSenderBal << std::fixed << std::setprecision(6) << GetAccountBalance();
        ssDestBal << std::fixed << std::setprecision(6) << DestinationClient.GetAccountBalance();

        return UtilLib::GetSystemDateTime() + Separator +
               GetAccountNumber() + Separator +
               DestinationClient.GetAccountNumber() + Separator +
               ssAmount.str() + Separator +
               ssSenderBal.str() + Separator +
               ssDestBal.str() + Separator +
               UserName;
    }

    void _RegisterTransferLog(double Amount, const clsBankClient &DestinationClient, const std::string &UserName)
    {
        std::fstream MyFile(Global::TransferLogFilePath, std::ios::out | std::ios::app);
        if (MyFile.is_open())
        {
            MyFile << _PrepareTransferLog(Amount, DestinationClient, UserName) << "\n";
            MyFile.close();
        }
    }

    static stTransferLogRecord _ConvertTransferLogLineToRecord(const std::string &Line, const std::string &Separator = "#//#")
    {
        const std::vector<std::string> vData = MyStringLib::SplitString(Line, Separator);
        if (vData.size() < 7)
            return {};

        double Amount = 0.0, SBal = 0.0, DBal = 0.0;
        try
        {
            Amount = std::stod(vData[3]);
            SBal = std::stod(vData[4]);
            DBal = std::stod(vData[5]);
        }
        catch (...)
        {
        }

        return {vData[0], vData[1], vData[2], Amount, SBal, DBal, vData[6]};
    }

    static std::vector<stTransferLogRecord> _LoadTransferLogDataFromFile()
    {
        std::vector<stTransferLogRecord> vRecords;
        std::fstream MyFile(Global::TransferLogFilePath, std::ios::in);

        if (MyFile.is_open())
        {
            std::string Line;
            while (std::getline(MyFile, Line))
            {
                if (!Line.empty())
                    vRecords.push_back(_ConvertTransferLogLineToRecord(Line));
            }
            MyFile.close();
        }
        return vRecords;
    }

public:
    // Status Queries
    bool IsEmpty() const noexcept { return (_Mode == enMode::EmptyMode); }
    bool IsMarkedForDelete() const noexcept { return _MarkedForDelete; }

    // Setters
    void SetPinCode(std::string PinCode) { _PinCode = std::move(PinCode); }
    void SetAccountBalance(double AccountBalance) noexcept { _AccountBalance = AccountBalance; }

    // Getters
    const std::string& GetAccountNumber() const noexcept { return _AccountNumber; }
    const std::string& GetPinCode() const noexcept { return _PinCode; }
    double GetAccountBalance() const noexcept { return _AccountBalance; }

    // -------------------------------------------------------------------------
    // Static Query Operations
    // -------------------------------------------------------------------------
    static clsBankClient Find(const std::string &AccountNumber)
    {
        std::fstream MyFile(Global::ClientsFilePath, std::ios::in);
        if (MyFile.is_open())
        {
            std::string line;
            while (std::getline(MyFile, line))
            {
                if (!line.empty())
                {
                    clsBankClient Client = _ConvertLineToClientObject(line);
                    if (Client.GetAccountNumber() == AccountNumber)
                    {
                        MyFile.close();
                        return Client;
                    }
                }
            }
            MyFile.close();
        }
        return _GetEmptyClientObject();
    }

    static clsBankClient Find(const std::string &AccountNumber, const std::string &PinCode)
    {
        clsBankClient Client = Find(AccountNumber);
        if (!Client.IsEmpty() && Client.GetPinCode() == PinCode)
            return Client;

        return _GetEmptyClientObject();
    }

    static bool IsClientExist(const std::string &AccountNumber)
    {
        return (!Find(AccountNumber).IsEmpty());
    }

    static clsBankClient GetAddNewClientObject(std::string AccountNumber)
    {
        return clsBankClient(enMode::AddNewMode, "", "", "", "", std::move(AccountNumber), "", 0.0);
    }

    static std::vector<clsBankClient> GetClientsList()
    {
        return _LoadClientsDataFromFile();
    }

    static double GetTotalBalances()
    {
        const std::vector<clsBankClient> vClients = GetClientsList();
        double TotalBalances = 0.0;
        for (const auto &Client : vClients)
        {
            TotalBalances += Client.GetAccountBalance();
        }
        return TotalBalances;
    }

    static std::vector<stTransferLogRecord> GetTransferLogList()
    {
        return _LoadTransferLogDataFromFile();
    }

    // -------------------------------------------------------------------------
    // Ledger & Transaction Operations
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
            if (IsClientExist(_AccountNumber))
            {
                return enSaveResults::svFailedAccountNumberExists;
            }
            _AddNew();
            _Mode = enMode::UpdateMode;
            return enSaveResults::svSucceeded;

        default:
            return enSaveResults::svFailedEmptyObject;
        }
    }

    bool Deposit(double Amount)
    {
        if (Amount <= 0.0)
            return false;

        _AccountBalance += Amount;
        return (Save() == enSaveResults::svSucceeded);
    }

    bool Withdraw(double Amount)
    {
        if (Amount <= 0.0 || Amount > _AccountBalance)
            return false;

        _AccountBalance -= Amount;
        return (Save() == enSaveResults::svSucceeded);
    }

    bool Transfer(double Amount, clsBankClient &DestinationClient, const std::string &UserName)
    {
        if (Amount <= 0.0 || Amount > _AccountBalance)
            return false;

        if (!Withdraw(Amount))
            return false;

        if (!DestinationClient.Deposit(Amount))
        {
            // Rollback in memory and file
            Deposit(Amount);
            return false;
        }

        _RegisterTransferLog(Amount, DestinationClient, UserName);
        return true;
    }

    bool Delete()
    {
        if (IsEmpty())
            return false;

        std::vector<clsBankClient> vClients = _LoadClientsDataFromFile();
        if (_MarkForDelete(vClients))
        {
            _SaveClientsDataToFile(vClients);
            *this = _GetEmptyClientObject();
            return true;
        }
        return false;
    }
};