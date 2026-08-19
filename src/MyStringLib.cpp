#include "MyStringLib.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <utility>

// -----------------------------------------------------------------------------
// Private Helper Methods
// -----------------------------------------------------------------------------
size_t MyStringLib::_CountLetters(const std::string &Str, _enWhatToCount WhatToCount)
{
    size_t Counter = 0;
    for (const unsigned char Letter : Str)
    {
        if (WhatToCount == _enWhatToCount::All && std::isalpha(Letter))
            Counter++;
        else if (WhatToCount == _enWhatToCount::CapitalLetters && std::isupper(Letter))
            Counter++;
        else if (WhatToCount == _enWhatToCount::SmallLetters && std::islower(Letter))
            Counter++;
    }
    return Counter;
}

// -----------------------------------------------------------------------------
// Constructors, Setters & Getters
// -----------------------------------------------------------------------------
MyStringLib::MyStringLib(std::string Value) : _Value(std::move(Value)) {}

void MyStringLib::SetValue(std::string Value) { _Value = std::move(Value); }

const std::string& MyStringLib::GetValue() const noexcept { return _Value; }

// -----------------------------------------------------------------------------
// Static Utility Methods
// -----------------------------------------------------------------------------
std::string MyStringLib::LowerAllString(std::string Text)
{
    for (char &Letter : Text)
        Letter = static_cast<char>(std::tolower(static_cast<unsigned char>(Letter)));
    return Text;
}

std::string MyStringLib::CapitalizeAllString(std::string Text)
{
    for (char &Letter : Text)
        Letter = static_cast<char>(std::toupper(static_cast<unsigned char>(Letter)));
    return Text;
}

char MyStringLib::InvertCharacterCase(char c) noexcept
{
    const auto uc = static_cast<unsigned char>(c);
    return std::islower(uc) ? static_cast<char>(std::toupper(uc)) : static_cast<char>(std::tolower(uc));
}

std::string MyStringLib::InvertAllLettersCase(std::string Text)
{
    for (char &Letter : Text)
        Letter = InvertCharacterCase(Letter);
    return Text;
}

std::string MyStringLib::CapitalizeFirstLetterOfEachWord(std::string Text)
{
    bool IsFirstLetter = true;
    for (char &c : Text)
    {
        if (c != ' ' && IsFirstLetter)
        {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            IsFirstLetter = false;
        }
        else if (c == ' ')
        {
            IsFirstLetter = true;
        }
    }
    return Text;
}

std::string MyStringLib::LowerFirstLetterOfEachWord(std::string Text)
{
    bool IsFirstLetter = true;
    for (char &c : Text)
    {
        if (c != ' ' && IsFirstLetter)
        {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            IsFirstLetter = false;
        }
        else if (c == ' ')
        {
            IsFirstLetter = true;
        }
    }
    return Text;
}

size_t MyStringLib::CountLength(const std::string &Text) noexcept
{
    return Text.length();
}

size_t MyStringLib::CountAllLetters(const std::string &Text)
{
    return _CountLetters(Text, _enWhatToCount::All);
}

size_t MyStringLib::CountLowerCaseLetters(const std::string &Text)
{
    return _CountLetters(Text, _enWhatToCount::SmallLetters);
}

size_t MyStringLib::CountUpperCaseLetters(const std::string &Text)
{
    return _CountLetters(Text, _enWhatToCount::CapitalLetters);
}

size_t MyStringLib::CountSpecificCharacter(const std::string &Text, char CharacterToCount, bool MatchCase)
{
    size_t Counter = 0;
    const auto target = static_cast<unsigned char>(CharacterToCount);

    for (const unsigned char Letter : Text)
    {
        if (MatchCase)
        {
            if (Letter == target)
                Counter++;
        }
        else
        {
            if (std::tolower(Letter) == std::tolower(target))
                Counter++;
        }
    }
    return Counter;
}

bool MyStringLib::IsVowel(char c) noexcept
{
    const char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return (lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u');
}

size_t MyStringLib::CountVowelLetters(const std::string &Text)
{
    size_t Counter = 0;
    for (const char Letter : Text)
    {
        if (IsVowel(Letter))
            Counter++;
    }
    return Counter;
}

// Optimized O(N) Word Counting (Zero Erase Memory Shifts)
size_t MyStringLib::CountWords(const std::string &Text)
{
    size_t Count = 0;
    bool InWord = false;

    for (const char c : Text)
    {
        if (c == ' ' || c == '\t' || c == '\n')
        {
            InWord = false;
        }
        else if (!InWord)
        {
            InWord = true;
            Count++;
        }
    }
    return Count;
}

void MyStringLib::PrintFirstLetterOfEachWord(const std::string &Text)
{
    bool IsFirstLetter = true;
    std::cout << "\nFirst Letter of Each Word:\n";
    for (const char c : Text)
    {
        if (c != ' ' && IsFirstLetter)
        {
            std::cout << c << "\n";
            IsFirstLetter = false;
        }
        else if (c == ' ')
        {
            IsFirstLetter = true;
        }
    }
}

void MyStringLib::PrintAllVowels(const std::string &Text)
{
    std::cout << "Vowels in string: ";
    for (const char Letter : Text)
    {
        if (IsVowel(Letter))
            std::cout << Letter << "   ";
    }
    std::cout << "\n";
}

void MyStringLib::PrintEachWord(const std::string &Text)
{
    const std::vector<std::string> words = SplitString(Text, " ");
    for (const auto &w : words)
    {
        if (!w.empty())
            std::cout << w << "\n";
    }
}

// Optimized O(N) String Splitting via Substring Offsets
std::vector<std::string> MyStringLib::SplitString(const std::string &Text, const std::string &Delimiter)
{
    std::vector<std::string> Tokens;
    if (Text.empty())
        return Tokens;

    if (Delimiter.empty())
    {
        Tokens.push_back(Text);
        return Tokens;
    }

    size_t Start = 0;
    size_t End = Text.find(Delimiter);

    while (End != std::string::npos)
    {
        Tokens.push_back(Text.substr(Start, End - Start));
        Start = End + Delimiter.length();
        End = Text.find(Delimiter, Start);
    }
    Tokens.push_back(Text.substr(Start));

    return Tokens;
}

std::string MyStringLib::TrimLeftSpaces(const std::string &Text)
{
    const size_t Start = Text.find_first_not_of(" \t\n\r");
    return (Start == std::string::npos) ? "" : Text.substr(Start);
}

std::string MyStringLib::TrimRightSpaces(const std::string &Text)
{
    const size_t End = Text.find_last_not_of(" \t\n\r");
    return (End == std::string::npos) ? "" : Text.substr(0, End + 1);
}

std::string MyStringLib::TrimSpaces(const std::string &Text)
{
    return TrimLeftSpaces(TrimRightSpaces(Text));
}

std::string MyStringLib::JoinStrings(const std::vector<std::string> &VectorOfStrings, const std::string &Delimiter)
{
    if (VectorOfStrings.empty())
        return "";

    std::string Result;
    for (size_t i = 0; i < VectorOfStrings.size(); ++i)
    {
        Result += VectorOfStrings[i];
        if (i + 1 < VectorOfStrings.size())
            Result += Delimiter;
    }
    return Result;
}

std::string MyStringLib::JoinStrings(const std::string ArrayOfStrings[], size_t ArrayLength, const std::string &Delimiter)
{
    if (ArrayLength == 0)
        return "";

    std::string Result;
    for (size_t i = 0; i < ArrayLength; ++i)
    {
        Result += ArrayOfStrings[i];
        if (i + 1 < ArrayLength)
            Result += Delimiter;
    }
    return Result;
}

std::string MyStringLib::ReverseWordsOrder(const std::string &Text)
{
    std::vector<std::string> Tokens = SplitString(Text, " ");
    std::reverse(Tokens.begin(), Tokens.end());
    return JoinStrings(Tokens, " ");
}

std::string MyStringLib::ReplaceWord(std::string Text, const std::string &TargetWord, const std::string &ReplacementWord, bool MatchCase)
{
    if (TargetWord.empty())
        return Text;

    size_t Pos = 0;
    if (MatchCase)
    {
        while ((Pos = Text.find(TargetWord, Pos)) != std::string::npos)
        {
            Text.replace(Pos, TargetWord.length(), ReplacementWord);
            Pos += ReplacementWord.length();
        }
    }
    else
    {
        std::string TextLower = LowerAllString(Text);
        std::string TargetLower = LowerAllString(TargetWord);

        while ((Pos = TextLower.find(TargetLower, Pos)) != std::string::npos)
        {
            Text.replace(Pos, TargetWord.length(), ReplacementWord);
            TextLower.replace(Pos, TargetLower.length(), LowerAllString(ReplacementWord));
            Pos += ReplacementWord.length();
        }
    }
    return Text;
}

std::string MyStringLib::RemovePunctuationMarks(const std::string &Text)
{
    std::string Result;
    Result.reserve(Text.length());
    for (const unsigned char Character : Text)
    {
        if (!std::ispunct(Character))
            Result += static_cast<char>(Character);
    }
    return Result;
}

// -----------------------------------------------------------------------------
// Instance Methods Forwarding
// -----------------------------------------------------------------------------
void MyStringLib::LowerAllString() { _Value = LowerAllString(_Value); }
void MyStringLib::CapitalizeAllString() { _Value = CapitalizeAllString(_Value); }
void MyStringLib::InvertAllLettersCase() { _Value = InvertAllLettersCase(_Value); }
void MyStringLib::CapitalizeFirstLetterOfEachWord() { _Value = CapitalizeFirstLetterOfEachWord(_Value); }
void MyStringLib::LowerFirstLetterOfEachWord() { _Value = LowerFirstLetterOfEachWord(_Value); }

size_t MyStringLib::CountLength() const noexcept { return CountLength(_Value); }
size_t MyStringLib::CountAllLetters() const { return CountAllLetters(_Value); }
size_t MyStringLib::CountLowerCaseLetters() const { return CountLowerCaseLetters(_Value); }
size_t MyStringLib::CountUpperCaseLetters() const { return CountUpperCaseLetters(_Value); }
size_t MyStringLib::CountSpecificCharacter(char CharacterToCount, bool MatchCase) const { return CountSpecificCharacter(_Value, CharacterToCount, MatchCase); }
size_t MyStringLib::CountVowelLetters() const { return CountVowelLetters(_Value); }
size_t MyStringLib::CountWords() const { return CountWords(_Value); }

void MyStringLib::PrintFirstLetterOfEachWord() const { PrintFirstLetterOfEachWord(_Value); }
void MyStringLib::PrintAllVowels() const { PrintAllVowels(_Value); }
void MyStringLib::PrintEachWord() const { PrintEachWord(_Value); }

std::vector<std::string> MyStringLib::SplitString(const std::string &Delimiter) const { return SplitString(_Value, Delimiter); }
std::string MyStringLib::TrimLeftSpaces() const { return TrimLeftSpaces(_Value); }
std::string MyStringLib::TrimRightSpaces() const { return TrimRightSpaces(_Value); }
std::string MyStringLib::TrimSpaces() const { return TrimSpaces(_Value); }
std::string MyStringLib::ReverseWordsOrder() const { return ReverseWordsOrder(_Value); }
std::string MyStringLib::RemovePunctuationMarks() const { return RemovePunctuationMarks(_Value); }

std::string MyStringLib::ReplaceWord(const std::string &TargetWord, const std::string &ReplacementWord, bool MatchCase)
{
    return ReplaceWord(_Value, TargetWord, ReplacementWord, MatchCase);
}