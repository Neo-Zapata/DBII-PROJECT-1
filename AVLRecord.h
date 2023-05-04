//
// Created by milagros on 4/28/23.
//

#ifndef AVL_RECORD_H
#define AVL_RECORD_H



#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

#define NAME_SIZE 110
#define TYPE_SIZ_E  14

struct AVLRecord{

public:
    int anime_id;
    char name[110];
    char type[14];
    int episodes;
    float rating;
    int members;
    
    void set_data()
    {
        cin.ignore();
        string anime_id, name, type, episodes, rating, members;
        // string key, type, total, generation, legendary;

        cout << "anime id: ";
        getline(cin, anime_id);
        this->anime_id = stoi(anime_id);

        cout << "Anime name: ";
        getline(cin, name);
        for (char &c : name)
            c = tolower(c);
        strncpy(this->name, name.c_str(), 110 - 1);
        this->name[110 - 1] = '\0';

        cout << "type: ";
        getline(cin, type);
        strncpy(this->type, type.c_str(), 14 - 1);
        this->type[14 - 1] = '\0';

        cout << "Episodes: ";
        
        getline(cin, episodes);
        this->episodes = stoi(episodes);

        cout << "rating: ";
        getline(cin, rating);
        this->rating = std::stof(rating);


        cout << "members: ";        
        getline(cin, members);
        this->members = stoi(members);

    }
    void get_data (){
        cout<<anime_id << " | " << name << " | " << type << " | " << episodes << " | " << rating << " | " << members<<endl;
    }

};
#endif //AVL_RECORD_H