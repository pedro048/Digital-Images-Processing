#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

#define RADIUS 20
using namespace cv;
using namespace std;

#define CV_LOAD_IMAGE_GRAYSCALE 0
#define CV_WINDOW_FREERATIO 0x00000100
#define CV_MINMAX 32

double    gammaH, gammaL, c, D0;
int       gammaH_slider=20, gammaL_slider=2, c_slider=1, D0_slider=5;
const int gammaH_max=50,   gammaL_max=10,   c_max=100,  D0_max=200;

Mat padded, imagem, filtragem;


void deslocaDFT(Mat& imagem ){ // Realiza a troca de quadrantes da imagem (DFT)
  Mat tmp, A, B, C, D;

  /*
   No caso da imagem ter tamanho impar, essa bloco
   de código recorta a região para evitar cópias com
   tamanhos desiguais
   */
  imagem = imagem(Rect(0, 0, imagem.cols & -2, imagem.rows & -2));
  int cx = imagem.cols/2;
  int cy = imagem.rows/2;

  // Reorganização dos quadrantes da transformada
  A = imagem(Rect(0, 0, cx, cy));
  B = imagem(Rect(cx, 0, cx, cy));
  C = imagem(Rect(0, cy, cx, cy));
  D = imagem(Rect(cx, cy, cx, cy));

  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);// A <-> D
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B); // C <-> B
}

 /*
  Realiza a construção do Filtro Homomórfico 
  gl, gh, c, d0 são as variáveis do filtro 
*/
Mat cria_filtro(double gl, double gh, double c, double d0){

  Mat filtro = Mat(padded.size(), CV_32FC2, Scalar(0)), tmp = Mat(padded.size(), CV_32F);
  int dft_M = padded.rows, dft_N = padded.cols;

  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
      tmp.at<float> (i,j) = (gh - gl)*(1 - exp(-c*(( (i-dft_M/2)*(i-dft_M/2) + (j-dft_N/2)*(j-dft_N/2) ) / (d0*d0) ))) + gl;
    }
  }

  Mat comps[]= {tmp,tmp};
  merge(comps, 2, filtro);
  normalize(tmp,tmp,0,1,CV_MINMAX);
  imshow("filtro", tmp);
  return filtro;
}

Mat filtra(){
  Mat filtro, complexImage;
  Mat_<float> realInput, zeros;  
  vector<Mat> planos; 

  // identifica os tamanhos otimos para calculo do FFT
  int dft_M = getOptimalDFTSize(imagem.rows);
  int dft_N = getOptimalDFTSize(imagem.cols);

  // realiza o padding da imagem
  copyMakeBorder(imagem, padded, 0,
                 dft_M - imagem.rows, 0,
                 dft_N - imagem.cols,
                 BORDER_CONSTANT, Scalar::all(0));
  zeros = Mat_<float>::zeros(padded.size()); 
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0)); 
  filtro = complexImage.clone(); 
 
  Mat comps[]= {Mat(dft_M, dft_N, CV_32F), Mat(dft_M, dft_N, CV_32F)}; 
  merge(comps, 2, filtro);

  planos.clear(); 

  realInput = Mat_<float>(padded); 
  realInput += Scalar::all(1);
  log(realInput, realInput);
  planos.push_back(realInput); 
  planos.push_back(zeros); 
  merge(planos, complexImage); 

  dft(complexImage, complexImage); // Calcula a dft
  deslocaDFT(complexImage); // Realiza a troca de quadrantes
  resize(complexImage, complexImage, padded.size());
  normalize(complexImage, complexImage, 0, 1, CV_MINMAX);

  filtro = cria_filtro(gammaH, gammaL, c, D0);

  mulSpectrums(complexImage,filtro,complexImage,0); // Filtro frequencial sendo aplicado
  deslocaDFT(complexImage); // Troca os quadrantes novamente
  idft(complexImage, complexImage); // Calcula a dft inversa

  planos.clear(); 
  split(complexImage, planos); // Separa as partes (real e imaginaria) da imagem filtrada
  exp(planos[0], planos[0]);
  planos[0] -= Scalar::all(1);
  normalize(planos[0], planos[0], 0, 1, CV_MINMAX); // Realiza a normalização da parte real 
  return planos[0];
}

/*
  Ativado como reação a qualquer interação nos sliders
*/
void on_trackbar(int, void*){
  gammaH = (double)gammaH_slider/10.0; // gammaH = real, variando entre 0 e 5
  gammaL = (double)gammaL_slider/10.0; // gammaL = real, variando entre 0 e 1
  c = (double)c_slider; // c = real, variando entre 0 e 60
  D0 = (double)D0_slider;
  if(D0<1.0){
    D0=1.0;
  }
  if(c<1.0){
    c=1.0;
  }
  filtragem = filtra();
  imshow("filtragem", filtragem); 
  normalize(filtragem, filtragem, 0, 255, CV_MINMAX); 
  imwrite("Imagem_filtrada.png", filtragem);
}


int main(int , char**){
  char TrackbarName[50];

  vector<String> end;
  imagem = cv::imread("Janela_escura.jpg",cv::IMREAD_GRAYSCALE);
  resize(imagem, imagem, cv::Size(700, 800));  // Realiza o redimensionamento da imagem para 700x800
  imshow("Imagem_original", imagem);

  namedWindow("filtragem", CV_WINDOW_FREERATIO);

  sprintf(TrackbarName, "GammaH: %d", gammaH_max);
  createTrackbar(TrackbarName, "filtragem", &gammaH_slider, gammaH_max, on_trackbar);

  sprintf(TrackbarName, "GammaL: %d", gammaL_max);
  createTrackbar(TrackbarName, "filtragem", &gammaL_slider, gammaL_max, on_trackbar);

  sprintf(TrackbarName, "c: %d", c_max);
  createTrackbar(TrackbarName, "filtragem", &c_slider, c_max, on_trackbar);

  sprintf(TrackbarName, "D0: %d", D0_max);
  createTrackbar(TrackbarName, "filtragem", &D0_slider,D0_max, on_trackbar);
  waitKey();

  return 0;
}
