#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

/* A função recebimento(), realiza a solicitação de dois pontos ao usuário
e retorna um vetor com esses dois pontos recebidos. O superior esquerdo na pos. 0
e o inferior direito na posição 1.
*/

vector<Point> recebimento(){
    Point p1, p2; /* Esses são os pontos fornecidos pelo usuário. Inicialmente 
    		     não tem como saber qual é o superior esquerdo ou inferior 
    		     direito
    		  */
    vector<Point> p; // Armazena os pontos na ordem correta
    cout<<"Digite as coordenadas (X, Y) do primeiro ponto: ";
    cin>>p1.x>>p1.y;
    cout<<"Digite as coordenadas (X, Y) do segundo ponto: ";
    cin>>p2.x>>p2.y;

    if(p1.x < p2.x){ 		// Caso p1 seja o ponto superior esquerdo
        p.push_back(p1);
        p.push_back(p2);
    } else{ 			// Caso p1 seja o ponto inferior direito
        p.push_back(p2);
        p.push_back(p1);
    }
    return p;
} 			      

int main(){
    cv::Mat imagem;
    vector<Point> pontos;
    Point ini, fim; 
    imagem = cv::imread("Bob.png",cv::IMREAD_GRAYSCALE); // Carrega a imagem Bob.png
    pontos = recebimento();
    ini = pontos[0]; fim = pontos[1];

    cout<<"Ponto superior esquerdo:\n";
    cout<<ini<<endl;
    cout<<"Ponto inferior direito:\n";
    cout<<fim<<endl;

    for(int i=ini.x; i<fim.x; i++){
        for(int j=ini.y; j<fim.y; j++){
            imagem.at<uchar>(j,i) = 255 - imagem.at<uchar>(j,i);
        }
    }
    imshow("Resultado do Processamento", imagem);
    waitKey();

    return 0;
}


