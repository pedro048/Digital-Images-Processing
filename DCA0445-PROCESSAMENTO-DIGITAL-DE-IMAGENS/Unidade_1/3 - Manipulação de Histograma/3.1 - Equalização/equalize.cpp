#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

#define CV_RGB2GRAY 6

/*
   Ao Receber uma imagem e a quantidade de barras do histograma desejado
   o programa retorna o histograma
 */
Mat gerar_Hist(Mat const imagem, int bins){
    int hist_Size[] = {bins};
    float lranges[] = {0, 256}; // Vetor de float com os valores lranges[0] = 0.0 e lranges[1] = 256.0
    const float* ranges[] = {lranges}; // Ponteiro para um vetor de float inicializado com o valor ranges[0] = &lranges
    Mat hist;
    int channels[]={0}; // Vetor de inteiros inicializado com channels[0] = 0 e só
    calcHist(&imagem, 1, channels, Mat(), hist, 1, hist_Size, ranges, true, false);
    return hist;
}

/**
 * Recebe um histograma e a quantidade de barras nele
 * Retorna uma imagem do histograma
 */
Mat3b hist_Imagem(Mat const hist, int bins){
    int const hist_height = 256;
    Mat3b hist_imagem = Mat3b::zeros(hist_height, bins); // Matriz na qual cada pixels tem 3 Bytes inicializada com 0 em todas posições
                                               //bins = largura do histograma
    double max_val = 0;
    minMaxLoc(hist, 0, &max_val);

    for(int b=0; b<256; b++){ // Imprimindo cada retângulo no fundo preto
        float const binVal = hist.at<float>(b);
        int const height = cvRound(binVal*hist_height/max_val);
        line(hist_imagem, Point(b, hist_height-height),
         Point(b, hist_height), Scalar::all(255));
    }
    return hist_imagem;
}


int main(){
    Mat imagem; // Imagem capturada pela webcam
    VideoCapture cap; // Objeto capturador

    cap.open(0);
    if(!cap.isOpened())
        return -1;

    while(1){
        cap >> imagem;
        cvtColor(imagem, imagem, CV_RGB2GRAY); // Transforma "imagem" numa imagem em escala de cinza
        Mat result_equalize;
    	equalizeHist(imagem, result_equalize); //aplicando a função de equalizar a imagem

    	imshow("Original", imagem); // Mostra original
    	imshow("Histograma do original", hist_Imagem(gerar_Hist(imagem, 256), 256)); // Cria o vetor histograma e depois a imagem dele

    	imshow("Imagem Equalizada", result_equalize);
    	imshow("Histograma da Equalizada", hist_Imagem(gerar_Hist(result_equalize, 256), 256));
        if(waitKey(30) >= 0){
         	break;
        }
    }
    return 0;
}
