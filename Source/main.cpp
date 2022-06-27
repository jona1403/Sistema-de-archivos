#include <stdio.h>
#include "../Headers/VariablesGlobales.h"
#include "../Headers/analizador.h"
#include <cstring>
#include <string>
#include <iostream>
int main()
{
  /*
  Analizador();*/
  string comando;
  FILE *file;
  cout << "*******************************************" << endl;
  cout << "************     TERMINAL     *************" << endl;
  cout << "*******************************************" << endl;

  Analizador *an = new Analizador();
  char linea[100];
  /*string a = "mkdisk $path=>sdkjfhksdf $size=>50";
  strcpy(linea, a.c_str());
  an->analizar(linea);*/
  //an->split(an->leer("Archivo.txt"), '\n');
  while (comando != "exit")
  {
    comando = "";
    cout << "Ingrese el comando: ";
    getline(cin, comando);

    strcpy(linea, comando.c_str());
    an->analizar(linea);

  }

  /*while (comando != "exit")
  {
    comando = "";
    cout << "Ingrese el comando: ";
    getline(cin, comando);

    file = fopen("Archivo.txt", "w");
    fprintf(file, comando.c_str());
    fclose(file);

    Analizador_driver driver;
    if (driver.parse("Archivo.txt"))
    {
      printf("La entrada es incorrecta\n");
    }
    else
    {
      printf("La entrada es correcta\n");
    }
  }*/

  return 0;
}