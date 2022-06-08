#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;

#define CV_RGB2GRAY 6
#define CV_COMP_BHATTACHARYYA 3


Mat gerar_Hist(Mat const imagem, int bins){
    int hist_Size[] = {bins}; 

    float lranges[] = {0, 256}; 
    const float* ranges[] = {lranges}; 

    Mat hist;
    int channels[]={0}; 


    calcHist(&imagem, 1, channels, Mat(), hist, 1, hist_Size, ranges, true, false);
    return hist;
}


int main(){
    Mat imagem, hist_presente, hist_passado; 
    int temp=0;
    float comparacao;
    VideoCapture cap; // Objeto com a função de capturar
    cap.open(0);
    if(!cap.isOpened()){
        cout << "cameras nao disponiveis";
        return -1;
    }

    cap >> imagem;
    hist_presente = gerar_Hist(imagem, 256);
    while(1){
        hist_presente.copyTo(hist_passado);
        cap >> imagem;
        cvtColor(imagem, imagem, CV_RGB2GRAY); //Imagem convertida para tons de cinza
        hist_presente = gerar_Hist(imagem, 256);

        comparacao = compareHist(hist_presente, hist_passado, CV_COMP_BHATTACHARYYA);
        if(comparacao >= 0.04) cout<<++temp<<" movimento detectado\n";

        imshow("webcam do notebook", imagem);
        if(waitKey(30) >= 0){ 
        	break;
        }
    }
    return 0;
}

