#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

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

    // Ahora puedes utilizar los datos de las entidades como desees
    for (const auto& entidad : entidades) {
        cout << "Código de Entidad: " << entidad.codigo_entidad << endl;
        cout << "Nombre: " << entidad.nombre << endl;
        cout << "Tipo de Gestión: " << entidad.tipo_gestion << endl;
        // ...
        cout << endl;
    }

    return 0;
}
