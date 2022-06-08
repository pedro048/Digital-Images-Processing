#include <opencv2/opencv.hpp>
#include <iostream>
#include <stack>
using namespace std;
using namespace cv;

 
void floodfill(Mat im, cv::Point p, int cor){
    stack<cv::Point> pilha;
    pilha.push(p); 
    int corAnt = im.at<uchar>(p);
    cv::Point temp; // Representa sempre o que está sendo processado.

    while(!pilha.empty()){ // Atualiza pilha
        temp = pilha.top(); // Recupera  topo
        pilha.pop(); // Remove topo

        if (im.at<uchar>(temp)==corAnt){
            im.at<uchar>(temp) = cor; // Recebe a nova cor
            if(temp.x >= 1)        pilha.push(cv::Point(temp.x-1, temp.y)); // Vizinho de cima
            if(temp.x+1 < im.rows) pilha.push(cv::Point(temp.x+1, temp.y)); // Vizinho de baixo
            if(temp.y >= 1)        pilha.push(cv::Point(temp.x,   temp.y-1)); // Vizinho da esquerda
            if(temp.y+1 < im.cols) pilha.push(cv::Point(temp.x,   temp.y+1)); // Vizinho da direita
        }
    }
}

/*
   Retira bolhas da bordas
 */
void filtraBordas(Mat im){
    for(int c=0; c<im.cols; c++){ // Percorre borda superior
        im.at<uchar>(0, c) = 255;
    }
    for(int c=0; c<im.cols; c++){ // Percorre borda inferior
        im.at<uchar>(im.cols-1, c) = 255;
    }
    for(int l=0; l<im.rows; l++){ // Percorre borda esquerda
        im.at<uchar>(l, 0) = 255;
    }
    for(int l=0; l<im.rows; l++){ // Percorre borda direita
        im.at<uchar>(l, im.rows-1) = 255;
    }
    floodfill(im, cv::Point(0,0), 0); // A borda de 'im' é pintada de preto 
}

int main(){
    
    cv::Mat imagem;
    cv::Mat original;
    imagem = cv::imread("bolhas.png",cv::IMREAD_GRAYSCALE); // Carrega a imagem bolhas.png
    original = cv::imread("bolhas.png",cv::IMREAD_GRAYSCALE);
    filtraBordas(imagem); // Tira as bolhas tocando a borda da imagem
    int qtdBolha=0, qtdBuraco=0;

    imshow("Sem bordas", imagem); // Mostra a imagem sem as bolhas nas bordas
    imwrite("sem_borda.png", imagem);
    for(int y=0; y<imagem.cols; y++){
        for(int x=0; x<imagem.rows; x++){
            if(imagem.at<uchar>(x, y)==255){
                qtdBolha++;
                floodfill(imagem, cv::Point(y, x), qtdBolha*5);
            }
        }
    }

    imshow("Original", original);
    imshow("Bolhas", imagem);
    imwrite("result_Bolhas.png", imagem);
    cout<<"Foram encontradas "<<qtdBolha<<" bolhas, ";

    floodfill(imagem, cv::Point(0,0), 255); /* Realiza a Pintura do fundo da imagem de branco, 
    						com o intuito de contar a quantida de buracos dentro de bohlas
    					     */

    for(int y=0; y<imagem.cols; y++){ // A matriz está sendo percorrida de coluna à coluna a partir da linha 0
        for(int x=0; x<imagem.rows; x++){
            if(imagem.at<uchar>(x, y)==0){ 
                qtdBuraco++;
                floodfill(imagem, cv::Point(y, x), qtdBuraco*10);
            }
        }
    }

    imshow("Buracos", imagem);
    imwrite("buracos.png", imagem);
    cout<<qtdBuraco<<" com buraco."<<endl;
    waitKey();

    return 0;
}
