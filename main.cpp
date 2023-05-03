#include "sequential.h"
#include "AVLSetFile.h"
#include "libs.h"
#include "parse.h"
#include <chrono>

void Clear()
{
    cout << "\x1B[2J\x1B[H";
}

void ingrese_0_para_salir(int &opcion)
{
    do
    {
        cout << "\nIngrese 0 para salir: ";
        cin >> opcion;
        Clear();
    } while (opcion != 0);
}

void Interactive_Menu_ExtendibleHash()
{
    extendible_hash ext_hash(16, "index.dat", "data.dat");
    int opcion;
    bool repetir = true;

    do
    {
        Clear();
        cout << "\n\n        Menu " << endl;
        cout << "--------------------" << endl;
        cout << "1. Load .csv" << endl;
        cout << "2. Add record" << endl;
        cout << "3. Remove record" << endl;
        cout << "4. Search record" << endl;
        cout << "5. Show all records" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while (opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5)
        {
            cerr << "\nEnter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }

        switch (opcion)
        {
        case 1:
        {
            ext_hash.memory_accesses = 0;
            string n;
            cout << "How many records you want to load? (0 if all)\n";
            cin.ignore();
            getline(cin, n);
            if (n == "0")
            {
                auto start = std::chrono::high_resolution_clock::now();
                read_dataset("animedata2.csv", ext_hash);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                cout << "ADD: Elapsed time: " << duration.count() << " segundos"
                     << "\n";
            }
            else
            {
                auto start = std::chrono::high_resolution_clock::now();
                read_dataset_until("animedata2.csv", ext_hash, stoi(n));
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                cout << "ADD: Elapsed time:  " << duration.count() << " seconds"
                     << "\n";
            }
        }
        break;
        case 2:
        {
            ext_hash.memory_accesses = 0;
            Record_h record;
            record.set_data();
            auto start = std::chrono::high_resolution_clock::now();
            ext_hash.insert_record(record);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
            cout << "ADD: Elapsed time:  " << duration.count() << " seconds"
                 << "\n";
            ingrese_0_para_salir(opcion);
        }
        break;
        case 3:
        {
            ext_hash.memory_accesses = 0;
            string key;
            cout << "Insert key to remove:";
            cin.ignore();
            getline(cin, key);
            auto start = std::chrono::high_resolution_clock::now();
            cout << ext_hash.delete_record(stoi(key)) << endl;
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
            cout << "DELETE: Elapsed time:  " << duration.count() << " seconds"
                 << "\n";
            ingrese_0_para_salir(opcion);
        }
        break;
        case 4:
        {
            ext_hash.memory_accesses = 0;
            string key;
            cout << "Insert key to search:\n";
            cin.ignore();
            getline(cin, key);
            Record_h searched;

            auto start = std::chrono::high_resolution_clock::now();
            if (ext_hash.search_record(stoi(key), searched) == true)
            {
                searched.showData();
                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> duration = end - start;

                cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                cout << "SEARCH: Elapsed time:  " << duration.count() << " seconds"
                     << "\n";
            }
            ingrese_0_para_salir(opcion);
        }
        break;
        case 5:
        {
            ext_hash.print_all();
            ingrese_0_para_salir(opcion);
        }
        break;
        case 0:
        {
            repetir = false;
        }
        break;
        default:
        {
            cout << "\nInvalid option.";
        }
        break;
        }
    } while (repetir);
}

void Interactive_Menu_SequentialFile()
{
    SequentialFile SF("animedata.csv");
    int opcion;
    bool repetir = true;

    do
    {
        Clear();
        cout << "\n\n        Menu " << endl;
        cout << "--------------------" << endl;
        cout << "1. Load .csv" << endl;
        cout << "2. Add record" << endl;
        cout << "3. Remove record" << endl;
        cout << "4. Search record" << endl;
        cout << "5. Range search record" << endl;
        cout << "6. Show all records" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while (opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5 and opcion != 6)
        {
            cerr << "\nEnter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }

        switch (opcion)
        {
        case 1:
        {
            string n;
            cout << "How many records you want to load? (0 if all)\n";
            cin.ignore();
            getline(cin, n);
            std::clock_t start = std::clock();
            SF.load(stoi(n));
            std::clock_t end = std::clock();
            double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
            std::cout << "ADD: Elapsed time: " << elapsed_time << " seconds" << std::endl;
            ingrese_0_para_salir(opcion);
        }
        break;
        case 2:
        {
            Record record;
            record.set_data();
            SF.add_record(record, true);
            ingrese_0_para_salir(opcion);
        }
        break;
        case 3:
        {
            string key;
            cout << "Insert key to remove:";
            cin.ignore();
            getline(cin, key);
            std::clock_t start = std::clock();
            cout << SF.remove(key) << endl;
            std::clock_t end = std::clock();
            double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
            std::cout << "REMOVE: Elapsed time: " << elapsed_time << " seconds" << std::endl;
            ingrese_0_para_salir(opcion);
        }
        break;
        case 4:
        {
            string key;
            cout << "Insert key to search:\n";
            // string key = get_key_from_user();
            // cin >> key;
            cin.ignore();
            getline(cin, key);
            std::clock_t start = std::clock();
            vector<Record> records = SF.search(key);
            std::clock_t end = std::clock();
            double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
            std::cout << "SEARCH: Elapsed time: " << elapsed_time << " seconds" << std::endl;
            for (Record rec : records)
                rec.show();
            ingrese_0_para_salir(opcion);
        }
        break;
        case 5:
        {
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
            for (Record rec : records)
                rec.show();
            ingrese_0_para_salir(opcion);
        }
        break;
        case 6:
        {
            SF.read_all_records();
            ingrese_0_para_salir(opcion);
        }
        break;
        case 0:
        {
            repetir = false;
        }
        break;
        default:
        {
            cout << "\nInvalid option.";
        }
        break;
        }
    } while (repetir);
}

void Interactive_Menu_AVLFile(){
    AVLFile avl_file;
    int opcion;
    bool repetir = true;

    do
    {
        //Clear();
        cout << "\n\n        Menu " << endl;
        cout << "--------------------" << endl;
        cout << "1. Load .csv" << endl;
        cout << "2. Add record" << endl;
        cout << "3. Remove record" << endl;
        cout << "4. Search record" << endl;
        cout << "5. Range search record" << endl;
        cout << "6. Show all records" << endl;
        cout << "7. Print Preorder" << endl;
        cout << "8. Print Inorder" << endl;
        cout << "9. Range Search Rating" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while (opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5 and opcion != 6 and opcion != 7 and opcion != 8 and opcion != 9) {
            cerr << "\nEnter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }

        switch (opcion)
        {
        case 1: {            
            read_write_dataset("animedata2.csv", "animedata_avl.dat", avl_file);
        }
        break;
        case 2: {
            AVLRecord record;
            record.set_data();
            avl_file.insert(record);
            ingrese_0_para_salir(opcion);
        }
        break;
        case 3: {
            int key;
            cout << "Insert key to remove:";            
            cin >> key;
            std::clock_t start = std::clock();
            avl_file.remove(key) ;
            std::clock_t end = std::clock();
            double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
            std::cout << "REMOVE: Elapsed time: " << elapsed_time << " seconds" << std::endl;
            ingrese_0_para_salir(opcion);
        }
        break;
        case 4: {
            int key;
            cout << "Insert key to search:\n";
            // string key = get_key_from_user();
            cin >> key;
           
            std::clock_t start = std::clock();
            avl_file.find(key);
            std::clock_t end = std::clock();
            double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
            std::cout << "SEARCH: Elapsed time: " << elapsed_time << " seconds" << std::endl;
           /*  for (Record rec : records)
                rec.show(); */
            ingrese_0_para_salir(opcion);
        }
        break;
        case 5: {
             int key1;int key2;
            cout << "first key:\n";
            cin >> key1;
            
            cout << "second key:\n";
            cin >> key2;
            
            vector<AVLRecord> records = avl_file.rangeSearch(key1, key2);
            for (AVLRecord rec : records)
                cout<<rec;
            ingrese_0_para_salir(opcion);
        }
        break;
        case 6:
        {
            read_binary_avl();
            ingrese_0_para_salir(opcion);
        }
        break;
        case 7: {
            avl_file.printPreorder();
            ingrese_0_para_salir(opcion);
        }
        break;
        case 8: {
            avl_file.printInorder();
            ingrese_0_para_salir(opcion);
        }
        break;
        case 9: {
             float key1;float key2;
            cout << "first rating key:\n";
            cin >> key1;
            
            cout << "second rating key:\n";
            cin >> key2;
            
            vector<AVLRecord> records = avl_file.rangeSearchRating(key1, key2);
            for (AVLRecord rec : records)
                cout<<rec;
            ingrese_0_para_salir(opcion);
        }
        case 0:
        {
            repetir = false;
        }
        break;
        default:
        {
            cout << "\nInvalid option.";
        }
        break;
        }
    } while (repetir);
}
void MMENU()
{
    int opcion;
    bool repetir = true;

    do
    {
        Clear();
        cout << "\n\n        Menu " << endl;
        cout << "--------------------" << endl;
        cout << "1. Sequential Indexing" << endl;
        cout << "2. Hash Indexing" << endl;
        cout << "3. AVL Tree" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while (opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3)
        {
            cerr << "\nEnter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }

        switch (opcion)
        {
        case 1:
        {
            Interactive_Menu_SequentialFile();
        }
        break;
        case 2:
        {
            Interactive_Menu_ExtendibleHash();
        }
        break;
        case 3:
        {
            Interactive_Menu_AVLFile();
        }
        break;
        case 0:
        {
            repetir = false;
        }
        break;
        default:
        {
            cout << "\nInvalid option.";
        }
        break;
        }
    } while (repetir);
}

int main()
{
    try
    {
        MMENU();
    }
    catch (const exception &e)
    {
        cerr << "\nError - " << e.what() << endl;
    }
}