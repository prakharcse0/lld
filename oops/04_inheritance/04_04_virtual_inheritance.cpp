// virtual_inheritance.cpp
// ------------------------------------------------------------
// This file demonstrates DIAMOND PROBLEM solved using
// VIRTUAL INHERITANCE.
//
// Key differences from hybrid_inheritance.cpp:
//
// 1. Only ONE Vehicle object exists.
// 2. Ambiguity disappears.
// 3. Most derived class constructs the virtual base.
// 4. Object layout is different (uses vbptr / vbtable).
//
// Structure:
//
//         Vehicle
//         /    \
//   virtual    virtual
//       Car    Truck
//         \    /
//           Bus
//
// Memory (conceptually):
//
// Bus
// ├── Car
// ├── Truck
// └── Vehicle (shared)
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

    Vehicle(int z) : maxSpeed(z), numTyres(0) {
        cout << "Vehicle constructor, maxSpeed = " << z << endl;
    }

    int max_speed() const {
        return maxSpeed;
    }

    // WHY virtual destructor?
    // If you ever delete a derived object through a base pointer:
    //
    //   Vehicle* v = new Bus();
    //   delete v;  // which destructor runs?
    //
    // Without virtual: destructor is resolved STATICALLY at compile time
    // based on pointer type (Vehicle*) → only ~Vehicle() runs.
    // Bus and Car and Truck destructors are SKIPPED → resource leaks.
    //
    // With virtual: destructor goes through the vtable (dynamic dispatch)
    // → correctly calls ~Bus() first, then ~Truck(), ~Car(), ~Vehicle()
    // in reverse construction order. No leaks.
    //
    // RULE: if a class is meant to be used polymorphically
    // (i.e., deleted via base pointer), its destructor MUST be virtual.
    // Presence of any other virtual function is a strong hint you need this.
    ~Vehicle() {
        cout << "Vehicle destructor" << endl;
    }
    // General rule: If a class has any virtual function, give it a virtual destructor. If it has no virtual functions, don't.
    // since the class doesn't have any virtual methods, its unlikely to be used like: Vehicle* v = new Bus()
    // No need of a virtual destructor whose cost would have been a vtable generated for each class: 
    // Every Vehicle (and Car, Truck, Bus) object would have gotten a hidden vptr added to it
};

class Car : virtual public Vehicle {
public:
    int numGears;

    // IMPORTANT RULE:
    // Even though we write Vehicle(3),
    // this will be IGNORED if a more derived class exists.
    //
    // The most derived class constructs the virtual base.
    Car() : Vehicle(3), numGears(5) {
        cout << "Car constructor" << endl;
        numTyres = 4;
    }

    ~Car() {
        cout << "Car destructor" << endl;
    }
};

class Truck : virtual public Vehicle {
public:
    Truck() : Vehicle(4) {
        cout << "Truck constructor" << endl;
        numTyres = 8;
    }

    void print_speed() {
        cout << "Truck sees max speed = " << max_speed() << endl;
    }

    ~Truck() {
        cout << "Truck destructor" << endl;
    }
};

class Bus : public Car, public Truck {
public:

    // CRUCIAL RULE:
    // The MOST DERIVED CLASS must construct the virtual base.
    //
    // If we remove Vehicle(5), compilation fails unless
    // Vehicle has a default constructor.
    // unlike in case of hybrid inheritance, where Bus didn't need to (& couldn't) construct a Vehicle
    Bus() : Vehicle(5) {
        cout << "Bus constructor" << endl;
    }

    ~Bus() {
        cout << "Bus destructor" << endl;
    }
};

int main() {

    cout << "---- Bus object ----\n";
    Bus b;
    // ---- Bus object ----
    // Vehicle constructor, maxSpeed = 5
    // Car constructor
    // Truck constructor
    // Bus constructor
    b.print_speed();
    // Truck sees max speed = 5

    // No ambiguity anymore
    b.color = "Yellow";

    cout << "Shared Vehicle color = " << b.color << endl;
    // Shared Vehicle color = Yellow

    // Only ONE Vehicle exists
    // Bus destructor
    // Truck destructor
    // Car destructor
    // Vehicle destructor


    // Note:
    // 1.
    // If bus was only inheriting from only one class, say Truck
    // which was still inheriting: class Truck : virtual public Vehicle
    // then the vehicle constructor would be calles by Bus, not Truck
    // The most derived class always initializes virtual bases — even if only one path exists.

    // 2.
    // If bus was still inheriting from both Truck & Car
    // Truck was still virtually inheriting from Vehicle:  class Truck : virtual public Vehicle
    // but Car was non-virtually inheriting from Vehicle:  class Car : public Vehicle
    // then one vehicle constructor would be calles by Bus, not Truck
    // and another by Car

    // The constructor order is determined by two rules:
    // Virtual base classes are constructed first (by the most derived class).
    // Then non-virtual base classes, in the order they appear in the class declaration.
    // Then the derived class itself.

    // So for Bus, the order is:
    // 1. Vehicle (the virtual base, constructed by Bus — for Truck's path)
    // 2. Vehicle (the one inside Car — non-virtual, Car constructs its own)
    // 3. Car
    // 4. Truck
    // 5. Bus

    return 0;
}

/*
CONSTRUCTION ORDER (virtual inheritance)

Virtual bases are constructed FIRST
by the most derived class.

Order for Bus:

Vehicle   <-- constructed by Bus
Car
Truck
Bus


DESTRUCTION ORDER

Bus
Truck
Car
Vehicle


VERY IMPORTANT RULE (people forget this):

If Bus constructor does NOT initialize Vehicle,
then Vehicle must have a default constructor.


Example that breaks compilation:

class Bus : public Car, public Truck {
public:
    Bus() { }  // ERROR
};

because Vehicle(int) exists but Vehicle() doesn't.


------------------------------------------------------------
ADVANCED TOPIC: MEMORY LAYOUT DIFFERENCE
------------------------------------------------------------

THE CORE PROBLEM virtual inheritance solves in memory:

Without virtual inheritance (normal diamond):
    Bus object in memory:
    [ Car subobject [ Vehicle copy 1 ] ]
    [ Truck subobject [ Vehicle copy 2 ] ]
    Two separate Vehicle copies → ambiguity + wasted memory.

With virtual inheritance:
    Bus object in memory:
    [ Car subobject  [ vbptr ] ]
    [ Truck subobject [ vbptr ] ]
    [ Vehicle subobject (ONE shared copy, at the END) ]

    Both Car's and Truck's vbptr point to this ONE Vehicle.


WHAT IS A vbptr (virtual base pointer)?

When you write: class Car : virtual public Vehicle

The compiler secretly adds a hidden pointer inside Car called vbptr.
This pointer points to a vbtable (virtual base table).
The vbtable says: "to find Vehicle, go +N bytes from here."
At runtime, Car (and Truck) follow their vbptr to locate the shared Vehicle.

This is similar to how virtual functions use a vptr → vtable,
but it's a completely separate mechanism for a completely different purpose.


WHY is Vehicle placed at the END of the Bus object?

Because Vehicle's exact offset inside Bus cannot be known
at Car or Truck compile time — it depends on how Bus is assembled.
So instead of hardcoding an offset, they use the vbptr indirection
to find it dynamically. This is the runtime cost of virtual inheritance.


CONCRETE MEMORY PICTURE (Bus b):

  [ Car part         ]  <- contains vbptr → vbtable → "Vehicle is at offset +X"
  [ Truck part       ]  <- contains vbptr → vbtable → "Vehicle is at offset +Y"
  [ Vehicle part     ]  <- the ONE shared Vehicle, lives here at the end


COST OF VIRTUAL INHERITANCE:

1. Extra hidden pointer (vbptr) added to each class that uses
   "virtual public SomeBase". Increases object size slightly.

2. Accessing Vehicle's members from Car or Truck requires
   one extra pointer dereference (through vbptr → vbtable).
   This is slightly slower than direct member access.

3. Constructor responsibility shifts to the most derived class,
   which can be surprising and easy to forget.


------------------------------------------------------------
vtable vs vbtable (important distinction)
------------------------------------------------------------

vtable:
Used for virtual functions.
Accessed via vptr (added when a class has any virtual function).

vbtable / vbptr:
Used for virtual inheritance.
Accessed via vbptr (added when a class inherits virtually).

These are completely separate mechanisms and serve different purposes.


Example:

class A {
    virtual void f();  // adds vptr → vtable
};

class B : virtual public A {};  // adds vbptr → vbtable

If a class has BOTH virtual functions AND virtual inheritance,
it has BOTH vptr and vbptr → object size grows further.
This is why deep, complex hierarchies with both features
can become large and slow.


------------------------------------------------------------
WHEN VIRTUAL INHERITANCE IS USED IN REAL SYSTEMS
------------------------------------------------------------

Rare, but important in:

1. iostream library
2. COM / component models
3. Some game engines
4. Multiple interface sharing


Example from the standard library:

ostream istream
   \   /
    iostream

Both inherit from ios virtually.


------------------------------------------------------------
INTERVIEW TRAP
------------------------------------------------------------

People think this calls Car's Vehicle constructor:

Bus b;

But actually this happens:

Vehicle constructed by Bus.

Car's Vehicle(3) is ignored.


This is a very common mistake.
*/