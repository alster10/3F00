#ifndef MAIN_H
#define MAIN_H


#include "Common.h"
#include "time.h"

HWND	hDialog;

HWND	hWindow;
extern HWND d3dHWNDsave;

HINSTANCE hInstanceSave;
bool imagesLoaded=false;
bool dispGenerated=false;

bool wire=false;
bool image=true;
bool changeImageDisp=false;//� modifica do em Main.cpp-DialogProc e em D3D9.cpp-OnFrameRender

// Matcher
Bitmap* Pad=0;
Bitmap* BitmapL=0;
Bitmap* BitmapR=0;
Bitmap* DispMap=0;

bool ImgBufferLoaded=false;
bool LoadOnlyDispMap=false;//indica que em paint() deve ser carregado o buffer voidDispMap e vari�veis globais necess�rias, pois o mapa de disparidades � carregado diretamente do arquivo sem chamar CorrespSearch()
void *voidImgL=0;
void *voidImgR=0;
void *voidDispMap=0;
void *voidDispMapLegacy=0;
unsigned short rangeSize=16;
unsigned short maxDisparity;
unsigned long globalWidth=0;
unsigned long globalHeight=0;
bool first=true;
WCHAR left[]=L"IM0.bmp";//"tsukubaL.bmp";//"AndreL25.bmp";//"vasoL.bmp";//"venusL.bmp";//
WCHAR right[]=L"IM5.bmp";//"tsukubaR.bmp";//"AndreR25.bmp";//"vasoR.bmp";//"venusR.bmp";//
WCHAR disparityMap[]=L"DisparityVenus.bmp";//"DispTsukubaLegacy.bmp";//"DisparityVaso.bmp";//

//***************************
bool compare=false;//true;//usado para comparar legado com nova stripping Technique
int windowSizeForCompare;//=9;//21;//
unsigned short MaxdispForCompare;//=20;//16;//75;//Max Disparity para a fun��o 

//novas vari�veis para compara��o e cronometragem em tempo de execu��o
clock_t startST, finishST,startT, finishT;//ST=Stripping Technique T=Tradicional
double  durationST,durationT;

//**************************

//variaveis globais para carga e tratamento das imagens

WCHAR GleftFile[256];
WCHAR GrightFile[256];
WCHAR GdisparityMap[256];
WCHAR GVRMLfile[256];
int GmaxDisp;//tem que ser int para tratar com if se est� menor que 0, pois vem da digita��o do usu�rio
int GmaxLineVar;//tem que ser int para tratar com if se est� menor que 0, pois vem da digita��o do usu�rio
int GwindowSize;//tem que ser int por compatibilidade com CorrespSearch, l� esta variavel participa de um for com um y que pode ser negativo, apesar de windowSize n�o poder ser negativo
WCHAR GintensityOrGradient;
DWORD VERTS_PER_EDGE_WIDTH;//=32;
DWORD VERTS_PER_EDGE_HEIGHT;//=32;


bool gradient;
DWORD   g_dwNumVertices; //depende de VERTS_PER_EDGE_WIDTH e VERTS_PER_EDGE_HEIGHT
DWORD   g_dwNumIndices; //depende de VERTS_PER_EDGE_WIDTH e VERTS_PER_EDGE_HEIGHT

#endif