#include "InputValidateLib.h"
#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <conio.h>

// Named ASCII constants for clean readable keystroke handling
namespace
{
    constexpr char KEY_ENTER = 13;
    constexpr char KEY_BACKSPACE = 8;
    constexpr char KEY_ESCAPE_PREFIX_1 = 0;
    constexpr char KEY_ESCAPE_PREFIX_2 = -32;
}

// -----------------------------------------------------------------------------
// Integer Forwarding
// -----------------------------------------------------------------------------
int InputValidateLib::ReadIntNumber(const std::string &Msg, const std::string &ErrMsg)
{
    return ReadNumber<int>(Msg, ErrMsg);
}

int InputValidateLib::ReadIntPositiveNumber(const std::string &Msg, const std::string &ErrMsg)
{
    return ReadPositiveNumber<int>(Msg, ErrMsg);
}

int InputValidateLib::ReadIntNumberInRange(int From, int To, const std::string &Msg, const std::string &ErrMsg)
{
    return ReadNumberInRange<int>(From, To, Msg, ErrMsg);
}

// -----------------------------------------------------------------------------
// Short Forwarding
// -----------------------------------------------------------------------------
short InputValidateLib::ReadShortNumber(const std::string &Msg, const std::string &ErrMsg)
{
    return ReadNumber<short>(Msg, ErrMsg);
}

short InputValidateLib::ReadShortPositiveNumber(const std::string &Msg, const std::string &ErrMsg)
{
    return ReadPositiveNumber<short>(Msg, ErrMsg);
}

short InputValidateLib::ReadShortNumberInRange(short From, short To, const std::string &Msg, const std::string &ErrMsg)
{
    return ReadNumberInRange<short>(From, To, Msg, ErrMsg);
}

// -----------------------------------------------------------------------------
// Long Long Forwarding
// -----------------------------------------------------------------------------
long long InputValidateLib::ReadLongLongPositiveNumber(const std::string &Msg, const std::string &ErrMsg)
{
    return ReadPositiveNumber<long long>(Msg, ErrMsg);
}

// -----------------------------------------------------------------------------
// Double Forwarding
// -----------------------------------------------------------------------------
double InputValidateLib::ReadDblNumber(const std::string &Msg, const std::string &ErrMsg)
{
    return ReadNumber<double>(Msg, ErrMsg);
}

double InputValidateLib::ReadDblPositiveNumber(const std::string &Msg, const std::string &ErrMsg)
{
    return ReadPositiveNumber<double>(Msg, ErrMsg);
}

double InputValidateLib::ReadDblNumberInRange(double From, double To, const std::string &Msg, const std::string &ErrMsg)
{
    return ReadNumberInRange<double>(From, To, Msg, ErrMsg);
}

// -----------------------------------------------------------------------------
// Text & Character Validations
// -----------------------------------------------------------------------------
std::string InputValidateLib::ReadText(const std::string &Msg, const std::string &ErrMsg)
{
    std::string textContent;
    std::cout << Msg;
    while (true)
    {
        if (std::getline(std::cin >> std::ws, textContent))
        {
            return textContent;
        }
        if (std::cin.eof())
            return "";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << ErrMsg << "\n" << Msg;
    }
}

std::string InputValidateLib::ReadLimitedText(short MaxLength, const std::string &Msg, const std::string &ErrMsg)
{
    while (true)
    {
        std::string textContent = ReadText(Msg, ErrMsg);
        if (textContent.length() <= static_cast<size_t>(MaxLength))
        {
            return textContent;
        }
        std::cout << "  [!] Input exceeds maximum allowed length of " << MaxLength << " characters! Please try again.\n\n";
    }
}

char InputValidateLib::ReadCharacter(const std::string &Msg, const std::string &ErrMsg)
{
    std::string inputLine;
    std::cout << Msg;
    while (true)
    {
        if (!std::getline(std::cin >> std::ws, inputLine))
        {
            if (std::cin.eof())
                return '\0';

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << ErrMsg << "\n" << Msg;
            continue;
        }
        if (inputLine.length() == 1)
        {
            return inputLine[0];
        }
        std::cout << ErrMsg << "\n" << Msg;
    }
}

char InputValidateLib::getYesNoAnswer(const std::string &Msg, const std::string &ErrMsg)
{
    while (true)
    {
        const char ch = ReadCharacter(Msg, ErrMsg);
        const char targetChar = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        if (targetChar == 'y' || targetChar == 'n')
        {
            return targetChar;
        }
        std::cout << ErrMsg << "\n";
    }
}

// -----------------------------------------------------------------------------
// Date Utilities
// -----------------------------------------------------------------------------
bool InputValidateLib::IsDateBetween(const DateLib &Date, DateLib From, DateLib To) noexcept
{
    if (DateLib::IsDate1BeforeDate2(To, From))
    {
        DateLib::SwapDates(From, To);
    }
    return !(DateLib::IsDate1BeforeDate2(Date, From) || DateLib::IsDate1AfterDate2(Date, To));
}

bool InputValidateLib::IsValidDate(const DateLib &Date) noexcept
{
    return DateLib::IsValidDate(Date);
}

bool InputValidateLib::IsValideDate(const DateLib &Date) noexcept
{
    return IsValidDate(Date);
}

// -----------------------------------------------------------------------------
// Password & PIN Masked Readers
// -----------------------------------------------------------------------------
std::string InputValidateLib::ReadPassword(const std::string &Msg, short MinLength, const std::string &/*ErrMsg*/)
{
    std::string password;
    while (true)
    {
        password.clear();
        std::cout << Msg;

        while (true)
        {
            const int ch = _getch();

            if (ch == KEY_ENTER)
            {
                std::cout << "\n";
                break;
            }
            if (ch == KEY_BACKSPACE)
            {
                if (!password.empty())
                {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            }
            else if (ch == KEY_ESCAPE_PREFIX_1 || ch == KEY_ESCAPE_PREFIX_2)
            {
                _getch(); // Safely consume special key secondary byte
            }
            else if (std::isprint(static_cast<unsigned char>(ch)))
            {
                password += static_cast<char>(ch);
                std::cout << '*';
            }
        }

        if (password.length() < static_cast<size_t>(MinLength))
        {
            std::cout << "  [!] Security Warning: Password must be at least " << MinLength << " characters long!\n\n";
            continue;
        }
        return password;
    }
}

std::string InputValidateLib::_ReadPinMasked(const std::string &Msg)
{
    std::string pin;
    std::cout << Msg;

    while (true)
    {
        const int ch = _getch();

        if (ch == KEY_ENTER)
        {
            if (!pin.empty())
                break;
        }
        else if (ch == KEY_BACKSPACE)
        {
            if (!pin.empty())
            {
                pin.pop_back();
                std::cout << "\b \b";
            }
        }
        else if (std::isdigit(static_cast<unsigned char>(ch)) && pin.length() < 4)
        {
            pin += static_cast<char>(ch);
            std::cout << '*';
        }
    }

    std::cout << "\n";
    return pin;
}

std::string InputValidateLib::GetValidPIN(const std::string &Msg, const std::string &ErrMsg)
{
    while (true)
    {
        std::string pin = _ReadPinMasked(Msg);
        if (pin.length() != 4)
        {
            std::cout << ErrMsg;
            continue;
        }
        return pin;
    }
}