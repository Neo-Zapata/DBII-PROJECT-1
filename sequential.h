#ifndef SEQ_H
#define SEQ_H

#include "libs.h"
#include "Record.h"

int counter = 1;

class SequentialFile{
private:
    string dataset;
    string filename;
    string aux_file;
    string start_pos_file;
    int disk_accesses;
    long RECORD_SIZE;
    bool is_loaded = false;
    Pos start_position;
public:
    SequentialFile(string dataset){
        Record record;
        this->dataset = dataset; // .csv
        this->filename = "file.dat";
        this->aux_file = "aux_" + filename;
        this->start_pos_file = "start_file.dat";
        this->disk_accesses = 0;
        this->RECORD_SIZE = sizeof(record);
        create_file_if_doesnt_exist(this->filename);
        create_file_if_doesnt_exist(this->aux_file);
        create_file_if_doesnt_exist(this->start_pos_file);
        this->start_position = get_start_pos_from_file();

        // cout << get_n_records(this->filename) << endl;
        // cout << get_n_records(this->aux_file) << endl;
    }

    Pos get_start_pos_from_file(){
        Pos posi;
        ifstream sfile("files/" + start_pos_file, ios::binary);
        if(!sfile.is_open())
            throw file_not_found("Could not open file - get_start_pos_from_file()\n");

        if(file_is_empty(this->start_pos_file)){
            posi = Pos(0, 'D');
        } else{
            sfile.seekg(0, ios::beg);
            sfile.read((char *)(&posi), sizeof(posi));
        }
        sfile.close();
        return posi;
    }

    ~SequentialFile(){
        Rebuild();

        Pos new_pos = get_start_pos();

        ofstream sfile("files/" + start_pos_file, ios::binary);
        if(!sfile.is_open())
            cerr << "error" << endl;
            // throw "Could not open file - destructor()\n";

        sfile.seekp(0, ios::beg);
        sfile.write((char *)(&new_pos), sizeof(new_pos));
        
        sfile.close();
    }

    void change_start_pos(Pos new_){
        this->start_position = new_;
    }

    Pos get_start_pos(){
        return this->start_position;
    }

    int get_n_records(string file){
        ifstream ffile("files/" + file, ios::binary);
        if(!ffile.is_open())
            throw file_not_found("Could not open file - get_n_records()\n");
        
        ffile.seekg(0, ios::end);
        long n_bytes = ffile.tellg();
        return int(n_bytes / RECORD_SIZE);

        ffile.close();
    }

    void load(int n_to_read){
        this->disk_accesses = 0;

        // if(this->is_loaded){
        //     cerr << "Data is already loaded - load()\n";
        //     return;
        // }
        if(n_to_read == 0)
            n_to_read = 1000000000;
        int counter = 0;
        string line;
        ifstream ffile("dataset/" + this->dataset, ios::binary);

        if(!ffile.is_open())
            throw file_not_found("Could not open file - load()\n");

        // string key, mpg, cylinders, displacement, horsepower, weight, acceleration, model, origin;
        string anime_id, key, type, episodes, rating, members;
        string first_, second_;
        // string key, type, total, generation, legendary;

        getline(ffile, line); // column names
        this->disk_accesses++;
        
        while(getline(ffile, line)){
            counter++;
            this->disk_accesses++;
            Record record;
            stringstream ss(line);

            getline(ss, anime_id, ',');
            record.anime_id = stoi(anime_id);
            // record.show();

            if(ss.peek() == '"'){
                getline(ss, first_, '"');
                getline(ss, key, '"');
                for (char& c : key) {
                    c = tolower(c);
                }
                strncpy(record.key, key.c_str(), KEY_SIZE - 1);
                record.key[KEY_SIZE - 1] = '\0';
                // record.show();
                getline(ss, second_, ',');
            } else {
                getline(ss, key, ',');
                for (char& c : key) {
                    c = tolower(c);
                }
                strncpy(record.key, key.c_str(), KEY_SIZE - 1);
                record.key[KEY_SIZE - 1] = '\0';
                // record.show();
            }

            getline(ss, type, ',');
            strncpy(record.type, type.c_str(), TYPE_SIZE - 1);
            record.type[TYPE_SIZE - 1] = '\0';
            // record.show();

            getline(ss, episodes, ',');
            strncpy(record.episodes, episodes.c_str(), EPISODE_SIZE - 1);
            record.episodes[EPISODE_SIZE - 1] = '\0';
            // record.show();

            getline(ss, rating, ',');
            record.rating = stof(rating);
            // record.show();

            getline(ss, members, ',');
            record.members = stoi(members);
            // record.show();

            if(file_is_empty(this->filename)){
                clear_file(this->aux_file);
                ofstream outfile("files/" + this->filename, ios::binary | ios::app);
                if(!outfile.is_open())
                    throw file_not_found("Could not open data file - load()\n");

                outfile.seekp(0, ios::beg);
                outfile.write((char *)(&record), RECORD_SIZE);
                this->disk_accesses++;
                outfile.close();
            } else {
                add_record(record, false);
            }
            if(counter >= n_to_read)
                break;

        }
        cout << "There was a total of " << this->disk_accesses << " accesses to disk in load() -> O(n)\n";
        ffile.close();
        // this->is_loaded = true;
    }

    void clear_file(string file){
        ofstream outfile("files/" + file, ios::binary);

        if(!outfile.is_open())
            throw file_not_found("Could not open data file - clear_file()\n");
        
        outfile.close();
    }

    void Rebuild(){
        this->disk_accesses = 0;
        Pos prev_(-1, 'D');
        Pos next_(int(RECORD_SIZE), 'D');

        if(file_is_empty(this->filename) and file_is_empty(this->aux_file)){
            cerr << "There is no data.\n";
        } else {

            char oldname[] = "files/file.dat";
            char newname[] = "files/temporal.dat";

            if(rename(oldname, newname) != 0)
                cout << "error renaming 1.\n";

            fstream ffile("files/temporal.dat", ios::binary | ios::in | ios::out);
            fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);
            ofstream new_file("files/new_file.dat", ios::binary); // this should create a new file
            
            if(!ffile.is_open())
                throw file_not_found("Could not open data file - load()\n");
            
            if(!auxfile.is_open())
                throw file_not_found("Could not open aux file - load()\n");

            if(!new_file.is_open())
                throw file_not_found("Could not open aux file - load()\n");

            Pos start = get_start_pos();
            Record traverse;
            new_file.seekp(0, ios::beg);

            if(start.file == 'A'){
                auxfile.seekg(start.pos, ios::beg);
                auxfile.read((char *)(&traverse), RECORD_SIZE);
                this->disk_accesses++;
            } else if (start.file == 'D'){
                ffile.seekg(start.pos, ios::beg);
                ffile.read((char *)(&traverse), RECORD_SIZE);
                this->disk_accesses++;
            } else{
                throw ayudame_Dios("diosito, soy yo otra vez - read_all_records\n");
            }

            Record temp = traverse;
            // temp.prev = prev_;
            // temp.next = next_;
            // temp.show();
            // new_file.write((char *)(&temp), RECORD_SIZE);
            // this->disk_accesses++;

            while(traverse.next.pos != -1){
                temp.prev = prev_;
                temp.next = next_;
                // temp.show();
                new_file.write((char *)(&temp), RECORD_SIZE);
                this->disk_accesses++;

                if((traverse.next).file == 'D'){
                    ffile.seekg((traverse.next).pos, ios::beg);
                    ffile.read((char *)(&traverse), RECORD_SIZE);
                    this->disk_accesses++;
                } else if((traverse.next).file == 'A'){
                    auxfile.seekg((traverse.next).pos, ios::beg);
                    auxfile.read((char *)(&traverse), RECORD_SIZE);
                    this->disk_accesses++;
                }
                temp = traverse;
                prev_ = Pos(int(next_.pos - RECORD_SIZE), 'D');
                next_ = Pos(int(next_.pos + RECORD_SIZE), 'D');
            }
            temp.prev = prev_;
            temp.next = Pos(-1, 'D');
            // temp.show();
            new_file.write((char *)(&temp), RECORD_SIZE);
            this->disk_accesses++;


            ffile.close();
            auxfile.close();
            new_file.close();

            char to_remove[] = "files/temporal.dat";

            if(std::remove(to_remove) != 0)
                cout << "error removing\n" << endl;

            char n_oldname[] = "files/new_file.dat";
            char n_newname[] = "files/file.dat";

            if(rename(n_oldname, n_newname) != 0)
                cout << "error renaming 2.\n";
            
            
            change_start_pos(Pos(0, 'D'));
        }
        clear_file(this->aux_file);
        cout << "There was a total of " << this->disk_accesses << " accesses to disk in read_all_records() -> O(n)\n";
    }

    void read_all_records(){
        this->disk_accesses = 0;
        if(file_is_empty(this->filename) and file_is_empty(this->aux_file)){
            cerr << "There is no data.\n";
        } else {
            fstream ffile("files/" + this->filename, ios::binary | ios::in | ios::out);
            fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);

            // cout << get_n_records(this->filename) << endl;
            // cout << get_n_records(this->aux_file) << endl;
            
            if(!ffile.is_open())
                throw file_not_found("Could not open data file - load()\n");
            
            if(!auxfile.is_open())
                throw file_not_found("Could not open aux file - load()\n");

            Pos start = get_start_pos();
            // cout << "start position is: ";
            // start.show();
            Record traverse;

            if(start.file == 'A'){
                auxfile.seekg(start.pos, ios::beg);
                auxfile.read((char *)(&traverse), RECORD_SIZE);
                this->disk_accesses++;
            } else if (start.file == 'D'){
                ffile.seekg(start.pos, ios::beg);
                ffile.read((char *)(&traverse), RECORD_SIZE);
                this->disk_accesses++;
            } else{
                throw ayudame_Dios("diosito, soy yo otra vez - read_all_records\n");
            }

            while(traverse.next.pos != -1){
                traverse.show();
                if((traverse.next).file == 'D'){
                    ffile.seekg((traverse.next).pos, ios::beg);
                    ffile.read((char *)(&traverse), RECORD_SIZE);
                    this->disk_accesses++;
                } else if((traverse.next).file == 'A'){
                    auxfile.seekg((traverse.next).pos, ios::beg);
                    auxfile.read((char *)(&traverse), RECORD_SIZE);
                    this->disk_accesses++;
                }
            }
            traverse.show();

            ffile.close();
            auxfile.close();
        }
        cout << "There was a total of " << this->disk_accesses << " accesses to disk in read_all_records() -> O(n)\n";
    }

    bool file_is_empty(string file){
        ifstream ffile("files/" + file, ios::binary);

        if(ffile.is_open()){
            ffile.seekg(0, ios::end);
            if(ffile.tellg() == 0)
                return true;
            else return false;
        } else {
            throw file_not_found("Could not open file - file_is_empty()\n");
        }
        ffile.close();
    }

    bool add_record(Record record, bool start_over){
        if(start_over)
            this->disk_accesses = 0;

        if(file_is_empty(this->filename)){
            fstream outfile("files/" + this->filename, ios::binary | ios::in | ios::out);

            if(!outfile.is_open())
                throw file_not_found("Could not open data file - add_record()\n");

            outfile.seekp(0, ios::beg);
            outfile.write((char *)(&record), RECORD_SIZE);
            this->disk_accesses++;
            outfile.close();

        } else {
            add(record);
        }
        if(start_over)
            cout << "There was a total of " << this->disk_accesses << " accesses to disk in add_record() -> O(logn + k)\n";
        return true;
    }

    Pos pos_binary_search(Record record){

        fstream ffile("files/" + this->filename, ios::binary | ios::in | ios::out);
        fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);

        if(!ffile.is_open())
            throw file_not_found("Could not open data file - add_record()\n");

        if(!auxfile.is_open())
            throw file_not_found("Could not open auxiliar file - add_record()\n");

        Pos final_pos(-1, 'D');
        int left = 0;
        int right = get_n_records(this->filename) - 1;
        int last_record = right;
        // cout << "right: " << right << endl;
        int mid = floor((left + right) / 2);
        while(left <= right) {
            mid = floor((left + right) / 2);
            // cout << "mid: " << mid << endl;
            ffile.seekg(mid * RECORD_SIZE, ios::beg);
            Record traverse;
            ffile.read((char *)(&traverse), RECORD_SIZE);
            this->disk_accesses++;
            if(traverse == record){
                // if we are adding, this case is probably never met (but it still works if we insert the same key twice)
                break;
            } else if(traverse > record){
                left = mid + 1;
                // if(mid == 0){
                //     break;
                // }
            } else {
                right = mid - 1;
                // if(mid == last_record){
                //     break;
                // }
            }
        }
        final_pos.pos = mid * RECORD_SIZE;
        ffile.close();
        auxfile.close();
        return final_pos;
    }

    Pos look_prevs(Pos start_p, Record record, Pos& begin){

        fstream ffile("files/" + this->filename, ios::binary | ios::in | ios::out);
        fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);

        if(!ffile.is_open())
            throw file_not_found("Could not open data file - add_record()\n");

        if(!auxfile.is_open())
            throw file_not_found("Could not open auxiliar file - add_record()\n");

        Pos fin;
        // Pos iter_pos = start_p;
        begin = start_p;
        Record traverse = read_record_by_pos(begin);

        // cout << record.key << " viene antes de " << traverse.key << " ? " << (record > traverse) << endl;
        
        if(record > traverse){ // podemos avanzar
            do{
                if(traverse.prev.pos == -1){
                    // cout << "WE ARE THE NEW FIRST RECORD" << endl;
                    // fin = Pos(0, 'D');
                    fin = get_start_pos();
                    return fin;
                }
                // cout << "PODEMOS AVANZAR EN LOOK PREVS" << endl;
                begin = traverse.prev;
                traverse = read_record_by_pos(begin);
                // iter_pos = traverse.next;
                // throw ayudame_Dios("??1");
                // cout << record.key << " viene antes de " << traverse.key << " ? " << (record > traverse) << endl;
            } while(record > traverse);
            fin = traverse.next;
        } else { // no podemos avanzar
            // cout << "NO PODEMOS AVANZAR EN LOOK PREVS" << endl;
            fin = Pos(-1, 'X'); // the X means we cannot look forward
        }
        ffile.close();
        auxfile.close();
        return fin;
    }

    Pos look_nexts(Pos start_p, Record record, Pos& begin){

        fstream ffile("files/" + this->filename, ios::binary | ios::in | ios::out);
        fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);

        if(!ffile.is_open())
            throw file_not_found("Could not open data file - add_record()\n");

        if(!auxfile.is_open())
            throw file_not_found("Could not open auxiliar file - add_record()\n");

        Pos fin;
        // Pos iter_pos = start_p;
        begin = start_p;
        Record traverse = read_record_by_pos(begin);
        
        // cout << record.key << " viene despues de " << traverse.key << " ? " << (record < traverse) << endl;
        
        if(record < traverse){ // podemos avanzar
            do{
                if(traverse.next.pos == -1){
                    // cout << "WE ARE THE NEW LAST RECORD" << endl;
                    auxfile.seekg(0, ios::end);
                    fin = Pos(auxfile.tellg(), 'A');
                    // fin = Pos(-1, 'D');
                    return fin;
                }
                // cout << "PODEMOS AVANZAR EN LOOK NEXTS" << endl;
                begin = traverse.next;
                
                traverse = read_record_by_pos(begin);
                
                // cout << record.key << " viene despues de " << traverse.key << " ? " << (record < traverse) << endl;
            } while(record < traverse);
            fin = begin;
        } else { // no podemos avanzar
            // cout << "NO PODEMOS AVANZAR EN LOOK NEXTS" << endl;
            fin = Pos(-1, 'X'); // the X means we cannot look forward
        }
        ffile.close();
        auxfile.close();
        return fin;
    }

    void get_positions(Record record, Pos& begin, Pos& mid, Pos& end){
        fstream ffile("files/" + this->filename, ios::binary | ios::in | ios::out);
        fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);

        if(!ffile.is_open())
            throw file_not_found("Could not open data file - add_record()\n");

        if(!auxfile.is_open())
            throw file_not_found("Could not open auxiliar file - add_record()\n");
        Pos near_position = pos_binary_search(record);
        // mid is the possible closest candidate, but we look for its prev and next to confirm this.

        Pos final_pos_to_insert;
        Pos possible_final_pos_1;
        Pos possible_final_pos_2;

        auxfile.seekg(0, ios::end);
        mid = Pos(auxfile.tellg(), 'A');

        Record near_record = read_record_by_pos(near_position);

        Pos pos_beg_1, pos_beg_2;
        possible_final_pos_1 = look_prevs(near_position, record, pos_beg_1);
        possible_final_pos_2 = look_nexts(near_position, record, pos_beg_1);

        // possible_final_pos_1.show();
        // possible_final_pos_2.show();
        // cout << "--------" << endl;
        // pos_beg_1.show();
        // pos_beg_2.show();
        // cout << "-" << endl;

        if(possible_final_pos_2.file == 'X' and possible_final_pos_1.file == 'X'){
            throw ayudame_Dios("Que esta pasando aqui?");
        } else if(possible_final_pos_2.file != 'X' and possible_final_pos_1.file != 'X'){
            // throw ayudame_Dios("Que esta pasando aqui? x2");
            cout << "???" << endl;
        }

        if(possible_final_pos_1.file == 'X'){
            final_pos_to_insert = possible_final_pos_2;
            begin = pos_beg_1;
        } else {
            final_pos_to_insert = possible_final_pos_1;
            begin = pos_beg_2;
        }

        if(final_pos_to_insert == get_start_pos()){
            // cout << "IT IS THE NEW FIRST RECORD" << endl;
            end = final_pos_to_insert;
            begin = Pos(-1, 'X'); // it wont be used,
            auxfile.flush();
            auxfile.seekg(0, ios::end);
            mid = Pos(auxfile.tellg(), 'A');
        } else if(final_pos_to_insert == mid){
            // cout << "IT IS THE NEW LAST RECORD" << endl;
            auxfile.seekg(0, ios::end);
            mid = Pos(auxfile.tellg(), 'A');
            end = mid;
            // begin already calculated
            // Pos x_begin = get_last_pos_from(near_position);
            // begin.show();
            // x_begin.show();
            // begin = x_begin;
            // begin = get_last_pos_from(near_position);
        } else {
            // cout << "IT IS A MID POSITION" << endl;
            Record end_record = read_record_by_pos(final_pos_to_insert);
            begin = end_record.prev;
            end = final_pos_to_insert;
            auxfile.seekg(0, ios::end);
            mid = Pos(auxfile.tellg(), 'A');
        }
        ffile.close();
        auxfile.close();
    }

    Pos get_last_pos_from(Pos posi){
        fstream ffile("files/" + this->filename, ios::binary | ios::in | ios::out);
        fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);

        if(!ffile.is_open())
            throw file_not_found("Could not open data file - add_record()\n");

        if(!auxfile.is_open())
            throw file_not_found("Could not open auxiliar file - add_record()\n");

        Record temp = read_record_by_pos(posi);

        while(temp.next.pos != -1){
            posi = temp.next;
            temp = read_record_by_pos(posi);
        }
        return posi;

        ffile.close();
        auxfile.close();
    }

    // there is at least 1 record in datafile
        //check if it is at the beginning (special case)
        // else, check if the pos is deleted
            // if it is deleted then insert there and update the pointers (special case)
            // else, get the 3 position and send to update_pointers(begin, mid, end)
    bool add(Record record){ // 3 cases
        Pos begin;
        Pos mid;
        Pos end;
        get_positions(record, begin, mid, end);
        // cout << "-----"<< endl;
        // begin.show();
        // mid.show();
        // end.show();
        // cout << "-----"<< endl;
        if(is_deleted(end)){
            // cout << "it is a deleted position, so just replace it" << endl; // now i need to update the pointers
            Pos temp_pos = end;
            Record deleted_record = read_record_by_pos(temp_pos);

            do{ // we are looking for the first prev alive (bc that is our begin)
                temp_pos = deleted_record.prev;
                deleted_record = read_record_by_pos(temp_pos);
            }while(deleted_record.is_deleted == true);

            begin = temp_pos;
            mid = end;
            Record begin_record = read_record_by_pos(begin);
            end = begin_record.next;

            save_record_by_pos(record, mid);
            update_pointers(begin, mid, end);

        } else if(end == get_start_pos()){ // it is the beginning
            // cout << "insert to the beginning of the file" << endl;
            record.next = end; // = get_Start_position();
            record.prev = Pos(-1, 'D');
            Record prev_start_pos = read_record_by_pos(end);
            prev_start_pos.prev = mid;

            save_record_by_pos(prev_start_pos, end);
            save_record_by_pos(record, mid);
            change_start_pos(mid);

            // cout << "final arrange: " << endl;
            // record.show();
            // prev_start_pos.show();

        } else if(mid == end){ // after the first element (the pointer is -1)
            // cout << "mid == end" << endl;
            record.prev = begin;
            record.next = Pos(-1, 'D');
            // mid.show();
            // end.show();
            save_record_by_pos(record, mid);
            Record begin_record = read_record_by_pos(begin);
            begin_record.next = mid;
            save_record_by_pos(begin_record, begin);

            // cout << "final arrange: " << endl;
            // record.show();
            // begin_record.show();
        } else{ // normal case
            // cout << "normal case, use the 3 positions" << endl;
            // insert the record
            save_record_by_pos(record, mid);
            update_pointers(begin, mid, end);
        }
        return true;
    }

    bool is_deleted(Pos posi){
        
        Record temp = read_record_by_pos(posi);

        if(temp.is_deleted == true){
            return true;
        } else {
            return false;
        }
    }

    void save_record_by_pos(Record record, Pos posi){

        fstream ffile("files/" + this->filename, ios::binary | ios::in | ios::out);
        fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);

        if(!ffile.is_open())
            throw file_not_found("Could not open data file - add_record()\n");

        if(!auxfile.is_open())
            throw file_not_found("Could not open auxiliar file - add_record()\n");


        if(posi.file == 'A'){
            auxfile.seekp(int(posi.pos), ios::beg);
            auxfile.write((char *)(&record), RECORD_SIZE);
            this->disk_accesses++;
        } else if(posi.file == 'D'){
            ffile.seekp(posi.pos, ios::beg);
            ffile.write((char *)(&record), RECORD_SIZE);
            this->disk_accesses++;
        } else {
            ffile.close();
            auxfile.close();
            throw ayudame_Dios("diosito, soy yo otra vez - save_record_by_pos()\n");
        }
        ffile.close();
        auxfile.close();
    }

    void update_pointers(Pos begin, Pos mid, Pos end){

        Record begin_record = read_record_by_pos(begin);
        Record mid_record   = read_record_by_pos(mid);
        Record end_record   = read_record_by_pos(end);

        mid_record.next = begin_record.next;
        mid_record.prev = end_record.prev;

        begin_record.next = mid;
        end_record.prev = mid;

        save_record_by_pos(begin_record, begin);
        save_record_by_pos(mid_record, mid);
        save_record_by_pos(end_record, end);

        // cout << "final arrange: " << endl;
        // begin_record.show();
        // mid_record.show();
        // end_record.show();
    }

    Record read_record_by_pos(Pos posi){
        fstream ffile("files/" + this->filename, ios::binary | ios::in | ios::out);
        fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);

        if(!ffile.is_open())
            throw file_not_found("Could not open data file - add_record()\n");

        if(!auxfile.is_open())
            throw file_not_found("Could not open auxiliar file - add_record()\n");

        Record temp;
        if(posi.file == 'A'){
            auxfile.seekg(posi.pos, ios::beg);
            auxfile.read((char *)(&temp), RECORD_SIZE);
            this->disk_accesses++;
        } else if(posi.file == 'D'){
            ffile.seekg(posi.pos, ios::beg);
            ffile.read((char *)(&temp), RECORD_SIZE);
            this->disk_accesses++;
        } else {
            throw ayudame_Dios("diosito, soy yo otra vez - read_record_by_pos()\n");
        }

        ffile.close();
        auxfile.close();
        return temp;
    }

    bool remove(string key){
        std::clock_t start_time = std::clock();
        this->disk_accesses = 0;

        Record temp;
        temp.set_key(key);

        ifstream infile("files/" + this->filename, ios::binary);

        if(!infile.is_open())
            throw file_not_found("Could not open data file - remove()\n");

        if(file_is_empty(this->filename)){
            cerr << "there is no data in the file";
            return false;
        }

        Pos begin, mid, end;
        get_positions(temp, begin, mid, end);

        // necesitamos hacer un desfase de las posiciones (para atras) (We only really need begin to get all of them)

        Record begin_record = read_record_by_pos(begin);
        // Record end_record = read_record_by_pos(end);

        Record end_record = read_record_by_pos(begin_record.next);
        Record mid_record = begin_record;
        begin_record = read_record_by_pos(begin_record.prev);
        
        end = begin_record.next;
        mid = begin;
        begin = begin_record.prev;

        // begin_record.show();
        // mid_record.show();
        // end_record.show();

        if(mid_record.next.pos == -1 and mid_record.prev.pos == -1){ // removing the onnly element in the datafile
            clear_file(this->filename);
            clear_file(this->aux_file);
            change_start_pos(Pos(0, 'D'));
        } else if(mid_record.next.pos == -1){ // remove last element
            mid_record.is_deleted = true;
            begin_record.next = mid_record.next;
            save_record_by_pos(begin_record, begin);
            save_record_by_pos(mid_record, mid);
        } else if (mid_record.prev.pos == -1){ // remove first element
            mid_record.is_deleted = true;
            end_record.prev = mid_record.prev;
            save_record_by_pos(mid_record, mid);
            save_record_by_pos(end_record, end);
            change_start_pos(end);
        } else { // remove any element
            mid_record.is_deleted = true;
            begin_record.next = mid_record.next;
            end_record.prev = mid_record.prev;
            save_record_by_pos(mid_record, mid);
            save_record_by_pos(end_record, end);
            save_record_by_pos(begin_record, begin);
        }

        // cout << "final_arrannge:" << endl;

        // begin_record.show();
        // mid_record.show();
        // end_record.show();

        cout << "There was a total of " << this->disk_accesses << " accesses to disk in remove()\n";
        std::clock_t end_time = std::clock();
        double elapsed_time = double(end_time - start_time) / CLOCKS_PER_SEC;
        // std::cout << "REMOVED: Elapsed time: " << elapsed_time << " seconds" << std::endl;
        return true;
    }

    vector<Record> search(string key){
        std::clock_t start_time = std::clock();
        this->disk_accesses = 0;
        vector<Record> records;
        Record rec;

        ifstream infile("files/" + this->filename, ios::binary);

        if(!infile.is_open())
            throw file_not_found("Could not open data file - asearch()\n");

        if(file_is_empty(this->filename)){
            cerr << "there is no data in the file";
            return records;
        }

        // infile.seekg(0, ios::end);
        // int filesize = infile.tellg();

        Record temp;
        temp.set_key(key);

        Pos best_candidate = pos_binary_search(temp);
        // if key == ford.
        // answer == ford fiesta, ford impala, etc.
        // best_candidate.show();
        Record candidate = read_record_by_pos(best_candidate);
        // candidate.show();
        vector<Record> before;
        vector<Record> after;

        if(string(candidate.key).find(key) != string::npos){
            // the candidate matches, we look forward and backwards
            // cout << "forth and back" << endl;
            records.push_back(candidate);
            before = look_before(candidate.prev, key, false);
            after = look_after(candidate.next, key, false);
        } else if(candidate > temp){ // candidate comes before key, so we can only look forward
            // cout << "onnly after" << endl;
            after = look_after(candidate.next, key, false);
        } else if(candidate < temp){ // candidate comes after key, so we can only look backwards
            // cout << "onnly before" << endl;
            before = look_before(candidate.prev, key, false);
        } else {
            throw ayudame_Dios("soy yo otra vez");
        }

        // for(Record rec : before)
        //     rec.show();



        records.insert(records.end(), before.begin(), before.end());
        records.insert(records.end(), after.begin(), after.end());

        // for(Record rec : records)
        //     rec.show();

        infile.close();
        cout << "There was a total of " << this->disk_accesses << " accesses to disk in search()\n";
        std::clock_t end_time = std::clock();
        double elapsed_time = double(end_time - start_time) / CLOCKS_PER_SEC;
        // std::cout << "SEARCH: Elapsed time: " << elapsed_time << " seconds" << std::endl;
        return records;
    }

    vector<Record> look_before(Pos posi, string key, bool is_range){
        vector<Record> result;
        Record traverse;
        size_t found;

        Record key_record;
        key_record.set_key(key);

        fstream ffile("files/" + this->filename, ios::binary | ios::in | ios::out);
        fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);

        if(!ffile.is_open())
            throw file_not_found("Could not open data file - add_record()\n");

        if(!auxfile.is_open())
            throw file_not_found("Could not open auxiliar file - add_record()\n");

        bool first_match_found = false;

        if(!is_range){
            do{
                traverse = read_record_by_pos(posi);
                found = string(traverse.key).find(key);
                if(found != string::npos){
                    first_match_found = true;
                    result.push_back(traverse);
                } else {
                    if(first_match_found == true)
                        break;
                }
                posi = traverse.prev;
            } while((string(traverse.key).find(key) != string::npos or key_record > traverse) and traverse.prev.pos != -1);
        } else {
            // cout << "range oop" << endl;
            // agregar todos hasta que traverse venga antes de key o hasta qwu traverse.prev = -1
            do{
                traverse = read_record_by_pos(posi);
                result.push_back(traverse);
                posi = traverse.prev;
                // cout << key_record.key << " > " << traverse.key << " ? " << (key_record > traverse) << endl;
            } while((string(traverse.key).find(key) != string::npos or key_record > traverse) and traverse.prev.pos != -1);
            result.pop_back();
        }
       
        ffile.close();
        auxfile.close();
        return result;
    }

    vector<Record> look_after(Pos posi, string key, bool is_range){
        vector<Record> result;
        Record traverse;
        size_t found;

        Record key_record;
        key_record.set_key(key);

        fstream ffile("files/" + this->filename, ios::binary | ios::in | ios::out);
        fstream auxfile("files/" + this->aux_file, ios::binary | ios::in | ios::out);

        if(!ffile.is_open())
            throw file_not_found("Could not open data file - add_record()\n");

        if(!auxfile.is_open())
            throw file_not_found("Could not open auxiliar file - add_record()\n");

        bool first_match_found = false;
        if(!is_range){
            do{
                traverse = read_record_by_pos(posi);
                found = string(traverse.key).find(key);
                if(found != string::npos){
                    first_match_found = true;
                    result.push_back(traverse);
                } else {
                    if(first_match_found == true)
                        break;
                }
                posi = traverse.next;
            } while((key_record < traverse or string(traverse.key).find(key) != string::npos) and traverse.next.pos != -1);
        } else {
            // cout << "range oop" << endl;
            // agregar todos hasta que traverse venga despues de key o hasta qwu traverse.next = -1
            do{
                traverse = read_record_by_pos(posi);
                result.push_back(traverse);
                posi = traverse.next;
                // cout << key_record.key << " < " << traverse.key << " ? " << (key_record < traverse) << endl;
            } while((key_record < traverse or string(traverse.key).find(key) != string::npos) and traverse.next.pos != -1);
            result.pop_back();
        }

       

        ffile.close();
        auxfile.close();
        return result;
    }

    vector<Record> range_search(string from, string to){
        // cout << from << " - " << to << endl;
        this->disk_accesses = 0;
        vector<Record> records;

        fstream infile("files/" + this->filename, ios::binary | ios::in | ios::out);

        if(!infile.is_open())
            throw file_not_found("Could not open data file - add_record()\n");

        Record from_record, to_record;

        from_record.set_key(from);
        to_record.set_key(to);

        if(file_is_empty(this->filename)){
            cerr << "there is no data in the file";
            return records;
        }

        // if(to_record < from_record){
        //     cerr << "from comes after to." << endl;
        //     return records;
        // } else if (from_record > to_record){
        //     cerr << "to comes before from." << endl;
        //     return records;
        // }

        Pos best_candidate_from = pos_binary_search(from_record);
        Pos best_candidate_to = pos_binary_search(to_record);

        // best_candidate_from.show();
        // best_candidate_to.show();

        Record candidate_from = read_record_by_pos(best_candidate_from);
        Record candidate_to = read_record_by_pos(best_candidate_to);

        vector<Record> before;
        vector<Record> after;

        // if(candidate_from < from_record or candidate_to > to_record){ // we need to go back
        //     before = look_before(candidate_from.prev, from, true);
        //     after = look_after(candidate_from.prev, to, true);
            // it will catch all in between
        // } else 
        if(candidate_from > from_record){ // our estimation is wrong and to low
            // cout << "our estimation is wrong and to low" << endl;
            Pos posi = candidate_from.next;
            do{
                candidate_from = read_record_by_pos(posi);
                posi = candidate_from.next;
            }while(candidate_from > from_record or candidate_from.next.pos == -1);
            // candidate_from.show();
        } else if(candidate_to < to_record){ // our estimation is wrong and to high
            // cout << "our estimation is wrong and to high" << endl;
            Pos posi = candidate_to.prev;
            do{
                
                candidate_to = read_record_by_pos(posi);
                posi = candidate_to.prev;
            }while(candidate_to < to_record or candidate_to.prev.pos == -1);
            // candidate_to.show();
        } else if(candidate_from != candidate_to){
            throw ayudame_Dios("por que a mi?");
        }

        // cout << "our estimation should be good" << endl;
        // candidate_from.show();
        before = look_before(candidate_from.prev, from, true);
        after = look_after(candidate_from.prev, to, true); 

        // for(Record rec : before)
        //     rec.show();
        // cout << "-----" << endl;
        // for(Record rec : after)
        //     rec.show();

        records.insert(records.end(), before.begin(), before.end());
        records.insert(records.end(), after.begin(), after.end());

        // for(Record rec : records)
        //     rec.show();

        infile.close();
        cout << "There was a total of " << this->disk_accesses << " accesses to disk in range_search()\n";
        return records;
    }
};


#endif