#include "extendible_hash.h"
using namespace std;

// Number of bits in index.dat file
int extendible_hash::index_size()
{
    index_file.seekg(0, ios::end);
    return index_file.tellg();
}

// Verify if index.dat file is empty
bool extendible_hash::index_empty()
{
    return index_size() == 0;
}

// Initialize an index, associates it with the hash_table and write it in index.dat. (push_back)
// initialize the associated bucket and write it in data.dat. (push_back)
// Returns the index object initialized
Index extendible_hash::start_index(int key, int d)
{
    int pos_log = data_size() / sizeof(Bucket);
    Index index = Index(key, pos_log);
    hash_table[index.key] = index.pos_log;

    index_file.seekp(index_size(), ios::beg);
    index_file.write((char *)&index, sizeof(Index));
    memory_accesses++;

    Bucket bucket = Bucket(0, -1, d);
    write_bucket(bucket, pos_log);

    return index;
}

// Reads an index by its logical position
Index extendible_hash::read_index(int pos_log)
{
    int seekpos = pos_log * (sizeof(Index));
    Index index;
    index_file.seekg(seekpos, ios::beg);
    index_file.read((char *)&index, sizeof(Index));
    memory_accesses++;
    return index;
}

// Numbers of bits in data.dat file
int extendible_hash::data_size()
{
    data_file.seekg(0, ios::end);
    return data_file.tellg();
}

// Writes a bucket in data.dat
void extendible_hash::write_bucket(Bucket &bucket, int pos_log)
{
    data_file.seekp(pos_log * (sizeof(Bucket)), ios::beg);
    data_file.write((char *)&bucket, sizeof(Bucket));
    memory_accesses++;
}

// Reads a bucket by its logical position
Bucket extendible_hash::read_bucket(int pos_log)
{
    int seekpos = pos_log * (sizeof(Bucket));
    Bucket bucket = Bucket();
    data_file.seekg(seekpos, ios::beg);
    data_file.read((char *)&bucket, sizeof(Bucket));
    memory_accesses++;
    return bucket;
}

// Adds a record into a bucket. (It happens just if size<fb)
// void extendible_hash::add_to_bucket(Bucket &bucket, int pos_log, Record record){
void extendible_hash::add_to_bucket(Index &index, Bucket &bucket, Record_h record)
{
    bucket.records[bucket.size] = record;
    bucket.size++;
    write_bucket(bucket, index.pos_log);
}

// Reads a bucket and overwrite it increasing the depth
// void extendible_hash::increment_depth(int pos_log)
int extendible_hash::increment_depth(int key)
{
    Bucket bucket;
    data_file.seekg(hash_table[key] * (sizeof(Bucket)), ios::beg);
    data_file.read((char *)&bucket, sizeof(Bucket));
    memory_accesses++;
    bucket.d++;
    data_file.seekp(hash_table[key] * (sizeof(Bucket)), ios::beg);
    data_file.write((char *)&bucket, sizeof(Bucket));
    memory_accesses++;
    return bucket.d;
}

// Redistributes the records from the resized bucket
void extendible_hash::resize(Index &index, Bucket &bucket, Index &new_index, Bucket &new_bucket, int d)
{
    int bucket_size = 0, new_bucket_size = 0;
    // Redistributes the records from the resized bucket
    for (int i = 0; i < bucket.size; i++)
    {
        Record_h record = bucket.records[i];
        int key = hash_function(record.id);
        // if the bit is 1 in d position the record goes to new_bucket
        if (key & (1 << (d - 1)))
        {
            new_bucket.records[new_bucket_size] = record;
            new_bucket_size++;
        }
        // if the bit is 0 in d position the record goes to bucket
        else
        {
            bucket.records[bucket_size] = record;
            bucket_size++;
        }
    }

    bucket.size = bucket_size;
    new_bucket.size = new_bucket_size;
    write_bucket(bucket, index.pos_log);
    write_bucket(new_bucket, new_index.pos_log);
}

// Overflow cases
// Case 1: d < D -> split
// Case 2: d == D  -> chaining
// Case 2.1: (d == D) && (size < fb)
// Case 2.2: (d == D) && (size == fb)

// void extendible_hash::overflow_handle(Index &index, Bucket &bucket, Record &record, int key)
void extendible_hash::overflow_handle(Index &index, Bucket &bucket, Record_h &record)
{
    // Case 1
    if (bucket.d < D)
    {
        bucket.d = increment_depth(index.key);
        // Initizalizes a new bucket
        int new_index_key = index.key + (1 << (bucket.d - 1));
        Index new_index = start_index(new_index_key, bucket.d);
        Bucket new_bucket = read_bucket(new_index.pos_log);
        resize(index, bucket, new_index, new_bucket, bucket.d);

        // Once we divided the bucket we try to insert the record again
        int key = hash_function(record.id);
        // new_bucket
        if (key & (1 << bucket.d))
        {
            if (new_bucket.size < fb)
                add_to_bucket(new_index, new_bucket, record);
            else
                overflow_handle(new_index, new_bucket, record);
        }
        // bucket
        else
        {
            if (bucket.size < fb)
                add_to_bucket(index, bucket, record);
            else
                overflow_handle(index, bucket, record);
        }
    }

    // Case 2
    else
    {
        int buck_pos_log = index.pos_log;
        // Loop until the last chained bucket (if exist) or find a bucket with size < fb
        while (bucket.next != -1)
        {
            if (bucket.size < fb)
            {
                break;
                break;
            }
            buck_pos_log = bucket.next;
            bucket = read_bucket(buck_pos_log);
        }
        // Case 2.1 | Case 2.2
        if (bucket.size == fb)
        {
            // Update the current bucket
            bucket.next = data_size() / sizeof(Bucket);
            write_bucket(bucket, buck_pos_log);

            // Create the new bucket and points to it
            buck_pos_log = bucket.next;
            bucket = Bucket(0, -1, D);
        }
        // Insert the record to bucket
        bucket.records[bucket.size] = record;
        bucket.size++;
        write_bucket(bucket, buck_pos_log);
    }
}

// extendible_hash constructor
extendible_hash::extendible_hash(int D, string index_filename, string data_filename)
{
    this->D = D;
    this->index_file.open(index_filename, ios::in | ios::out | ios::binary);
    this->data_file.open(data_filename, ios::in | ios::out | ios::binary);
    // Fill the hash_table
    if (index_empty())
    {
        start_index(0, 1);
        start_index(1, 1);
    }
    else
    {
        update_hash_table();
    }
}

// Limit the record_key by D
// record_key must be an integer
int extendible_hash::hash_function(int record_key)
{
    return record_key % (1 << D);
}

// Fill the hash_table with the indexes in index.dat
void extendible_hash::update_hash_table()
{
    // Go through the index.dat file and insert each index into the hash_map
    Index index;
    int cur_index = 0;
    int n_index = index_size() / sizeof(Index);
    while (cur_index < n_index)
    {
        index = read_index(cur_index);
        hash_table[index.key] = index.pos_log;
        cur_index++;
    }
}

// Searches a key in the hash table and returns an index associated to it (if exist)
Index extendible_hash::search_index(int key)
{
    Index index;
    // Index found
    if (hash_table.find(key) != hash_table.end())
    {
        index.key = key;
        index.pos_log = hash_table[key];
        return index;
    }
    // Index not found
    else
    {
        index.key = -1;
        // index.pos_log = -1;
        return index;
    }
}
// Iterates a key seeking for an equal key in hash_map
Index extendible_hash::iterate_key(int key)
{
    Index index;
    int search_key = key & ((1 << D) - 1);

    cout << "\nsearch_key: " << search_key << "\n";
    index = search_index(search_key);
    if (index.key == search_key)
        return index;

    for (int i = D - 1; i >= 0; i--)
    {
        if (key & (1 << i))
        {
            search_key ^= 1 << i;

            cout << "\nsearch_key: " << search_key << "\n";
            index = search_index(search_key);
            if (index.key == search_key)
                break;
        }
    }
    return index;
}

// Case 1: size < fb
// Case 2: size == fb

// Insert record
void extendible_hash::insert_record(Record_h record)
{
    // Search if a record with id == record.id has been inserted
    Record_h find_record;
    if (search_record(record.id, find_record))
    {
        cerr << "Record " << record.id << " wasn't inserted. It already exists" << endl;
    }
    else
    {
        int key = hash_function(record.id);
        Index index = iterate_key(key);
        Bucket bucket = read_bucket(index.pos_log);

        // Case 1
        if (bucket.size < fb)
        {
            add_to_bucket(index, bucket, record);
        }
        // Case 2
        else
        {
            overflow_handle(index, bucket, record);
        }
    }
}

// Search record
bool extendible_hash::search_record(int record_key, Record_h &record)
{
    int key = hash_function(record_key);
    Index index = iterate_key(key);
    Bucket bucket = read_bucket(index.pos_log);

    // main bucket
    for (int i = 0; i < bucket.size; i++)
    {
        record = bucket.records[i];
        if (record.id == record_key)
            return true;
    }

    // chained buckets
    while (bucket.next != -1)
    {
        bucket = read_bucket(bucket.next);
        for (int i = 0; i < bucket.size; i++)
        {
            record = bucket.records[i];
            if (record.id == record_key)
                return true;
        }
    }
    cerr << "Record " << record_key << " wasn't found" << endl;
    return false;
}

// Delete record
bool extendible_hash::delete_record(int record_key)
{
    Record_h record;

    int key = hash_function(record_key);
    Index index = iterate_key(key);
    int buck_pos_log = index.pos_log;
    Bucket bucket = read_bucket(buck_pos_log);

    // main bucket
    for (int i = 0; i < bucket.size; i++)
    {
        record = bucket.records[i];
        if (record.id == record_key)
        {
            // Overwrite the record to delete with the next continuous record
            for (int j = i + 1; j < bucket.size; j++)
            {
                bucket.records[j - 1] = bucket.records[j];
            }
            bucket.size--;
            write_bucket(bucket, buck_pos_log);
            return true;
        }
    }
    // chained buckets
    while (bucket.next != -1)
    {
        buck_pos_log = bucket.next;
        bucket = read_bucket(buck_pos_log);
        for (int i = 0; i < bucket.size; i++)
        {
            record = bucket.records[i];
            if (record.id == record_key)
            {
                bucket.records[i] = bucket.records[bucket.size - 1];
                bucket.size--;
                write_bucket(bucket, buck_pos_log);
                return true;
            }
        }
    }
    cerr << "Record " << record_key << " wasn't found";
    return false;
}

// Print all indexes with his associated buckets and chained buckets
void extendible_hash ::print_all()
{
    for (int i = 0; i < index_size() / sizeof(Index); i++)
    {
        Index index = read_index(i);
        cout << "(" << index.key << ", " << index.pos_log << ")";

        Bucket bucket = read_bucket(index.pos_log);
        cout << " | d: " << bucket.d << " | ";
        for (int j = 0; j < bucket.size; j++)
            cout << "[" << bucket.records[j].id << "]";
        while (bucket.next != -1)
        {
            cout << "-->";
            bucket = read_bucket(bucket.next);
            for (int j = 0; j < bucket.size; j++)
                cout << "[" << bucket.records[j].id << "]";
        }
        cout << endl;
    }
}