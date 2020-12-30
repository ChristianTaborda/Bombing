/********************************************
* Solución del problema de los bombardeos   *
* utilizando la estrategia de divide y      *
* venceras                                  *
********************************************/
//librerias y constante utilizada
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <ctime> 
#define PI 3.1416;

using namespace std;
//Definicion de metodos
vector<vector<double>> merge(vector<vector<double>> B1, vector<vector<double>> B2);
vector<vector<double>> mergeSort(vector<vector<double>> B);
double area(int num);
vector<double> max(vector<double> num1, vector<double> num2);
vector<double> maximum(vector<vector <double>> array,vector<double> obje,int x,int r,int N,int M);
int castear(string numero);
void leer(string nombreArchivo,  vector <int> &lectura);
void crearBlancos(vector <int> lectura,vector <vector<double>> &blancos);

int main(int argc, char *argv[]) {
	//Variables usadas para calcular el tiempo 
	unsigned t0, t1;
	//Vector para almacenar la lectura
	vector <int> lectura;
	//Captación del nombre del archivo
	string nombreArchivo;
	cout << "\nIngrese el nombre del archivo: ";
	cin >> nombreArchivo;
	//Lectura del archivo y almacenamiento de las palabras en el vector
	leer(nombreArchivo,lectura);
	//Variables para la cantidad de blancos y las dimensiones del cuadro
	int N = lectura[0];	 
	double M= lectura[1];
	int n = (lectura.size()-2)/2;
	//Control de existencia de blancos
	if(n > 0){
		//Inicialización del vector de blancos con las palabras del vector de lectura
		vector<vector<double>> blancos;	
		crearBlancos(lectura,blancos);
		//Inicializacion del tiempo de ejecucion
		t0=clock();
		//Ordenamiento de los blancos
		blancos=mergeSort(blancos);
		//Impresion de los objetivos iniciales ordenados
		cout<<"objetivos iniciales:"<<endl;
		for(int x=0; x<blancos.size(); x++){//se imprimen los blancos iniciales
			cout<<"<"<<blancos[x].front()<<", "<<blancos[x].back()<<"> ";
		}
		cout<<endl;
		//Vector para almacenar el area maxima y las posiciones solucion
		vector <double> objetivos;
		objetivos.push_back(0);
		//Calculo de las soluciones 
		objetivos= maximum(blancos,objetivos,0,0,N,M);
		//Impresion de las soluciones:
		cout<<"Area de daño máxima: "<<objetivos.front()<<endl;
		cout<<"objetivos seleccionados:"<<endl;
		for(int x=1; x<objetivos.size(); x++){
			cout<<"<"<<objetivos[x]<<", "<<M/2<<"> ";
		}
		//Calculo e impresión del tiempo de ejecucion
		t1 = clock();
		double tiempo = (double(t1-t0)/CLOCKS_PER_SEC);
		cout<<"\nTiempo: "<<tiempo<<endl;
	}else{
		cout << "\nNo hay blancos para evaluar.\n";
	}	
	return 0;

}
//Método para combinar dos vectores ordenados según su posición
vector<vector<double>> merge(vector<vector<double>> B1, vector<vector<double>> B2){
	//Vector para almacenar la combinación de los blancos entrantes
	vector<vector<double>> salida;
	//Combinación iterativa de los arreglos entrantes
	while (B1.size() > 0 || B2.size() > 0) {
		//Control de la cantidad de blancos en ambos vectores
		if (B1.size() > 0 && B2.size() > 0) {
			//Control del orden del vector resultante:
			if (B1.front().front() <= B2.front().front()) {
				//Almacenamiento de el blanco en orden
				salida.push_back(B1.front());
				B1.erase(B1.begin());
			} 
			else {
				//Almacenamiento de el blanco en orden
				salida.push_back(B2.front());
				B2.erase(B2.begin());
			}
			//Control de cantidad de blancos para un vector(primero)
		}  else if (B1.size() > 0) {
			for (int i = 0; i < B1.size(); i++)
				//Almacenamiento de el blanco en orden
				salida.push_back(B1[i]);
			break;
			//Control de cantidad de blancos para un vector(segundo)
		}  else if (B2.size() > 0) {
			for (int i = 0; i < B2.size(); i++)
				//Almacenamiento de el blanco en orden
				salida.push_back(B2[i]);
			break;
		}
	}
	return salida;
}
//Método para ordenar un arreglo de blancos de acuerdo a su posición
vector<vector<double>> mergeSort(vector<vector<double>> B){
	//Control del ordenamiento de acuerdo a la cantidad de blancos
	if (B.size() <= 1){
		//Con un sólo blanco se retorna el mismo vector
		return B;
	}else{
		
		vector<vector<double>> B1, B2;
		//Se divide el arreglo en dos partes
		int mitad = floor(B.size()/2);
		
		for (int i = 0; i < mitad; i++) {
			B1.push_back(B[i]);
		}
		
		for (int i =mitad; i < B.size(); i++) {
			B2.push_back(B[i]);
		}
		//Combinación del ordenamiento de ambas mitades
		return merge(mergeSort(B1),mergeSort(B2));
	}
}

//Método para calcular el alcance de un blanco
double area(int num){
	double area= num*num*PI;
	return area;
}
//Método para elegir el máximo entre dos números(posicion[0] de los vectores)  y retorna cuál fue elegido
vector <double> max(vector <double> num1, vector <double> num2){
	if(num1[0] > num2[0]){
		return num1;
	}else{
		return  num2;
	}
}
//Metodo para calcular la solucion mas optima
vector <double> maximum(vector<vector <double>> array,vector <double> obje,int x,int r,int N,int M){
	//Se obtiene el tamaño del vector de blancos entrante
	int size = array.size();
	//Se "clona" el vector solucion, esto para controlar entre haber tomado o no el blanco
	vector <double> clon = obje;
	//control de la cantidad de blancos
	if(size==0){
		return obje;
	}
	else{
		//suma que ayuda a controlar cuando ocurre solapamiento(suma de la posicion anterior)
		int add= x + r;
		//Extracción la posicion y el radio actual
		int xp = array.front().front();
		int rp = array.front().back();
		//Control de solapamiento y de desborde 
		if(add <= (xp-rp) && (M/2 + rp)<=M && (xp+rp)<=N){
			//Almacenamiento de la posicion tomada
			clon.push_back(xp); 
			//Actualizacion del area con la posicion tomada
			clon.front() = clon.front()+area(rp);
			//Eliminacion del blanco actual del vector de blancos
			array.erase(array.begin());
			//Ejecucion de la recursion y calculo de la solucion mas optima entre tomar o no tomar algunos blancos
			return max(maximum(array,obje,x,r,N,M), maximum(array,clon,xp,rp,N,M));
		}else{
			//Con solapamiento se eliminación de el blanco y ejecución de la recursion de manera normal
			array.erase(array.begin());
			return  maximum(array,obje,x,r,N,M);
		}
	}
}

//Método para castear de string a int
int castear(string numero){
	stringstream S(numero);
	int salida = 0;
	S >> salida;
	return salida;
}
//Método para leer un archivo de texto y almacenar sus palabras en un vector
void leer(string nombreArchivo,  vector <int> &lectura){
	//Variable para almacenar cada palabra leída
	string palabra;
	//Apertura del archivo
	ifstream archivo(nombreArchivo);
	//Control de la existencia del archivo
	if(archivo.good() == 1){
		//Lectura palabra por palabra del archivo
		while(!archivo.eof()){
			archivo >> palabra;
			
			//Almacen amiento de la palabra en el vector
			lectura.push_back(castear(palabra));
		}		
	}else{
		cout << "El archivo no existe. \n";
	}
	//Cierre del archivo
	archivo.close();
}
//Método para inicializar un vector de vectores de blancos a partir de un vector
void crearBlancos(vector <int> lectura,vector <vector<double>> &blancos){
	//Iterador para recorrer el vector
	int contador = 2;
	int n=(lectura.size()-2)/2;
	for(int x=0; x<n; x++){
		//Inicialización iterativa de cada blanco
		//vector auxiliar para crear las duplas
		vector <double> vaux;
		//Guardado de posicion
		vaux.push_back(lectura[contador]);
		//Guardado de  radio
		vaux.push_back(lectura[contador+1]);
		//Guardado del blanco
		blancos.push_back(vaux);
		contador += 2;
	}
}

