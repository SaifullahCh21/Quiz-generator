#include<iostream>
#include<string>
#include<fstream>
#include <chrono>
#include <thread>

using namespace std;

struct teacher
{
    int CNIC, numCourse;
    string name, department, loginID, password, * courses;

};
struct student
{
    string name, department, section, password, loginID;
    float gpa, cgpa;
    int CNIC, quizMarks;

};


//Functions

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//------------*********---------STUDENT BLOCK I ADMIN---------***********--------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

void captureStudentData(student* s, int i)
{
    cout << "----------- Student No. " << i + 1 << " ---------------" << endl;

    cout << "         Assign a unique Login ID: ";
    cin >> s[i].loginID;

    cout << "         Enter Student's Name: ";
    cin.ignore();
    getline(cin, s[i].name);

    cout << "         Assign a unique Password: ";
    cin >> s[i].password;

    cout << "         Enter CNIC: ";
    cin >> s[i].CNIC;

    cout << "         Enter Department: ";
    cin.ignore();
    getline(cin, s[i].department);

    cout << "         Enter Section: ";
    cin >> s[i].section;

    cout << "         Enter GPA: ";
    cin >> s[i].gpa;

    cout << "         Enter CGPA: ";
    cin >> s[i].cgpa;

    cout << "         Enter Quiz Marks: ";
    cin >> s[i].quizMarks;

    cout << endl << "-------------------------------------------" << endl;
}

void saveStudentDataToFile(student* s, int count)
{
    ofstream studentsData("studentsData.txt", ios::app);

    if (studentsData.is_open())
    {
        for (int i = 0; i < count; i++)
        {

            studentsData << "Login ID: " << s[i].loginID << endl;
            studentsData << "Name: " << s[i].name << endl;
            studentsData << "Password: " << s[i].password << endl;
            studentsData << "CNIC: " << s[i].CNIC << endl;
            studentsData << "Department: " << s[i].department << endl;
            studentsData << "Section: " << s[i].section << endl;
            studentsData << "GPA: " << s[i].gpa << endl;
            studentsData << "CGPA: " << s[i].cgpa << endl;
            studentsData << "Quiz Marks: " << s[i].quizMarks << endl;
            studentsData << "--------------------" << endl;
        }
        studentsData.close();
        cout << "Student data saved successfully!" << endl;
    }
    else
    {
        cout << "OOPS!!!!! Something went wrong, File could not be opened" << endl;
    }
}

void addStudentsData(int count)
{
    student* s = new student[count];

    for (int i = 0; i < count; i++)
    {
        captureStudentData(s, i); // Predefined function to take the input data of a student
    }

    saveStudentDataToFile(s, count); // Predefined function to save the data of students to a file

    delete[] s; // Free the memory after storing the data in the file
}

void showAllStudents()
{
    ifstream studentsData("studentsData.txt");
    string data;

    if (studentsData.is_open())
    {
        cout << "---------- STUDENTS DATA ----------" << endl;

        while (getline(studentsData, data))
        {
            cout << data << endl; // Print each line
        }
        studentsData.close();
    }
    else
    {
        cout << "OOOPSSS!!!! Could not open the file. Make sure 'studentsData.txt' exists." << endl;
    }
}


void searchStudent()
{
    ifstream studentsData("studentsData.txt");
    string loginID, line, foundData = "";
    bool isFound = false;

    cout << "Enter the Login ID of the student to search: ";
    cin >> loginID;

    if (studentsData.is_open())
    {
        while (getline(studentsData, line))
        {
            if (line.find("Login ID: " + loginID) != string::npos)
            {
                isFound = true;
                foundData += line + "\n"; // Add the Login ID line
                // Read and save the next 8 lines (assuming each student has 9 lines of data in the file)
                for (int i = 0; i < 8; i++)
                {
                    getline(studentsData, line);
                    foundData += line + "\n";
                }
                break;
            }
        }
        studentsData.close();

        if (isFound)
        {
            cout << "Student Found!" << endl;
            cout << foundData;

            // Option to update student data
            char choice;
            cout << "Do you want to update this student's data? (y/n): ";
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                student updatedStudent;
                cout << "Enter new data for the student:" << endl;
                captureStudentData(&updatedStudent, 0); // Capture updated student data

                // Update student data in the file
                ifstream studentsData("studentsData.txt");
                ofstream tempFile("temp.txt");
                string currentLine;
                bool updateDone = false;

                if (studentsData.is_open() && tempFile.is_open())
                {
                    while (getline(studentsData, currentLine))
                    {
                        if (!updateDone && currentLine.find("Login ID: " + loginID) != string::npos)
                        {
                            tempFile << "Login ID: " << updatedStudent.loginID << endl;
                            tempFile << "Name: " << updatedStudent.name << endl;
                            tempFile << "Password: " << updatedStudent.password << endl;
                            tempFile << "CNIC: " << updatedStudent.CNIC << endl;
                            tempFile << "Department: " << updatedStudent.department << endl;
                            tempFile << "Section: " << updatedStudent.section << endl;
                            tempFile << "GPA: " << updatedStudent.gpa << endl;
                            tempFile << "CGPA: " << updatedStudent.cgpa << endl;
                            tempFile << "Quiz Marks: " << updatedStudent.quizMarks << endl;
                            tempFile << "--------------------" << endl;
                            // Skip old data

                            for (int i = 0; i < 8; i++)
                                getline(studentsData, currentLine);

                            updateDone = true;
                        }
                        else
                        {
                            tempFile << currentLine << endl; // Copy the rest of the file
                        }
                    }
                    studentsData.close();
                    tempFile.close();

                    // Replace the original file with the updated file
                    remove("studentsData.txt");
                    rename("temp.txt", "studentsData.txt");

                    cout << "Student data updated successfully!" << endl;
                }
                else
                {
                    cout << "OOPS! Something went wrong while updating the file!" << endl;
                }
            }
        }
        else
        {
            cout << "Student with Login ID: " << loginID << " not found!" << endl;
        }
    }
    else
    {
        cout << "OOOPSSS!!!! Could not open the file. Make sure 'studentsData.txt' exists." << endl;
    }
}



void clearAllStudents()
{
    ofstream studentsData("studentsData.txt", ios::out);
    if (studentsData.is_open())
    {
        cout << "File content has been cleared!" << endl;
        studentsData.close();
    }

    else
    {
        cout << "OOOPPPSSS!!!: We Could not open the file to clear content! Try Again" << endl;
    }

}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//------------*********------------TEACHERS BLOCK IN ADMIN----***********--------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------


void captureTeacherData(teacher* t, int i)
{
    cout << "-----------Teacher No. " << i + 1 << "---------------" << endl;

    cout << "         Enter teacher's name: ";
    cin.ignore();
    getline(cin, t[i].name);

    cout << "         Assign a unique LOG_IN ID to teacher: ";
    cin >> t[i].loginID;

    cout << "         Assign a unique Password: ";
    cin >> t[i].password;

    cout << "         Enter CNIC: ";
    cin >> t[i].CNIC;

    cout << "         Enter Department: ";
    cin >> t[i].department;

    cout << "         How many Courses does Teacher teach: ";
    cin >> t[i].numCourse;

    cout << endl << "-------------------------------------------" << endl;
}



void saveTeacherDataToFile(teacher* t, int s)
{
    ofstream teachersData("teachersData.txt", ios::app);
    if (teachersData.is_open())
    {
        for (int i = 0; i < s; i++)
        {

            teachersData << "Login ID: " << t[i].loginID << endl;
            teachersData << "Name: " << t[i].name << endl;
            teachersData << "Password: " << t[i].password << endl;
            teachersData << "CNIC: " << t[i].CNIC << endl;
            teachersData << "Department: " << t[i].department << endl;
            teachersData << "Number of Courses: " << t[i].numCourse << endl;
            teachersData << "--------------------" << endl;
        }
        teachersData.close();
        cout << "Teacher data saved successfully!" << endl;
    }
    else
    {
        cout << "OOPS!!!!! Something went wrong, File could not be opened" << endl;
    }
}

void addTeachersData(int s)
{
    teacher* t = new teacher[s];

    for (int i = 0; i < s; i++)
    {
        captureTeacherData(t, i); //A predefined function to take the input data of teacher from admin
    }

    saveTeacherDataToFile(t, s);  //A predefined function to save the  data of teacher in file named teachersData.txt

    delete[] t; // This will free the memory, because we have already stored the data into file

}


void showAllTeachers()
{
    ifstream teachersData("teachersData.txt");
    string data;

    if (teachersData.is_open())
    {
        cout << "---------- TEACHERS DATA ----------" << endl;
        while (getline(teachersData, data))
        {
            cout << data << endl; // Print each line
        }
        teachersData.close();
    }
    else
    {
        cout << "OOOPSSS!!!! Could not open the file. Make sure 'teachersData.txt' exists." << endl;
    }
}
void searchTeachers()
{
    ifstream teachersData("teachersData.txt");
    string loginID, line, foundData = "";
    bool isFound = false;

    cout << "Enter the Login ID of the teacher to search: ";
    cin >> loginID;

    if (teachersData.is_open())
    {
        while (getline(teachersData, line))
        {
            // Check if the current line contains the login ID
            if (line.find("Login ID: " + loginID) != string::npos)
            {
                isFound = true;
                foundData += line + "\n"; // Add the Login ID line
                // Read and save the next 6 lines (assuming each teacher has 7 lines of data)
                for (int i = 0; i < 6; i++)
                {
                    getline(teachersData, line);
                    foundData += line + "\n";
                }
                break;

            }
        }
        teachersData.close();

        if (isFound)
        {
            cout << "Teacher Found!" << endl;
            cout << foundData;

            // Option to update teacher data
            char choice;
            cout << "Do you want to update this teacher's data? (y/n): ";
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                teacher updatedTeacher;
                cout << "Enter new data for the teacher:" << endl;
                cout << "Name: ";
                cin.ignore();
                getline(cin, updatedTeacher.name);
                cout << "Login ID: ";
                cin >> updatedTeacher.loginID;
                cout << "Password: ";
                cin >> updatedTeacher.password;
                cout << "CNIC: ";
                cin >> updatedTeacher.CNIC;
                cout << "Department: ";
                cin >> updatedTeacher.department;
                cout << "Number of Courses: ";
                cin >> updatedTeacher.numCourse;

                // Update teacher data in the file
                ifstream teachersData("teachersData.txt");
                ofstream tempFile("temp.txt");
                string currentLine;
                bool updateDone = false;

                if (teachersData.is_open() && tempFile.is_open())
                {
                    while (getline(teachersData, currentLine))
                    {
                        if (!updateDone && currentLine.find("Login ID: " + loginID) != string::npos)
                        {
                            // Write the updated teacher data to the temp file
                            tempFile << "Name: " << updatedTeacher.name << endl;
                            tempFile << "Login ID: " << updatedTeacher.loginID << endl;
                            tempFile << "Password: " << updatedTeacher.password << endl;
                            tempFile << "CNIC: " << updatedTeacher.CNIC << endl;
                            tempFile << "Department: " << updatedTeacher.department << endl;
                            tempFile << "Number of Courses: " << updatedTeacher.numCourse << endl;
                            tempFile << "--------------------" << endl;
                            // Skip old data (the 6 lines after the Login ID)

                            for (int i = 0; i < 6; i++)
                            {
                                getline(teachersData, currentLine);
                            }
                            updateDone = true;
                        }
                        else
                        {
                            tempFile << currentLine << endl; // Copy the rest of the file
                        }
                    }
                    teachersData.close();
                    tempFile.close();

                    // Replace the original file with the updated file
                    remove("teachersData.txt");
                    rename("temp.txt", "teachersData.txt");

                    cout << "Teacher data updated successfully!" << endl;
                }
                else
                {
                    cout << "OOPS! Something went wrong while updating the file!" << endl;
                }
            }
        }
        else
        {
            cout << "Teacher with Login ID: " << loginID << " not found!" << endl;
        }
    }
    else
    {
        cout << "OOOPSSS!!!! Could not open the file. Make sure 'teachersData.txt' exists." << endl;
    }
}


void clearAllTeachers()
{
    ofstream teachersData("teachersData.txt", ios::out);

    if (teachersData.is_open())
    {
        cout << "File content has been cleared!" << endl;
        teachersData.close();
    }

    else
    {
        cout << "OOOPPPSSS!!!: We Could not open the file to clear content! Try Again" << endl;
    }

}

//------------*********------------LOGIN AS TEACHER BLOCK--------------***********--------------


bool teacherLogin()
{
    string loginID, password, cnic;
    string currentLoginID, currentName, currentPassword, currentCNIC, currentDepartment, currentCourses;

    // Prompt for Login ID and Password
    cout << "Enter Login ID: ";
    cin >> loginID;
    cout << "Enter Password: ";
    cin >> password;

    // Open the teachers file
    ifstream teachersFile("teachersData.txt");
    string line;
    bool loginSuccessful = false;

    // Validate Login ID and Password
    while (getline(teachersFile, line))
    {
        if (line.find("Login ID: ") != string::npos)
        {
            currentLoginID = line.substr(10); // Extract Login ID
        }
        else if (line.find("Name: ") != string::npos)
        {
            currentName = line.substr(6); // Extract Name
        }
        else if (line.find("Password: ") != string::npos)
        {
            currentPassword = line.substr(10); // Extract Password

            // Check Login ID and Password
            if (currentLoginID == loginID && currentPassword == password)
            {
                loginSuccessful = true;
                break;
            }
        }
    }

    teachersFile.close();

    if (loginSuccessful)
    {
        cout << "\nWelcome, " << currentName << "!" << endl; // greeting
        return true;
    }

    // If Login ID/Password validation fails, prompt for CNIC
    cout << "Invalid ID or Password! Enter your CNIC: ";
    cin >> cnic;

    teachersFile.open("teachersData.txt");
    bool found = false;

    // Search for teacher by CNIC
    while (getline(teachersFile, line))
    {
        if (line.find("Name: ") != string::npos)
        {
            currentName = line.substr(6); // Extract Name
        }
        else if (line.find("CNIC: ") != string::npos)
        {
            currentCNIC = line.substr(6); // Extract CNIC

            // Compare the extracted CNIC with the user input
            if (currentCNIC == cnic)
            {
                found = true;
            }
        }
        else if (line.find("Department: ") != string::npos)
        {
            currentDepartment = line.substr(12); // Extract Department
        }

        // At the end of a record, check if a match was found
        if (found)
        {
            cout << "\nWelcome, " << currentName << "!" << endl; // Personalized greeting
            teachersFile.close(); // Close file since match is found
            return true;          // Return true for successful login
        }

    }

    teachersFile.close();

    // If no match is found, notify the user
    cout << "Teacher with CNIC: " << cnic << " not found!\n";
    return false; // Return false for unsuccessful login
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//------------*********------------QUIZ CREATION--------------***********--------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

void createQuiz()
{
    string title, description, subjectTag, question, correctAnswer, fileName;
    int numQuestions, timeLimit;

    // Get quiz details
    cout << "Enter the title of the quiz: ";
    cin.ignore();
    getline(cin, title);

    cout << "Enter the description of the quiz: ";
    getline(cin, description);

    cout << "Enter the subject tag: ";
    getline(cin, subjectTag);

    cout << "Enter the number of questions: ";
    cin >> numQuestions;

    cout << "Enter the time limit for the quiz (in minutes): ";
    cin >> timeLimit;

    // Generate a file name based on the quiz title
    fileName = title + ".txt";

    // Save quiz metadata in the quiz file

    ofstream quizFile(fileName, ios::app);

    if (!quizFile.is_open())
    {
        cout << "Error: Unable to create quiz file." << endl;
        return;
    }

    quizFile << "Title: " << title << endl;
    quizFile << "Description: " << description << endl;
    quizFile << "Subject Tag: " << subjectTag << endl;
    quizFile << "Number of Questions: " << numQuestions << endl;
    quizFile << "Time Limit: " << timeLimit << " minutes" << endl;
    quizFile << "--------------------" << endl;

    cin.ignore(); // Clear the input buffer for question input

    // Get each question and options
    for (int i = 1; i <= numQuestions; ++i)
    {
        string options[4];

        cout << "\nEnter Question " << i << ": ";
        getline(cin, question);
        quizFile << "Question " << i << ": " << question << endl;

        for (int j = 0; j < 4; ++j)
        {
            cout << "Enter Option " << j + 1 << ": ";
            getline(cin, options[j]);
            quizFile << "Option " << j + 1 << ": " << options[j] << endl;
        }

        cout << "Enter the correct answer (correct option): ";
        getline(cin, correctAnswer);
        quizFile << "Correct Answer: " << correctAnswer << endl;
        quizFile << "--------------------" << endl;
    }

    quizFile.close();

    // Append the quiz name to quizList.txt
    ofstream quizListFile("quizList.txt", ios::app);

    if (quizListFile.is_open())
    {
        quizListFile << fileName << endl;
        quizListFile.close();
    }
    else
    {
        cout << "Error: Unable to update quiz list." << endl;
    }

    cout << "Quiz '" << title << "' created and saved successfully in " << fileName << "!" << endl;
}

void showQuizMarks()
{
    string quizName, line, currentQuiz;
    bool quizFound = false;

    cout << "Enter the name of the quiz to view marks: ";
    cin.ignore();
    getline(cin, quizName);

    ifstream resultsFile("studentResults.txt");

    if (!resultsFile.is_open())
    {
        cout << "Error: Unable to open 'studentResults.txt'!" << endl;
        return;
    }

    cout << "\n---------- Marks for Quiz: " << quizName << " ----------" << endl;

    while (getline(resultsFile, line))
    {
        if (line.find("Quiz: ") != string::npos)
        {
            currentQuiz = line.substr(6); // Extract Quiz Name
        }

        if (currentQuiz == quizName)
        {
            quizFound = true;
            cout << line << endl; // Print the current line (Quiz name or score)

            // Print subsequent lines until the next quiz or EOF

            while (getline(resultsFile, line) && line.find("Quiz: ") == string::npos)
            {
                cout << line << endl;
            }
            break; // Stop after showing the relevant quiz
        }
    }

    if (!quizFound)
    {
        cout << "No marks found for quiz: " << quizName << "." << endl;
    }

    resultsFile.close();
    cout << "----------------------------------------------------------" << endl;
}

void editQuizQuestion(const string& fileName, int questionNumber);

void viewOrEditQuiz()
{
    string quizName, fileName, line;
    cout << "Enter the name of the quiz you want to view: ";
    cin.ignore();
    getline(cin, quizName);

    fileName = quizName + ".txt";

    ifstream quizFile(fileName);

    if (!quizFile.is_open())
    {
        cout << "Error: Quiz '" << quizName << "' not found!" << endl;
        return;
    }

    cout << "\n---------- Quiz: " << quizName << " ----------" << endl;

    // Display the quiz content
    while (getline(quizFile, line))
    {
        cout << line << endl;
    }
    quizFile.close();

    // Ask if the teacher wants to edit the quiz
    char editChoice;
    cout << "\nDo you want to edit a question? (y/n): ";
    cin >> editChoice;

    if (editChoice == 'y' || editChoice == 'Y')
    {
        int questionNumber;
        cout << "Enter the question number you want to edit: ";
        cin >> questionNumber;

        editQuizQuestion(fileName, questionNumber); // Call function to edit the question
    }
}

void editQuizQuestion(const string& fileName, int questionNumber)
{
    ifstream quizFile(fileName);

    if (!quizFile.is_open())
    {
        cout << "Error: Unable to open quiz file for editing." << endl;
        return;
    }

    ofstream tempFile("temp.txt");

    if (!tempFile.is_open())
    {
        cout << "Error: Unable to create a temporary file for editing." << endl;
        return;
    }

    string line;
    int currentQuestion = 0;
    bool questionEdited = false;

    while (getline(quizFile, line))
    {
        if (line.find("Question ") != string::npos)
        {
            currentQuestion++;
        }

        if (currentQuestion == questionNumber && !questionEdited)
        {
            // Skip the current question's data
            cout << "Editing Question " << questionNumber << ":" << endl;

            string newQuestion, options[4], correctAnswer;

            cout << "Enter the new question: ";
            cin.ignore();
            getline(cin, newQuestion);

            for (int i = 0; i < 4; ++i)
            {
                cout << "Enter Option " << i + 1 << ": ";
                getline(cin, options[i]);
            }

            cout << "Enter the correct answer (text of the correct option): ";
            getline(cin, correctAnswer);

            // Write the updated question
            tempFile << "Question " << questionNumber << ": " << newQuestion << endl;
            for (int i = 0; i < 4; ++i)
            {
                tempFile << "Option " << i + 1 << ": " << options[i] << endl;
            }
            tempFile << "Correct Answer: " << correctAnswer << endl;
            tempFile << "--------------------" << endl;

            // Skip the existing question's lines
            while (getline(quizFile, line) && line.find("Question ") == string::npos)
            {
                // Move to the next question or EOF
            }
            questionEdited = true;

            if (!quizFile.eof())
            {
                tempFile << line << endl; // Write the next question header if not EOF
            }
        }
        else
        {
            // Copy the rest of the file as-is
            tempFile << line << endl;
        }
    }

    quizFile.close();
    tempFile.close();

    // Replace the original quiz file with the updated one
    remove(fileName.c_str());
    rename("temp.txt", fileName.c_str());

    if (questionEdited)
    {
        cout << "Question " << questionNumber << " has been successfully updated!" << endl;
    }
    else
    {
        cout << "Error: Question " << questionNumber << " not found in the quiz." << endl;
    }
}


void modifyStudentGPA()
{
    ifstream studentsFile("studentsData.txt");
    ofstream tempFile("temp.txt");
    string loginID, line;
    float newGPA;
    bool studentFound = false;

    cout << "Enter the Login ID of the student to modify GPA: ";
    cin >> loginID;

    cout << "Enter the new GPA: ";
    cin >> newGPA;

    if (studentsFile.is_open() && tempFile.is_open())
    {
        while (getline(studentsFile, line))
        {
            if (line.find("Login ID: " + loginID) != string::npos)
            {
                studentFound = true;
                tempFile << line << endl;  // Login ID
                getline(studentsFile, line);
                tempFile << line << endl;  // Name
                getline(studentsFile, line);
                tempFile << line << endl;  // Password
                getline(studentsFile, line);
                tempFile << line << endl;  // CNIC
                getline(studentsFile, line);
                tempFile << line << endl;  // Department
                getline(studentsFile, line);
                tempFile << line << endl;  // Section
                getline(studentsFile, line);
                tempFile << "GPA: " << newGPA << endl;  // Updated GPA
                getline(studentsFile, line); // Skip old GPA
                getline(studentsFile, line);
                tempFile << line << endl;  // CGPA
                getline(studentsFile, line);
                tempFile << line << endl;  // Quiz Marks
                getline(studentsFile, line);
                tempFile << line << endl;  // Divider
            }
            else
            {
                tempFile << line << endl;
            }
        }
        studentsFile.close();
        tempFile.close();

        if (studentFound)
        {
            remove("studentsData.txt");
            rename("temp.txt", "studentsData.txt");
            cout << "Student GPA updated successfully!" << endl;
        }
        else
        {
            cout << "Student with Login ID: " << loginID << " not found!" << endl;
        }
    }
    else
    {
        cout << "Error accessing the file!" << endl;
    }
}


void showStudentsBySection()
{
    ifstream studentsFile("studentsData.txt");

    if (!studentsFile.is_open())
    {
        cout << "Error: Unable to open 'studentsData.txt'!" << endl;
        return;
    }

    string sectionToSearch, line, currentSection, currentStudentData;
    bool found = false;

    // Ask teacher for the section to search
    cout << "Enter the section you want to view: ";
    cin >> sectionToSearch;

    cout << "\n---------- Students in Section " << sectionToSearch << " ----------" << endl;

    while (getline(studentsFile, line))
    {
        if (line.find("Section: ") != string::npos)
        {
            currentSection = line.substr(9); // Extract the section
        }

        // Start capturing student data
        currentStudentData += line + "\n";

        // Detect the end of a student's record
        if (line == "--------------------")
        {
            if (currentSection == sectionToSearch)
            {
                cout << currentStudentData << endl;
                found = true;
            }
            // Reset for next student
            currentStudentData = "";
        }
    }

    if (!found)
    {
        cout << "No students found in section " << sectionToSearch << "." << endl;
    }

    cout << "------------------------------------------------" << endl;
    studentsFile.close();
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//------------*********------------STUDENT LOGIN BLOCK--------***********--------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------


bool studentLogin(string& studentName)
{
    string loginID, password, currentLoginID, currentName, currentPassword, currentDepartment, line;

    // Prompt for Login ID and Password
    cout << "Enter Login ID: ";
    cin >> loginID;
    cout << "Enter Password: ";
    cin >> password;

    ifstream studentsFile("studentsData.txt");

    if (!studentsFile.is_open())
    {
        cout << "Error: Unable to open 'studentsData.txt'!" << endl;
        return false;
    }

    // Validate Login ID and Password
    while (getline(studentsFile, line))
    {
        if (line.find("Login ID: ") != string::npos)
        {
            currentLoginID = line.substr(10); // Extract Login ID
        }
        else if (line.find("Name: ") != string::npos)
        {
            currentName = line.substr(6); // Extract Name
        }
        else if (line.find("Password: ") != string::npos)
        {
            currentPassword = line.substr(10); // Extract Password
        }

        // Check Login ID and Password
        if (currentLoginID == loginID && currentPassword == password)
        {
            studentName = currentName;
            cout << "\nWelcome, " << studentName << "!" << endl;
            studentsFile.close();
            return true;
        }

    }

    studentsFile.close();
    cout << "Invalid ID or Password!" << endl;
    return false;
}

void viewAssignedQuizzes(string studentCourse)
{
    ifstream quizListFile("quizList.txt");

    if (!quizListFile.is_open())
    {
        cout << "No quizzes found or unable to access 'quizList.txt'." << endl;
        return;
    }

    cout << "\n---------- Available Quizzes for Course: " << studentCourse << " ----------" << endl;

    string quizFileName, line;
    bool quizFound = false;

    while (getline(quizListFile, quizFileName))
    {
        ifstream quizFile(quizFileName);

        if (!quizFile.is_open())
        {
            cout << "Error: Unable to read quiz file '" << quizFileName << "'." << endl;
            continue;
        }

        // Check if the file has a matching Subject Tag
        while (getline(quizFile, line))
        {
            if (line.find("Subject Tag: " + studentCourse) != string::npos)
            {
                string quizName = quizFileName.substr(0, quizFileName.find_last_of('.'));
                cout << "- " << quizName << endl;
                quizFound = true;
                break;
            }
        }
        quizFile.close();
    }

    if (!quizFound)
    {
        cout << "No quizzes found for department " << studentCourse << "." << endl;
    }

    quizListFile.close();
    cout << "---------------------------------------------------------------" << endl;
}

void viewStudentData(const string& loginID)
{
    ifstream studentsFile("studentsData.txt");

    if (!studentsFile.is_open())
    {
        cout << "Error: Unable to open 'studentsData.txt'!" << endl;
        return;
    }

    string line, currentLoginID, studentData;
    bool found = false;

    while (getline(studentsFile, line))
    {
        if (line.find("Login ID: ") != string::npos)
        {
            currentLoginID = line.substr(10); // Extract Login ID
        }

        // Start capturing student data
        studentData += line + "\n";

        // End of student record
        if (line == "--------------------")
        {
            if (currentLoginID == loginID)
            {
                cout << "\n---------- Student Data ----------" << endl;
                cout << studentData;
                found = true;
                break;
            }
            studentData = ""; // Reset for the next student
        }
    }

    if (!found)
    {
        cout << "No data found for student with Login ID: " << loginID << "." << endl;
    }

    studentsFile.close();
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//------------*********----------------TIMER CODE-------------***********--------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

void displayTime(int hours, int minutes, int seconds)
{
    cout << "\r"
        << (hours < 10 ? "0" : "") << hours << ":"
        << (minutes < 10 ? "0" : "") << minutes << ":"
        << (seconds < 10 ? "0" : "") << seconds
        << flush;
}

void countdownTimer(int totalSeconds)
{
    int hours, minutes, seconds;

    for (int i = totalSeconds; i >= 0; --i)
    {
        hours = i / 3600;
        minutes = (i % 3600) / 60;
        seconds = i % 60;

        displayTime(hours, minutes, seconds);
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "\nTime's up!" << std::endl;
}
void takeQuiz(const string& quizName)
{

    string fileName = quizName + ".txt";

    ifstream quizFile(fileName);

    if (!quizFile.is_open())
    {
        cout << "Error: Quiz '" << quizName << "' not found!" << endl;
        return;
    }

    string name;
    cout << endl << "Enter you name: ";
    cin >> name;

    cout << "\nStarting Quiz: " << quizName << endl;

    string line, userAnswer, correctAnswer;
    int score = 0, totalQuestions = 0, timeLimit = 0;
    bool timeLimitSet = false;

    // Read quiz metadata to find the time limit

    while (getline(quizFile, line))
    {
        if (line.find("Time Limit: ") != string::npos)
        {
            timeLimit = stoi(line.substr(12)); // Extract the time limit in minutes
            timeLimitSet = true;
            break;
        }
    }

    // If time limit is set, convert it to seconds
    int timeLimitSeconds = timeLimitSet ? timeLimit * 60 : 0;
    // Start the quiz timer
    auto quizStartTime = chrono::steady_clock::now();

    while (getline(quizFile, line))
    {
        // Check if the question starts
        if (line.find("Question ") != string::npos)
        {
            totalQuestions++;

            // Check if time has expired
            if (timeLimitSet)
            {
                auto currentTime = chrono::steady_clock::now();
                auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - quizStartTime).count();
                if (elapsedTime >= timeLimitSeconds)
                {
                    cout << "\nTime's up! The quiz has ended." << endl;
                    break;
                }
                cout << "Remaining Time: " << (timeLimitSeconds - elapsedTime) / 60 << " minutes " << (timeLimitSeconds - elapsedTime) % 60 << " seconds" << endl;
            }

            cout << line << endl; // Display the question

            // Display options
            for (int i = 0; i < 4; ++i)
            {
                getline(quizFile, line);
                cout << line << endl;
            }

            // Read the correct answer
            getline(quizFile, line);
            correctAnswer = line.substr(16); // Extract Correct Answer

            // Get the user's answer
            cout << "Your Answer: ";
            cin >> userAnswer;

            if (userAnswer == correctAnswer)
            {
                score++;
            }

            cout << "--------------------" << endl;
        }
    }

    quizFile.close();

    // Display the final score
    cout << "\nQuiz Completed! Your Score: " << score << " out of " << totalQuestions << endl;

    // Save results to the file
    ofstream resultsFile("studentResults.txt", ios::app);

    if (resultsFile.is_open())
    {

        resultsFile << "Quiz: " << quizName << endl;
        resultsFile << name << ": Score: " << score << " out of " << totalQuestions << endl;
        resultsFile << "--------------------" << endl;
        resultsFile.close();
    }
    else
    {
        cout << "Error: Unable to save results." << endl;
    }
}


int main()
{


mainMenu:
    {

        char ch; //A variable for taking option from user in menu

        cout << "       **********************************************************************" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *      LOGIN AS :                                                    *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *          -----ADMIN------                                          *" << endl;
        cout << "       *          ----TEACHER-----                                          *" << endl;
        cout << "       *          ----STUDENT-----                                          *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *   Press 'A' for admin____'T' for teacher____'S' for student :      *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       **********************************************************************";

        cout << endl << endl << "        Enter: ";
        cin >> ch;
        cout << endl << endl;

        if (ch == 'A' || ch == 'a')
        {
            goto adminMenu;
        }
        else if (ch == 'T' || ch == 't')
        {
            if (teacherLogin()) {
                goto teacherMenu;
            }
            else
            {
                goto mainMenu;
            }

        }
        else if (ch == 'S' || ch == 's')
        {
            string studentName;

            // Call the studentLogin function
            if (studentLogin(studentName))
            {
                goto studentMenu; // Proceed to the student menu
            }
            else
            {
                goto mainMenu; // Return to the main menu if login fails
            }
        }
        else
        {
            cout << "------Please select from given options------- \n";
            goto mainMenu;
        }

    }


adminMenu:
    {

        char ch1; //A variable for taking option from user in admin menu



        cout << "       **********************************************************************" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *      Hey Admin!                                                    *" << endl;
        cout << "       *      You logged in successfully                                    *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *      -----------------------------------------------               *" << endl;
        cout << "       *             -------Teachers--------                                *" << endl;
        cout << "       *             -------Students--------                                *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *       ----------------------------------------------               *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *           Press 'T' for teacher____'S' for student                 *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       **********************************************************************";

        cout << endl << endl << "        Enter: ";
        cin >> ch1;

        if (ch1 == 't' || ch1 == 'T')
        {

            goto adminAddTeachers;

        }
        else if (ch1 == 's' || ch1 == 'S')
        {
            goto adminAddStudents;

        }

        else
        {
            cout << "\n\n Invalid Command\n";
            goto adminMenu;
        }

    adminAddTeachers:
        {

            char ch2; //Variable for input option        
            cout << endl << endl << endl;
            cout << "       **********************************************************************" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *      --------(Teachers)----------                                  *" << endl;
            cout << "       *      What do you want to do?                                       *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *      -----------------------------------------------               *" << endl;
            cout << "       *             -------Add Teachers--------(Press A)                   *" << endl;
            cout << "       *             -------Show All Teachers---(Press T)                   *" << endl;
            cout << "       *             -------Search Teachers-----(Press S)                   *" << endl;
            cout << "       *             -------Clear All Teachers--(Press c)                   *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *       ----------------------------------------------               *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *             -------Go to Your Dashboard--(Press D)                 *" << endl;
            cout << "       *             -------Logout as Admin--(Press L)                      *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *       ----------------------------------------------               *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       **********************************************************************";

            cout << endl << endl << "        Enter: ";
            cin >> ch2;

            cout << endl << endl;

            if (ch2 == 'A' || ch2 == 'a')
            {
                cout << endl;
                int size;
                cout << "How many record of the teachers you want to add: " << endl;
                cin >> size;
                addTeachersData(size);   //A predefined function to take input and save the  data of teacher in file named teachersData.txt

                goto adminAddTeachers;
            }
            else if (ch2 == 'T' || ch2 == 't')
            {
                showAllTeachers();
                goto adminAddTeachers;
            }

            else if (ch2 == 'S' || ch2 == 's')
            {
                searchTeachers();
                goto adminAddTeachers;
            }

            else if (ch2 == 'c' || ch2 == 'C')
            {
                clearAllTeachers();
                goto adminAddTeachers;
            }

            else if (ch2 == 'd' || ch2 == 'D')
            {
                goto adminMenu;
            }

            else if (ch2 == 'l' || ch2 == 'L')
            {
                goto mainMenu;
            }

        }

    adminAddStudents:
        {

            char ch3; //Variable for input option        
            cout << endl << endl << endl;
            cout << "       **********************************************************************" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *      --------(Students)----------                                  *" << endl;
            cout << "       *      What do you want to do?                                       *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *      -----------------------------------------------               *" << endl;
            cout << "       *             -------Add Students--------(Press A)                   *" << endl;
            cout << "       *             -------Show All Students---(Press T)                   *" << endl;
            cout << "       *             -------Search Students-----(Press S)                   *" << endl;
            cout << "       *             -------Clear All Students--(Press c)                   *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *       ----------------------------------------------               *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *             -------Go to Your Dashboard--(Press D)                 *" << endl;
            cout << "       *             -------Logout as Admin--(Press L)                      *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *       ----------------------------------------------               *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       **********************************************************************";

            cout << endl << endl << "        Enter: ";
            cin >> ch3;

            cout << endl << endl;

            if (ch3 == 'A' || ch3 == 'a')
            {
                cout << endl;
                int size;
                cout << "How many record of the teachers you want to add: " << endl;
                cin >> size;
                addStudentsData(size);   //A predefined function to take input and save the  data of teacher in file named teachersData.txt

                goto adminAddStudents;
            }
            else if (ch3 == 'T' || ch3 == 't')
            {
                showAllStudents();
                goto adminAddStudents;
            }

            else if (ch3 == 'S' || ch3 == 's')
            {
                searchStudent();
                goto adminAddStudents;
            }

            else if (ch3 == 'c' || ch3 == 'C')
            {
                clearAllStudents();
                goto adminAddStudents;
            }

            else if (ch3 == 'd' || ch3 == 'D')
            {
                goto adminMenu;
            }

            else if (ch3 == 'l' || ch3 == 'L')
            {
                goto mainMenu;
            }
        }
    }
teacherMenu:
    {
        char ch5; // A variable for taking option from the user in the teacher menu

        cout << endl
            << endl
            << "       **********************************************************************" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *                        TEACHER DASHBOARD                           *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       *      -----------------------------------------------               *" << endl;
        cout << "       *             -------View Students Data--------(Press V)             *" << endl;
        cout << "       *             -------Create a Quiz------------(Press C)              *" << endl;
        cout << "       *             -------Read the Quiz and edit--------(Press R)         *" << endl;
        cout << "       *             -------Show Quiz Marks----------(Press Q)              *" << endl;
        cout << "       *             -------Modify Student GPA--------(Press G)             *" << endl;
        cout << "       *             -------Logout as Teacher--------(Press L)              *" << endl;
        cout << "       *                                                                    *" << endl;
        cout << "       **********************************************************************" << endl;

        cout << endl
            << endl
            << "        Enter: ";
        cin >> ch5;

        if (ch5 == 'Q' || ch5 == 'q')
        {
            showQuizMarks(); // Call the function to show quiz marks
            goto teacherMenu; // Return to the teacher menu
        }
        else if (ch5 == 'C' || ch5 == 'c')
        {
            createQuiz();
            goto teacherMenu; // Return to the teacher menu
        }
        else if (ch5 == 'R' || ch5 == 'r')
        {
            viewOrEditQuiz();
            goto teacherMenu; // Return to the teacher menu
        }
        else if (ch5 == 'G' || ch5 == 'g')
        {
            modifyStudentGPA(); // Function to modify a student's GPA
            goto teacherMenu;   // Return to the teacher menu
        }
        else if (ch5 == 'V' || ch5 == 'v')
        {
            showStudentsBySection(); // Function to display all student data
            goto teacherMenu; // Return to the teacher menu
        }
        else if (ch5 == 'L' || ch5 == 'l')
        {
            cout << "\nYou have successfully logged out.\n";
            goto mainMenu; // Return to the main menu
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
            goto teacherMenu; // Retry the teacher menu for invalid input
        }
    }

studentMenu:
    {
        char ch6; // A variable for taking options from the user in the student menu
        string loginID;

        cout << "\nEnter your Login ID to access your data: ";
        cin >> loginID;

        do {
            cout << endl
                << endl
                << "       **********************************************************************" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *                        STUDENT DASHBOARD                           *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       *      -----------------------------------------------               *" << endl;
            cout << "       *             -------View My Data--------------(Press V)             *" << endl;
            cout << "       *             -------View Assigned Quizzes-----(Press A)             *" << endl;
            cout << "       *             -------Take a Quiz---------------(Press T)             *" << endl;
            cout << "       *             -------Logout as Student--------(Press L)              *" << endl;
            cout << "       *                                                                    *" << endl;
            cout << "       **********************************************************************" << endl;

            cout << endl
                << endl
                << "        Enter: ";
            cin >> ch6;

            if (ch6 == 'V' || ch6 == 'v')
            {
                viewStudentData(loginID); // Display the student's data
            }
            else if (ch6 == 'A' || ch6 == 'a')
            {
                string studentCourse;
                cout << "\nThe quizzes of which subject do you want to see?";
                cin >> studentCourse;
                viewAssignedQuizzes(studentCourse); // Display quizzes for the department
            }
            else if (ch6 == 'T' || ch6 == 't')
            {
                string quizName;
                cout << "Enter the name of the quiz you want to take: ";
                cin.ignore();
                getline(cin, quizName);
                takeQuiz(quizName); // Modified function with time limit
                goto studentMenu; // Return to the student menu
            }

            else if (ch6 == 'L' || ch6 == 'l')
            {
                cout << "\nYou have successfully logged out.\n";
                goto mainMenu; // Return to the main menu
            }
            else
            {
                cout << "Invalid choice! Please try again.\n";
            }

        } while (true); // Keep showing the menu until the student logs out
    }

    return 0;

}