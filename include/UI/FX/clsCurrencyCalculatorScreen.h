#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include "clsCurrencyScreenBase.h"
#include "clsCurrency.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsCurrencyCalculatorScreen : protected clsCurrencyScreenBase
{
private:
    clsCurrencyCalculatorScreen() : clsCurrencyScreenBase(122) {}

    clsCurrency _GetExistingCurrency(const string &PromptMsg)
    {
        cout << "\n";
        string CurrencyCode = InputValidateLib::ReadLimitedText(3, PromptMsg);
        clsCurrency Currency = clsCurrency::FindByCode(CurrencyCode);

        while (Currency.IsEmpty())
        {
            cout << "\n"
                 << UtilLib::GetColor(UtilLib::enColor::BrightRed)
                 << "  [!] Currency Code [" << CurrencyCode << "] was not found! Please try again.\n"
                 << UtilLib::GetColor(UtilLib::enColor::Reset);

            CurrencyCode = InputValidateLib::ReadLimitedText(3, PromptMsg);
            Currency = clsCurrency::FindByCode(CurrencyCode);
        }

        return Currency;
    }

    void _PrintConversionResultCard(double Amount, clsCurrency &FromCurrency, clsCurrency &ToCurrency)
    {
        double AmountInUSD = FromCurrency.ConvertToUSD(Amount);
        double ConvertedAmount = FromCurrency.ConvertToOtherCurrency(Amount, ToCurrency);

        const int BoxWidth = 65;
        const int LeftMargin = (_ScreenWidth - BoxWidth) / 2;
        const string Indent(LeftMargin, ' ');
        const string BorderLine = string(BoxWidth, '=');

        cout << "\n"
             << Indent << UtilLib::GetColor(UtilLib::enColor::BrightCyan) << BorderLine << endl;

        string Title = "EXCHANGE TRANSACTION SUMMARY";
        int LeftSpaces = (BoxWidth - 4 - (int)Title.length()) / 2;
        cout << Indent << "| " << string(LeftSpaces, ' ')
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow) << Title
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan)
             << string(BoxWidth - 4 - (int)Title.length() - LeftSpaces, ' ') << " |" << endl;

        cout << Indent << BorderLine << endl;

        cout << Indent << "| " << UtilLib::GetColor(UtilLib::enColor::White)
             << left << setw(20) << " Converting From" << ": "
             << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
             << left << setw(39) << (to_string((int)Amount) + " " + FromCurrency.GetCurrencyCode() + " (" + FromCurrency.GetCurrencyName() + ")")
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan) << " |" << endl;

        if (ToCurrency.GetCurrencyCode() != "USD" && FromCurrency.GetCurrencyCode() != "USD")
        {
            ostringstream ssUSD;
            ssUSD << fixed << setprecision(2) << AmountInUSD;
            cout << Indent << "| " << UtilLib::GetColor(UtilLib::enColor::White)
                 << left << setw(20) << " USD Equivalent" << ": "
                 << UtilLib::GetColor(UtilLib::enColor::BrightCyan)
                 << left << setw(39) << ("$ " + ssUSD.str() + " USD")
                 << UtilLib::GetColor(UtilLib::enColor::BrightCyan) << " |" << endl;
        }

        ostringstream ssResult;
        ssResult << fixed << setprecision(2) << ConvertedAmount;
        string FinalStr = ssResult.str() + " " + ToCurrency.GetCurrencyCode();

        cout << Indent << "| " << UtilLib::GetColor(UtilLib::enColor::White)
             << left << setw(20) << " Total Received" << ": "
             << UtilLib::GetColor(UtilLib::enColor::BrightGreen) << UtilLib::GetColor(UtilLib::enColor::Bold)
             << left << setw(39) << FinalStr
             << UtilLib::GetColor(UtilLib::enColor::BrightCyan) << " |" << endl;

        cout << Indent << BorderLine << UtilLib::GetColor(UtilLib::enColor::Reset) << "\n\n";
    }

    void _Show()
    {
        char Answer = 'N';
        do
        {
            _ResetTheScreen();
            _DrawScreenHeader("CURRENCY CALCULATOR DASHBOARD", "Convert & Calculate Exchange Amounts Seamlessly");

            clsCurrency CurrencyFrom = _GetExistingCurrency("  [>] Enter Currency Code To Convert FROM: ");
            clsCurrency CurrencyTo = _GetExistingCurrency("  [>] Enter Currency Code To Convert TO  : ");

            cout << "\n";
            string AmountPrompt = UtilLib::GetColor(UtilLib::enColor::BrightYellow) +
                                  "  [>] Enter Amount to Exchange: " +
                                  UtilLib::GetColor(UtilLib::enColor::Reset);

            double ExchangeAmount = InputValidateLib::ReadDblPositiveNumber(AmountPrompt, "  [!] Invalid Amount! Please enter a valid positive number: ");

            cout << "\n";
            _ShowProgressBar("Calculating conversion rate and exchange values...");

            _PrintCurrencyCard(CurrencyFrom, "SOURCE CURRENCY DETAILS");

            if (CurrencyFrom.GetCurrencyCode() != CurrencyTo.GetCurrencyCode())
            {
                _PrintCurrencyCard(CurrencyTo, "TARGET CURRENCY DETAILS");
            }

            _PrintConversionResultCard(ExchangeAmount, CurrencyFrom, CurrencyTo);

            _PrintAnimatedSuccess("  [!] Currency Exchange Calculated Successfully!", 15);

            cout << "\n";
            string RepeatPrompt = UtilLib::GetColor(UtilLib::enColor::BrightYellow) +
                                  "  [?] Do you want to perform another calculation? (y/n): " +
                                  UtilLib::GetColor(UtilLib::enColor::Reset);

            Answer = InputValidateLib::getYesNoAnswer(RepeatPrompt);

        } while (toupper(Answer) == 'Y');
    }

public:
    static void ShowCurrencyCalculator()
    {
        clsCurrencyCalculatorScreen CurrencyCalculatorScreen;
        CurrencyCalculatorScreen._Show();
    }
};