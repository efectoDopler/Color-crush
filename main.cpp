/* Jose Escudero Gómez 1º C */

#include <iostream>

using namespace std;
 
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <Windows.h>
#include <string>


const int N = 8; //Ancho y largo del tablero

const int INTENTOS = 15; //Intentos maximos del juego


//Fichas disponibles en el juego

typedef enum {rosa, verde, azul, amarillo, neutro} tFicha;

//Colores disponibles para las fichas del juego

typedef enum {
	black, dark_blue, dark_green, dark_cyan,  
	dark_red, dark_magenta, dark_yellow, light_gray,  
	dark_gray, light_blue, light_green, light_cyan, 
	light_red, light_magenta, light_yellow, white 
} tColor;

//Direcciones posibles para desplazar la ficha seleccionada

typedef enum {A, B, I, D} tDireccion;

//Tablero de juego Color Crush

typedef tFicha tTablero [N][N];

typedef bool tTableroAux [N][N];

//Controla todos los datos utiles para el jugador del juego

typedef struct{

	tTablero tablero;
	int intentos;
	int puntos;
	string usuario;
	int puntuacionAcumulada;
} tJuego;

//Controla todos los datos cuando el jugador mueve una ficha

typedef struct {

	tDireccion direccion;
	int fila;
	int columna;
} tMovimiento;

//Crear un tablero NxN con elementos aleatorios

void generarTablero(tJuego &juego);

//Juego Color Crush

void Partida(tJuego &juego);

//Funcion que pinta char

void setColor(tColor color);

//Selecciona el color a pintar segun la ficha que sea

void PintarCasilla(tFicha ficha);

//Muestra el tablero por pantalla

void mostrarTablero(tJuego juego);

//Muestra la cabeza del tablero dentro de mostrarTablero

void mostrarCabezaTablero();

//Muestra el cuerpo del tablero dentro de mostrarTablero

void mostrarCuerpoTablero();

//Muestra el "culo" del tablero dentro de mostrarTablero

void mostrarFinalTablero();

//Comprueba que el movimiento  y las coordenadas introducidas son correctas

bool movimientoValido(tMovimiento movimiento);

void procesarTablero(tJuego &juego);

/*Genera un tablero boleano[N][N] que servira para ver posiciones neutras 
a colocar en el tablero de juego con grupos de fichas del mismo color*/

void generarAux(tTableroAux &tableroAux);

//Busca grupos de fichas del mismo color en todas las posiciones posibles

void mirarCombinaciones(tJuego &juego,tTableroAux &tableroAux);

//Posibles lugares donde esta un grupo de fichas del mismo color

void procesarArriba(tJuego &juego,tTableroAux &tableroAux,int i, int j);
void procesarDerecha(tJuego &juego,tTableroAux &tableroAux,int i, int j);

/*Dado el tablero de arrays coloca un color neutro 
en el tablero de juego si su pos del array aux es true*/

void crearNeutro(tJuego &juego,tTableroAux tableroAux);

/*Tras procesar el tablero hace caer las fichas en 
el espacio vacio consecutivo y genera una nueva*/

void caidaFichas(tTablero &tablero);

//Calcula el numero maximo de vacios que hay en una columna

int calcularVacios(tTablero tablero);

void moverFichas(tJuego &juego, tMovimiento movimiento);

//Muestra la puntuacion de todos los jugadores guardados en un txt

void puntuacion();

//Carga el usuario del txt o lo crea nuevo

void usuarios(tJuego &juego,string nombre);

void guardar(tJuego juego);

//Guardamos partida en un txt

void guardarTablero(tJuego juego);

/*Carga partidas o tableros de un txt*/
void cargarPartidas(tJuego &juego);
void cargarTablero(tJuego &juego, string nombreFichero);

//Se encarga de que se desplacen las fichas solo si forman conjunto
void desplazamientoFichas(tJuego &juego, tMovimiento movimiento);


int main() {

	tJuego juego;
	string nombre;

	srand((unsigned int)time(0)); 

	do{
		cout << "Login: ";
		cin >> nombre;

		if(nombre == "XXX"){
			cout << "Nombre no valido!" <<endl;
		}
		else{
			usuarios(juego,nombre);
		}
	}while(nombre == "XXX");
	
	cargarPartidas(juego);
	Partida(juego);


	return 0;
}

void generarTablero(tJuego &juego){

	juego.puntos = 0;
	juego.intentos = INTENTOS;

	for(int fila = 0; fila < N; fila++){
		for(int columna = 0; columna < N; columna++){
			juego.tablero[fila][columna] = tFicha((rand() % 4));
		}
	}
}
void Partida(tJuego &juego) {

	char direccion;
	tMovimiento movimiento;
	bool error, salir = false, noIntentos = false;

	mostrarTablero(juego);
	procesarTablero(juego);
	
	do{
		
		error = false;
		cout << " Fila, Columna y letra de direccion";
		cout << "(A, B, I o D, 0 para cancelar," <<endl;
		cout << " M mostrar puntuaciones, G guardar partida y C para cargar tablero): " << endl;
		cout << " Direccion: "; cin >> direccion;		
		//En caso de que la direccion introducida este en minuscula

		if(direccion > char('Z')) direccion -= char('z'-'Z');

		if(direccion == '0') {
			
			cout << "Gracias por haber jugado" << endl;
			juego.puntuacionAcumulada = juego.puntuacionAcumulada + juego.puntos;
			guardar(juego);
			salir = true;
		}


		else if (direccion == 'G') {
			guardarTablero(juego);
			cout << " Partida guardada con exito!" << endl;
		}

		else if(direccion == 'M'){
			guardar(juego);
			puntuacion();
		}

		else if(direccion == 'C') {
			cargarPartidas(juego);
			mostrarTablero(juego);
			procesarTablero(juego);
		}

		else {

			cout << " Fila: ";  cin >> movimiento.fila; 
			cout << " Columna: "; cin >> movimiento.columna;
		
			switch(direccion) {

				case 'A': movimiento.direccion = A; break;

				case 'B': movimiento.direccion = B; break;

				case 'I': movimiento.direccion = I; break;

				case 'D': movimiento.direccion = D; break;


				default: {
					error = true;
					cout << "Opcion no valida" << endl;
						 } break;
			}

			if(!movimientoValido(movimiento)||(error)) {
				cout << " Movimiento imposible de realizar";
				cout << endl << endl;
			}
			
			else {
				if(juego.intentos == 0) noIntentos = true;

				else{
					desplazamientoFichas(juego, movimiento);
					juego.intentos--;
				}
			}

			procesarTablero(juego);
		}
		

	}while((!noIntentos)&&(!salir));
}
bool movimientoValido(tMovimiento movimiento) {

	int fila, columna;
	bool valido = true;

	columna =  movimiento.columna-1;
	fila = N - movimiento.fila;

	//Compruebo que no se hallan introducido unas coordenadas errorneas desde el principio
	if( (fila < 0 || fila > N)) valido = false;
	if(columna < 0 || columna > N ) valido = false;

	switch(movimiento.direccion) {

		case A: fila--; break;

		case B: fila++; break;

		case I: columna--; break;

		case D: columna++; break;

	}
	//Compruebo que las coordenadas no salgan fuera del tablero debido a un movimiento erroneo
	if( fila < 0 || fila > N ) valido = false;
	if( columna < 0 || columna > N ) valido = false;

	return valido;
}
void setColor(tColor color){

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(handle, color); 
}
void PintarCasilla(tFicha ficha){

	switch(ficha){
	
		case rosa: setColor(light_magenta);break;

		case verde: setColor(light_green);break;

		case azul: setColor(light_blue);break;

		case amarillo: setColor(light_yellow);break;

		case neutro: setColor(black);break;
	}
}
void mostrarTablero(tJuego juego){

	int num = N;

	Sleep(500);
	system("cls");

	cout  << endl << "\t ";
	cout  << " Intentos: " << juego.intentos;
	cout <<"    Puntos: " << juego.puntos << endl;
	mostrarCabezaTablero();
	for(int fila = 0; fila <N; fila++){
		for(int columna = 0; columna < N; columna++){

		//Enumera las filas del tablero
			if(columna == 0){
				if( num < 10){
					cout << " " << num << " ";
				}
				else{
					cout << " " << num;
				}
				num--;
			}
			

			cout << char(179);
			PintarCasilla(juego.tablero[fila][columna]);
			cout << char(219)<< char(219)<< char(219); 
			setColor(white);
	
			if(columna == N-1){
				cout << char(179) << endl;
				
				if( fila != N-1){
					mostrarCuerpoTablero();
				}

				else{
					mostrarFinalTablero();
				}
			}
		}
	}
}
void mostrarCabezaTablero(){

	setColor(white);
	cout << "   " << char(218);

	for(int fila = 0; fila < N; fila++) {
		cout << char(196);
		cout << char(196);
		cout << char(196);

		if (fila<N-1){
			cout << char(194);
		}

		else {
			cout << char(191) << endl;
		}
	}
}
void mostrarCuerpoTablero(){

	cout << "   " << char (195);
	
	for(int fila = 0; fila < N; fila++){
		cout << char(196);
		cout << char(196);
		cout << char(196);
						
		if(fila <(N-1)){
			cout << char (197);
		}
		
		else{
			cout << char (180) << endl;
		}
	}
}
void mostrarFinalTablero(){

	cout << "   " << char(192);
					
	for(int fila = 0; fila < N; fila++) {	
		cout << char(196);
		cout << char(196);
		cout << char(196);

		if (fila<(N-1)){
			cout << char(193);
		}
						
		else {				
			cout << char(217) << endl;
		}
	}

	//Enumera las columnas del tablero
	cout << "   ";
	for(int columna = 0; columna < N; columna++){
		if( columna < 11){
			cout << "  " << columna+1 << " ";
		}
		else{
			cout << " " << columna+1 << " ";
		}
	}
	cout << endl;
}
void procesarTablero(tJuego &juego){

	int vacios, aux;
	tTableroAux tableroAux;

	do{

		aux = juego.puntos;
		generarAux(tableroAux);
		mirarCombinaciones(juego,tableroAux);
		crearNeutro(juego,tableroAux);
		vacios = calcularVacios(juego.tablero);
		mostrarTablero(juego);

		for(int i = 0 ; i < vacios; i++) {
			caidaFichas(juego.tablero);
			mostrarTablero(juego);
		}

	}while(aux != juego.puntos);
}
void generarAux(tTableroAux &tableroAux){

	for (int fila = 0; fila < N; fila++){
		for (int columna = 0; columna < N; columna++){
			tableroAux[fila][columna] = false;
		}
	}

}
void mirarCombinaciones(tJuego &juego,tTableroAux &tableroAux){

	for(int fila = 0; fila < N; fila++){
		for(int columna = 0; columna < N; columna++){
			if(fila >= 2){
				procesarArriba(juego,tableroAux,fila,columna);
			}
			if(columna < (N-2)){
				procesarDerecha(juego,tableroAux,fila,columna);
			}
		}
	}
}
void procesarArriba(tJuego &juego,tTableroAux &tableroAux,int i, int j){

	int posAux = i, contador = 1, contAux = 1;

	while(juego.tablero[i][j] == juego.tablero[i-1][j]){
		contador++;
		i--;
	}
	if(contador > 2){

		/*Cambiamos el tipo de ficha a neutro y actualizamos 
		el valor del tablero de arrays que controla posiciones*/

		do{
			tableroAux[posAux][j] = true;
			posAux--;
			contAux++;
		}while(contAux <= contador);

		//Actualizamos la puntuacion del jugador

		if(contador == 3){
			juego.puntos = juego.puntos + 3;
		}
		else if(contador == 4){
			juego.puntos = juego.puntos + 8;
		}
		else if (contador > 4){
			juego.puntos = juego.puntos + 15;
		}
	}
}
void procesarDerecha(tJuego &juego,tTableroAux &tableroAux,int i, int j){
		
	int posAux = j, contador = 1, contAux = 1;

	while (juego.tablero[i][j] == juego.tablero[i][j+1]){
		contador++;
		j++;
	}
	
	if(contador > 2){

		/*Cambiamos el tipo de ficha a neutro y actualizamos 
		el valor del tablero de arrays que controla posiciones*/

		do{
			tableroAux[i][posAux] = true;
			posAux++;
			contAux++;
		}while(contAux <= contador);

		//Actualizamos la puntuacion del jugador

		if(contador == 3){
			juego.puntos = juego.puntos + 3;
		}
		else if(contador == 4){
			juego.puntos = juego.puntos + 8;
		}
		else if (contador > 4){
			juego.puntos = juego.puntos + 15;
		}
	}

}
void crearNeutro(tJuego &juego,tTableroAux tableroAux){

	for(int fila = 0; fila < N; fila++){
		for(int columna = 0; columna < N; columna++){
			if(tableroAux[fila][columna] == true){
				juego.tablero[fila][columna] = neutro;
			}
		}
		
	}
}
void caidaFichas(tTablero &tablero) {
	
	for (int columna = 0; columna < N; columna++) {
		for (int fila = N-1; fila >= 0; fila--) {
			if(tablero[fila][columna] == neutro && tablero[fila-1][columna] != neutro && fila > 1) {
				tablero[fila][columna] = tablero[fila-1][columna];
				tablero[fila-1][columna] = neutro;
			}
			else if(tablero[1][columna] == neutro && tablero[0][columna] != neutro){
				tablero[1][columna] = tablero[0][columna];
				tablero[0][columna] = tFicha((rand() % 4));
			}
			
		}
		//En vez de un if utilizo un while por si el rand volviera a generar neutro otra vez
			while(tablero[0][columna] == neutro) {
				tablero[0][columna] = tFicha((rand() % 4));
			}
	}
}
int calcularVacios(tTablero tablero) {

	int masVacios, vacios, contador;
	masVacios = 0;
	for (int columna = 0; columna < N; columna++) {
		contador = 0;
		vacios = 0;
		while(contador < N) {
			if(tablero[contador][columna] == neutro){
				vacios++;
			}
			contador++;
		}
		if(masVacios < vacios) masVacios = vacios;
	}

	return masVacios;
}
void moverFichas(tJuego &juego, tMovimiento movimiento) {

	int fila = N-movimiento.fila, columna = movimiento.columna-1,contador = 1;
	//int i,j;
	tFicha fichaAux;
	bool ok = false;
	
	switch(movimiento.direccion) {

				case A: {
					fichaAux = juego.tablero[fila-1][columna];
					juego.tablero[fila-1][columna] = juego.tablero[fila][columna]; 
					juego.tablero[fila][columna] = fichaAux;
						} break;


				case B: {
					fichaAux = juego.tablero[fila+1][columna];
					juego.tablero[fila+1][columna] = juego.tablero[fila][columna]; 
					juego.tablero[fila][columna] = fichaAux;
						} break;

				case I: {
					fichaAux = juego.tablero[fila][columna-1];
					juego.tablero[fila][columna-1] = juego.tablero[fila][columna]; 
					juego.tablero[fila][columna] = fichaAux;
						} break;

				case D: {
					fichaAux = juego.tablero[fila][columna+1];
					juego.tablero[fila][columna+1] = juego.tablero[fila][columna]; 
					juego.tablero[fila][columna] = fichaAux;
						} break;

			}
}
void desplazamientoFichas(tJuego &juego, tMovimiento movimiento) {

	tTableroAux tableroAux;
	int fila = N-movimiento.fila, columna = movimiento.columna-1;
	int filaAux = fila, columnaAux = columna;
	/*Guardo la puntuacion porque se actualizara en mirarCombinaciones() y no es necesario que se actualice ahora*/
	int puntuacion = juego.puntos;

	/*Utilizo FilaAux y columnaAux para saber en que posicion estara la ficha que he desplazado*/
	/*y en fila y columna estara la ficha que se coloco en su lugar*/
	switch(movimiento.direccion) {

		case A: filaAux--; break;

		case B: filaAux++; break;

		case I: columnaAux--; break;

		case D: columnaAux++; break;

	}

	moverFichas(juego, movimiento);
	generarAux(tableroAux);
	mirarCombinaciones(juego,tableroAux);
	/*Vuelvo a colocar la puntuacion anterior a mirarCombinaciones()*/
	juego.puntos = puntuacion;

	/*Si la ficha que he desplazado y la ficha que se ha puesto en su lugar son false 
	entonces las vuelvo a colocar en su posicion inicial*/
	if((tableroAux[fila][columna]==false)&&(tableroAux[filaAux][columnaAux]==false)) {
		moverFichas(juego, movimiento);
	}
}
void puntuacion() {
	ifstream archivo;
	string texto;
	int puntos;

	archivo.open("puntuacion.txt");
	cout << "Usuario        Puntos" << endl;
	archivo >> texto;

	while(texto!= "XXX"){
		archivo >> puntos;
		cout <<" " << texto << "\t\t" << puntos << endl;
		archivo >> texto;
	}

	cout << endl;
	archivo.close();
}
void usuarios(tJuego &juego,string nombre) {
	ifstream archivo;
	string user;
	int puntos;

	archivo.open("puntuacion.txt");

	if(archivo.is_open()){
		archivo >> user;
		archivo >> puntos;

		while(user != "XXX" && user != nombre){
			archivo >> user;
			archivo >> puntos;
		}
		
		if(user == nombre){
			juego.usuario = nombre;
			juego.puntuacionAcumulada = puntos;
		}
		else if(user == "XXX"){
			juego.usuario = nombre;
			juego.puntuacionAcumulada = 0;
		}
	}

	else{
		cout << "Fallo a la hora de abrir el archivo txt!" << endl;
	}
	archivo.close();
}
void guardar(tJuego juego){
	ifstream archivo;
	ofstream archivo2;
	string aux;
	bool existe = false;
	int puntos;

	archivo.open("puntuacion.txt");
	archivo2.open("tmp.txt");

	
		archivo >> aux;

		while(aux != "XXX"){
			archivo >> puntos;

			if(aux != juego.usuario){
				archivo2 << aux;
				archivo2 << " ";
				archivo2 << puntos;
				archivo2 << endl;
			}

			else{
				archivo2 << juego.usuario;
				archivo2 << " ";
				archivo2 << juego.puntuacionAcumulada;
				archivo2 << endl;
				existe = true;
			}
			archivo >> aux;
		}
		if(!existe){
			archivo2 << juego.usuario;
			archivo2 << " ";
			archivo2 << juego.puntuacionAcumulada;
			archivo2 << endl;
		}
		archivo2 << "XXX";

		archivo.close();
		archivo2.close();

		archivo.open("tmp.txt");
		archivo2.open("puntuacion.txt");

		archivo >> aux;
		while(aux != "XXX"){

			archivo >> puntos;
			archivo2 << aux;
			archivo2 << " ";
			archivo2 << puntos;
			archivo2 << endl;
			archivo >> aux;
		}
		archivo2 << "XXX";

		archivo.close();
		archivo2.close();
}
void guardarTablero(tJuego juego) {

	ofstream archivoTab;
	archivoTab.open("tablero.txt");

	archivoTab << juego.intentos << endl;
	archivoTab << juego.puntos << endl;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			archivoTab << juego.tablero[i][j] << " "; 
		}
		archivoTab << endl;
	}

	archivoTab.close();
	cout << " El tablero ha sido guardado como tablero.txt" << endl;
}
void cargarPartidas(tJuego &juego) {

	string opcion, nombreFichero;

	cin.sync();

	cout << "Si deseas cargar un tablero en especifico introduce el nombre de su archivo." << endl;
	cout << "Si deseas cargar una partida anterior usa (tablero)." << endl;
	cout << "En caso contrario para generar un tablero aleatorio pulsa Enter: ";
	getline (cin,opcion);

	if (opcion == "\0") generarTablero(juego);

	else {
		opcion = opcion+".txt";
		nombreFichero = opcion;
		cargarTablero(juego, nombreFichero);
	}
}
void cargarTablero(tJuego &juego, string nombreFichero) {

	ifstream archivoTab;
	archivoTab.open(nombreFichero);

	int fichaAux;
	char aux;


	if (archivoTab.is_open()){
		if(nombreFichero == "tablero.txt") {
			archivoTab >> juego.intentos;
			archivoTab >> juego.puntos;
		}

		else {
			juego.intentos = INTENTOS;
			juego.puntos = 0;
		}

		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				archivoTab >> fichaAux;
				archivoTab.get(aux);
			
				switch(fichaAux) {
					case 0: juego.tablero[i][j] = rosa; break;
					case 1: juego.tablero[i][j] = verde; break;
					case 2: juego.tablero[i][j] = azul; break;
					case 3: juego.tablero[i][j] = amarillo; break;
					case 4: juego.tablero[i][j] = neutro; break;
				}

			}
		}
	}

	else cout << "El archivo no se ha podido abrir correctamente" << endl;

	archivoTab.close();
}
