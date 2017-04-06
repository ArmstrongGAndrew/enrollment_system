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

  Department() {dept = "Null";}
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
    // Dangerous if not cautious about parameter order. Class oop(30,2) = :(

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
: Building
{
	virtual ~Classroom() override {}
	int number;

	int val() override {}
	void display() override {}
};



struct College // An example of this would be the college of engineering at UA
: Course, Department
{
	virtual ~College() override {}
	std::vector<Course*> courses;

	int val() override {}
	void display() override {}
};

struct School // An example of this would be the University of Akron, or the Ohio State University
: College
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
	std::unordered_map<Position*, double> positions;

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
	void display() override {}

	~Student() override {}	
};





// Load any type of file and return a string containing all raw content of the file
std::string loadFile(std::string file)
{	
	std::string content;
	std::ifstream data(file);
	std::stringstream stream;

	stream << data.rdbuf();
	content = stream.str();

	data.close();
	return content;
}

/* Implement parsers that create objects given databases for each class */

// "member" below represents the member variable of the person object being parsed. There will be a
// subparser implemented for each member variable of the object's class.
void parse_Person_member(char *&f)
{

}
void parse_Person_db(std::string people_db)
{
	std::string db = loadFile(people_db);
	char * f = &db[0];
	char * l = &db[0] + db.size();

// This condition will change according to the format of the database
	if (*f == '[') // Condition for detecting a new student 
	{
		Value * v = new Person;
		Person * pers = static_cast<Person*>(v);
		++f;

		if (*f >= A_CAP && *f <= Z_CAP) // Condition(s) for detecting a name
		{
			parse_Person_member(f); // "member" will likely be modified to "name" in future implementation
		}

		if (*f >= ZERO && *f <= NINE)
		{
			parse_Person_member(f); // "member" will likely be modified to "ssn" in future implementation
		}
	}	
}

int main()
{
	printf("\nTesting work in progress for successful compilation\n\n");
}
