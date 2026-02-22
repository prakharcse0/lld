// hybrid_inheritance.cpp
// ------------------------------------------------------------
// This file demonstrates the classic DIAMOND PROBLEM
// using NON-VIRTUAL inheritance.
//
// Key ideas:
// 1. Bus ends up containing TWO Vehicle subobjects.
// 2. This causes ambiguity.
// 3. Constructor and destructor order becomes important.
// 4. Accessing Vehicle members may require qualification.
//
// Structure:
//
//         Vehicle
//         /    \
//       Car    Truck
//         \    /
//           Bus
//
// But in memory (non-virtual inheritance):
//
// Bus
// ├── Car
// │    └── Vehicle   <-- separate object
// └── Truck
//      └── Vehicle   <-- separate object
//
// So Bus contains TWO Vehicles.
//
// ------------------------------------------------------------

#include <iostream>
#include <string>
using namespace std;

class Vehicle {
private:
    int maxSpeed;

protected:
    int numTyres;

public:
    string color;

    void print_vehicle() {
        cout << "Vehicle::print_vehicle()" << endl;
    }

    Vehicle(int z) : maxSpeed(z), numTyres(0) {
        cout << "Vehicle constructor, maxSpeed = " << z << endl;
    }

    int max_speed() const {
        return maxSpeed;
    }

    ~Vehicle() {
        cout << "Vehicle destructor" << endl;
    }
};

class Car : public Vehicle {
public:
    int numGears;

    // NOTE:
    // In non-virtual inheritance, Car is responsible for
    // constructing its own Vehicle subobject.
    Car() : Vehicle(3), numGears(5) {
        cout << "Car constructor" << endl;
        numTyres = 4;
    }

    ~Car() {
        cout << "Car destructor" << endl;
    }

    void print() {
        cout << "Car::print()" << endl;
        cout << "numTyres = " << numTyres << endl;
        cout << "color = " << color << endl;
        cout << "numGears = " << numGears << endl;
        cout << "maxSpeed = " << max_speed() << endl;
    }
};

class Truck : public Vehicle {
public:
    Truck() : Vehicle(4) {
        cout << "Truck constructor" << endl;
        numTyres = 6;
    }

    ~Truck() {
        cout << "Truck destructor" << endl;
    }
};

class Bus : public Car, public Truck {
public:
    Bus() {
        cout << "Bus constructor" << endl;
    }

    ~Bus() {
        cout << "Bus destructor" << endl;
    }
};

int main() {

    // --------------------------------------------------------
    // CONSTRUCTION ORDER (important interview topic)
    //
    // Base classes are constructed in the order they appear
    // in the class declaration of Bus:
    //
    // class Bus : public Car, public Truck
    //
    // So order:
    //
    // Vehicle (via Car)
    // Car
    // Vehicle (via Truck)
    // Truck
    // Bus
    // --------------------------------------------------------

    Bus b;

    cout << "\n--- Calling Car version of print ---\n";
    b.Car::print();

    // --------------------------------------------------------
    // AMBIGUITY EXAMPLE
    //
    // Bus has TWO Vehicle objects.
    //
    // So this is ambiguous:
    //
    // b.print_vehicle();  // ERROR
    //
    // Compiler doesn't know which path:
    // Bus -> Car -> Vehicle
    // Bus -> Truck -> Vehicle
    //
    // Fix:
    // --------------------------------------------------------

    b.Car::print_vehicle();
    b.Truck::print_vehicle();

    // --------------------------------------------------------
    // Ambiguity also applies to data members
    // --------------------------------------------------------

    b.Car::color = "Red";
    b.Truck::color = "Blue";

    cout << "\nCar Vehicle color: " << b.Car::color << endl;
    cout << "Truck Vehicle color: " << b.Truck::color << endl;

    // --------------------------------------------------------
    // OBJECT LAYOUT (conceptual)
    //
    // Memory roughly looks like:
    //
    // [Car part]
    //   Vehicle
    //   Car fields
    //
    // [Truck part]
    //   Vehicle
    //   Truck fields
    //
    // Bus fields
    //
    // TWO complete Vehicle objects exist.
    //
    // This wastes memory and causes ambiguity.
    // That is why virtual inheritance exists.
    // --------------------------------------------------------

    return 0;
}

/*
DESTRUCTION ORDER

Exact reverse of construction:

Bus
Truck
Vehicle (from Truck)
Car
Vehicle (from Car)

Important rule:
C++ destroys members and bases in reverse order.
*/