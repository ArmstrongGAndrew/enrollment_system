/*******************************************
 * Andrew Armstrong
 * Object Oriented Programming
 *
 * Project: Enrollment System
 *
 ******************************************/

/* ASCII values representing characters for object detection when parsing */
#define A_CAP 65
#define Z_CAP 90
#define A_LOW 97
#define Z_LOW 122
#define ZERO 48
#define NINE 57

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <locale>

using namespace std;

void move_separator(char *&f, char separator)
{
  while (*f != separator)
    f++;
}
bool is_alpha(char *f)
{
  if (*f >= 'A' && *f <= 'z')
    return true;
  return false;
}

bool is_int(char *f)
{
	if (*f >= '0' && *f <= '9')
		return true;
	return false;
}

void move_int(char *&f)
{
  while (!is_int(f)) f++;
}

void move_alpha(char *&f)
{
  while (!is_alpha(f)) f++;
}

string parse_string(char *&f)
{
  string temp = "";
  move_alpha(f);
  while (is_alpha(f))
    temp += *f++;
  return temp;
}

int parse_int(char *&f)
{
  string temp = "";
  move_int(f);

  while (is_int(f))
    temp += *f++;
  return stoi(temp, nullptr, 10);
}

double parse_double(char *&f)
{
  string temp = "";
  move_int(f);

  while (is_int(f))
    temp += *f++;
  if (*f == '.')
  	temp += *f++;
  while (is_int(f))
  	temp += *f++;
  return stod(temp, nullptr);
}

struct Value
{
	virtual ~Value() {};
	virtual int val() = 0; // Updates value and can return value local to each derived class
	virtual void display() = 0;
	int value;
};


struct Department
: Value
{
  std::string dept;
  int number;

  Department() {dept = "Null"; number = -1;}
  Department(std::string dept) {this->dept = dept;}

  int val() override {value = 0;}
  void display() override {std::cout << dept << ": ["<<value<<"]";}

  void operator=(Department& target) {this->dept = target.dept;}
  ~Department() override {}
};

struct Position
: Value
{
	bool faculty; // True if faculty position, false if staff position
	std::string title;
	Department dept;

	Position() {title = "Null";}
	Position(std::string title) {this->title = title;}
	Position(Department dept) {this->title = "Null"; this->dept = dept;}

	int val() override {}
	void display() override {std::cout<<title<<" "<<dept.dept<<" "<<faculty<< "["<<value<<"]";}

	~Position() {}
	
};


struct Class
: Value
{
  int number;
  int size;

  Class() {number = -1; size = -1;}	
  Class(int num) {} // Determine which member variable to assign value
  Class(int number, int size) {this->number = number; this->size = size;}


  virtual int val() override {}
  virtual void display() override {std::cout<<number<<": "<<size<< " ["<<value<<"]";}

  virtual ~Class() override {}
};

struct Course
: Value
{
	std::vector<Class*> classes;
	std::string title;
	int number;
	double credits;

	int val() override {}
	void display() override {}

	~Course() override {}
};

struct Building
: Value
{
	virtual ~Building() override {}
	std::string name;
	int number;

	int val() override {}
	void display() override {}
};

struct Classroom
: Value
{
	virtual ~Classroom() override {}
	int number;

	int val() override {}
	void display() override {}
};



struct College // An example of this would be the college of engineering at UA
: Value
{
	virtual ~College() override {}
	std::vector<Course*> courses;
	vector<Department*> departments;
	Building loc;

	int val() override {}
	void display() override {}
};

struct School // An example of this would be the University of Akron, or the Ohio State University
: Value
{
	virtual ~School() override {}	
	std::string name;
	std::vector<College*> colleges;

	int val() override {}
	void display() override {}
};

struct Person // Base class for all people, not constrained to only the people qualified for enrollment
: Value
{
	std::string name;
	std::string ssn;


	Person() {name = "Null"; ssn = "Null";}
	Person(std::string str) {} // Check str for Integer char or non-integer char

	virtual ~Person() override {}
	virtual void parse(std::vector<std::string> lines) {} // Vector of strings for flexibility (multiple lines)
	virtual int val() override {if(name == "Null" && ssn == "Null") value = 0; }
	virtual void display() override {std::cout << name << ": " << ssn <<"\t"<<value;}

};

struct Applicant
: Person
{
	bool accepted;

	Applicant() {accepted = false;}
	Applicant(bool accepted) {this->accepted = accepted;}

	virtual int val() override {}
	virtual void display() override {}

	virtual ~Applicant() override {}

	// An applicant can either be accepted or not (yet) accepted
};

struct Member // "Member" as in a member of the University of Akron who has a UANET ID and ID number
: Person
{
	int number;
	std::string id;	
        
	Member() {number = -1; id = "Null";}
	Member(int number) {this->number = number; this->id = "Null";}
	Member(std::string id) {this->id = id; this->number = -1;}
	Member(std::string id, int number) {this->id = id; this->number = number;}
	Member(int number, std::string id) {this->id = id; this->number = number;}
	virtual ~Member() override {}
};

struct Employee
: Member
{
	virtual ~Employee() override {}
	vector<pair<Position, double>> positions;

	Employee() {}
	Employee(Position pos, double pay) {
		positions.push_back(make_pair(pos,pay));
	}
	Employee(vector<Position> pos, vector<double> pay)
	{
		if (pos.size() != pay.size()) { }
		else {
		for (int i = 0; i < pos.size(); i++) {
			positions.push_back(make_pair(pos[i], pay[i]));
		}
	}
	}

	int val() override {}
	void display() override {}
};

struct Student
: Member, Applicant // A student must be an applicant
{
	double gpa;
	std::vector<Class> classes; // Can be in progress, taken, or will be taking
	std::vector<Course> courses; // 

	Student() {}
	Student(double gpa) {}

	int val() override {}
	void display() override {
		cout << Member::name << ": " << Member::id << "  " << Member::number << "\n";
	}

	~Student() override {}	
};





// Load any type of file and return a string containing all raw content of the file
std::string loadFile(std::string filename)
{
	ifstream file(filename);
	string content;

	file.seekg(0, ios::end);
	content.reserve(file.tellg());
	file.seekg(0, ios::beg);

	content.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	file.close();

	return content;
}

/* Implement parsers that create objects given databases for each class */

// "member" below represents the member variable of the person object being parsed. There will be a
// subparser implemented for each member variable of the object's class.
vector<Person*> parse_Person_db(std::string people_db)
{
	std::string db = loadFile(people_db);
	char * f = &db[0];
	char * l = &db[0] + db.size();

	vector<Person*> people;

	while (f != l)
	{
		Person * person = new Person;

		person->name = parse_string(f) + ", " + parse_string(f);
		person->ssn = parse_int(f);
		people.push_back(person);
	}

	return people;
}

vector<Student> parse_Student_db(string student_db)
{
	string db = loadFile(student_db);

	char *f = &db[0];
	char *l = &db[db.size()-1];

	vector<Student> students;

	while (f != l)
	{
		Student * student = new Student;

		//student->Member::name= parse_string(f) + ", " + parse_string(f);
		
		student->Member::name = parse_string(f);
		student->Member::name += ", ";
		student->Member::name += parse_string(f);
		student->id = parse_string(f);
		student->Member::number = parse_int(f);
		student->accepted = true;
		students.push_back(*student);
	}
	return students;
}

vector<Course> parse_Course_db(string course_db)
{
	string db = loadFile(course_db);
	vector<Course> courses;

	char *f = &db[0];
	char *l = &db[db.size()-1];


	while (f != l)
	{
		Course * course = new Course;
		course->title = parse_string(f);
		course->number = parse_int(f);
		course->credits = parse_double(f);
		courses.push_back(*course);
	}
	return courses;
}


int main()
{
	printf("\nTesting work in progress for successful compilation\n\n");
	vector<Student> students = parse_Student_db("students.txt");
	vector<Course> courses = parse_Course_db("courses.txt");

	students[0].display();
	students[0].courses.push_back(courses[0]);

	Value *v = new Student::Member;
	static_cast<Student::Member*>(v)->Member::name = "Andrew";
	cout << static_cast<Student::Member*>(v)->Member::name;
	v = new Employee;

	Position p;
	if (dynamic_cast<Employee*>(v))
		static_cast<Employee*>(v)->positions.push_back(make_pair(p,8.10));
	else
		cout << "Value is not an Employee\n";

	return 0;
}
