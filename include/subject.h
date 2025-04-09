#ifndef SUBJECT_H
#define SUBJECT_H

#include <QString>

class Discipline {
    QString name;
public:
    explicit Discipline(const QString& name) : name(name) {}
    QString getName() const { return name; }
};

#endif // SUBJECT_H
