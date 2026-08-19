#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>

#include "Global.h"
#include "MyStringLib.h"

/**
 * @class clsCurrency
 * @brief Domain Entity & Service Model managing foreign exchange rates and financial conversions.
 */
class clsCurrency
{
private:
    enum class enMode
    {
        EmptyMode = 0,
        UpdateMode = 1
    };
    enMode _Mode;

    std::string _Country;
    std::string _CurrencyCode;
    std::string _CurrencyName;
    double _Rate;

    inline static std::string _SystemCurrencyCode = "USD";

    // -------------------------------------------------------------------------
    // Private Serialization & File I/O Helpers
    // -------------------------------------------------------------------------
    static clsCurrency _ConvertLineToCurrencyObject(const std::string &Line, const std::string &Separator = "#//#")
    {
        const std::vector<std::string> vData = MyStringLib::SplitString(Line, Separator);
        if (vData.size() < 4)
            return GetEmptyCurrencyObject();

        double Rate = 0.0;
        try
        {
            Rate = std::stod(vData[3]);
        }
        catch (...)
        {
            Rate = 0.0;
        }

        return clsCurrency(enMode::UpdateMode, vData[0], vData[1], vData[2], Rate);
    }

    static std::string _ConvertCurrencyObjectToLine(const clsCurrency &Currency, const std::string &Separator = "#//#")
    {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(4) << Currency.GetRate();

        return Currency.GetCountry() + Separator +
               Currency.GetCurrencyCode() + Separator +
               Currency.GetCurrencyName() + Separator +
               ss.str();
    }

    static std::vector<clsCurrency> _LoadCurrenciesDataFromFile()
    {
        std::vector<clsCurrency> vCurrencies;
        std::fstream MyFile(Global::CurrenciesFilePath, std::ios::in);

        if (MyFile.is_open())
        {
            std::string Line;
            while (std::getline(MyFile, Line))
            {
                if (!Line.empty())
                {
                    vCurrencies.push_back(_ConvertLineToCurrencyObject(Line));
                }
            }
            MyFile.close();
        }
        return vCurrencies;
    }

    static void _SaveCurrencyDataToFile(const std::vector<clsCurrency> &vCurrencies)
    {
        std::fstream MyFile(Global::CurrenciesFilePath, std::ios::out);
        if (MyFile.is_open())
        {
            for (const clsCurrency &C : vCurrencies)
            {
                MyFile << _ConvertCurrencyObjectToLine(C) << "\n";
            }
            MyFile.close();
        }
    }

    void _Update()
    {
        std::vector<clsCurrency> vCurrencies = _LoadCurrenciesDataFromFile();
        for (clsCurrency &C : vCurrencies)
        {
            if (C.GetCurrencyCode() == GetCurrencyCode())
            {
                C = *this;
                break;
            }
        }
        _SaveCurrencyDataToFile(vCurrencies);
    }

public:
    // Constructors
    clsCurrency(enMode Mode, std::string Country, std::string CurrencyCode, std::string CurrencyName, double Rate)
        : _Mode(Mode),
          _Country(std::move(Country)),
          _CurrencyCode(std::move(CurrencyCode)),
          _CurrencyName(std::move(CurrencyName)),
          _Rate(Rate)
    {
    }

    static clsCurrency GetEmptyCurrencyObject()
    {
        return clsCurrency(enMode::EmptyMode, "", "", "", 0.0);
    }

    // Status Queries
    bool IsEmpty() const noexcept { return (_Mode == enMode::EmptyMode); }

    // Getters (Zero-copy const references)
    const std::string& GetCountry() const noexcept { return _Country; }
    const std::string& GetCurrencyCode() const noexcept { return _CurrencyCode; }
    const std::string& GetCurrencyName() const noexcept { return _CurrencyName; }
    double GetRate() const noexcept { return _Rate; }

    void UpdateRate(double NewRate)
    {
        _Rate = NewRate;
        _Update();
    }

    // -------------------------------------------------------------------------
    // Financial Calculations & Conversions
    // -------------------------------------------------------------------------
    double ConvertToUSD(double Amount) const noexcept
    {
        if (_Rate <= 0.0)
            return 0.0;
        return Amount / _Rate;
    }

    double ConvertToOtherCurrency(double Amount, const clsCurrency &TargetCurrency) const noexcept
    {
        const double AmountInUSD = ConvertToUSD(Amount);
        if (TargetCurrency.GetCurrencyCode() == "USD")
            return AmountInUSD;

        return AmountInUSD * TargetCurrency.GetRate();
    }

    // -------------------------------------------------------------------------
    // System Currency Configuration
    // -------------------------------------------------------------------------
    static clsCurrency GetSystemCurrency()
    {
        clsCurrency Currency = FindByCode(_SystemCurrencyCode);
        if (Currency.IsEmpty())
        {
            _SystemCurrencyCode = "USD";
            return FindByCode("USD");
        }
        return Currency;
    }

    static void SetSystemCurrency(const clsCurrency &Currency)
    {
        if (!Currency.IsEmpty())
        {
            _SystemCurrencyCode = Currency.GetCurrencyCode();
        }
    }

    static double ConvertFromBaseUSD(double AmountInUSD)
    {
        const clsCurrency SysCurrency = GetSystemCurrency();
        return AmountInUSD * SysCurrency.GetRate();
    }

    static double ConvertToBaseUSD(double AmountInSystemCurrency)
    {
        const clsCurrency SysCurrency = GetSystemCurrency();
        if (SysCurrency.GetRate() <= 0.0)
            return 0.0;
        return AmountInSystemCurrency / SysCurrency.GetRate();
    }

    static std::string FormatSystemAmount(double AmountInUSD)
    {
        const clsCurrency SysCurrency = GetSystemCurrency();
        const double ConvertedAmount = ConvertFromBaseUSD(AmountInUSD);

        std::ostringstream ss;
        ss << "$ " << std::fixed << std::setprecision(2) << ConvertedAmount << " " << SysCurrency.GetCurrencyCode();
        return ss.str();
    }

    // -------------------------------------------------------------------------
    // Static Lookup Methods
    // -------------------------------------------------------------------------
    static clsCurrency FindByCode(std::string CurrencyCode)
    {
        CurrencyCode = MyStringLib::CapitalizeAllString(std::move(CurrencyCode));
        const std::vector<clsCurrency> vCurrencies = _LoadCurrenciesDataFromFile();

        for (const clsCurrency &C : vCurrencies)
        {
            if (C.GetCurrencyCode() == CurrencyCode)
            {
                return C;
            }
        }
        return GetEmptyCurrencyObject();
    }

    static clsCurrency FindByCountry(std::string Country)
    {
        Country = MyStringLib::CapitalizeAllString(std::move(Country));
        const std::vector<clsCurrency> vCurrencies = _LoadCurrenciesDataFromFile();

        for (const clsCurrency &C : vCurrencies)
        {
            if (MyStringLib::CapitalizeAllString(C.GetCountry()) == Country)
            {
                return C;
            }
        }
        return GetEmptyCurrencyObject();
    }

    static bool IsCurrencyExist(const std::string &CurrencyCode)
    {
        const clsCurrency C = FindByCode(CurrencyCode);
        return (!C.IsEmpty());
    }

    static std::vector<clsCurrency> GetCurrenciesList()
    {
        return _LoadCurrenciesDataFromFile();
    }
};