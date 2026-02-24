// ============================================================
// MASTER GUIDE: vbase ptr — EVERYTHING IN ONE PLACE
// ============================================================

#include <iostream>
using namespace std;

// ============================================================
// CHAPTER 1: THE PROBLEM — DIAMOND INHERITANCE
// ============================================================

// Without virtual inheritance:

class Animal_NV {              // NV = NonVirtual
public:
    int age = 5;
};

class Lion_NV : public Animal_NV {
public:
    int lion_data = 10;
};

class Tiger_NV : public Animal_NV {
public:
    int tiger_data = 20;
};

class Liger_NV : public Lion_NV, public Tiger_NV {
public:
    int liger_data = 30;
};

// Liger_NV layout:
//   [ Lion_NV::Animal_NV::age  ] 4   ← copy 1 of Animal
//   [ Lion_NV::lion_data       ] 4
//   [ Tiger_NV::Animal_NV::age ] 4   ← copy 2 of Animal — PROBLEM!
//   [ Tiger_NV::tiger_data     ] 4
//   [ liger_data               ] 4
//
// sizeof(Liger_NV) = 20
//
// TWO copies of Animal_NV inside Liger_NV.
// liger.age is AMBIGUOUS — compiler error.
// liger.Lion_NV::age and liger.Tiger_NV::age are different variables!
// This is the diamond problem.

// ============================================================
// CHAPTER 2: THE SOLUTION — VIRTUAL INHERITANCE
// ============================================================

class Animal {
public:
    int age = 5;
};

class Lion : virtual public Animal {    // virtual!
public:
    int lion_data = 10;
};

class Tiger : virtual public Animal {  // virtual!
public:
    int tiger_data = 20;
};

class Liger : public Lion, public Tiger {
public:
    int liger_data = 30;
};

// Now there is exactly ONE shared Animal subobject inside Liger.
// liger.age is unambiguous. ✓
//
// But HOW does Lion know where the shared Animal lives?
// Lion doesn't know — at compile time, Lion has no idea
// whether it's standalone or embedded inside Liger.
// The shared Animal could be at different offsets depending on context.
//
// This is the problem vbase ptr solves.

// ============================================================
// CHAPTER 3: WHAT IS A vbase ptr?
// ============================================================

// A vbase ptr is a hidden pointer injected into classes
// that use virtual inheritance.
//
// It stores the OFFSET (or address) of the shared virtual base
// subobject at runtime — because that offset can vary
// depending on the final most-derived class.
//
// Lion standalone:
//   [ vbase ptr ] → points to Animal subobject at offset X
//   [ lion_data ]
//   [ Animal    ]
//
// Lion embedded in Liger:
//   [ vbase ptr ] → points to Animal subobject at offset Y (different!)
//   [ lion_data ]
//   ...
//   [ Animal    ] ← shared, further away now
//
// The vbase ptr is updated at construction time to reflect
// the actual runtime location of the shared base.

// ============================================================
// CHAPTER 4: vbase ptr vs vptr — THEY ARE DIFFERENT
// ============================================================

//  vptr:
//    - exists because of VIRTUAL FUNCTIONS
//    - points to vtable
//    - enables runtime function dispatch
//    - one per independent base with virtual functions
//
//  vbase ptr:
//    - exists because of VIRTUAL INHERITANCE
//    - points to (or encodes offset of) shared base subobject
//    - enables correct access to shared base members
//    - one per virtually inherited base
//
//  They are completely independent.
//  A class can have:
//    - only vptr       (virtual functions, no virtual inheritance)
//    - only vbase ptr  (virtual inheritance, no virtual functions)
//    - both            (virtual functions AND virtual inheritance)
//    - neither         (plain class)

// ============================================================
// CHAPTER 5: SIZES — NO VIRTUAL FUNCTIONS, ONLY VIRTUAL INHERITANCE
// ============================================================

// Using Animal, Lion, Tiger, Liger from Chapter 2
// (no virtual functions anywhere)

// Animal:
//   [ age(4) ]
//   sizeof = 4
//   No vptr  — no virtual functions
//   No vbase — doesn't inherit anyone

// Lion:
//   [ vbase ptr(8) ] ← points to shared Animal
//   [ lion_data(4) ]
//   [ padding(4)   ]
//   [ Animal::age(4) ] ← shared Animal subobject placed at end
//   [ padding(4)   ]
//   sizeof = 24

// Tiger:
//   [ vbase ptr(8)   ]
//   [ tiger_data(4)  ]
//   [ padding(4)     ]
//   [ Animal::age(4) ]
//   [ padding(4)     ]
//   sizeof = 24

// Liger:
//   [ Lion's vbase ptr(8)  ] ← Lion subobject start
//   [ lion_data(4)         ]
//   [ padding(4)           ]
//   [ Tiger's vbase ptr(8) ] ← Tiger subobject start
//   [ tiger_data(4)        ]
//   [ liger_data(4)        ] ← fits in Tiger's padding
//   [ shared Animal::age(4)] ← ONE shared Animal at end
//   [ padding(4)           ]
//   sizeof = 40
//
// ONE Animal subobject shared between Lion and Tiger parts.
// Both vbase ptrs point to this same Animal.

// ============================================================
// CHAPTER 6: SIZES — BOTH virtual functions AND virtual inheritance
// ============================================================

class Base {
public:
    virtual void speak() { cout << "Base::speak\n"; }
    int base_data = 1;
};
// Layout:
//   [ vptr(8)      ] ← virtual functions → vptr
//   [ base_data(4) ]
//   [ padding(4)   ]
// sizeof = 16
// Has vptr, NO vbase ptr (inherits nobody)

class Left : virtual public Base {
public:
    void speak() override { cout << "Left::speak\n"; }
    int left_data = 2;
};
// Layout:
//   [ vptr(8)      ] ← for virtual functions
//   [ left_data(4) ]
//   [ padding(4)   ]
//   [ Base::vptr(8)] ← shared Base subobject (Base has virtual funcs so needs vptr)
//   [ base_data(4) ]
//   [ padding(4)   ]
// sizeof = 32
//
// NOTE: In GCC's Itanium ABI, the vbase offset is encoded INSIDE
// the vtable (in a slot before the function pointers).
// So Left's vptr does double duty:
//   → function dispatch (virtual functions)
//   → vbase location (virtual inheritance offset encoded in table)
// No separate visible vbase ptr field — it's folded into the vtable.

class Right : virtual public Base {
public:
    // speak() NOT overridden
    int right_data = 3;
};
// Layout:
//   [ vptr(8)       ]
//   [ right_data(4) ]
//   [ padding(4)    ]
//   [ Base::vptr(8) ]
//   [ base_data(4)  ]
//   [ padding(4)    ]
// sizeof = 32

class Child : public Left, public Right {
public:
    int child_data = 4;
};
// Layout:
//   [ Left's vptr(8)   ] ← Left subobject
//   [ left_data(4)     ]
//   [ padding(4)       ]
//   [ Right's vptr(8)  ] ← Right subobject
//   [ right_data(4)    ]
//   [ child_data(4)    ] ← packed into Right's padding
//   [ Base::vptr(8)    ] ← ONE shared Base subobject at end
//   [ base_data(4)     ]
//   [ padding(4)       ]
// sizeof = 48

// ============================================================
// CHAPTER 7: HOW vbase ptr IS STORED IN GCC (Itanium ABI)
// ============================================================

// In GCC, there is no separate raw pointer field for vbase.
// Instead, the vtable is extended with extra slots BEFORE
// the function pointer slots:
//
//  Left's vtable (full structure):
//  ┌─────────────────────────────────────┐
//  │ offset_to_top   (how far vptr is    │  ← used for dynamic_cast
//  │                  from object start) │
//  ├─────────────────────────────────────┤
//  │ RTTI pointer    (type_info*)        │  ← used for typeid
//  ├─────────────────────────────────────┤  ← vptr points HERE
//  │ slot[0] → &Left::speak             │  ← virtual function slots
//  ├─────────────────────────────────────┤
//  │ vbase_offset    (how far Base is    │  ← virtual base offset
//  │                  from this vptr)    │
//  └─────────────────────────────────────┘
//
// When Left needs to access Base members:
//   1. Load vptr
//   2. Read vbase_offset from vtable (at a known negative index)
//   3. Add offset to current `this`
//   4. Now pointing at Base subobject
//
// This is why "vbase ptr" is sometimes called a "virtual base offset"
// in GCC — it's not a raw pointer in the object, it's an offset
// stored in the vtable and looked up via the existing vptr.

// ============================================================
// CHAPTER 8: WHAT IF THERE ARE NO VIRTUAL FUNCTIONS?
// ============================================================

// If a class uses virtual inheritance but has NO virtual functions,
// there is NO vptr to piggyback on.
// So the compiler injects an EXPLICIT vbase ptr field into the object.
//
// That's why Lion (from Chapter 2, no virtual functions) has:
//   [ vbase ptr ] ← explicit raw pointer, not folded into vtable
//   [ lion_data ]
//   [ Animal    ]
//
// And Left (from Chapter 6, has virtual functions) has:
//   [ vptr      ] ← vbase offset folded INTO vtable, no extra field
//   [ left_data ]
//   [ Base      ]
//
// In both cases the MECHANISM is the same — locate shared base at runtime.
// The IMPLEMENTATION differs based on whether a vptr already exists.

// ============================================================
// CHAPTER 9: VERIFY WITH CODE
// ============================================================

int main() {
    cout << "=== Chapter 1: Diamond without virtual inheritance ===\n";
    cout << "sizeof(Animal_NV): " << sizeof(Animal_NV) << "\n";  // 4
    cout << "sizeof(Lion_NV):   " << sizeof(Lion_NV)   << "\n";  // 8
    cout << "sizeof(Tiger_NV):  " << sizeof(Tiger_NV)  << "\n";  // 8
    cout << "sizeof(Liger_NV):  " << sizeof(Liger_NV)  << "\n";  // 20 — two Animal copies

    cout << "\n=== Chapter 5: Virtual inheritance, no virtual functions ===\n";
    cout << "sizeof(Animal): " << sizeof(Animal) << "\n";  // 4
    cout << "sizeof(Lion):   " << sizeof(Lion)   << "\n";  // 24 — vbase ptr added
    cout << "sizeof(Tiger):  " << sizeof(Tiger)  << "\n";  // 24
    cout << "sizeof(Liger):  " << sizeof(Liger)  << "\n";  // 40 — one shared Animal

    cout << "\n=== Chapter 6: Virtual inheritance + virtual functions ===\n";
    cout << "sizeof(Base):  " << sizeof(Base)  << "\n";  // 16
    cout << "sizeof(Left):  " << sizeof(Left)  << "\n";  // 32
    cout << "sizeof(Right): " << sizeof(Right) << "\n";  // 32
    cout << "sizeof(Child): " << sizeof(Child) << "\n";  // 48

    cout << "\n=== Shared base in action ===\n";
    Liger liger;
    liger.age = 99;                    // unambiguous — only one Animal ✓
    cout << "Lion  part sees age: " << liger.Lion::age  << "\n";  // 99
    cout << "Tiger part sees age: " << liger.Tiger::age << "\n";  // 99 — SAME object

    cout << "\n=== Virtual dispatch through virtual inheritance ===\n";
    Child child;
    Base* bp = &child;
    bp->speak();  // Left::speak — Left is the final overrider

    return 0;
}

// ============================================================
// MASTER SUMMARY
// ============================================================
//
// vbase ptr:
//   - exists because of VIRTUAL INHERITANCE
//   - locates the shared virtual base subobject at runtime
//   - necessary because the base's offset varies by most-derived class
//   - if NO virtual functions: explicit pointer field in object
//   - if virtual functions exist: folded into vtable as vbase_offset
//     (GCC Itanium ABI), looked up via existing vptr
//
// diamond problem:
//   - without virtual: N bases each carry own copy → ambiguity
//   - with virtual: one shared subobject, located via vbase ptr
//
// sizing impact:
//   - virtual inheritance (no virtuals): +8 bytes per virtually inherited base
//   - virtual inheritance (with virtuals): no extra field, offset in vtable
//   - shared base appears ONCE at end of most-derived object
//
// construction order with virtual inheritance:
//   - most-derived class constructs the virtual base FIRST
//   - then Left, Right, then Child itself
//   - ensures there's exactly one Animal constructed