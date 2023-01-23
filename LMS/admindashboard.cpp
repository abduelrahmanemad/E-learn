#include "admindashboard.h"
#include "ui_admindashboard.h"
#include <bits/stdc++.h>

// Regular expressions for validation
QRegularExpression emailRegEx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
                      QRegularExpression::CaseInsensitiveOption);
QRegularExpression phoneRegEx("^01[0125][0-9]{8}$");
QRegularExpression hallRegEx("[1-9][0-9]{3,5}");
QRegularExpression usernameRegEx("^[A-Za-z][A-Za-z0-9_]{2,9}$");
QRegularExpression passwordRegEx("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{6,}$");
QRegularExpression nameRegEx("^[A-Za-z][a-z,-]{2,}$");

// Constructor
AdminDashBoard::AdminDashBoard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminDashBoard) {
    ui->setupUi(this);

    // Connect buttons to their slots
    connect (ui->courseDetailsBackBtn, &QPushButton:: clicked, this, &AdminDashBoard::on_return_clicked);
    connect (ui->addCourseReturnBtn, &QPushButton:: clicked, this, &AdminDashBoard::on_return_clicked);
    connect (ui->staffDetailsBackBtn, &QPushButton:: clicked, this, &AdminDashBoard::on_return_clicked);
    connect (ui->addStaffReturnBtn, &QPushButton:: clicked, this, &AdminDashBoard::on_return_clicked);
    connect (ui->studentsDetailsBackBtn, &QPushButton:: clicked, this, &AdminDashBoard::on_return_clicked);
    connect (ui->addStudentReturnBtn, &QPushButton:: clicked, this, &AdminDashBoard::on_return_clicked);
    connect (ui->adminDetailsBack, &QPushButton:: clicked, this, &AdminDashBoard::on_return_clicked);
    connect (ui->addAdminReturnBtn, &QPushButton:: clicked, this, &AdminDashBoard::on_return_clicked);
    connect (ui->searchBar, &QLineEdit:: textChanged, this, &AdminDashBoard:: fillTables);

    // Set all tables settings
    changeTableSettings(ui->table);
    changeTableSettings(ui->courseStaffTable);
    changeTableSettings(ui->couseStudentsTable);
    changeTableSettings(ui->studentCoursesTable);
    changeTableSettings(ui->staffCoursesTable);
    changeTableSettings(ui->staffTable);
    changeTableSettings(ui->studentsTable);
    changeTableSettings(ui->departmentsTable);

    // Configure admins log list
    ui->logList->clear();
    ui->logList->setWordWrap(true);
    ui->logList->setTextElideMode(Qt::TextElideMode());

    // Import CSV data
    importCSV();

    // Set current pages
    ui->tabs->setCurrentIndex(6);
    ui->mainBodyTabs->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);

    // Hide filters
    ui->filters->hide();

    // Set validator for admin username
    QRegularExpressionValidator *usernameValidator = new QRegularExpressionValidator(QRegularExpression("^[A-Za-z][A-Za-z0-9_]{2,9}$"), this);
    ui->adminUserEdit->setValidator(usernameValidator);

    // Add genders to all comboboxes
    for (auto &p : genders) {
        ui->studentsGenderBox->addItem(QString::fromStdString(p));
        ui->staffGenderBox->addItem(QString::fromStdString(p));
        ui->adminGenderBox->addItem(QString::fromStdString(p));
    }

    // Add positions to staff combobox
    for (auto &p : positions) {
        ui->staffGenderBox_3->addItem(QString::fromStdString(p.second), QVariant(p.first));
    }

    // Add years to students combobox
    for (int i = 0; i < int(sizeof(years) / sizeof(years[0])); i++) {
        ui->studentsGradeBox->addItem(QString::fromStdString(years[i] + " year"), QVariant(i));
    }

    // Add day and times to course combobox
    for (int i = 0; i < int(sizeof(days) / sizeof(days[0])); i++) {
        ui->dayBox->addItem(QString::fromStdString(days[i]), QVariant(i));
    }

    for (int i = 0; i < int(sizeof(times) / sizeof(times[0])); i++) {
        ui->timeBox->addItem(QString::fromStdString(times[i]), QVariant(i));
    }

    // Set application title
    QCoreApplication::setApplicationName("ELearn.");
    setWindowTitle(QCoreApplication::applicationName());
}

// Destructor
AdminDashBoard::~AdminDashBoard() {
    delete ui;
}

// Log in button
void AdminDashBoard::on_loginBtn_clicked() {
    // Get input username & password
    QString username = ui->usernameInput->text().toLower();
    QString password = ui->passwordInput->text();

    // Loop over all admins and compare their usernames and passwords to the inputs
    for (auto &p : admins) {
        if (QString::compare(QString::fromStdString(p.second->getUsername()), username, Qt::CaseInsensitive) == 0 && QString::compare(QString::fromStdString(p.second->getPassword()), password, Qt:: CaseSensitive) == 0) {
            // Set the current admin, change the program size and move it to the center of the screen
            currentAdmin = p.first;

//            this->setMinimumSize(1318, 750);
//            this->setMaximumSize(16777215, 16777215);
//            this->setWindowState(Qt::WindowMaximized);

            ui->stackedWidget->setCurrentIndex(1);
            on_coursesBtn_clicked();

            // Change the current admin username and name
            ui->adminUserLabel->setText(QString::fromStdString(admins[currentAdmin]->getUsername()));
            ui->adminNameLabel->setText(QString::fromStdString(admins[currentAdmin]->getFirstName() + " " + admins[currentAdmin]->getLastName()));

            // Fill the departments combo box
            for (auto &p : departments) {
                if (p.first != "") {
                    ui->staffDepartBox->addItem(QString::fromStdString(p.second), QVariant(QString::fromStdString(p.first)));
                    ui->courseDepartBox->addItem(QString::fromStdString(p.second), QVariant(QString::fromStdString(p.first)));
                }
            }

            // Return back inputs stylesheet
            ui->usernameInput->setStyleSheet("background-color: #535353; border:1px solid rgba(0, 0, 0, 0); padding-left:10px; border-radius:7px;");
            ui->passwordInput->setStyleSheet("background-color: #535353; border:1px solid rgba(0, 0, 0, 0); padding-left:10px; border-radius:7px;");
            break;
        } else {
            //Make a warning if inocorrect username/password
            ui->usernameInput->setStyleSheet("background-color: #535353; border:1px solid rgba(0, 0, 0, 0); color: red; padding-left:10px; border-radius:7px;");
            ui->passwordInput->setStyleSheet("background-color: #535353; border:1px solid rgba(0, 0, 0, 0); color: red; padding-left:10px; border-radius:7px;");
        }
    }
}

// Logout button
void AdminDashBoard::on_logoutBtn_clicked() {
    // Ask user if they want to export their data
    Dialog *dial = new Dialog(this, "Logout", "Do you want to export you current data to CSV file before logging out?.");
    dial->setWindowTitle("Logout");

    if (dial->exec())
        exportCSV();

    // Go back to log in screen
    ui->stackedWidget->setCurrentIndex(0);
    importCSV();
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
//    this->setMinimumSize(550, 600);
//    this->setMaximumSize(550, 600);
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    QCoreApplication::setApplicationName("Elearn.");
    setWindowTitle(QCoreApplication::applicationName());
}

// Ask user if they want to export their data before quitting the app
void AdminDashBoard::closeEvent(QCloseEvent *event) {
    if (ui->stackedWidget->currentIndex() == 1) {
        Dialog *dial = new Dialog(this, "Logout", "Do you want to export you current data to CSV file before logging out?.");
        dial->setWindowTitle("Logout");
        if (dial->exec())
            exportCSV();
    }
}

// Set tables settings
void AdminDashBoard::changeTableSettings(QTableWidget* table) {
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setFocusPolicy(Qt::NoFocus);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

// Return button function
void AdminDashBoard::on_return_clicked() {
    if (mode == "admins" || mode == "tools")
        ui->tabs->setCurrentIndex(9);
    else {
        ui->tabs->setCurrentIndex(6);
        showCharts();
    }
}

// Helper functions for left menu
void AdminDashBoard::activateButton(QWidget* button) {
    button->setStyleSheet("color: white; border-top-right-radius:7px; border-bottom-right-radius:7px; background-color: rgb(63, 140, 255); text-align:left;");
}

void AdminDashBoard::deactivateButton(QWidget* button) {
    button->setStyleSheet("color: rgb(128, 129, 145); border-top-right-radius:7px; border-bottom-right-radius:7px; text-align:left;");
}

void AdminDashBoard::setIcon(QPushButton* button, QString path) {
    QIcon ico;
    ico.addPixmap(QPixmap(":/img/" + path), QIcon::Normal, QIcon::Off);
    button->setIcon(ico);
}

void AdminDashBoard::changeLeftButtons() {
    deactivateButton(ui->coursesFrame);
    deactivateButton(ui->coursesBtn);
    deactivateButton(ui->studentsFrame);
    deactivateButton(ui->studentsBtn);
    deactivateButton(ui->staffFrame);
    deactivateButton(ui->staffBtn);
    deactivateButton(ui->adminsFrame);
    deactivateButton(ui->adminsBtn);
    deactivateButton(ui->toolsFrame);
    deactivateButton(ui->toolsBtn);
    if (mode == "students") {
        activateButton(ui->studentsFrame);
        activateButton(ui->studentsBtn);
    } else if (mode == "staff") {
        activateButton(ui->staffFrame);
        activateButton(ui->staffBtn);
    } else if (mode == "courses") {
        activateButton(ui->coursesFrame);
        activateButton(ui->coursesBtn);
    } else if (mode == "tools") {
        activateButton(ui->toolsFrame);
        activateButton(ui->toolsBtn);
    } else if (mode == "admins") {
        activateButton(ui->adminsFrame);
        activateButton(ui->adminsBtn);
    }
}

/// Left menu buttons

// Courses button
void AdminDashBoard::on_coursesBtn_clicked() {
    if (mode != "courses") {
        ui->table->setHorizontalHeaderLabels({"Course ID", "Course Name", "Total Grade", "Course Department"});
        setIcon(ui->coursesBtn, "course.png");
        if (mode == "staff") setIcon(ui->staffBtn, "staff-inactive.png");
        if (mode == "students") setIcon(ui->studentsBtn, "student-inactive.png");
        if (mode == "tools") setIcon(ui->toolsBtn, "tools-inactive.png");
        if (mode == "admins") setIcon(ui->adminsBtn, "admin_inactive.png");
        ui->mainBodyTabs->setCurrentIndex(0);
        mode = "courses";
        changeLeftButtons();
        ui->headerLabel->setText(QString("Courses"));
        ui->addBtn->setText(QString("Add Course"));
        if (filterOpen)
            on_filtterBtn_clicked();
        ui->searchBar->clear();
        on_return_clicked();
        QCoreApplication::setApplicationName("ELearn. - Courses");
        setWindowTitle(QCoreApplication::applicationName());
    }
    fillTables();
}

// Students button
void AdminDashBoard::on_studentsBtn_clicked() {
    if (mode != "students") {
        ui->table->setHorizontalHeaderLabels({"Student ID", "Student Name", "Student Year", "Student Department"});
        if (mode == "courses") setIcon(ui->coursesBtn, "course-inactive.png");
        if (mode == "staff") setIcon(ui->staffBtn, "staff-inactive.png");
        setIcon(ui->studentsBtn, "student.png");
        if (mode == "tools") setIcon(ui->toolsBtn, "tools-inactive.png");
        if (mode == "admins") setIcon(ui->adminsBtn, "admin_inactive.png");
        ui->mainBodyTabs->setCurrentIndex(0);
        mode = "students";
        changeLeftButtons();
        ui->headerLabel->setText(QString("Students"));
        ui->addBtn->setText(QString("Add Student"));
        if (filterOpen)
            on_filtterBtn_clicked();
        ui->searchBar->clear();
        on_return_clicked();
        QCoreApplication::setApplicationName("ELearn. - Students");
        setWindowTitle(QCoreApplication::applicationName());
    }
    fillTables();
}

// Staff button
void AdminDashBoard::on_staffBtn_clicked() {
    if (mode != "staff") {
        ui->table->setHorizontalHeaderLabels({"Staff ID", "Staff Name", "Staff Position", "Staff Department"});
        if (mode == "courses") setIcon(ui->coursesBtn, "course-inactive.png");
        setIcon(ui->staffBtn, "staff.png");
        if (mode == "students") setIcon(ui->studentsBtn, "student-inactive.png");
        if (mode == "tools") setIcon(ui->toolsBtn, "tools-inactive.png");
        if (mode == "admins") setIcon(ui->adminsBtn, "admin_inactive.png");
        ui->mainBodyTabs->setCurrentIndex(0);
        mode = "staff";
        changeLeftButtons();
        ui->headerLabel->setText(QString("Staff"));
        ui->addBtn->setText(QString("Add Staff"));
        if (filterOpen)
            on_filtterBtn_clicked();
        ui->searchBar->clear();
        on_return_clicked();
        QCoreApplication::setApplicationName("ELearn. - Staff");
        setWindowTitle(QCoreApplication::applicationName());
    }
    fillTables();
}

// Admins button
void AdminDashBoard::on_adminsBtn_clicked() {
    if (mode != "admins") {
        ui->table->setHorizontalHeaderLabels({"Admin ID", "Admin Name", "Admin Username", "Admin Age"});
        if (mode == "courses") setIcon(ui->coursesBtn, "course-inactive.png");
        if (mode == "staff") setIcon(ui->staffBtn, "staff-inactive.png");
        if (mode == "students") setIcon(ui->studentsBtn, "student-inactive.png");
        if (mode == "tools") setIcon(ui->toolsBtn, "tools-inactive.png");
        setIcon(ui->adminsBtn, "admin.png");
        ui->mainBodyTabs->setCurrentIndex(0);
        mode = "admins";
        changeLeftButtons();
        ui->headerLabel->setText("Admins");
        ui->addBtn->setText("Add Admin");
        if (filterOpen)
            on_filtterBtn_clicked();
        on_return_clicked();
        QCoreApplication::setApplicationName("ELearn. - Admins");
        setWindowTitle(QCoreApplication::applicationName());
    }
    fillTables();
}

// Tools button
void AdminDashBoard::on_toolsBtn_clicked() {
    if (mode != "tools") {
        if (mode == "courses") setIcon(ui->coursesBtn, "course-inactive.png");
        if (mode == "staff") setIcon(ui->staffBtn, "staff-inactive.png");
        if (mode == "students") setIcon(ui->studentsBtn, "student-inactive.png");
        if (mode == "admins") setIcon(ui->adminsBtn, "admin_inactive.png");
        setIcon(ui->toolsBtn, "tools.png");
        mode = "tools";
        changeLeftButtons();
        ui->mainBodyTabs->setCurrentIndex(1);

        // Make validators for halls and departments
        QRegularExpressionValidator *hallValidator = new QRegularExpressionValidator(QRegularExpression("[1-9][0-9]{3,5}"), this);
        QRegularExpressionValidator *departCodeValidator = new QRegularExpressionValidator(QRegularExpression("[A-Za-z]{3}"), this);
        QRegularExpressionValidator *departNameValidator = new QRegularExpressionValidator(QRegularExpression("^[A-Za-z ]+$"), this);

        ui->hallLineEdit->setValidator(hallValidator);
        ui->departCode->setValidator(departCodeValidator);
        ui->departName->setValidator(departNameValidator);

        // Add current halls to halls list
        ui->hallsList->clear();
        for (auto &p : halls)
            ui->hallsList->addItem(QString::fromStdString(p));

        // Add current departments to departments table
        ui->departmentsTable->setRowCount(0);
        for (auto &p : departments) {
            int rowCount = ui->departmentsTable->rowCount();
            ui->departmentsTable->setRowCount(rowCount + 1);

            QTableWidgetItem *items [] = {
                new QTableWidgetItem(QString::fromStdString(p.first).toUpper()),
                new QTableWidgetItem(QString::fromStdString(p.second))
            };

            for (short column = 0; column < 2; column++) {
                items[column]->setTextAlignment(Qt::AlignCenter);
                ui->departmentsTable->setItem(rowCount, column, items[column]);
            }
        }
        on_return_clicked();
        QCoreApplication::setApplicationName("ELearn. - Admin Tools");
        setWindowTitle(QCoreApplication::applicationName());
    }
}

/// Filling table functions

// Fill table
void AdminDashBoard::fillTables() {
    if (mode == "students")
        fillStudents(ui->searchBar->text());
    else if (mode == "staff")
        fillStaff(ui->searchBar->text());
    else if (mode == "courses")
        fillCourses(ui->searchBar->text());
    else if (mode == "admins")
        fillAdmins(ui->searchBar->text());
}

// Fill students table
void AdminDashBoard::fillStudents(QString input) {
    input = input.toLower();
    ui->table->setRowCount(0);
    for (auto const &p: students) {
        Student *currentStudent = p.second;
        QString studentName = QString::fromStdString(currentStudent->getFirstName()).toLower() + " " + QString::fromStdString(currentStudent->getLastName());

        // Filters
        bool nameFilter = input == "" ? true : studentName.replace(" ", "").toLower().startsWith(input.replace(" ", ""));
        bool genderFilter = genderFilters.size() == 0 ? true : (count(genderFilters.begin(), genderFilters.end(), currentStudent->getGender()));
        bool yearFilter = yearFilters.size() == 0 ? true : (count(yearFilters.begin(), yearFilters.end(), currentStudent->getGrade()));
        bool departFilter = departFilters.size() == 0 ? true : (count(departFilters.begin(), departFilters.end(), currentStudent->getDepartment()));
        bool ageFilter = currentStudent->getAge() >= sliderFilter;

        if (genderFilter && yearFilter && departFilter && ageFilter && nameFilter) {
            int rowCount = ui->table->rowCount();
            ui->table->setRowCount(rowCount + 1);

            QTableWidgetItem *items [] = {
                new QTableWidgetItem(QString::number(currentStudent->getStudentID())),
                new QTableWidgetItem(QString::fromStdString(currentStudent->getFirstName() + " " + currentStudent->getLastName())),
                new QTableWidgetItem(QString::fromStdString(years[currentStudent->getGrade()])),
                new QTableWidgetItem(QString::fromStdString(currentStudent->getDepartment()).toUpper()),
            };

            for (short column = 0; column < 4; column++) {
                items[column]->setTextAlignment(Qt::AlignCenter);
                ui->table->setItem(rowCount, column, items[column]);
            }
        }
    }
}

// Fill staff table
void AdminDashBoard::fillStaff(QString input) {
    input = input.toLower();
    ui->table->setRowCount(0);
    for (auto const &p: staffList) {
        Staff *currentStaff = p.second;
        QString staffName = QString::fromStdString(currentStaff->getFirstName()) + " " + QString::fromStdString(currentStaff->getLastName());

        // Filters
        bool nameFilter = input == "" ? true : staffName.replace(" ", "").toLower().startsWith(input.replace(" ", ""));
        bool genderFilter = genderFilters.size() == 0 ? true : (count(genderFilters.begin(), genderFilters.end(), currentStaff->getGender()));
        bool departFilter = departFilters.size() == 0 ? true : (count(departFilters.begin(), departFilters.end(), currentStaff->getDepartment()));
        bool positionFilter = positionFilters.size() == 0 ? true : (count(positionFilters.begin(), positionFilters.end(), currentStaff->getPosition()));
        bool ageFilter = currentStaff->getAge() >= sliderFilter;

        if (genderFilter && departFilter && positionFilter && nameFilter && ageFilter) {
            int rowCount = ui->table->rowCount();
            ui->table->setRowCount(rowCount + 1);

            QTableWidgetItem *items [] = {
                new QTableWidgetItem(QString::number(currentStaff->getStaffID())),
                new QTableWidgetItem(QString::fromStdString(currentStaff->getFirstName() + " " + currentStaff->getLastName())),
                new QTableWidgetItem(QString::fromStdString(positions[currentStaff->getPosition()])),
                new QTableWidgetItem(QString::fromStdString(currentStaff->getDepartment()).toUpper()),
            };

            for (short column = 0; column < 4; column++) {
                items[column]->setTextAlignment(Qt::AlignCenter);
                ui->table->setItem(rowCount, column, items[column]);
            }
        }
    }
}

// Fill courses table
void AdminDashBoard::fillCourses(QString input) {
    input = input.toLower();
    ui->table->setRowCount(0);
    for (auto const &p: courses) {
        Course *currentCourse = p.second;
        QString courseName = QString::fromStdString(currentCourse->getCourseName()).toLower();

        // Filters
        bool nameFilter = input == "" ? true : courseName.replace(" ", "").toLower().startsWith(input.replace(" ", ""));
        bool departFilter = departFilters.size() == 0 ? true : (count(departFilters.begin(), departFilters.end(), currentCourse->getCourseDepartment()));
        bool gradeFilter = currentCourse->getCourseGrade() >= sliderFilter;

        if (departFilter && nameFilter && gradeFilter) {
            int rowCount = ui->table->rowCount();
            ui->table->setRowCount(rowCount + 1);

            QTableWidgetItem *items [] = {
                new QTableWidgetItem(QString::number(currentCourse->getCourseID())),
                new QTableWidgetItem(QString::fromStdString(currentCourse->getCourseName())),
                new QTableWidgetItem(QString::number(currentCourse->getCourseGrade())),
                new QTableWidgetItem(QString::fromStdString(currentCourse->getCourseDepartment()).toUpper()),
            };

            for (short column = 0; column < 4; column++) {
                items[column]->setTextAlignment(Qt::AlignCenter);
                ui->table->setItem(rowCount, column, items[column]);
            }
        }
    }
}

// Fill admins table
void AdminDashBoard::fillAdmins(QString input) {
    input = input.toLower();
    ui->table->setRowCount(0);
    for (auto const &p: admins) {
        Admin *currentAdmin = p.second;
        QString adminName = QString::fromStdString(currentAdmin->getFirstName()).toLower() + " " + QString::fromStdString(currentAdmin->getLastName());

        // Filters
        bool nameFilter = input == "" ? true : adminName.replace(" ", "").toLower().startsWith(input.replace(" ", ""));
        bool genderFilter = genderFilters.size() == 0 ? true : (count(genderFilters.begin(), genderFilters.end(), currentAdmin->getGender()));
        bool ageFilter = currentAdmin->getAge() >= sliderFilter;

        if (genderFilter && nameFilter && ageFilter) {
            int rowCount = ui->table->rowCount();
            ui->table->setRowCount(rowCount + 1);

            QTableWidgetItem *items [] = {
                new QTableWidgetItem(QString::number(currentAdmin->getAdminID())),
                new QTableWidgetItem(QString::fromStdString(currentAdmin->getFirstName() + " " + currentAdmin->getLastName())),
                new QTableWidgetItem(QString::fromStdString(currentAdmin->getUsername())),
                new QTableWidgetItem(QString::number(currentAdmin->getAge())),
            };

            for (short column = 0; column < 4; column++) {
                items[column]->setTextAlignment(Qt::AlignCenter);
                ui->table->setItem(rowCount, column, items[column]);
            }
        }
    }
}

// Charts displaying function
void AdminDashBoard::showCharts() {
    ui->chartOneFrame->show();
    ui->chartTwoFrame->show();
    ui->chartThreeFrame->show();

    // Set background for all charts
    QLinearGradient backgroundGradient;
    backgroundGradient.setStart(QPointF(0, 0));
    backgroundGradient.setFinalStop(QPointF(0, 1));
    backgroundGradient.setColorAt(0.0, QRgb(0x1E90FF));
    backgroundGradient.setColorAt(1.0, QRgb(0x4169E1));
    backgroundGradient.setCoordinateMode(QGradient::ObjectMode);

    // Set title font for all charts
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setItalic(true);
    titleFont.setCapitalization(QFont::AllUppercase);
    titleFont.setPixelSize(18);

    // Set legend font of the bar charts
    QFont barLegend;
    barLegend.setBold(true);
    barLegend.setPixelSize(14);

    // Set labels for departments charts
    QFont departLabel;
    departLabel.setPixelSize(9);

    // Set gender labels and legend for gender charts
    QFont genderLabel;
    genderLabel.setBold(true);
    genderLabel.setPixelSize(11);
    QFont genderLegend;
    genderLegend.setBold(true);
    genderLegend.setPixelSize(12);

    // Remove all old layouts
    QLayout* layoutOne = ui->chartOneFrame->layout();
    if (layoutOne != 0) {
        QLayoutItem *item;
        while ((item = layoutOne->takeAt(0)) != 0)
            layoutOne->removeItem (item);
        delete layoutOne;
    }

    QLayout* layoutTwo = ui->chartTwoFrame->layout ();
    if (layoutTwo != 0) {
        QLayoutItem *item;
        while ((item = layoutTwo->takeAt(0)) != 0)
            layoutTwo->removeItem(item);
        delete layoutTwo;
    }

    QLayout* layoutThree = ui->chartThreeFrame->layout ();
    if (layoutThree != 0) {
        QLayoutItem *item;
        while ((item = layoutThree->takeAt(0)) != 0)
            layoutThree->removeItem(item);
        delete layoutThree;
    }

    // Students charts
    if (mode == "students") {
        // Students by year bar char
        QBarSet *yearSet = new QBarSet("Students");
        yearSet->setColor(QRgb(0xFFD700));
        *yearSet  << dashboard->yearsStudentsNum[0] << dashboard->yearsStudentsNum[1]  << dashboard->yearsStudentsNum[2]
                << dashboard->yearsStudentsNum[3]  << dashboard->yearsStudentsNum[4];

        QBarSeries *yearSeries = new QBarSeries();
        yearSeries->append(yearSet);

        QChart *yearChart = new QChart();
        yearChart->setBackgroundBrush(backgroundGradient);
        yearChart->addSeries(yearSeries);
        yearChart->setTitle("Students Per Year");
        yearChart->setTitleFont(titleFont);
        yearChart->setAnimationOptions(QChart::AllAnimations);

        QStringList yearCategories;
        yearCategories << "Preparatory" << "First" << "Second" << "Third" << "Fourth";
        QBarCategoryAxis *yearAxis = new QBarCategoryAxis();

        QFont yearLabelsFont;
        yearLabelsFont.setPixelSize(15);
        yearAxis->setLabelsFont(yearLabelsFont);
        yearAxis->append(yearCategories);
        yearChart->createDefaultAxes();
        yearChart->setAxisX(yearAxis, yearSeries);
        yearChart->legend()->setVisible(true);
        yearChart->legend()->setAlignment(Qt::AlignBottom);
        yearChart->legend()->setFont(barLegend);

        QChartView *yearChartView = new QChartView(yearChart);
        yearChartView->setRenderHint(QPainter::Antialiasing);
        layoutOne = new QVBoxLayout(ui->chartOneFrame);
        layoutOne->addWidget(yearChartView);

        // Studenys by gender pie chart
        QPieSeries *genderSeries = new QPieSeries();
        genderSeries->append("Male", dashboard->maleStudents);
        genderSeries->append("Female", dashboard->femaleStudents);
        QPieSlice *maleSlice = genderSeries->slices().at(0);
        QPieSlice *femaleSlice = genderSeries->slices().at(1);

        maleSlice->setLabelVisible();
        maleSlice->setLabelFont(genderLabel);
        maleSlice->setBorderColor(QRgb(0xFFFFFF));
        maleSlice->setColor(QRgb(0xFF7C5D));

        femaleSlice->setLabelVisible();
        femaleSlice->setLabelFont(genderLabel);
        femaleSlice->setBorderColor(QRgb(0xFFFFFF));
        femaleSlice->setColor(QRgb(0x9ACD32));

        QChart *genderChart = new QChart;
        genderChart->addSeries(genderSeries);
        genderChart->setTitle("Male to Female students");
        genderChart->setTitleFont(titleFont);
        genderChart->setAnimationOptions(QChart::AllAnimations);
        genderChart->legend()->setVisible(true);
        genderChart->legend()->setAlignment(Qt::AlignBottom);
        genderChart->legend()->setFont(genderLegend);

        maleSlice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
        maleSlice->setLabel("Male " + QString("%1%").arg(100 * maleSlice->percentage(), 0, 'f', 1));
        femaleSlice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
        femaleSlice->setLabel("Female " + QString("%1%").arg(100 * femaleSlice->percentage(), 0, 'f', 1));
        genderChart->setBackgroundBrush(backgroundGradient);

        QChartView * genderChartView = new QChartView(genderChart);
        genderChartView->setRenderHint(QPainter::Antialiasing);
        layoutTwo = new QVBoxLayout(ui->chartTwoFrame);
        layoutTwo->addWidget(genderChartView);

        // Students by department pie chart
        QPieSeries *departSeries = new QPieSeries();
        int slice = 0;
        int colorIndex = 0;
        for (auto &p : departments) {
            if (dashboard->departmentStudentsNum[p.first] > 0) {
                departSeries->append(QString::fromStdString(p.first).toUpper(), dashboard->departmentStudentsNum[p.first]);
                departSeries->slices().at(slice)->setBorderColor(QRgb(0xFFFFFF));
                departSeries->slices().at(slice)->setColor(colors[colorIndex]);
                slice++;
                if (colorIndex == int(colors.size()))
                    colorIndex = 0;
                else
                    colorIndex++;
            }
        }

        for (auto const &slice : departSeries->slices()) {
            slice->setLabelVisible();
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
            slice->setLabelFont(departLabel);
            slice->setLabel(slice->label() + "\n" + QString("%1%").arg(100 * slice->percentage(), 0, 'f', 1));
        }

        QChart *departChart = new QChart;
        departChart->addSeries(departSeries);
        departChart->setTitle("Student per department");
        departChart->legend()->setFont(departLabel);
        departChart->setTitleFont(titleFont);
        departChart->setAnimationOptions(QChart::AllAnimations);
        departChart->legend()->setVisible(false);
        departChart->setBackgroundBrush(backgroundGradient);

        QChartView * departChartView = new QChartView(departChart);
        departChartView->setRenderHint(QPainter::Antialiasing);
        layoutThree = new QVBoxLayout(ui->chartThreeFrame);
        layoutThree->addWidget(departChartView);
    }

    // Staff charts
    if (mode == "staff") {
        // Staff by position and gender bar chart
        QBarSet *maleSet = new QBarSet("Male Staff");
        *maleSet  << dashboard->maleProfNum << dashboard->maleTaNum;
        maleSet->setColor(QRgb(0xFF7C5D));

        QBarSet *femaleSet = new QBarSet("Female Staff");
        *femaleSet << dashboard->profNum - dashboard->maleProfNum << dashboard->taNum - dashboard->maleTaNum;
        femaleSet->setColor(QRgb(0x9ACD32));

        QBarSeries *positionSeries = new QBarSeries();
        positionSeries->append(maleSet);
        positionSeries->append(femaleSet);

        QChart *positionChart = new QChart();
        positionChart->setBackgroundBrush(backgroundGradient);
        positionChart->addSeries(positionSeries);
        positionChart->setTitle("Staff by position");
        positionChart->setTitleFont(titleFont);
        positionChart->setAnimationOptions(QChart::AllAnimations);

        QStringList positionCategories;
        positionCategories << "Prof" << "TA";

        QBarCategoryAxis *yearAxis = new QBarCategoryAxis();
        QFont yearLabelsFont;
        yearLabelsFont.setPixelSize(15);
        yearAxis->setLabelsFont(yearLabelsFont);
        yearAxis->append(positionCategories);
        positionChart->createDefaultAxes();
        positionChart->setAxisX(yearAxis, positionSeries);
        positionChart->legend()->setVisible(true);
        positionChart->legend()->setAlignment(Qt::AlignBottom);
        positionChart->legend()->setFont(barLegend);

        QChartView *positionChartView = new QChartView(positionChart);
        positionChartView->setRenderHint(QPainter::Antialiasing);
        layoutOne = new QVBoxLayout(ui->chartOneFrame);
        layoutOne->addWidget(positionChartView);

        // Staff by gender pie chart
        QPieSeries *genderSeries = new QPieSeries();
        genderSeries->append("Male", dashboard->maleStaff);
        genderSeries->append("Female", dashboard->femaleStaff);
        QPieSlice *maleSlice = genderSeries->slices().at(0);
        QPieSlice *femaleSlice = genderSeries->slices().at(1);

        maleSlice->setLabelVisible();
        maleSlice->setLabelFont(genderLabel);
        maleSlice->setBorderColor(QRgb(0xFFFFFF));
        maleSlice->setColor(QRgb(0xFF7C5D));

        femaleSlice->setLabelVisible();
        femaleSlice->setLabelFont(genderLabel);
        femaleSlice->setBorderColor(QRgb(0xFFFFFF));
        femaleSlice->setColor(QRgb(0x9ACD32));

        QChart *genderChart = new QChart;
        genderChart->addSeries(genderSeries);
        genderChart->setTitle("Male to Female staff");
        genderChart->setTitleFont(titleFont);
        genderChart->setAnimationOptions(QChart::AllAnimations);
        genderChart->legend()->setVisible(true);
        genderChart->legend()->setAlignment(Qt::AlignBottom);
        genderChart->legend()->setFont(genderLegend);

        maleSlice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
        maleSlice->setLabel("Male " + QString("%1%").arg(100 * maleSlice->percentage(), 0, 'f', 1));
        femaleSlice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
        femaleSlice->setLabel("Female " + QString("%1%").arg(100 * femaleSlice->percentage(), 0, 'f', 1));
        genderChart->setBackgroundBrush(backgroundGradient);

        QChartView * genderChartView = new QChartView(genderChart);
        genderChartView->setRenderHint(QPainter::Antialiasing);
        layoutTwo = new QVBoxLayout(ui->chartTwoFrame);
        layoutTwo->addWidget(genderChartView);

        // Staff by department pie chart
        QPieSeries *departSeries = new QPieSeries();
        int slice = 0;
        int colorIndex = 0;
        for (auto &p : departments) {
            if (dashboard->departmentStaffNum[p.first] > 0) {
                departSeries->append(QString::fromStdString(p.first).toUpper(), dashboard->departmentStaffNum[p.first]);
                departSeries->slices().at(slice)->setBorderColor(QRgb(0xFFFFFF));
                departSeries->slices().at(slice)->setColor(colors[colorIndex]);
                slice++;
                if (colorIndex == int(colors.size()))
                    colorIndex = 0;
                else
                    colorIndex++;
            }
        }

        for (auto const &slice : departSeries->slices()) {
            slice->setLabelVisible();
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
            slice->setLabelFont(departLabel);
            slice->setLabel(slice->label() + "\n" + QString("%1%").arg(100 * slice->percentage(), 0, 'f', 1));
        }

        QChart *departChart = new QChart;
        departChart->addSeries(departSeries);
        departChart->setTitle("Staff per department");
        departChart->legend()->setFont(departLabel);
        departChart->setTitleFont(titleFont);
        departChart->setAnimationOptions(QChart::AllAnimations);
        departChart->legend()->setVisible(false);
        departChart->setBackgroundBrush(backgroundGradient);

        QChartView * departChartView = new QChartView(departChart);
        departChartView->setRenderHint(QPainter::Antialiasing);
        layoutThree = new QVBoxLayout(ui->chartThreeFrame);
        layoutThree->addWidget(departChartView);
    }

    // Courses charts
    if (mode == "courses") {
        ui->chartTwoFrame->hide();
        ui->chartThreeFrame->hide();

        // Courses by department pie chart
        QPieSeries *departSeries = new QPieSeries();
        int slice = 0;
        int colorIndex = 0;
        for (auto &p : departments) {
            if (dashboard->departmentCoursesNum[p.first] > 0) {
                departSeries->append(QString::fromStdString(p.first).toUpper(), dashboard->departmentCoursesNum[p.first]);
                departSeries->slices().at(slice)->setBorderColor(QRgb(0xFFFFFF));
                departSeries->slices().at(slice)->setColor(colors[colorIndex]);
                slice++;
                if (colorIndex == int(colors.size()))
                    colorIndex = 0;
                else
                    colorIndex++;
            }
        }

        for (auto const &slice : departSeries->slices()) {
            slice->setLabelVisible();
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
            slice->setLabelFont(departLabel);
            slice->setLabel(slice->label() + "\n" + QString("%1%").arg(100 * slice->percentage(), 0, 'f', 1));
        }

        QChart *departChart = new QChart;
        departChart->addSeries(departSeries);
        departChart->setTitle("Courses per department");
        departChart->legend()->setFont(departLabel);
        departChart->setTitleFont(titleFont);
        departChart->setAnimationOptions(QChart::AllAnimations);
        departChart->legend()->setVisible(false);
        departChart->setBackgroundBrush(backgroundGradient);

        QChartView * departChartView = new QChartView(departChart);
        departChartView->setRenderHint(QPainter::Antialiasing);
        layoutOne = new QVBoxLayout(ui->chartOneFrame);
        layoutOne->addWidget(departChartView);
    }
}

// Displaying informations in right menu
void AdminDashBoard::on_table_cellClicked(int row){
    // Save the id of the selected item
    QString ID = ui->table->item(row, 0)->text();
    int id = ID.toInt();
    currentID = id;
    if (mode == "courses") {
        // Display course informations
        ui->tabs->setCurrentIndex(0);
        QString courseName = QString::fromStdString(courses[id]->getCourseName());
        QCoreApplication::setApplicationName("ELearn. - " + courseName);
        setWindowTitle(QCoreApplication::applicationName());

        // Set the labels values to the course informations
        ui->courseNameValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt; font-weight:700;\">" + courseName +  "</span></p><br></body></html>");
        ui->courseCodeValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + ID + "</span></p></body></html>");
        QString day = QString::fromStdString(days[courses[id]->getLectureDay()]);
        QString time = QString::fromStdString(times[courses[id]->getLectureTime()]);
        ui->courseDayValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + day + "</span></p></body></html>");
        ui->courseTimeValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + time + "</span></p></body></html>");
        ui->courseGradeValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::number(courses[id]->getCourseGrade()) + "</span></p></body></html>");
        ui->averageGrade->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::number(courses[id]->averageGrade) + "/" + QString::number(courses[id]->getCourseGrade()) + "</span></p></body></html>");
        ui->ratioValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::number(courses[id]->passRatio) + "%</span></p></body></html>");
        ui->courseHallValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(courses[id]->getLectureHall()) + "</span></p></body></html>");
        ui->courseDepartValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:12pt;\">" + QString::fromStdString(courses[id]->getCourseDepartment()).toUpper() + "</span></p></body></html>");

        // Fill the course students table
        ui->couseStudentsTable->setRowCount(0);
        for (auto &p : courses[id]->students) {
            if (p.first == 0)
                continue;
            int courseGrade = p.second;
            int rowCount = ui->couseStudentsTable->rowCount();
            ui->couseStudentsTable->setRowCount(rowCount + 1);
            QTableWidgetItem *items [] = {
                new QTableWidgetItem(QString::fromStdString(students[p.first]->getFirstName() + " " + students[p.first]->getLastName())),
                new QTableWidgetItem(QString::number(courseGrade))
            };
            const size_t count = sizeof(items) / sizeof(QTableWidgetItem*);
            for (size_t column = 0; column < count; column++) {
                items[column]->setTextAlignment(Qt::AlignCenter);
                ui->couseStudentsTable->setItem(rowCount, column, items[column]);
            }
        }

        // Fill the course staff table
        ui->courseStaffTable->setRowCount(0);
        for (auto &p : courses[id]->staff) {
            if (p == 0)
                continue;
            int rowCount = ui->courseStaffTable->rowCount();
            ui->courseStaffTable->setRowCount(rowCount + 1);
            QTableWidgetItem *items [] = {
                new QTableWidgetItem(QString::fromStdString(staffList[p]->getFirstName() + " " + staffList[p]->getLastName())),
                new QTableWidgetItem(QString::fromStdString(positions[staffList[p]->getPosition()]))
            };
            const size_t count = sizeof(items) / sizeof(QTableWidgetItem*);
            for (size_t column = 0; column < count; column++) {
                items[column]->setTextAlignment(Qt::AlignCenter);
                ui->courseStaffTable->setItem(rowCount, column, items[column]);
            }
        }

    } else if (mode == "staff") {
        // Display staff informations
        ui->tabs->setCurrentIndex(2);
        QString name = QString::fromStdString(staffList[id]->getFirstName() + " " + staffList[id]->getLastName());
        QCoreApplication::setApplicationName("ELearn. - " + name);
        setWindowTitle(QCoreApplication::applicationName());

        // Set the labels values to the staff informations
        ui->staffNameValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt; font-weight:700;\">" + name +  "</span></p><br></body></html>");
        ui->staffEmailValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(staffList[id]->getEmail()) + "</span></p></body></html>");
        ui->staffPhoneValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(staffList[id]->getMobile()) + "</span></p></body></html>");
        ui->staffGenderValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(staffList[id]->getGender()) + "</span></p></body></html>");
        ui->staffAgeValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::number(staffList[id]->getAge()) + "</span></p></body></html>");
        ui->staffDepartValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:12pt;\">" + QString::fromStdString(departments[staffList[id]->getDepartment()]) + "</span></p></body></html>");
        ui->staffIDValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + ID + "</span></p></body></html>");
        ui->staffPositionValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(positions[staffList[id]->getPosition()]) + "</span></p></body></html>");

        // Fill the staff courses table
        ui->staffCoursesTable->setRowCount(0);
        for (auto const &p : staffList[id]->getCourses()) {
            if (p == 0)
                continue;
            int courseID = p;
            Course* course = courses[courseID];
            int rowCount = ui->staffCoursesTable->rowCount();
            ui->staffCoursesTable->setRowCount(rowCount + 1);
            QTableWidgetItem *items [] = {
                new QTableWidgetItem(QString::number(course->getCourseID())),
                new QTableWidgetItem(QString::fromStdString(course->getCourseName())),
            };
            const size_t count = sizeof(items) / sizeof(QTableWidgetItem*);
            for (size_t column = 0; column < count; column++) {
                items[column]->setTextAlignment(Qt::AlignCenter);
                ui->staffCoursesTable->setItem(rowCount, column, items[column]);
            }
        }
    }
    else if (mode == "students") {
        // Display students informations
        ui->tabs->setCurrentIndex(4);
        QString name = QString::fromStdString(students[id]->getFirstName() + " " + students[id]->getLastName());
        QCoreApplication::setApplicationName("ELearn. - " + name);
        setWindowTitle(QCoreApplication::applicationName());

        // Set the labels values to the student informations
        ui->studentNameValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt; font-weight:700;\">" + name +  "</span></p><br></body></html>");
        ui->studentEmailValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(students[id]->getEmail()) + "</span></p></body></html>");
        ui->studentPhoneValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(students[id]->getMobile()) + "</span></p></body></html>");
        ui->studentGenderValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(students[id]->getGender()) + "</span></p></body></html>");
        ui->studentAgeValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::number(students[id]->getAge()) + "</span></p></body></html>");
        ui->studentDepartValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:12pt;\">" + QString::fromStdString(departments[students[id]->getDepartment()]) + "</span></p></body></html>");
        ui->studentGradeValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(years[students[id]->getGrade()] + " year </span></p></body></html>"));
        ui->studentsIDValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + ID + "</span></p></body></html>");

        // Fill the student courses table
        ui->studentCoursesTable->setRowCount(0);
        for (auto const &p : students[id]->getCourses()) {
            if (p.first == 0)
                continue;
            int courseID = p.first;
            int courseGrade = p.second;
            Course* course = courses[courseID];
            int rowCount = ui->studentCoursesTable->rowCount();
            ui->studentCoursesTable->setRowCount(rowCount + 1);
            QTableWidgetItem *items [] = {
                new QTableWidgetItem(QString::number(course->getCourseID())),
                new QTableWidgetItem(QString::fromStdString(course->getCourseName())),
                new QTableWidgetItem(QString::number(courseGrade))
            };
            const size_t count = sizeof(items) / sizeof(QTableWidgetItem*);
            for (size_t column = 0; column < count; column++) {
                items[column]->setTextAlignment(Qt::AlignCenter);
                ui->studentCoursesTable->setItem(rowCount, column, items[column]);
            }
        }
    } else {
        // Display admin informations
        ui->tabs->setCurrentIndex(8);
        QString name = QString::fromStdString(admins[id]->getFirstName() + " " + admins[id]->getLastName());
        QCoreApplication::setApplicationName("ELearn. - " + name);
        setWindowTitle(QCoreApplication::applicationName());

        // Set the labels values to the admin informations
        ui->adminNameValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt; font-weight: 400;\">" + name +  "</span></p></body></html>");
        ui->adminEmailValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(admins[id]->getEmail()) + "</span></p></body></html>");
        ui->adminPhoneValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(admins[id]->getMobile()) + "</span></p></body></html>");
        ui->adminGenderValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(admins[id]->getGender()) + "</span></p></body></html>");
        ui->adminAgeValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::number(admins[id]->getAge()) + "</span></p></body></html>");
        ui->adminUsernameValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(admins[id]->getUsername()) + "</span></p></body></html>");

        // Make edit availalbe only for the current admin profile
        if (id == currentAdmin)
            ui->adminEdit->show();
        else
            ui->adminEdit->hide();
    }
}

// Open current admin profile
void AdminDashBoard::on_adminAvatar_clicked() {
    ui->tabs->setCurrentIndex(8);
    QString name = QString::fromStdString(admins[currentAdmin]->getFirstName() + " " + admins[currentAdmin]->getLastName());
    QCoreApplication::setApplicationName("ELearn. - " + name);
    setWindowTitle(QCoreApplication::applicationName());

    ui->adminNameValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt; font-weight: 400;\">" + name +  "</span></p></body></html>");
    ui->adminEmailValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(admins[currentAdmin]->getEmail()) + "</span></p></body></html>");
    ui->adminPhoneValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(admins[currentAdmin]->getMobile()) + "</span></p></body></html>");
    ui->adminGenderValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(admins[currentAdmin]->getGender()) + "</span></p></body></html>");
    ui->adminAgeValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::number(admins[currentAdmin]->getAge()) + "</span></p></body></html>");
    ui->adminUsernameValue->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:14pt;\">" + QString::fromStdString(admins[currentAdmin]->getUsername()) + "</span></p></body></html>");

    ui->adminEdit->show();
}

// Main body add button
void AdminDashBoard::on_addBtn_clicked() {
    editMode = false;
    if (mode == "courses") {
        // Add course tab, reset all added and removed and deleted structures
        ui->tabs->setCurrentIndex(1);
        addedStudents.clear();
        removedStudents.clear();
        addedStaff.clear();
        removedStaff.clear();
        deletedStaff.clear();
        deletedStudents.clear();

        // Clear all values
        ui->courseNameEdit->clear();
        ui->dayBox->setCurrentIndex(0);
        ui->timeBox->setCurrentIndex(0);
        ui->courseDepartBox->setCurrentIndex(0);
        ui->hallBox->clear();
        ui->addStaffBox->clear();
        ui->addStudentsBox->clear();

        // Set minimum and maximum values to the grade spins
        ui->gradeSpin->setMinimum(50);
        ui->gradeSpin->setMaximum(250);
        ui->addStudentsGrade->setMinimum(0);
        ui->addStudentsGrade->setMaximum(50);
        ui->gradeSpin->setValue(ui->gradeSpin->minimum());
        ui->addStudentsGrade->setValue(ui->addStudentsGrade->minimum());

        // Set available halls list and students & staff lists
        setAvailableHalls(0, 0);
        setAvailableLists(0, 0);

        // Reset the students & staff tables
        ui->staffTable->setRowCount(0);
        ui->studentsTable->setRowCount(0);

        // Change app title name
        QCoreApplication::setApplicationName("ELearn. - Add Course");
        setWindowTitle(QCoreApplication::applicationName());
    } else if (mode == "staff") {
        // Add staff tab, clear all the values
        ui->tabs->setCurrentIndex(3);
        ui->staffFirstEdit->clear();
        ui->staffLastEdit->clear();
        ui->staffEmailEdit->clear();
        ui->staffNumberEdit->clear();
        ui->staffDepartBox->setCurrentIndex(0);
        ui->staffGenderBox->setCurrentIndex(0);

        // Set minimum and maximum age spin
        ui->staffAgeSpin->setMinimum(16 + (sizeof(years) / sizeof(years[0])));
        ui->staffAgeSpin->setMaximum(65);
        ui->staffAgeSpin->setValue(ui->staffAgeSpin->minimum());

        // Change app title name
        QCoreApplication::setApplicationName("ELearn. - Add Staff");
        setWindowTitle(QCoreApplication::applicationName());
    } else if (mode == "students") {
        // Add student tab, clear all the values
        ui->tabs->setCurrentIndex(5);
        ui->studentsFirstEdit->clear();
        ui->studentsLastEdit->clear();
        ui->studentsEmailLast->clear();
        ui->studentsMobileEdit->clear();
        ui->studentsDepartBox->setCurrentIndex(0);
        ui->studentsGradeBox->setCurrentIndex(0);

        // Set minimum and maximum age spin
        ui->studentsAgeSpin->setMinimum(16);
        ui->studentsAgeSpin->setMaximum(16 + years->size() * 2);
        ui->studentsAgeSpin->setValue(16);

        // Change app title name
        QCoreApplication::setApplicationName("ELearn. - Add Student");
        setWindowTitle(QCoreApplication::applicationName());
    } else if (mode == "admins") {
        // Add admin tab, clear all the values
        ui->tabs->setCurrentIndex(7);
        ui->adminFirstEdit->clear();
        ui->adminLastEdit->clear();
        ui->adminEmailEdit->clear();
        ui->admingNumberEdit->clear();
        ui->adminUserEdit->clear();
        ui->adminPasswordEdit->clear();
        ui->adminConfirmEdit->clear();
        ui->adminGenderBox->setCurrentIndex(0);

        // Set minimum and maximum age
        ui->adminAgeSpin->setMinimum(22);
        ui->adminAgeSpin->setMaximum(60);
        ui->adminAgeSpin->setValue(ui->adminAgeSpin->minimum());

        // Hide warning labels
        ui->passwordWarningLabel->hide();
        ui->confirmWarningLabel->hide();

        // Change app title name
        QCoreApplication::setApplicationName("ELearn. - Add Admin");
        setWindowTitle(QCoreApplication::applicationName());
    }
}

// Change table row color function
void setRowBackground(const QBrush& brush, QAbstractItemModel* model, int row, const QModelIndex& parent = QModelIndex() ){
    for(int i=0;i<model->columnCount(parent);++i)
        Q_ASSUME(model->setData(model->index(row,i,parent),brush,Qt::BackgroundRole));
}

// Label validator
void AdminDashBoard::labelValidate(QWidget* label, bool condition, QString color) {
    if (!condition)
        label->setStyleSheet("color: red; text-decoration: underline; background-color: " + color + "; border-top-left-radius:5px; border-top-right-radius:5px; border-bottom-left-radius:5px; border-bottom-right-radius:5px; text-align:center;");
    else
        label->setStyleSheet("text-decoration: none; background-color: " + color + "; border-top-left-radius:5px; border-top-right-radius:5px; border-bottom-left-radius:5px; border-bottom-right-radius:5px; text-align:center;");
}

/// Add or edit course functions

// Add course validators
void AdminDashBoard::setAvailableHalls(int dayIndex, int timeIndex) {
    // Set the available halls combo box
    ui->hallBox->clear();
    vector<string> resHalls = hallDates[dayIndex * 10 + timeIndex];
    for (int i = 0; i < int(halls.size()); i++) {
        if (!count(resHalls.begin(), resHalls.end(), halls[i])) {
             ui->hallBox->addItem(QString::fromStdString(halls[i]), QVariant(i));
        }
    }
}

void AdminDashBoard::setAvailableLists(int dayIndex, int timeIndex) {
    ui->addStudentsBox->clear();
    ui->addStaffBox->clear();

    // Loop over each student and check whether they are available or not at the day & time index
    for (auto &s : students) {
        bool addStudent = true;
        Student *currentStudent = s.second;

        // If students is already added to this course, don't add them to the list
        if (addedStudents.find(s.first) != addedStudents.end())
                addStudent = false;

        // Loop over student's courses and check if their dates have the same day & time index
        for (auto &p : currentStudent->getCourses()) {
            if (p.first == 0)
                continue;
            Course *currentCourse = courses[p.first];
            if ((currentCourse->getLectureDay() == dayIndex && currentCourse->getLectureTime() == timeIndex)) {
                addStudent = false;
            }
        }

        // If the student is available, add them to the combo box
        if (addStudent)
            ui->addStudentsBox->addItem(QString::fromStdString(currentStudent->getFirstName() + " " + currentStudent->getLastName()), QVariant(s.first));
    }

    // Loop over each staff and check whether they are available or not at the day & time index
    for (auto &s : staffList) {
        bool addStaff = true;
        Staff *currentStaff = s.second;

        // If staff is already added to this course, don't add them to the list
        if (addedStaff.find(s.first) != addedStaff.end())
                addStaff = false;

        // Loop over staff's courses and check if their dates have the same day & time index
        for (auto &p : currentStaff->getCourses()) {
            if (p == 0)
                continue;
            Course *currentCourse = courses[p];
            if ((currentCourse->getLectureDay() == dayIndex && currentCourse->getLectureTime() == timeIndex)) {
                addStaff = false;
            }
        }

        // If the student is available, add them to the combo box
        if (addStaff)
            ui->addStaffBox->addItem(QString::fromStdString(currentStaff->getFirstName() + " " + currentStaff->getLastName()), QVariant(s.first));
    }
}

// Check if all students & staff in the tables are available after day, time, or grade are changed
void AdminDashBoard::setAvailableTables(int dayIndex, int timeIndex, int grade) {
    removedStaff.clear();
    removedStudents.clear();

    // Loop over all added staff members, check whether they are still available or not
    for (auto &s : addedStaff) {
        bool removeStaff = false;
        Staff *currentStaff = staffList[s.first];

        // Loop over staff's courses and check if they are available or not
        for (auto &p : currentStaff->getCourses()) {
            if (p == 0)
                continue;
            Course *currentCourse = courses[p];
            if ((currentCourse->getLectureDay() == dayIndex && currentCourse->getLectureTime() == timeIndex))
                removeStaff = true;

            if (editMode && p == currentID) {
                if (currentCourse->getLectureDay() == dayIndex && currentCourse->getLectureTime() == timeIndex)
                    removeStaff = false;
            }
        }

        // If the staff isn't available, they will be removed
        if (removeStaff) {
            removedStaff.push_back(s.first);
            setRowBackground(QColor(114, 17, 2), ui->staffTable->model(), s.second);

            // Change the icon next to staff cell to the removed icon
            QIcon removedIcon(":/img/removed.png");
            ui->staffTable->item(s.second, 0)->setIcon(removedIcon);

            // Remove the staff from the user-deleted list if they exist
            for (int i = 0; i < int(deletedStaff.size()); i++) {
                if (deletedStaff[i] == s.first)
                    deletedStaff[i] = 0;
            }
        } else {
            // Check whether the staff was removed before or not, and remove them from the removed list
            for (int i = 0; i < int(removedStaff.size()); i++) {
                if (removedStaff[i] == s.first)
                    removedStaff[i] = 0;
            }

            // If the staff isn't deleted by the user, bring them back to the added list
            if (!count(deletedStaff.begin(), deletedStaff.end(), s.first)) {
                setRowBackground(QColor(26, 26, 26), ui->staffTable->model(), s.second);
                QIcon minusIcon(":/img/remove.png");
                ui->staffTable->item(s.second, 0)->setIcon(minusIcon);
            }
        }
    }

    // Loop over all added students, check whether they are still available or not
    for (auto &s : addedStudents) {
        bool removeStudent = false;
        Student *currentStudent = students[s.first];

        // Check if the new grade is smaller than the student's grade, if yes, remove the student
        if (grade < s.second[0])
            removeStudent = true;

        // Loop over staff's courses and check if they are available or not
        for (auto &p : currentStudent->getCourses()) {
            if (p.first == 0)
                continue;
            Course *currentCourse = courses[p.first];
            if ((currentCourse->getLectureDay() == dayIndex && currentCourse->getLectureTime() == timeIndex))
                removeStudent = true;

            if (editMode && p.first == currentID) {
                if (currentCourse->getLectureDay() == dayIndex && currentCourse->getLectureTime() == timeIndex)
                    removeStudent = false;
            }
        }

        // If the student isn't available, they will be removed
        if (removeStudent) {
            removedStudents.push_back(s.first);
            setRowBackground(QColor(114, 17, 2), ui->studentsTable->model(), s.second[1]);

            // Change the icon next to student cell to the removed icon
            QIcon removedIcon(":/img/removed.png");
            ui->studentsTable->item(s.second[1], 0)->setIcon(removedIcon);

            // Remove the student from the user-deleted list if they exist
            for (int i = 0; i < int(deletedStudents.size()); i++) {
                if (deletedStudents[i] == s.first)
                    deletedStudents[i] = 0;
            }
        } else {
            // Check whether the student was removed before or not, and remove them from the removed list
            for (int i = 0; i < int(removedStudents.size()); i++) {
                if (removedStudents[i] == s.first)
                    removedStudents[i] = 0;
            }

            // If the student isn't deleted by the user, bring them back to the added list
            if (!count(deletedStudents.begin(), deletedStudents.end(), s.first)) {
                setRowBackground(QColor(26, 26, 26), ui->studentsTable->model(), s.second[1]);
                QIcon minusIcon(":/img/remove.png");
                ui->studentsTable->item(s.second[1], 0)->setIcon(minusIcon);
            }
        }
    }
}

// If the day is changed, reset the available halls, lists, tables
void AdminDashBoard::on_dayBox_currentIndexChanged(int index) {
    setAvailableHalls(index, ui->timeBox->currentIndex());
    setAvailableLists(index, ui->timeBox->currentIndex());
    setAvailableTables(index, ui->timeBox->currentIndex(), ui->gradeSpin->value());
}

// If the time is changed, reset the available halls, lists, tables
void AdminDashBoard::on_timeBox_currentIndexChanged(int index) {
    setAvailableHalls(ui->dayBox->currentIndex(), index);
    setAvailableLists(ui->dayBox->currentIndex(), index);
    setAvailableTables(ui->dayBox->currentIndex(), index, ui->gradeSpin->value());
}

// If the grade is changed, reset the maximum student grade, reset the available tables
void AdminDashBoard::on_gradeSpin_valueChanged(int value) {
    ui->addStudentsGrade->setMaximum(value);
    if (ui->addStudentsGrade->value() > value)
        ui->addStudentsGrade->setValue(value);
    setAvailableTables(ui->dayBox->currentIndex(), ui->timeBox->currentIndex(), value);
}

// Validate course name when it is changed
void AdminDashBoard::on_courseNameEdit_textChanged(const QString &name) {
    labelValidate(ui->courseNameEdit, name.size() >= 5, "rgb(36, 36, 36)");
}

// Add staff to course
void AdminDashBoard::on_addStaffBtn_clicked() {
    // Get id of the added staff
    int addedStaffId = ui->addStaffBox->itemData(ui->addStaffBox->currentIndex()).value<int>();
    if (addedStaffId != 0) {
        // Add the staff to the added staff vector
        int rowCount = ui->staffTable->rowCount();
        ui->staffTable->setRowCount(rowCount + 1);
        addedStaff[addedStaffId] = ui->staffTable->rowCount() - 1;

        // Make an item for the added staff in the staff table
        QTableWidgetItem *items [] = {
            new QTableWidgetItem(QString::fromStdString(staffList[addedStaffId]->getFirstName() + " " + staffList[addedStaffId]->getLastName())),
            new QTableWidgetItem(QString::fromStdString(positions[staffList[addedStaffId]->getPosition()]))
        };

        QIcon minusIcon(":/img/remove.png");
        items[0]->setIcon(minusIcon);

        const size_t count = sizeof(items) / sizeof(QTableWidgetItem*);
        for (size_t column = 0; column < count; column++) {
            items[column]->setTextAlignment(Qt::AlignCenter);
            ui->staffTable->setItem(rowCount, column, items[column]);
        }

        // Reset the available staff list
        setAvailableLists(ui->dayBox->currentIndex(), ui->timeBox->currentIndex());
    }
}

// Add student to course
void AdminDashBoard::on_addStudentsBtn_clicked() {
    // Get id of the added student
    int addedStudentId = ui->addStudentsBox->itemData(ui->addStudentsBox->currentIndex()).value<int>();
    if (addedStudentId != 0) {
        // Add the staff to the added students vector
        int grade = ui->addStudentsGrade->value();
        addedStudents[addedStudentId][0] = grade;

        // Make an item for the added student in the students table
        int rowCount = ui->studentsTable->rowCount();
        ui->studentsTable->setRowCount(rowCount + 1);
        addedStudents[addedStudentId][1] = ui->studentsTable->rowCount() - 1;

        QTableWidgetItem *items [] = {
            new QTableWidgetItem(QString::fromStdString(students[addedStudentId]->getFirstName() + " " + students[addedStudentId]->getLastName())),
            new QTableWidgetItem(QString::number(grade)),
        };

        QIcon minusIcon(":/img/remove.png");
        items[0]->setIcon(minusIcon);

        const size_t count = sizeof(items) / sizeof(QTableWidgetItem*);
        for (size_t column = 0; column < count; column++) {
            items[column]->setTextAlignment(Qt::AlignCenter);
            ui->studentsTable->setItem(rowCount, column, items[column]);
        }

        // Reset the available students list
        ui->addStudentsGrade->clear();
        setAvailableLists(ui->dayBox->currentIndex(), ui->timeBox->currentIndex());
    }
}

// Delete staff from course students
void AdminDashBoard::on_staffTable_cellClicked(int row, int column) {
    if (column == 0) {
        // Loop over added staff to detect the id of the removed student
        for (auto const &p : addedStaff) {
            if (p.second == row) {
                // If the staff is not removed, and not deleted before, remove them from the staff table
                if (!count(removedStaff.begin(), removedStaff.end(), p.first)) {
                    if (!count(deletedStaff.begin(), deletedStaff.end(), p.first)) {
                        deletedStaff.push_back(p.first);
                        setRowBackground(QColor(114, 17, 2), ui->staffTable->model(), p.second);
                        QIcon plusIcon(":/img/add.png");
                        ui->staffTable->item(row, 0)->setIcon(plusIcon);
                    } else {
                        // If the staff is deleted before, remove them from the deleted list and add them back to the staff table
                        for (int i = 0; i < int(deletedStaff.size()); i++) {
                            if (deletedStaff[i] == p.first)
                                deletedStaff[i] = 0;
                        }
                        setRowBackground(QColor(26, 26, 26), ui->staffTable->model(), p.second);
                        QIcon minusIcon(":/img/remove.png");
                        ui->staffTable->item(row, 0)->setIcon(minusIcon);
                    }
                }
            }
        }
    }
}

// Delete student from course students
void AdminDashBoard::on_studentsTable_cellClicked(int row, int column) {
    if (column == 0) {
        // Loop over added students to detect the id of the removed student
        for (auto const &p : addedStudents) {
            if (p.second[1] == row) {
                // If the student is not removed, and not deleted before, remove them from the students table
                if (!count(removedStudents.begin(), removedStudents.end(), p.first)) {
                    if (!count(deletedStudents.begin(), deletedStudents.end(), p.first)) {
                        deletedStudents.push_back(p.first);
                        setRowBackground(QColor(114, 17, 2), ui->studentsTable->model(), p.second[1]);
                        QIcon plusIcon(":/img/add.png");
                        ui->studentsTable->item(row, 0)->setIcon(plusIcon);
                    } else {
                        // If the student is deleted before, remove them from the deleted list and add them back to the students table
                        for (int i = 0; i < int(deletedStudents.size()); i++) {
                            if (deletedStudents[i] == p.first)
                                deletedStudents[i] = 0;
                        }
                        setRowBackground(QColor(26, 26, 26), ui->studentsTable->model(), p.second[1]);
                        QIcon minusIcon(":/img/remove.png");
                        ui->studentsTable->item(row, 0)->setIcon(minusIcon);
                    }
                }
            }
        }
    }
}

// Add/Edit course button
void AdminDashBoard::on_addCourseBtn_clicked() {
    // Get the input values
    QString courseName = ui->courseNameEdit->text();
    short dayIndex = ui->dayBox->itemData(ui->dayBox->currentIndex()).value<short>();
    short timeIndex = ui->dayBox->itemData(ui->timeBox->currentIndex()).value<short>();
    int totalGrade = ui->gradeSpin->value();
    string hall = halls[ui->hallBox->itemData(ui->hallBox->currentIndex()).value<int>()];
    string department = ui->courseDepartBox->itemData(ui->courseDepartBox->currentIndex()).value<QString>().toStdString();

    // Check if the course name is longer than or equal to 5 characters
    if (courseName.size() >= 5) {
        // Make new course with input informations
        Course *newCourse = new Course(editMode ? currentID : currentCourseID);
        newCourse->setCourseName(courseName.toStdString());
        newCourse->setCourseGrade(totalGrade);
        newCourse->setLectureDay(dayIndex);
        newCourse->setLectureTime(timeIndex);
        newCourse->setLectureHall(hall);
        newCourse->setCourseDepartment(department);
        hallDates[dayIndex * 10 + timeIndex].push_back(hall);

        // If the course is new, add it to the list of courses
        if (!editMode) {
            currentCourseID++;
            courses[newCourse->getCourseID()] = newCourse;
            QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
            ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " added new course: " + courseName + ".");
        } else {
            // If the course is being edited, remove the old course and add the new one insead to the list of courses
            delete courses[currentID];
            courses[currentID] = newCourse;
            QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
            ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " edited course " + courseName + " informations.");
        }

        // Add all staff in the added list to the course
        for (auto &p : addedStaff) {
            if (!count(removedStaff.begin(), removedStaff.end(), p.first) && !count(deletedStaff.begin(), deletedStaff.end(), p.first))
                staffList[p.first]->addCourse(newCourse->getCourseID());
        }

        // Add all students in the added list to the course
        for (auto &p : addedStudents) {
            if (!count(removedStudents.begin(), removedStudents.end(), p.first) && (!count(deletedStudents.begin(), deletedStudents.end(), p.first)))
                students[p.first]->addCourse(newCourse->getCourseID(), p.second[0]);
        }

        on_coursesBtn_clicked();
        on_return_clicked();
        QCoreApplication::setApplicationName("Elearn. - Courses");
        setWindowTitle(QCoreApplication::applicationName());
    }
}

/// Add or edit student functions

// Add students validators

// Set the departments list if the student is not at the preparatory year
void AdminDashBoard::on_studentsGradeBox_currentIndexChanged(int index) {
    if (index > 0) {
        ui->studentsDepartBox->clear();
        for (auto &p : departments) {
            ui->studentsDepartBox->addItem(QString::fromStdString(p.second), QVariant(QString::fromStdString(p.first)));
        }
        if (ui->studentsDepartBox->currentText() == "")
            ui->studentsDepartBox->removeItem(0);
    } else {
        ui->studentsDepartBox->clear();
    }

    // Increase one minimum age year to each college year
    ui->studentsAgeSpin->setMinimum(16 + index);
    if (ui->studentsAgeSpin->value() < (16 + index))
        ui->studentsAgeSpin->setValue(ui->studentsAgeSpin->minimum());
}

// Validate first name
void AdminDashBoard::on_studentsFirstEdit_textChanged(const QString &name) {
    labelValidate(ui->studentsFirstEdit, nameRegEx.match(name).hasMatch(), "rgb(36, 36, 36)");
}

// Validate last name
void AdminDashBoard::on_studentsLastEdit_textChanged(const QString &name) {
    labelValidate(ui->studentsLastEdit, nameRegEx.match(name).hasMatch(), "rgb(36, 36, 36)");
}

// Validate email
void AdminDashBoard::on_studentsEmailLast_textChanged(const QString &email) {
    labelValidate(ui->studentsEmailLast, emailRegEx.match(email).hasMatch(), "rgb(36, 36, 36)");
}

// Validate mobile number
void AdminDashBoard::on_studentsMobileEdit_textChanged(const QString &phone) {
    labelValidate(ui->studentsMobileEdit, phoneRegEx.match(phone).hasMatch(), "rgb(36, 36, 36)");
}

// Add/Edit student button
void AdminDashBoard::on_addStudentBtn_clicked() {
    // Get the input values
    QString firstName = ui->studentsFirstEdit->text();
    QString lastName = ui->studentsLastEdit->text();
    QString email = ui->studentsEmailLast->text();
    QString phone = ui->studentsMobileEdit->text();
    string gender = genders[ui->studentsGenderBox->currentIndex()];
    int year = ui->studentsGradeBox->currentIndex();
    string department = ui->studentsDepartBox->itemData(ui->studentsDepartBox->currentIndex()).value<QString>().toStdString();
    int age = ui->studentsAgeSpin->value();

    // Check if the data is correctly validated
    bool dataValidated = nameRegEx.match(firstName).hasMatch() && nameRegEx.match(lastName).hasMatch() &&
            phoneRegEx.match(phone).hasMatch() && emailRegEx.match(email).hasMatch();
    if (dataValidated) {
        // Make new student with input informations
        Student *newStudent = new Student(editMode ? currentID : currentStudentID);
        newStudent->setFirstName(firstName.toStdString());
        newStudent->setLastName(lastName.toStdString());
        newStudent->setAge(age);
        newStudent->setMobile("+2" + phone.toStdString());
        newStudent->setEmail(email.toStdString());
        newStudent->setDepartment(department);
        newStudent->setGrade(year);
        newStudent->setGender(gender);

        if (!editMode) {
            // If the student is new, add it to the list of students
            students[newStudent->getStudentID()] = newStudent;
            currentStudentID++;
            QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
            ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " added new student: " + firstName + " " + lastName + ".");
        } else {
            // If the student is being edited, remove the old student and add the new one insead to the list of students
            // Remove the old student from all the courses, and add the new one to the courses
            for (auto &p : students[currentID]->getCourses()) {
                if (p.first == 0)
                    continue;
                students[currentID]->removeCourse(p.first);
                newStudent->addCourse(p.first, p.second);
                QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
                ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " edited student " + firstName + " " + lastName + " informations.");
            }
            delete students[currentID];
            students[currentID] = newStudent;
        }

        on_studentsBtn_clicked();
        on_return_clicked();
        QCoreApplication::setApplicationName("Elearn. - Students");
        setWindowTitle(QCoreApplication::applicationName());
    }
}

/// Add or edit staff functions

// Add staff validators

// Staff first name validator
void AdminDashBoard::on_staffFirstEdit_textChanged(const QString &name) {
    labelValidate(ui->staffFirstEdit, nameRegEx.match(name).hasMatch(), "rgb(36, 36, 36)");
}

// Staff last name validator
void AdminDashBoard::on_staffLastEdit_textChanged(const QString &name) {
    labelValidate(ui->staffLastEdit, nameRegEx.match(name).hasMatch(), "rgb(36, 36, 36)");
}

// Staff email validator
void AdminDashBoard::on_staffEmailEdit_textChanged(const QString &email) {
    labelValidate(ui->staffEmailEdit, emailRegEx.match(email).hasMatch(), "rgb(36, 36, 36)");
}

// Staff mobile number validator
void AdminDashBoard::on_staffNumberEdit_textChanged(const QString &phone) {
    labelValidate(ui->staffNumberEdit, phoneRegEx.match(phone).hasMatch(), "rgb(36, 36, 36)");
}

// Add/Edit staff button
void AdminDashBoard::on_addNewStaffBtn_clicked() {
    // Get the input values
    QString firstName = ui->staffFirstEdit->text();
    QString lastName = ui->staffLastEdit->text();
    QString email = ui->staffEmailEdit->text();
    QString phone = ui->staffNumberEdit->text();
    string gender = genders[ui->staffGenderBox->currentIndex()];
    string department = ui->staffDepartBox->itemData(ui->staffDepartBox->currentIndex()).value<QString>().toStdString();
    char position = ui->staffGenderBox_3->itemData(ui->staffGenderBox_3->currentIndex()).value<char>();
    int age = ui->staffAgeSpin->value();

    // Check if the data is correctly validated
    bool dataValidated = nameRegEx.match(firstName).hasMatch() && nameRegEx.match(lastName).hasMatch() &&
            phoneRegEx.match(phone).hasMatch() && emailRegEx.match(email).hasMatch();
    if (dataValidated) {
        // Make new staff with input informations
        Staff *newStaff = new Staff(editMode ? currentID : currentStaffID);
        newStaff->setFirstName(firstName.toStdString());
        newStaff->setLastName(lastName.toStdString());
        newStaff->setAge(age);
        newStaff->setMobile("+2" + phone.toStdString());
        newStaff->setEmail(email.toStdString());
        newStaff->setDepartment(department);
        newStaff->setGender(gender);
        newStaff->setPosition(position);

        if (!editMode) {
            // If the staff is new, add them to the list of staff
            staffList[newStaff->getStaffID()] = newStaff;
            currentStaffID++;
            QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
            ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " added new staff member: " + firstName + " " + lastName + ".");
        } else {
            // If the staff is being edited, remove the old staff and add the new one insead to the list of staff
            // Remove the old staff from all the courses, and add the new one to the courses
            for (auto &p : staffList[currentID]->getCourses()) {
                if (p == 0)
                    continue;
                staffList[currentID]->removeCourse(p);
                newStaff->addCourse(p);
            }
            delete staffList[currentID];
            staffList[currentID] = newStaff;
            QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
            ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " edited staff member " + firstName + " " + lastName + " informations.");
        }

        on_staffBtn_clicked();
        on_return_clicked();
        QCoreApplication::setApplicationName("Elearn. - Staff");
        setWindowTitle(QCoreApplication::applicationName());
    }
}

/// Add or edit admin functions

// Admin first name validator
void AdminDashBoard::on_adminFirstEdit_textChanged(const QString &name) {
    labelValidate(ui->adminFirstEdit, nameRegEx.match(name).hasMatch(), "rgb(36, 36, 36)");
}

// Admin last name validator
void AdminDashBoard::on_adminLastEdit_textChanged(const QString &name) {
    labelValidate(ui->adminLastEdit, nameRegEx.match(name).hasMatch(), "rgb(36, 36, 36)");
}

// Admin email validator
void AdminDashBoard::on_adminEmailEdit_textChanged(const QString &email) {
    labelValidate(ui->adminEmailEdit, emailRegEx.match(email).hasMatch(), "rgb(36, 36, 36)");
}

// Admin phone number validator
void AdminDashBoard::on_admingNumberEdit_textChanged(const QString &phone) {
    labelValidate(ui->admingNumberEdit, phoneRegEx.match(phone).hasMatch(), "rgb(36, 36, 36)");
}

// Admin username validator
void AdminDashBoard::on_adminUserEdit_textChanged(const QString &user) {
    labelValidate(ui->adminUserEdit, usernameRegEx.match(user).hasMatch(), "rgb(36, 36, 36)");
}

// Admin password validator
void AdminDashBoard::on_adminPasswordEdit_textChanged(const QString &pw) {
    labelValidate(ui->adminPasswordEdit, passwordRegEx.match(pw).hasMatch(), "rgb(36, 36, 36)");
    if (!passwordRegEx.match(pw).hasMatch())
        ui->passwordWarningLabel->show();
    else
        ui->passwordWarningLabel->hide();
}

// Admin confirm password validator
void AdminDashBoard::on_adminConfirmEdit_textChanged(const QString &pw) {
    QString password = ui->adminPasswordEdit->text();
    labelValidate(ui->adminConfirmEdit, QString::compare(pw, password, Qt::CaseSensitive) == 0, "rgb(36, 36, 36)");
    if (QString::compare(pw, password, Qt::CaseSensitive) == 0)
        ui->confirmWarningLabel->hide();
    else
        ui->confirmWarningLabel->show();
}

// Add/Edit admin button
void AdminDashBoard::on_addNewAdminBtn_clicked() {
    // Get the input values
    QString firstName = ui->adminFirstEdit->text();
    QString lastName = ui->adminLastEdit->text();
    QString email = ui->adminEmailEdit->text();
    QString phone = ui->admingNumberEdit->text();
    string gender = genders[ui->adminGenderBox->currentIndex()];
    int age = ui->adminAgeSpin->value();
    QString username = ui->adminUserEdit->text();
    QString password = ui->adminPasswordEdit->text();

    // Check if the data is correctly validated
    bool dataValidated = nameRegEx.match(firstName).hasMatch() && nameRegEx.match(lastName).hasMatch() &&
            phoneRegEx.match(phone).hasMatch() && emailRegEx.match(email).hasMatch() && usernameRegEx.match(username).hasMatch()
            && passwordRegEx.match(password).hasMatch() && QString::compare(ui->adminConfirmEdit->text(), password, Qt::CaseSensitive) == 0;
    if (firstName.size() >= 3 && lastName.size() >= 3 && dataValidated) {
        // Make new admin with input informations
        Admin *newAdmin = new Admin(editMode ? currentAdmin : currentAdminID);
        newAdmin->setFirstName(firstName.toStdString());
        newAdmin->setLastName(lastName.toStdString());
        newAdmin->setAge(age);
        newAdmin->setGender(gender);
        newAdmin->setMobile("+2" + phone.toStdString());
        newAdmin->setEmail(email.toStdString());
        newAdmin->setUsername(username.toStdString());
        newAdmin->setPassword(password.toStdString());

        if (!editMode) {
            // If the admin is new, add them to the list of staff
            admins[newAdmin->getAdminID()] = newAdmin;
            currentAdminID++;
            QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
            ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " added new admin: " + firstName + " " + lastName + ".");
        } else {
            // If the admin is being edited, remove the old admin and add the new one insead to the list of admins
            delete admins[currentAdmin];
            admins[currentAdmin] = newAdmin;
        }

        on_adminsBtn_clicked();
        on_return_clicked();
        QCoreApplication::setApplicationName("Elearn. - Admins");
        setWindowTitle(QCoreApplication::applicationName());
    }
}

/// Removing functions

// Remove student function
void AdminDashBoard::removeStudent() {
    int id = currentID;
    // Remove the student from all courses
    for (auto &p : students[id]->getCourses()) {
        if (p.first == 0)
            continue;
        students[id]->removeCourse(p.first);
    }

    // Add the removing action to the admins log
    QString firstName = QString::fromStdString(students[id]->getFirstName());
    QString lastName = QString::fromStdString(students[id]->getFirstName());
    QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
    ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " removed student: " + firstName + " " + lastName + ".");

    // Remove the student from the students list
    delete students[id];
    students.erase(id);

    on_studentsBtn_clicked();
    on_return_clicked();
    QCoreApplication::setApplicationName("Elearn. - Students");
    setWindowTitle(QCoreApplication::applicationName());
}

// Remove staff function
void AdminDashBoard::removeStaff() {
    int id = currentID;
    // Remove the staff from all courses
    for (auto &p : staffList[id]->getCourses()) {
        if (p == 0)
            continue;
        staffList[id]->removeCourse(p);
    }

    // Add the removing action to the admins log
    QString firstName = QString::fromStdString(staffList[id]->getFirstName());
    QString lastName = QString::fromStdString(staffList[id]->getFirstName());
    QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
    ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " removed staff member: " + firstName + " " + lastName + ".");

    // Remove the staff from the staff list
    delete staffList[id];
    staffList.erase(id);

    on_staffBtn_clicked();
    on_return_clicked();
    QCoreApplication::setApplicationName("Elearn. - Staff");
    setWindowTitle(QCoreApplication::applicationName());
}

// Remove course function
void AdminDashBoard::removeCourse() {
    int id = currentID;
    // Remove enrolled staff from the course
    for (auto &p : courses[id]->staff) {
        if (p == 0)
            continue;
        staffList[p]->removeCourse(id);
    }

    // Remove enrolled students from the course
    for (auto &p : courses[id]->students) {
        if (p.first == 0)
            continue;
        students[p.first]->removeCourse(id);
    }

    // Remove the hall from the course date
    vector<string> timeHalls = hallDates[courses[id]->getLectureDay() * 10 + courses[id]->getLectureTime()];
    if (timeHalls.size() > 0) {
        for (int i = 0; i < int(timeHalls.size()); i++) {
            if (timeHalls[i] == courses[id]->getLectureHall())
                hallDates[courses[id]->getLectureDay() * 10 + courses[id]->getLectureTime()][i] = "";
        }
    }

    // Add the removing action to the admins log
    QString courseName = QString::fromStdString(courses[id]->getCourseName());
    QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
    ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " removed course: " + courseName + ".");

    // Remove the course from the courses list
    delete courses[id];
    courses.erase(id);

    on_coursesBtn_clicked();
    on_return_clicked();
    QCoreApplication::setApplicationName("Elearn. - Courses");
    setWindowTitle(QCoreApplication::applicationName());
}

// Remove student button
void AdminDashBoard::on_studentsRemove_clicked() {
    Dialog *dial = new Dialog(this, "Remove Student", "Are you sure you want to remove the student " + QString::fromStdString(students[currentID]->getFirstName() + " " + students[currentID]->getLastName()) + "?");
    dial->setWindowTitle("Remove " + QString::fromStdString(students[currentID]->getFirstName() + " " + students[currentID]->getLastName()));
    if (dial->exec())
        removeStudent();
}

// Remove staff button
void AdminDashBoard::on_staffRemoveBtn_clicked() {
    Dialog *dial = new Dialog(this, "Remove Course", "Are you sure you want to remove the staff " + QString::fromStdString(staffList[currentID]->getFirstName() + " " + staffList[currentID]->getLastName()) + "?");
    dial->setWindowTitle("Remove " + QString::fromStdString(staffList[currentID]->getFirstName() + " " + staffList[currentID]->getLastName()));
    if (dial->exec())
        removeStaff();
}

// Remove course button
void AdminDashBoard::on_courseDetailsRemoveBtn_clicked() {
    Dialog *dial = new Dialog(this, "Remove Student", "Are you sure you want to remove the course " + QString::fromStdString(courses[currentID]->getCourseName()) + "?");
    dial->setWindowTitle("Remove " + QString::fromStdString(courses[currentID]->getCourseName()));
    if (dial->exec())
        removeCourse();
}

// Edit students information button
void AdminDashBoard::on_studentsEdit_clicked() {
    // Get the student information and set the add student values with them
    Student* student = students[currentID];
    ui->tabs->setCurrentIndex(5);
    ui->studentsFirstEdit->setText(QString::fromStdString(student->getFirstName()));
    ui->studentsLastEdit->setText(QString::fromStdString(student->getLastName()));
    ui->studentsEmailLast->setText(QString::fromStdString(student->getEmail()));
    ui->studentsMobileEdit->setText(QString::fromStdString(student->getMobile()).remove(0, 2));
    ui->studentsGradeBox->setCurrentText(QString::fromStdString(years[student->getGrade()]) + " year");

    // If the student is not at the preparatory year, choose their department
    if (student->getGrade() != 0) {
        ui->studentsDepartBox->setCurrentText(QString::fromStdString(departments[student->getDepartment()]));
    }

    ui->studentsGenderBox->setCurrentText(QString::fromStdString(student->getGender()));
    ui->studentsAgeSpin->setValue(student->getAge());
    editMode = true;
}

// Edit staff information
void AdminDashBoard::on_staffEditBtn_clicked() {
    // Get the staff information and set the add staff values with them
    Staff* staff = staffList[currentID];
    ui->tabs->setCurrentIndex(3);
    editMode = true;

    ui->staffFirstEdit->setText(QString::fromStdString(staff->getFirstName()));
    ui->staffLastEdit->setText(QString::fromStdString(staff->getLastName()));
    ui->staffEmailEdit->setText(QString::fromStdString(staff->getEmail()));
    ui->staffNumberEdit->setText(QString::fromStdString(staff->getMobile()).remove(0, 2));
    ui->staffDepartBox->setCurrentText(QString::fromStdString(departments[staff->getDepartment()]));
    ui->staffGenderBox->setCurrentText(QString::fromStdString(staff->getGender()));
    ui->staffGenderBox_3->setCurrentText(QString::fromStdString(positions[staff->getPosition()]));
    ui->staffAgeSpin->setValue(staff->getAge());
}

// Edit course information
void AdminDashBoard::on_courseDetailsEditBtn_clicked() {
    // Get the course information and set the add course values with them
    Course* course = courses[currentID];
    ui->tabs->setCurrentIndex(1);
    editMode = true;
    addedStudents.clear();
    removedStudents.clear();
    addedStaff.clear();
    removedStaff.clear();
    deletedStaff.clear();
    deletedStudents.clear();

    ui->courseNameEdit->setText(QString::fromStdString(course->getCourseName()));
    ui->dayBox->setCurrentText(QString::fromStdString(days[course->getLectureDay()]));
    ui->timeBox->setCurrentText(QString::fromStdString(times[course->getLectureTime()]));
    ui->courseDepartBox->setCurrentText(QString::fromStdString(departments[course->getCourseDepartment()]));

    // Remove the hall from the reserved halls
    vector<string> timeHalls = hallDates[course->getLectureDay() * 10 + course->getLectureTime()];
    if (timeHalls.size() > 0) {
        for (int i = 0; i < int(timeHalls.size()); i++) {
            if (timeHalls[i] == course->getLectureHall())
                hallDates[course->getLectureDay() * 10 + course->getLectureTime()][i] = "";
        }
    }

    ui->hallBox->setCurrentText(QString::fromStdString(course->getLectureHall()));
    ui->addStaffBox->clear();
    ui->addStudentsBox->clear();

    ui->gradeSpin->setMinimum(50);
    ui->gradeSpin->setMaximum(250);
    ui->addStudentsGrade->setMinimum(0);
    ui->addStudentsGrade->setMaximum(course->getCourseGrade());
    ui->addStudentsGrade->setValue(0);

    ui->gradeSpin->setValue(course->getCourseGrade());

    setAvailableHalls(course->getLectureDay(), course->getLectureTime());
    setAvailableLists(course->getLectureDay(), course->getLectureTime());

    // Fill the course staff table
    ui->staffTable->setRowCount(0);
    for (auto &p : course->staff) {
        if (p == 0)
            continue;
        int rowCount = ui->staffTable->rowCount();
        ui->staffTable->setRowCount(rowCount + 1);
        addedStaff[p] = ui->staffTable->rowCount() - 1;
        QTableWidgetItem *items [] = {
            new QTableWidgetItem(QString::fromStdString(staffList[p]->getFirstName() + " " + staffList[p]->getLastName())),
            new QTableWidgetItem(QString::fromStdString(positions[staffList[p]->getPosition()]))
        };

        QIcon minusIcon(":/img/remove.png");
        items[0]->setIcon(minusIcon);

        const size_t count = sizeof(items) / sizeof(QTableWidgetItem*);
        for (size_t column = 0; column < count; column++) {
            items[column]->setTextAlignment(Qt::AlignCenter);
            ui->staffTable->setItem(rowCount, column, items[column]);
        }
    }

    // Fill the course students table
    ui->studentsTable->setRowCount(0);
    for (auto &p : course->students) {
        if (p.first == 0)
            continue;
        int courseGrade = p.second;
        int rowCount = ui->studentsTable->rowCount();
        ui->studentsTable->setRowCount(rowCount + 1);
        addedStudents[p.first][0] = p.second;
        addedStudents[p.first][1] = ui->studentsTable->rowCount() - 1;
        QTableWidgetItem *items [] = {
            new QTableWidgetItem(QString::fromStdString(students[p.first]->getFirstName() + " " + students[p.first]->getLastName())),
            new QTableWidgetItem(QString::number(courseGrade))
        };

        QIcon minusIcon(":/img/remove.png");
        items[0]->setIcon(minusIcon);

        const size_t count = sizeof(items) / sizeof(QTableWidgetItem*);
        for (size_t column = 0; column < count; column++) {
            items[column]->setTextAlignment(Qt::AlignCenter);
            ui->studentsTable->setItem(rowCount, column, items[column]);
        }
    }
}

// Edit admin information button
void AdminDashBoard::on_adminEdit_clicked() {
    // Get the admin information and set the add admin values with them
    Admin *admin = admins[currentAdmin];
    ui->tabs->setCurrentIndex(7);
    editMode = true;

    ui->adminFirstEdit->setText(QString::fromStdString(admin->getFirstName()));
    ui->adminLastEdit->setText(QString::fromStdString(admin->getLastName()));
    ui->adminEmailEdit->setText(QString::fromStdString(admin->getEmail()));
    ui->admingNumberEdit->setText(QString::fromStdString(admin->getMobile()).remove(0, 2));
    ui->adminUserEdit->setText(QString::fromStdString(admin->getUsername()).toUpper());
    ui->adminPasswordEdit->clear();
    ui->adminConfirmEdit->clear();
    ui->adminAgeSpin->setValue(admin->getAge());
    ui->adminGenderBox->setCurrentText(QString::fromStdString(admin->getGender()));
}


/// Admin Tools

// Hall validators
void AdminDashBoard::on_hallLineEdit_textChanged(const QString &hall) {
    labelValidate(ui->hallLineEdit, hallRegEx.match(hall).hasMatch() && !count(halls.begin(), halls.end(), hall.toStdString()), "rgb(27, 27, 27)");
    if (hall.size() == 0)
        labelValidate(ui->hallLineEdit, true, "rgb(27, 27, 27)");
}

// Department code validator
void AdminDashBoard::on_departCode_textChanged(const QString &code) {
    ui->departCode->setText(code.toUpper());
    labelValidate(ui->departCode, departments.find(code.toLower().toStdString()) == departments.end() && code.size() == 3, "rgb(27, 27, 27)");
    if (code.size() == 0)
        labelValidate(ui->departCode, true, "rgb(27, 27, 27)");
}


// Department name validator
void AdminDashBoard::on_departName_textChanged(const QString &name) {
    labelValidate(ui->departName, name.size() > 15, "rgb(27, 27, 27)");
    if (name.size() == 0)
        labelValidate(ui->departName, true, "rgb(27, 27, 27)");
}

// Add new hall button
void AdminDashBoard::on_addHallBtn_clicked() {
    // If hall's size is more than 4 characters and doesn't exist in the halls list, add it to the halls list
    if (ui->hallLineEdit->text().size() >= 4 && !count(halls.begin(), halls.end(), ui->hallLineEdit->text().toStdString())) {
        halls.push_back(ui->hallLineEdit->text().toStdString());
        ui->hallsList->addItem(ui->hallLineEdit->text());
        QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
        ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " added new hall: " + ui->hallLineEdit->text() + ".");
        ui->hallLineEdit->clear();
    }
}

// Add new department button
void AdminDashBoard::on_addDepartBtn_clicked() {
    // Get the input department code & department name
    QString code = ui->departCode->text();
    QString name = ui->departName->text();

    // If the department code is equal 3 characters and name is more than 15 characters and department code deosn't already exist
    if (code.size() == 3 && (departments.find(code.toLower().toStdString()) == departments.end()) && name.size() > 15) {
        // Add the department to the departments list
        departments[code.toLower().toStdString()] = name.toStdString();
        ui->departCode->clear();
        ui->departName->clear();

        int rowCount = ui->departmentsTable->rowCount();
        ui->departmentsTable->setRowCount(rowCount + 1);
        QTableWidgetItem *items [] = {
            new QTableWidgetItem(code.toUpper()),
            new QTableWidgetItem(name),
        };

        for (short column = 0; column < 2; column++) {
            items[column]->setTextAlignment(Qt::AlignCenter);
            ui->departmentsTable->setItem(rowCount, column, items[column]);
        }

        // Add new department to the departments combo box
        ui->staffDepartBox->addItem(QString::fromStdString(name.toStdString()), QVariant(QString::fromStdString(code.toLower().toStdString())));
        ui->courseDepartBox->addItem(QString::fromStdString(name.toStdString()), QVariant(QString::fromStdString(code.toLower().toStdString())));

        QString dateTime = "[" + QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss") + "] ";
        ui->logList->addItem(dateTime + QString::fromStdString(admins[currentAdmin]->getUsername()) + " added new department: " + name + ".");
    }
}

/// Export CSV file

// Exporting function
void AdminDashBoard::exportCSV() {
    // Update or create file for the admin log
    QFile logFile("../LMS/database/admin.log");
    logFile.open(QIODevice::WriteOnly);
    QApplication::processEvents();
    QTextStream logOut(&logFile);

    // Add each admin log item to the log file
    for (int i = 0; i < ui->logList->count(); i++) {
        logOut << ui->logList->item(i)->text() << "\n";
    }
    logOut << "************************************************************************";

    // Update or creat CSV file for the database
    QFile dataFile("../LMS/database/data.csv");
    dataFile.open(QIODevice::WriteOnly);
    QApplication::processEvents();
    QTextStream out(&dataFile);

    // Saving halls list in CSV file
    for (auto const &p : halls) {
        out << QString::fromStdString(p) << ",";
    }
    out << "\n************************************************************************\n";

    // Saving departments list in CSV file
    out << "DepartmentCode,DepartmentName\n";
    for (auto const &p : departments) {
        if (p.first != "")
            out << QString::fromStdString(p.first) << "," << QString::fromStdString(p.second) << ",\n";
    }
    out << "************************************************************************\n";

    // Saving admins list in CSV file
    out << "adminID,adminFirstName,adminLastName,adminEmail,adminPhone,adminGender,adminAge,adminUsername,adminPassword\n";
    for (auto p : admins){
        Admin *admin = p.second;
        out << p.first << "," << QString::fromStdString(admin->getFirstName()) << "," << QString::fromStdString(admin->getLastName()) << ","
            << QString::fromStdString(admin->getEmail()) << "," <<
            QString::fromStdString(admin->getMobile()) << "," << QString::fromStdString(admin->getGender()) << "," << admin->getAge() << "," <<
            QString::fromStdString(admin->getUsername()) << "," <<QString::fromStdString(admin->getPassword());
        out << "\n";
    }
    out<<"************************************************************************\n";

    // Saving courses details in CSV file
    out << "courseID,courseName,lectureDay,lectureTime,lectureHall,courseGrade,courseDepartment,[courseStudent],[courseStaff]\n";
    for(auto p : courses){
        Course *course = p.second;
        QString tmp = QString::fromStdString(course->getCourseName());
        out << p.first << "," << tmp << "," << course->getLectureDay() << "," << course->getLectureTime() << "," <<
               QString::fromStdString(course->getLectureHall()) << "," << course->getCourseGrade() << ","
               << QString::fromStdString(course->getCourseDepartment()) << ",";
        for (auto s : course->students){
            if (s.first != 0)
                out << s.first << "-" << s.second << "-";
        }
        out << ",";
        for(auto s : course->staff){
            if (s != 0)
                out << s << "-";
        }
        out << "\n";
    }
    out << "************************************************************************\n";

    // Saving students details to the CSV file
    out << "studentID,studentFirstName,studentLastName,studentEmail,studentPhone,studentGender,studentAge,studentYear,studentDepartment,[studentCourses]\n";
    for (auto p : students){
        Student *student = p.second;
        out << p.first << "," << QString::fromStdString(student->getFirstName()) << "," << QString::fromStdString(student->getLastName()) << ","
            << QString::fromStdString(student->getEmail()) << "," <<
            QString::fromStdString(student->getMobile()) << "," << QString::fromStdString(student->getGender()) << "," << student->getAge() << "," <<
            student->getGrade() << "," <<QString::fromStdString(student->getDepartment())<<",";
        for (auto c : student->getCourses())
            if (c.first != 0)
                out << c.first << "-" << c.second << "-";
        out << "\n";
    }
    out<<"************************************************************************\n";

    // Saving staff details to the CSV file
    out << "staffID,staffFirstName,staffLastName,staffEmail,staffPhone,staffGender,staffAge,staffPosition,staffDepartment,[staffCourses]\n";
    for (auto p :staffList){
        Staff *staff = p.second;
        out << p.first << "," << QString::fromStdString(staff->getFirstName()) << "," << QString::fromStdString(staff->getLastName()) <<
               "," << QString::fromStdString(staff->getEmail()) << "," << QString::fromStdString(staff->getMobile()) << "," <<
               QString::fromStdString(staff->getGender()) << "," << staff->getAge() << "," <<
               staff->getPosition() << "," << QString::fromStdString(staff->getDepartment())<<",";
        for (auto &c :staff->getCourses() ){
            if (c != 0)
                out << c << "-";
        }
        out << "\n";
    }
    out<<"************************************************************************\n";
    dataFile.close();
}

// Export button
void AdminDashBoard::on_exportBtn_clicked() {
    Dialog *dial = new Dialog(this, "Export to CSV File", "Are you sure you want to export you current data to CSV file?.");
    dial->setWindowTitle("Export to CSV File");
    if (dial->exec())
        exportCSV();
}

/// Import CSV file

// Import function
void AdminDashBoard::importCSV() {
    // Remove all students
    for (auto &p : students) {
        for (auto &c : p.second->getCourses())
            if (c.first != 0)
                p.second->removeCourse(c.first);
        delete p.second;
    }

    // Remove all staff
    for (auto &p : staffList) {
        for (auto &c : p.second->getCourses())
            if (c != 0)
                p.second->removeCourse(c);
        delete p.second;
    }

    // Remove all courses
    for (auto &p : courses) {
        delete p.second;
    }

    // Clear all lists
    students.clear();
    staffList.clear();
    courses.clear();
    halls.clear();
    departments.clear();
    hallDates.clear();

    // Clear all dashboard data
    dashboard->studentsNum = dashboard->maleStudents = dashboard->femaleStudents = 0;
    dashboard->staffNum = dashboard->maleStaff = dashboard->femaleStaff = 0;
    dashboard->profNum = dashboard->taNum = dashboard->coursesNum = 0;
    dashboard->maleProfNum = dashboard->maleTaNum = 0;
    dashboard->departmentStudentsNum.clear();
    dashboard->yearsStudentsNum.clear();
    dashboard->departmentCoursesNum.clear();
    dashboard->departmentStaffNum.clear();

    // Clear admins log list & reload it from the log file
    ui->logList->clear();
    QFile logFile("../LMS/database/admin.log");
    logFile.open(QIODevice::ReadOnly);
    QApplication::processEvents();
    QTextStream logStream(&logFile);

    // Add each line to the admin logs list
    while (true) {
        QString line = logStream.readLine();
        if (line == "************************************************************************") {
            break;
        }
        ui->logList->addItem(line);
    }

    // Load the database CSV file
    QFile dataFile("../LMS/database/data.csv");
    dataFile.open(QIODevice::ReadOnly);
    QApplication::processEvents();
    QTextStream stream(&dataFile);

    int count = 0;
    while (count != 6) {
        // Each stars line, increase the count
        QString line = stream.readLine();
        if (line == "************************************************************************") {
            count++;
            continue;
        }

        // Split the line using commas
        QStringList lineSplit = line.split(",");
        // Add halls to the halls list (all in one line)
        if (count == 0) {
            for (auto const &p : lineSplit) {
                if (p != "") {
                    halls.push_back(p.toStdString());
                }
            }
        }

        // Add departments to the departments list (line by line)
        else if (count == 1) {
            if (lineSplit[0] != "DepartmentCode")
                departments[lineSplit[0].toStdString()] = lineSplit[1].toStdString();
        }

        // Add admins to the admins list
        else if (count == 2) {
            if (lineSplit[0] != "adminID") {
                int adminID = lineSplit[0].toInt();
                Admin *newAdmin = new Admin(adminID);
                newAdmin->setFirstName(lineSplit[1].toStdString());
                newAdmin->setLastName(lineSplit[2].toStdString());
                newAdmin->setEmail(lineSplit[3].toStdString());
                newAdmin->setMobile(lineSplit[4].toStdString());
                newAdmin->setGender(lineSplit[5].toStdString());
                newAdmin->setAge(lineSplit[6].toInt());
                newAdmin->setUsername(lineSplit[7].toStdString());
                newAdmin->setPassword(lineSplit[8].toStdString());
                admins[adminID] = newAdmin;
                currentAdminID = adminID + 1;
            }
        }

        // Add courses to the courses list
        else if (count == 3) {
            if (lineSplit[0] != "courseID") {
                int courseID = lineSplit[0].toInt();
                Course *newCourse = new Course(courseID);
                newCourse->setCourseName(lineSplit[1].toStdString());
                newCourse->setLectureDay(lineSplit[2].toInt());
                newCourse->setLectureTime(lineSplit[3].toInt());
                newCourse->setLectureHall(lineSplit[4].toStdString());
                newCourse->setCourseGrade(lineSplit[5].toInt());
                newCourse->setCourseDepartment(lineSplit[6].toStdString());
                courses[courseID] = newCourse;
                hallDates[lineSplit[2].toInt() * 10 + lineSplit[3].toInt()].push_back(lineSplit[4].toStdString());
                currentCourseID = courseID + 1;
            }
        }

        // Add students to the students list
        else if (count == 4) {
            if (lineSplit[0] != "studentID") {
                int studentID = lineSplit[0].toInt();
                Student *newStudent = new Student(studentID);
                newStudent->setFirstName(lineSplit[1].toStdString());
                newStudent->setLastName(lineSplit[2].toStdString());
                newStudent->setEmail(lineSplit[3].toStdString());
                newStudent->setMobile(lineSplit[4].toStdString());
                newStudent->setGender(lineSplit[5].toStdString());
                newStudent->setAge(lineSplit[6].toInt());
                newStudent->setGrade(lineSplit[7].toInt());
                newStudent->setDepartment(lineSplit[8].toStdString());
                students[studentID] = newStudent;
                currentStudentID = studentID + 1;

                // Get the student courses and add them course by course
                if (lineSplit[9] != "") {
                    QStringList courses = lineSplit[9].split("-");
                    for (int i = 0; i < courses.size() - 1; i += 2) {
                        if (courses[i].toInt() != 0)
                            newStudent->addCourse(courses[i].toInt(), courses[i + 1].toInt());
                    }
                }
            }
        }

        // Add staff to the staff list
        else if (count == 5) {
            if (lineSplit[0] != "staffID") {
                int staffID = lineSplit[0].toInt();
                Staff *newStaff = new Staff(staffID);
                newStaff->setFirstName(lineSplit[1].toStdString());
                newStaff->setLastName(lineSplit[2].toStdString());
                newStaff->setEmail(lineSplit[3].toStdString());
                newStaff->setMobile(lineSplit[4].toStdString());
                newStaff->setGender(lineSplit[5].toStdString());
                newStaff->setAge(lineSplit[6].toInt());
                newStaff->setPosition(lineSplit[7][0].toLatin1());
                newStaff->setDepartment(lineSplit[8].toStdString());
                staffList[staffID] = newStaff;
                currentStaffID = staffID + 1;
                // Get the staff courses and add them course by course
                if (lineSplit[9] != "") {
                    QStringList courses = lineSplit[9].split("-");
                    for (int i = 0; i < courses.size() - 1; i++) {
                        if (courses[i].toInt() != 0)
                            newStaff->addCourse(courses[i].toInt());
                    }
                }
            }
        }
    }
}

// Import CSV button
void AdminDashBoard::on_importBtn_clicked() {
    Dialog *dial = new Dialog(this, "Import CSV File", "Are you sure you want to import the CSV file?<br/>All your current data will be removed.");
    dial->setWindowTitle("Import CSV File");
    if (dial->exec())
        importCSV();
}

/// Filters

// Open filters button
void AdminDashBoard::on_filtterBtn_clicked() {
    // Clear all filters
    yearFilters.clear();
    genderFilters.clear();
    departFilters.clear();
    positionFilters.clear();
    sliderFilter = 0;

    // Clear filter department list
    ui->departList->clear();
    ui->filterAgeLabel->setText("Age");

    if (!filterOpen) {
        // Show filters if filters aren't open
        ui->filters->show();
        // Apply all filters
        genderFilters = {"Male", "Female"};
        yearFilters = {0, 1, 2, 3, 4};
        positionFilters = {'p', 't'};

        // Add departments to departments filter
        for (auto const &p : departments) {
            if (p.first != "") {
                QListWidgetItem *newItem = new QListWidgetItem();
                newItem->setText(QString::fromStdString(p.first).toUpper());
                newItem->setCheckState(Qt::Checked);
                ui->departList->addItem(newItem);
                departFilters.push_back(p.first);
            }
        }

        departFilters.push_back("");
        ui->filterYearFrame->show();
        ui->filterGenderFrame->show();
        ui->filterYearLabel->setText("Year");
        ui->prepBox->setText("Preparatory Year");
        ui->firstBox->setText("First Year");
        ui->secondBox->show();
        ui->thirdBox->show();
        ui->fourthBox->show();

        ui->filterAgeSpin->setMinimum(16);
        ui->filterAgeSpin->setMaximum(16 + 2 * 5);

        // Change year filters, set new minimum and maximum age
        if (mode == "staff") {
            ui->filterYearFrame->show();
            ui->filterYearLabel->setText("Position");
            ui->prepBox->setText("Professor");
            ui->firstBox->setText("Teaching Assistant");
            ui->secondBox->hide();
            ui->thirdBox->hide();
            ui->fourthBox->hide();
            ui->filterAgeSpin->setMinimum(16 + (sizeof(years) / sizeof(years[0])));
            ui->filterAgeSpin->setMaximum(65);
        }

        // Remove year, department filters
        if (mode == "admins") {
            ui->filterDepartFrame->hide();
            ui->filterYearFrame->hide();
            ui->filterAgeSpin->setMinimum(22);
            ui->filterAgeSpin->setMaximum(60);
        }

        // Remove gender, year filters. Change age filter to grade filters
        if (mode == "courses") {
            ui->filterGenderFrame->hide();
            ui->filterYearFrame->hide();
            ui->filterDepartFrame->show();
            ui->filterAgeLabel->setText("Grade");
            ui->filterAgeSpin->setMinimum(50);
            ui->filterAgeSpin->setMaximum(250);
        }

        ui->filterSlider->setMinimum(ui->filterAgeSpin->minimum());
        ui->filterSlider->setMaximum(ui->filterAgeSpin->maximum());
        ui->filterAgeSpin->setValue(ui->filterAgeSpin->minimum());
        sliderFilter = ui->filterAgeSpin->minimum();

        ui->maleBox->setCheckState(Qt::Checked);
        ui->femaleBox->setCheckState(Qt::Checked);
        ui->prepBox->setCheckState(Qt::Checked);
        ui->firstBox->setCheckState(Qt::Checked);
        ui->secondBox->setCheckState(Qt::Checked);
        ui->thirdBox->setCheckState(Qt::Checked);
        ui->fourthBox->setCheckState(Qt::Checked);
        filterOpen = true;
    } else {
        // Close filters
        filterOpen = false;
        ui->filters->hide();
    }
    fillTables();
}

// Male filter
void AdminDashBoard::on_maleBox_stateChanged() {
    if (ui->maleBox->isChecked()) {
        // Add male to the filters
        genderFilters.push_back("Male");
    } else {
        // Remove male from the filters
        for (int i = 0; i < int(genderFilters.size()); i++) {
            if (genderFilters[i] == "Male")
                genderFilters[i] = "";
        }
    }
    fillTables();
}

// Female filter
void AdminDashBoard::on_femaleBox_stateChanged() {
    if (ui->femaleBox->isChecked()) {
        // Add female to the filters
        genderFilters.push_back("Female");
    } else {
        // Remove female from the filters
        for (int i = 0; i < int(genderFilters.size()); i++) {
            if (genderFilters[i] == "Female")
                genderFilters[i] = "";
        }
    }
    fillTables();
}

// Year filter
void AdminDashBoard::checkYear(QCheckBox* checkBox, int value) {
    if (checkBox->isChecked()) {
        // Add year to filters
        yearFilters.push_back(value);
    } else {
        // Remove year from filters
        for (int i = 0; i < int(yearFilters.size()); i++) {
            if (yearFilters[i] == value)
                yearFilters[i] = -1;
        }
    }
    fillTables();
}

// Preparatory year & professor filter
void AdminDashBoard::on_prepBox_stateChanged() {
    if (mode == "students")
        // Change preparatory year filter
        checkYear(ui->prepBox, 0);
    else if (mode == "staff") {
        if (ui->prepBox->isChecked()) {
            // Add professor to the filters
            positionFilters.push_back('p');
        } else {
            // Remove professor to the filters
            for (int i = 0; i < int(positionFilters.size()); i++) {
                if (positionFilters[i] == 'p')
                        positionFilters[i] = 'n';
            }
        }
        fillTables();
    }
}

// First year & TA filter
void AdminDashBoard::on_firstBox_stateChanged() {
    if (mode == "students")
        // Change first year filter
        checkYear(ui->firstBox, 1);
    else if (mode == "staff") {
        if (ui->firstBox->isChecked()) {
            // Add TA to filters
            positionFilters.push_back('t');
        } else {
            // Remove TA from filters
            for (int i = 0; i < int(positionFilters.size()); i++) {
                if (positionFilters[i] == 't')
                    positionFilters[i] = 'n';
            }
        }
        fillTables();
    }
}

// Second year filter
void AdminDashBoard::on_secondBox_stateChanged() {
    checkYear(ui->secondBox, 2);
}

// Third year filter
void AdminDashBoard::on_thirdBox_stateChanged() {
    checkYear(ui->thirdBox, 3);
}

// Fourth year filter
void AdminDashBoard::on_fourthBox_stateChanged() {
    checkYear(ui->fourthBox, 4);
}

// Departments filters
void AdminDashBoard::on_departList_itemClicked(QListWidgetItem *item) {
    if (item->checkState() == Qt::Unchecked) {
        // Add department to filters
        item->setCheckState(Qt::Checked);
        departFilters.push_back(item->text().toLower().toStdString());
    } else {
        // Remove department from filters
        item->setCheckState(Qt::Unchecked);
        for (int i = 0; i < int(departFilters.size()); i++) {
            if (departFilters[i] == item->text().toLower().toStdString())
                departFilters[i] = "";
        }
    }
    fillTables();
}

// Age & Grade filters
void AdminDashBoard::on_filterAgeSpin_valueChanged(int value) {
    sliderFilter = value;
    ui->filterSlider->setValue(value);
    fillTables();
}

// Slider value changed by spin box change
void AdminDashBoard::on_filterSlider_valueChanged(int value) {
    ui->filterAgeSpin->setValue(value);
}

