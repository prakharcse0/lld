#include <iostream>
#include <string>

class Teacher {
    public : 
        std::string name;
        int age;

        void print() {
            std::cout << "Teacher" <<' ' << name <<std::endl;
        }
};

class Student {
    public :
        std::string name;

        void print() {
            std::cout << "Student" <<' ' << name << std::endl;
        }
};

class TA : public Teacher, public Student {

    public :
        void print() {
            std::cout << "TA" << std::endl;
            // std::cout << "TA" <<name <<std::endl;   // error: reference to ‘name’ is ambiguous
        }
};


int main() {
    TA a;

    // If multiple base classes contain members with the same name,
    // we must specify which base class we want using the scope resolution operator ::

    a.Teacher::name = "abcd";
    a.Student::name = "xyz";
    // a.name = "abcd"; // ERROR: ambiguous

    a.print();           // calls TA::print()   // method overriding: TA::print() overrides the base versions
    a.Teacher::print();  // explicitly call Teacher version
    a.Student::print();  // explicitly call Student version
    // output:
    // TA
    // Teacher abcd
    // Student xyz
}