// DNI 45929352P Gabriel De Lamo Dutra

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <cstring>
#include <cstdlib>


using namespace std;

const int MINCONTAINERWEIGHT=100;
const int MINCONTAINERVALUE=100;
const int MINMAXCONTAINERS=5;
const int MINMAXWEIGHT=500;
unsigned int CONTAINERID=1;
const unsigned int MAXNAME=20;

struct Container{
    unsigned int id;
    unsigned int weight;
    unsigned int value;
};
struct Ship{
    string name;
    unsigned int weight;
    unsigned int value;
    unsigned int maxWeight;
    unsigned int maxContainers;
    vector<Container> containers;
};
struct Base{
    string name;
    vector<Container> containers;
    vector<Ship> ships;
};
struct BinShip
{
char name[MAXNAME];
unsigned int weight;
unsigned int value;
unsigned int maxWeight;
unsigned int maxContainers;
unsigned int numContainers;
};
struct BinBase
{
char name[MAXNAME];
unsigned int numContainers;
unsigned int nextId;
};
	//Errors
enum Error
{
  ERR_MENU_OPTION,
  ERR_CONTAINER_WEIGHT,
  ERR_CONTAINER_VALUE,
  ERR_CONTAINER_ID,
  ERR_SHIP_NAME,
  ERR_SHIP_MAXCONTAINERS,
  ERR_SHIP_MAXWEIGHT,
  ERR_SHIP_NO_MORE_CONTAINERS,
  ERR_SHIP_NO_MORE_WEIGHT,
  ERR_CANT_OPEN_FILE,
  ERR_ARGS
};
void error(Error n)
{
  switch (n)
  {
    case ERR_MENU_OPTION:
      cout << "Error: unknown menu option" << endl;
      break;

    case ERR_CONTAINER_WEIGHT:
      cout << "Error: container weight is below minimum" << endl;
      break;

    case ERR_CONTAINER_VALUE:
      cout << "Error: container value is below minimum" << endl;
      break;

    case ERR_CONTAINER_ID:
      cout << "Error: wrong container id" << endl;
      break;

    case ERR_SHIP_NAME:
      cout << "Error: wrong ship name" << endl;
      break;

    case ERR_SHIP_MAXCONTAINERS:
      cout << "Error: wrong maximum number of containers" << endl;
      break;

    case ERR_SHIP_MAXWEIGHT:
      cout << "Error: wrong maximum weight" << endl;
      break;

    case ERR_SHIP_NO_MORE_CONTAINERS:
      cout << "Error: ship cannot carry more containers" << endl;
      break;

    case ERR_SHIP_NO_MORE_WEIGHT:
      cout << "Error: ship cannot not hold more containers" << endl;
      break;

    case ERR_CANT_OPEN_FILE:
      cout << "Error: can't open file" << endl;
      break;

    case ERR_ARGS:
      cout << "Error: wrong arguments" << endl;
  }
}

	//Prints
void printContainer(const Container &c){
    cout<<"["<<c.id<<" "<<c.weight<<":"<<c.value<<"]";
}
void printShip(const Ship &s){
    cout<<"{"<<s.name<<":"<<" "<<s.weight<<" "<<"("<<s.maxWeight<<")"<<", "<<s.containers.size()<<" ("<<s.maxContainers<<"), "<<s.value<<endl;
    for (unsigned int i = 0; i <s.containers.size() ; i++){
        printContainer(s.containers[i]);
    }
    cout<<"\n"<<"}";
}
void printBase(const Base &b){
    cout<<"Name: "<<b.name<<endl;
    cout<<"Containers: "<<endl;
    for (unsigned int i = 0; i <b.containers.size() ; i++){
        printContainer(b.containers[i]);
        cout<<endl;
    }
    cout <<"Ships: " <<endl;
    for (unsigned int i = 0; i <b.ships.size() ; i++){
        printShip(b.ships[i]);
        cout << endl;
    }
}
	//Container
void addContainer(Base &b,bool fromImport, int imWeight, int imValue){

    Container newContainer;
    int nuevopeso,nuevovalor;

    if(fromImport){
        if (imWeight< MINCONTAINERWEIGHT) {
            error(ERR_CONTAINER_WEIGHT);
            return;
        }
        if (imValue< MINCONTAINERVALUE) {
            error(ERR_CONTAINER_VALUE);
            return;
        }
        newContainer.id=CONTAINERID;
        newContainer.weight=imWeight;
        newContainer.value=imValue;
        b.containers.push_back(newContainer);
        CONTAINERID++;
        return;
    }

    //Check Weight
    cout<<"Container weight: ";
    cin >> nuevopeso;
    cin.get();
    if (nuevopeso < MINCONTAINERWEIGHT) {
        error(ERR_CONTAINER_WEIGHT);
        return;
    }
    //Check Value
    cout<<"Container value: ";
    cin >> nuevovalor;
    cin.get();
    if (nuevovalor < MINCONTAINERVALUE) {
        error(ERR_CONTAINER_VALUE);
        return;
    }

    newContainer.id=CONTAINERID;
    newContainer.weight=nuevopeso;
    newContainer.value=nuevovalor;
    b.containers.push_back(newContainer);
    CONTAINERID++;
}
void removeContainer(Base &b){
    unsigned int IDtoDelete;
    bool found=false;
    cout <<"Container id: ";
    cin>>IDtoDelete;
    for (size_t i = 0; i < b.containers.size(); i++) {
        if (IDtoDelete==b.containers[i].id) {
            b.containers.erase(b.containers.begin()+i);
            found=true;
        }
    }
    if (found==false) {
        error(ERR_CONTAINER_ID);
    }
}
void unassignContainer(Base &b){
    string ShipName;
    unsigned int ShipPosition,IDtoDelete;
    bool foundShip=false,foundContainer=false;

	cout << "Ship name: ";
	getline(cin,ShipName);
	for(unsigned int i=0 ; i<b.ships.size() ; i++){
		if(ShipName == b.ships[i].name){
			foundShip=true;
	        ShipPosition=i;
		}
	 }

	 if (foundShip==true) {
		 cout <<"Container id: ";
		 cin>>IDtoDelete;
		 for (size_t i = 0; i < b.ships[ShipPosition].containers.size() ; i++) {
			 if (IDtoDelete== b.ships[ShipPosition].containers[i].id) {

		        	 b.ships[ShipPosition].value-=b.ships[ShipPosition].containers[i].value;
		        	 b.ships[ShipPosition].weight-=b.ships[ShipPosition].containers[i].weight;
		        	 b.containers.push_back(b.ships[ShipPosition].containers[i]);
		        	 b.ships[ShipPosition].containers.erase(b.ships[ShipPosition].containers.begin()+i); //Borramos el Container del Barco


		             foundContainer=true;

		         }
		     }
	 }

	 if (foundShip==false) {
		 error(ERR_SHIP_NAME);
		 return;
	 }
     if (foundContainer==false) {
         error(ERR_CONTAINER_ID);
         return;
     }
}
void clearAssignations(Base &b){
	for(size_t i=0; i< b.ships.size(); i++){
		for(size_t j=0; j< b.ships[i].containers.size(); j++){
			b.ships[i].value-=b.ships[i].containers[j].value;
			b.ships[i].weight-=b.ships[i].containers[j].weight;
			b.containers.push_back(b.ships[i].containers[j]);
			b.ships[i].containers.erase(b.ships[i].containers.begin()+j); //Borramos el Container del Barco
			j--;
		}
	}
}
	//Ship
void addShip(Base &b,bool fromImport, int imWeight, int imMaxCont, string name){

    Ship newShip;
    string ShipName;
    int maxpeso,maxcontainer;
    bool found=false;

    if(fromImport){
    	for(unsigned int i=0 ; !found && i<b.ships.size() ; i++){
    		if(name == b.ships[i].name){
    			error(ERR_SHIP_NAME);
    	       	return;
    		}
    	}
    	if (imMaxCont < MINMAXCONTAINERS) {
    		error(ERR_SHIP_MAXCONTAINERS);
    		return;
    	}
        if (imWeight<MINMAXWEIGHT) {
            error(ERR_SHIP_MAXWEIGHT);
            return;
        }
        newShip.name=name;
        newShip.weight=0;
        newShip.value=0;
        newShip.maxWeight=imWeight;
        newShip.maxContainers=imMaxCont;
        b.ships.push_back(newShip);
        return;
    }


    //Ship name
    cout<<"Ship name: ";
    getline(cin,newShip.name);
    for(unsigned int i=0 ; !found && i<b.ships.size() ; i++){
        if(newShip.name == b.ships[i].name){
            found=true;
            error(ERR_SHIP_NAME);
            return;
        }
    }

    //Ship Container
    cout<<"Ship max. containers: ";
    cin >> maxcontainer;
    cin.get();
    if (maxcontainer < MINMAXCONTAINERS) {
        error(ERR_SHIP_MAXCONTAINERS);
        return;
    }

    //Ship wheight
    cout<<"Ship max. weight: ";
    cin>> maxpeso;
    cin.get();
    if (maxpeso<MINMAXWEIGHT) {
        error(ERR_SHIP_MAXWEIGHT);
        return;
    }

    //Next Atributtes
    newShip.weight=0;
    newShip.value=0;
    newShip.maxWeight=maxpeso;
    newShip.maxContainers=maxcontainer;
    b.ships.push_back(newShip);
}
void removeShip(Base &b){
    string ShipName;
    unsigned int ShipPosition;
    bool found=false;

    cout<< "Ship name: ";
    getline(cin,ShipName);

    for(unsigned int i=0 ; i<b.ships.size() ; i++){
        if(ShipName == b.ships[i].name){
            found=true;
            ShipPosition=i;
        }
    }

    if (found==true) {
        for (size_t i = 0; i < b.ships[ShipPosition].containers.size(); i++) {
            b.containers.push_back(b.ships[ShipPosition].containers[i]);
        }
        b.ships.erase(b.ships.begin()+ShipPosition);
    }
    if (found==false) {
        error(ERR_SHIP_NAME);
    }

}
	//Distribution
void manualDistribution(Base &b){
    unsigned int CONTAINERIDsearch,ContainerPosition,ShipPosition;
    string shipNAMEsearch="";
    bool ContainerFound=false,shipFound=false;

    cout << "Container id: ";
    cin >> CONTAINERIDsearch;
    cin.get();
    for (size_t i = 0; i < b.containers.size(); i++) {
        if (b.containers[i].id==CONTAINERIDsearch) {
            ContainerFound=true;
            ContainerPosition=i;
        }
    }
    if (ContainerFound==false) {
        error(ERR_CONTAINER_ID);
        return;
    }
    cout << "Ship name: ";

    getline(cin,shipNAMEsearch);
    for (size_t i = 0; i < b.ships.size(); i++) {
        if (b.ships[i].name == shipNAMEsearch) {
            shipFound=true;
            ShipPosition=i;
        }
    }
    if (shipFound==false) {
        error(ERR_SHIP_NAME);
        return;
    }
    if (b.ships[ShipPosition].containers.size() == b.ships[ShipPosition].maxContainers){
        error(ERR_SHIP_NO_MORE_CONTAINERS);
        return;
    }
    if ( (b.ships[ShipPosition].weight + b.containers[ContainerPosition].weight) > b.ships[ShipPosition].maxWeight ) {
        error(ERR_SHIP_NO_MORE_WEIGHT);
        return;
    }
    b.ships[ShipPosition].containers.push_back(b.containers[ContainerPosition]);
    b.ships[ShipPosition].value+=b.containers[ContainerPosition].value;
    b.ships[ShipPosition].weight+=b.containers[ContainerPosition].weight;
    b.containers.erase(b.containers.begin()+ContainerPosition);
}
void automaticDistribution(Base &b){
    do{
        bool ContainerFound=false,ShipFound=false;
        unsigned int ContainerPosition,
        ShipPosition;
        //De donde no hay , no se puede sacar
        if (b.containers.empty() || b.ships.empty()) {
            return;
        }
        //For que busca el contenedor adecuado
        for (size_t i = 0; i < b.containers.size(); i++) {
            if (ContainerFound==false) {
                ContainerPosition=i;
                ContainerFound=true;
            }
            if (ContainerFound==true) {
                if (b.containers[i].value > b.containers[ContainerPosition].value) {
                    ContainerPosition=i;
                }
                if (b.containers[i].value == b.containers[ContainerPosition].value) {
                    if (b.containers[i].weight < b.containers[ContainerPosition].weight ) {
                        ContainerPosition=i;
                    }
                }
            }
        }
        //For que busca el Barco adecuado
        for (size_t i = 0; i < b.ships.size(); i++) {
            if (b.ships[i].containers.size() < b.ships[i].maxContainers){
                if ( (b.ships[i].weight + b.containers[ContainerPosition].weight) <= b.ships[i].maxWeight) {
                    if (ShipFound==false) {
                        ShipPosition=i;
                        ShipFound=true;
                    }
                    if (ShipFound==true) {
                        if (b.ships[i].value < b.ships[ShipPosition].value) {
                            ShipPosition=i;
                        }
                    }
                }
            }
        }
        //En caso de encontrar ambos el contenedor y el barco
        if (ShipFound==true && ContainerFound==true) {
            b.ships[ShipPosition].containers.push_back(b.containers[ContainerPosition]);
            b.ships[ShipPosition].value+=b.containers[ContainerPosition].value;
            b.ships[ShipPosition].weight+=b.containers[ContainerPosition].weight;
            b.containers.erase(b.containers.begin()+ContainerPosition);
        }
        //En caso de encontrar el contenedor pero no un barco adecuado
        if (ShipFound==false && ContainerFound==true) {
            bool EveryShipFull=true;
            for (size_t i = 0; i < b.ships.size(); i++) {
                if (b.ships[i].containers.size() != b.ships[i].maxContainers) {
                    if ( (b.ships[i].maxWeight - b.ships[i].weight) >= MINCONTAINERWEIGHT) {
                        EveryShipFull=false;
                    }
                }
            }
            if (EveryShipFull==false) {
                Container container1,container2;

                if (b.containers[ContainerPosition].weight/2 < MINCONTAINERWEIGHT || b.containers[ContainerPosition].value/2 < MINCONTAINERVALUE) {
                    return;
                }

                container1.weight=(b.containers[ContainerPosition].weight/2);
                container1.value=(b.containers[ContainerPosition].value/2);
                container1.id=CONTAINERID;
                CONTAINERID++;

                container2.weight=(b.containers[ContainerPosition].weight/2);
                container2.value=(b.containers[ContainerPosition].value/2);
                container2.id=CONTAINERID;
                CONTAINERID++;

                if (b.containers[ContainerPosition].weight%2 != 0) {
                    container2.weight++;
                }
                if (b.containers[ContainerPosition].value %2 != 0) {
                    container2.value++;
                }

                b.containers.erase(b.containers.begin()+ContainerPosition);
                b.containers.push_back(container1);
                b.containers.push_back(container2);
            }
            if (EveryShipFull==true) {
                return;
            }
        }
    }while (b.containers.size() != 0) ;
}

	///FicheroLandia
void eraseData(Base &b){
	b.ships.clear();
	b.containers.clear();
}
void diffCom(const string &linea , unsigned int &i ,string &varia){
  while(linea[i]!='"'){
    varia+=linea[i]; // sumamos a la variable  los caracteres hasta leer las dobles comillas finales
    i++;
  }
}
void skipWS( const string &linea , unsigned int &i){
  //Función para saltar espacios en blanco a la hora de leer las cadenas
  while(linea[i]==' '){
    i++;
  }
}
void digito( const string &linea , unsigned int &i ,string &varia){
  //función para evaluar si es un dígito y procede añadiendo a la variable , avanza al siguiente espacio
  while(isdigit(linea[i])){
	  varia += linea[i];
	  i++;
  }
}

void parseContainer(Base &b, const string &linea,unsigned int &k){
	string weightSt="" , valueSt="";

	skipWS(linea,k);

	k++; //Saltamos al parentesis
	skipWS(linea,k);

	k++; //Saltamos al primer numero
	skipWS(linea,k);

	digito(linea,k,weightSt);
	skipWS(linea,k);

	k++; //Saltamos la coma
	skipWS(linea,k);

	digito(linea,k,valueSt);
	skipWS(linea,k);

	k++; //Saltamos el parentesis
	skipWS(linea,k);

	addContainer(b,true,stoi(weightSt),stoi(valueSt));
}
void parseShip(     Base &b, const string &linea,unsigned int &i){
	string shipName="";
	string shipWeight="";
	string shipMaxcontainer="";

	i++; //Saltamos al corchete
	skipWS(linea,i);

	i++; //Saltamos a las comillas
	skipWS(linea,i);

	i++; //Saltamos a la primera posición que forma parte del nombre de la nave
	diffCom(linea,i,shipName);

	i++; //Saltamos las comillas
	skipWS(linea,i);

	digito(linea,i,shipWeight); //Copiamos el weight
	skipWS(linea,i);

	i++; //Saltamos los dos puntos
	skipWS(linea,i);

	digito(linea,i,shipMaxcontainer); //Copiamos el MaxContainer
	skipWS(linea,i);

	i++; //Saltamos el corchete
	skipWS(linea,i);

	addShip(b,true,stoi(shipWeight),stoi(shipMaxcontainer),shipName);

}

void SorC(Base &b,const string &linea, unsigned int &i){
	if(linea[i]!='C' && linea[i]!='S'){
		i++;
	}
	if(linea[i]=='C'){
		parseContainer(b,linea,i);
	}
	if(linea[i]=='S'){
		parseShip(b,linea,i);
	}
}
void importData(Base &b, bool fromArg, string nombre){
	ifstream fichero;
	string nombreFichero=nombre,
		   lacadena="";
	char c=0;
	unsigned int j=0;

	//Si no viene por argumento
	if(!fromArg){
		cout << "Enter filename: ";
		nombreFichero.clear();
		getline(cin,nombreFichero);
	}

	fichero.open(nombreFichero.c_str(),ios::in);
	if(fichero.is_open()){
		while(fichero.get(c)){
				lacadena+=c; //Voy almacenando la cadena
		}
		fichero.close();
	}else{
		error(ERR_CANT_OPEN_FILE);
	}

	do{
		skipWS(lacadena,j);
		SorC(b,lacadena,j);
		skipWS(lacadena,j);
	}while(j<lacadena.length());
}

void exportBases(Base &b,ofstream &fichero,bool fromShip,unsigned int shipPosition){
		if(fromShip){
			for(unsigned int i=0;i<b.ships[shipPosition].containers.size();i++){
				fichero << "C(" << b.ships[shipPosition].containers[i].weight  << "," << b.ships[shipPosition].containers[i].value << ")" << endl;
			}
			return;
		}

		for(unsigned int i=0;i<b.containers.size();i++){
			fichero << "C(" << b.containers[i].weight  << "," << b.containers[i].value << ")" << endl;
		}
}
void exportShips(Base &b,ofstream &fichero){
	for(unsigned int i=0;i<b.ships.size();i++){
		fichero << "S[" << '"' << b.ships[i].name << '"' << " " << b.ships[i].maxWeight << ":" << b.ships[i].maxContainers << "]" <<endl;
			exportBases(b,fichero,true,i);
	}
}
void exportData( Base &b, bool fromArg, string nombre){
	ofstream fichero;
	string nombreFichero=nombre;

	if(!fromArg){
		cout << "Enter filename:";
		nombreFichero.clear();
		getline(cin,nombreFichero);
	}

	fichero.open(nombreFichero.c_str(),ios::out);
	if(fichero.is_open()){
		exportBases(b,fichero,false,0);
		exportShips(b,fichero);
		fichero.close();
	}else{
		error(ERR_CANT_OPEN_FILE);
	}
}

void loadData(Base &b, bool fromArg, string nombre){
	string nombreFichero=nombre;
	ifstream fichero;
	Ship newShip;
	Container newContainer,ctopush;

	BinBase baseB;
	BinShip shipB;

	string confirm;

	if(!fromArg){
		cout << "Enter filename:";
		nombreFichero.clear();
		getline(cin,nombreFichero);
	}

	 fichero.open(nombreFichero.c_str(),ios::in | ios::binary);

	 if(fichero.is_open()){
		 if(!fromArg){
			do{
				cout << "Confirm? (y/n):";
				getline(cin,confirm);
				if(confirm != "y" && confirm != "n"){
					confirm.clear();
				}
			}while(confirm != "y" && confirm !="n");

			if(confirm=="n"){
				return;
			}
			if(confirm=="y"){
				//Base
				eraseData(b);
				fichero.read((char *) &baseB, sizeof(baseB));
				b.name=baseB.name;

				//Container
				for(unsigned int i=0; i<baseB.numContainers; i++){
					fichero.read((char *) &newContainer, sizeof(Container));
					ctopush.id=newContainer.id;
					ctopush.weight=newContainer.weight;
					ctopush.value=newContainer.value;
					b.containers.push_back(ctopush);
				}
				//Ship

		 	 }
		}
		 fichero.close();
	}else{
		error(ERR_CANT_OPEN_FILE);
	}
}
void saveData(Base &b){
	string nombreFichero;
	ofstream fichero;
	string resizeName;

	cout << "Enter filename:";
	getline(cin,nombreFichero);

	fichero.open(nombreFichero.c_str(),ios::out);
		if(fichero.is_open()){
			BinBase baseB;
			BinShip shipB;

			resizeName=b.name;
			if (b.name.size() > MAXNAME-1){
				resizeName.resize(MAXNAME-1);
			}

			strcpy(baseB.name,resizeName.c_str());
    		baseB.numContainers=b.containers.size();

    			//Containers
    		for(unsigned i=0; i<b.containers.size();i++){
    			fichero.write((const char *) &b.containers[i],sizeof(b.containers));
    		}
    		fichero.close();
		}else{
			error(ERR_CANT_OPEN_FILE);
		}
}

////////////////////////////////////////////////////////////////////
bool processArguments(int argc,char *argv[],Base &b){
	if(argc%2!=0){
		for(int i=1; i<argc; i+=2){

		}
	}
}
void menu(){
    cout << "1- Info base" << endl
    << "2- Add container" << endl
    << "3- Remove container" << endl
    << "4- Add ship" << endl
    << "5- Remove ship" << endl
    << "6- Manual distribution" << endl
    << "7- Automatic distribution" << endl
	<< "8- Unassign container" << endl
	<< "9- Clear assignations" << endl
	<< "i- Import data"<< endl
	<< "x- Export data" << endl
	<< "l- Load base" << endl
	<< "s- Save base" << endl
    << "q- Quit" << endl
    << "Option: ";
}
int main(int argc,char *argv[]){
    Base base;
    base.name = "Logistic Center";
    char option;

    if (processArguments(argc,argv,base)){
    	do{
    		menu();
    		cin >> option; cin.get();
    		switch (option) {
				case '1': {
					printBase(base);
					break;
				}
				case '2':
				{
					addContainer(base,false,0,0);
					break;
				}
				case '3':
				{
					removeContainer(base);
					break;
				}
				case '4':
				{
					addShip(base,false,0,0,"lmao");
					break;
				}
				case '5':
				{
					removeShip(base);
					break;
				}
				case '6':
				{
					manualDistribution(base);
					break;
				}
				case '7':
				{
					automaticDistribution(base);
					break;
				}
				case '8':
				{
					unassignContainer(base);
					break;
				}
				case '9':
				{
					clearAssignations(base);
					break;
				}
				case 'i':{
					importData(base,false,"");
					break;
				}
				case 'x':{
					exportData(base,false,"");
					break;
				}
				case 'l':{
					loadData(base,false,"");
					break;
				}
				case 's':{
					saveData(base);
					break;
				}
				case 'q': {
					break;
				}
				default:  {
					error(ERR_MENU_OPTION);
					break;
				}
    		}
    	} while(option != 'q');
    }
    return 0;
}
