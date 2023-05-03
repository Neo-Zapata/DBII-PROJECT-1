//
// Created by milagros on 4/28/23.
//

#ifndef RECORDLECTURE_SETAVLFILE_H
#define RECORDLECTURE_SETAVLFILE_H

#include "avlfile.h"

istream & operator >> (istream & stream, AVLRecord & p){
    string campo;
    getline(stream, campo,',');
    p.anime_id = stoi(campo);

    getline(stream, campo,',');
    strncpy(p.name, campo.c_str(), NAME_SIZE -1);
    p.name[NAME_SIZE - 1] = '\0';

    getline(stream, campo,',');
    strncpy(p.type, campo.c_str(), TYPE_SIZE -1);
    p.type[TYPE_SIZE - 1] = '\0';

    getline(stream, campo,',');
    p.episodes = stoi(campo);

    getline(stream, campo, ',');
    p.rating = atof(campo.c_str());

    getline(stream, campo);
    p.members = stoi(campo);

    return stream;
}

ostream & operator << (ostream & stream, AVLRecord & p)
{
    stream << p.anime_id << " | ";
    stream << p.name << " | ";
    stream << p.type << " | ";
    stream << p.rating << " | ";
    stream << p.members;
    stream << "\n";
    stream << flush;
    return stream;
}


//Crear un archivo para el uso del AVL (con nodes) a partir de los Record del dataset
void read_write_dataset(string filename, string avlfilename, AVLFile& avl_file){
    fstream file(filename, ios::in | ios::out);
   // create_file_if_doesnt_exist(avlfilename);
    fstream avlfile(avlfilename, ios::in | ios::out | ios::app | ios::binary);

    if (file.fail()) file_not_found("No se pudo abrir el archivo");
    if (avlfile.fail()) file_not_found("No se pudo abrir el archivo");

    // Colocar el nombre del archivo de AVL
    avl_file.set_filename(avlfilename);

    // Nos ubicamos en la primera posicion del csv    
    avlfile.seekg(0, ios::end);
   
   // tellg:  posicion actual
    if (avlfile.tellg() == 0){
        
        AVLRecord anime{};
        string line;
        int cont = 0;
        getline(file, line); // Leemos toda la linea
        while (getline(file, line)){
            cout<<endl<<"| ------------- " << ++cont << " ------------- |"<<endl;
            cout<<endl<<"\nEntra: -> ";
            stringstream stream(line);
            stream >> anime;
            cout<<anime;
            avl_file.insert(anime);// Insertamos al AVL si es que existe
        }
        cout<<"root final: "; avl_file.getDates();
        cout<<"\nFin de Lectura\n";

    }
    else ayudame_Dios("El archivo ya ha sido llenado inicialmente");
    file.close();

   /*  // BUSCAR
    cout<<endl<<"| ------------- SEARCH ------------- |"<<endl;
    avl_file.find(33876);

    cout<<endl<<"| ------------- RANGE SEARCH ------------- |"<<endl;
    vector<Record> results = avl_file.rangeSearch(33874 , 33876);
    for (auto record:results) {
        cout<<record;
    }
 */
    /* cout<<endl<<"| ------------- REMOVE ------------- |"<<endl;
    avl_file.remove(33872); */
    
}


void read_binary_avl(){

    // Leer los registros del archivo
    AVLFile::NodeBT registro;
    fstream archivo("animedata_avl.dat", ios::in | ios::binary);
    if (!archivo) {
        cout << "Error al abrir el archivo" << endl;
    }
    int cont = 0;
   /*  while (archivo.read((char*) &registro, sizeof(registro))) {        
        cout <<"("<<cont++ <<")"<< registro.anime_id << " | " << registro.name << " | " << registro.type << " | " << registro.episodes << " | " << registro.rating << " | " << registro.members << endl;
    } */

    
    while (archivo.read((char*) &registro, sizeof(registro))) {
        //cout << registro.data.anime_id << " | " << registro.data.name << " | " << registro.data.type << " | " << registro.data.episodes << " | " << registro.data.rating << " | " << registro.data.members << " | " <<registro.left << " | " << registro.right << " | " << registro.height << " | " << registro.pos<< endl;
        cout << registro.data.anime_id << " | " << registro.data.name <<" | " <<registro.left << " | " << registro.right << " | " << registro.height << " | " << registro.pos<< endl;
    }
    
    // Cerrar el archivo
    archivo.close();

}

#endif //RECORDLECTURE_SETAVLFILE_H

