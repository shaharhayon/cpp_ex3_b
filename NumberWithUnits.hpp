#include <string>
#include <iostream>
#include <fstream>
#include <map>
namespace ariel
{
    class NumberWithUnits
    {
    private:
        double number;
        std::string units;
        inline static std::map<std::string, std::map<std::string, double>> ct;

    public:
        NumberWithUnits(double n, const std::string& u);
        static void read_units(std::ifstream& unit_file);
        NumberWithUnits operator+(const NumberWithUnits &n);
        NumberWithUnits operator+=(const NumberWithUnits &n);
        NumberWithUnits operator+();
        NumberWithUnits operator-(const NumberWithUnits &n);
        NumberWithUnits operator-=(const NumberWithUnits &n);
        NumberWithUnits operator-();
        bool operator>(const NumberWithUnits &n) const;
        bool operator>=(const NumberWithUnits &n) const;
        bool operator<(const NumberWithUnits &n) const;
        bool operator<=(const NumberWithUnits &n) const;
        bool operator==(const NumberWithUnits &n) const;
        bool operator!=(const NumberWithUnits &n) const;
        NumberWithUnits& operator++();
        NumberWithUnits operator++(int);
        NumberWithUnits& operator--();
        NumberWithUnits operator--(int);

        friend NumberWithUnits operator*(double d, const NumberWithUnits& n);
        friend NumberWithUnits operator*(const NumberWithUnits& n, double d);
        friend std::istream &operator>>(std::istream &is, NumberWithUnits& n);
        friend std::ostream &operator<<(std::ostream &os, const NumberWithUnits& n);

        static double convert(const std::string& u1, const std::string& u2);
    };
}