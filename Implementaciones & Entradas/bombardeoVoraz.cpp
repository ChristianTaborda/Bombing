
/********************************************
 * Solución del problema de los bombardeos  *
 * utilizando la estrategia de programación *
 * voraz.                                *
 ********************************************/

//Librerías y constantes utilizadas:
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>

using namespace std;

//Estructura para los blancos (posiciónX,Radio): 
struct blanco{	
	//Atributos de la estructura:
	int pos;
	int R;	
	//Constructor general:
	blanco(){}
	//Constructor específico:
	blanco(int pos, int R){
		this->pos = pos;
		this->R = R;
	}	
};

//Método para combinar dos arreglos ordenados según su posición:
//Método para combinar dos arreglos ordenados según su posición:
blanco * merge(blanco * B1, blanco * B2, int n, int nB1, int nB2){
	//Arreglo para almacenar la combinación de los arreglos entrantes:
	blanco * salida = new blanco[n];	
	//Iteradores para recorrer los arreglos entrantes:
	int x=0, y=0;	
	//Combinación iterativa de los arreglos entrantes:
	for(int z=0; z<n; z++){		
		//Control del orden del arreglo resultante:
		if(x == nB1 && y != nB2){
			salida[z] = B2[y];
			y++;
		}
		if(y == nB2 && x != nB1){
			salida[z] = B1[x];
			x++;
		}
		if(x != nB1 && y != nB1){
			if(B1[x].pos > B2[y].pos){
				salida[z] = B2[y];
				y++;
			}else{
				salida[z] = B1[x];
				x++;
			}
		}
	}	
	return salida;	
}

//Método para ordenar un arreglo de blancos de acuerdo a su posición:
blanco * mergesort(blanco * B, int n){
	//Control del ordenamiento de acuerdo a la cantidad de blancos:
	if(n == 1){		
		//Con un sólo blanco se retorna el mismo arreglo:
		return B;		
	}else{		
		if(n == 2){			
			//Con dos blancos se verifica si se deben intercambiar de lugar:
			if(B[0].pos>B[1].pos){
				//Intercambio de lugar de ambos blancos:
				blanco aux = B[0];				
				B[0] = B[1];
				B[1] = aux;				
				return B;				
			}else{				
				return B;				
			}			
		}else{	
			//Con más blancos se divide el arreglo en dos partes:
			int mitad = floor(n/2);
			blanco * B1 = new blanco[mitad];
			blanco * B2 = new blanco[n-mitad];
			for(int x=0; x<mitad; x++){
				B1[x] = B[x];
			}
			for(int x=0; x<(n-mitad); x++){
				B2[x] = B[mitad+x];
			}	
			//Combinación del ordenamiento de ambas mitades:
			return merge(mergesort(B1,mitad),mergesort(B2,n-mitad),n,mitad,n-mitad);		
		}		
	}
}

//Método para calcular el alcance de un blanco:
double area(blanco B){
	double area = B.R * B.R * M_PI;
	return area;
}

//Método para castear de string a int:
int castear(string numero){
	stringstream S(numero);
	int salida = 0;
	S >> salida;
	return salida;
}

//Método para leer un archivo de texto y almacenar sus palabras en un vector:
void leer(string nombreArchivo, vector <int> &lectura){
	//Variable para almacenar cada palabra leída:
	string palabra;
	//Apertura del archivo:
	ifstream archivo(nombreArchivo.c_str());
	//Control de la existencia del archivo:
	if(archivo.good() == 1){
		//Lectura palabra por palabra del archivo:
		while(!archivo.eof()){
			archivo >> palabra;
			//Almacenamiento de la palabra en el vector:
			lectura.push_back(castear(palabra));
		}		
	}
	//Cierre del archivo:
	archivo.close();
}

//Método para inicializar un arreglo de blancos a partir de un vector:
void crearBlancos(vector <int> lectura, blanco * blancos, int n){
	//Iterador para recorrer el vector:
	int contador = 2;
	for(int x=0; x<n; x++){
		//Inicialización iterativa de cada blanco:
		blancos[x] = blanco(lectura[contador],lectura[contador+1]);
		contador += 2;
	}
}

//Método para elegir el máximo entre dos números e indicar cuál fue elegido:
double max(double A, double B, int &aux){
	if(A > B){
		//Indicación de que se eligió como máximo el primer parámetro:
		aux = 1;
		return A;
	}else{
		//Indicación de que se eligió como máximo el segundo parámetro:
		aux = 2;
		return B;
	}
}

//Método para calcular el punto donde acaba el alcance de un blanco:
int solapamiento(blanco B){
	return (B.pos + B.R);
}

bool revisarSolapamiento(blanco b1, blanco b2){
	double sumaDeRadios = b1.R + b2.R;
	double distanciaEntreCentros = b2.pos - b1.pos;
	if(distanciaEntreCentros >= sumaDeRadios){
		return false;
	}else{
		return true;
	}
}

int main(){
	//Vector para almacenar las palabras del archivo de texto:
	vector <int> lectura;
	//Captación del nombre del archivo:
	string nombreArchivo;
	cout << "\nIngrese el nombre del archivo: ";
	cin >> nombreArchivo;
	//Lectura del archivo y almacenamiento de las palabras en el vector:
	leer(nombreArchivo,lectura);
	//Variables para la cantidad de blancos y las dimension vertical del cuadro:
	double alto = lectura[1]/2;
	int n = (lectura.size()-2)/2;
	//Control de la existencia de blancos:
	if(n > 0){
		//Inicialización del arreglo de blancos con las palabras del vector:
		blanco * blancos = new blanco[n];
		crearBlancos(lectura,blancos,n);
		
		//Variables para calcular el tiempo de ejecución:
		unsigned ti, tf;
		//Cálculo del tiempo inicial de ejecución:
		ti = clock();
		
		//Ordenamiento de los blancos según su posición:
		blancos = mergesort(blancos,n);
		
		// Vector para guardar los blancos que irán en la solucion. Se agrega en un principio el primer blanco
		vector <blanco> solution;
		solution.push_back(blancos[0]);
		// Variable para ir adhiriendo las areas que cubren los blancos de la solucion. Se agrega en un principio el area del primer blanco
		double totalExplosionArea = area(blancos[0]);
		// Adherir a la solucion cada blanco que esté primero en la lista de blancos y que no se solape con alguno ya en la solucion
		for(int i=1; i<n; i++){
			// El blanco actual se adiciona a la solucion si no se solapa el ultimo elemento de esta
			if(!(revisarSolapamiento(solution[solution.size()-1], blancos[i]))){
				solution.push_back(blancos[i]);
				totalExplosionArea += area(blancos[i]);
			}
		}
		//Cálculo del tiempo final de ejecución:
		tf = clock();
		double tiempoEjecucion = (double (tf-ti)/CLOCKS_PER_SEC);
		cout << "\nEl tiempo total de ejecución es: " << tiempoEjecucion << endl;
		/*// Imprimir blancos guardados en la solucion
		cout << solution.size() << endl;
		for(int i=0; i<solution.size(); i++){
			cout << solution[i].pos << " | " << solution[i].R << endl;
		}*/
		cout << "El área de daño es: " << totalExplosionArea << endl;
	}else{
		cout << "\nNo hay blancos para evaluar.\n";
	}	
	return 0;	
}
