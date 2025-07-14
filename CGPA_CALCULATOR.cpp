#include<iostream>
#include<vector>
#include<iomanip>
#include<limits>
#include<algorithm>
#include<map>
#include<sstream>
#include<stdexcept>
#include<cctype> 
using namespace std;


class CGPACalculator{
private:
    struct Course{
        string name;
        char grade;
        double creditHours;
        double gradePoint;
    };

    vector<Course> courses;
    double totalCredits=0.0;
    double totalGradePoints=0.0;
    double semesterGPA=0.0;
    double overallCGPA=0.0;
    int semestersCompleted=0;

   //mapping grades with numerical values
    const map<char, double> gradeValues={
        {'A', 4.0},{'B', 3.0},{'C', 2.0},{'D', 1.0},{'F', 0.0}
    };

   //uppercase is only valid grade  
    const vector<char> validGrades={'A', 'B', 'C', 'D', 'F'};


    void clearInputBuffer(){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    bool isValidGrade(char grade){
        char upperGrade=toupper(grade);
        return find(validGrades.begin(),validGrades.end(),upperGrade)!=validGrades.end();
    }
    
    //credit hours input
    bool isValidCreditHours(double credits){
        return credits>=0.5 && credits<=5.0; 
    }

    //Calculate grade point for course
    double calculateGradePoint(char grade, double creditHours){
        char upperGrade=toupper(grade);
        auto it=gradeValues.find(upperGrade);
        if(it !=gradeValues.end()){
            return it->second*creditHours;
        }
        return 0.0;
    }

    //checks if inputs are valid
    template <typename T, typename Validator>
    T getValidInput(const string& prompt, const string& errorMsg, Validator validator){
        T input;
        while(true){
            cout<<prompt;
            if(cin>> input && validator(input)){
                return input;
            }
            cout<<errorMsg<<endl;
            clearInputBuffer();
        }
    }

    //get course details from user
    void getCourseDetails(){
        int numCourses=getValidInput<int>(
            "Enter number of courses taken this semester: ",
            "Invalid input. Please enter a positive integer.",
            [](int x) { return x > 0; }
        );
cout<<endl;
        for(int i=0;i<numCourses;++i){
            Course course;
            cout<<"\n----------------------------------";
            cout<<"\n          Course "<<i + 1<<" Details:     \n";
            cout<<"----------------------------------\n";
            //course name
            clearInputBuffer();
            cout<<"Course name:";
            getline(cin, course.name);

            //grade
            course.grade=getValidInput<char>(
                "Enter grade (A, B, C, D, F): ",
                "Invalid grade. Please enter A, B, C, D, or F.",
                [this](char g){ return isValidGrade(g);}
            );

            //credit hours 
            course.creditHours=getValidInput<double>(
                "Credit hours: ",
                "Invalid credit hours. Please enter a value between 0.5 and 5.0.",
                [this](double c){ return isValidCreditHours(c);}
            );

            //calculating grade point
            course.gradePoint=calculateGradePoint(course.grade, course.creditHours);

            //total
            totalCredits+=course.creditHours;
            totalGradePoints+=course.gradePoint;

            courses.push_back(course);
        }
    }
    //SGPA
    void calculateSemesterGPA(){
        if(totalCredits>0){
            semesterGPA=totalGradePoints/totalCredits;
        }
    }

    //CGPA
    void calculateOverallCGPA(){
        if(semestersCompleted>0){
            overallCGPA=((overallCGPA*semestersCompleted)+semesterGPA)/(semestersCompleted+1);
            semestersCompleted++;
        }
        else{
            overallCGPA=semesterGPA;
            semestersCompleted=1;
        }
    }

    //display results
    void displayResults(){
        cout<<"\n\n+--------------------------------------------+\n";
        cout<<"|              ACADEMIC RESULTS              |\n";
        cout<<"+---------------------+-----------+----------+\n";
        cout<<"| Course Name         | Grade     | Credits  |\n";
        cout<<"+---------------------+-----------+----------+\n";
        for(const auto& course : courses){
            ostringstream line;
            line<<"| "<<left<<setw(19)<<course.name<<" | "<<setw(9)<<static_cast<char>(toupper(course.grade))<<" | "<<setw(8)<<course.creditHours<<" |\n";
            cout<<line.str();
        }
        cout<<"+---------------------+-----------+----------+\n";
        cout<<"| Semester GPA:       | "<<right<<setw(9)<<fixed<<setprecision(2)<<"    "<<semesterGPA<<"        |\n";
        if(semestersCompleted>1){
            cout<<"| Overall CGPA:       | "<<right<<setw(9)<<fixed<<setprecision(2)<<"     "<<overallCGPA<<"        |\n";
        }
        cout << "+---------------------+-----------+----------+\n\n";
    }
public:
    void run(){
        cout<<"\n================================================\n";
        cout<<"|               CGPA CALCULATOR                |\n";
        cout<<"================================================\n\n";

        //get previous CGPA if available
        if(semestersCompleted==0){
            char hasPrevious;
            cout<<"Do you have previous semester results? (Y/N): ";
            cin>>hasPrevious;
            cout<<"\n";

            if(toupper(hasPrevious)=='Y'){
                overallCGPA=getValidInput<double>(
                    "Enter your previous CGPA: ",
                    "Invalid CGPA. Please enter a value between 0.0 and 4.0.",
                    [](double g) {return g>=0.0&&g<=4.0;}

                );

                cout<<"\n";

                semestersCompleted=getValidInput<int>(
                    "Enter number of semesters completed: ",
                    "Invalid input. Please enter a positive integer.",
                    [](int x){return x>0;}
                 
                );
                cout<<endl;
            }
            clearInputBuffer();
        }

        //current semester details
        getCourseDetails();
        calculateSemesterGPA();
        calculateOverallCGPA();
        displayResults();

        //if user wants to calculate again 
        char continueOption;
        cout<<"\nWould you like to calculate for another semester? (Y/N):";
        cin>>continueOption;

        if(toupper(continueOption)=='Y'){
                  
            courses.clear();
            totalCredits=0.0;
            totalGradePoints=0.0;
            semesterGPA=0.0;
            
            clearInputBuffer();
            run();
        } 
        else{
            cout<<"\nFinal CGPA: "<<fixed<<setprecision(2)<<overallCGPA<<endl;
        }
    }
};

int main(){
    try{
        CGPACalculator calculator;
        calculator.run();
    } 
    catch(const exception& e){
        cerr<<"An error occurred: "<<e.what()<<endl;
        return 1;
    }
    return 0;
}
