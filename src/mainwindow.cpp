#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QListWidget>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->gradesTable->setColumnCount(3);
    ui->gradesTable->setHorizontalHeaderLabels({"Предмет", "Оценка", "Дата"});

    connect(ui->groupsList, &QListWidget::currentRowChanged,
            this, &MainWindow::onGroupSelected);
    connect(ui->studentsList, &QListWidget::currentRowChanged,
            this, &MainWindow::onStudentSelected);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_addGroupButton_clicked()
{
    bool ok;
    QString groupName = QInputDialog::getText(
        this,
        "Новая группа",
        "Введите название группы:",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (ok && !groupName.isEmpty()) {
        journal.addGroup(ManagedObject<Group>(new Group(groupName)));
        updateGroupsList();
    }
}

void MainWindow::updateGroupsList()
{
    ui->groupsList->clear();
    for (const auto& group : journal.getGroups()) {
        ui->groupsList->addItem(group->getName());
    }
}

void MainWindow::onGroupSelected(int index)
{
    if (index >= 0 && static_cast<size_t>(index) < journal.getGroups().size()) {
        updateStudentsList();
    }
}

void MainWindow::updateStudentsList()
{
    ui->studentsList->clear();
    int groupIndex = ui->groupsList->currentRow();
    if (groupIndex < 0) return;

    const auto& students = journal.getGroups()[groupIndex]->getStudents();
    for (const auto& student : students) {
        ui->studentsList->addItem(
            student->getLastName() + " " + student->getFirstName() + " " + student->getPatronymic()
            );
    }
}

void MainWindow::on_addStudentButton_clicked() {
    if(ui->groupsList->currentRow() < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите группу!");
        return;
    }

    bool ok;
    QString studentFirstName = QInputDialog::getText(
        this, "Новый студент", "Имя:", QLineEdit::Normal, "", &ok);

    QString studentLastName = QInputDialog::getText(
        this, "Новый студент", "Фамилия:", QLineEdit::Normal, "", &ok);

    QString studentPatronymic = QInputDialog::getText(
        this, "Новый студент", "Отчество:", QLineEdit::Normal, "", &ok);

    if(ok && !studentFirstName.isEmpty() && !studentLastName.isEmpty()) {
        int groupIndex = ui->groupsList->currentRow();
        auto& group = journal.getGroups()[groupIndex];
        group->addStudent(ManagedObject<Person>(new Person(studentFirstName, studentLastName, studentPatronymic)));
        updateStudentsList();
    }
}

void MainWindow::on_addSubjectButton_clicked() {
    bool ok;
    QString subjectName = QInputDialog::getText(
        this, "Новый предмет", "Название:", QLineEdit::Normal, "", &ok);

    if(ok && !subjectName.isEmpty()) {
        journal.addDiscipline(ManagedObject<Discipline>(new Discipline(subjectName)));
        updateSubjectsList();
    }
}

void MainWindow::on_addGradeButton_clicked() {
    Person* student = getSelectedStudent();
    Discipline* subject = getSelectedSubject();

    if(!student || !subject) {
        QMessageBox::warning(this, "Ошибка", "Выберите студента и предмет!");
        return;
    }

    bool ok;
    const int value = QInputDialog::getInt(
        this, "Новая оценка", "Введите оценку (1-5):", 3, 1, 5, 1, &ok);

    if(ok) {
        try {
            const QDate date = QDate::currentDate();
            auto newSubject = CreateManaged<Discipline>(subject->getName());
            student->addGrade(CreateManaged<Mark>(
                value,
                date,
                std::move(newSubject)
                ));
            updateGradesTable();
        }
        catch(const std::exception& e) {
            QMessageBox::critical(this, "Ошибка", e.what());
        }
    }
}


void MainWindow::updateSubjectsList() {
    ui->subjectsComboBox->clear();
    for(const auto& subject : journal.getDisciplines()) {
        ui->subjectsComboBox->addItem(subject->getName());
    }
}

void MainWindow::updateGradesTable() {
    ui->gradesTable->setRowCount(0);

    if(auto student = getSelectedStudent()) {
        const auto& marks = student->getMarks();
        ui->gradesTable->setRowCount(marks.size());

        for(int row = 0; row < static_cast<int>(marks.size()); ++row) {
            const auto& grade = marks[row];

            if(grade) {
                QString disciplinesName = grade->getDisciplines()
                ? grade->getDisciplines()->getName()
                : "Неизвестный предмет";

                ui->gradesTable->setItem(row, 0, new QTableWidgetItem(disciplinesName));

                ui->gradesTable->setItem(row, 1,
                                         new QTableWidgetItem(QString::number(grade->getValue())));

                ui->gradesTable->setItem(row, 2,
                                         new QTableWidgetItem(grade->getMarkDate().toString("dd.MM.yyyy")));
            }
        }
    }
}

Person* MainWindow::getSelectedStudent() const {
    const int groupIndex = ui->groupsList->currentRow();
    const int studentIndex = ui->studentsList->currentRow();

    if(groupIndex >= 0 && studentIndex >= 0) {
        const auto& groups = journal.getGroups();
        if(groupIndex < static_cast<int>(groups.size())) {
            const auto& students = groups[groupIndex]->getStudents();
            if(studentIndex < static_cast<int>(students.size())) {
                return students[studentIndex].Raw(); // Возвращаем сырой указатель
            }
        }
    }
    return nullptr;
}

Discipline* MainWindow::getSelectedSubject() const {
    const int index = ui->subjectsComboBox->currentIndex();
    const auto& disciplines = journal.getDisciplines();

    if(index >= 0 && index < static_cast<int>(disciplines.size())) {
        return disciplines[index].Raw();
    }
    return nullptr;
}

void MainWindow::onStudentSelected(int index) {
    Q_UNUSED(index);
    updateGradesTable();
}

