#pragma once

#include <string>
#include <iostream>
#include <limits>
#include "DateLib.h"

/**
 * @class InputValidateLib
 * @brief Robust, type-safe console stream input validation and masked credential reader.
 */
class InputValidateLib
{
private:
    static std::string _ReadPinMasked(const std::string &Msg = "Enter PIN Code: ");

public:
    // -------------------------------------------------------------------------
    // Generic Template Validations
    // -------------------------------------------------------------------------
    template <typename T>
    static bool IsNumberBetween(T Num, T From, T To) noexcept
    {
        return (Num >= From && Num <= To);
    }

    template <typename T>
    static T ReadNumber(const std::string &Msg = "Please enter a number: ",
                        const std::string &ErrMsg = "Invalid Number, Enter again: ")
    {
        T Number{};
        std::cout << Msg;
        while (!(std::cin >> Number) || std::cin.peek() != '\n')
        {
            if (std::cin.eof())
                return Number;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << ErrMsg << "\n" << Msg;
        }
        return Number;
    }

    template <typename T>
    static T ReadPositiveNumber(const std::string &Msg = "Please enter a positive number: ",
                                const std::string &ErrMsg = "Invalid Positive Number, Enter again: ")
    {
        T Number{};
        do
        {
            Number = ReadNumber<T>(Msg, ErrMsg);
            if (Number < 0)
            {
                std::cout << ErrMsg << "\n";
            }
        } while (Number < 0);

        return Number;
    }

    template <typename T>
    static T ReadNumberInRange(T From, T To,
                               const std::string &Msg = "Please enter a number in range: ",
                               const std::string &ErrMsg = "Number is not within range, Enter again: ")
    {
        T Number{};
        do
        {
            Number = ReadNumber<T>(Msg, ErrMsg);
            if (!IsNumberBetween(Number, From, To))
            {
                std::cout << ErrMsg << "\n";
            }
        } while (!IsNumberBetween(Number, From, To));

        return Number;
    }

    // -------------------------------------------------------------------------
    // Integer & Float Wrappers (Preserved for compatibility)
    // -------------------------------------------------------------------------
    static int ReadIntNumber(const std::string &Msg = "Please enter a number: ", const std::string &ErrMsg = "Invalid Number, Enter again: ");
    static int ReadIntPositiveNumber(const std::string &Msg = "Please enter a positive number: ", const std::string &ErrMsg = "Invalid Positive Number, Enter again: ");
    static int ReadIntNumberInRange(int From, int To, const std::string &Msg = "Please enter a number in range: ", const std::string &ErrMsg = "Number is not within range, Enter again: ");

    static short ReadShortNumber(const std::string &Msg = "Please enter a short number: ", const std::string &ErrMsg = "Invalid Number, Enter again: ");
    static short ReadShortPositiveNumber(const std::string &Msg = "Please enter a positive short number: ", const std::string &ErrMsg = "Invalid Positive Number, Enter again: ");
    static short ReadShortNumberInRange(short From, short To, const std::string &Msg = "Please enter a short number in range: ", const std::string &ErrMsg = "Number is not within range, Enter again: ");

    static long long ReadLongLongPositiveNumber(const std::string &Msg = "Please enter a positive number: ", const std::string &ErrMsg = "Invalid Positive Number, Enter again: ");

    static double ReadDblNumber(const std::string &Msg = "Please enter a double number: ", const std::string &ErrMsg = "Invalid Number, Enter again: ");
    static double ReadDblPositiveNumber(const std::string &Msg = "Please enter a positive double number: ", const std::string &ErrMsg = "Invalid Positive Number, Enter again: ");
    static double ReadDblNumberInRange(double From, double To, const std::string &Msg = "Please enter a double number in range: ", const std::string &ErrMsg = "Number is not within range, Enter again: ");

    // -------------------------------------------------------------------------
    // Text & Character Validations
    // -------------------------------------------------------------------------
    static std::string ReadText(const std::string &Msg = "Please enter text: ", const std::string &ErrMsg = "Invalid Input, Enter again: ");
    static std::string ReadLimitedText(short MaxLength, const std::string &Msg = "Please enter text: ", const std::string &ErrMsg = "Invalid Input, Enter again: ");
    static char ReadCharacter(const std::string &Msg = "Please enter a character: ", const std::string &ErrMsg = "Invalid Character, Enter again: ");
    static char getYesNoAnswer(const std::string &Msg = "Please enter (Y/N): ", const std::string &ErrMsg = "Invalid Answer! Please enter (Y/N): ");

    // -------------------------------------------------------------------------
    // Date Utilities
    // -------------------------------------------------------------------------
    static bool IsDateBetween(const DateLib &Date, DateLib From, DateLib To) noexcept;
    static bool IsValidDate(const DateLib &Date) noexcept;
    static bool IsValideDate(const DateLib &Date) noexcept; // Legacy typo alias

    // -------------------------------------------------------------------------
    // Masked Input & PIN Verification
    // -------------------------------------------------------------------------
    static std::string ReadPassword(const std::string &Msg = "Enter Password: ", short MinLength = 4, const std::string &ErrMsg = "\nError: Invalid Password!\n\n");
    static std::string GetValidPIN(const std::string &Msg = "Enter PinCode (4 digits): ", const std::string &ErrMsg = "\nError: PIN must be exactly 4 digits.\n\n");
};