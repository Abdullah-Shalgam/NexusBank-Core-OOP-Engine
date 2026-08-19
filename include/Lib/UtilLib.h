#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <string_view>
#include <vector>
#include <thread>
#include <chrono>

/**
 * @class UtilLib
 * @brief Comprehensive utility toolkit for formatting, security, RNG, and UI visual effects.
 */
class UtilLib
{
private:
    static constexpr short _ConsoleWidth = 80;

public:
    enum class enCharType
    {
        SmallCharacter = 1,
        CapitalCharacter = 2,
        Digit = 3,
        MixChars = 4,
        SpecialCharacter = 5
    };

    enum class enColor
    {
        Reset = 0,
        Bold = 1,
        Underline = 4,
        Red = 31,
        Green = 32,
        Yellow = 33,
        Blue = 34,
        Magenta = 35,
        Cyan = 36,
        White = 37,
        DarkGray = 90,
        BrightRed = 91,
        BrightGreen = 92,
        BrightYellow = 93,
        BrightBlue = 94,
        BrightMagenta = 95,
        BrightCyan = 96,
        BgRed = 41,
        BgGreen = 42,
        BgYellow = 43,
        BgBlue = 44,
        BgCyan = 46
    };

    // System Environment & Formatting
    static void Srand() noexcept;
    static std::string GetSystemDateTime();
    static std::string Tabs(int Count);
    static void PrintHeaderLine(char symbol = '=', short width = _ConsoleWidth);
    static void PrintCentered(const std::string &text, short width = _ConsoleWidth);
    static std::string Truncate(const std::string &Text, short MaxLen, const std::string &Ellipsis = "...");
    static void ResetTheScreen();

    // Visual Styling & Colors
    static std::string GetColor(enColor Color);
    static std::string ColorText(const std::string &Text, enColor Color);
    static void PrintColor(const std::string &Text, enColor Color, bool NewLine = true);
    static void TypeWriterText(const std::string &text = "Processing...", int speedInMs = 30);
    static void ShowSpinner(const std::string &Message = "Processing, please wait...", int cycles = 3);
    static void ShowProgressBar(const std::string &Message = "Loading data, please wait...");
    static void FlashWarningText(const std::string &Message = "WARNING");

    // Cryptography & Security
    static std::string EncryptText(std::string Text, short EncryptionKey);
    static std::string DecryptText(std::string Text, short EncryptionKey);
    static std::string XOREncryptDecrypt(std::string Text, const std::string &Key);

    // Random Number & Token Generation
    static int RandomNumber(int From, int To) noexcept;
    static char GetRandomCharacter(enCharType CharType) noexcept;
    static std::string GenerateWord(enCharType CharType, short Length);
    static std::string GenerateKey(enCharType CharType = enCharType::CapitalCharacter);
    static void GenerateKeys(short NumberOfKeys, enCharType CharType = enCharType::CapitalCharacter);

    // Financial Number-to-Words Conversion
    static std::string ConvertNumToText(long long Num);
    static long long GetFractionalPartAsInt(double num, int precision) noexcept;
    static std::string ConvertNumToTextWithDecimals(double Number, const std::string &MainCurrency = "Dollars", const std::string &SubCurrency = "Cents");

    // Array Utilities
    static void FillArrayWithRandomNumbers(int Array[], int ArrayLength, int From, int To) noexcept;
    static void FillArrayWithRandomWords(std::string Array[], int ArrayLength, enCharType CharType, short Length);
    static void FillArrayWithRandomKeys(std::string Array[], int ArrayLength, enCharType CharType);

    // Modern Template Swappers & Shufflers
    template <typename T>
    static void Swap(T &A, T &B) noexcept
    {
        T Temp = std::move(A);
        A = std::move(B);
        B = std::move(Temp);
    }

    template <typename T>
    static void ShuffleArray(T Array[], int ArrayLength)
    {
        for (int i = ArrayLength - 1; i > 0; --i)
        {
            Swap(Array[i], Array[RandomNumber(0, i)]);
        }
    }
};