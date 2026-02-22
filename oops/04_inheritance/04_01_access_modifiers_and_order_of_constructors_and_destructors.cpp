#include <iostream>
#include <string>
using namespace std;

// Before the creation of an object of a child class, its parent class constructor runs first.
// Constructors run from base → derived.
// Destructors run in the reverse order (derived → base).

class Vehicle {
    private :
        int maxSpeed;   // not accessible directly in derived classes

    protected :
        int numTyres;   // accessible inside derived classes

    public :
        string color;   // accessible everywhere

    // default inheritance access specifiers for class : private inheritance
    // Most well-designed base classes still keep data private, use protected methods occasionallt & protected members rarely

    Vehicle(int z) {
        cout << "Vehicle's Parametrized constructor" << endl;
        maxSpeed = z;
        numTyres = 4;
        color = "Black";
    }

    ~Vehicle() {
        cout << "Vehicle's Destructor" << endl;
    }

    int get_max_speed() const {   // controlled access to private data
        return maxSpeed;
    }
};


class Car : public Vehicle {
    public :
        int numGears;

        // Using initialization list to call the Vehicle constructor.
        // Without this, the compiler would try to call Vehicle's default constructor.
        // Since it doesn't exist, compilation would fail.
        Car(int x, int y) : Vehicle(x) {
            cout << "Car's constructor" << endl;
            numGears = y;
        }

        ~Car() {
            cout << "Car's Destructor" << endl;
        }

        void print() {
            cout << "NumTyres : " << numTyres << endl;   // protected → accessible
            cout << "Color : " << color << endl;         // public → accessible
            cout << "Num gears : " << numGears << endl;
            cout << "Max Speed : " << get_max_speed() << endl; // access private via method
            // cout << maxSpeed; // ERROR: private in Vehicle
        }
};


class Tesla : public Car {
    public :

        Tesla(int x, int y) : Car(x, y) {
            cout << "Tesla's constructor" << endl;
        }

        ~Tesla() {
            cout << "Tesla's destructor" << endl;
        }
};

// public inheritance :
// private members are not accessible in derived class
// protected members remain protected
// public members remain public

// protected inheritance :
// private members are not accessible in derived class
// protected & public members become protected

// private inheritance :
// private members are not accessible in derived class
// protected & public members become private


int main() {
    Tesla t(200, 6);
    // Vehicle's Parametrized constructor
    // Car's constructor
    // Tesla's constructor

    t.print();
    // NumTyres : 4
    // Color : Black
    // Num gears : 6
    // Max Speed : 200

    // Tesla's destructor
    // Car's Destructor
    // Vehicle's Destructor
}