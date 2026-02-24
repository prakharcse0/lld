#include <iostream>
#include <string>

// Base class
class Base {
protected:
    int id;  
    // protected member:
    // Accessible inside Base and derived classes,
    // but NOT accessible from outside (like main).

public:
    // Constructor
    Base(int id) : id(id) {}

    // Two overloaded functions
    void log(int x) {
        std::cout << "Base::log(int): " << x << "\n";
    }

    void log(double x) {
        std::cout << "Base::log(double): " << x << "\n";
    }
};


// Derived class
class Derived : public Base {
public:

    // =========================
    // CASE 6: Inheriting constructors
    // =========================
    using Base::Base;
    /*
    What this does:
    - Imports all constructors from Base into Derived.

    Without this line, the following would NOT compile:

        Derived d(42);

    because constructors are NOT automatically inherited in C++.

    Without 'using Base::Base', you would need to write:

        Derived(int id) : Base(id) {}

    This is very useful when Derived doesn't need custom construction logic.
    */


    // =========================
    // CASE 5: Bringing base overloads into scope
    // =========================
    using Base::log;
    /*
    Important C++ rule:
    If a derived class defines a function with the same name,
    it HIDES ALL overloads from the base class.

    For example, if we add:

        void log(std::string)

    then BOTH of these become hidden:
        Base::log(int)
        Base::log(double)

    So this would fail without 'using Base::log':

        d.log(10);     // error
        d.log(3.14);   // error

    'using Base::log' restores the entire overload set.
    */


    // New overload added in Derived
    void log(const std::string& s) {
        std::cout << "Derived::log(string): " << s << "\n";
    }


    // =========================
    // Changing access level using 'using'
    // =========================
    using Base::id;
    /*
    This re-exposes Base::id with the access level
    of the section where this appears (public).

    So effectively:
        protected -> public

    Without this, the following would fail:

        d.id

    because id is protected in Base.
    */
};


int main() {
    // Works because of: using Base::Base
    Derived d(42);

    // Works because of: using Base::log
    d.log(10);       // calls Base::log(int)
    d.log(3.14);     // calls Base::log(double)

    // Calls Derived version
    d.log("hello");

    // Works because we exposed id publicly
    std::cout << "id = " << d.id << "\n";
}