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
#include <vector>
#include <sstream>
#include <fstream>

struct Value
{
	virtual ~Value() {}
};

struct Person // Base class for all people, not constrained to only the people qualified for enrollment
: Value
{
	virtual ~Person() override {}
	std::string name;
	char ssn[9];
};

struct Applicant
: Person
{
	virtual ~Applicant() override {}
	bool accepted;
	// An applicant can either be accepted or not (yet) accepted
};

struct Member // "Member" as in a member of the University of Akron who has a UANET ID and ID number
: Person
{
	virtual ~Member() override {}
	int number;
	std::string id;	
};

struct Employee
: Member
{
	virtual ~Employee() override {}
	double pay;

};

struct Student
: Member, Applicant // A student must be an applicant
{
	~Student() override {}	
	double gpa;
};

struct Professor
: Member 
{
	~Professor() override {}
		
};



struct Building
: Value
{
	virtual ~Building() override {}
	std::string name;
	int number;
};

struct Classroom
: Building
{
	virtual ~Classroom() override {}
	int number;
};

struct Class
: Value
{
	virtual ~Class() override {}
	int number;
	int size;
};

struct Course
: Class
{
	virtual ~Course() override {}
	std::vector<Class*> classes;
	std::string title;
	int number;

};

struct Department
: Value
{};

struct College // An example of this would be the college of engineering at UA
: Course, Department
{
	virtual ~College() override {}
	std::vector<Course*> courses;
};

struct School // An example of this would be the University of Akron, or the Ohio State University
: College
{
	virtual ~School() override {}	
	std::string name;
	std::vector<College*> colleges;
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
