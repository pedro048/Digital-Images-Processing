#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(){
    cv::Mat imagem;
    imagem = cv::imread("Bob.png",cv::IMREAD_GRAYSCALE); // Carrega a imagem Bob.png
    /*	Declaração de um retângulo: Rect retangulo(x,y, largura, altura);
       Declarando as regiões da imagem original
    */
    
    Mat resultado = imagem.clone();  //imagem é clonado para resultado, para apresentar o mesmo tamanho
    Mat S_esquer(imagem, Rect(0,0, imagem.cols/2, imagem.rows/2)); // Região superior esquerda
    Mat S_dir(imagem, Rect(imagem.cols/2, 0, imagem.cols/2, imagem.rows/2)); // Região superior direita
    Mat I_esquer(imagem, Rect(0, imagem.rows/2, imagem.cols/2, imagem.rows/2)); // Região inferior esquerda
    Mat I_dir(imagem, Rect(imagem.cols/2, imagem.rows/2, imagem.cols/2, imagem.rows/2)); // Região inferior direita


    I_dir.copyTo(resultado(Rect(0, 0, I_dir.cols, I_dir.rows))); // I_dir para S_esquer
    S_esquer.copyTo(resultado(Rect(imagem.cols/2, imagem.rows/2, S_esquer.cols, S_esquer.rows))); // S_esquer para I_dir
    I_esquer.copyTo(resultado(Rect(imagem.cols/2, 0, I_esquer.cols, I_esquer.rows))); // I_esquer -> S_dir
    S_dir.copyTo(resultado(Rect(0, imagem.rows/2, S_dir.cols, S_dir.rows))); // S_dir -> I_esquer

    imshow("Imagem Original", imagem);
    imshow("Regiões Trocadas", resultado);
    waitKey();
    
    return 0;
}
