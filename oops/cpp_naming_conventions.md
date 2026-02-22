# C++ Naming Conventions (Concise Guide)


## General

-   Classes / Structs: `PascalCase`
-   Functions / Methods: `snake_case`
-   Private / Protected / Static member variables: `snake_case_`
-   Variables / Public members: `snake_case_`
-   Static member variables: `snake_case_`
-   Static member constants: `kConstantName`
-   Constants: `kConstantName`
-   Enum class → `PascalCase`
-   Enum values → `PascalCase` or `kStyle`
-   Macros: `ALL_CAPS_WITH_UNDERSCORES`
-   Files: `snake_case.cpp`, `snake_case.h`
-   Namespaces: `lowercase`


## Member Variables

-   Private / Protected: `snake_case_`
-   Public fields (if used): `snake_case`

Example:

``` cpp
class Student {
public:
    int roll;
private:
    int age_;
};
```


## Constructor Parameters

-   Same name as concept, no underscore.
-   Initialize via initializer list.

``` cpp
Student(int roll, int age) : roll_(roll), age_(age) {}
```


## Static Members

-   Static variables: `snake_case_`
-   Static constants: `kConstantName` or `CONSTANTNAME`

``` cpp
static int total_students_;
static constexpr int kMaxStudents = 100;    // prefer this
static constexpr int MAX_STUDENTS = 100;
```


## Methods

-   Member functions: `snake_case`
-   Getters / setters:
    -   `get_x()` / `set_x()`
    -   or `x()` (modern style)

``` cpp
int get_age() const;
void set_age(int age);
int age() const;    // prefer get_age()
```


## Constant

-   Static variables: `snake_case_`
-   Static constants: `kConstantName` or `CONSTANTNAME`

``` cpp
static int total_students_;
static constexpr int kMaxStudents = 100;    // prefer this
static constexpr int MAX_STUDENTS = 100;
```

### <U>C++: Defined Constants vs `const` Qualifiers</U>
### Use `kConstantName` for defined constants
Values that represent fixed program-level constants.

``` cpp
constexpr int kMaxConnections = 1000;
static constexpr int kDefaultTimeoutMs = 5000;
constexpr double kPi = 3.1415926535;
```

Typical uses: - Limits / thresholds - Configuration constants - Protocol
values - Compile-time constants - File / namespace constants - Static
class constants

### Do NOT use `k` for `const` qualifiers
When `const` only means the variable cannot change after initialization.

``` cpp
void process(const Request& request);

const int result = compute();
const int size = vec.size();
```

These are **not program constants**, just immutable variables.

### Quick Rule
    kConstantName → true constant definition
    snake_case    → normal or const-qualified variable
<b>Example</b>

``` cpp
class Server {
public:
    static constexpr int kMaxConnections = 10000;

    void handle_request(const Request& request) {
        const int request_size = request.size();

        if (request_size > kMaxConnections) {
            reject();
        }
    }

private:
    int active_connections_;
};
```


## Enums

-   Use `enum class`
-   Enum type: `PascalCase`
-   Values: `PascalCase` or `kStyle`

``` cpp
enum class Color {
    Red,
    Green,
    Blue
};

enum class Status {
    kOk,
    kError,
    kTimeout
};
```


## Template Parameters

-   `PascalCase` or single-letter (e.g., `T`, `Key`, `Value`)

``` cpp
template <typename T>
template <typename Key, typename Value>
```


## Global Variables (avoid if possible)

-   `g_variable_name`

``` cpp
int g_total_requests = 0;
```


## Constants

-   Preferred: `kConstantName`
-   Alternative: `ALL_CAPS`

``` cpp
constexpr int kBufferSize = 1024;
```


## Recommended Modern Style Summary

-   Class: `PascalCase`
-   Functions: `snake_case`
-   Constants: `kConstantName`
-   Files: `snake_case`
-   Namespaces: `lowercase`
