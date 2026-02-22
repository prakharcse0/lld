//operator overloading
#include <iostream>
#include <numeric>
using namespace std;

class Fraction {
private:
    int numerator;
    int denominator;

    void simplify() {
        int fraction_gcd = std::gcd(numerator, denominator);
        numerator /= fraction_gcd;
        denominator /= fraction_gcd;
    }

public:
    Fraction(int numerator, int denominator) {
        this->numerator = numerator;
        this->denominator = denominator;
        // simplify();
    }

    // operators overloading (+, *, +=, ++, ==)
    Fraction operator+ (Fraction const &f2) const {
        int numerator = this->numerator * f2.denominator + f2.numerator * this->denominator;
        int denominator = this->denominator * f2.denominator;
        Fraction sum(numerator, denominator);
        sum.simplify();
        return sum;
    }

    Fraction operator* (Fraction const &f2) const {
        Fraction product(this->numerator * f2.numerator, this->denominator * f2.denominator);
        product.simplify();
        return product;
    }

    bool operator== (Fraction const &f2) const {
        return (this->numerator*f2.denominator == f2.numerator*this->denominator);
    }

    // pre-increment ++
    // We return Fraction& instead of Fraction 
    // this allows us to do something like ++(++f1); //f1 is a fraction //f1 += 2 here
    Fraction& operator++() {
        this->numerator += this->denominator;
        return *this;
    }
    // ofcourse, since this is our own class & definitions, we dont need to return by reference even for pre-fix increment
    // but thats whats done everywhere in the language
    // and is the expected behaviour / convention generally

    // post-increment ++  
    // represented by ++(int) by convention to differentiate from pre-increment ++
    Fraction operator++(int) {
        Fraction original(this->numerator, this->denominator);
        this->numerator += denominator;
        return original;
    }

    Fraction& operator+=(Fraction const & f2) {
        this->numerator = this->numerator * f2.denominator + f2.numerator * this->denominator;
        this->denominator = this->denominator * f2.denominator;
        simplify();
        return *this;
    }


    // Overloading <<

    // OPTION 1: use public getters: requires public getters / variables:
    /*
    std::ostream& operator<<(std::ostream& os, const Student& s) {
        os << s.get_roll() << '\t' << s.get_name() << '\t' << s.get_age();
        return os;
    }
    */
    // why return os?
    // std::cout << s1 << s2 << '\n';
    // is parsed as:
    // ((std::cout << s1) << s2) << '\n';
    // each << must return the stream for the next << to work
    // prefer this option unless performance critical or getters don't exist

    // OPTION 2: friend function (access private members directly)
    // declare inside class — grants access to privates
    friend std::ostream& operator<<(std::ostream&, const Fraction &);
    // define outside class
    // friend is not a member function — just a free function with private access
};

std::ostream& operator<<(std::ostream &os, const Fraction &f) {
    os <<f.numerator <<'/' <<f.denominator;
    return os;
}
