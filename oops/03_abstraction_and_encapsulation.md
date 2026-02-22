## Encapsulation
**Encapsulation** means bundling data and methods together in a class and **controlling access** to the data.

- Implemented using: `private`, `protected`, `public`
- Prevents direct modification of internal state
- Ensures data is modified only through defined methods (maintains class invariants)

Example idea:
- Data is hidden and accessed through getters/setters.

**Summary:**  
Encapsulation = **data hiding + access control**

---

## Abstraction
**Abstraction** means exposing **what an object does** while hiding **how it works internally**.

- Users interact with a simple interface
- Internal implementation details remain hidden

Example idea:
- `f1 + f2` works without the user needing to know how fraction addition is implemented internally.

**Summary:**  
Abstraction = **hide complexity, expose functionality**

---

## Relationship
Encapsulation is a **mechanism** that helps achieve abstraction.

**One-line takeaway:**
Encapsulation protects data; abstraction hides complexity.