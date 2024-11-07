#include <iostream>
#include <cstdlib>
using namespace std;

const int N = 4; // Número de vértices

//----------------------------- COLA SECUENCIAL --------------------------------

// Cola para la implementación de la función BEA
class cola{
    int* items;
    int pr;
    int ult;
    int cant;
    int maximo;
public:
    cola(int xmax=0): maximo(xmax){
        pr = 0;
        ult = 0;
        cant = 0;
        items = new int[maximo];
    }

    int vacia(void){
        return (cant == 0);
    }

    int insertar(int x){
        if (cant < maximo){
            items[ult] = x;
            ult = (ult+1)%maximo; //Lo hacemos de esta manera pensando en la cola almacenada en un espacio circular.
            cant++;
            return x;
        } else {
            return 0;
        }
    }

    int suprimir(void){
        int x;
        if (vacia()){
            cout << "Cola vacía" << endl;
            return 0;
        } else {
            x = items[pr];
            pr = (pr+1)%maximo;
            cant--;
            return x;
        }
    }
};
//----------------------------- GRAFO --------------------------------
// Representación enlazada

class celda{
    int vertice;
    celda *sig;
public:
    void cargar_vertice(int xvertice){
        vertice = xvertice;
    }

    int obtener_vertice(void){
        return vertice;
    }

    void cargar_sig(celda* xsig){
        sig = xsig;
    }

    celda* obtener_sig(void){
        return sig;
    }
};

class Grafo{
    celda** lista;
public:
    Grafo(){
        lista = new celda*[N];
        for(int i = 0; i < N; i++){
            lista[i]  = nullptr;
        }
    }

    // --- Inserción Lista de adyacencia (en orden ascendente, aunque no hace falta) ----------------------------------------
    void insertar(int i, int v) {
        celda* psl = new celda();
        psl->cargar_vertice(v);

        celda* cab = lista[i];

        // Caso en el que la lista está vacía o el nuevo vértice es menor que el actual cabecera
        if ((cab == nullptr) || (v < cab->obtener_vertice())) {
            psl->cargar_sig(cab);  // psl apunta a la antigua cabecera
            lista[i] = psl;  // Actualizar la cabecera de la lista
        } else {
            celda* aux = cab;
            celda* anterior = nullptr;

            // Encontrar la posición correcta en orden ascendente
            while ((aux != nullptr) && (aux->obtener_vertice() < v)) {
                anterior = aux;
                aux = aux->obtener_sig();
            }

            // Insertar el nuevo nodo en la posición
            if (anterior != nullptr) {
                psl->cargar_sig(aux);  // El nuevo nodo apunta al siguiente nodo
                anterior->cargar_sig(psl);  // El nodo anterior apunta al nuevo nodo
            }
        }
    }

    //----------------------------- [Búsqueda en Profundidad] ---------------------------------------------
    void BEP(int d[], bool& es_DAG){
        // Variables iniciales
        for(int i = 0; i < N; i++){
            d[i] = 0; // Todos los nodos están sin descubrir
        }

        int tiempo = 0;

        // BEP
        for(int j = 0; j < N; j++){
            if (d[j] == 0){
                BEP_Visita(j, d, tiempo, es_DAG); // Si no ha sido visitado, lo visitamos
            }
        }
    }

    void BEP_Visita(int s, int d[], int& tiempo, bool& es_DAG){
        // Variables iniciales
        int u;
        // int f[N];

        tiempo++;
        d[s] = tiempo; // Almacenamos en d el tiempo en el que fue visitado (recordemos que el vértice coincide con el índice)
        for(int i = 0; i < N; i++){
            u = lista[s]->obtener_vertice();
            if(d[u] == 0){
                BEP_Visita(u, d, tiempo, es_DAG);
            } else {
                es_DAG = false;
            }
        }
        tiempo++;
        // f[s] = tiempo; // Se completa el tratamiento del nodo -> Preguntar como podríamos utilizarlo
    }

    //---------------------------------- [Búsqueda en Anchura] --------------------------------------------
    void BEA(int s, int d[], int* padre = nullptr){
        // Variables iniciales
        cola Cola(N);
        int v, u;

        // BEA
        for(int i = 0; i < N; i++){
            d[i] = 9999; // Ningún nodo ha sido marcado
            if (padre) { padre[i] = -1; }
        }
        d[s] = 0;
        Cola.insertar(s);
        while(!Cola.vacia()){
            v = Cola.suprimir();
            celda* cab = lista[v];
            while(cab != nullptr){
                u = cab->obtener_vertice();
                if(d[u] == 9999){
                    d[u] = d[v] + 1; // Marcamos el nodo, que contiene el mínimo de aristas que hay desde el nodo origen s hasta u
                    if (padre) { padre[u] = v; }
                    Cola.insertar(u);
                }
                cab = cab->obtener_sig();
            }
        }
    }
    //---------------------------------- [Camino] ----------------------------------------------
    void camino(int u, int v, int padre[]){
        if (u == v) {
            cout << u << " ";  // El camino es el mismo nodo
        }
        else if (padre[v] == -1) {
            cout << "No existe camino de " << u << " a " << v << endl;
        }
        else {
            camino(u, padre[v], padre);  // Llamada recursiva
            cout << v << " ";  // Imprime el nodo actual
        }
    }

    //---------------------------------- [CONEXO] ----------------------------------------------
    bool Conexo() {
        int d[N];
        int padre[N];
        BEA(0, d, nullptr);  // Iniciar la búsqueda desde el nodo 0

        // Comprobar si todos los nodos fueron alcanzados
        for (int i = 0; i < N; i++) {
            if (d[i] == 9999) {  // Si hay un nodo inalcanzable, no es conexo
                return false;
            }
        }

        return true;  // Todos los nodos fueron alcanzados, es conexo
    }

    //------------------------------------ [ACÍCLICO] --------------------------------------------//--------------------------------------------------------------------------------
    void Aciclico(bool es_DAG){
        if(!es_DAG){
            cout << "El grafo es cíclico";
        } else {
            cout << "El grafo es acíclico";
        }
    }

    //------------------------------------- [MOSTRAR LISTA DE ADYACENCIA] --------------------------------
    void mostrar_lista() {
        cout << "Lista de adyacencia:" << endl;
        for (int i = 0; i < N; i++) {
            cout << i << " -> ";
            celda* actual = lista[i];
            while (actual != nullptr) {
                cout << actual->obtener_vertice() << " ";
                actual = actual->obtener_sig();
            }
            cout << endl;
        }
    }
    //--------------------------------------------------------------------------------

};


int main(){
    Grafo mi_grafo;

    // --- [Inserciones] ---
    mi_grafo.insertar(0, 1);
    mi_grafo.insertar(0, 2);
    mi_grafo.insertar(3, 2);
    mi_grafo.insertar(1, 0);
    mi_grafo.insertar(2, 0);
    mi_grafo.insertar(2, 3);

    mi_grafo.mostrar_lista();

    //--------------------------------------------------------------------------------

    // --- [BEA] ---

    int d[N]; // Arreglo para marcar las distancia del nodo origen s al resto
    int padre[N]; // Almacena los vértices padres que formarán el camino de un nodo s (origen) a otro (destino).
    int origen = 1, destino = 2; // Definimos el origen y destino, para poder determinar el camino entre dichos nodos/vértices.

    mi_grafo.BEA(origen, d, padre);

    // --- [Camino] ---
    cout << "Camino de " << origen << " a " << destino << ": ";
    mi_grafo.camino(origen, destino, padre);
    cout << endl;

    //--------------------------------------------------------------------------------
    // --- [Conexidad] ---

    // -> Conexidad Simple
    bool conexo = mi_grafo.Conexo();
    if (conexo){
        cout << "El grafo es conexo" << endl;
    }

    //--------------------------------------------------------------------------------
    // --- [BEP] ---
    int e[N]; // Arreglo para marcar el tiempo en el que el nodo fue visitado
    bool es_DAG = false; // booleano para determinar si existe una "arista hacia atrás" según la clasificación de Heileman. Es decir, que nos lleva a un nodo ya visitado (bucle)
    mi_grafo.BEP(e, es_DAG);

    // --- [Acíclico] ---
    mi_grafo.Aciclico(es_DAG);

    //--------------------------------------------------------------------------------

    return 0;
}

