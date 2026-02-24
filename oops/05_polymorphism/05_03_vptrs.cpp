// ============================================================
// COMPREHENSIVE GUIDE TO vptr
// Everything explained through code and comments
// ============================================================

#include <iostream>
using namespace std;

// ============================================================
// PART 1: WHY DOES vptr EXIST?
// ============================================================

// Without virtual — no vptr, no runtime dispatch
class AnimalStatic {
public:
    void speak() { cout << "AnimalStatic::speak\n"; }  // regular function
    int age = 5;
};

class DogStatic : public AnimalStatic {
public:
    void speak() { cout << "DogStatic::speak\n"; }  // hides, does NOT override
};

// With virtual — vptr is injected
class Animal {
public:
    virtual void speak() { cout << "Animal::speak\n"; }  // virtual!
    int age = 5;
};

class Dog : public Animal {
public:
    void speak() override { cout << "Dog::speak\n"; }
};

// ============================================================
// PART 2: WHAT DOES vptr POINT TO? (vtable)
//
// The compiler creates a STATIC table per class — the vtable.
// It's an array of function pointers, one per virtual function.
//
// Animal's vtable:
//   slot[0] -> &Animal::speak
//
// Dog's vtable:
//   slot[0] -> &Dog::speak     <- overridden
//
// Every OBJECT gets a hidden vptr injected at the START.
// It points to its CLASS's vtable.
//
// Animal object layout:
//   [ vptr ] -> Animal's vtable
//   [ age  ]
//
// Dog object layout:
//   [ vptr ] -> Dog's vtable   <- different table!
//   [ age  ]
// ============================================================

// ============================================================
// PART 3: SIZES
// ============================================================

class NoVirtual {
    int x;       // 4 bytes
    int y;       // 4 bytes
};
// sizeof = 8 — no vptr, no overhead

class OneVirtual {
    virtual void f();   // triggers vptr injection
    int x;              // 4 bytes
};
// Layout:
//   [ vptr ] 8 bytes
//   [ x    ] 4 bytes
//   [ pad  ] 4 bytes (align to 8)
// sizeof = 16

class TwoVirtuals {
    virtual void f();
    virtual void g();   // DOES NOT add another vptr
    int x;
};
// Layout:
//   [ vptr ] 8 bytes  <- STILL just one vptr
//   [ x    ] 4 bytes
//   [ pad  ] 4 bytes
// sizeof = 16
//
// vtable has TWO slots now, but the object still has ONE vptr.
// More virtual functions = bigger vtable, NOT more vptrs.

class TenVirtuals {
    virtual void a(); virtual void b(); virtual void c();
    virtual void d(); virtual void e(); virtual void f();
    virtual void g(); virtual void h(); virtual void i();
    virtual void j();
    int x;
};
// sizeof = 16 — still! one vptr regardless of how many virtuals

// ============================================================
// PART 4: INHERITANCE AND vptr
// ============================================================

class Base {
public:
    virtual void speak()    { cout << "Base::speak\n"; }
    virtual void identify() { cout << "Base::identify\n"; }
    int base_data = 1;
};
// Layout:
//   [ vptr      ] 8  -> Base's vtable: [&Base::speak, &Base::identify]
//   [ base_data ] 4
//   [ padding   ] 4
// sizeof = 16

class Derived : public Base {
public:
    void speak() override { cout << "Derived::speak\n"; }
    // identify() NOT overridden
    int derived_data = 2;
};
// Layout:
//   [ vptr        ] 8  -> Derived's vtable: [&Derived::speak, &Base::identify]
//   [ base_data   ] 4  <- inherited from Base
//   [ padding     ] 4
//   [ derived_data] 4
//   [ padding     ] 4
// sizeof = 24
//
// IMPORTANT:
// Derived does NOT get a NEW vptr.
// It REUSES the one from Base (at the same offset).
// The vptr just points to a DIFFERENT vtable now.

// ============================================================
// PART 5: THE MAGIC — HOW RUNTIME DISPATCH ACTUALLY WORKS
// ============================================================
//
// Base* ptr = new Derived();
// ptr->speak();
//
// Compiled to something like:
//   1. Load vptr from *ptr        (always at offset 0)
//   2. Look up slot[0] in vtable  (speak is slot 0)
//   3. Call whatever is there
//
// Since ptr actually points to a Derived object,
// its vptr -> Derived's vtable,
// slot[0] -> &Derived::speak
//
// This is why it prints "Derived::speak" even through a Base pointer.
// The TYPE of the pointer doesn't matter — the vptr in the object does.

// ============================================================
// PART 6: MULTIPLE INHERITANCE — TWO vptrs
// ============================================================

class A {
public:
    virtual void fa() { cout << "A::fa\n"; }
    int a_data = 10;
};
// sizeof = 16 (vptr + data + pad)

class B {
public:
    virtual void fb() { cout << "B::fb\n"; }
    int b_data = 20;
};
// sizeof = 16

class C : public A, public B {
public:
    void fa() override { cout << "C::fa\n"; }
    void fb() override { cout << "C::fb\n"; }
    int c_data = 30;
};
// Layout:
//   [ vptr_A  ] 8  -> C's vtable for A-part: [&C::fa]
//   [ a_data  ] 4
//   [ padding ] 4
//   [ vptr_B  ] 8  -> C's vtable for B-part: [&C::fb]
//   [ b_data  ] 4
//   [ padding ] 4
//   [ c_data  ] 4
//   [ padding ] 4
// sizeof = 40
//
// TWO vptrs because C inherits from TWO unrelated bases.
// Each base introduced its own vptr, and C carries both.
// When you cast C* to B*, the pointer shifts to the B-subobject,
// so B's vptr is correctly accessible from there.

// ============================================================
// PART 7: VERIFY EVERYTHING WITH sizeof
// ============================================================

int main() {
    cout << "=== PART 3: Basic sizes ===\n";
    cout << "NoVirtual:   " << sizeof(NoVirtual)   << "\n";  // 8
    cout << "OneVirtual:  " << sizeof(OneVirtual)  << "\n";  // 16
    cout << "TwoVirtuals: " << sizeof(TwoVirtuals) << "\n";  // 16 — same!
    cout << "TenVirtuals: " << sizeof(TenVirtuals) << "\n";  // 16 — same!

    cout << "\n=== PART 4: Inheritance sizes ===\n";
    cout << "Base:    " << sizeof(Base)    << "\n";  // 16
    cout << "Derived: " << sizeof(Derived) << "\n";  // 24

    cout << "\n=== PART 6: Multiple inheritance sizes ===\n";
    cout << "A: " << sizeof(A) << "\n";  // 16
    cout << "B: " << sizeof(B) << "\n";  // 16
    cout << "C: " << sizeof(C) << "\n";  // 40

    cout << "\n=== PART 1: Static vs virtual dispatch ===\n";
    AnimalStatic* s = new DogStatic();
    s->speak();  // AnimalStatic::speak — pointer type decides (static)

    Animal* a = new Dog();
    a->speak();  // Dog::speak — vptr decides (dynamic)

    cout << "\n=== PART 5: vptr in action ===\n";
    Base* b = new Derived();
    b->speak();    // Derived::speak — vptr points to Derived's table
    b->identify(); // Base::identify — Derived didn't override this

    cout << "\n=== PART 6: Multiple inheritance dispatch ===\n";
    C obj;
    A* ap = &obj;
    B* bp = &obj;
    ap->fa();  // C::fa
    bp->fb();  // C::fb

    return 0;
}

// ============================================================
// FINAL SUMMARY (as comments)
// ============================================================
//
// vptr:
//   - injected by compiler into EVERY object of a class that has virtual functions
//   - always at offset 0 (start of object)
//   - 8 bytes on 64-bit systems
//   - points to the class's vtable
//   - ONE per base class in the hierarchy (multiple inheritance = multiple vptrs)
//   - NOT duplicated for multiple virtual functions (still one vptr, bigger table)
//
// vtable:
//   - static, per-class array of function pointers
//   - one slot per virtual function
//   - created by compiler, exists once in binary (not per object)
//   - derived class gets its OWN vtable, with overridden slots replaced
//
// Rule of thumb for sizing:
//   sizeof(class) = sizeof(vptr) * num_independent_bases_with_virtuals
//                 + sizeof(all data members)
//                 + padding for alignment