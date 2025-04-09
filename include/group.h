#ifndef GROUP_H
#define GROUP_H

#include <QString>
#include <vector>
#include "managedObject.h"
#include "student.h"

class Group {
    QString name;
    std::vector<ManagedObject<Person>> students;
public:
    explicit Group(const QString& name) : name(name) {}
    
    void addStudent(ManagedObject<Person> student) {
        students.push_back(std::move(student));
    }
    
    QString getName() const { 
        return name; 
    }

    const std::vector<ManagedObject<Person>>& getStudents() const {
        return students; 
    }
};

#endif // GROUP_H
