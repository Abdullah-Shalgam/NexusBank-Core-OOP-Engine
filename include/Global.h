#pragma once
#include <string>
#include <fstream>

namespace Global
{
    inline const std::string AppName = "Core Banking Suite";
    inline const std::string AppVersion = "v2.5.0";
    inline const std::string CompanyName = "SecureBank Systems Inc.";

    inline const short MaxLoginAttempts = 3;
    inline const short EncryptionKey = 10;

    inline std::string ResolveDataPath(const std::string& fileName)
    {
        std::string path1 = "data/" + fileName;
        std::ifstream file1(path1.c_str());
        if (file1.good())
        {
            file1.close();
            return path1;
        }

        std::string path2 = "../data/" + fileName;
        std::ifstream file2(path2.c_str());
        if (file2.good())
        {
            file2.close();
            return path2;
        }

        return "data/" + fileName;
    }

    inline const std::string ClientsFilePath = ResolveDataPath("Clients.txt");
    inline const std::string UsersFilePath = ResolveDataPath("Users.txt");
    inline const std::string CurrenciesFilePath = ResolveDataPath("Currencies.txt");
    inline const std::string LoginRegisterFilePath = ResolveDataPath("LoginRegister.txt");
    inline const std::string TransferLogFilePath = ResolveDataPath("TransferLog.txt");
}

#include "clsUser.h"

namespace Global
{
    inline clsUser CurrentUser = clsUser::Find("", "");
    inline std::string CurrentUserLoginTime = "";
}