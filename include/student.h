#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <vector>
#include "managedObject.h"
#include "grade.h"

class Person {
    QString m_firstName;
    QString m_lastName;
    QString m_patronymic;
    std::vector<ManagedObject<Mark>> m_marks;
public:
    Person(const QString& firstName, const QString& lastName, const QString& patronymic)
        : m_firstName(firstName), m_lastName(lastName), m_patronymic(patronymic) {}
    
    void addGrade(ManagedObject<Mark> mark) {
        m_marks.push_back(std::move(mark));
    }
    
    QString getFirstName() const { 
        return m_firstName;
    }

    QString getLastName() const { 
        return m_lastName;
    }

    QString getPatronymic() const {
        return m_patronymic;
    }

    const std::vector<ManagedObject<Mark>>& getMarks() const {
        return m_marks;
    }
};

#endif // STUDENT_H
