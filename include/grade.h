#ifndef GRADE_H
#define GRADE_H

#include <QDate>
#include "managedObject.h"
#include "subject.h"

class Mark {
    int value;
    QDate markDate;
    ManagedObject<Discipline> m_disciplines;
public:
    Mark(int value, const QDate& date, ManagedObject<Discipline>&& disciplines)
        : value(value), markDate(date), m_disciplines(std::move(disciplines)) {}

    int getValue() const { 
        return value; 
    }

    QDate getMarkDate() const {
        return markDate;
    }

    const ManagedObject<Discipline>& getDisciplines() const {
        return m_disciplines;
    }
};

#endif // GRADE_H
