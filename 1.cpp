#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <chrono>

using namespace std;
using json = nlohmann::json;

struct Entidad {
    string codigo_entidad;
    string nombre;
    string tipo_gestion;
    string estado_licenciamiento;
    string fecha_inicio_licenciamiento;
    string fecha_fin_licenciamiento;
    string periodo_licenciamiento;
    string departamento;
    string provincia;
    string distrito;
    string ubigeo;
    string latitud;
    string longitud;
    string fecha_corte;
};

void buscarPorDepartamento(const vector<Entidad>& entidades);
void ordenarPorPeriodoLicenciamiento(vector<Entidad>& entidades);
void mostrarEntidad(const Entidad& entidad);
void mostrarTiempoRestante(const Entidad& entidad);
pair<int, int> obtenerTiempoRestante(const string& fecha_fin);

int main() {
    ifstream file("./data.json");

    if (!file.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return 1;
    }

    json jsonData;

    try {
        file >> jsonData;
    } catch (json::parse_error& e) {
        cerr << "Error al parsear el archivo JSON: " << e.what() << endl;
        return 1;
    }

    vector<Entidad> entidades;

    for (const auto& objeto : jsonData) {
        Entidad entidad;

        entidad.codigo_entidad = objeto["CODIGO_ENTIDAD"];
        entidad.nombre = objeto["NOMBRE"];
        entidad.tipo_gestion = objeto["TIPO_GESTION"];
        entidad.estado_licenciamiento = objeto["ESTADO_LICENCIAMIENTO"];
        entidad.fecha_inicio_licenciamiento = objeto["FECHA_INICIO_LICENCIAMIENTO"];
        entidad.fecha_fin_licenciamiento = objeto["FECHA_FIN_LICENCIAMIENTO"];
        entidad.periodo_licenciamiento = objeto["PERIODO_LICENCIAMIENTO"];
        entidad.departamento = objeto["DEPARTAMENTO"];
        entidad.provincia = objeto["PROVINCIA"];
        entidad.distrito = objeto["DISTRITO"];
        entidad.ubigeo = objeto["UBIGEO"];
        entidad.latitud = objeto["LATITUD"];
        entidad.longitud = objeto["LONGITUD"];
        entidad.fecha_corte = objeto["FECHA_CORTE"];

        entidades.push_back(entidad);
    }

    file.close();

    int opcion;

    do {
        cout << "Menú de opciones:" << endl;
        cout << "1. Buscar por departamento" << endl;
        cout << "2. Ordenar por periodo de licenciamiento" << endl;
        cout << "3. Mostrar tiempo restante para finalizar el periodo de licenciamiento" << endl;
        cout << "4. Salir" << endl;
        cout << "Ingrese la opción deseada: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                buscarPorDepartamento(entidades);
                break;
            case 2:
                ordenarPorPeriodoLicenciamiento(entidades);
                break;
            case 3:
                for (const auto& entidad : entidades) {
                    mostrarTiempoRestante(entidad);
                }
                break;
            case 4:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opción inválida. Intente nuevamente." << endl;
                break;
        }

        cout << endl;
    } while (opcion != 4);

    return 0;
}

void buscarPorDepartamento(const vector<Entidad>& entidades) {
    string departamentoBuscado;
    cout << "Ingrese el nombre del departamento: ";
    cin >> departamentoBuscado;

    vector<Entidad> entidadesFiltradas;

    for (const auto& entidad : entidades) {
        if (entidad.departamento == departamentoBuscado) {
            entidadesFiltradas.push_back(entidad);
        }
    }

    if (entidadesFiltradas.empty()) {
        cout << "No se encontraron entidades en el departamento especificado." << endl;
        return;
    }

    cout << "Entidades encontradas en el departamento " << departamentoBuscado << ":" << endl;
    cout << string(50, '-') << endl;

    for (const auto& entidad : entidadesFiltradas) {
        mostrarEntidad(entidad);
        cout << string(50, '-') << endl;
    }
}

void ordenarPorPeriodoLicenciamiento(vector<Entidad>& entidades) {
    sort(entidades.begin(), entidades.end(), [](const Entidad& entidad1, const Entidad& entidad2) {
        return entidad1.periodo_licenciamiento < entidad2.periodo_licenciamiento;
    });

    cout << "Entidades ordenadas por periodo de licenciamiento:" << endl;
    cout << string(50, '-') << endl;

    for (const auto& entidad : entidades) {
        mostrarEntidad(entidad);
        cout << string(50, '-') << endl;
    }
}

void mostrarEntidad(const Entidad& entidad) {
    cout << "Código de Entidad: " << entidad.codigo_entidad << endl;
    cout << "Nombre: " << entidad.nombre << endl;
    cout << "Tipo de Gestión: " << entidad.tipo_gestion << endl;
    cout << "Estado de Licenciamiento: " << entidad.estado_licenciamiento << endl;
    cout << "Fecha de Inicio de Licenciamiento: " << entidad.fecha_inicio_licenciamiento << endl;
    cout << "Fecha de Fin de Licenciamiento: " << entidad.fecha_fin_licenciamiento << endl;
    cout << "Periodo de Licenciamiento: " << entidad.periodo_licenciamiento << endl;
    cout << "Departamento: " << entidad.departamento << endl;
    cout << "Provincia: " << entidad.provincia << endl;
    cout << "Distrito: " << entidad.distrito << endl;
    cout << "Ubigeo: " << entidad.ubigeo << endl;
    cout << "Latitud: " << entidad.latitud << endl;
    cout << "Longitud: " << entidad.longitud << endl;
    cout << "Fecha de Corte: " << entidad.fecha_corte << endl;
}

void mostrarTiempoRestante(const Entidad& entidad) {
    pair<int, int> tiempoRestante = obtenerTiempoRestante(entidad.fecha_fin_licenciamiento);
    int aniosRestantes = tiempoRestante.first;
    int mesesRestantes = tiempoRestante.second;

    if (aniosRestantes < 0 || mesesRestantes < 0) {
        cout << "Nombre: " << entidad.nombre << " - Licenciamiento vencido" << endl;
    } else {
        cout << "Nombre: " << entidad.nombre << " - Tiempo restante: ";
        if (aniosRestantes > 0) {
            cout << aniosRestantes << " año(s) ";
        }
        if (mesesRestantes > 0) {
            cout << mesesRestantes << " mes(es) ";
        }
        cout << endl;
    }
}

pair<int, int> obtenerTiempoRestante(const string& fecha_fin) {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    struct tm* now_tm = localtime(&now_time);

    int anio_fin = stoi(fecha_fin.substr(0, 4));
    int mes_fin = stoi(fecha_fin.substr(4, 2));

    int aniosRestantes = anio_fin - (now_tm->tm_year + 1900);
    int mesesRestantes = mes_fin - (now_tm->tm_mon + 1);

    if (mesesRestantes < 0) {
        mesesRestantes += 12;
        aniosRestantes--;
    }

    return make_pair(aniosRestantes, mesesRestantes);
}
