#include "sequential.h"
#include "libs.h"

void Clear(){
    cout << "\x1B[2J\x1B[H";
}

void ingrese_0_para_salir(int& opcion){
    do{
        cout << "\nIngrese 0 para salir: ";
        cin >> opcion;
        Clear();
    } while(opcion != 0);
}

void Interactive_Menu_SequentialFile(){
    SequentialFile SF("Pokemon.csv");
    int opcion;
    bool repetir = true;

    do {
        Clear();
        cout << "\n\n        Menu " << endl;
        cout << "--------------------"<<endl;
        cout << "1. Load .csv" << endl;
        cout << "2. Add record" << endl;
        cout << "3. Remove record" << endl;
        cout << "4. Search record" << endl;
        cout << "5. Range search record" << endl;
        cout << "6. Show all records" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while(opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5 and opcion != 6){
            cerr << "\nEnter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }

        switch (opcion) {
            case 1:{
                SF.load();
                ingrese_0_para_salir(opcion);
            }break;
            case 2:{
                Record record;
                record.set_data();
                SF.add_record(record, true);
                ingrese_0_para_salir(opcion);
            }break;
            case 3:{
                string key;
                cout << "Insert key to remove:";
                cin.ignore();
                getline(cin, key);
                cout << SF.remove(key) << endl;
                ingrese_0_para_salir(opcion);
            }break;
            case 4:{
                string key;
                cout << "Insert key to search:\n";
                // string key = get_key_from_user();
                // cin >> key;
                cin.ignore();
                getline(cin, key);
                vector<Record> records = SF.search(key);
                for(Record rec : records)
                    rec.show();
                ingrese_0_para_salir(opcion);
            }break;
            case 5:{
                string from, to;
                cout << "first key:\n";
                cin.ignore();
                getline(cin, from);
                // string from = get_key_from_user();
                // cin >> from;
                cout << "Press enter to continue\n";
                cin.ignore();
                cout << "second key:\n";
                getline(cin, to);
                // string to = get_key_from_user();
                // cin >> to;
                vector<Record> records = SF.range_search(from, to);
                for(Record rec : records)
                    rec.show();
                ingrese_0_para_salir(opcion);
            }break;
            case 6:{
                SF.read_all_records();
                ingrese_0_para_salir(opcion);
            }break;
            case 0:{
                repetir = false;
            }break;
            default:{
                cout << "\nInvalid option.";
            }break;
        }
    } while (repetir);
}


int main(){
    try {
        Interactive_Menu_SequentialFile();
    } catch (const exception& e){
        cerr << "\nError - " << e.what() << endl;
    }
}