#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

#include "DateLib.h"
#include "MyStringLib.h"
#include <ctime>
#include <iomanip>
#include <cstdio>

// 1. Constructors
DateLib::DateLib()
{
    time_t t = time(0);
    tm *now = localtime(&t);
    _Day = now->tm_mday;
    _Month = now->tm_mon + 1;
    _Year = now->tm_year + 1900;
}

DateLib::DateLib(const string &DateString)
{
    *this = StringToDate(DateString);
}

DateLib::DateLib(short Day, short Month, short Year)
{
    _Day = Day;
    _Month = Month;
    _Year = Year;
}

DateLib::DateLib(short DateOrderInYear, short Year)
{
    *this = GetDateFromDayOrderInYear(DateOrderInYear, Year);
}

// 2. Setters & Getters
void DateLib::SetDay(short Day) { _Day = Day; }
short DateLib::GetDay() const { return _Day; }

void DateLib::SetMonth(short Month) { _Month = Month; }
short DateLib::GetMonth() const { return _Month; }

void DateLib::SetYear(short Year) { _Year = Year; }
short DateLib::GetYear() const { return _Year; }

// 3. Printing & String Conversions
string DateLib::DateToString(const DateLib &Date)
{
    return to_string(Date.GetDay()) + "/" + to_string(Date.GetMonth()) + "/" + to_string(Date.GetYear());
}

string DateLib::DateToString() const
{
    return DateToString(*this);
}

DateLib DateLib::StringToDate(const string &DateString, const string &Delim)
{
    vector<string> vDate = MyStringLib::SplitString(DateString, Delim);
    return DateLib(stoi(vDate[0]), stoi(vDate[1]), stoi(vDate[2]));
}

void DateLib::Print() const
{
    cout << DateToString() << endl;
}

string DateLib::FormatDate(const DateLib &Date, string DateFormat)
{
    string FormattedDateString = DateFormat;
    FormattedDateString = MyStringLib::ReplaceWord(FormattedDateString, "dd", to_string(Date.GetDay()));
    FormattedDateString = MyStringLib::ReplaceWord(FormattedDateString, "mm", to_string(Date.GetMonth()));
    FormattedDateString = MyStringLib::ReplaceWord(FormattedDateString, "yyyy", to_string(Date.GetYear()));
    return FormattedDateString;
}

string DateLib::FormatDate(const string &DateFormat) const
{
    return FormatDate(*this, DateFormat);
}

// 4. System Date & Validation
DateLib DateLib::GetSystemDate()
{
    time_t t = time(0);
    tm *now = localtime(&t);
    return DateLib(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

bool DateLib::IsValidDate(const DateLib &Date)
{
    if (Date.GetDay() < 1 || Date.GetDay() > 31)
        return false;
    if (Date.GetMonth() < 1 || Date.GetMonth() > 12)
        return false;

    short DaysInMonth = NumberOfDaysInAMonth(Date.GetMonth(), Date.GetYear());
    if (Date.GetDay() > DaysInMonth)
        return false;

    return true;
}

bool DateLib::IsValid() const
{
    return IsValidDate(*this);
}

// 5. Time Calculations
bool DateLib::isLeapYear(short Year)
{
    return (Year % 400 == 0 || (Year % 4 == 0 && Year % 100 != 0));
}

bool DateLib::isLeapYear() const
{
    return isLeapYear(_Year);
}

short DateLib::NumberOfDaysInAYear(short Year)
{
    return isLeapYear(Year) ? 366 : 365;
}

short DateLib::NumberOfDaysInAYear() const
{
    return NumberOfDaysInAYear(_Year);
}

short DateLib::NumberOfHoursInAYear(short Year) { return NumberOfDaysInAYear(Year) * 24; }
short DateLib::NumberOfHoursInAYear() const { return NumberOfHoursInAYear(_Year); }

int DateLib::NumberOfMinutesInAYear(short Year) { return NumberOfHoursInAYear(Year) * 60; }
int DateLib::NumberOfMinutesInAYear() const { return NumberOfMinutesInAYear(_Year); }

int DateLib::NumberOfSecondsInAYear(short Year) { return NumberOfMinutesInAYear(Year) * 60; }
int DateLib::NumberOfSecondsInAYear() const { return NumberOfSecondsInAYear(_Year); }

short DateLib::NumberOfDaysInAMonth(short Month, short Year)
{
    if (Month < 1 || Month > 12)
        return 0;
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (Month == 2) ? (isLeapYear(Year) ? 29 : 28) : days[Month - 1];
}

short DateLib::NumberOfDaysInAMonth() const
{
    return NumberOfDaysInAMonth(_Month, _Year);
}

short DateLib::NumberOfHoursInAMonth(short Month, short Year) { return NumberOfDaysInAMonth(Month, Year) * 24; }
short DateLib::NumberOfHoursInAMonth() const { return NumberOfHoursInAMonth(_Month, _Year); }

int DateLib::NumberOfMinutesInAMonth(short Month, short Year) { return NumberOfHoursInAMonth(Month, Year) * 60; }
int DateLib::NumberOfMinutesInAMonth() const { return NumberOfMinutesInAMonth(_Month, _Year); }

int DateLib::NumberOfSecondsInAMonth(short Month, short Year) { return NumberOfMinutesInAMonth(Month, Year) * 60; }
int DateLib::NumberOfSecondsInAMonth() const { return NumberOfSecondsInAMonth(_Month, _Year); }

// 6. Calendar & Short Names
short DateLib::DayOfWeekOrder(short Day, short Month, short Year)
{
    short a = (14 - Month) / 12;
    short y = Year - a;
    short m = Month + (12 * a) - 2;
    return (Day + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7;
}

short DateLib::DayOfWeekOrder() const
{
    return DayOfWeekOrder(_Day, _Month, _Year);
}

string DateLib::DayShortName(short DayOfWeekOrder)
{
    string arrDayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    return arrDayNames[DayOfWeekOrder];
}

string DateLib::DayShortName() const
{
    return DayShortName(DayOfWeekOrder());
}

string DateLib::MonthShortName(short MonthNumber)
{
    string Months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    return Months[MonthNumber - 1];
}

string DateLib::MonthShortName() const
{
    return MonthShortName(_Month);
}

void DateLib::PrintMonthCalendar(short Month, short Year)
{
    int current = DayOfWeekOrder(1, Month, Year);
    int NumberOfDays = NumberOfDaysInAMonth(Month, Year);

    printf("\n  _______________%s_______________\n\n", MonthShortName(Month).c_str());
    printf("  Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");

    int i;
    for (i = 0; i < current; i++)
        printf("     ");

    for (int j = 1; j <= NumberOfDays; j++)
    {
        printf("%5d", j);
        if (++i == 7)
        {
            i = 0;
            printf("\n");
        }
    }
    printf("\n  _________________________________\n");
}

void DateLib::PrintMonthCalendar() const
{
    PrintMonthCalendar(_Month, _Year);
}

void DateLib::PrintYearCalendar(int Year)
{
    printf("\n  _________________________________\n\n");
    printf("           Calendar - %d\n", Year);
    printf("  _________________________________\n");

    for (int i = 1; i <= 12; i++)
    {
        PrintMonthCalendar(i, Year);
    }
}

void DateLib::PrintYearCalendar() const
{
    PrintYearCalendar(_Year);
}

// 7. Days Order & Conversions
short DateLib::DaysFromTheBeginingOfTheYear(short Day, short Month, short Year)
{
    short TotalDays = 0;
    for (int i = 1; i <= Month - 1; i++)
    {
        TotalDays += NumberOfDaysInAMonth(i, Year);
    }
    TotalDays += Day;
    return TotalDays;
}

short DateLib::DaysFromTheBeginingOfTheYear() const
{
    return DaysFromTheBeginingOfTheYear(_Day, _Month, _Year);
}

DateLib DateLib::GetDateFromDayOrderInYear(short DateOrderInYear, short Year)
{
    DateLib Date;
    short RemainingDays = DateOrderInYear;
    short MonthDays = 0;

    Date.SetYear(Year);
    Date.SetMonth(1);

    while (true)
    {
        MonthDays = NumberOfDaysInAMonth(Date.GetMonth(), Year);
        if (RemainingDays > MonthDays)
        {
            RemainingDays -= MonthDays;
            Date.SetMonth(Date.GetMonth() + 1);
        }
        else
        {
            Date.SetDay(RemainingDays);
            break;
        }
    }
    return Date;
}

void DateLib::AddDays(short Days)
{
    short RemainingDays = Days + DaysFromTheBeginingOfTheYear(_Day, _Month, _Year);
    short MonthDays = 0;

    _Month = 1;

    while (true)
    {
        MonthDays = NumberOfDaysInAMonth(_Month, _Year);

        if (RemainingDays > MonthDays)
        {
            RemainingDays -= MonthDays;
            _Month++;

            if (_Month > 12)
            {
                _Month = 1;
                _Year++;
            }
        }
        else
        {
            _Day = RemainingDays;
            break;
        }
    }
}

// 8. Date Comparisons
bool DateLib::IsDate1BeforeDate2(const DateLib &Date1, const DateLib &Date2)
{
    return (Date1.GetYear() < Date2.GetYear()) ? true : ((Date1.GetYear() == Date2.GetYear()) ? (Date1.GetMonth() < Date2.GetMonth() ? true : (Date1.GetMonth() == Date2.GetMonth() ? Date1.GetDay() < Date2.GetDay() : false)) : false);
}

bool DateLib::IsDateBeforeDate2(const DateLib &Date2) const
{
    return IsDate1BeforeDate2(*this, Date2);
}

bool DateLib::IsDate1EqualDate2(const DateLib &Date1, const DateLib &Date2)
{
    return (Date1.GetYear() == Date2.GetYear() && Date1.GetMonth() == Date2.GetMonth() && Date1.GetDay() == Date2.GetDay());
}

bool DateLib::IsDateEqualDate2(const DateLib &Date2) const
{
    return IsDate1EqualDate2(*this, Date2);
}

bool DateLib::IsDate1AfterDate2(const DateLib &Date1, const DateLib &Date2)
{
    return (!IsDate1BeforeDate2(Date1, Date2) && !IsDate1EqualDate2(Date1, Date2));
}

bool DateLib::IsDateAfterDate2(const DateLib &Date2) const
{
    return IsDate1AfterDate2(*this, Date2);
}

DateLib::enCompareDates DateLib::CompareDates(const DateLib &Date1, const DateLib &Date2)
{
    if (IsDate1BeforeDate2(Date1, Date2))
        return enCompareDates::Before;
    if (IsDate1EqualDate2(Date1, Date2))
        return enCompareDates::Equal;
    return enCompareDates::After;
}

DateLib::enCompareDates DateLib::CompareDates(const DateLib &Date2) const
{
    return CompareDates(*this, Date2);
}

// 9. Date Boundary Checks
bool DateLib::IsLastDayInMonth(const DateLib &Date)
{
    return (Date.GetDay() == NumberOfDaysInAMonth(Date.GetMonth(), Date.GetYear()));
}

bool DateLib::IsLastDayInMonth() const
{
    return IsLastDayInMonth(*this);
}

bool DateLib::IsLastMonthInYear(short Month)
{
    return (Month == 12);
}

bool DateLib::IsLastMonthInYear() const
{
    return IsLastMonthInYear(_Month);
}

// 10. Operations: Add / Swap / Differences
DateLib DateLib::AddOneDay(DateLib Date)
{
    if (IsLastDayInMonth(Date))
    {
        if (IsLastMonthInYear(Date.GetMonth()))
        {
            Date.SetMonth(1);
            Date.SetDay(1);
            Date.SetYear(Date.GetYear() + 1);
        }
        else
        {
            Date.SetDay(1);
            Date.SetMonth(Date.GetMonth() + 1);
        }
    }
    else
    {
        Date.SetDay(Date.GetDay() + 1);
    }

    return Date;
}

void DateLib::AddOneDay()
{
    *this = AddOneDay(*this);
}

void DateLib::SwapDates(DateLib &Date1, DateLib &Date2)
{
    DateLib TempDate = Date1;
    Date1 = Date2;
    Date2 = TempDate;
}

int DateLib::GetDifferenceInDays(DateLib Date1, DateLib Date2, bool IncludeEndDay)
{
    int Days = 0;
    short SwapFlagValue = 1;

    if (!IsDate1BeforeDate2(Date1, Date2))
    {
        SwapDates(Date1, Date2);
        SwapFlagValue = -1;
    }

    while (IsDate1BeforeDate2(Date1, Date2))
    {
        Days++;
        Date1 = AddOneDay(Date1);
    }

    return IncludeEndDay ? ++Days * SwapFlagValue : Days * SwapFlagValue;
}

int DateLib::GetDifferenceInDays(const DateLib &Date2, bool IncludeEndDay) const
{
    return GetDifferenceInDays(*this, Date2, IncludeEndDay);
}

int DateLib::CalculateMyAgeInDays(const DateLib &DateOfBirth)
{
    return GetDifferenceInDays(DateOfBirth, DateLib::GetSystemDate(), true);
}

// 11. Increase Dates
DateLib DateLib::IncreaseDateByOneWeek(DateLib &Date)
{
    for (int i = 1; i <= 7; i++)
    {
        Date = AddOneDay(Date);
    }
    return Date;
}

void DateLib::IncreaseDateByOneWeek()
{
    IncreaseDateByOneWeek(*this);
}

DateLib DateLib::IncreaseDateByXWeeks(short Weeks, DateLib &Date)
{
    for (short i = 1; i <= Weeks; i++)
    {
        Date = IncreaseDateByOneWeek(Date);
    }
    return Date;
}

void DateLib::IncreaseDateByXWeeks(short Weeks)
{
    IncreaseDateByXWeeks(Weeks, *this);
}

DateLib DateLib::IncreaseDateByOneMonth(DateLib &Date)
{
    if (Date.GetMonth() == 12)
    {
        Date.SetMonth(1);
        Date.SetYear(Date.GetYear() + 1);
    }
    else
    {
        Date.SetMonth(Date.GetMonth() + 1);
    }

    short NumberOfDaysInCurrentMonth = NumberOfDaysInAMonth(Date.GetMonth(), Date.GetYear());
    if (Date.GetDay() > NumberOfDaysInCurrentMonth)
    {
        Date.SetDay(NumberOfDaysInCurrentMonth);
    }

    return Date;
}

void DateLib::IncreaseDateByOneMonth()
{
    IncreaseDateByOneMonth(*this);
}

DateLib DateLib::IncreaseDateByXDays(short Days, DateLib &Date)
{
    for (short i = 1; i <= Days; i++)
    {
        Date = AddOneDay(Date);
    }
    return Date;
}

void DateLib::IncreaseDateByXDays(short Days)
{
    IncreaseDateByXDays(Days, *this);
}

DateLib DateLib::IncreaseDateByXMonths(short Months, DateLib &Date)
{
    for (short i = 1; i <= Months; i++)
    {
        Date = IncreaseDateByOneMonth(Date);
    }
    return Date;
}

void DateLib::IncreaseDateByXMonths(short Months)
{
    IncreaseDateByXMonths(Months, *this);
}

DateLib DateLib::IncreaseDateByOneYear(DateLib &Date)
{
    Date.SetYear(Date.GetYear() + 1);
    short DaysInMonth = NumberOfDaysInAMonth(Date.GetMonth(), Date.GetYear());
    if (Date.GetDay() > DaysInMonth)
    {
        Date.SetDay(DaysInMonth);
    }
    return Date;
}

void DateLib::IncreaseDateByOneYear()
{
    IncreaseDateByOneYear(*this);
}

DateLib DateLib::IncreaseDateByXYears(short Years, DateLib &Date)
{
    Date.SetYear(Date.GetYear() + Years);
    short DaysInMonth = NumberOfDaysInAMonth(Date.GetMonth(), Date.GetYear());
    if (Date.GetDay() > DaysInMonth)
    {
        Date.SetDay(DaysInMonth);
    }
    return Date;
}

void DateLib::IncreaseDateByXYears(short Years)
{
    IncreaseDateByXYears(Years, *this);
}

DateLib DateLib::IncreaseDateByOneDecade(DateLib &Date)
{
    return IncreaseDateByXYears(10, Date);
}

void DateLib::IncreaseDateByOneDecade()
{
    IncreaseDateByOneDecade(*this);
}

DateLib DateLib::IncreaseDateByXDecades(short Decades, DateLib &Date)
{
    return IncreaseDateByXYears(Decades * 10, Date);
}

void DateLib::IncreaseDateByXDecades(short Decades)
{
    IncreaseDateByXDecades(Decades, *this);
}

DateLib DateLib::IncreaseDateByOneCentury(DateLib &Date)
{
    return IncreaseDateByXYears(100, Date);
}

void DateLib::IncreaseDateByOneCentury()
{
    IncreaseDateByOneCentury(*this);
}

DateLib DateLib::IncreaseDateByOneMillennium(DateLib &Date)
{
    return IncreaseDateByXYears(1000, Date);
}

void DateLib::IncreaseDateByOneMillennium()
{
    IncreaseDateByOneMillennium(*this);
}

// 12. Decrease Dates
DateLib DateLib::DecreaseDateByOneDay(DateLib Date)
{
    if (Date.GetDay() == 1)
    {
        if (Date.GetMonth() == 1)
        {
            Date.SetMonth(12);
            Date.SetDay(31);
            Date.SetYear(Date.GetYear() - 1);
        }
        else
        {
            Date.SetMonth(Date.GetMonth() - 1);
            Date.SetDay(NumberOfDaysInAMonth(Date.GetMonth(), Date.GetYear()));
        }
    }
    else
    {
        Date.SetDay(Date.GetDay() - 1);
    }

    return Date;
}

void DateLib::DecreaseDateByOneDay()
{
    *this = DecreaseDateByOneDay(*this);
}

DateLib DateLib::DecreaseDateByXDays(short Days, DateLib &Date)
{
    for (short i = 1; i <= Days; i++)
    {
        Date = DecreaseDateByOneDay(Date);
    }
    return Date;
}

void DateLib::DecreaseDateByXDays(short Days)
{
    DecreaseDateByXDays(Days, *this);
}

DateLib DateLib::DecreaseDateByOneWeek(DateLib &Date)
{
    return DecreaseDateByXDays(7, Date);
}

void DateLib::DecreaseDateByOneWeek()
{
    DecreaseDateByOneWeek(*this);
}

DateLib DateLib::DecreaseDateByXWeeks(short Weeks, DateLib &Date)
{
    for (short i = 1; i <= Weeks; i++)
    {
        Date = DecreaseDateByOneWeek(Date);
    }
    return Date;
}

void DateLib::DecreaseDateByXWeeks(short Weeks)
{
    DecreaseDateByXWeeks(Weeks, *this);
}

DateLib DateLib::DecreaseDateByOneMonth(DateLib &Date)
{
    if (Date.GetMonth() == 1)
    {
        Date.SetMonth(12);
        Date.SetYear(Date.GetYear() - 1);
    }
    else
        Date.SetMonth(Date.GetMonth() - 1);

    short DaysInMonth = NumberOfDaysInAMonth(Date.GetMonth(), Date.GetYear());
    if (Date.GetDay() > DaysInMonth)
    {
        Date.SetDay(DaysInMonth);
    }

    return Date;
}

void DateLib::DecreaseDateByOneMonth()
{
    DecreaseDateByOneMonth(*this);
}

DateLib DateLib::DecreaseDateByXMonths(short Months, DateLib &Date)
{
    for (short i = 1; i <= Months; i++)
    {
        Date = DecreaseDateByOneMonth(Date);
    }
    return Date;
}

void DateLib::DecreaseDateByXMonths(short Months)
{
    DecreaseDateByXMonths(Months, *this);
}

DateLib DateLib::DecreaseDateByOneYear(DateLib &Date)
{
    Date.SetYear(Date.GetYear() - 1);
    short DaysInMonth = NumberOfDaysInAMonth(Date.GetMonth(), Date.GetYear());
    if (Date.GetDay() > DaysInMonth)
    {
        Date.SetDay(DaysInMonth);
    }
    return Date;
}

void DateLib::DecreaseDateByOneYear()
{
    DecreaseDateByOneYear(*this);
}

DateLib DateLib::DecreaseDateByXYears(short Years, DateLib &Date)
{
    Date.SetYear(Date.GetYear() - Years);
    short DaysInMonth = NumberOfDaysInAMonth(Date.GetMonth(), Date.GetYear());
    if (Date.GetDay() > DaysInMonth)
    {
        Date.SetDay(DaysInMonth);
    }
    return Date;
}

void DateLib::DecreaseDateByXYears(short Years)
{
    DecreaseDateByXYears(Years, *this);
}

DateLib DateLib::DecreaseDateByOneDecade(DateLib &Date)
{
    return DecreaseDateByXYears(10, Date);
}

void DateLib::DecreaseDateByOneDecade()
{
    DecreaseDateByOneDecade(*this);
}

DateLib DateLib::DecreaseDateByXDecades(short Decades, DateLib &Date)
{
    return DecreaseDateByXYears(Decades * 10, Date);
}

void DateLib::DecreaseDateByXDecades(short Decades)
{
    DecreaseDateByXDecades(Decades, *this);
}

DateLib DateLib::DecreaseDateByOneCentury(DateLib &Date)
{
    return DecreaseDateByXYears(100, Date);
}

void DateLib::DecreaseDateByOneCentury()
{
    DecreaseDateByOneCentury(*this);
}

DateLib DateLib::DecreaseDateByOneMillennium(DateLib &Date)
{
    return DecreaseDateByXYears(1000, Date);
}

void DateLib::DecreaseDateByOneMillennium()
{
    DecreaseDateByOneMillennium(*this);
}

// 13. Business Days & Vacations
bool DateLib::IsEndOfWeek(const DateLib &Date)
{
    return DayOfWeekOrder(Date.GetDay(), Date.GetMonth(), Date.GetYear()) == 6;
}

bool DateLib::IsEndOfWeek() const
{
    return IsEndOfWeek(*this);
}

bool DateLib::IsWeekEnd(const DateLib &Date)
{
    short DayIndex = DayOfWeekOrder(Date.GetDay(), Date.GetMonth(), Date.GetYear());
    return (DayIndex == 5 || DayIndex == 6);
}

bool DateLib::IsWeekEnd() const
{
    return IsWeekEnd(*this);
}

bool DateLib::IsBusinessDay(const DateLib &Date)
{
    return !IsWeekEnd(Date);
}

bool DateLib::IsBusinessDay() const
{
    return IsBusinessDay(*this);
}

short DateLib::DaysUntilTheEndOfWeek(const DateLib &Date)
{
    return 6 - DayOfWeekOrder(Date.GetDay(), Date.GetMonth(), Date.GetYear());
}

short DateLib::DaysUntilTheEndOfWeek() const
{
    return DaysUntilTheEndOfWeek(*this);
}

short DateLib::DaysUntilTheEndOfMonth(const DateLib &Date1)
{
    DateLib EndOfMonthDate;
    EndOfMonthDate.SetDay(NumberOfDaysInAMonth(Date1.GetMonth(), Date1.GetYear()));
    EndOfMonthDate.SetMonth(Date1.GetMonth());
    EndOfMonthDate.SetYear(Date1.GetYear());

    return GetDifferenceInDays(Date1, EndOfMonthDate, true);
}

short DateLib::DaysUntilTheEndOfMonth() const
{
    return DaysUntilTheEndOfMonth(*this);
}

short DateLib::DaysUntilTheEndOfYear(const DateLib &Date1)
{
    DateLib EndOfYearDate;
    EndOfYearDate.SetDay(31);
    EndOfYearDate.SetMonth(12);
    EndOfYearDate.SetYear(Date1.GetYear());

    return GetDifferenceInDays(Date1, EndOfYearDate, true);
}

short DateLib::DaysUntilTheEndOfYear() const
{
    return DaysUntilTheEndOfYear(*this);
}

short DateLib::CalculateBusinessDays(DateLib DateFrom, DateLib DateTo)
{
    short Days = 0;
    while (IsDate1BeforeDate2(DateFrom, DateTo))
    {
        if (IsBusinessDay(DateFrom))
            Days++;

        DateFrom = AddOneDay(DateFrom);
    }

    return Days;
}

short DateLib::CalculateVacationDays(const DateLib &DateFrom, const DateLib &DateTo)
{
    return CalculateBusinessDays(DateFrom, DateTo);
}

DateLib DateLib::CalculateVacationReturnDate(DateLib DateFrom, short VacationDays)
{
    while (VacationDays > 0)
    {
        if (IsBusinessDay(DateFrom))
            VacationDays--;

        DateFrom = AddOneDay(DateFrom);
    }

    while (IsWeekEnd(DateFrom))
    {
        DateFrom = AddOneDay(DateFrom);
    }

    return DateFrom;
}