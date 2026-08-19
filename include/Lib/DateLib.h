#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class DateLib
{
public:
    enum enCompareDates
    {
        Before = -1,
        Equal = 0,
        After = 1
    };

private:
    short _Day = 1;
    short _Month = 1;
    short _Year = 1900;

public:
    // Constructors
    DateLib();
    DateLib(const string &DateString);
    DateLib(short Day, short Month, short Year);
    DateLib(short DateOrderInYear, short Year);

    // Setters & Getters
    void SetDay(short Day);
    short GetDay() const;

    void SetMonth(short Month);
    short GetMonth() const;

    void SetYear(short Year);
    short GetYear() const;

    // Printing & String Conversions
    static string DateToString(const DateLib &Date);
    string DateToString() const;

    static DateLib StringToDate(const string &DateString, const string &Delim = "/");
    void Print() const;

    static string FormatDate(const DateLib &Date, string DateFormat = "dd/mm/yyyy");
    string FormatDate(const string &DateFormat = "dd/mm/yyyy") const;

    // System Date & Validation
    static DateLib GetSystemDate();
    static bool IsValidDate(const DateLib &Date);
    bool IsValid() const;

    // Time Calculations
    static bool isLeapYear(short Year);
    bool isLeapYear() const;

    static short NumberOfDaysInAYear(short Year);
    short NumberOfDaysInAYear() const;

    static short NumberOfHoursInAYear(short Year);
    short NumberOfHoursInAYear() const;

    static int NumberOfMinutesInAYear(short Year);
    int NumberOfMinutesInAYear() const;

    static int NumberOfSecondsInAYear(short Year);
    int NumberOfSecondsInAYear() const;

    static short NumberOfDaysInAMonth(short Month, short Year);
    short NumberOfDaysInAMonth() const;

    static short NumberOfHoursInAMonth(short Month, short Year);
    short NumberOfHoursInAMonth() const;

    static int NumberOfMinutesInAMonth(short Month, short Year);
    int NumberOfMinutesInAMonth() const;

    static int NumberOfSecondsInAMonth(short Month, short Year);
    int NumberOfSecondsInAMonth() const;

    // Calendar & Short Names
    static short DayOfWeekOrder(short Day, short Month, short Year);
    short DayOfWeekOrder() const;

    static string DayShortName(short DayOfWeekOrder);
    string DayShortName() const;

    static string MonthShortName(short MonthNumber);
    string MonthShortName() const;

    static void PrintMonthCalendar(short Month, short Year);
    void PrintMonthCalendar() const;

    static void PrintYearCalendar(int Year);
    void PrintYearCalendar() const;

    // Days Order & Conversions
    static short DaysFromTheBeginingOfTheYear(short Day, short Month, short Year);
    short DaysFromTheBeginingOfTheYear() const;

    static DateLib GetDateFromDayOrderInYear(short DateOrderInYear, short Year);
    void AddDays(short Days);

    // Date Comparisons
    static bool IsDate1BeforeDate2(const DateLib &Date1, const DateLib &Date2);
    bool IsDateBeforeDate2(const DateLib &Date2) const;

    static bool IsDate1EqualDate2(const DateLib &Date1, const DateLib &Date2);
    bool IsDateEqualDate2(const DateLib &Date2) const;

    static bool IsDate1AfterDate2(const DateLib &Date1, const DateLib &Date2);
    bool IsDateAfterDate2(const DateLib &Date2) const;

    static enCompareDates CompareDates(const DateLib &Date1, const DateLib &Date2);
    enCompareDates CompareDates(const DateLib &Date2) const;

    // Date Boundary Checks
    static bool IsLastDayInMonth(const DateLib &Date);
    bool IsLastDayInMonth() const;

    static bool IsLastMonthInYear(short Month);
    bool IsLastMonthInYear() const;

    // Operations: Add / Swap / Differences
    static DateLib AddOneDay(DateLib Date);
    void AddOneDay();

    static void SwapDates(DateLib &Date1, DateLib &Date2);

    static int GetDifferenceInDays(DateLib Date1, DateLib Date2, bool IncludeEndDay = false);
    int GetDifferenceInDays(const DateLib &Date2, bool IncludeEndDay = false) const;

    static int CalculateMyAgeInDays(const DateLib &DateOfBirth);

    // Increase Dates
    static DateLib IncreaseDateByOneWeek(DateLib &Date);
    void IncreaseDateByOneWeek();

    static DateLib IncreaseDateByXWeeks(short Weeks, DateLib &Date);
    void IncreaseDateByXWeeks(short Weeks);

    static DateLib IncreaseDateByOneMonth(DateLib &Date);
    void IncreaseDateByOneMonth();

    static DateLib IncreaseDateByXDays(short Days, DateLib &Date);
    void IncreaseDateByXDays(short Days);

    static DateLib IncreaseDateByXMonths(short Months, DateLib &Date);
    void IncreaseDateByXMonths(short Months);

    static DateLib IncreaseDateByOneYear(DateLib &Date);
    void IncreaseDateByOneYear();

    static DateLib IncreaseDateByXYears(short Years, DateLib &Date);
    void IncreaseDateByXYears(short Years);

    static DateLib IncreaseDateByOneDecade(DateLib &Date);
    void IncreaseDateByOneDecade();

    static DateLib IncreaseDateByXDecades(short Decades, DateLib &Date);
    void IncreaseDateByXDecades(short Decades);

    static DateLib IncreaseDateByOneCentury(DateLib &Date);
    void IncreaseDateByOneCentury();

    static DateLib IncreaseDateByOneMillennium(DateLib &Date);
    void IncreaseDateByOneMillennium();

    // Decrease Dates
    static DateLib DecreaseDateByOneDay(DateLib Date);
    void DecreaseDateByOneDay();

    static DateLib DecreaseDateByXDays(short Days, DateLib &Date);
    void DecreaseDateByXDays(short Days);

    static DateLib DecreaseDateByOneWeek(DateLib &Date);
    void DecreaseDateByOneWeek();

    static DateLib DecreaseDateByXWeeks(short Weeks, DateLib &Date);
    void DecreaseDateByXWeeks(short Weeks);

    static DateLib DecreaseDateByOneMonth(DateLib &Date);
    void DecreaseDateByOneMonth();

    static DateLib DecreaseDateByXMonths(short Months, DateLib &Date);
    void DecreaseDateByXMonths(short Months);

    static DateLib DecreaseDateByOneYear(DateLib &Date);
    void DecreaseDateByOneYear();

    static DateLib DecreaseDateByXYears(short Years, DateLib &Date);
    void DecreaseDateByXYears(short Years);

    static DateLib DecreaseDateByOneDecade(DateLib &Date);
    void DecreaseDateByOneDecade();

    static DateLib DecreaseDateByXDecades(short Decades, DateLib &Date);
    void DecreaseDateByXDecades(short Decades);

    static DateLib DecreaseDateByOneCentury(DateLib &Date);
    void DecreaseDateByOneCentury();

    static DateLib DecreaseDateByOneMillennium(DateLib &Date);
    void DecreaseDateByOneMillennium();

    // Business Days & Vacations
    static bool IsEndOfWeek(const DateLib &Date);
    bool IsEndOfWeek() const;

    static bool IsWeekEnd(const DateLib &Date);
    bool IsWeekEnd() const;

    static bool IsBusinessDay(const DateLib &Date);
    bool IsBusinessDay() const;

    static short DaysUntilTheEndOfWeek(const DateLib &Date);
    short DaysUntilTheEndOfWeek() const;

    static short DaysUntilTheEndOfMonth(const DateLib &Date1);
    short DaysUntilTheEndOfMonth() const;

    static short DaysUntilTheEndOfYear(const DateLib &Date1);
    short DaysUntilTheEndOfYear() const;

    static short CalculateBusinessDays(DateLib DateFrom, DateLib DateTo);
    static short CalculateVacationDays(const DateLib &DateFrom, const DateLib &DateTo);
    static DateLib CalculateVacationReturnDate(DateLib DateFrom, short VacationDays);
};