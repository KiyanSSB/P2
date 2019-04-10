#!/bin/bash

MAXTIME=10     # Tiempo máximo de ejecución (en segundos) de una prueba
PRINCIPAL=./prac2	# Nombre del ejecutable de la práctica
ENT=datos-entrada    # Directorio con los ficheros de pruebas de entrada
SAL=salida-correcta-esperada     # Directorio con los ficheros de salidas correctas
OBT=salida-obtenida       # Directorio con los ficheros obtenidos tras la ejecución de la práctica
VAL="valgrind -q"  # Si valgrind no está instalado, quitar "valgrind -q" de esta linea (quedaría VAL=""). Ojo: La práctica debe probarse siempre con valgrind antes de la entrega.
#npruebasa=4     # Número de pruebas a ejecutar del autocorrector
npruebasa=$(ls -1 datos-entrada/*.txt|wc -l)
mata=./mata
comparefiles=./comparefiles

mkdir -p $OBT

# -------------- generar y compilar los ficheros auxiliares mata.c y comparefiles.cc -----
function genMata() {

if ! test -x mata ; then  # si ya existe no se genera
echo "#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

const int TAMMAXFICH=300000;
int f;int segundos,st;
int segejec=0;
char *nfsal=NULL;
void SeAcabo(int i){
  fprintf(stderr,\"ERROR, tiempo de ejecución agotado... (%d segundos)\\n\",segundos);
  fflush(stderr);
  kill(f,SIGKILL);
  exit(-1);
}
void SeAcaboFich(int i){
  fprintf(stderr,\"ERROR, fichero de salida muy grande (cuelgue?)... \\n\");
  fflush(stderr);
  kill(f,SIGKILL);
  exit(-1);
}
int FicheroGrande() {
  struct stat s;  int ret=0;
  if (nfsal != NULL) {
    stat(nfsal,&s);
    if (s.st_size > TAMMAXFICH) ret = 1;
  }
  return ret;
}
void Control(int i){
  segejec++;
  if (segejec >= segundos) SeAcabo(i);
  else if (FicheroGrande()) SeAcaboFich(i);
  else alarm(1);
}
void Salir(int i){exit(1);}
int main(int argc,char *argv[]){
  if (argc < 4) exit(1);
  segundos = atoi(argv[1]);
  nfsal = argv[2];
  signal(SIGALRM,Control);
  signal(SIGCHLD,Salir);
  alarm(1);
  if ((f = fork()) == 0) execvp(argv[3],&argv[3]);
  else wait(&st);
}
" > mata.c
gcc -o mata mata.c
fi
}

function genComparefiles() {

if ! test -x comparefiles ; then # si ya existe no se genera
echo "#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
  int salida=0;

  if (argc!=3) cout << \"Sintaxis: \" << argv[0] << \" <obtenido.txt> <correcto.txt>\" << endl;
  else
  {
    ifstream fo(argv[1]);
    ifstream fc(argv[2]);
    if (fo.is_open() && fc.is_open())
    {
      string so,sc,tmp;

      fo >> tmp;
      while (!fo.eof() || tmp.length()!=0)
      {
        so=so+tmp;
        tmp=\"\";
        fo >> tmp;
      }
      fc >> tmp;
      while (!fc.eof() || tmp.length()!=0)
      {
        sc=sc+tmp;
        tmp=\"\";
        fc >> tmp;
      }

      // ignorar los '-' en las líneas
      string scok, sook;
      for (unsigned int i=0; i<so.length(); i++)
        if (so[i]!='-') sook=sook+so[i];
      for (unsigned int i=0; i<sc.length(); i++)
        if (sc[i]!='-') scok=scok+sc[i];

      if (sook!=scok) {
        exit(-1);
      }
    }
    else {
      cout << \"Fichero \" << argv[1] << \" o \" << argv[2] << \" no encontrado\" << endl;
      exit(-1);
    }
  }
  exit(0);
  return salida;
}
" > comparefiles.cc

g++ -o comparefiles comparefiles.cc

fi
}

echo "*********************************************************"
echo "Autocorrector P2p2"


# Comprobar si está valgrind instalado
hayValgrind=$(which valgrind)
if test "$hayValgrind" == "" ; then
  echo "AVISO: El autocorrector se ejecutará sin valgrind, por lo que es posible que"
  echo "el resultado de la corrección no sea fiable. Para comprobar realmente que la"
  echo "práctica es correcta, debes probar el autocorrector en un ordenador Linux"
  echo "con valgrind instalado antes de la entrega."
  echo
  read -p "Pulsa [Enter] para continuar"
  VAL=""
fi


echo " Generando ficheros auxiliares... "
genMata
genComparefiles
rm -f mata.c
rm -f comparefiles.cc

# función que prepara los argumentos y el fichero binario, si es necesario
function prepArgsBin ()
{
  args=""
  VAL="valgrind -q" 
  
  if test "$1" == "p2_07_Save" ; then
    VAL="" # desactivar valgrind para esta prueba
  elif test "$1" == "p2_09_Param" ; then
    args="-i datos-entrada/file-import02"
  elif test "$1" == "p2_10_Param-error" ; then
    args="-l datos-entrada/file-load01.bin  -l datos-entrada/file-load01.bin"
  elif test "$1" == "p2_11_Param" ; then
    args="-l datos-entrada/file-load01.bin  -a"
  fi
}



let npruebas=$npruebasa
pok=0;

# Compilacion
echo
echo "*********************************************************"
echo " Compilando..."
echo "*********************************************************"
rm -rf *.o
rm -rf $PRINCIPAL
tempfile=`mktemp /tmp/prog2iiXXXXX`
g++ -Wall -g -std=c++11 $PRINCIPAL.cc -o $PRINCIPAL 2> $tempfile
if [ $? -ne 0 ]; then
        cat $tempfile
  echo "LA PRACTICA NO COMPILA"
else
  # aviso warnings
  nlineasWarnings=$(wc --lines $tempfile | cut -f 1 -d ' ')
  if [ $nlineasWarnings -ne 0 ]; then
     cat $tempfile
     echo
     echo "LA PRACTICA COMPILA, PERO TIENE WARNINGS, DEBES QUITARLOS"
     read -p "Pulsa [Enter] para continuar"
     echo
  fi


  # Ejecucion y comprobacion de la salida
  echo
  echo "*********************************************************"
  echo " Ejecutando y comprobando salida a la vez..."
  echo "*********************************************************"
#  chmod +x $mata
#  chmod +x $comparefiles


  for i in `ls $ENT`
  do
        bn=$(basename $i .txt)
	if test $bn = $i ; then 
        continue  # si no es un .txt no es un fichero de entrada
        fi

        echo -n "Prueba $bn"
        prepArgsBin $bn

        if test "$args" != "" ; then
          echo " (argumentos:$args)"
        else
          echo 
        fi


  # Ejecucion del programa
  $mata $MAXTIME $OBT/${bn}.salida-obtenida $VAL $PRINCIPAL $args < $ENT/$i > $OBT/${bn}.salida-obtenida 2> $tempfile

  if test -s $tempfile; then
    echo "ERROR DE EJECUCION..."
    cat $tempfile
    rm -rf $OBT/${bn}.salida-obtenida $tempfile
  else
    $comparefiles $OBT/${bn}.salida-obtenida $SAL/${bn}.salida-correcta-esperada
    if [ $? -ne 0 ]; then
      diff -EwB $OBT/${bn}.salida-obtenida $SAL/${bn}.salida-correcta-esperada 2>&1
    else     
     if test "$bn" == "p2_05_Export"; then
 	$comparefiles $OBT/file-export01 $SAL/file-export01
	if [ $? -ne 0 ]; then
		diff -EwB $OBT/file-export01 $SAL/file-export01 2>&1
	else
 	   echo "OK"
	   let pok++
	fi
      else
        echo "OK"
        let pok++
      fi
    fi
  fi;
  echo "--------------------------"
  done

  if [ $pok -eq $npruebas ]; then
  echo -e "\nTODAS LAS PRUEBAS DEL AUTOCORRECTOR FUNCIONAN\n"
  else
    echo -e "\nOJO: FALLAN" $[npruebas-pok] "PRUEBAS DEL AUTOCORRECTOR\n"
  fi;
fi;

rm -f $tempfile
