#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <map>

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
    for (auto i: tripulaciones){
        std::cout << i << "\n";
    }
    /*std::cout << yates << "\n";
    std::cout << periodos << "\n";*/
    return 0;
    }