#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <map>
#include <set>
#pragma GCC diagnostic ignored "-Wsign-compare"


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

void print_solucion(vector<vector<int>> sol, vector<int> capacidades, vector<int> trips, vector<int> hosts,int yates,int periodos){
    int i;
    for(int j = 0; j < periodos; j++){
        vector<vector<int>> orden(sol.size());
        cout << "Periodo " << j+1 << "\n" ;
        cout << "--------------" "\n";
        vector<int> usabilidad(yates);
        vector<int> hosts_per;
        for(i = 0; i < yates; i++){

            if( sol[i][j] != -1){
                orden[sol[i][j]].push_back(i);
                usabilidad[sol[i][j]] = usabilidad[sol[i][j]] + trips[i];
            }
             
        }

        for(i = 0; i < hosts.size(); i++){
            usabilidad[hosts[i]] = usabilidad[hosts[i]] + trips[hosts[i]];
        }

        for(i = 0; i < hosts.size(); i++){
            cout << "Anfitrion " << hosts[i] << "->" ;
            cout << hosts[i] ;
            for(int j = 0; j < orden[hosts[i]].size(); j++){
                cout << "," << orden[hosts[i]][j] ;
            }
            cout << " [" << usabilidad[hosts[i]] << "/" << capacidades[hosts[i]] << "]";
            cout << "\n";        
        }
       
    cout << "\n";
    }
 

}



std::vector<std::pair<int,int>> sorted_tripulaciones(std::vector<int> trip, std::vector<int> caps, vector<int> invitados){
    std::vector<std::pair<int, int> > vp; 
    std::vector<int> disponibilidad;
    std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());
    
    for (int i = 0; i < invitados.size(); ++i) { 
        vp.push_back(std::make_pair(trip[invitados[i]], invitados[i])); 
    }
    std::sort(vp.begin(), vp.end(), std::greater<std::pair<int,int>>());
    return vp;
}


std::vector<std::pair<int,int>> asc_sorted_disponibilidades(std::vector<int> trip, std::vector<int> caps, vector<int> hosts){
    std::vector<std::pair<int, int> > vp; 
    std::vector<int> disponibilidad;
    std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());
    
    for (int i = 0; i < hosts.size(); ++i) { 
        vp.push_back(std::make_pair(disponibilidad[hosts[i]], hosts[i])); 
    }

    
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

    if( (available[host] - tripulacion) < 0){
        return 1;
    }
    else{
        return 0;
    }
}

// revisa si una tripulación vio la tripulacion de otro yate, retorna 1 si rompe la restricción, retorna 0 en el caso contrario
int check_ya_visito_tripulacion (vector<vector<int>> visitas, int visitante, int host){
    if( visitas[visitante][host] == 1){
        return 1;
    }
    else{
        return 0;
    }
}



// determino el un limite inferior de cuantos yates necesita una instancia cualquiera
int determinar_min (vector<int> tripulaciones, int periodos, vector<pair<int,int>> disponibilidades_or){
    int total = 0;
    int pueden = 0;
    for (int i = 0; i < tripulaciones.size(); i++){
        total = total + tripulaciones[i];
    }
    int min_yates = 0;
    int j = 0;
     while (
          disponibilidades_or.size()){
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


// cuenta cuantas veces una tripulacion vio a otra
vector<vector<int>> actualizar_visitados(vector<vector<int>> solucion, int periodos, int yates, vector<int> hosts, vector<int> invitados){
    vector<vector<int>> visitas;
    visitas = init_visitados(yates);
    int i,j,k;
    
    for(j = 0 ; j < periodos; j++){
        for(i = 0; i < hosts.size() ; i++){
            set<int> encuentros;
            bool flag = false;
            for(k = 0; k < invitados.size(); k++){
                if(solucion[invitados[k]][j] == hosts[i] ){
                    flag = true;
                    encuentros.insert(invitados[k]);
                }
            }
            if(flag){
                encuentros.insert(hosts[i]);
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





// determina la sobrecarga de un host segun el periodo en que este (para el caso de todos son hosts/invitados) verifica en una matriz , revisar para cuando los hosts son fijos
vector<vector<int>> actualizar_capacidades(vector<vector<int>> solucion, int yates, int periodos, vector<int> hosts, vector<int> disp, vector<int> trips,vector<int> invitados){
    vector<vector<int>> capacidades_periodo(yates);
    int i,j;
    for(i = 0 ; i < yates; i++){
        capacidades_periodo[i].resize(periodos,-1);

    }

    for(i = 0; i < hosts.size(); i++){
        for(j = 0;j < periodos; j++){
            capacidades_periodo[hosts[i]][j] = -disp[hosts[i]];
        }

    }
    
    for(j = 0;j < periodos; j++){
        for(i = 0 ; i < invitados.size(); i++){
            if(solucion[invitados[i]][j] != -1){
                capacidades_periodo[solucion[invitados[i]][j]][j] = capacidades_periodo[solucion[invitados[i]][j]][j] + trips[invitados[i]];
            }
        }
    }

    return capacidades_periodo;
}




// genera una solucion random para los yates habilitados como host
vector<vector<int>> solucion_random(int periodos, int yates, vector<int> hosts,vector<int> invitados){

    vector<vector<int>> solucion_random(yates);
    for(int i = 0; i < yates; i++){
        solucion_random[i].resize(periodos);
    }

    for(int i = 0; i < yates; i++){
        for(int j = 0; j < periodos; j++){
            if(find(hosts.begin(), hosts.end(), i) != hosts.end()){
                solucion_random[i][j] = -1;
                
            }
            else{
                int Random = rand() % hosts.size();
                int random_host = hosts[Random];
                solucion_random[i][j] = random_host;
            }
        }
    }

    return solucion_random;



}


// Genera solucion greedy que relaja restriccion de dos invitados no pueden volver a verse
// La decision greedy es llenar siempre primero los barcos con menor capacidad que no rompan el resto de las restricciones
vector<vector<int>> greedy(vector<int> trip, vector<int> caps, int periodos, int yates, vector<int> hosts, vector<int> invitados){
    
    vector<pair<int,int>> ordenadas;
    vector<pair<int,int>> ordenadas_inv;

    ordenadas = asc_sorted_disponibilidades(trip,caps,hosts);
    ordenadas_inv = sorted_tripulaciones(trip,caps,invitados);
    
    vector<vector<int>> visitados(yates);
    vector<vector<int>> hosts_visitados(yates);
    visitados = init_visitados(yates);
    hosts_visitados = init_visitados(yates);
    vector<vector<int>> solucion_greedy(yates);

    for(int i = 0 ; i < yates; i++){
        solucion_greedy[i].resize(periodos,-1);
    }

    
    for(int j = 0; j < periodos; j++){
        vector<int> hosts(yates);
        vector<int> disponibilidad;
        std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());
        for(int i = 0; i < invitados.size(); i++){
            int pos = 0;

            while(pos < hosts.size() ){
                int cumplidas = 2;
                cumplidas = cumplidas - check_capacidad(disponibilidad,trip[ordenadas_inv[i].second],ordenadas[pos].second);
                cumplidas = cumplidas - check_ya_visito_tripulacion(visitados, ordenadas_inv[i].second , ordenadas[pos].second);
                if (cumplidas == 2){
                    visitados[ordenadas_inv[i].second][ordenadas[pos].second] = 1;
                    disponibilidad[ordenadas[pos].second] = disponibilidad[ordenadas[pos].second] - trip[ordenadas_inv[i].second];
                    solucion_greedy[ordenadas_inv[i].second][j] = ordenadas[pos].second;
                    pos = pos + hosts.size();

                }
                pos++;

            }
            
        }
    }

  
    return solucion_greedy;
}


// funcion de costos para el caso que todos pueden ser host e invitados intercambiando
int funcion_costos(vector<vector<int>> solucion_candidata, vector<vector<int>> disp_periodos, vector<vector<int>> visitados, int yates, int periodos,vector<int> invitados, vector<int> hosts){
    int costo ,i,j,init;
    int rotas_dispo, rotas_visitas,rotas_hosts, sin_asignar;
    rotas_dispo = 0;
    rotas_visitas = 0;
    rotas_hosts = 0;
    sin_asignar = 0;
    vector<int> aux;
    costo = 0;

    for(init = 0; init < invitados.size(); init++){
        aux.push_back(invitados[init]);
    }
    vector<int> aux2;
    aux2 = aux;
    for(i = 0 ; i < aux.size() ; i++){
        aux2.erase(std::remove(aux2.begin(), aux2.end(), i), aux2.end());
        for(j = 0; j < aux2.size(); j++){
            if(visitados[aux[i]][aux2[j]] > 1){
                rotas_visitas = rotas_visitas + visitados[aux[i]][aux2[j]] - 1 ;
            }
        }
    }




    for(j = 0 ; j < periodos ; j++){
        for(i = 0; i < yates; i++){
            if(disp_periodos[i][j] > 0){
                
                rotas_dispo = rotas_dispo + 1 + ((disp_periodos[i][j] - 1)*0.5);
            }
        }
    }

    for(i = 0; i < invitados.size(); i++){
        set<int> diff_hosts;
        for(j = 0 ; j < periodos ; j++){
            if(solucion_candidata[invitados[i]][j] == -1){
                sin_asignar++;
            }
          
          
        }
    }
    
    for(i = 0; i < invitados.size(); i++){
        set<int> diff_hosts;
        for(j = 0 ; j < periodos ; j++){
            if(solucion_candidata[invitados[i]][j] != -1){
                diff_hosts.insert(solucion_candidata[invitados[i]][j]);
            }
            else{
                sin_asignar++;
            }
          
        }

        if( diff_hosts.size() != periodos){
            rotas_hosts = periodos - diff_hosts.size();
        }
 

    }
    


   

    costo = rotas_dispo * 8 + rotas_visitas * 3 + rotas_hosts * 3 + sin_asignar * 50;

    return costo;

}


// busca el mejor de los vecinos de una solucion candidata entregada
vector<vector<int>> vecindario_mm(vector<vector<int>> solucion, vector<int> hosts, vector<int> disponibilidad, vector<int> trip, vector<int> invitados,int periodos) {
    int costo_vecino;

    int mejor_costo = 9999999;
    vector<vector<int>> mejor_vecino,visitados_sc,disponibles_sc;
    for(int i = 0; i < hosts.size(); i++){
        for(int invitado = 0; invitado < invitados.size(); invitado++){
            for(int periodo = 0; periodo < periodos; periodo++){
                vector<vector<int>> copia_solucion = solucion;
                copia_solucion[invitados[invitado]][periodo] = hosts[i];
                visitados_sc = actualizar_visitados(copia_solucion,periodos,solucion.size(),hosts,invitados);
                disponibles_sc = actualizar_capacidades(copia_solucion,solucion.size(),periodos,hosts,disponibilidad,trip,invitados);
                costo_vecino = funcion_costos(copia_solucion,disponibles_sc,visitados_sc,solucion.size(),solucion[0].size(),invitados,hosts);


                if (costo_vecino < mejor_costo){
                    mejor_costo = costo_vecino;
                    mejor_vecino = copia_solucion;
                }
  

            }
        }
    }
    return mejor_vecino;
}


void greedy_HC_original(vector<int> trip, vector<int> caps, int periodos, int yates, vector<int> hosts, vector<int> invitados){

    int restarts = 0;
    vector<int> disponibilidad;
    std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());
   

    vector<pair<int,int>> ordenados; 
    
    vector<vector<int>> visitados_sc,mejor_solucion,solucion_greedy,visitados_s_n,disponibles_s_n,visitados_s_b,disponibles_s_b;
    vector<vector<int>> disponibles_sc;
    vector<vector<int>> solucion_candidata;

    solucion_greedy = greedy(trip,caps,periodos,yates,hosts,invitados);
    


    while (restarts < 250 ){
        int mejora = 1;
        vector<vector<int>> s_n;
        if (restarts != 0){
            solucion_candidata = solucion_random(periodos,yates,hosts,invitados);
        }
        else{
            solucion_candidata = solucion_greedy;
        }
        while(mejora == 1){

            visitados_sc = actualizar_visitados(solucion_candidata,periodos,yates,hosts,invitados);
            disponibles_sc = actualizar_capacidades(solucion_candidata,yates,periodos,hosts,disponibilidad,trip,invitados);
            int costo_sc = funcion_costos(solucion_candidata,disponibles_sc,visitados_sc,yates,periodos,invitados,hosts);
            
            s_n = vecindario_mm(solucion_candidata,hosts,disponibilidad,trip,invitados,periodos);
            visitados_s_n = actualizar_visitados(s_n,periodos,yates,hosts,invitados);
            disponibles_s_n = actualizar_capacidades(s_n,yates,periodos,hosts,disponibilidad,trip,invitados);
            int costo_n = funcion_costos(s_n,disponibles_s_n,visitados_s_n,yates,periodos,invitados,hosts);
            
            
            if (costo_n < costo_sc ){
                solucion_candidata = s_n ;
            }
            else{
                mejora = 0;
            }
        }

        if (restarts == 0){
            mejor_solucion = solucion_candidata;

        }
        
        else{
            visitados_s_b = actualizar_visitados(mejor_solucion,periodos,yates,hosts,invitados);
            disponibles_s_b = actualizar_capacidades(mejor_solucion,yates,periodos,hosts,disponibilidad,trip,invitados);
            int costo_s_b = funcion_costos(mejor_solucion,disponibles_s_b,visitados_s_b,yates,periodos,invitados,hosts);
        

            visitados_sc = actualizar_visitados(solucion_candidata,periodos,yates,hosts,invitados);
            disponibles_sc = actualizar_capacidades(solucion_candidata,yates,periodos,hosts,disponibilidad,trip,invitados);
            int costo_sc = funcion_costos(solucion_candidata,disponibles_sc,visitados_sc,yates,periodos,invitados,hosts);
        
        
            if(costo_sc < costo_s_b){
                mejor_solucion = solucion_candidata;
            }
        }
        restarts++;

    }



    //visitados_sc = actualizar_visitados(mejor_solucion,periodos,yates,hosts,invitados);
    //disponibles_sc = actualizar_capacidades(mejor_solucion,yates,periodos,hosts,disponibilidad,trip,invitados);
    //int costo_sc = funcion_costos(mejor_solucion,disponibles_sc,visitados_sc,yates,periodos,invitados,hosts);

    print_solucion(mejor_solucion,caps,trip,hosts,yates,periodos);
    
}



template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
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
    std::ifstream infile("./PPP.txt");
    std::string mytext;
    int yates = 0;
    int i = 0 ;
    int periodos = 0;
    std::vector<int> capacidades;
    std::vector<int> tripulaciones;
    std::vector<std::pair<int,int>> ordenadas;
    vector<int> hosts;
    vector<int> invitados;
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

    std::ifstream infile2("./config1.txt");
    while(std::getline(infile2, mytext)){            
        std::vector<std::string> words;
        split(mytext, words);
        for(std::string value: words)  {
            std::stringstream geek(value);
            int aux = 0;
            geek >> aux; 
            hosts.push_back(aux-1);
        }
    }
    
    for(i = 0; i < yates; i++){
        if(find(hosts.begin(), hosts.end(), i) != hosts.end()){
            hosts = hosts;
        }
        else{
            invitados.push_back(i);
        }
    }




    /*
    vector<vector<int>> matrix();
    for ( int i = 0 ; i < RR ; i++ ){
        matrix[i].resize(CC);
    }*/
    
    greedy_HC_original(tripulaciones,capacidades,periodos,yates,hosts,invitados);
    
    return 0;
    }