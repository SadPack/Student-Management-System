/*
 * Student Management System
 * ----------------------------------------
 * A console-based C++ project demonstrating core OOP concepts:
 *   - Abstraction   : Person is an abstract base class
 *   - Encapsulation : private data members, accessed via getters/setters
 *   - Inheritance   : Student derives from Person
 *   - Polymorphism  : virtual displayInfo() overridden in Student
 *
 * Compile: g++ -std=c++11 -O2 -o student_management student_management.cpp
 * Run:     ./student_management   (or student_management.exe on Windows)
 *
 * Data persists to "students.txt" in the same folder between runs.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

/* ===================== Abstraction + Encapsulation ===================== */
/* Person is an abstract base class -- you can never create a "Person"
 * directly, only its concrete subclasses (e.g. Student). */
class Person {
protected:
    string name;
    int age;

public:
    Person(string name, int age) : name(move(name)), age(age) {}
    virtual ~Person() = default;

    string getName() const { return name; }
    int getAge() const { return age; }

    void setName(const string &n) { name = n; }
    void setAge(int a) { age = a; }

    /* Pure virtual function -- forces every subclass to implement its
     * own version of displayInfo(). This is what makes Person abstract. */
    virtual void displayInfo() const = 0;

    /* Pure virtual serialization -- every subclass defines its own
     * file format. */
    virtual string toFileString() const = 0;
};

/* ===================== Inheritance + Polymorphism ===================== */
class Student : public Person {
private:
    int id;
    string course;
    double gpa;

public:
    Student(int id, string name, int age, string course, double gpa)
        : Person(move(name), age), id(id), course(move(course)), gpa(gpa) {}

    int getId() const { return id; }
    string getCourse() const { return course; }
    double getGpa() const { return gpa; }

    void setCourse(const string &c) { course = c; }
    void setGpa(double g) { gpa = g; }

    /* Overrides the pure virtual function from Person (polymorphism) */
    void displayInfo() const override {
        cout << left
             << setw(6)  << id
             << setw(18) << name
             << setw(5)  << age
             << setw(15) << course
             << setw(6)  << fixed << setprecision(2) << gpa
             << "\n";
    }

    string toFileString() const override {
        /* Simple pipe-delimited format for saving to disk */
        ostringstream oss;
        oss << id << "|" << name << "|" << age << "|" << course << "|" << gpa;
        return oss.str();
    }

    static Student fromFileString(const string &line) {
        stringstream ss(line);
        string idStr, name, ageStr, course, gpaStr;
        getline(ss, idStr, '|');
        getline(ss, name, '|');
        getline(ss, ageStr, '|');
        getline(ss, course, '|');
        getline(ss, gpaStr, '|');
        return Student(stoi(idStr), name, stoi(ageStr), course, stod(gpaStr));
    }
};

/* ===================== Manager class (handles the collection) ===================== */
class StudentManager {
private:
    vector<Student> students;
    const string filename = "students.txt";

    int generateNextId() const {
        int maxId = 0;
        for (const auto &s : students) maxId = max(maxId, s.getId());
        return maxId + 1;
    }

public:
    StudentManager() { loadFromFile(); }

    void addStudent(const string &name, int age, const string &course, double gpa) {
        int id = generateNextId();
        students.emplace_back(id, name, age, course, gpa);
        cout << "Student added with ID: " << id << "\n";
        saveToFile();
    }

    void displayAll() const {
        if (students.empty()) {
            cout << "No student records found.\n";
            return;
        }
        cout << "\n" << left
             << setw(6) << "ID" << setw(18) << "Name" << setw(5) << "Age"
             << setw(15) << "Course" << setw(6) << "GPA" << "\n";
        cout << string(50, '-') << "\n";
        for (const auto &s : students) {
            s.displayInfo(); /* polymorphic call */
        }
    }

    Student* findById(int id) {
        for (auto &s : students) {
            if (s.getId() == id) return &s;
        }
        return nullptr;
    }

    bool updateStudent(int id, const string &course, double gpa) {
        Student *s = findById(id);
        if (!s) return false;
        s->setCourse(course);
        s->setGpa(gpa);
        saveToFile();
        return true;
    }

    bool deleteStudent(int id) {
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getId() == id) {
                students.erase(it);
                saveToFile();
                return true;
            }
        }
        return false;
    }

    void searchByName(const string &query) const {
        bool found = false;
        cout << "\n" << left
             << setw(6) << "ID" << setw(18) << "Name" << setw(5) << "Age"
             << setw(15) << "Course" << setw(6) << "GPA" << "\n";
        cout << string(50, '-') << "\n";
        for (const auto &s : students) {
            if (s.getName().find(query) != string::npos) {
                s.displayInfo();
                found = true;
            }
        }
        if (!found) cout << "No matching student found.\n";
    }

    void saveToFile() const {
        ofstream out(filename);
        for (const auto &s : students) {
            out << s.toFileString() << "\n";
        }
    }

    void loadFromFile() {
        ifstream in(filename);
        if (!in) return; /* file may not exist yet on first run */
        string line;
        while (getline(in, line)) {
            if (!line.empty()) {
                students.push_back(Student::fromFileString(line));
            }
        }
    }
};

/* ===================== Console UI helpers ===================== */
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInputBuffer();
            return value;
        }
        cout << "Invalid input. Please enter a number.\n";
        clearInputBuffer();
    }
}

double readDouble(const string &prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInputBuffer();
            return value;
        }
        cout << "Invalid input. Please enter a number.\n";
        clearInputBuffer();
    }
}

string readLine(const string &prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

/* ===================== Main menu ===================== */
int main() {
    StudentManager manager;
    int choice;

    do {
        cout << "\n===== Student Management System =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student by Name\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Exit\n";
        choice = readInt("Enter your choice (1-6): ");

        switch (choice) {
            case 1: {
                string name = readLine("Enter name: ");
                int age = readInt("Enter age: ");
                string course = readLine("Enter course: ");
                double gpa = readDouble("Enter GPA: ");
                manager.addStudent(name, age, course, gpa);
                break;
            }
            case 2:
                manager.displayAll();
                break;
            case 3: {
                string query = readLine("Enter name to search: ");
                manager.searchByName(query);
                break;
            }
            case 4: {
                int id = readInt("Enter student ID to update: ");
                string course = readLine("Enter new course: ");
                double gpa = readDouble("Enter new GPA: ");
                if (manager.updateStudent(id, course, gpa)) {
                    cout << "Student updated successfully.\n";
                } else {
                    cout << "Student ID not found.\n";
                }
                break;
            }
            case 5: {
                int id = readInt("Enter student ID to delete: ");
                if (manager.deleteStudent(id)) {
                    cout << "Student deleted successfully.\n";
                } else {
                    cout << "Student ID not found.\n";
                }
                break;
            }
            case 6:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please select 1-6.\n";
        }
    } while (choice != 6);

    return 0;
}
