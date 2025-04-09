#include "../include/teacherjournal.h"

void TeacherJournal::addGroup(ManagedObject<Group> group) {
    groups.push_back(std::move(group));
}

void TeacherJournal::addDiscipline(ManagedObject<Discipline> discipline) {
    disciplines.push_back(std::move(discipline));
}

const std::vector<ManagedObject<Group>>& TeacherJournal::getGroups() const {
    return groups;
}

const std::vector<ManagedObject<Discipline>>& TeacherJournal::getDisciplines() const {
    return disciplines;
}
