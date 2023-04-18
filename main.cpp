#include "sequential.h"

int main(){
    try {
        SequentialFile SF("test.csv");
        SF.load();
    } catch (const exception& e){
        cerr << "Error - " << e.what() << endl;
    }
}