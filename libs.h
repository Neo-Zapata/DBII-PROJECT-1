#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <exception>
using namespace std;

class file_not_found : public exception {
    public:
        file_not_found(const char* message) : m_message(message){}

        virtual const char* what() const throw() {
            return m_message.c_str();
        }
    private:
        string m_message;
};

void create_file_if_doesnt_exist(string filename){
    ofstream file("files/" + filename, ios::out | ios::app);
    if(!file.is_open())
        throw file_not_found("\nCould not open file - Class Constructor");
    file.close();
}