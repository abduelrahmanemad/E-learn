#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QCloseEvent>
#include <bits/stdc++.h>
#include <QTimeEdit>
#include <regex>
#include "config.h"
#include "dialog.h"

namespace Ui {
class AdminDashBoard;
}

class AdminDashBoard : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminDashBoard(QWidget *parent = nullptr);
    ~AdminDashBoard();
    string mode;

private slots:
    /// Validators
    void on_dayBox_currentIndexChanged(int index);
    void on_timeBox_currentIndexChanged(int index);
    void on_studentsGradeBox_currentIndexChanged(int index);
    void on_gradeSpin_valueChanged(int value);
    void on_courseNameEdit_textChanged(const QString &name);
    void on_studentsFirstEdit_textChanged(const QString &name);
    void on_studentsLastEdit_textChanged(const QString &name);
    void on_studentsEmailLast_textChanged(const QString &email);
    void on_studentsMobileEdit_textChanged(const QString &phone);
    void on_staffFirstEdit_textChanged(const QString &name);
    void on_staffLastEdit_textChanged(const QString &name);
    void on_staffEmailEdit_textChanged(const QString &email);
    void on_staffNumberEdit_textChanged(const QString &phone);
    void on_adminFirstEdit_textChanged(const QString &name);
    void on_adminLastEdit_textChanged(const QString &name);
    void on_adminEmailEdit_textChanged(const QString &email);
    void on_admingNumberEdit_textChanged(const QString &phone);
    void on_adminUserEdit_textChanged(const QString &user);
    void on_adminPasswordEdit_textChanged(const QString &pw);
    void on_adminConfirmEdit_textChanged(const QString &pw);
    void on_hallLineEdit_textChanged(const QString &hall);
    void on_departCode_textChanged(const QString &code);
    void on_departName_textChanged(const QString &name);

    /// Buttons
    void on_table_cellClicked(int row);
    void on_coursesBtn_clicked();
    void on_studentsBtn_clicked();
    void on_staffBtn_clicked();
    void on_addBtn_clicked();
    void on_addCourseBtn_clicked();
    void on_addStaffBtn_clicked();
    void on_addStudentsBtn_clicked();
    void on_addStudentBtn_clicked();
    void on_addNewStaffBtn_clicked();
    void on_studentsRemove_clicked();
    void on_staffRemoveBtn_clicked();
    void on_courseDetailsRemoveBtn_clicked();
    void on_logoutBtn_clicked();
    void on_toolsBtn_clicked();
    void on_studentsEdit_clicked();
    void on_staffEditBtn_clicked();
    void on_courseDetailsEditBtn_clicked();
    void on_addHallBtn_clicked();
    void on_addDepartBtn_clicked();
    void on_adminsBtn_clicked();
    void on_adminEdit_clicked();
    void on_addNewAdminBtn_clicked();
    void on_exportBtn_clicked();
    void on_importBtn_clicked();
    void on_loginBtn_clicked();
    void on_filtterBtn_clicked();
    void on_departList_itemClicked(QListWidgetItem *item);
    void on_staffTable_cellClicked(int row, int column);
    void on_studentsTable_cellClicked(int row, int column);

    /// Filters
    void on_maleBox_stateChanged();
    void on_femaleBox_stateChanged();
    void on_prepBox_stateChanged();
    void on_firstBox_stateChanged();
    void on_secondBox_stateChanged();
    void on_thirdBox_stateChanged();
    void on_fourthBox_stateChanged();
    void on_filterAgeSpin_valueChanged(int value);
    void on_filterSlider_valueChanged(int value);

    void on_adminAvatar_clicked();

private:
    Ui::AdminDashBoard *ui;

    // Fill table methods
    void fillTables();
    void fillCourses(QString input);
    void fillStudents(QString input);
    void fillStaff(QString input);
    void fillAdmins(QString input);

    // Buttons and validation methods
    void activateButton(QWidget *button);
    void deactivateButton(QWidget *button);
    void setIcon(QPushButton* button, QString path);
    void setAvailableHalls(int dayIndex, int timeIndex);
    void setAvailableLists(int dayIndex, int timeIndex);
    void setAvailableTables(int dayIndex, int timeIndex, int grade);
    void labelValidate(QWidget* label, bool condition, QString color);
    void changeLeftButtons();
    void on_return_clicked();

    // Table setttings changer
    void changeTableSettings(QTableWidget* table);

    // CSV functions
    void importCSV();
    void exportCSV();

    // Remove functions
    void removeCourse();
    void removeStudent();
    void removeStaff();

    // App quitting
    void closeEvent(QCloseEvent *event);

    // Filters
    void checkYear(QCheckBox* checkBox, int value);

    // Chart
    void showCharts();

    // Added and remove and deleted lists
    map<int, int> addedStaff;
    vector<int> removedStaff;
    vector<int> deletedStaff;
    map<int, int[2]> addedStudents;
    vector<int> removedStudents;
    vector<int> deletedStudents;

    // CurrentID and edit mode
    int currentID;
    bool editMode;

    // Filters
    bool filterOpen = false;
    vector<string> genderFilters;
    vector<int> yearFilters;
    vector<string> departFilters;
    vector<char> positionFilters;
    int sliderFilter;

    // Charts colors
    vector<QRgb> colors = {
        QRgb(0xFF7F50),
        QRgb(0x6B8E23),
        QRgb(0xCD5C5C),
        QRgb(0x9ACD32),
        QRgb(0xBDB76B),
        QRgb(0xF08080),
        QRgb(0xFFFF00),
        QRgb(0xE9967A),
        QRgb(0xADFF2F),
        QRgb(0xFA8072),
        QRgb(0x7FFF00),
        QRgb(0xFFA07A),
        QRgb(0x7CFC00),
        QRgb(0xFF4500),
        QRgb(0x556B2F),
        QRgb(0xFF8C00),
        QRgb(0xFFFF00),
        QRgb(0xFFA500),
        QRgb(0x808000),
        QRgb(0xFFD700),
        QRgb(0xF0E68C),
        QRgb(0xB8860B),
        QRgb(0xEEE8AA),
        QRgb(0xDAA520),
    };

};

#endif // ADMINDASHBOARD_H
