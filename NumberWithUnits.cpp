#include "NumberWithUnits.hpp"
#include <vector>
#include <algorithm>

using namespace std;
using namespace ariel;

NumberWithUnits::NumberWithUnits(double n, const string& u) : number(n), units(u){
    // if(!ct.contains(u)){
    if((ct.count(u)) == 0){
        throw invalid_argument("Illegal unit!");
    }
}
void NumberWithUnits::read_units(ifstream& unit_file)
{
    string line;
    while(getline(unit_file, line)){
        unsigned long eq = line.find('=');
        unsigned long u=0;
        //double l_num=0;
        double r_num=0;
        if(line.at(eq-1) != ' ' || line.at(eq+1) != ' '){
            throw invalid_argument("No spaces at the sides of '='!");
        }
        //line.erase(' ');
        line.erase(remove(line.begin(),line.end(), ' '), line.end());
        eq = line.find('=');
        string l;
        string r;
        string l_unit;
        string r_unit;
        l=line.substr(0, eq);
        r=line.substr(eq+1,line.length()-eq);
        while((isdigit(l.at(u)) != 0) || l.at(u) == '.'){
            u++;
        }
        //l_num = stod(l.substr(0,u));
        l_unit = l.substr(u, l.length() - u);
        u=0;
        while((isdigit(r.at(u)) != 0) || r.at(u) == '.'){
            u++;
        }
        r_num = stod(r.substr(0,u));
        r_unit = r.substr(u, r.length() - u);
 

        ct[l_unit].insert(pair<string, double>(r_unit, r_num));
        ct[r_unit].insert(pair<string, double>(l_unit, 1.0/r_num));
    }
}
NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &n)
{
    double converted = convert(n.units, this->units);
    if(converted == 0){
        throw invalid_argument("Illegal units for operation (+)!");
    }    
    return NumberWithUnits(this->number + converted * n.number, this->units);
}
NumberWithUnits NumberWithUnits::operator+=(const NumberWithUnits &n)
{
    *this = NumberWithUnits(*this + n);
    return (*this);
}
NumberWithUnits NumberWithUnits::operator+()
{
    return *this;
}
NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &n)
{
    double converted = convert(n.units, this->units);
    if(converted == 0){
        throw invalid_argument("Illegal units for operation (-)!");
    }    
    return NumberWithUnits(this->number - converted * n.number, this->units);
}
NumberWithUnits NumberWithUnits::operator-=(const NumberWithUnits &n)
{
    *this = NumberWithUnits(*this - n);
    return (*this);
}
NumberWithUnits NumberWithUnits::operator-()
{
    return NumberWithUnits(this->number * (-1), this->units);
}
bool NumberWithUnits::operator>(const NumberWithUnits &n) const
{
    NumberWithUnits num1(this->number, this->units);
    num1-=n;
    NumberWithUnits num2(n.number, n.units);
    num2-=*this;
    return ((num1.number > 0) && (num2.number < 0));
    // if(num.number > 0){
    //     return true;
    // }
    // return false;
}
bool NumberWithUnits::operator>=(const NumberWithUnits &n) const
{
    return ((*this > n) || (*this == n));
    // if((*this > n) || (*this == n)){
    //     return true;
    // }
    // return false;
}
bool NumberWithUnits::operator<(const NumberWithUnits &n) const
{
    NumberWithUnits num1(this->number, this->units);
    num1-=n;
    NumberWithUnits num2(n.number, n.units);
    num2-=*this;
    return ((num1.number < 0) && (num2.number > 0));
    // if(num.number < 0){
    //     return true;
    // }
    // return false;
}
bool NumberWithUnits::operator<=(const NumberWithUnits &n) const
{
    return ((*this < n) || (*this == n));
    // if((*this < n) || (*this == n)){
    //     return true;
    // }
    // return false;
}
bool NumberWithUnits::operator==(const NumberWithUnits &n) const
{
    const double EPS = 0.001;
    NumberWithUnits num1(this->number, this->units);
    num1-=n;
    NumberWithUnits num2(n.number, n.units);
    num2-=*this;
    return ((num1.number<=EPS && num1.number>= -EPS) &&
            (num2.number<=EPS && num2.number>= -EPS));
    // if(num.number<=EPS && num.number>= -EPS){ 
    //     return true;
    // }
    // return false;
}
bool NumberWithUnits::operator!=(const NumberWithUnits &n) const
{
    return (!(*this == n));
    // if(!(*this == n)){
    //     return true;
    // }
    // return false;
}
NumberWithUnits NumberWithUnits::operator++(int)
{
    // double tmp = this->number;
    // this->number++;
    // return NumberWithUnits(tmp,this->units);
    this->number++;
    return NumberWithUnits(this->number - 1,this->units);
}
NumberWithUnits& NumberWithUnits::operator++()
{
    this->number++;
    return *this;
}
NumberWithUnits NumberWithUnits::operator--(int)
{
    double tmp = this->number;
    this->number--;
    return NumberWithUnits(tmp,this->units);
}
NumberWithUnits& NumberWithUnits::operator--()
{
    this->number--;
    return *this;
}

NumberWithUnits ariel::operator*(double d, const NumberWithUnits& n)
{
    return NumberWithUnits(d * n.number, n.units);
}
NumberWithUnits ariel::operator*(const NumberWithUnits& n, double d)
{
    return (d * n);
}
ostream & ariel::operator<<(ostream &os, const NumberWithUnits& n)
{
    os << n.number << '[' << n.units << ']';
    return os;
}
istream & ariel::operator>>(istream &is, NumberWithUnits& n)
{
    unsigned long u=0;
    double num=0;
    string input;
    string unit;
    // getline(is, input);
    char c=0;
    is >> c;
    while(c != ']'){
            input+=c;
            is >> c;
    }
    input+=c;
    input.erase(remove(input.begin(),input.end(), ' '), input.end());
    input.erase(remove(input.begin(),input.end(), '['), input.end());
    input.erase(remove(input.begin(),input.end(), ']'), input.end());
    u=0;
    while((u<input.length()) && (((isdigit(input.at(u)) != 0) || input.at(u) == '.' || input.at(u) == '-') || input.at(u) == '+')){
            u++;
    }
    if(u >= input.length()){
        throw invalid_argument("Illegal unit!");
    }
    num = stod(input.substr(0,u));
    unit = input.substr(u, input.length() - u);
    //if(!(n.ct.contains(unit))){
    if((NumberWithUnits::ct.count(unit)) == 0){
        throw invalid_argument("Illegal unit!");
    }
        n.number=num;
        n.units=unit;
    return is;
}

double NumberWithUnits::convert(const string& u1, const string& u2){
    static vector<string> loop;
    if(u1 == u2){
        return 1;
    }
    if(ct.at(u1).find(u2) != ct.at(u1).end()){
        loop.clear();
        return ct.at(u1).at(u2);
    }
    
    for(auto it = ct.at(u1).begin(); it != ct.at(u1).end(); it++){
        if(find(loop.begin(), loop.end(), it->first) != loop.end()){
            continue;
        }
        loop.insert(loop.end()--, u1);
        double coefficient = (it->second * convert(it->first, u2));
        if (coefficient == 0){
            continue;
        }
        return coefficient;
        
    }
    return 0;
    // if(ct.find(pair(u1,u2)) != ct.end()){
    //     return ct.at(pair(u1,u2));
    // }
    // vector<pair<string,string>> vunits;
    // for(map<pair<string,string>, double>::const_iterator it = ct.begin(); it != ct.end(); it++){
        
    // }
    // int res = ct.at(pair(u1,u2));


    // return res;
}