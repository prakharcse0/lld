#include <iostream>
using namespace std;

class Vehicle {
    public:
        string color;

        virtual void print() {
            cout << "Vehicle" << endl;
        }

        virtual void num_tyres() {
            cout << "Unknown" << endl;
        }

        // Virtual destructor is essential when using base class pointers
        // Without it, deleting a derived object via base pointer causes undefined behavior
        // (only ~Vehicle() would be called, leaking Car's resources)
        virtual ~Vehicle() {
            cout << "~Vehicle destructor called" << endl;
        }
        // Rule of thumb: define a virtual destructor iff the class has any virtual function
};

class Car : public Vehicle {
    public:
        int numGears;

        // 'override' keyword is best practice:
        // - makes intent explicit
        // - compiler error if signature doesn't match base class (catches typos)
        void print() override {
            cout << "Car" << endl;
        }

        // Car implicitly has a virtual print() even without the 'virtual' keyword,
        // because it inherits virtual-ness from the base class

        ~Car() {
            cout << "~Car destructor called" << endl;
        }
};


int main() {
    Vehicle v;
    Car c;
    v.print();  // Output : Vehicle
    c.print();  // Output : Car


    // Virtual functions allow us to achieve runtime polymorphism
    // The correct function to call is resolved at runtime via the vtable
    // based on the actual object the pointer points to, not the pointer's type
    Vehicle *v1 = new Vehicle;
    Vehicle *v2;
    v2 = &c;
    v1 -> print();  // Output : Vehicle
    v2 -> print();  // Output : Car  <-- key: v2 is Vehicle* but calls Car::print()

    // If a virtual function is not overridden by the derived class,
    // the base class implementation is used
    v2 -> num_tyres();  // Output : Unknown  (Car didn't override this)

    // Because ~Vehicle() is virtual, deleting via base pointer correctly calls
    // ~Car() first, then ~Vehicle() (destructor chain)
    delete v1;  // Output : ~Vehicle destructor called
    // (v2 points to stack object c, so don't delete it)
}