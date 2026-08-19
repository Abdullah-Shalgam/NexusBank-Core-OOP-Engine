#pragma once
#include <iostream>
#include "clsCurrencyScreenBase.h"
#include "clsCurrency.h"
#include "InputValidateLib.h"
#include "UtilLib.h"

using namespace std;

class clsSetSystemCurrencyScreen : protected clsCurrencyScreenBase
{
private:
     clsSetSystemCurrencyScreen() : clsCurrencyScreenBase(122) {}

     void _Show()
     {
          _ResetTheScreen();
          _DrawScreenHeader("SET SYSTEM CURRENCY DASHBOARD", "Configure Global Main Currency For All Financial Displays");

          clsCurrency ActiveCurrency = clsCurrency::GetSystemCurrency();
          cout << UtilLib::GetColor(UtilLib::enColor::BrightYellow)
               << "  [i] CURRENT SYSTEM CURRENCY: "
               << UtilLib::GetColor(UtilLib::enColor::BrightGreen)
               << ActiveCurrency.GetCurrencyCode() << " (" << ActiveCurrency.GetCurrencyName() << ")\n"
               << UtilLib::GetColor(UtilLib::enColor::Reset);
          cout << UtilLib::GetColor(UtilLib::enColor::Cyan) << "  --------------------------------------------------------\n\n"
               << UtilLib::GetColor(UtilLib::enColor::Reset);

          clsCurrency NewCurrency = _GetExistingCurrency(true);

          _PrintCurrencyCard(NewCurrency, "SELECTED NEW SYSTEM CURRENCY");

          string PromptMsg = UtilLib::GetColor(UtilLib::enColor::BrightYellow) +
                             "  [?] Are you sure you want to set [" + NewCurrency.GetCurrencyCode() +
                             "] as the main System Currency? (y/n): " +
                             UtilLib::GetColor(UtilLib::enColor::Reset);

          char Answer = InputValidateLib::getYesNoAnswer(PromptMsg);

          if (tolower(Answer) == 'y')
          {
               cout << "\n";
               _ShowProgressBar("Updating global system currency settings...");

               clsCurrency::SetSystemCurrency(NewCurrency);

               cout << "\n\n";
               string SuccessMsg = "  [!] System Currency Updated Successfully To [" + NewCurrency.GetCurrencyCode() + "]!";
               _PrintAnimatedSuccess(SuccessMsg, 15);
          }
          else
          {
               cout << UtilLib::GetColor(UtilLib::enColor::Yellow)
                    << "\n  [i] Operation cancelled. System currency remains: " << ActiveCurrency.GetCurrencyCode() << "\n"
                    << UtilLib::GetColor(UtilLib::enColor::Reset);
          }
     }

public:
     static void ShowSetSystemCurrency()
     {
          clsSetSystemCurrencyScreen SetSystemCurrencyScreen;
          SetSystemCurrencyScreen._Show();
     }
};