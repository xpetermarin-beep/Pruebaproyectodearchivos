// Pruebaproyectodearchivos.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//



#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ==================================================
// ESTRUCTURAS DE DATOS
// ==================================================

// ----- Producto -----
struct Producto {
    int codigo;
    string nombre;
    string categoria;
    double precio;
    bool disponible;
};

// ----- Pedido -----
struct Pedido {
    int numeroPedido;
    string nombreCliente;
    string nombreProducto;
    int cantidad;
    double precioUnitario;
    double total;
    string estado;
};

// ========== ARREGLO DINÁMICO (Productos) ==========
class ArregloDinamico {
private:
    Producto* datos;
    int capacidad;
    int tamano;
    void expandir() {
        capacidad *= 2;
        Producto* nuevo = new Producto[capacidad];
        for (int i = 0; i < tamano; i++) nuevo[i] = datos[i];
        delete[] datos;
        datos = nuevo;
    }
public:
    ArregloDinamico() { capacidad = 2; tamano = 0; datos = new Producto[capacidad]; }
    ~ArregloDinamico() { delete[] datos; }

    void agregar(Producto p) {
        if (tamano == capacidad) expandir();
        datos[tamano++] = p;
    }
    int cantidad() { return tamano; }
    Producto obtener(int i) { return datos[i]; }
    void modificar(int i, Producto p) { datos[i] = p; }
    Producto* puntero() { return datos; }
};

// ========== COLA (Pedidos Pendientes - FIFO) ==========
class Cola {
private:
    struct NodoCola {
        Pedido dato;
        NodoCola* siguiente;
        NodoCola(Pedido p) : dato(p), siguiente(nullptr) {}
    };
    NodoCola* frente;
    NodoCola* final;
public:
    Cola() { frente = final = nullptr; }
    ~Cola() { while (frente != nullptr) desencolar(); }

    void encolar(Pedido p) {
        NodoCola* n = new NodoCola(p);
        if (!frente) frente = final = n;
        else { final->siguiente = n; final = n; }
    }
    bool estaVacia() { return frente == nullptr; }
    Pedido verFrente() { return frente->dato; }
    Pedido desencolar() {
        Pedido p = frente->dato;
        NodoCola* temp = frente;
        frente = frente->siguiente;
        if (!frente) final = nullptr;
        delete temp;
        return p;
    }
    void mostrar() {
        NodoCola* act = frente;
        if (!act) { cout << "→ Sin pedidos pendientes\n"; return; }
        while (act) {
            cout << "  #" << act->dato.numeroPedido
                << " | " << act->dato.nombreCliente
                << " | " << act->dato.nombreProducto
                << " | Estado: " << act->dato.estado << "\n";
            act = act->siguiente;
        }
    }
};

// ========== LISTA ENLAZADA (Pedidos Procesados) ==========
class ListaEnlazada {
private:
    struct NodoLista {
        Pedido dato;
        NodoLista* siguiente;
        NodoLista(Pedido p) : dato(p), siguiente(nullptr) {}
    };
    NodoLista* inicio;
public:
    ListaEnlazada() { inicio = nullptr; }
    ~ListaEnlazada() { while (inicio) { NodoLista* t = inicio; inicio = inicio->siguiente; delete t; } }

    void agregar(Pedido p) {
        NodoLista* n = new NodoLista(p);
        if (!inicio) inicio = n;
        else { NodoLista* a = inicio; while (a->siguiente) a = a->siguiente; a->siguiente = n; }
    }
    void mostrar() {
        if (!inicio) { cout << "→ Sin pedidos procesados\n"; return; }
        NodoLista* a = inicio;
        while (a) {
            cout << "  #" << a->dato.numeroPedido
                << " | " << a->dato.nombreCliente
                << " | Total: ₡" << fixed << setprecision(2) << a->dato.total
                << " | " << a->dato.estado << "\n";
            a = a->siguiente;
        }
    }
    bool eliminar(int num) {
        NodoLista* ant = nullptr;
        NodoLista* act = inicio;
        while (act && act->dato.numeroPedido != num) {
            ant = act; act = act->siguiente;
        }
        if (!act) return false;
        if (!ant) inicio = act->siguiente;
        else ant->siguiente = act->siguiente;
        delete act;
        return true;
    }
};

// ========== PILA (Historial - LIFO) ==========
class Pila {
private:
    struct NodoPila {
        string mensaje;
        NodoPila* anterior;
        NodoPila(string m) : mensaje(m), anterior(nullptr) {}
    };
    NodoPila* tope;
public:
    Pila() { tope = nullptr; }
    ~Pila() { while (tope) { NodoPila* t = tope; tope = tope->anterior; delete t; } }

    void push(string m) {
        NodoPila* n = new NodoPila(m);
        n->anterior = tope;
        tope = n;
    }
    void mostrar() {
        if (!tope) { cout << "→ Historial vacío\n"; return; }
        NodoPila* a = tope;
        int i = 1;
        while (a) {
            cout << i << ". " << a->mensaje << "\n";
            a = a->anterior;
            i++;
        }
    }
};

// ========== ÁRBOL BINARIO DE BÚSQUEDA (Productos) ==========
class BST {
private:
    struct NodoArbol {
        Producto dato;
        NodoArbol* izq;
        NodoArbol* der;
        NodoArbol(Producto p) : dato(p), izq(nullptr), der(nullptr) {}
    };
    NodoArbol* raiz;

    void insertarRec(NodoArbol*& n, Producto p) {
        if (!n) n = new NodoArbol(p);
        else if (p.codigo < n->dato.codigo) insertarRec(n->izq, p);
        else insertarRec(n->der, p);
    }
    void inorden(NodoArbol* n) {
        if (!n) return;
        inorden(n->izq);
        cout << "  C" << n->dato.codigo << " - " << n->dato.nombre
            << " | ₡" << fixed << setprecision(2) << n->dato.precio << "\n";
        inorden(n->der);
    }
    bool buscarRec(NodoArbol* n, int cod) {
        if (!n) return false;
        if (n->dato.codigo == cod) {
            cout << "  Encontrado: " << n->dato.nombre << " | ₡" << fixed << setprecision(2) << n->dato.precio << "\n";
            return true;
        }
        if (cod < n->dato.codigo) return buscarRec(n->izq, cod);
        return buscarRec(n->der, cod);
    }
public:
    BST() { raiz = nullptr; }
    void insertar(Producto p) { insertarRec(raiz, p); }
    void mostrarInorden() { inorden(raiz); }
    bool buscar(int cod) { return buscarRec(raiz, cod); }
};

// ==================================================
// ALGORITMOS DE ORDENAMIENTO Y BÚSQUEDA
// ==================================================

// Ordenamiento: Bubble Sort implementado desde cero
void ordenarPorNombre(ArregloDinamico& arr) {
    int n = arr.cantidad();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr.obtener(j).nombre > arr.obtener(j + 1).nombre) {
                Producto temp = arr.obtener(j);
                arr.modificar(j, arr.obtener(j + 1));
                arr.modificar(j + 1, temp);
            }
        }
    }
}

// Búsqueda secuencial
int buscarSecuencial(ArregloDinamico& arr, int codigo) {
    for (int i = 0; i < arr.cantidad(); i++) {
        if (arr.obtener(i).codigo == codigo) return i;
    }
    return -1;
}

// ==================================================
// VARIABLES GLOBALES PARA PRUEBA
// ==================================================
ArregloDinamico listaProductos;
Cola colaPendientes;
ListaEnlazada listaProcesados;
Pila historial;
BST arbolProductos;
int contadorPedidos = 1;

// ==================================================
// FUNCIONES DEL MENÚ
// ==================================================

void agregarProducto() {
    Producto p;
    cout << "\n--- Agregar Producto ---\n";
    cout << "Código: "; cin >> p.codigo;
    cin.ignore();
    cout << "Nombre: "; getline(cin, p.nombre);
    cout << "Categoría: "; getline(cin, p.categoria);
    cout << "Precio: "; cin >> p.precio;
    cout << "Disponible (1=Sí, 0=No): "; cin >> p.disponible;

    listaProductos.agregar(p);
    arbolProductos.insertar(p);
    historial.push("Producto C" + to_string(p.codigo) + " - " + p.nombre + " agregado");
    cout << "✅ Producto registrado!\n";
}

void mostrarProductos() {
    cout << "\n--- Lista de Productos ---\n";
    if (listaProductos.cantidad() == 0) { cout << "Sin productos\n"; return; }
    for (int i = 0; i < listaProductos.cantidad(); i++) {
        Producto p = listaProductos.obtener(i);
        cout << "C" << p.codigo << " | " << p.nombre << " | " << p.categoria
            << " | ₡" << fixed << setprecision(2) << p.precio
            << " | " << (p.disponible ? "Disponible" : "No disponible") << "\n";
    }
}

void registrarPedido() {
    if (listaProductos.cantidad() == 0) {
        cout << "⚠️  Primero registra productos\n";
        return;
    }
    Pedido ped;
    ped.numeroPedido = contadorPedidos++;
    cout << "\n--- Registrar Pedido #" << ped.numeroPedido << " ---\n";
    cin.ignore();
    cout << "Nombre del cliente: "; getline(cin, ped.nombreCliente);
    cout << "Código del producto: ";
    int cod; cin >> cod;
    int idx = buscarSecuencial(listaProductos, cod);
    if (idx == -1) { cout << "❌ Producto no encontrado\n"; return; }
    Producto p = listaProductos.obtener(idx);
    ped.nombreProducto = p.nombre;
    ped.precioUnitario = p.precio;
    cout << "Cantidad: "; cin >> ped.cantidad;
    ped.total = ped.cantidad * ped.precioUnitario;
    ped.estado = "Pendiente";

    colaPendientes.encolar(ped);
    historial.push("Pedido #" + to_string(ped.numeroPedido) + " registrado - " + ped.nombreCliente);
    cout << "✅ Pedido registrado! Total: ₡" << fixed << setprecision(2) << ped.total << "\n";
}

void procesarPedido() {
    if (colaPendientes.estaVacia()) { cout << "\n⚠️  No hay pedidos pendientes\n"; return; }
    Pedido ped = colaPendientes.desencolar();
    ped.estado = "Entregado";
    listaProcesados.agregar(ped);
    historial.push("Pedido #" + to_string(ped.numeroPedido) + " ENTREGADO - " + ped.nombreCliente);
    cout << "\n✅ Pedido procesado:\n";
    cout << "  #" << ped.numeroPedido << " | " << ped.nombreCliente
        << " | " << ped.nombreProducto << " x" << ped.cantidad
        << " | Total: ₡" << fixed << setprecision(2) << ped.total << "\n";
}

void mostrarHistorial() {
    cout << "\n--- HISTORIAL DE OPERACIONES ---\n";
    historial.mostrar();
}

void mostrarArbol() {
    cout << "\n--- Árbol Binario (Inorden por código) ---\n";
    arbolProductos.mostrarInorden();
}

// ==================================================
// MENÚ PRINCIPAL
// ==================================================
int main() {
    int opcion;
    do {
        cout << "\n";
        cout << "========================================\n";
        cout << "       Sistema de Cafeteria \n";
        cout << "========================================\n";
        cout << "1. Agregar producto\n";
        cout << "2. Ver todos los productos\n";
        cout << "3. Ordenar productos por nombre (Bubble Sort)\n";
        cout << "4. Realizar busqueda de productos (búsqueda secuencial)\n";
        cout << "5. Registrar nuevo pedido\n";
        cout << "6. Ver pedidos pendientes\n";
        cout << "7. Procesar siguiente pedido\n";
        cout << "8. Ver pedidos procesados\n";
        cout << "9. Historial de operaciones (Pila - LIFO)\n";
        cout << "10. Ver árbol de productos\n";
        cout << "0. Salir\n";
        cout << "----------------------------------------\n";
        cout << "Seleccione opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1: agregarProducto(); break;
        case 2: mostrarProductos(); break;
        case 3: ordenarPorNombre(listaProductos); cout << "✅ Productos ordenados por nombre!\n"; mostrarProductos(); break;
        case 4: {
            cout << "Ingrese código a buscar: "; int c; cin >> c;
            int res = buscarSecuencial(listaProductos, c);
            if (res != -1) { Producto p = listaProductos.obtener(res); cout << "✅ " << p.nombre << " | ₡" << fixed << setprecision(2) << p.precio << "\n"; }
            else cout << "❌ No encontrado\n";
            break;
        }
        case 5: registrarPedido(); break;
        case 6: cout << "\n--- Pedidos Pendientes ---\n"; colaPendientes.mostrar(); break;
        case 7: procesarPedido(); break;
        case 8: cout << "\n--- Pedidos Procesados ---\n"; listaProcesados.mostrar(); break;
        case 9: mostrarHistorial(); break;
        case 10: mostrarArbol(); break;
        case 0: cout << "👋 Saliendo del sistema...\n"; break;
        default: cout << "❌ Opción inválida\n";
        }
    } while (opcion != 0);
    return 0;
}
