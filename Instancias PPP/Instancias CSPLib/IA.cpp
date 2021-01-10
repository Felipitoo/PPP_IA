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


// Funcion para imprimir una matriz compuesta de valores enteros de dos dimensiones
void print_matrix(vector<vector<int>> matrix){
     for(int i = 0 ; i < matrix.size(); i++){
         for(int j = 0 ; j< matrix[0].size();j++){
             cout << matrix[i][j] << " ";
         }
         cout << "\n";
     }
}

// Funcion para imprimir un vector de valores enteros
void print_vector(vector<int> vect){
    for(int i = 0 ; i < vect.size(); i++){
        cout << vect[i] << " ";
        cout << "\n";
    }
}


// funcion para imprimir la solución en el formato solicitado
void print_solucion(vector<vector<int>> sol, vector<int> capacidades, vector<int> trips,int yates,int periodos){
    int i,j;
    vector<vector<int>> hosts_periodos(periodos);
    for(j = 0; j < periodos; j++){
        for(i = 0; i < yates; i++){
            if(sol[i][j] == -1){
                hosts_periodos[j].push_back(i);
            }
            
        }
    }

    for(int j = 0; j < periodos; j++){
        vector<vector<int>> orden(sol.size());
        cout << "Periodo " << j+1 << "\n" ;
        cout << "--------------" "\n";
        vector<int> usabilidad(yates);
        vector<int> hosts_per;
        vector<int> hosts = hosts_periodos[j];
        vector<int> invitados;
        for(i = 0; i < yates; i++){

            if( sol[i][j] != -1){
                orden[sol[i][j]].push_back(i);
                usabilidad[sol[i][j]] = usabilidad[sol[i][j]] + trips[i];
                invitados.push_back(i);
            }
          
             
        }


        for(i = 0; i < hosts.size(); i++){
            usabilidad[hosts[i]] = usabilidad[hosts[i]] + trips[hosts[i]];
        }

        for(i = 0; i < yates; i++){
            if(find(hosts.begin(), hosts.end(), i) != hosts.end()){
                cout << "Yate " << i << "->" ;
                cout << i ;
                for(int j = 0; j < orden[i].size(); j++){
                    cout << "," << orden[i][j] ;
                }
                cout << " [" << usabilidad[i] << "/" << capacidades[i] << "]";
                cout << "\n";   
            }
            if(find(invitados.begin(), invitados.end(), i) != invitados.end()){
                cout << "Yate " << i << " -> " << "0 \n" ;

            }     
        }
       
    cout << "\n";
    }
 

}





// funcion para ordenar las capacidades de los yates en orden descendente
std::vector<std::pair<int,int>> sorted_crews(std::vector<int> trip, std::vector<int> caps){
    std::vector<std::pair<int, int> > vp; 
    std::vector<int> disponibilidad;
    std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());

    for (int i = 0; i < disponibilidad.size(); ++i) { 
        vp.push_back(std::make_pair(disponibilidad[i], i)); 
    } 
  
    std::sort(vp.begin(), vp.end(), std::greater<std::pair<int,int>>());

    return vp;
}

std::vector<std::pair<int,int>> sorted_crews_ascending(std::vector<int> trip, std::vector<int> caps){
    std::vector<std::pair<int, int> > vp; 
    std::vector<int> disponibilidad;
    std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());


    for (int i = 0; i < disponibilidad.size(); ++i) { 
        vp.push_back(std::make_pair(disponibilidad[i], i)); 
    } 
  
    std::sort(vp.begin(), vp.end());

    return vp;
}

//funcion para inicializar la matriz que controla cuantas veces la tripulación de un yate vio a otra
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

// determino el un limite inferior de cuantos yates necesita una instancia cualquiera
int determinar_min (vector<int> tripulaciones, int periodos, vector<pair<int,int>> disponibilidades_or){
    int total = 0;
    int pueden = 0;
    for (int i = 0; i < tripulaciones.size(); i++){
        total = total + tripulaciones[i];
    }
    int min_yates = 0;
    int j = 0;
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


// determino el un limite inferior de cuantos yates necesita una instancia cualquiera en 1 solo periodo
int determinar_min_un_periodo (vector<int> tripulaciones, int periodos, vector<pair<int,int>> disponibilidades_or){
    int total = 0;
    int pueden = 0;
    for (int i = 0; i < tripulaciones.size(); i++){
        total = total + tripulaciones[i];
    }
    int min_yates = 0;
    int j = 0;
     while (j < disponibilidades_or.size()){
        pueden = pueden + disponibilidades_or[j].first;
        min_yates++;
        if(pueden > total){
            j = j + disponibilidades_or.size();
        }
        j++;
    }

    return min_yates;

}




// determina el total de tripulaciones
int determinar_total (vector<int> tripulaciones, int periodos, vector<pair<int,int>> disponibilidades_or){
    int total = 0;
    for (int i = 0; i < tripulaciones.size(); i++){
        total = total + tripulaciones[i];
    }


    return total;

}
// funcion utilizada en el reinicio del HC para asignar diferentes hosts en cada periodo de tiempo
vector<vector<int>> actualizar_hosts(vector<int> trip,vector<int> caps,int periodos, vector<int> disponibilidades){
    vector<pair<int,int>> disponibilidades_or = sorted_crews(trip,caps);
    vector<vector<int>> designados(periodos);
    int min_yates_solucion = determinar_min(trip,periodos,disponibilidades_or);
    vector<int> a_usar;
    int aux;


    int total = determinar_total(trip,periodos,disponibilidades_or);
    for(int i=0; i < min_yates_solucion;i++ ){
        a_usar.push_back(disponibilidades_or[i].second);
    }

    for(int j = 0 ; j < periodos; j++){
        aux = 0;
        while(aux < total){
            int Random = rand() % a_usar.size();
            int random_host = a_usar[Random];
            if(find(designados[j].begin(), designados[j].end(), random_host) == designados[j].end()){
                aux = aux + disponibilidades[random_host];
                designados[j].push_back(random_host);
            }
        }

    }
    return designados;
    

}


//genera la solucion inicial utilizada en el restart del HC
vector<vector<int>> solucion_restart_especial(int periodos, int yates, vector<pair<int,int>> disp,vector<int> caps, vector<int> trips, vector<vector<int>> hosts_designa){

    vector<vector<int>> solucion_random(yates);
    vector<int> hosts;
    int i,j;

    for(i = 0; i < yates; i++){
        solucion_random[i].resize(periodos);
    }    
    for(j = 0; j < periodos;j++){
        for(i = 0 ; i < yates ; i++){
            if(find(hosts_designa[j].begin(), hosts_designa[j].end(), i) != hosts_designa[j].end()){
                solucion_random[i][j] = -1;
            }
            else{
                int Random = rand() % hosts_designa[j].size();
                int random_host = hosts_designa[j][Random];
                solucion_random[i][j] = random_host;
            }
        }
    }




    return solucion_random;



}




// cuenta cuantas veces una tripulacion vio a otra
vector<vector<int>> actualizar_visitados(vector<vector<int>> solucion, int periodos, int yates){
    vector<vector<int>> visitas;
    visitas = init_visitados(yates);
    int i,j,k;
    
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

// determina la sobrecarga de un host segun el periodo en que este (para el caso de todos son hosts/invitados) verifica en una matriz 
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




// Genera solucion greedy que relaja restriccion de dos invitados no pueden volver a verse
// La decision greedy es llenar siempre primero los barcos con menor capacidad que no rompan el resto de las restricciones
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


// funcion de costos para el caso que todos pueden ser host e invitados intercambiando
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
                rotas_visitas = rotas_visitas + visitados[aux[i]][aux2[j]] - 1 ;
            }
        }
    }


    for(j = 0 ; j < periodos ; j++){
        for(i = 0; i < yates; i++){
            if(disp_periodos[i][j] > 0){
                
                rotas_dispo = rotas_dispo + 1 + ((disp_periodos[i][j] - 1)*0.25);
            }
        }
    }

   

    costo = rotas_dispo * 4 + rotas_visitas * 2 ;


    return costo;

}


// busca el mejor de los vecinos de una solucion candidata entregada
vector<vector<int>> vecindario_mm(vector<vector<int>> solucion, vector<int> disponibilidad, vector<int> trip,vector<vector<int>> designados) {
    int costo_vecino ;

    int mejor_costo = 9999999;
    vector<vector<int>> mejor_vecino,visitados_sc,disponibles_sc;
    for(int periodo = 0; periodo < solucion[0].size(); periodo++){
        for(int invitado = 0; invitado < solucion.size(); invitado++){
            for(auto yate_designa : designados[periodo] ){
                if(yate_designa != invitado){
                    vector<vector<int>> copia_solucion = solucion;
                    copia_solucion[invitado][periodo] = yate_designa;
                    visitados_sc = actualizar_visitados(copia_solucion,solucion[0].size(),solucion.size());
                    disponibles_sc = actualizar_capacidades(copia_solucion,solucion[0].size(),solucion.size(),disponibilidad,trip);
                    costo_vecino = funcion_costos(copia_solucion,disponibles_sc,visitados_sc,solucion.size(),solucion[0].size());

                    if (costo_vecino < mejor_costo){
                        mejor_costo = costo_vecino;
                        mejor_vecino = copia_solucion;
                    }
                }

           
  

            }
        }
    }
    return mejor_vecino;
}


void greedy_HC_peque(vector<int> trip, vector<int> caps, int periodos, int yates){
    int invitados = trip.size();

    int restarts = 0;
    vector<int> disponibilidad;
    std::transform(caps.begin(), caps.end(), trip.begin(), std::back_inserter(disponibilidad), std::minus<int>());
    vector<vector<int>> matrix(invitados);
    for ( int i = 0 ; i < invitados ; i++ ){
        matrix[i].resize(periodos);
    }

    vector<pair<int,int>> ordenados; 
    
    vector<vector<int>> visitados_sc,mejor_solucion,solucion_greedy,visitados_s_n,disponibles_s_n,visitados_s_b,disponibles_s_b;
    vector<vector<int>> disponibles_sc;
    vector<vector<int>> solucion_candidata;
    vector<vector<int>> sol_inicial(invitados);

    solucion_greedy = greedy(trip,caps,periodos,yates);
    

    ordenados = sorted_crews(trip,caps);

    vector<vector<int>> hosts_designados;


    while (restarts < 250 ){
        int mejora = 1;
        vector<vector<int>> s_n;
        hosts_designados = actualizar_hosts(trip,caps,periodos,disponibilidad) ;
        if (restarts != 0){
            solucion_candidata = solucion_restart_especial(periodos,yates,ordenados,caps,trip,hosts_designados);
        }
        else{
            solucion_candidata = solucion_greedy;
        }
        while(mejora == 1){
            visitados_sc = actualizar_visitados(solucion_candidata,periodos,yates);
            disponibles_sc = actualizar_capacidades(solucion_candidata,periodos,yates,disponibilidad,trip);
            int costo_sc = funcion_costos(solucion_candidata,disponibles_sc,visitados_sc,yates,periodos);

            // obtengo el costo de la solucion s_n
            s_n = vecindario_mm(solucion_candidata,disponibilidad,trip,hosts_designados);
            visitados_s_n = actualizar_visitados(s_n,periodos,yates);
            disponibles_s_n = actualizar_capacidades(s_n,periodos,yates,disponibilidad,trip);
            int costo_n = funcion_costos(s_n,disponibles_s_n,visitados_s_n,yates,periodos);
            
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
            visitados_s_b = actualizar_visitados(mejor_solucion,periodos,yates);
            disponibles_s_b = actualizar_capacidades(mejor_solucion,periodos,yates,disponibilidad,trip);
            int costo_s_b = funcion_costos(mejor_solucion,disponibles_s_b,visitados_s_b,yates,periodos);
        

            visitados_sc = actualizar_visitados(solucion_candidata,periodos,yates);
            disponibles_sc = actualizar_capacidades(solucion_candidata,periodos,yates,disponibilidad,trip);
            int costo_sc = funcion_costos(solucion_candidata,disponibles_sc,visitados_sc,yates,periodos);
        
        
            if(costo_sc < costo_s_b){
                mejor_solucion = solucion_candidata;
            }
        }
        restarts++;
    }
    //visitados_s_b = actualizar_visitados(mejor_solucion,periodos,yates);
    //disponibles_s_b = actualizar_capacidades(mejor_solucion,periodos,yates,disponibilidad,trip);
    //int costo_s_b = funcion_costos(mejor_solucion,disponibles_s_b,visitados_s_b,yates,periodos);
    print_solucion(mejor_solucion,caps,trip,yates,periodos);
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
    std::ifstream infile("./Ian10.txt");
    std::string mytext;
    int yates = 0;
    int i = 0 ;
    int periodos = 0;
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

 
    ordenadas = sorted_crews(tripulaciones,capacidades);
    greedy_HC_peque(tripulaciones,capacidades,periodos,yates);
    
    return 0;
    }