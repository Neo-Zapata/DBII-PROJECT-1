#ifndef LIBS_H
#define LIBS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <exception>
#include <cmath>
#include <cctype>
#include <iterator>
#include <cstdio>
#include <ctime>
#include <cstring>
using namespace std;

// #define KEY_SIZE 30
#define KEY_SIZE 103
// #define TYPE_SIZE 10
#define TYPE_SIZE 15
#define EPISODE_SIZE 10

class Pos
{
public:
    long pos;
    char file;

    Pos() {}

    Pos(long pos, char file)
    {
        this->pos = pos;
        this->file = file;
    }

    void show()
    {
        cout << "> (" << this->pos << " - " << this->file << ")\n";
    }

    bool operator==(const Pos &x)
    {
        if (this->pos == x.pos and this->file == x.file)
            return true;
        else
            return false;
    }

    bool operator!=(const Pos &x)
    {
        if (this->pos != x.pos or this->file != x.file)
            return true;
        else
            return false;
    }
};

class file_not_found : public exception
{
public:
    file_not_found(const char *message) : m_message(message) {}
    virtual const char *what() const throw()
    {
        return m_message.c_str();
    }

private:
    string m_message;
};

class key_not_found : public exception
{
public:
    key_not_found(const char *message) : m_message(message) {}
    virtual const char *what() const throw()
    {
        return m_message.c_str();
    }

private:
    string m_message;
};

class ayudame_Dios : public exception
{
public:
    ayudame_Dios(const char *message) : m_message(message) {}
    virtual const char *what() const throw()
    {
        return m_message.c_str();
    }

private:
    string m_message;
};

void create_file_if_doesnt_exist(string filename)
{
    ofstream file("files/" + filename, ios::out | ios::app);
    if (!file.is_open())
        throw file_not_found("\nCould not open file - Class Constructor");
    file.close();
}

// string get_key_from_user(){
// cin.ignore();
//     string key;
//     cin, key);
//     return key;
// }

#endif