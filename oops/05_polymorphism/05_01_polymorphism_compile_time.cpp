#include <iostream>
using namespace std;

class Vehicle {
    public :
        string color;

    void print() {
        cout << "Vehicle" << endl;
    }
};

class Car : public Vehicle {
    public :
        int numGears;

        void print() {
            cout << "Car" << endl;
        } 
        // not exactly function overriding, thats runtime polymorphism
        // Overriding without virtual is just hiding/shadowing, which is what the code actually shows
        // function hiding (the base class method gets hidden by the derived class method)

        void print_car_speed() {
            cout <<"car_speed : " <<endl;
        }
};


int main() {
    Vehicle v;
    Car c;
    v.print();  // Output : Vehicle
    c.print();  // Output : Car


    // pointer of a base class can point to address of it's derived class
    // In such cases, unless it's a virtual function, 
    // the pointer will access the members & methods defined in the base class
    // even if it points to a derived class
    // Error on accessing a variable / method not defined in the base class
    // a type of static polymorphism

    // static dispatch / early/no-virtual binding
    Vehicle *v1 = new Vehicle;
    Vehicle *v2;
    v2 = &c;
    v1 -> print();  // Output : Vehicle
    v2 -> print();  // Output : Vehicle
    // v2 -> print_car_speed();  // ‘class Vehicle’ has no member named ‘print_car_speed’
}

// Other common forms/examples of compile_time polymorphism:
// Function overloading ✓ — resolved at compile time based on argument types
// Operator overloading ✓ — same idea, just special syntax
// Templates ✓ — the most canonical example
