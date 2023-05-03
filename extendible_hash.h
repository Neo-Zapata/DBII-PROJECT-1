#include "structures.h"
using namespace std;

class extendible_hash
{
private:
    fstream index_file;
    fstream data_file;
    int D;

    int index_size();
    bool index_empty();
    Index start_index(int key, int d);
    Index read_index(int pos_log); // Initialize hash_table from index.dat

    int data_size();
    void write_bucket(Bucket &bucket, int pos_log);
    Bucket read_bucket(int pos_log);
    void add_to_bucket(Index &index, Bucket &bucket, Record_h record);

    int increment_depth(int key);
    void resize(Index &index, Bucket &bucket, Index &new_index, Bucket &new_bucket, int d);
    void overflow_handle(Index &index, Bucket &bucket, Record_h &record);

public:
    unordered_map<int, int> hash_table;
    extendible_hash(int D, string index_filename, string data_filename);

    int hash_function(int record_key);
    void update_hash_table();
    Index search_index(int key);
    Index iterate_key(int key);

    void insert_record(Record_h record);
    bool search_record(int record_key, Record_h &record);
    bool delete_record(int record_key);

    void print_all();

    int memory_accesses = 0;
};
