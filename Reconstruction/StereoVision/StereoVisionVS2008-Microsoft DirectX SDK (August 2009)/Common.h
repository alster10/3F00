#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <windowsx.h>//GET_WM_COMMAND_ID
#include <gdiplus.h>
#include <strsafe.h>
#include "resource.h"
//#include "dxstdafx.h" //DXUtil
#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "DXUTgui.h"
#include "SDKmisc.h"


using namespace Gdiplus;


struct posicao
	{
	// ponto de referencia do observador
	float x,y,z;
	signed short indiceSpinX;
	signed short indiceSpinY;
	signed short indiceSpinZ;
	};

struct IMGpixel{unsigned char a;unsigned char r;unsigned char g;unsigned char b;};

struct dispMap
	{
	unsigned short disp;
	unsigned short type;
	};

struct lastArguments
	{
	WCHAR leftFile[256];
	WCHAR rightFile[256];
	int maxDisp;
	int maxLineVar;
	int windowSize;
	WCHAR intensityOrGradient;
	DWORD VertsPerEdgeWidth;
	DWORD VertsPerEdgeHeight;
	};

struct D3DNMVERTEX
	{
    D3DVECTOR v;
	D3DVECTOR n;
	FLOAT     tu, tv;
	};


//--------------------------------------------------------------------------------------
// Main Functions
//--------------------------------------------------------------------------------------
void teste(void);
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

void deleteBuffer(void);
void carregaDispMapBuffer(unsigned long *pointLocImg,Bitmap *bitmap);
void carregaImgBuffer(IMGpixel *pointLocImg,Bitmap bitmap);
void carregaImgBufferWithSobel(IMGpixel *pointLocImg,Bitmap *bitmap);
void setPixelCount(int x, int y ,int i,Bitmap *bitmap);
void CorrespSearch(HDC hdc,unsigned int width,unsigned int height,unsigned int maxDisp,unsigned int maxLineVar,int windowSize);
void sumFirstWindow(unsigned long width,unsigned long x,unsigned long y,unsigned long n,unsigned long *AggregationStrip,unsigned long *DispSelectLine,unsigned long stepLine);
void sumNextWindow(unsigned long width,unsigned long x,unsigned long y,unsigned long n,unsigned long *AggregationStrip,unsigned long *DispSelectLine,unsigned long stepLine);
void FastCorrespSearch(HDC hdc,unsigned int width,unsigned int height,unsigned int maxDisp,unsigned int maxLineVar,int windowSize);
void sumPair(unsigned long x,unsigned long stepImageUnderStripOut,unsigned long stepImageUnderStripIn,unsigned long n,unsigned long *AggregationStrip,unsigned long *DispSelectLine,unsigned long stepLine);


void matchingCost(HDC hdc,unsigned int width,unsigned int height,unsigned int range);
void selection(HDC hdc,unsigned long width,unsigned long height,unsigned int range,float *squaredDiff);
void sameLayer(dispMap *pointDispMap,unsigned long width,unsigned long height);
void sameLayer(dispMap *pointDispMap,unsigned long width,unsigned long height,unsigned long x,unsigned long y,int disp);
void aggregation(unsigned long width,unsigned long height,unsigned int range,unsigned long Kernel,float *squaredDiff);
void aggregationOneLayer(unsigned long width,unsigned long height,unsigned int range,unsigned long Kernel,float *squaredDiff);
void sumFirstKernel(unsigned long width,unsigned long x,unsigned long y,unsigned long n,float *squaredDiff,float *sumSquaredDiff);
void sumNextKernel(unsigned long width,unsigned long x,unsigned long y,unsigned long n,float *squaredDiff,float *sumSquaredDiff);
void sumKernel(unsigned long width,unsigned long x,unsigned long y,unsigned long n,float *squaredDiff,float *sumSquaredDiff);
void logW(char *file,WCHAR *message);
void log(char *file,WCHAR *message);
void log(WCHAR *message);

void paint(HDC hdc);
VOID OnPaint(HDC hdc);
VOID OnPaint2(HDC hdc);
void enableItem(HWND hDlg,int dlgItem,bool enable);
bool getData(HWND hDlg,int dlgItem);
bool getOnlyDispMap(HWND hDlg,int dlgItem);
void load10RecentInComboBox(HWND hDlg,int dlgItem);
int load10Recent(char *file,lastArguments *last);
void loadRecentInComboBox(HWND hDlg,int dlgItem);
//void loadRecent(char *file,WCHAR *leftFile,WCHAR *rightFile,int *maxDisp,int *maxLineVar,int *windowSize);
void loadRecent(char *file,WCHAR *leftFile,WCHAR *rightFile,int *maxDisp,int *maxLineVar,int *windowSize,WCHAR* intensityOrGradient,DWORD* VertsPerEdgeWidth,DWORD* VertsPerEdgeHeight);
void ErrorMessage(int number,WCHAR *message);
void ErrorMessage(WCHAR *name,WCHAR *message);

bool exist(WCHAR *file);
BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Text(HDC hdc,WCHAR *text);


//--------------------------------------------------------------------------------------
// Direct X Functions
//--------------------------------------------------------------------------------------


bool    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
//bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
bool	CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
//void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void	CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
//void	cameraNavigation(IDirect3DDevice9* pd3dDevice);


void exportVRML(WCHAR *filename,D3DNMVERTEX* pNMVertex,WORD* pIndices);
void	posicaoInicial(void);
void SetupMatrices(IDirect3DDevice9* pd3dDevice);
void preparaLookup(void);
void MatrixMultiply( D3DMATRIX& q, D3DMATRIX& a, D3DMATRIX& b );
void calcSenCoss(float* seno,float* coss,signed short* spin);
void montaViewMatrizTurbo(void);
HRESULT SetProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,FLOAT fNearPlane, FLOAT fFarPlane );
inline VOID montaMatrizIdentidade( D3DMATRIX& m )
{
	m._11=1.0f;m._12=0.0f;m._13=0.0f;m._14=0.0f;
	m._21=0.0f;m._22=1.0f;m._23=0.0f;m._24=0.0f;
	m._31=0.0f;m._32=0.0f;m._33=1.0f;m._34=0.0f;
	m._41=0.0f;m._42=0.0f;m._43=0.0f;m._44=1.0f;

}

void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void	CALLBACK MouseProc( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext );
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
//bool	leiaTextur(IDirect3DDevice9* pd3dDevice,WCHAR *texturePath);
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );

int		InitD3D9(HINSTANCE hInstance);
void	OnSize();
void    InitApp();
void    RenderText();
bool	TestKey( BYTE vKey );
