//
// Created by milagros on 4/28/23.
//

#ifndef RECORDLECTURE_AVLFILE_H
#define RECORDLECTURE_AVLFILE_H

#include <fstream>
#include <ostream>
#include <vector>
#include <cstring>
#include "libs.h"
#include "AVLRecord.h"
using namespace std;

class AVLFile {
public:
    struct NodeBT {
        AVLRecord data;
        long left;
        long right;
        long height;
        long pos ;
        NodeBT() {
            left = right = -1;
            height = 0;
        }
        NodeBT(AVLRecord &AVLRecord) {
            this->data = AVLRecord;
            left = right = -1;
            height = 0;
        }
        void print_node(){
            cout<<"Insertar -> ";
            cout << data.anime_id << " | " << data.name << " | ";
            //cout<< "L("<<left << ") | R(" << right << ") | H(" << height << ") | P("<<pos<<") | "<<endl;
        }
       /*  void killSelf(){
            if(left != -1) left->killSelf();
            if(right != -1) right->killSelf();
            delete this;
        }  */
    };

private:
    long root;
    int disk_accesses_search;
    int disk_accesses_insert;
    int disk_accesses_remove;
    std::string filename;

public:
    AVLFile() : root(-1) {};
    AVLFile(string filename){
        this->filename = filename;
        this->disk_accesses_search = 0;
        this->disk_accesses_insert = 0;
        this->disk_accesses_remove = 0;
        this-> root = -1;
    }
    string getFileName() {
        return this->filename;
    }
    void getDates(){
        cout<<root<<endl;
    }

    int getDiskAccessSearch(){
        return this->disk_accesses_search;
    }
    int getDiskAccessInsert(){
        return this->disk_accesses_insert;
    }
    int getDiskAccessRemove(){
        return this->disk_accesses_remove;
    }

    void set_filename (string filename){ this->filename = filename;}

    AVLRecord find(int animeid) {
        cout<<"------ Search ------"<<endl;
        this->disk_accesses_search = 0;
        std::ifstream file(this->filename, std::ios::binary);
        
        cout<<"root que pasa: "<<this->root<<endl;
        AVLRecord result = find(file, this->root, animeid); // El root puede cambiar o solo es -1 cuando no tiene valores el archivo?
        result.get_data();
        file.close();
        return result;
    }

    void insert(AVLRecord& record) {
        cout<<"------ Insert ------"<<endl;
        this->disk_accesses_insert = 0;
        std::fstream file(this->filename, std::ios::in|std::ios::out|std::ios::binary );
        //insert(file, this->root, record , true);
        insert(file, this->root, record );
        file.close();
    }

    void remove(int anime_id) {
        cout<<"------ Remove ------"<<endl;
        this->disk_accesses_remove = 0;
        std::fstream file(this->filename, std::ios::in|std::ios::out|std::ios::binary );
        remove(file, this -> root, anime_id);
        file.close();
    }


   /*  Record search (Record key){
        std::ifstream file(this->filename, std::ios::binary);
        Record result = search(file, this->root, key.anime_id);
        file.close();
        return result;
    } */

    vector<AVLRecord> rangeSearch(int begin_key, int end_key) {
        cout<<"------ Range Search ------"<<endl;

        vector<AVLRecord> results;
        fstream file(filename, ios::in | ios::binary);
        if (file) {
            rangeSearch(file, root, begin_key, end_key, results);
            file.close();
        } else {
            cout << "Error al abrir el archivo " << filename << endl;
        }
        return results;
    }

    vector<AVLRecord> rangeSearchRating(float begin_key, float end_key) {
        cout<<"------ Range Search Rating ------"<<endl;

        vector<AVLRecord> results;
        fstream file(filename, ios::in | ios::binary);
        if (file) {
            rangeSearchRating(file, root, begin_key, end_key, results);
            file.close();
        } else {
            cout << "Error al abrir el archivo " << filename << endl;
        }
        return results;
    }

    void printPreorder(){
        cout<<"------ Search Preorder ------"<<endl;
        fstream file(filename, ios::in | ios::binary);
        getPreorder(file, root); // Donde "root" es la posición del nodo raíz del árbol.
        file.close();
    }

    void printInorder() {
        cout<<"------ Search Inorder ------"<<endl;
        ifstream file(filename, ios::in | ios::binary);
        getInorder(file, root); // Donde "root" es la posición del nodo raíz del árbol.
        file.close();
    }

    
private:
    // FUNCTIONS TO USE

    void rangeSearch(fstream &file, long node, int begin_key, int end_key, vector<AVLRecord> &results);

    AVLRecord search(std::fstream &file, long& record_pos, int key);
    
    void insert(std::fstream& file, long& root, AVLRecord& record);
    
    long height (std::fstream& file , long record_pos );

    long calculate_balance_factor(std::fstream& file , NodeBT node );

    void balance( std::fstream &file , long &node_pos);

    long maxValue(long node_pos, std::fstream &file);

    void remove(std::fstream& file, long& record_pos, int value);

    AVLRecord find(std::ifstream &file, long record_pos, int value);

    void rotateLeft(std::fstream& file, long& node) ;

    void rotateRight(std::fstream& file, long& node);

    void writeNode(std::fstream& file, long pos, const NodeBT& node);

    AVLFile::NodeBT readNode(std::fstream& file, long pos);

    AVLFile::NodeBT find_predecessor(std::fstream& file, long& record_pos, NodeBT& predecessor);
    
    AVLFile::NodeBT find_max(std::fstream& file, long record_pos);

    void getPreorder(std::fstream &file, long node);

    void getInorder(std::ifstream& file, long node);

    void rangeSearchRating(fstream &file, long node, float begin_key, float end_key, vector<AVLRecord> &results);

};

// ------------------ IMPLEMENTATIONS ------------------

AVLRecord AVLFile::find(std::ifstream &file, long record_pos, int value){
    if (record_pos == -1) {
        // El archivo está vacío o no se encontró el valor buscado
        cout<<"No se encontro el valor"<<endl;
        return AVLRecord(); // Retorna un registro vacío
    }
    NodeBT temp;
    file.seekg(record_pos ); // Nos ubicamos en la posición actual
    file.read((char*)&temp, sizeof(NodeBT));
    this->disk_accesses_search++;    

    if (value < temp.data.anime_id)
        return find(file, temp.left, value);
    else if (value > temp.data.anime_id)
        return find(file, temp.right, value);
    else
        return temp.data;
}

void AVLFile::insert(std::fstream& file, long& root, AVLRecord& record) {
    if (root == -1) {
        // El árbol está vacío, creamos un nuevo nodo
        file.seekp(0, std::ios_base::end);
        root = file.tellp();
        NodeBT newNode(record);
        newNode.pos = root;
        file.write((char*)&newNode, sizeof(NodeBT));
        this->disk_accesses_insert++;
        return;
    }
    // Buscamos la posición adecuada para insertar el nuevo nodo
    long node_pos = root;
    while (true) {
        NodeBT node;
        file.seekg(node_pos);
        file.read((char*)&node, sizeof(NodeBT));
        this->disk_accesses_insert++;
        if (record.anime_id == node.data.anime_id) {
            std::cout << "El registro ya existe en el árbol" << std::endl;
            return;
        } else if (record.anime_id < node.data.anime_id) {
            if (node.left == -1) {
                // Creamos un nuevo nodo y lo enlazamos a la izquierda del nodo actual
                file.seekp(0, std::ios_base::end);
                long new_node_pos = file.tellp();
                NodeBT new_node(record);
                new_node.pos = new_node_pos;
                node.left = new_node_pos;
                file.seekp(node_pos);
                file.write((char*)&node, sizeof(NodeBT));
                file.write((char*)&new_node, sizeof(NodeBT));
                this->disk_accesses_insert+= 2;
                break;
            } else {
                node_pos = node.left;
            }
        } else {
            if (node.right == -1) {
                // Creamos un nuevo nodo y lo enlazamos a la derecha del nodo actual
                file.seekp(0, std::ios_base::end);
                long new_node_pos = file.tellp();
                NodeBT new_node(record);
                new_node.pos = new_node_pos;
                node.right = new_node_pos;
                file.seekp(node_pos);
                file.write((char*)&node, sizeof(NodeBT));
                file.write((char*)&new_node, sizeof(NodeBT));
                this->disk_accesses_insert+= 2;
                break;
            } else {
                node_pos = node.right;
            }
        }
    }
    // Rebalanceamos el árbol
    balance(file, root);
}


long AVLFile::height(std::fstream& file, long node) {

    if (node == -1) {
        return 0; // Si el nodo es nulo, su altura es 0
    } else {
        NodeBT currentNode;
        file.seekg(node , ios::beg);
        file.read((char*)&currentNode, sizeof(NodeBT));
        this->disk_accesses_insert++;
        this->disk_accesses_remove++;
        //cout<<"height: "<<currentNode.height<<endl;
        return currentNode.height; 
    }
}

void AVLFile::remove(std::fstream& file, long& record_pos, int value) {
    
    if (record_pos == -1) {
        std::cout << "El registro no se encuentra en el árbol" << std::endl;
        return;
    }
    NodeBT current_node;
    file.seekg(record_pos, std::ios::beg);
    file.read((char*)&current_node, sizeof(NodeBT));
    this->disk_accesses_remove++;

    //current_node.print_node();
    if (value < current_node.data.anime_id) {
        remove(file, current_node.left, value);
    } else if (value > current_node.data.anime_id) {
        remove(file, current_node.right, value);
    } else {
        // Caso en el que se encuentra el valor a eliminar
        if (current_node.left == -1 && current_node.right == -1) {
            // Caso 1: El nodo a eliminar es una hoja
            record_pos = -1;
            balance(file, record_pos);
            file.seekp(0, std::ios::beg);
            file.write((char*)&root, sizeof(long));
            this->disk_accesses_remove++;
        } else if (current_node.left == -1 || current_node.right == -1) {
            // Caso 2: El nodo a eliminar tiene un solo hijo
            record_pos = (current_node.left != -1) ? current_node.left : current_node.right;
            balance(file, record_pos);
            file.seekp(0, std::ios::beg);
            file.write((char*)&root, sizeof(long));
            this->disk_accesses_remove++;
        } else {
            // Caso 3: El nodo a eliminar tiene dos hijos
            NodeBT predecessor;
            find_predecessor(file, current_node.left, predecessor);
            current_node.data = predecessor.data;
            remove(file, current_node.left, predecessor.data.anime_id);
            file.seekp(record_pos, std::ios::beg);
            file.write((char*)&current_node, sizeof(NodeBT));
            this->disk_accesses_remove++;
        }
    }
    // Actualizamos la altura del nodo actual
    current_node.height = std::max(height(file, current_node.left), height(file, current_node.right)) + 1;
    // Rebalanceamos el árbol si es necesario
    balance(file, record_pos);
    // Guardamos los cambios en el archivo
    file.seekp(record_pos, std::ios::beg);
    file.write((char*)&current_node, sizeof(NodeBT));
}

AVLFile::NodeBT AVLFile::find_predecessor(std::fstream& file, long& record_pos, NodeBT& predecessor) {
    NodeBT currentNode;
    file.seekg(record_pos); // Nos ubicamos en el nodo actual
    file.read((char*)&currentNode, sizeof(NodeBT));
    this->disk_accesses_remove++;

    // Si hay un subárbol izquierdo, el predecesor es el nodo más grande de ese subárbol
    if (currentNode.left != -1) {
        return find_max(file, currentNode.left);
    }

    // Si no hay un subárbol izquierdo, buscamos el primer ancestro que sea el hijo derecho de su padre
    // El predecesor es el padre de ese nodo
    while (true) {
        long parent_pos = currentNode.pos;
        if (parent_pos == -1) {
            // No hay predecesor
            return NodeBT();
        }
        NodeBT parent;
        file.seekg(parent_pos); // Nos ubicamos en el padre del nodo actual
        file.read((char*)&parent, sizeof(NodeBT));
        this->disk_accesses_remove++;
        if (parent.right == record_pos) {
            // El nodo actual es el hijo derecho del padre
            // El predecesor es el padre
            predecessor = parent;
            return parent;
        }
        // Si no es el hijo derecho, seguimos subiendo en el árbol
        currentNode = parent;
        record_pos = parent_pos;
    }
}

AVLFile::NodeBT AVLFile::find_max(std::fstream& file, long record_pos) {
    NodeBT currentNode;
    file.seekg(record_pos); // Nos ubicamos en el nodo actual
    file.read((char*)&currentNode, sizeof(NodeBT));
    this->disk_accesses_remove++;

    while (currentNode.right != -1) {
        // Seguimos bajando por el subárbol derecho
        file.seekg(currentNode.right);
        file.read((char*)&currentNode, sizeof(NodeBT));
        this->disk_accesses_remove++;
    }

    return currentNode;
}

void AVLFile::rangeSearch(fstream &file, long node, int begin_key, int end_key, vector<AVLRecord> &results) {
    if (node == -1) {
        return;
    }
    NodeBT current;
    file.seekg(node);
    file.read((char*)&current, sizeof(NodeBT));
    if (current.data.anime_id >= begin_key && current.data.anime_id <= end_key) {
        results.push_back(current.data);
    }
    if (current.data.anime_id > begin_key) {
        rangeSearch(file, current.left, begin_key, end_key, results);
    }
    if (current.data.anime_id < end_key) {
        rangeSearch(file, current.right, begin_key, end_key, results);
    }
}

void AVLFile::rangeSearchRating(fstream &file, long node, float begin_key, float end_key, vector<AVLRecord> &results) {
    if (node == -1) {
        return;
    }
    NodeBT current;
    file.seekg(node);
    file.read((char*)&current, sizeof(NodeBT));
    if (current.data.rating >= begin_key && current.data.rating <= end_key) {
        results.push_back(current.data);
    }
    if (current.data.rating > begin_key) {
        rangeSearchRating(file, current.left, begin_key, end_key, results);
    }
    if (current.data.rating < end_key) {
        rangeSearchRating(file, current.right, begin_key, end_key, results);
    }
}

/*
AVLRecord AVLFile::search(std::fstream &file, long& record_pos, int key) {
    if (record_pos == -1)
        throw "El archivo se encuentra vacío";

    NodeBT temp;
    file.seekg(record_pos);
    file.read((char*)&temp, sizeof(NodeBT));

    if (key == temp.data.anime_id)
        return temp.data;
    else if (key < temp.data.anime_id && temp.left != -1)
        return search(file, temp.left, key);
    else if (key > temp.data.anime_id && temp.right != -1)
        return search(file, temp.right, key);
    else
        throw "El elemento no fue encontrado";
}*/



long AVLFile::calculate_balance_factor(std::fstream& file , NodeBT node ){
    long balance_factor = height(file , node.left) - height(file , node.right);
    //cout<<"hl - hr: " << height(file , node.left) << " - " <<height(file , node.right) << endl;
    return balance_factor;
}

void AVLFile::balance(std::fstream& file, long& node) {
    
    NodeBT currentNode;
    file.seekg(node);
    file.read((char*)&currentNode, sizeof(NodeBT));
    this->disk_accesses_insert++;
    this->disk_accesses_remove++;

    // Obtenemos las alturas de los subárboles izquierdo y derecho
    long heightLeft = height(file, currentNode.left);
    long heightRight = height(file, currentNode.right);
    //cout<<"hl - hr: (" << heightLeft << ") - (" <<heightRight <<")"<< endl;

    // Calculamos el factor de balance del nodo actual
    long balanceFactor = heightRight - heightLeft;
    //cout<<"indice: " << balanceFactor<< endl;

    // Si el factor de balance está fuera de los límites (-1, 0, 1), el árbol está desequilibrado
    if (balanceFactor < -1 || balanceFactor > 1) {
        //cout<<"...Balanceamos..."<<endl;
        // Si el subárbol derecho es más alto, realizamos una rotación simple o doble hacia la izquierda
        if (balanceFactor > 0) {
            NodeBT rightNode;
            file.seekg(currentNode.right);
            file.read((char*)&rightNode, sizeof(NodeBT));
            this->disk_accesses_insert++;
            this->disk_accesses_remove++;

            long heightRightLeft = height(file, rightNode.left);
            long heightRightRight = height(file, rightNode.right);

            if (heightRightLeft > heightRightRight) {
                rotateRight(file, currentNode.right);
            }

            rotateLeft(file, node);

        // Si el subárbol izquierdo es más alto, realizamos una rotación simple o doble hacia la derecha
        } else {
            NodeBT leftNode;
            file.seekg(currentNode.left);
            file.read((char*)&leftNode, sizeof(NodeBT));
            this->disk_accesses_insert++;
            this->disk_accesses_remove++;

            long heightLeftLeft = height(file, leftNode.left);
            long heightLeftRight = height(file, leftNode.right);

            if (heightLeftRight > heightLeftLeft) {
                rotateLeft(file, currentNode.left);
            }

            rotateRight(file, node);
        }

        // Después de la rotación, actualizamos las alturas del nodo y sus hijos
        currentNode.height = 1 + std::max(height(file, currentNode.left), height(file, currentNode.right));
        this -> root = node;
        NodeBT leftChild, rightChild;
        file.seekg(currentNode.left);
        file.read((char*)&leftChild, sizeof(NodeBT));
        file.seekg(currentNode.right);
        file.read((char*)&rightChild, sizeof(NodeBT));

        leftChild.height = 1 + std::max(height(file, leftChild.left), height(file, leftChild.right));
        rightChild.height = 1 + std::max(height(file, rightChild.left), height(file, rightChild.right));

        file.seekp(currentNode.pos);
        file.write((char*)&currentNode, sizeof(NodeBT));
        file.seekp(leftChild.pos);
        file.write((char*)&leftChild, sizeof(NodeBT));
        file.seekp(rightChild.pos);
        file.write((char*)&rightChild, sizeof(NodeBT));
        this->disk_accesses_insert+= 5;
        this->disk_accesses_remove+= 5;
    }
}

void AVLFile::rotateLeft(std::fstream& file, long& node) {
    NodeBT tmpNode = readNode(file, node);
    NodeBT rightNode = readNode(file, tmpNode.right);
    
    tmpNode.right = rightNode.left;
    rightNode.left = node;

    tmpNode.height = 1 + std::max(height(file, tmpNode.left), height(file, tmpNode.right));
    rightNode.height = 1 + std::max(height(file, rightNode.left), height(file, rightNode.right));
    
    writeNode(file, node, rightNode);
    writeNode(file, tmpNode.pos, tmpNode);
    this->disk_accesses_insert=+4;
    this->disk_accesses_remove=+4;
    node = rightNode.pos;
}

void AVLFile::rotateRight(std::fstream& file, long& node) {
    NodeBT tmpNode = readNode(file, node);
    NodeBT leftNode = readNode(file, tmpNode.left);
    
    tmpNode.left = leftNode.right;
    leftNode.right = node;

    tmpNode.height = 1 + std::max(height(file, tmpNode.left), height(file, tmpNode.right));
    leftNode.height = 1 + std::max(height(file, leftNode.left), height(file, leftNode.right));
    
    writeNode(file, node, leftNode);
    writeNode(file, tmpNode.pos, tmpNode);
    this->disk_accesses_insert=+4;
    this->disk_accesses_remove=+4;
    node = leftNode.pos;
}

void AVLFile::writeNode(std::fstream& file, long pos, const NodeBT& node) {
    file.seekp(pos * sizeof(NodeBT));
    file.write(reinterpret_cast<const char*>(&node), sizeof(NodeBT));
}

AVLFile::NodeBT AVLFile::readNode(std::fstream& file, long pos) {
    NodeBT node;
    file.seekg(pos * sizeof(NodeBT));
    file.read(reinterpret_cast<char*>(&node), sizeof(NodeBT));
    node.pos = pos;
    return node;
}

void AVLFile::getPreorder(std::fstream &file, long node) {
    if (node != -1) {
        NodeBT currentNode;
        file.seekg(node , ios::beg);
        file.read((char*)&currentNode, sizeof(NodeBT));
        std::cout << currentNode.data.anime_id << "  -->  ";
        getPreorder(file, currentNode.left);
        getPreorder(file, currentNode.right);
    }
}

void AVLFile::getInorder(std::ifstream& file, long node) {
    if (node != -1) {
        NodeBT currentNode;
        file.seekg(node);
        file.read((char*)&currentNode, sizeof(NodeBT));

        getInorder(file, currentNode.left); // 2. Visita el subárbol izquierdo
        std::cout << currentNode.data.anime_id << " -->  "; // 3. Procesa el nodo actual
        getInorder(file, currentNode.right); // 4. Visita el subárbol derecho
    }
}

#endif //RECORDLECTURE_AVLFILE_H
