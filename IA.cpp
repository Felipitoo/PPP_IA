#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <map>

using namespace std;



void print_matrix(vector<vector<int>> matrix){
    for(int i = 0 ; i < matrix.size(); i++){
        for(int j = 0 ; j< matrix[0].size();j++){
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}


void print_vector(vector<int> vect){
    for(int i = 0 ; i < vect.size(); i++){
         cout << vect[i] << " ";
        cout << "\n";
    }
}



std::vector<std::pair<int,int>> sorted_crews(std::vector<int> trip, std::vector<int> caps){
    std::vector<std::pair<int, int> > vp; 
    std::vector<int> disponibilidad;
    std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());

    // Inserting element in pair vector 
    // to keep track of previous indexes 
    for (int i = 0; i < disponibilidad.size(); ++i) { 
        vp.push_back(std::make_pair(disponibilidad[i], i)); 
    } 
  
    // Sorting pair vector 
    std::sort(vp.begin(), vp.end(), std::greater<std::pair<int,int>>());

    return vp;
}

std::vector<std::pair<int,int>> sorted_crews_ascending(std::vector<int> trip, std::vector<int> caps){
    std::vector<std::pair<int, int> > vp; 
    std::vector<int> disponibilidad;
    std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());

    // Inserting element in pair vector 
    // to keep track of previous indexes 
    for (int i = 0; i < disponibilidad.size(); ++i) { 
        vp.push_back(std::make_pair(disponibilidad[i], i)); 
    } 
  
    // Sorting pair vector 
    std::sort(vp.begin(), vp.end());

    return vp;
}


vector<vector<int>> init_visitados(int invitados){
    vector<vector<int>> visitados(invitados);
    for ( int i = 0 ; i < invitados ; i++ ){
        visitados[i].resize(invitados);
    }

    for ( int i = 0 ; i < invitados ; i++ ){
        visitados[i][i] = 1;
    }

    return visitados;

}


// Retorna un 1 si rompe la condición de capacidad para asaginar un host y un 0 si no
int check_capacidad (vector<int> available, int tripulacion, int host){  

    //cout << "capacidad comp " << "disponible" << available[host] << " " << tripulacion << " host " << host << "\n" ;
    if( (available[host] - tripulacion) < 0){
        return 1;
    }
    else{
        return 0;
    }
}

// revisa si una tripulación vio la tripulacion de otro yate, retorna 1 si rompe la restricción, retorna 0 en el caso contrario
int check_ya_visito_host (vector<vector<int>> visitas, int visitante, int host){
    int flag = 0;
    if( visitas[visitante][host] == 1){
        return 1;
    }
    else{
        return 0;
    }
}

// revisa si un host visito a alguien siendo host, retorna 1 si rompe esto, retorna 0 en el caso contrario
 int check_es_host (vector<vector<int>> solucion , vector<int> host_periodo, int periodo){
     int contador = 0;
     for(int i = 0; i< host_periodo.size(); i++){
         if( (solucion[i][periodo] != -1) && (host_periodo[i] == 1) ){
             contador++;
         }
     }
     return contador;

 }


int determinar_min (vector<int> tripulaciones, int periodos, vector<pair<int,int>> disponibilidades_or){
    int total = 0;
    int pueden = 0;
    for (int i = 0; i < tripulaciones.size(); i++){
        total = total + tripulaciones[i];
    }
    int min_yates = 0;
    int j;
     while (j < disponibilidades_or.size()){
        pueden = pueden + disponibilidades_or[j].first;
        min_yates++;
        if(pueden > total){
            j = j + disponibilidades_or.size();
        }
        j++;
    }

    if (periodos < min_yates){


        return min_yates;
    }
    else{

        return periodos;
    }



}


map<int, vector<int>> init_relaciones(int yates){
    map<int, vector<int>> aux;
    for (int i = 0; i < yates; i++){
        aux[i] = {};
    }
    return aux;
}

vector<vector<int>> actualizar_visitados(vector<vector<int>> solucion, int periodos, int yates){
    vector<vector<int>> visitas;
    visitas = init_visitados(yates);
    int i,j,k,y,z;
    

    for(j = 0 ; j < periodos; j++){
        for(i = 0; i < yates ; i++){
            vector<int> encuentros;
            bool flag = false;
            for(k = 0; k < yates; k++){
                if(solucion[k][j] == i ){
                    flag = true;
                    encuentros.push_back(k);
                }
            }
            if(flag){
                encuentros.push_back(i);
            }
          
            for(auto fila : encuentros){
                for(auto columna : encuentros){
                    if( fila != columna){
                        visitas[fila][columna]++;
                    }
                }
            }
        }

    }
    
    return visitas;
    

}

vector<vector<int>> actualizar_capacidades(vector<vector<int>> solucion, int periodos, int yates, vector<int> disp, vector<int> trips){
    vector<vector<int>> capacidades_periodo(yates);
    int i,j;
    for(i = 0 ; i < yates; i++){
        capacidades_periodo[i].resize(periodos);

    }

    for(i = 0;i <yates; i++){
        for(j = 0;j < periodos; j++){
            capacidades_periodo[i][j] = -disp[i];
        }

    }
    
    for(j = 0;j < periodos; j++){
        for(i = 0 ; i < yates; i++){
            if (solucion[i][j] != -1){
                capacidades_periodo[solucion[i][j]][j] = capacidades_periodo[solucion[i][j]][j] + trips[i];
            } 
        }
    }

    return capacidades_periodo;
}

void forzar_restriccion_no_visito(vector<vector<int>>& solucion, vector<vector<int>>& dispo, int periodos, int yates, vector<int> trips){
    vector<vector<int>> host_periodos(periodos); 

    for(int j = 0; j < solucion[0].size(); j++){
        vector<int> hosts_periodo;
        for(int i = 0; i < solucion.size(); i++ ){
            if(std::find(hosts_periodo.begin(), hosts_periodo.end(), solucion[i][j]) != hosts_periodo.end()) {
                hosts_periodo.push_back(solucion[i][j]);
            }
        }
    }

     for (int j = 0; j < periodos ; j++){
        for(int i = 0; i < yates ; i++){
            if (host_periodos[j][i] == 1){
                solucion[i][j] = -1;
                dispo[i][j] = dispo[i][j] + trips[i];
            }
        }
    }

}


// Genera solucion greedy que relaja restriccion de dos invitados no pueden volver a verse
// La decision greedy es llenar siempre primero los barcos con mayor capacidad que no rompan el resto de las restricciones
vector<vector<int>> greedy(vector<int> trip, vector<int> caps, int periodos, int yate){
    
    vector<pair<int,int>> ordenadas;
    ordenadas = sorted_crews(trip,caps);

    int invitados = trip.size();
    vector<vector<int>> visitados(invitados);
    visitados = init_visitados(invitados);
    int yates = trip.size();

    vector<vector<int>> solucion_greedy(yates);

    for(int i = 0 ; i < yates; i++){
        solucion_greedy[i].resize(periodos,-1);
    }

    vector<vector<int>> host_periodos(periodos); 
    
    for(int j = 0; j < periodos; j++){
        vector<int> hosts(yates);
        vector<int> disponibilidad;
        std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());
        for(int i = 0; i < yates; i++){
            if (!hosts[i]){
                int host_candidato = 0;
                while(host_candidato < yates){
                    int cumplidas = 2;
                    cumplidas = cumplidas - check_capacidad(disponibilidad,trip[i],ordenadas[host_candidato].second);
                    cumplidas = cumplidas - check_ya_visito_host(visitados, i , ordenadas[host_candidato].second);
                    if (cumplidas == 2){
                        //cout << "eleji" << host_candidato << "\n";
                        visitados[i][ordenadas[host_candidato].second] = 1;
                        disponibilidad[ordenadas[host_candidato].second] = disponibilidad[ordenadas[host_candidato].second] - trip[i];
                        hosts[ordenadas[host_candidato].second] = 1;
                        solucion_greedy[i][j] = ordenadas[host_candidato].second;
                        host_candidato = host_candidato + yates;

                    }
                    host_candidato++;

                }
            }
        }
        host_periodos[j] = hosts;
    }

    for (int j = 0; j < periodos ; j++){
        for(int i = 0; i < yates ; i++){
            if (host_periodos[j][i] == 1){
                solucion_greedy[i][j] = -1;
            }
        }
    }
    return solucion_greedy;
}

int funcion_costos(vector<vector<int>> solucion_candidata, vector<vector<int>> disp_periodos, vector<vector<int>> visitados, int yates, int periodos){
    int costo ,i,j,init;
    int rotas_dispo, rotas_visitas;
    rotas_dispo = 0;
    rotas_visitas = 0;
    vector<int> aux;
    costo = 0;
    for(init = 0; init < yates; init++){
        aux.push_back(init);
    }
    vector<int> aux2;
    aux2 = aux;
    for(i = 0 ; i < aux.size() ; i++){
        aux2.erase(std::remove(aux2.begin(), aux2.end(), i), aux2.end());
        for(j = 0; j < aux2.size(); j++){
            if(visitados[aux[i]][aux2[j]] > 1){
                rotas_visitas = visitados[aux[i]][aux2[j]] - 1 ;
            }
        }
    }

    print_matrix(disp_periodos);

    for(j = 0 ; j < periodos ; j++){
        for(i = 0; i < yates; i++){
            if(disp_periodos[i][j] < 0){
                rotas_dispo = 0;
            }
        }
    }

   

    costo = rotas_dispo * 4 + rotas_visitas * 2 ;

    return costo;

}


void greedy_HC_peque(vector<int> trip, vector<int> caps, int periodos, int yates){
    int invitados = trip.size();
    vector<int> disponibilidad;
    std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());
    vector<vector<int>> matrix(invitados);
    for ( int i = 0 ; i < invitados ; i++ ){
        matrix[i].resize(periodos);
    }

    vector<pair<int,int>> ordenados; 
    
    vector<vector<int>> visitados_sc;
    vector<vector<int>> disponibles_sc;
    vector<vector<int>> solucion_candidata;
    vector<vector<int>> sol_inicial(invitados);
    solucion_candidata = greedy(trip,caps,periodos,yates);
 
    visitados_sc = actualizar_visitados(solucion_candidata,periodos,yates);
    disponibles_sc = actualizar_capacidades(solucion_candidata,periodos,yates,disponibilidad,trip);
    funcion_costos(solucion_candidata,disponibles_sc,visitados_sc,yates,periodos);



}

int check_capacidades(vector<int> trip, vector<int> caps){
        std::vector<int> disponibilidad;
        std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());

}


template <class Container>
void split2(const std::string& str, Container& cont, char delim = ';')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

template <class Container>
void split3(const std::string& str, Container& cont, char delim = ',')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

int main(){

    std::vector <std::vector<int>> matrix;
    std::ifstream infile("./Instancias PPP/Instancias CSPLib/Ian01.txt");
    std::string mytext;
    int yates;
    int i = 0 ;
    int periodos;
    std::vector<int> capacidades;
    std::vector<int> tripulaciones;
    std::vector<std::pair<int,int>> ordenadas;
            
    while(std::getline(infile, mytext)){
        if (i == 0){
            yates = std::stoi(mytext);
        }
        if (i == 1){
            periodos = std::stoi(mytext);
        }
        else if( i == 2){
            
            
            std::vector<std::string> words;
            split2(mytext, words);
            for(std::string value: words)  {
                std::vector<std::string> numeros;
                split3(value,numeros);
                std::stringstream geek(numeros[0]);
                int aux = 0;
                geek >> aux; 
                capacidades.push_back(aux);
                std::stringstream geek2(numeros[1]);
                geek2 >> aux; 
                tripulaciones.push_back(aux);
            }
        }
        i++;
    }

    /*
    vector<vector<int>> matrix();
    for ( int i = 0 ; i < RR ; i++ ){
        matrix[i].resize(CC);
    }*/
    ordenadas = sorted_crews(tripulaciones,capacidades);
    greedy_HC_peque(tripulaciones,capacidades,periodos,yates);
    
    return 0;
    }