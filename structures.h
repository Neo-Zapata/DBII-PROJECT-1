#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>

#include <bitset>
#include <unordered_map>
#include <iomanip>
#include <cstdint>
#include <cmath>

using namespace std;
const int fb = 3;

struct Record_h
{
    int id;
    char name[120];
    char type[10];
    char episodes[5];
    char rating[5];
    char members[10];

    void showData()
    {
        cout << id << "|";
        cout << name << "|";
        cout << type << "|";
        cout << episodes << "|";
        cout << rating << "|";
        cout << members << "|";
    }

    void set_data()
    {
        cin.ignore();
        string anime_id, key, type, episodes, rating, members;
        // string key, type, total, generation, legendary;

        cout << "anime id: ";
        getline(cin, anime_id);
        this->id = stoi(anime_id);

        cout << "Anime name: ";
        getline(cin, key);
        for (char &c : key)
            c = tolower(c);
        strncpy(this->name, key.c_str(), 120 - 1);
        this->name[120 - 1] = '\0';

        cout << "type: ";
        getline(cin, type);
        strncpy(this->type, type.c_str(), 10 - 1);
        this->type[10 - 1] = '\0';

        cout << "Episodes: ";
        getline(cin, episodes);
        strncpy(this->episodes, episodes.c_str(), 5 - 1);
        this->episodes[5 - 1] = '\0';

        cout << "rating: ";
        getline(cin, members);
        strncpy(this->members, members.c_str(), 5 - 1);
        this->members[5 - 1] = '\0';

        cout << "members: ";
        getline(cin, members);
        strncpy(this->members, members.c_str(), 10 - 1);
        this->members[10 - 1] = '\0';
    }
};

struct Bucket
{
    Record_h records[fb];
    int size;
    long next;
    int d;

    Bucket(long size, int next, int d)
    {
        this->size = size;
        this->next = next;
        this->d = d;
    }
    Bucket() = default;
};

struct Index
{
    int key;
    int pos_log;
    ;

    Index(int key, int pos_log)
    {
        this->key = key;
        this->pos_log = pos_log;
    }
    Index() = default;
};