#pragma once

#include <string>
#include <vector>

/**
 * @class MyStringLib
 * @brief High-performance string manipulation and formatting utility library.
 */
class MyStringLib
{
private:
    std::string _Value;

    enum class _enWhatToCount
    {
        CapitalLetters,
        SmallLetters,
        All
    };

    static size_t _CountLetters(const std::string &Str, _enWhatToCount WhatToCount = _enWhatToCount::All);

public:
    // Constructors
    MyStringLib() = default;
    explicit MyStringLib(std::string Value);

    // Setters & Getters
    void SetValue(std::string Value);
    const std::string& GetValue() const noexcept;

    // Static Utility Methods
    static std::string LowerAllString(std::string Text);
    static std::string CapitalizeAllString(std::string Text);
    static char InvertCharacterCase(char c) noexcept;
    static std::string InvertAllLettersCase(std::string Text);
    static std::string CapitalizeFirstLetterOfEachWord(std::string Text);
    static std::string LowerFirstLetterOfEachWord(std::string Text);

    static size_t CountLength(const std::string &Text) noexcept;
    static size_t CountAllLetters(const std::string &Text);
    static size_t CountLowerCaseLetters(const std::string &Text);
    static size_t CountUpperCaseLetters(const std::string &Text);
    static size_t CountSpecificCharacter(const std::string &Text, char CharacterToCount, bool MatchCase = true);
    static bool IsVowel(char c) noexcept;
    static size_t CountVowelLetters(const std::string &Text);
    static size_t CountWords(const std::string &Text);

    static void PrintFirstLetterOfEachWord(const std::string &Text);
    static void PrintAllVowels(const std::string &Text);
    static void PrintEachWord(const std::string &Text);

    // High-Performance $O(N)$ Parsing & Formatting
    static std::vector<std::string> SplitString(const std::string &Text, const std::string &Delimiter);
    static std::string TrimLeftSpaces(const std::string &Text);
    static std::string TrimRightSpaces(const std::string &Text);
    static std::string TrimSpaces(const std::string &Text);
    static std::string JoinStrings(const std::vector<std::string> &VectorOfStrings, const std::string &Delimiter = " ");
    static std::string JoinStrings(const std::string ArrayOfStrings[], size_t ArrayLength, const std::string &Delimiter = " ");
    static std::string ReverseWordsOrder(const std::string &Text);
    static std::string ReplaceWord(std::string Text, const std::string &TargetWord, const std::string &ReplacementWord, bool MatchCase = true);
    static std::string RemovePunctuationMarks(const std::string &Text);

    // Instance Methods (Operating on _Value)
    void LowerAllString();
    void CapitalizeAllString();
    void InvertAllLettersCase();
    void CapitalizeFirstLetterOfEachWord();
    void LowerFirstLetterOfEachWord();

    size_t CountLength() const noexcept;
    size_t CountAllLetters() const;
    size_t CountLowerCaseLetters() const;
    size_t CountUpperCaseLetters() const;
    size_t CountSpecificCharacter(char CharacterToCount, bool MatchCase = true) const;
    size_t CountVowelLetters() const;
    size_t CountWords() const;

    void PrintFirstLetterOfEachWord() const;
    void PrintAllVowels() const;
    void PrintEachWord() const;

    std::vector<std::string> SplitString(const std::string &Delimiter) const;
    std::string TrimLeftSpaces() const;
    std::string TrimRightSpaces() const;
    std::string TrimSpaces() const;
    std::string ReverseWordsOrder() const;
    std::string RemovePunctuationMarks() const;
    std::string ReplaceWord(const std::string &TargetWord, const std::string &ReplacementWord, bool MatchCase = true);
};