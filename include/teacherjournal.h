#ifndef TEACHERJOURNAL_H
#define TEACHERJOURNAL_H

#include <vector>
#include "managedObject.h"
#include "group.h"
#include "subject.h"

class TeacherJournal {
    std::vector<ManagedObject<Group>> groups;
    std::vector<ManagedObject<Discipline>> disciplines;
    
public:
    void addGroup(ManagedObject<Group> group);
    void addDiscipline(ManagedObject<Discipline> discipline);
    
    const std::vector<ManagedObject<Group>>& getGroups() const;
    const std::vector<ManagedObject<Discipline>>& getDisciplines() const;
};

#endif // TEACHERJOURNAL_H
