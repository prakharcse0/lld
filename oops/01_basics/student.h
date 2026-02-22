// constructors, destructors, copy-constructors
// getters, setters
// intitalisation list, constant functions
// static members, static functions

#include <iostream>
#include <string>

class Student {

private:
    const int roll_;
    std::string name_;
    int age_;
    // int &age_reference_;
    static int total_students_; // a static variable  // for a class, not objects 
    // correct way to call : std::cout <<Student::total_students <<'\n';
    static const int annswer_to_the_life_universe_and_everything_ = 42;
    // static const std::string str;   // NOT ok inline pre-C++17, needs out-of-class definition

    // couldn't be initialised inside the class pre-cpp17 for non-cosnt vars // automatically assigned 0
    // however if U dont define it outside the class,
    // it creates a compile-time constant, but not necessarily a real variable in memory.
    // so can't do something like: std::cout << &my_class::count;
    // to do so, U must first do: const int my_class::count; outide the class to define it

    // static vars could be initialises outside, 
    // but not a good idea to intialise them in the .h files they are defined
    // why ?
    // ex:
    // U do Students::total_students_ = 20 in this file
    // 2 other .cpp files do include "student.h"
    // During linking: multiple definitions of my_class::count : error
    // One Definition Rule (ODR)


    // since c==17, could be initialised inside the class using inline: 
    inline static int rand_non_const_stat = 8;  // use thissss methodddd
    // Inline:
    // Allows a variable to be defined in a header without violating the ODR.
    // The linker merges all copies into one — one address, one value.

    // --- Rules ---
    // 1. Must have the same definition in every translation unit (UB if not)
    // 2. constexpr variables at namespace scope are implicitly inline (C++17)
    // 3. Guaranteed to have a unique address across all TUs
    // 4. Initialization order across TUs is still unspecified (same as always)


public:
    // 4 inbuilt functions we get with all classes: 
    // constructors, copy-constructor, copy-assignment operator(=), destrucor

    Student(Student&&) = default;  // move constructor // explicitly compiler-generated // good for documentation/clarity
    Student& operator=(const Student&) = delete;    // deleting the impplicit copy-assignment operator without defining custom one
    Student& operator=(Student&&) = delete;       // Move assignment operator


    // Initialisation list  
    // allows to initialise constant/reference variables 
    // Initialises the variables, 
    // instead of declaration then assignment that would happen otherwise
    // defining a constructor supresses the defaul one
    // Members initialize in the order declared in the class, NOT initializer list order.
    // Always write initializer list in the same order as declaration.
    Student(int roll, std::string name, int age) : roll_(roll), name_(std::move(name)), age_(age) {// this->age_(age_reference) : // how to initialise reference variables using intialisation list
        // just to show how, but generally do this in the initalisation list only, and dont repeat inside  :
        this->name_ = name; // but generally do this in the initalisation list only, and noo need to do it again inside
        this->set_name(name); // cons: adds logs / potentially slower; // pros: does validations if required
        // b/w the above 2, usually ise the 1st for method internals, but choose according to the situation
        total_students_++;
    }
    // Here cost of string initialisation: lvalue: 1 copy + 1move; rvalue : 1–2 moves

    // Alterntive:
    // Student(int roll, const std::string& name, int age) : roll_(roll), name_(name), age_(age) {}
    // cost of string initialisation: lvalue: 1 copy; rvalue : 1 copy 
    // the first constructor(the one using move)
    // U can pass rvalues to the string despite it taking reference because of its constness:
    // const reference extends lifetime and binds to rvalues
    // const T&  → binds to lvalues AND rvalues
    // T&  → binds to lvalues ONLY
    // can pass string literals, char *, anything implicitly convertible to strings here in the string arg


    // Another paramaterised constructor
    explicit Student(int roll) : roll_(roll) {
        // what does explicit do ?

        // without explicit:
        // Student s = 10;  // is valid // implicit conversion from int → Student
        // cpp converts the same to: Student s = Student(10);
        // dangerous

        // with explicit:
        // Student s = 10;   // error
    }

    // Copy constructor  
    // inbuilt copy-constructor alsways does shallow copy 
    // just to show, but dont define unless needed
    Student(Student const &s) : roll_(s.get_roll()), age_(s.get_age()) {
        this->name_ = s.name_; // Access control in C++ is per-class, not per-object:
        // Any member function of Student can access private members of any Student object.
        total_students_++;
    }


    // Destructor
    ~Student(){
        // destructors, constructors needn't and couldn't be declared const
        // ON ~Student() const : error: destructors may not be cv-qualified 
    }
    // don't define constructors / destructors unless required, let compiler do most of the work
    // see rule of 0, 3, 5


    // THE RULE OF 0, 3, 5
    //  
    // All about: who manages resources (heap memory, file handles, sockets...)
    // The "special members" in question:
    //   - Destructor
    //   - Copy constructor
    //   - Copy assignment operator
    //   - Move constructor          (C++11)
    //   - Move assignment operator  (C++11)

    // RULE OF ZERO — preferred, aim for this
    // ============================================================
    // Don't define ANY of the 5 special members.
    // Let the compiler generate them all.
    // Achieved by using RAII types that manage themselves.

    // RULE OF THREE — pre-C++11 era
    // ============================================================
    // If you manually define ANY ONE of these three, define ALL THREE:
    //   - Destructor
    //   - Copy constructor
    //   - Copy assignment operator

    // RULE OF FIVE — C++11 and beyond
    // ============================================================
    // Extends rule of three: if you define any one, define ALL FIVE.
    // Adding move constructor + move assignment enables efficient transfers.


    void display() const{ // a const function  
        // only const functions, constructors & deconstructors 
        // could be called by constant variables
        std::cout <<roll_ <<'\t' <<name_ <<'\t' <<age_ <<'\n';
    }


    //setters
    void set_name(const std::string &name) {
        this->name_ = name;
    }

    void set_age(const int age) {
        if(age<0)
            return;
        this->age_ = age;
    }


    //getters
    int get_age() const{
        // Only const member functions can be called on const objects.
        return this->age_;
    }

    const std::string &get_name() const{
        return this->name_;
        // No copy — returns reference to the internal member.
        // Caller reads directly from the object's memory.
        // BUT: lifetime is tied to the object!

        // use as reference
        // const std::string& name = s1.get_name();  // just an alias, no copy
                                               // dangling if student dies

        // std::string &name = Student(1, "John", 20).get_name(); // dangerous
        // std::string name = Student(1, "John", 20).get_name(); // copy-constructed from the ref //safe, even though object does instantly
    }

    int get_roll() const{
        return this->roll_;
    }

    static int get_total_students() { // static function  
        return total_students_;
        // how to use : std::cout <<Student::Total_students() <<'\n';
    }

};

// int Student :: total_students = 0; // initializes static data member // can'if not initialised inside // dont do this in a .h file 
