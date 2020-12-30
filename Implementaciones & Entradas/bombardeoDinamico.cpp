
/********************************************
 * Solución del problema de los bombardeos  *
 * utilizando la estrategia de programación *
 * dinámica.                                *
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

//Estructura para los blancos (posiciónX, Radio): 
struct blanco{	
	//Atributos de la estructura:
	int pos, R;	
	//Constructor general:
	blanco(){}
	//Constructor específico:
	blanco(int pos, int R){
		this->pos = pos;
		this->R = R;
	}	
};

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

//Método para calcular el área de un blanco:
double area(blanco B){
	return (B.R * B.R * M_PI);
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

//Método para calcular el punto donde acaba el área de un blanco:
int solapamiento(blanco B){
	return (B.pos + B.R);
}

//Método para inicializar la matriz de beneficios y la matriz auxiliar para construir la solución:
void beneficios(blanco * blancos, double ** V, int ** A, int m, int n){
	//Variables para indicar las decisiones tomadas y los criterios de solapamiento:
	int aux, S;
	for(int x=0; x<m; x++){
		for(int y=0; y<n; y++){
			//Control de la cantidad de blancos o del criterio de solapamiento:
			if(x == 0 || y == 0){
				//Con los 0 primeros blancos y un criterio de solapamiento en la posición 0:
				V[x][y] = 0;
				A[x][y] = 0;
			}else{
				//Cálculo del punto donde termina el área del blanco actual:
				S = solapamiento(blancos[y-1]);
				//Control de solapamiento:
				if(S <= x){
					//Sin solapamiento se maximiza el área entre incluir o no incluir el blanco actual:
					V[x][y] = max(V[x][y-1],area(blancos[y-1])+V[S-(2*blancos[y-1].R)][y-1],aux);
					//Control de la inclusión o exclusión del blanco actual:
					if(aux == 2){
						A[x][y] = 1;
					}else{
						A[x][y] = 0;
					}
				}else{
					//Con solapamiento se excluye el blanco actual:
					V[x][y] = V[x][y-1];
					A[x][y] = 0;
				}
			}
		}
	}
}

//Método para imprimir la solución a partir de una matriz auxiliar:
void imprimirSolucion(int ** A, blanco * blancos, int m, int n, double alto){
	cout << "\nLos blancos seleccionados son: ";
	while(n > 0){
		//Control de la inclusión o exclusión del blanco actual:
		if(A[m][n] == 1){
			//Impresión del blanco incluido y avance hasta la posición del siguiente blanco:
			cout << "(" << blancos[n-1].pos << "," << alto << ") ";
			m = blancos[n-1].pos - blancos[n-1].R;
			n--;
		}else{
			//Avance hasta la posición del siguiente blanco:
			n--;
		}
	}
	cout << endl;
}

//Método para encontrar el mayor criterio de solapamiento:
int limitar(blanco * blancos, int n){
	//Variable para contener el mayor criterio de solapamiento:
	int limite = solapamiento(blancos[0]);
	//Variable para contener el criterio de solapamiento parcial:
	int S;
	for(int x=1; x<n; x++){
		//Cálculo del solapamiento parcial:
		S = solapamiento(blancos[x]); 
		//Control en caso de encontrar un criterio de solapamiento mayor:
		if(S > limite){
			limite = S; 
		}
	}
	return limite;
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
	//Control de la existencia del archivo:
	if(!lectura.empty()){
		//Variables para almacenar la cantidad de blancos y la dimensión vertical del cuadro:
		int n = (lectura.size()-2)/2;
		double alto = lectura[1]/2;
		//Control de la existencia de blancos:
		if(n > 0){
			//Inicialización del arreglo de blancos con las palabras del vector:
			blanco * blancos = new blanco[n];
			crearBlancos(lectura,blancos,n);
			//Variable para almacenar el limite a utilizar de la dimensión horizontal del cuadro:
			int m = limitar(blancos,n);
			//Matrices para almacenar los beneficios y las decisiones para la solución:
			double ** V = new double * [m+1];
			int ** A = new int * [m+1];
			for(int x=0; x<(m+1); x++){
				V[x] = new double[n+1];
				A[x] = new int[n+1];
			}
			//Variables para calcular el tiempo de ejecución:
			unsigned ti, tf;
			//Cálculo del tiempo inicial de ejecución:
			ti = clock();
			//Ordenamiento de los blancos según su posición:
			blancos = mergesort(blancos,n);
			//Inicialización de la matriz de beneficios y la matriz auxiliar:
			beneficios(blancos,V,A,m+1,n+1);
			//Impresión de la solución encontrada:
			imprimirSolucion(A,blancos,m,n,alto);
			cout << "\nEl área máxima de daño es: " << V[m][n] << endl;
			//Cálculo del tiempo final de ejecución:
			tf = clock();
			//Impresión del tiempo total de ejecución:
			double tiempoEjecucion = (double (tf-ti)/CLOCKS_PER_SEC);
			cout << "\nEl tiempo total de ejecución es: " << tiempoEjecucion << endl;
			//Liberación de la memoria reservada con los punteros:
			for(int x=0; x<(m+1); x++){
				delete[] V[x];
				delete[] A[x];
			}
			delete[] V;
			delete[] A;
		}else{
			cout << "\nNo hay blancos para evaluar.\n";
		}		
	}else{
		cout << "\nEl archivo no existe.\n";
	}	
	return 0;	
}
