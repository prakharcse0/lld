#include <iostream>
using namespace std;

// Pure Virtual Functions:
// Virtual functions that are declared in the base class but have no definition there (typically).
// A virtual function whose declaration ends with = 0 is called a pure virtual function.
// They must be overridden by derived classes (with rare exceptions, see note below).

// Abstract Class:
// A class with at least one pure virtual function.
// Cannot be instantiated (no objects can be created from it directly).
// A derived class must implement ALL pure virtual functions of its base,
// otherwise the derived class becomes abstract too.

// Note: Pure virtual functions CAN have a definition in the base class (legal but rare).
// Even so, derived classes are still forced to override them.

class Vehicle {
    public:
        string color;

        // Virtual destructor — essential for polymorphic base classes
        // Ensures correct destructor chain when deleting via base pointer
        virtual ~Vehicle() {}

        // Pure virtual functions — no definition here, derived classes must override
        virtual void print() = 0;
        virtual void print_tyres() = 0;
};

// Car overrides print() but NOT print_tyres()
// So Car is still abstract — cannot be instantiated
class Car : public Vehicle {
    public:
        int numGears;

        void print() override {
            cout << "Car" << endl;
        }
};

// Tesla overrides print_tyres() (inherits print() from Car)
// Now ALL pure virtual functions are implemented -> Tesla is concrete, can be instantiated
class Tesla : public Car {
    public:
        void print_tyres() override {
            cout << "4" << endl;
        }
};


int main() {
    // Vehicle v;  // error: cannot declare variable 'v' of abstract type 'Vehicle'
    // Car c;      // error: cannot declare variable 'c' of abstract type 'Car'
    //             // (print_tyres() still not implemented in Car)

    Tesla t;        // OK: all pure virtual functions are implemented
    t.print();      // Output: Car       (inherited from Car)
    t.print_tyres();// Output: 4

    // Polymorphism still works as expected
    Vehicle *v = &t;
    v->print();       // Output: Car
    v->print_tyres(); // Output: 4
}

// Abstract Class:
// A class with at least one pure virtual function.
// Cannot be instantiated (no objects can be created from it directly).
// A derived class must implement ALL pure virtual functions of its base,
// otherwise the derived class becomes abstract too.
// 
// Note: Pure virtual functions CAN have a definition in the base class (legal but rare).
// Even so, derived classes are still forced to override them.
// Example:
//   virtual void print() = 0;          // pure virtual declaration (inside class)
//   void Vehicle::print() { ... }      // definition provided outside the class
//
// Derived classes can then explicitly call the base version if they want:
//   void Car::print() {
//       Vehicle::print();  // explicitly calling base version
//       cout << "Car" << endl;
//   }
// 
// Why do this?
//   - Provide a sensible default that derived classes can optionally reuse
//   - Still forces derived classes to make a conscious override
// 
// Why is it rare?
//   - If you want a default, just use a regular virtual function
//   - Pure virtual is usually chosen precisely because you DON'T want a default
//     you want to force the derived class to implement it themselves

