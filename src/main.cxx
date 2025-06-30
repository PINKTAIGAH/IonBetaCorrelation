#include<iostream>
#include "IonBeta.hh"

int main (int argc, const char** argv){

  if ( argc != 3 ){
    std::cerr << "Program call must follow the following structure: ./ionbeta <Input File> <Output File> " << std::endl;
    return 1;
  }

  // Call IonBeta Function (Main action function)
  IonBeta(argv[1], argv[2]);

  return 0;
}