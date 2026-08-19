#include "UtilLib.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <ctime>

// -----------------------------------------------------------------------------
// Safe Local Time Helper
// -----------------------------------------------------------------------------
static std::tm GetSafeLocalTime(const std::time_t &timeVal)
{
    std::tm timeinfo{};
#if defined(_WIN32) || defined(_MSC_VER)
    localtime_s(&timeinfo, &timeVal);
#else
    localtime_r(&timeVal, &timeinfo);
#endif
    return timeinfo;
}

void UtilLib::Srand() noexcept
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

std::string UtilLib::GetSystemDateTime()
{
    const std::time_t now = std::time(nullptr);
    const std::tm ltm = GetSafeLocalTime(now);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d | %H:%M:%S", &ltm);
    return std::string(buffer);
}

std::string UtilLib::Tabs(int Count)
{
    return (Count > 0) ? std::string(static_cast<size_t>(Count), '\t') : "";
}

void UtilLib::PrintHeaderLine(char symbol, short width)
{
    if (width > 0)
        std::cout << std::string(static_cast<size_t>(width), symbol) << "\n";
}

void UtilLib::PrintCentered(const std::string &text, short width)
{
    if (static_cast<short>(text.length()) >= width)
    {
        std::cout << text << "\n";
        return;
    }
    const short padding = static_cast<short>((width - text.length()) / 2);
    std::cout << std::string(static_cast<size_t>(padding), ' ') << text << "\n";
}

std::string UtilLib::Truncate(const std::string &Text, short MaxLen, const std::string &Ellipsis)
{
    if (MaxLen <= 0)
        return "";

    if (static_cast<short>(Text.length()) > MaxLen)
    {
        if (MaxLen <= static_cast<short>(Ellipsis.length()))
            return Text.substr(0, static_cast<size_t>(MaxLen));

        return Text.substr(0, static_cast<size_t>(MaxLen - Ellipsis.length())) + Ellipsis;
    }
    return Text;
}

void UtilLib::ResetTheScreen()
{
    // Fast ANSI Terminal Clear + Safe system cls fallback
    std::cout << "\033[2J\033[1;1H" << GetColor(enColor::Reset) << std::flush;
#if defined(_WIN32)
    system("cls");
#endif
}

// -----------------------------------------------------------------------------
// Cryptography
// -----------------------------------------------------------------------------
std::string UtilLib::EncryptText(std::string Text, short EncryptionKey)
{
    for (char &c : Text)
    {
        c = static_cast<char>(c + EncryptionKey);
    }
    return Text;
}

std::string UtilLib::DecryptText(std::string Text, short EncryptionKey)
{
    for (char &c : Text)
    {
        c = static_cast<char>(c - EncryptionKey);
    }
    return Text;
}

std::string UtilLib::XOREncryptDecrypt(std::string Text, const std::string &Key)
{
    if (Key.empty())
        return Text;

    for (size_t i = 0; i < Text.length(); ++i)
    {
        Text[i] = Text[i] ^ Key[i % Key.length()];
    }
    return Text;
}

// -----------------------------------------------------------------------------
// Random Utilities
// -----------------------------------------------------------------------------
int UtilLib::RandomNumber(int From, int To) noexcept
{
    if (From > To)
        std::swap(From, To);
    return std::rand() % (To - From + 1) + From;
}

char UtilLib::GetRandomCharacter(enCharType CharType) noexcept
{
    if (CharType == enCharType::MixChars)
    {
        CharType = static_cast<enCharType>(RandomNumber(1, 3));
    }

    switch (CharType)
    {
    case enCharType::SmallCharacter:
        return static_cast<char>(RandomNumber(97, 122));
    case enCharType::CapitalCharacter:
        return static_cast<char>(RandomNumber(65, 90));
    case enCharType::SpecialCharacter:
    {
        const int range = RandomNumber(1, 4);
        if (range == 1) return static_cast<char>(RandomNumber(33, 47));
        if (range == 2) return static_cast<char>(RandomNumber(58, 64));
        if (range == 3) return static_cast<char>(RandomNumber(91, 96));
        return static_cast<char>(RandomNumber(123, 126));
    }
    case enCharType::Digit:
        return static_cast<char>(RandomNumber(48, 57));
    default:
        return '\0';
    }
}

std::string UtilLib::GenerateWord(enCharType CharType, short Length)
{
    std::string Word;
    Word.reserve(Length);
    for (short i = 0; i < Length; ++i)
    {
        Word += GetRandomCharacter(CharType);
    }
    return Word;
}

std::string UtilLib::GenerateKey(enCharType CharType)
{
    return GenerateWord(CharType, 4) + "-" +
           GenerateWord(CharType, 4) + "-" +
           GenerateWord(CharType, 4) + "-" +
           GenerateWord(CharType, 4);
}

void UtilLib::GenerateKeys(short NumberOfKeys, enCharType CharType)
{
    for (int i = 1; i <= NumberOfKeys; ++i)
    {
        std::cout << "Key [" << i << "] \t: " << GenerateKey(CharType) << "\n";
    }
}

// -----------------------------------------------------------------------------
// Number To Words
// -----------------------------------------------------------------------------
std::string UtilLib::ConvertNumToText(long long Num)
{
    if (Num == 0)
        return "";

    static const std::string NumbersFrom1To19[] = {
        "", "One", "Two", "Three", "Four",
        "Five", "Six", "Seven", "Eight", "Nine",
        "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen",
        "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};

    static const std::string Tens[] = {
        "", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};

    if (Num >= 1 && Num <= 19)
    {
        return NumbersFrom1To19[Num];
    }
    if (Num >= 20 && Num <= 99)
    {
        const std::string remainder = ConvertNumToText(Num % 10);
        return Tens[Num / 10] + (remainder.empty() ? "" : " " + remainder);
    }
    if (Num >= 100 && Num <= 999)
    {
        const std::string remainder = ConvertNumToText(Num % 100);
        return ConvertNumToText(Num / 100) + " Hundred" + (remainder.empty() ? "" : " " + remainder);
    }
    if (Num >= 1000 && Num <= 999999)
    {
        const std::string remainder = ConvertNumToText(Num % 1000);
        return ConvertNumToText(Num / 1000) + " Thousand" + (remainder.empty() ? "" : " " + remainder);
    }
    if (Num >= 1000000 && Num <= 999999999)
    {
        const std::string remainder = ConvertNumToText(Num % 1000000);
        return ConvertNumToText(Num / 1000000) + " Million" + (remainder.empty() ? "" : " " + remainder);
    }
    if (Num >= 1000000000 && Num <= 999999999999LL)
    {
        const std::string remainder = ConvertNumToText(Num % 1000000000);
        return ConvertNumToText(Num / 1000000000) + " Billion" + (remainder.empty() ? "" : " " + remainder);
    }
    if (Num >= 1000000000000LL && Num <= 999999999999999LL)
    {
        const std::string remainder = ConvertNumToText(Num % 1000000000000LL);
        return ConvertNumToText(Num / 1000000000000LL) + " Trillion" + (remainder.empty() ? "" : " " + remainder);
    }

    return "";
}

long long UtilLib::GetFractionalPartAsInt(double num, int precision) noexcept
{
    if (num < 0)
        num = -num;

    double fractional_part = num - static_cast<long long>(num);
    for (int i = 0; i < precision; ++i)
    {
        fractional_part *= 10;
    }
    return static_cast<long long>(fractional_part + 0.5);
}

std::string UtilLib::ConvertNumToTextWithDecimals(double Number, const std::string &MainCurrency, const std::string &SubCurrency)
{
    const auto IntegerPart = static_cast<long long>(Number);
    const int FractionPart = static_cast<int>(GetFractionalPartAsInt(Number, 2));

    std::string Text = ConvertNumToText(IntegerPart) + " " + MainCurrency;
    if (FractionPart > 0)
    {
        Text += " and " + ConvertNumToText(FractionPart) + " " + SubCurrency;
    }
    return Text;
}

void UtilLib::FillArrayWithRandomNumbers(int Array[], int ArrayLength, int From, int To) noexcept
{
    for (int i = 0; i < ArrayLength; ++i)
    {
        Array[i] = RandomNumber(From, To);
    }
}

void UtilLib::FillArrayWithRandomWords(std::string Array[], int ArrayLength, enCharType CharType, short Length)
{
    for (int i = 0; i < ArrayLength; ++i)
    {
        Array[i] = GenerateWord(CharType, Length);
    }
}

void UtilLib::FillArrayWithRandomKeys(std::string Array[], int ArrayLength, enCharType CharType)
{
    for (int i = 0; i < ArrayLength; ++i)
    {
        Array[i] = GenerateKey(CharType);
    }
}

// -----------------------------------------------------------------------------
// Visual & Animation Effects
// -----------------------------------------------------------------------------
std::string UtilLib::GetColor(enColor Color)
{
    return "\033[" + std::to_string(static_cast<int>(Color)) + "m";
}

std::string UtilLib::ColorText(const std::string &Text, enColor Color)
{
    return GetColor(Color) + Text + GetColor(enColor::Reset);
}

void UtilLib::PrintColor(const std::string &Text, enColor Color, bool NewLine)
{
    std::cout << ColorText(Text, Color);
    if (NewLine)
        std::cout << "\n";
}

void UtilLib::TypeWriterText(const std::string &text, int speedInMs)
{
    for (const char c : text)
    {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(speedInMs));
    }
}

void UtilLib::ShowSpinner(const std::string &Message, int cycles)
{
    static constexpr char spinner[] = {'|', '/', '-', '\\'};
    std::cout << "\n\t" << Message << " ";

    for (int i = 0; i < cycles * 4; ++i)
    {
        std::cout << spinner[i % 4] << "\b" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Done!\n";
}

void UtilLib::ShowProgressBar(const std::string &Message)
{
    std::cout << "\n\n\t" << Message << "\n\t[";
    for (int i = 0; i < 30; ++i)
    {
        std::cout << "=" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    std::cout << "] 100% Completed!\n\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

void UtilLib::FlashWarningText(const std::string &Message)
{
    std::cout << GetColor(enColor::BgRed)
              << GetColor(enColor::White)
              << " " << Message << " "
              << GetColor(enColor::Reset) << std::flush;

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}