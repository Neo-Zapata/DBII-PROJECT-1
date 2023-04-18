#include "libs.h"

// #define K 500

/*
JOURNEY:
- DATA MUST BE STORED FROM THE .CSV TO A .DAT (MAIN AND AUX)
    - THIS CAN BE ALREADY DONE, SO MANAGE BOTH SITUATIONS OR EVEN ASK BEFORE DOING IT
- APPLY THE METHODS WATCHING IF REBUILDING IS NECESAARY

REBUILD:
- WHEN IT REACHES 10.000 RECORD
- WHEN IT IS FRAGMENTED (IF IN THE ORIGINAL FILE ARE MANY DELETED RECORDS - MORE THAN 40%)
- WHEN THE AUX FILE REACHES A CERTAIN NUMBER OF MEGABYTES
*/

struct TitanicPassengers{
    int PassengerID;
    bool Survived; // 0 or 1
    int Pclass; // 1, 2 or 3
    string Name;
    string Sex; // male or female
    int Age;
    int SibSp; // # of siblings / spouses aboard the Titanic
    int Parch; // # of parents / children aboard the Titanic
    string Ticket;
    float Fare; // passenger fare
    string Cabin; // cabin number
    char Embarked; // C (Cherbourg), Q (Queenstown), S (southampton)
};

class SequentialFile{
private:
    string dataset;
    string filename;
    string aux_file;
    int disk_accesses = 0;
public:
    SequentialFile(string dataset){
        this->dataset = dataset; // .csv
        this->filename = "file.dat";
        this->aux_file = "aux_" + filename;

        create_file_if_doesnt_exist(this->filename);
        create_file_if_doesnt_exist(this->aux_file);
    }

    void load(){
        string line;
        ifstream infile("dataset/" + this->dataset);

        if(infile.is_open()){
            while(getline(infile, line)){
                stringstream ss(line);
                string token;
                TitanicPassengers passenger;

            }
        } else throw file_not_found("\nCould not open file - load()");
    }

    template<typename T>
    vector<TitanicPassengers> search(T key){
    vector<TitanicPassengers> people;
    return people;
    }

    template<typename T>
    vector<TitanicPassengers> rangeSearch(T begin_key, T end_key){
        vector<TitanicPassengers> people;
        return people;
    }

    /*
    must use auxiliar space to store new record.
    When the auxiliar space reach K records. Apply rebuild of the main file.
    Keeping physical order (key), make sure to leep the pointers updated
    */
    bool add(TitanicPassengers record){ 
        return true;
    }

    template<typename T>
    bool remove(T key){
        return true;
    }

};


