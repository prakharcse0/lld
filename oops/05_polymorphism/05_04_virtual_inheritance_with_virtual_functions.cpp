// diamond_virtual_functions.cpp
// ------------------------------------------------------------
// This file demonstrates:
//
// 1) Diamond inheritance
// 2) Virtual inheritance (only ONE Base subobject exists)
// 3) Virtual functions
// 4) Final overrider rule
// 5) Ambiguity when multiple parents override
// 6) How the most derived class resolves ambiguity
//
// IMPORTANT CONCEPTS:
//
// Diamond structure:
//
//           Base
//          /    \
//   (virtual)  (virtual)
//        Left     Right
//          \      /
//            Child
//
// Without virtual inheritance:
// Child would contain TWO Base objects.
//
// With virtual inheritance:
// Child contains ONE shared Base object.
//
// Internally compilers typically use:
// - vptr  -> for virtual function dispatch
// - vbptr -> to locate the shared virtual base
//
// ------------------------------------------------------------

#include <iostream>
using namespace std;

// ============================================================
// BASE CLASS
// ============================================================

class Base {
public:
    int base_data = 100;

    // Virtual function
    // This allows runtime polymorphism.
    virtual void speak() {
        cout << "Base::speak\n";
    }

    virtual void identify() {
        cout << "I am Base\n";
    }
};

// ============================================================
// LEFT BRANCH
// ============================================================

class Left : virtual public Base {
public:
    int left_data = 200;

    // Override speak()
    // This becomes a candidate for FINAL OVERRIDER.
    void speak() override {
        cout << "Left::speak\n";
    }
};

// ============================================================
// RIGHT BRANCH
// ============================================================

class Right : virtual public Base {
public:
    int right_data = 300;

    // Notice:
    // We DO NOT override speak() here.
    // This means Right simply inherits Base::speak.
};

// ============================================================
// CHILD CLASS
// ============================================================

class Child : public Left, public Right {
public:
    int child_data = 400;

    // IMPORTANT:
    // We are NOT overriding speak() here.
    //
    // So what happens?
    //
    // Left overrides speak()
    // Right does not.
    //
    // Therefore:
    // Left::speak becomes the FINAL OVERRIDER
    // for objects of type Child.
};

// ============================================================
// CASE 2: BOTH CLASSES OVERRIDE (AMBIGUITY)
// ============================================================

class Left2 : virtual public Base {
public:
    void speak() override {
        cout << "Left2::speak\n";
    }
};

class Right2 : virtual public Base {
public:
    void speak() override {
        cout << "Right2::speak\n";
    }
};

// If Child2 does NOT override speak():
// The compiler will produce an error:
//
// "no unique final overrider"
//
// Because BOTH Left2 and Right2 override speak().

class Child2 : public Left2, public Right2 {
public:
    // To fix ambiguity, the most derived class
    // must provide the final overrider.

    void speak() override {
        cout << "Child2 resolves ambiguity -> ";
        Left2::speak(); // we choose Left2
    }
};

// ============================================================
// CASE 3: NEITHER BRANCH OVERRIDES
// ============================================================

class Left3 : virtual public Base {};
class Right3 : virtual public Base {};

class Child3 : public Left3, public Right3 {
    // No overrides anywhere.
    // Final overrider = Base::speak
};

// ============================================================
// MAIN
// ============================================================

int main() {

    // Case 1: Only Left overrides\n

    Child c;

    // Even though Child inherits from two classes,
    // only ONE Base exists because of virtual inheritance.
    //
    // Layout conceptually:
    //
    // Child
    //   Left
    //   Right
    //   Base (shared)

    Base* b = &c;

    // Virtual dispatch occurs.
    // Final overrider = Left::speak
    b->speak();
    // Left::speak

    c.speak(); // also calls Left::speak
    // Left::speak


    // Case 2: Both branches override
    Child2 c2;
    Base* b2 = &c2;
    // Child2 resolves ambiguity
    b2->speak();
    // Child2 resolves ambiguity -> Left2::speak


    // Case 3: No overrides
    // No class overrides speak()
    Child3 c3;
    Base* b3 = &c3;
    b3->speak();
    // Base::speak


    // Memory / object insight

    cout << "Size of Base  : " << sizeof(Base) << "\n";
    cout << "Size of Left  : " << sizeof(Left) << "\n";
    cout << "Size of Right : " << sizeof(Right) << "\n";
    cout << "Size of Child : " << sizeof(Child) << "\n";

    // Size of Base  : 16
    // Size of Left  : 32
    // Size of Right : 32
    // Size of Child : 48

    // Sizes may surprise you because:
    //
    // Virtual inheritance requires extra pointers:
    // - vbptr (virtual base pointer)
    // - vptr  (virtual function table pointer)
    //
    // Exact layout depends on compiler.


    // Base — 16 bytes
    // vptr (8) — for virtual functions speak(), identify()
    // base_data int (4) + padding (4)
    // No vbase ptr — Base doesn't virtually inherit anything.

    // Left — 32 bytes
    // vptr (8) — for its own vtable (overrides speak())
    // left_data (4) + padding (4)
    // shared virtual Base subobject: base_data (4) + padding (4) ... but where's Base's vptr?
    // Actually the shared Base subobject's vptr gets merged into Left's vtable in practice — GCC folds the virtual base's vptr into the most-derived class's vtable structure. So:
    // Left's own vptr (8) + left_data (4) + pad (4) = 16
    // vbase ptr is encoded inside the vtable, not a separate pointer on Left
    // shared Base: base_data (4) + pad (4) = 8
    // Total: 32

    // Right — 32 bytes
    // Same reasoning. Right has:
    // its own vptr (8) + right_data (4) + pad (4) = 16
    // shared Base: base_data (4) + pad (4) = 8
    // vbase offset encoded in vtable
    // Total: 32

    // Child — 48 bytes
    // Left subobject:  vptr + left_data + pad16
    // Right subobject: vptr + right_data + pad16
    // Shared Base:     base_data + pad8
    // child_data:      packed into Base padding
    // Total: 48


    return 0;
}