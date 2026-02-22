#include <iostream>
using namespace std;

// Friend classes & friend functions of a class can access ALL members
// (private, protected, public) of the befriended class.

// Where you declare the friendship (public/private/protected) makes no difference.

// Friendship is NOT mutual:
//   If Truck befriends Bus, Bus can access Truck's members,
//   but Truck cannot access Bus's members unless Bus explicitly befriends Truck too.

// Friendship is NOT inherited:
//   If Car inherits from Bus, Car does NOT automatically get access to Truck's private members.

// Friendship is a break in encapsulation — use sparingly.

// Friend functions/classes must access members via an explicit object instance.
// There is no implicit 'this' for the befriended class.
// e.g. inside Bus::print(), you can't just write x = 10,
//      you need an explicit Truck object: Truck t; t.x = 10;

// Declaration rules:
//   - Friend global functions (e.g. void test()) need not be declared before being tagged friend.
//   - Friend classes (e.g. friend class Bus) need not be declared before being tagged friend.
//   - A specific method of another class (e.g. void Bus::print()) MUST have its class
//     declared before being tagged friend, so the compiler knows the method exists.


// Bus must be declared first because Truck tags a specific method: Bus::print()
class Bus {
    public:
        void print();
};

// Example of a friend class
class Bike {
    public:
        void show();
};

class Truck {
    private:
        int x;
    protected:
        int y;
    public:
        int z;

        // Specific method of Bus tagged as friend — Bus must be declared above
        friend void Bus::print();

        // Global function tagged as friend — no prior declaration needed
        friend void test();

        // Entire class tagged as friend — no prior declaration needed
        friend class Bike;
};

void Bus::print() {
    Truck t;
    t.x = 10;  // private — accessible because Bus::print() is a friend
    t.y = 20;  // protected — accessible because Bus::print() is a friend
    cout << t.x << " " << t.y << endl;
}

void test() {
    Truck t;
    t.x = 10;  // private — accessible because test() is a friend
    t.y = 20;  // protected — accessible because test() is a friend
    cout << t.x << " " << t.y << endl;
}

void Bike::show() {
    Truck t;
    t.x = 10;  // private — accessible because Bike is a friend class
    t.y = 20;  // protected — accessible because Bike is a friend class
    cout << t.x << " " << t.y << endl;
}

int main() {
    Bus b;
    b.print();  // Output: 10 20

    test();     // Output: 10 20

    Bike bk;
    bk.show();  // Output: 10 20
}