#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <cstring>

#include "./extendible_hash.cpp"
using namespace std;

void read_dataset(string filename, extendible_hash &hash_structure)
{
    fstream fio;

    fio.open(filename, ios::in | ios::out | ios::binary);
    string line;

    getline(fio, line);
    while (getline(fio, line))
    {
        string str;
        stringstream ss(line);
        vector<string> vec;

        while (getline(ss, str, ','))
        {
            vec.push_back(str);
        }

        Record_h record;

        int k = 0;
        int m = 0;

        while (m < vec.size())
        {
            if (k == 0)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }
                record.id = stoi(vec[m]);
                m++;
                k++;
            }
            else if (k == 1)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }

                string temp;
                if (vec[m][0] == '\"')
                {
                    temp = vec[m];
                    temp += ",";
                    m++;

                    while (vec[m][vec[m].size() - 1] != '\"')
                    {
                        temp += vec[m];
                        temp += ",";
                        m++;
                    }
                    if (vec[m][vec[m].size() - 1] == '\"')
                    {
                        temp += vec[m];
                        m++;
                    }

                    strcpy(record.name, temp.c_str());
                    k++;
                }
                else
                {
                    strcpy(record.name, vec[m].c_str());
                    m++;
                    k++;
                }
            }
            else if (k == 2)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }

                string temp;
                if (vec[m][0] == '\"')
                {
                    temp = vec[m];
                    temp += ",";
                    m++;

                    while (vec[m][vec[m].size() - 1] != '\"')
                    {
                        temp += vec[m];
                        temp += ";";
                        m++;
                    }
                    if (vec[m][0] == '\"')
                    {
                        temp += vec[m];
                        m++;
                    }

                    strcpy(record.type, temp.c_str());
                    k++;
                }
                else
                {
                    strcpy(record.type, vec[m].c_str());
                    m++;
                    k++;
                }
            }
            else if (k == 3)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }

                string temp;
                if (vec[m][0] == '\"')
                {
                    temp = vec[m];
                    temp += ",";
                    m++;

                    while (vec[m][vec[m].size() - 1] != '\"')
                    {
                        temp += vec[m];
                        temp += ",";
                        m++;
                    }
                    if (vec[m][0] == '\"')
                    {
                        temp += vec[m];
                        m++;
                    }

                    strcpy(record.episodes, temp.c_str());
                    k++;
                }
                else
                {
                    strcpy(record.episodes, vec[m].c_str());
                    m++;
                    k++;
                }
            }
            else if (k == 4)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }

                string temp;
                if (vec[m][0] == '\"')
                {
                    temp = vec[m];
                    temp += ",";
                    m++;

                    while (vec[m][vec[m].size() - 1] != '\"')
                    {
                        temp += vec[m];
                        temp += ",";
                        m++;
                    }
                    if (vec[m][0] == '\"')
                    {
                        temp += vec[m];
                        m++;
                    }

                    strcpy(record.rating, temp.c_str());
                    k++;
                }
                else
                {
                    strcpy(record.rating, vec[m].c_str());
                    m++;
                    k++;
                }
            }
            else if (k == 5)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }

                string temp;
                if (vec[m][0] == '\"')
                {
                    temp = vec[m];
                    temp += ",";
                    m++;

                    while (vec[m][vec[m].size() - 1] != '\"')
                    {
                        temp += vec[m];
                        temp += ",";
                        m++;
                    }
                    if (vec[m][0] == '\"')
                    {
                        temp += vec[m];
                        temp += ",";
                        m++;
                    }

                    strcpy(record.members, temp.c_str());
                    k++;
                }
                else
                {
                    strcpy(record.members, vec[m].c_str());
                    m++;
                    k++;
                }
            }
        }
        hash_structure.insert_record(record);
    }
}

void read_dataset_until(string filename, extendible_hash &hash_structure, int i)
{
    int cont = 0;
    fstream fio;

    fio.open(filename, ios::in | ios::out | ios::binary);
    string line;

    getline(fio, line);
    while (getline(fio, line))
    {
        string str;
        stringstream ss(line);
        vector<string> vec;

        while (getline(ss, str, ','))
        {
            vec.push_back(str);
        }

        Record_h record;

        int k = 0;
        int m = 0;

        while (m < vec.size())
        {
            if (k == 0)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }
                record.id = stoi(vec[m]);
                m++;
                k++;
            }
            else if (k == 1)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }

                string temp;
                if (vec[m][0] == '\"')
                {
                    temp = vec[m];
                    temp += ",";
                    m++;

                    while (vec[m][vec[m].size() - 1] != '\"')
                    {
                        temp += vec[m];
                        temp += ",";
                        m++;
                    }
                    if (vec[m][vec[m].size() - 1] == '\"')
                    {
                        temp += vec[m];
                        m++;
                    }

                    strcpy(record.name, temp.c_str());
                    k++;
                }
                else
                {
                    strcpy(record.name, vec[m].c_str());
                    m++;
                    k++;
                }
            }
            else if (k == 2)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }

                string temp;
                if (vec[m][0] == '\"')
                {
                    temp = vec[m];
                    temp += ",";
                    m++;

                    while (vec[m][vec[m].size() - 1] != '\"')
                    {
                        temp += vec[m];
                        temp += ";";
                        m++;
                    }
                    if (vec[m][0] == '\"')
                    {
                        temp += vec[m];
                        m++;
                    }

                    strcpy(record.type, temp.c_str());
                    k++;
                }
                else
                {
                    strcpy(record.type, vec[m].c_str());
                    m++;
                    k++;
                }
            }
            else if (k == 3)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }

                string temp;
                if (vec[m][0] == '\"')
                {
                    temp = vec[m];
                    temp += ",";
                    m++;

                    while (vec[m][vec[m].size() - 1] != '\"')
                    {
                        temp += vec[m];
                        temp += ",";
                        m++;
                    }
                    if (vec[m][0] == '\"')
                    {
                        temp += vec[m];
                        m++;
                    }

                    strcpy(record.episodes, temp.c_str());
                    k++;
                }
                else
                {
                    strcpy(record.episodes, vec[m].c_str());
                    m++;
                    k++;
                }
            }
            else if (k == 4)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }

                string temp;
                if (vec[m][0] == '\"')
                {
                    temp = vec[m];
                    temp += ",";
                    m++;

                    while (vec[m][vec[m].size() - 1] != '\"')
                    {
                        temp += vec[m];
                        temp += ",";
                        m++;
                    }
                    if (vec[m][0] == '\"')
                    {
                        temp += vec[m];
                        m++;
                    }

                    strcpy(record.rating, temp.c_str());
                    k++;
                }
                else
                {
                    strcpy(record.rating, vec[m].c_str());
                    m++;
                    k++;
                }
            }
            else if (k == 5)
            {
                if (vec[m].size() == 0)
                {
                    m++;
                    k++;
                    continue;
                }

                string temp;
                if (vec[m][0] == '\"')
                {
                    temp = vec[m];
                    temp += ",";
                    m++;

                    while (vec[m][vec[m].size() - 1] != '\"')
                    {
                        temp += vec[m];
                        temp += ",";
                        m++;
                    }
                    if (vec[m][0] == '\"')
                    {
                        temp += vec[m];
                        temp += ",";
                        m++;
                    }

                    strcpy(record.members, temp.c_str());
                    k++;
                }
                else
                {
                    strcpy(record.members, vec[m].c_str());
                    m++;
                    k++;
                }
            }
        }
        if (cont < i)
        {
            hash_structure.insert_record(record);
            cont++;
        }
        else
        {
            return;
        }
    }
}