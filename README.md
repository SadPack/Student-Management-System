# Student Management System (C++)

A console-based Student Management System built in C++ to demonstrate 
core Object-Oriented Programming concepts.

## OOP Concepts Demonstrated

| Concept | Implementation |
|---|---|
| Abstraction | `Person` is an abstract base class with pure virtual functions |
| Encapsulation | All data members are private/protected, accessed via getters/setters |
| Inheritance | `Student` inherits from `Person` |
| Polymorphism | `displayInfo()` is overridden and called virtually |

## Features
- Add, display, search, update, and delete student records
- Auto-incrementing student IDs
- Data persists to `students.txt` between runs
- Input validation for numeric fields
