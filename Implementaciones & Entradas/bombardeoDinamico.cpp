
/********************************************
 * Soluci�n del problema de los bombardeos  *
 * utilizando la estrategia de programaci�n *
 * din�mica.                                *
 ********************************************/

//Librer�as y constantes utilizadas:
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>

using namespace std;

//Estructura para los blancos (posici�nX, Radio): 
struct blanco{	
	//Atributos de la estructura:
	int pos, R;	
	//Constructor general:
	blanco(){}
	//Constructor espec�fico:
	blanco(int pos, int R){
		this->pos = pos;
		this->R = R;
	}	
};

//M�todo para combinar dos arreglos ordenados seg�n su posici�n:
blanco * merge(blanco * B1, blanco * B2, int n, int nB1, int nB2){
	//Arreglo para almacenar la combinaci�n de los arreglos entrantes:
	blanco * salida = new blanco[n];	
	//Iteradores para recorrer los arreglos entrantes:
	int x=0, y=0;	
	//Combinaci�n iterativa de los arreglos entrantes:
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

//M�todo para ordenar un arreglo de blancos de acuerdo a su posici�n:
blanco * mergesort(blanco * B, int n){
	//Control del ordenamiento de acuerdo a la cantidad de blancos:
	if(n == 1){		
		//Con un s�lo blanco se retorna el mismo arreglo:
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
			//Con m�s blancos se divide el arreglo en dos partes:
			int mitad = floor(n/2);
			blanco * B1 = new blanco[mitad];
			blanco * B2 = new blanco[n-mitad];
			for(int x=0; x<mitad; x++){
				B1[x] = B[x];
			}
			for(int x=0; x<(n-mitad); x++){
				B2[x] = B[mitad+x];
			}	
			//Combinaci�n del ordenamiento de ambas mitades:
			return merge(mergesort(B1,mitad),mergesort(B2,n-mitad),n,mitad,n-mitad);		
		}		
	}
}

//M�todo para calcular el �rea de un blanco:
double area(blanco B){
	return (B.R * B.R * M_PI);
}

//M�todo para castear de string a int:
int castear(string numero){
	stringstream S(numero);
	int salida = 0;
	S >> salida;
	return salida;
}

//M�todo para leer un archivo de texto y almacenar sus palabras en un vector:
void leer(string nombreArchivo, vector <int> &lectura){
	//Variable para almacenar cada palabra le�da:
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

//M�todo para inicializar un arreglo de blancos a partir de un vector:
void crearBlancos(vector <int> lectura, blanco * blancos, int n){
	//Iterador para recorrer el vector:
	int contador = 2;
	for(int x=0; x<n; x++){
		//Inicializaci�n iterativa de cada blanco:
		blancos[x] = blanco(lectura[contador],lectura[contador+1]);
		contador += 2;
	}
}

//M�todo para elegir el m�ximo entre dos n�meros e indicar cu�l fue elegido:
double max(double A, double B, int &aux){
	if(A > B){
		//Indicaci�n de que se eligi� como m�ximo el primer par�metro:
		aux = 1;
		return A;
	}else{
		//Indicaci�n de que se eligi� como m�ximo el segundo par�metro:
		aux = 2;
		return B;
	}
}

//M�todo para calcular el punto donde acaba el �rea de un blanco:
int solapamiento(blanco B){
	return (B.pos + B.R);
}

//M�todo para inicializar la matriz de beneficios y la matriz auxiliar para construir la soluci�n:
void beneficios(blanco * blancos, double ** V, int ** A, int m, int n){
	//Variables para indicar las decisiones tomadas y los criterios de solapamiento:
	int aux, S;
	for(int x=0; x<m; x++){
		for(int y=0; y<n; y++){
			//Control de la cantidad de blancos o del criterio de solapamiento:
			if(x == 0 || y == 0){
				//Con los 0 primeros blancos y un criterio de solapamiento en la posici�n 0:
				V[x][y] = 0;
				A[x][y] = 0;
			}else{
				//C�lculo del punto donde termina el �rea del blanco actual:
				S = solapamiento(blancos[y-1]);
				//Control de solapamiento:
				if(S <= x){
					//Sin solapamiento se maximiza el �rea entre incluir o no incluir el blanco actual:
					V[x][y] = max(V[x][y-1],area(blancos[y-1])+V[S-(2*blancos[y-1].R)][y-1],aux);
					//Control de la inclusi�n o exclusi�n del blanco actual:
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

//M�todo para imprimir la soluci�n a partir de una matriz auxiliar:
void imprimirSolucion(int ** A, blanco * blancos, int m, int n, double alto){
	cout << "\nLos blancos seleccionados son: ";
	while(n > 0){
		//Control de la inclusi�n o exclusi�n del blanco actual:
		if(A[m][n] == 1){
			//Impresi�n del blanco incluido y avance hasta la posici�n del siguiente blanco:
			cout << "(" << blancos[n-1].pos << "," << alto << ") ";
			m = blancos[n-1].pos - blancos[n-1].R;
			n--;
		}else{
			//Avance hasta la posici�n del siguiente blanco:
			n--;
		}
	}
	cout << endl;
}

//M�todo para encontrar el mayor criterio de solapamiento:
int limitar(blanco * blancos, int n){
	//Variable para contener el mayor criterio de solapamiento:
	int limite = solapamiento(blancos[0]);
	//Variable para contener el criterio de solapamiento parcial:
	int S;
	for(int x=1; x<n; x++){
		//C�lculo del solapamiento parcial:
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
	//Captaci�n del nombre del archivo:
	string nombreArchivo;
	cout << "\nIngrese el nombre del archivo: ";
	cin >> nombreArchivo;
	//Lectura del archivo y almacenamiento de las palabras en el vector:
	leer(nombreArchivo,lectura);
	//Control de la existencia del archivo:
	if(!lectura.empty()){
		//Variables para almacenar la cantidad de blancos y la dimensi�n vertical del cuadro:
		int n = (lectura.size()-2)/2;
		double alto = lectura[1]/2;
		//Control de la existencia de blancos:
		if(n > 0){
			//Inicializaci�n del arreglo de blancos con las palabras del vector:
			blanco * blancos = new blanco[n];
			crearBlancos(lectura,blancos,n);
			//Variable para almacenar el limite a utilizar de la dimensi�n horizontal del cuadro:
			int m = limitar(blancos,n);
			//Matrices para almacenar los beneficios y las decisiones para la soluci�n:
			double ** V = new double * [m+1];
			int ** A = new int * [m+1];
			for(int x=0; x<(m+1); x++){
				V[x] = new double[n+1];
				A[x] = new int[n+1];
			}
			//Variables para calcular el tiempo de ejecuci�n:
			unsigned ti, tf;
			//C�lculo del tiempo inicial de ejecuci�n:
			ti = clock();
			//Ordenamiento de los blancos seg�n su posici�n:
			blancos = mergesort(blancos,n);
			//Inicializaci�n de la matriz de beneficios y la matriz auxiliar:
			beneficios(blancos,V,A,m+1,n+1);
			//Impresi�n de la soluci�n encontrada:
			imprimirSolucion(A,blancos,m,n,alto);
			cout << "\nEl �rea m�xima de da�o es: " << V[m][n] << endl;
			//C�lculo del tiempo final de ejecuci�n:
			tf = clock();
			//Impresi�n del tiempo total de ejecuci�n:
			double tiempoEjecucion = (double (tf-ti)/CLOCKS_PER_SEC);
			cout << "\nEl tiempo total de ejecuci�n es: " << tiempoEjecucion << endl;
			//Liberaci�n de la memoria reservada con los punteros:
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
