#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <map>



std::vector<std::pair<int,int>  > sorted_crews(std::vector<int> trip, std::vector<int> caps){
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
    for (auto i: vp){
        std::cout << i.first << "," << i.second << "\n";
    }
    return vp;
}

/*
void sortArr(int arr[], int n) 
{ 
  
    // Vector to store element 
    // with respective present index 
    vector<pair<int, int> > vp; 
  
    // Inserting element in pair vector 
    // to keep track of previous indexes 
    for (int i = 0; i < n; ++i) { 
        vp.push_back(make_pair(arr[i], i)); 
    } 
  
    // Sorting pair vector 
    sort(vp.begin(), vp.end()); 
  
    // Displaying sorted element 
    // with previous indexes 
    // corresponding to each element 
    cout << "Element\t"
         << "index" << endl; 
    for (int i = 0; i < vp.size(); i++) { 
        cout << vp[i].first << "\t"
             << vp[i].second << endl; 
    } 
} */



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
    sorted_crews(tripulaciones,capacidades);
    /*std::cout << yates << "\n";
    std::cout << periodos << "\n";*/
    return 0;
    }