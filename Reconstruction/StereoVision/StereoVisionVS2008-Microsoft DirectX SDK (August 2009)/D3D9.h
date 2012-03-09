#ifndef D3D9_H
#define D3D9_H


#include "Common.h"


HWND d3dHWNDsave;

extern HWND	hDialog;
extern HWND	hWindow;

//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont*              g_pFont = NULL;         // Font for drawing text
ID3DXSprite*            g_pTextSprite = NULL;   // Sprite for batching draw text calls
//ID3DXEffect*            g_pEffect = NULL;       // D3DX effect interface
CModelViewerCamera      g_Camera;               // A model viewing camera
bool                    g_bShowHelp = true;     // If true, it renders the UI control text
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg         g_SettingsDlg;          // Device settings dialog
CDXUTDialog             g_HUD;                  // dialog for standard controls
CDXUTDialog             g_SampleUI;             // dialog for sample specific controls

// Scene
//Buffers de Vertex e Index
LPDIRECT3DVERTEXBUFFER9      g_pVB = NULL;
LPDIRECT3DINDEXBUFFER9       g_pIB = NULL;
//LPDIRECT3DTEXTURE9*          g_pTexture  = NULL; // Textures for our mesh
IDirect3DTexture9*			pTexture = NULL; 

//t #define VERTS_PER_EDGE 64
extern DWORD   VERTS_PER_EDGE_WIDTH;//=32;
extern DWORD   VERTS_PER_EDGE_HEIGHT;//=32;
extern DWORD   g_dwNumVertices; //calculado em MAIN.CPP getData(HWND hDlg,int dlgItem)
extern DWORD   g_dwNumIndices;//calculado em MAIN.CPP getData(HWND hDlg,int dlgItem)



extern WCHAR GleftFile[256];
extern WCHAR GrightFile[256];
extern WCHAR GdisparityMap[256];
extern WCHAR GVRMLfile[256];
extern int GmaxDisp;
extern int GmaxLineVar;
extern int GwindowSize;

extern unsigned long globalWidth;
extern unsigned long globalHeight;
extern void *voidImgR;//somente para exportVRML() "saber" se foi carregado somente o mapa de disparidades
extern void *voidDispMap;
extern void *voidDispMapLegacy;
extern unsigned short maxDisparity;
extern bool wire;
extern bool image;
extern bool changeImageDisp;


//eLPDIRECT3DVERTEXSHADER9      g_pVertexShader = NULL;
//uLPD3DXCONSTANTTABLE          g_pConstantTable = NULL;
LPDIRECT3DVERTEXDECLARATION9 g_pVertexDeclaration = NULL;

//j struct Vertex
//j {
//j    D3DXVECTOR3 Position;
//j    D3DCOLOR    Diffuse;
//j };



//codigo do NovoMundo

posicao p;
posicao old;
posicao cam;

float passoMinimo=0.25f;//0.125f;
const unsigned short angulos1Q=361;//721; //90 divido pelo passo minimo + 1
unsigned short quadrante1;// variaveis para pre-calculo do numero de valores 
unsigned short quadrante2;// para cada quadrante
unsigned short quadrante3;
unsigned short quadrante4;
double lookup[angulos1Q];
// ** Matriz Global,Visual e de Projecao **
D3DMATRIX matWorld, matView, matProj;
float senX,cosX,senY,cosY,senZ,cosZ; // variaveis da funcao montaViewMatrizTurbo
//fim código NovoMundo




//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_CHANGEDEVICE        3


//Navegação
bool firstT=true;
D3DXMATRIXA16 ViewMatrix;

int width=385;
int height=285;


#endif
