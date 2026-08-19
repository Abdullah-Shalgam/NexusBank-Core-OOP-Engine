#pragma once

#include <string>
#include <utility>

/**
 * @class clsPerson
 * @brief Base domain entity encapsulating common personal identity attributes.
 */
class clsPerson
{
private:
    std::string _FirstName;
    std::string _LastName;
    std::string _Email;
    std::string _Phone;

public:
    // Constructors & Virtual Destructor
    clsPerson() = default;

    clsPerson(std::string FirstName, std::string LastName, std::string Email, std::string Phone)
        : _FirstName(std::move(FirstName)),
          _LastName(std::move(LastName)),
          _Email(std::move(Email)),
          _Phone(std::move(Phone))
    {
    }

    // Essential for safe polymorphic deletion
    virtual ~clsPerson() = default;

    // Setters (Modern Pass-by-value with Move Semantics)
    void SetFirstName(std::string FirstName) { _FirstName = std::move(FirstName); }
    void SetLastName(std::string LastName) { _LastName = std::move(LastName); }
    void SetEmail(std::string Email) { _Email = std::move(Email); }
    void SetPhone(std::string Phone) { _Phone = std::move(Phone); }

    // Getters (Zero-copy const references for high performance)
    const std::string& GetFirstName() const noexcept { return _FirstName; }
    const std::string& GetLastName() const noexcept { return _LastName; }
    const std::string& GetEmail() const noexcept { return _Email; }
    const std::string& GetPhone() const noexcept { return _Phone; }

    // Domain Properties
    std::string GetFullName() const
    {
        return _FirstName.empty() ? _LastName : (_FirstName + " " + _LastName);
    }
};