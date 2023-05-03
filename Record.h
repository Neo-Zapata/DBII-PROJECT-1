#ifndef REC_H
#define REC_H

#include "libs.h"

class Record{
public:
    int anime_id;
    char key[KEY_SIZE];
    char type[TYPE_SIZE];
    char episodes[EPISODE_SIZE];
    float rating;
    int members;
    Pos prev;
    Pos next;
    bool is_deleted = 0;

    Record(){
        anime_id = 0;
        memset(key, ' ', KEY_SIZE - 1);
        memset(type, ' ', TYPE_SIZE - 1);
        memset(episodes, ' ', EPISODE_SIZE - 1);
        rating = 0;
        members = 0;
        prev = Pos(-1, 'D');
        next = Pos(-1, 'D');
        is_deleted = 0;
    }

    void show(){
        // cout << "> " << key << "|" << int(prev.pos) << "-" << prev.file << "|" << int(next.pos) << "-" << next.file << "|" << type << "|" << total << "|" << generation << "|" << legendary << "\n";
        cout << "> " << anime_id << "|" << key << "|" << int(prev.pos) << "-" << prev.file << "|" << int(next.pos) << "-" << next.file << "|" << type << "|" << episodes << "|" << rating << "|" << members << "\n";
    }

    void set_key(string new_key){
        for (char& c : new_key) {
            c = tolower(c);
        }
        strncpy(this->key, new_key.c_str(), KEY_SIZE - 1);
        this->key[KEY_SIZE - 1] = '\0';
    }

    void set_data(){
        cin.ignore();
        string anime_id, key, type, episodes, rating, members;
        // string key, type, total, generation, legendary;

        cout << "anime id: ";
        getline(cin, anime_id);
        this->anime_id = stoi(anime_id);
        
        cout << "key (pokemon name): ";
        getline(cin, key);
        for (char& c : key) 
            c = tolower(c);
        strncpy(this->key, key.c_str(), KEY_SIZE - 1);
        this->key[KEY_SIZE - 1] = '\0';

        cout << "type: ";
        getline(cin, type);
        strncpy(this->type, type.c_str(), TYPE_SIZE - 1);
        this->type[TYPE_SIZE - 1] = '\0';

        cout << "Episodes: ";
        getline(cin, episodes);
        strncpy(this->episodes, episodes.c_str(), EPISODE_SIZE - 1);
        this->episodes[EPISODE_SIZE - 1] = '\0';

        cout << "members: ";
        getline(cin, members);
        this->members = stof(members);

        cout << "rating: ";
        getline(cin, rating);
        this->rating = stoi(rating);

        this->prev = Pos(-1, 'D');
        this->next = Pos(-1, 'D');
        this->is_deleted = false;
    }
    bool operator==(const Record &x) { return string(this->key) == string(x.key); }
    bool operator!=(const Record &x) { return string(this->key) != string(x.key); }

    bool operator<=(const Record &x) { return string(this->key) <= string(x.key); }
    bool operator>=(const Record &x) { return string(this->key) >= string(x.key); }

    bool operator<(const Record &x) { 
        string first = string(this->key);
        string second = string(x.key);
        int len = min(first.length(), second.length()); // get minimum length of both strings
        for(int i = 0 ; i < len ; i++){
            if(first[i] == second[i]){
                // cout << "HEYYY" << endl;
                continue;
            }
            if(first[i] < second[i]){ // if first comes after second
                // cout << first[i] << " < " << second[i] << " = false" << endl;
                return false;
            } else if (first[i] > second[i]){ // if first comes before second
                // cout << first[i] << " > " << second[i] << " = true" << endl;
                return true;
            } else {
                continue;
            }
        }
        if(first.length() > second.length()){
            return false;
        } else {
            return true;
        }
    }
    bool operator>(const Record &x) { 
        string first = string(this->key);
        string second = string(x.key);
        int len = min(first.length(), second.length()); // get minimum length of both strings
        for(int i = 0 ; i < len ; i++){
            if(first[i] == ' ' and second[i] == ' '){
                // cout << "HEYYY" << endl;
                continue;
            }
            if(first[i] < second[i]){ // if first comes after second
                // cout << first[i] << " < " << second[i] << " = true" << endl;
                return true;
            } else if (first[i] > second[i]){ // if first comes before second
                // cout << first[i] << " > " << second[i] << " = false" << endl;
                return false;
            } else {
                continue;
            }
        }
        if(first.length() > second.length()){
            return true;
        } else {
            return false;
        }
    }
};

// class Record{
// public:
//     char key[KEY_SIZE];
//     double mpg;
//     int cylinders;
//     double displacement;
//     double horsepower;
//     double weight;
//     double acceleration;
//     int model;
//     char origin[ORIGIN_SIZE];
//     Pos prev;
//     Pos next;
//     bool is_deleted = 0;

//     Record(){
//         memset(key, ' ', KEY_SIZE - 1);
//         mpg = 0;
//         cylinders = 0;
//         displacement = 0;
//         horsepower = 0;
//         weight = 0;
//         acceleration = 0;
//         model = 0;
//         memset(origin, ' ', ORIGIN_SIZE - 1);
//         prev = Pos(-1, 'D');
//         next = Pos(-1, 'D');
//         is_deleted = 0;
//     }

//     void show(){
//         cout << "> " << key << "|" << int(prev.pos) << "-" << prev.file << "|" << int(next.pos) << "-" << next.file << "|" << mpg << "|" << cylinders << "|" << displacement << "|" << horsepower << "|" << weight << "|" << acceleration << "|" << model << "|" << origin << "\n";
//     }

//     void set_key(string new_key){
//         for (char& c : new_key) {
//             c = tolower(c);
//         }
//         strncpy(this->key, new_key.c_str(), KEY_SIZE - 1);
//         this->key[KEY_SIZE - 1] = '\0';
//     }

//     void set_data(){
//         cin.ignore();
//         string key, mpg, cylinders, displacement, horsepower, weight, acceleration, model, origin;
        
//         cout << "key (car name): ";
//         getline(cin, key);
//         for (char& c : key) {
//             c = tolower(c);
//         }
//         strncpy(this->key, key.c_str(), KEY_SIZE - 1);
//         this->key[KEY_SIZE - 1] = '\0';

//         cout << "mpg: ";
//         getline(cin, mpg);
//         this->mpg = stod(mpg);

//         cout << "cylinders: ";
//         getline(cin, cylinders);
//         this->cylinders = stoi(cylinders);

//         cout << "displacement: ";
//         getline(cin, displacement);
//         this->displacement = stod(displacement);

//         cout << "horsepower: ";
//         getline(cin, horsepower);
//         this->horsepower = stod(horsepower);

//         cout << "weight: ";
//         getline(cin, weight);
//         this->weight = stod(weight);

//         cout << "acceleration: ";
//         getline(cin, acceleration);
//         this->acceleration = stod(acceleration);

//         cout << "model: ";
//         getline(cin, model);
//         this->model = stoi(model);

//         cout << "origin: ";
//         getline(cin, origin);
//         strncpy(this->origin, origin.c_str(), ORIGIN_SIZE - 1);
//         this->origin[ORIGIN_SIZE - 1] = '\0';

//         this->prev = Pos(-1, 'D');
//         this->next = Pos(-1, 'D');
//         this->is_deleted = false;
//     }
//     bool operator==(const Record &x) { return string(this->key) == string(x.key); }
//     bool operator!=(const Record &x) { return string(this->key) != string(x.key); }

//     bool operator<=(const Record &x) { return string(this->key) <= string(x.key); }
//     bool operator>=(const Record &x) { return string(this->key) >= string(x.key); }

//     bool operator<(const Record &x) { 
//         string first = string(this->key);
//         string second = string(x.key);
//         int len = min(first.length(), second.length()); // get minimum length of both strings
//         for(int i = 0 ; i < len ; i++){
//             if(first[i] == second[i]){
//                 // cout << "HEYYY" << endl;
//                 continue;
//             }
//             if(first[i] < second[i]){ // if first comes after second
//                 // cout << first[i] << " < " << second[i] << " = false" << endl;
//                 return false;
//             } else if (first[i] > second[i]){ // if first comes before second
//                 // cout << first[i] << " > " << second[i] << " = true" << endl;
//                 return true;
//             } else {
//                 continue;
//             }
//         }
//         if(first.length() > second.length()){
//             return false;
//         } else {
//             return true;
//         }
//     }
//     bool operator>(const Record &x) { 
//         string first = string(this->key);
//         string second = string(x.key);
//         int len = min(first.length(), second.length()); // get minimum length of both strings
//         for(int i = 0 ; i < len ; i++){
//             if(first[i] == ' ' and second[i] == ' '){
//                 // cout << "HEYYY" << endl;
//                 continue;
//             }
//             if(first[i] < second[i]){ // if first comes after second
//                 // cout << first[i] << " < " << second[i] << " = true" << endl;
//                 return true;
//             } else if (first[i] > second[i]){ // if first comes before second
//                 // cout << first[i] << " > " << second[i] << " = false" << endl;
//                 return false;
//             } else {
//                 continue;
//             }
//         }
//         if(first.length() > second.length()){
//             return true;
//         } else {
//             return false;
//         }
//     }
// };



// Swap two elements in a vector
void swap(std::vector<Record>& vec, int i, int j) {
    Record temp = vec[i];
    vec[i] = vec[j];
    vec[j] = temp;
}

// Partition the vector for quicksort
int partition(std::vector<Record>& vec, int low, int high) {
    Record pivot = vec[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (vec[j] < pivot) {
            i++;
            swap(vec, i, j);
        }
    }
    swap(vec, i+1, high);
    return i + 1;
}

void quickSort(std::vector<Record>& vec, int low, int high) {
    if (low < high) {
        int pivot = partition(vec, low, high);
        quickSort(vec, low, pivot - 1);
        quickSort(vec, pivot + 1, high);
    }
}

#endif