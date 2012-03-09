#include "Main.h"
// Para ver como usar GDI+:	Using Images, Bitmaps, and Metafiles 
//							Reading and Writing Metadata (exemplo da classe bitmap)
//							Loading and Displaying Bitmaps 
// Para salvar imagem veja o help: "Reading and Writing Metadata "

//Styles for the Progress Control


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
	{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
   
	hInstanceSave=hInstance;
  
	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
   	
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1),NULL,DialogProc);
	deleteBuffer();
	GdiplusShutdown(gdiplusToken);
	return true;
	}  // WinMain



void enableItem(HWND hDlg,int dlgItem,bool enable)
	{
	HWND hwnditem=GetDlgItem (hDlg,dlgItem);
	EnableWindow(hwnditem,enable);
	}



bool getData(HWND hDlg,int dlgItem)
	{
	WCHAR argument[256];

	GetDlgItemText(hDlg, dlgItem, argument, sizeof(argument));

//i	swscanf(argument,L"%s %s %d %d %d %c %d %d",GleftFile,GrightFile,&GmaxDisp,&GmaxLineVar,&GwindowSize,&GintensityOrGradient,&VERTS_PER_EDGE_WIDTH,&VERTS_PER_EDGE_HEIGHT);
	swscanf_s(argument,L"%s %s %d %d %d %c %d %d",GleftFile,_countof(GleftFile),GrightFile,_countof(GrightFile),&GmaxDisp,&GmaxLineVar,&GwindowSize,&GintensityOrGradient,sizeof(WCHAR),&VERTS_PER_EDGE_WIDTH,&VERTS_PER_EDGE_HEIGHT);

	//composição do nome do mapa de disparidades	
	LPCTSTR pszFormat = TEXT("%s_%d_%d_%d_%c.BMP");
	StringCbPrintfW(GdisparityMap, (256*sizeof(WCHAR)),pszFormat,GleftFile,GmaxDisp,GmaxLineVar,GwindowSize,GintensityOrGradient);
	if(compare)
		{
		LPCTSTR pszFormat = TEXT("%s_%d_%d_%d_%c_Trad.BMP");
		StringCbPrintfW(disparityMap, (256*sizeof(WCHAR)),pszFormat,GleftFile,GmaxDisp,GmaxLineVar,GwindowSize,GintensityOrGradient);
		}


	//composição do nome do arquivo 3D exportado em VRML	
	LPCTSTR pszFormat2 = TEXT("%s_%d_%d_%d_%c_%d_%d.WRL");
	StringCbPrintfW(GVRMLfile, (256*sizeof(WCHAR)),pszFormat2,GleftFile,GmaxDisp,GmaxLineVar,GwindowSize,GintensityOrGradient,VERTS_PER_EDGE_WIDTH,VERTS_PER_EDGE_HEIGHT);

	

	if(!exist(GleftFile)) {ErrorMessage(GleftFile,L"file not found.");return false;}
	if(!exist(GrightFile)) {ErrorMessage(GrightFile,L"file not found.");return false;}
    if(GmaxDisp<1)		{ErrorMessage(GmaxDisp,L"não é um Max Disparity válido, pois é menor que 1.");return false;}
	if(GmaxLineVar<1)	{ErrorMessage(GmaxLineVar,L"não é um Max Line Variation válido, pois é menor que 1.");return false;}
	if(GmaxLineVar%2==0)	{ErrorMessage(GmaxLineVar,L"não é um Max Line Variation válido, pois é um número par.");return false;}
	if(GwindowSize%2==0)	{ErrorMessage(GwindowSize,L"não é um Window Size válido, pois é um número par.");return false;}
	if(GwindowSize<3)	{ErrorMessage(GwindowSize,L"não é um Window Size válido, pois é menor que 3.");return false;}
	if((GintensityOrGradient!='I')&&(GintensityOrGradient!='G')) {ErrorMessage(L"Caracter Invalido.\n",L"Use I para Intensidade ou G para Gradiente.");return false;}
	if(VERTS_PER_EDGE_WIDTH<2)		{ErrorMessage(VERTS_PER_EDGE_WIDTH,L"Não é válido, pois é menor que 2.");return false;}
	if(VERTS_PER_EDGE_HEIGHT<2)		{ErrorMessage(VERTS_PER_EDGE_HEIGHT,L"Não é válido, pois é menor que 2.");return false;}

	if(GintensityOrGradient=='G') gradient=true;
	else gradient=false;
	
//PERECE QUE TEM QUE SER POTENCIA DE 2


	//variaveis usadas em D3D9.CPP
	g_dwNumVertices = VERTS_PER_EDGE_WIDTH * VERTS_PER_EDGE_HEIGHT;
	g_dwNumIndices = 6 * (VERTS_PER_EDGE_WIDTH - 1) * (VERTS_PER_EDGE_HEIGHT - 1);

/*
	WCHAR text[512];
	LPCTSTR pszFormat1 = TEXT("%s %s %u %u %u\n");
	StringCbPrintfW(text, (512*sizeof(WCHAR)),pszFormat1,GleftFile,GrightFile,GmaxDisp,GmaxLineVar,GwindowSize);
	log("last.txt",text);
*/


	//recent com 10 itens, ordenados pelo mais recente(de cima para baixo) e sem itens repetidos
	//Salva argumentos em last.txt, somente se for novo, e se já foi usado, passa a ser o primeiro da lista

	int numberOfRecents=0;
	int i;
	bool already=false;
	int itemRepetido=-1;
	lastArguments last[10];

	numberOfRecents=load10Recent("last.txt",&last[0]);
	//ErrorMessage(numberOfRecents,GleftFile);

	for(i=0;((i<numberOfRecents)&&(already==false));i++)
		{
		if(	(wcscmp(GleftFile,last[i].leftFile)==0)&&
			(wcscmp(GrightFile,last[i].rightFile)==0)&&
			(GmaxDisp==last[i].maxDisp)&&
			(GmaxLineVar==last[i].maxLineVar)&&
			(GwindowSize==last[i].windowSize)&& 
			(GintensityOrGradient==last[i].intensityOrGradient)&&
			(VERTS_PER_EDGE_WIDTH==last[i].VertsPerEdgeWidth)&&
			(VERTS_PER_EDGE_HEIGHT==last[i].VertsPerEdgeHeight)	) {already=true; itemRepetido=i;}
		}

	

	
	WCHAR text[512];
	LPCTSTR pszFormat1 = TEXT("%s %s %u %u %u %c %u %u\n");
	StringCbPrintfW(text, (512*sizeof(WCHAR)),pszFormat1,GleftFile,GrightFile,GmaxDisp,GmaxLineVar,GwindowSize,GintensityOrGradient,VERTS_PER_EDGE_WIDTH,VERTS_PER_EDGE_HEIGHT);
	logW("last.txt",text);

	if(numberOfRecents==10)
		for(i=0;i<numberOfRecents-1;i++)//perde o último
			{
			if(i!=itemRepetido)
				{
				StringCbPrintfW(text, (512*sizeof(WCHAR)),pszFormat1,last[i].leftFile,last[i].rightFile,last[i].maxDisp,last[i].maxLineVar,last[i].windowSize,last[i].intensityOrGradient,last[i].VertsPerEdgeWidth,last[i].VertsPerEdgeHeight);
				log("last.txt",text);
				}
			}
			
	else
		for(i=0;i<numberOfRecents;i++)
			{
			if(i!=itemRepetido)
				{
				StringCbPrintfW(text, (512*sizeof(WCHAR)),pszFormat1,last[i].leftFile,last[i].rightFile,last[i].maxDisp,last[i].maxLineVar,last[i].windowSize,last[i].intensityOrGradient,last[i].VertsPerEdgeWidth,last[i].VertsPerEdgeHeight);
				log("last.txt",text);
				}
			}

	
	return true;

/*	WCHAR text[512];
	WCHAR leftFile[256];
	WCHAR rightFile[256];


	GetDlgItemText(hDialog, IDC_COMBO1, leftFile, sizeof(leftFile));
	GetDlgItemText(hDialog, IDC_COMBO2, rightFile, sizeof(rightFile));

	if(!exist(leftFile)) {messageNotFound(leftFile);return false;}
	if(!exist(rightFile)) {messageNotFound(rightFile);return false;}

	
	LPCTSTR pszFormat1 = TEXT("%s;%s\n");
	StringCbPrintfW(text, (512*sizeof(WCHAR)),pszFormat1,leftFile,rightFile);
	log("last.txt",text);
	return true;*/
	}

	
bool getOnlyDispMap(HWND hDlg,int dlgItem)
	{
	WCHAR argument[256];
	
	
	GetDlgItemText(hDlg, dlgItem, argument, sizeof(argument));



	//i swscanf(argument,L"%s %s %d %d",GdisparityMap,GleftFile,&VERTS_PER_EDGE_WIDTH,&VERTS_PER_EDGE_HEIGHT);
	swscanf_s(argument,L"%s %s %d %d",GdisparityMap,_countof(GdisparityMap),GleftFile,_countof(GleftFile),&VERTS_PER_EDGE_WIDTH,&VERTS_PER_EDGE_HEIGHT);

	if(!exist(GdisparityMap))	{ErrorMessage(GdisparityMap,L"file not found.");return false;}
	if(!exist(GleftFile))		{ErrorMessage(GleftFile,L"file not found.");return false;}
	if(VERTS_PER_EDGE_WIDTH<2)	{ErrorMessage(VERTS_PER_EDGE_WIDTH,L"Não é válido, pois é menor que 2.");return false;}
	if(VERTS_PER_EDGE_HEIGHT<2)	{ErrorMessage(VERTS_PER_EDGE_HEIGHT,L"Não é válido, pois é menor que 2.");return false;}


	

	
	//composição do nome do arquivo 3D exportado em VRML	
	PCTSTR pszFormat = TEXT("%s_%s_%d_%d.WRL");
	StringCbPrintfW(GVRMLfile, (256*sizeof(WCHAR)),pszFormat,GdisparityMap,GleftFile,VERTS_PER_EDGE_WIDTH,VERTS_PER_EDGE_HEIGHT);


	

	
	//variaveis usadas em D3D9.CPP
	//VERTS_PER_EDGE_WIDTH=64;
	//VERTS_PER_EDGE_HEIGHT=64;
	g_dwNumVertices = VERTS_PER_EDGE_WIDTH * VERTS_PER_EDGE_HEIGHT;
	g_dwNumIndices = 6 * (VERTS_PER_EDGE_WIDTH - 1) * (VERTS_PER_EDGE_HEIGHT - 1);

	//Alocação de memória para o Mapa de Disparidades
	
	
	dispGenerated=true;//mimetismo com a função CorrespSearch(), porém somente indica para paint() ser o bitmap, não significa que voidDispMap esteja carregado
	LoadOnlyDispMap=true;//indica que em paint() deve ser carregado o buffer voidDispMap e variáveis globais necessárias
	return true;
	}


void load10RecentInComboBox(HWND hDlg,int dlgItem)
	{
	lastArguments last[10];
	int i,numberOfRecents=0;



	numberOfRecents=load10Recent("last.txt",&last[0]);

	WCHAR text[512];
	LPCTSTR pszFormat = TEXT("%s %s %u %u %u %c %u %u");
	
	//log("last2.txt",text);

	SendDlgItemMessage( hDlg, dlgItem,   CB_RESETCONTENT, 0, 0 );

	//char text[512];

	//sprintf(text,"%s %s %u %u %u",leftFile,rightFile,maxDisp,maxLineVar,windowSize);

	if(numberOfRecents!=0)
		{
		WPARAM dwItem;
		for(i=0;i<numberOfRecents;i++)
			{
			StringCbPrintfW(text, (512*sizeof(WCHAR)),pszFormat,last[i].leftFile,last[i].rightFile,last[i].maxDisp,last[i].maxLineVar,last[i].windowSize,last[i].intensityOrGradient,last[i].VertsPerEdgeWidth,last[i].VertsPerEdgeHeight);
			dwItem = SendDlgItemMessage( hDlg, dlgItem, CB_ADDSTRING, 0,(LPARAM)text);
			SendDlgItemMessage( hDlg, dlgItem, CB_SETITEMDATA,(WPARAM)dwItem, (LPARAM)i);
			}//for(i=0;i<numberOfRecents;i++)
		}
	}


int load10Recent(char *file,lastArguments *last)
	{
	int i=0,returned=-2;
	FILE *fileptr;
	errno_t err;

	//i if((fileptr=fopen(file,"r")) != NULL)
	if( (err  = fopen_s( &fileptr, file, "r" )) ==0 )
		{
		returned=10;//não pode ser 0 ou -1
		for(i=0;((i<10)&&(returned!=EOF)&&(returned!=0));i++)//returned=0 indica que a leitura não obteve dados
			{
			//i returned=fwscanf(fileptr,L"%s %s %d %d %d %c %d %d",last[i].leftFile,last[i].rightFile,&last[i].maxDisp,&last[i].maxLineVar,&last[i].windowSize,&last[i].intensityOrGradient,&last[i].VertsPerEdgeWidth,&last[i].VertsPerEdgeHeight);
			returned=fwscanf_s(fileptr,L"%s %s %d %d %d %c %d %d",last[i].leftFile,_countof(last[i].leftFile),last[i].rightFile,_countof(last[i].rightFile),&last[i].maxDisp,&last[i].maxLineVar,&last[i].windowSize,&last[i].intensityOrGradient,sizeof(WCHAR),&last[i].VertsPerEdgeWidth,&last[i].VertsPerEdgeHeight);
			//ErrorMessage(i,last[i].leftFile);
			}
		fclose(fileptr);
		}
	if((returned==EOF)||(returned==0)) return i-1;//nestas duas situações o contador atingiu um valor que não representa uma leitura válida
	else return i;
	}

//i código não usado, por enquanto não vou corrigir funções Deprecated
/*i  void loadRecentInComboBox(HWND hDlg,int dlgItem)
	{
	WCHAR leftFile[256];
	WCHAR rightFile[256];
	int maxDisp;
	int maxLineVar;
	int windowSize;
	WCHAR intensityOrGradient;
	DWORD VertsPerEdgeWidth;//=32;
	DWORD VertsPerEdgeHeight;//=32;



	loadRecent("last.txt",leftFile,rightFile,&maxDisp,&maxLineVar,&windowSize,&intensityOrGradient,&VertsPerEdgeWidth,&VertsPerEdgeHeight);

	WCHAR text[512];
	LPCTSTR pszFormat = TEXT("%s %s %u %u %u %c %u %u");
	StringCbPrintfW(text, (512*sizeof(WCHAR)),pszFormat,leftFile,rightFile,maxDisp,maxLineVar,windowSize,intensityOrGradient,VertsPerEdgeWidth,VertsPerEdgeHeight);
	//log("last2.txt",text);

	SendDlgItemMessage( hDlg, dlgItem,   CB_RESETCONTENT, 0, 0 );

	//char text[512];

	//sprintf(text,"%s %s %u %u %u",leftFile,rightFile,maxDisp,maxLineVar,windowSize);

	WPARAM dwItem;
	//for(i=0;i<37;i++)
	//	{
	    dwItem = SendDlgItemMessage( hDlg, dlgItem, CB_ADDSTRING, 0,(LPARAM)text);
		SendDlgItemMessage( hDlg, dlgItem, CB_SETITEMDATA,(WPARAM)dwItem, (LPARAM)0);
	//	}//fecha for(i=0;i<3;i++)
	}
*/



//i código não usado, por enquanto não vou corrigir funções Deprecated
//i void loadRecent(char *file,WCHAR *leftFile,WCHAR *rightFile,int *maxDisp,int *maxLineVar,int *windowSize,WCHAR *intensityOrGradient,DWORD *VertsPerEdgeWidth,DWORD *VertsPerEdgeHeight)
//i	{
//i	FILE *fileptr;
//i	if((fileptr=fopen(file,"r")) != NULL)
//i		{
//i		fwscanf(fileptr,L"%s %s %d %d %d %c %d %d",leftFile,rightFile,maxDisp,maxLineVar,windowSize,intensityOrGradient,VertsPerEdgeWidth,VertsPerEdgeHeight);
//i		fclose(fileptr);
//i		}
//i	}

void ErrorMessage(int number,WCHAR *message)
	{
	WCHAR text[512];
	LPCTSTR pszFormat1 = TEXT("%d %s");
	StringCbPrintfW(text, (512*sizeof(WCHAR)),pszFormat1,number,message);
	MessageBox(NULL,text,L"Argument Error",MB_OK);	
	}

void ErrorMessage(WCHAR *name,WCHAR *message)
	{
	WCHAR text[512];
	LPCTSTR pszFormat1 = TEXT("%s %s");
	StringCbPrintfW(text, (512*sizeof(WCHAR)),pszFormat1,name,message);
	MessageBox(NULL,text,L"Argument Error",MB_OK);	
	}

bool exist(WCHAR *file)
{
	FILE *pont;
	errno_t err;
	//i if ((pont=_wfopen(file,L"r"))!=0)//if existir o arquivo
	if( (err  =_wfopen_s( &pont,file,L"r" )) ==0 )
		{fclose(pont);
		return true;}
	return false;
}

BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	HDC          hdc;
    PAINTSTRUCT  ps;

	switch (uMsg)
		{
		case WM_INITDIALOG:
			{
			hDialog = hDlg;
			load10RecentInComboBox(hDlg,IDC_COMBO1);
			//InitD3D9(hInstanceSave);
			//MessageBox(NULL,L"teste",L"teste",MB_OK);
			break;
			}

		case WM_DRAWITEM:
			break;

		case WM_PAINT:
			{
			
			/*if(compare)
				{
				hdc = BeginPaint(hDlg, &ps);
				if(first)
					{
					OnPaint2(hdc);
					first=false;
					}
				else OnPaint(hdc);//carrega novamente a imagem... arrumar depois
				EndPaint(hDlg, &ps);
				}
			else
				{*/
				hdc = BeginPaint(hDlg, &ps);
				paint(hdc);
				EndPaint(hDlg, &ps);
			//	}
			
			
			break;
			}

		case WM_ACTIVATE:// WM_SETCURSOR: Tentativa de recarregar a imagem quando ganha foco
			{//if(show) render(hDlg);
			break;}

		case WM_DESTROY:
			{EndDialog(hDlg, TRUE);
			//deleteBuffer();
			break;}

		case WM_CLOSE://Fecha
			{
			EndDialog(hDlg, TRUE);
			//deleteBuffer();
			PostQuitMessage(1);//fecha também a parte DirectX
			return true;
			}

		case WM_COMMAND:
			{
			
			switch (GET_WM_COMMAND_ID(wParam, lParam))
				{
				case IDCLOSE:
					{EndDialog(hDlg, TRUE);
					//deleteBuffer();
					PostQuitMessage(1);//fecha também a parte DirectX
					break;}

				case IDC_LOAD:
					{
					if(IsDlgButtonChecked(hDlg,IDC_CHECK_COMPARE)==BST_CHECKED) compare=true;
					else compare=false;


					if(getData(hDlg,IDC_COMBO1))
						{
						enableItem(hDlg,IDGEN,true);
						enableItem(hDlg,IDC_CHECK_COMPARE,false);//bloqueia combo depois da leitura
						imagesLoaded=true;

						hdc = BeginPaint(hDlg, &ps);
						paint(hdc);
						EndPaint(hDlg, &ps);
						}
//fazer a leitura dos nomes e entender como se faz o refresh... veja como exemplo o que ocorre com o d3d9
					//RECT diagRect;
					//GetWindowRect(hDlg,&diagRect);
					//MoveWindow(hDlg,diagRect.left+1,diagRect.top,diagRect.right-diagRect.left+1,diagRect.bottom-diagRect.top,true);
    
					
					break;
					}	

				case IDC_LOAD_DISP_MAP:
					{
					if(getOnlyDispMap(hDlg,IDC_COMBO2))
						{
						enableItem(hDlg,ID3D,true);
						imagesLoaded=false;

						//hdc = BeginPaint(hDlg, &ps);
						//paint(hdc);
						//EndPaint(hDlg, &ps);

						
						hdc=GetDC(hDlg);//efeito imediato na tela
						paint(hdc);
						ReleaseDC(hDlg,hdc);

						//só para dar um refresh na tela, não consegui de outra maneira, mas está funcionando. Tive que mover a janela 1 pixel à direita. 
						//RECT diagRect;
						//GetWindowRect(hDlg,&diagRect);
						//MoveWindow(hDlg,diagRect.left+1,diagRect.top,diagRect.right-diagRect.left+1,diagRect.bottom-diagRect.top,true);
						}
					
					break;
					}	
				
				case ID3D:
					{
					enableItem(hDlg,IDC_WIRE,true);
					enableItem(hDlg,IDC_IMAGE_DISP,true);

					InitD3D9(hInstanceSave);
					break;
					}

				case IDC_WIRE:
					{
					if(wire) wire=false;
					else wire=true;
					
					//if(d3d9Active) SetFocus( d3dHWNDsave );
					SetFocus( d3dHWNDsave );
					break;
					}

				case IDC_IMAGE_DISP:
					{
					if(image) {image=false;
								changeImageDisp=true;}
					else	{image=true;
							changeImageDisp=true;}
					
					SetFocus( d3dHWNDsave );
	
					break;
					}

					

				case IDGEN:
					{
					

					//t clock_t startNew, finishNew,startLegacy, finishLegacy;
					//t double  durationNew,durationLegacy;
					enableItem(hDlg,ID3D,true);

					hdc = BeginPaint(hDlg, &ps);
					//t CorrespSearch(hdc,globalWidth,globalHeight,16,1,7);//tsukuba low
					//t startNew=clock();
					FastCorrespSearch(hdc,globalWidth,globalHeight,GmaxDisp,GmaxLineVar,GwindowSize);
					//t finishNew=clock();
					//t durationNew=finishNew-startNew;
					if(compare)
						{
						windowSizeForCompare=GwindowSize;
						matchingCost(hdc,globalWidth,globalHeight,GmaxDisp);
						}

					paint(hdc);
					EndPaint(hDlg, &ps);

					hdc=GetDC(hDlg);//efeito imediato na tela
					paint(hdc);
					ReleaseDC(hDlg,hdc);

					//só para dar um refresh na tela, não consegui de outra maneira, mas está funcionando. Tive que mover a janela 1 pixel à direita. 
					
					//RECT diagRect;
					//GetWindowRect(hDlg,&diagRect);
					//MoveWindow(hDlg,diagRect.left+1,diagRect.top,diagRect.right-diagRect.left+1,diagRect.bottom-diagRect.top,true);
    
					if(compare==false)
						{
						WCHAR text[60];
						LPCTSTR pszFormat1 = TEXT("Time :%2.3fs\n");
						StringCbPrintfW(text, (60*sizeof(WCHAR)),pszFormat1,durationST/CLOCKS_PER_SEC);
						SetDlgItemText(hDialog, IDC_STATIC_TIME,text);
						}
					else
						{
						WCHAR text[60];
						LPCTSTR pszFormat1 = TEXT("ST :%2.3fs\nTrad :%2.3fs\n");
						StringCbPrintfW(text, (60*sizeof(WCHAR)),pszFormat1,durationST/CLOCKS_PER_SEC,durationT/CLOCKS_PER_SEC);
						SetDlgItemText(hDialog, IDC_STATIC_TIME,text);

						WCHAR text1[120];
						LPCTSTR pszFormat2 = TEXT("Trad;%2.3f;ST;%2.3f;Dif;%2.3f;%s;%s;%d;%d\n");
						StringCbPrintfW(text1, (120*sizeof(WCHAR)),pszFormat2,durationT/CLOCKS_PER_SEC,durationST/CLOCKS_PER_SEC,(durationT/CLOCKS_PER_SEC)-(durationST/CLOCKS_PER_SEC),GleftFile,GrightFile,GmaxDisp,GwindowSize);
						log(text1);
						}

					//UpdateWindow(hDialog);
					//SendMessage(hDialog,WM_PAINT,NULL,NULL);
					//hdc = BeginPaint(hDlg, &ps);
					//OnPaint(hdc);
					//EndPaint(hDlg, &ps);
					
					/**HDC          hdc2;
					PAINTSTRUCT  ps2;
					hdc2 = BeginPaint(hDlg, &ps2);
					OnPaint(hdc2);
					EndPaint(hDlg, &ps2);*/
					break;}
				
				}//switch (GET_WM_COMMAND_ID(wParam, lParam))
			break;
			}//fecha case WM_COMMAND:

		default:
		return(FALSE);
		}

	return(TRUE);
	}

	// teste da geração de arquivo jpg depende da existência de um arquivo "FakePhoto.jpg"
void teste(void)
	{
   // Initialize <tla rid="tla_gdiplus"/>.
   //GdiplusStartupInput gdiplusStartupInput;
   //ULONG_PTR gdiplusToken;
   //GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   Status stat;
   CLSID  clsid;
   char   propertyValue[] = "Fake Photograph";
   Bitmap* bitmap = new Bitmap(L"FakePhoto.jpg");
   PropertyItem* propertyItem = new PropertyItem;
   // Get the CLSID of the JPEG encoder.
   GetEncoderClsid(L"image/jpeg", &clsid);
   propertyItem->id = PropertyTagImageTitle;
   propertyItem->length = 16;  // string length including NULL terminator
   propertyItem->type = PropertyTagTypeASCII; 
   propertyItem->value = propertyValue;
   bitmap->SetPropertyItem(propertyItem);
   stat = bitmap->Save(L"FakePhoto2.jpg", &clsid, NULL);
   if(stat == Ok)
      MessageBox(NULL,L"FakePhoto2.jpg saved successfully.\n",L"teste",MB_OK);
   
   delete propertyItem;
   delete bitmap;
   //GdiplusShutdown(gdiplusToken);
   
}

	

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}


void logW(char *file,WCHAR *message)//write
	{
	FILE *fileptr;
	errno_t err;

	//i if((fileptr=fopen(file,"w")) != NULL)
	if( (err  = fopen_s( &fileptr, file, "w" )) ==0 )
		{	
		fwprintf(fileptr,L"%s",message);
		fclose(fileptr);
		}
	}

void log(char *file,WCHAR *message)//append
	{
	FILE *fileptr;
	errno_t err;
	
	//i if((fileptr=fopen(file,"a")) != NULL)
	if( (err  = fopen_s( &fileptr, file, "a" )) ==0 )
		{	
		fwprintf(fileptr,L"%s",message);
		fclose(fileptr);
		}
	}



void log(WCHAR *message)
	{
	FILE *fileptr;
	errno_t err;

	//i if((fileptr=fopen("log.csv","a")) != NULL)
	if( (err  = fopen_s( &fileptr, "log.csv", "a" )) ==0 )
		{	
		fwprintf(fileptr,L"%s",message);
		fclose(fileptr);
		}
	}


//Novo algoritmo: Stripping Technique
void CorrespSearch(HDC hdc,unsigned int width,unsigned int height,unsigned int maxDisp,unsigned int maxLineVar,int windowSize)
	{
	maxDisparity=maxDisp;//variavel global usada em D3D9 para construção da interface 3D
	unsigned long n;
	int yVar;//long pois no for que será usado será colocado na variáve searchRegionY que é long
	//WCHAR text[250];
	
	//proteções
	if(windowSize%2==0)	{log(L"windowSize invalido, número par.\n");return;}
	if(windowSize<3)	{log(L"windowSize invalido, menor que 3.\n");return;}
	if(maxDisp<1)		{log(L"maxDisp invalido, menor que 1.\n");return;}
	if(maxLineVar<1)	{log(L"maxLineVar invalido, menor que 1.\n");return;}
	if(maxLineVar%2==0)	{log(L"maxLineVar invalido, número par.\n");return;}//funciona como a window, porém aceita tamanho 1, que na prática significa um busca 1D

	n=(windowSize-1)/2;//para N=3 n=1 para N=5 n=2 N é o tamanho do kernel e n é o incremento/decremento a ser feito em x e y
	yVar=(maxLineVar-1)/2;//para maxLineVar=7 yVar=3


	//Alocação de memória para a Faixa de Aggregação (Aggregation Strip)
	unsigned long stripSize,numberOfStrips,aggregationStripSize;
	void *voidAggregationStrip;
	unsigned long *pointLocAggregationStrip;

	if(maxDisp<=0) maxDisp=1;//maxDisp=0 é invalido
	if(maxLineVar<=0) maxLineVar=1;//maxLineVar=0 é invalido

	stripSize=width*windowSize;//a altura da strip é o windowSize
	numberOfStrips=maxDisp*maxLineVar;
	aggregationStripSize=stripSize*numberOfStrips;

	voidAggregationStrip= new unsigned long[aggregationStripSize];
	pointLocAggregationStrip=(unsigned long *)voidAggregationStrip;
	//*****************************************************************

	//Alocação de memória para as linhas de seleção de disparidade
	unsigned long dispSelectLineSize;
	void *voidDispSelectLine;
	unsigned long *pointLocDispSelectLine;

	dispSelectLineSize=width*numberOfStrips;//deve ter uma linha para cada faixa

	voidDispSelectLine= new unsigned long[dispSelectLineSize];
	pointLocDispSelectLine=(unsigned long *)voidDispSelectLine;
	//*****************************************************************
	
	//Alocação de memória para o Mapa de Disparidades
	
	//void *voidDispMap; mudou para variavel global que será usada em D3D9.cpp para montar a malha 3D
	unsigned long *pointLocDispMap;

	voidDispMap= new unsigned long[width*height];
	pointLocDispMap=(unsigned long *)voidDispMap;
	//*****************************************************************
	
	WCHAR text[10];
	LPCTSTR pszFormat = TEXT("%0.0f %%");
  
	

	
	
	//unsigned short tshort;
	//unsigned long tlong;

	//tshort=(255+255+255)*(255+255+255);//variação maxima entre dois pixels - unsigned short não serve
	//tlong=(255+255+255)*(255+255+255);//variação maxima entre dois pixels - unsigned long está bom
	if(compare) {PCTSTR pszFormat1 = TEXT("%s");	StringCbPrintfW(GleftFile, (256*sizeof(WCHAR)),pszFormat1,left);}
	Bitmap DispMap(GleftFile);//i usei o left só para gerar um bitmap de mesmas proporções, poderia ser o right

//u	LPCTSTR pszFormat = TEXT("width;%d\nheight;%d\nmaxDisp;%d\nmaxLineVar;%d\nwindowSize;%d\nstripSize;%d\nnumberOfStrips;%d\naggregationStripSize;%d\ndispSelectLineSize\n");
//u	StringCbPrintfW(text, (250*sizeof(WCHAR)),pszFormat,width,height,maxDisp,maxLineVar,windowSize,stripSize,numberOfStrips,aggregationStripSize,dispSelectLineSize);
//u	log(text);

	unsigned int x,yFull,searchRegionX,searchRegionY;
	int y;//tem que aceitar negativo pois vai ser um fator de variação de uma linha, isto é, irá tanto para cima (-) quanto para baixo (+)
	unsigned long stepStrip,stepLineStrip,stepImageL,stepImageR,stepImageUnderStrip,stepLine,increment;
	long jumpLine;


	for(yFull=0;yFull<height;yFull++)//yFull sempre esta no centro da Strip
	{
	StringCbPrintfW(text, (10*sizeof(WCHAR)),pszFormat,((float)yFull/(height))*100 );
	SetDlgItemText(hDialog, IDC_STATIC_PROGRESS,text);
	//Matching Cost
	long tempL,tempR,tempDiff,Sdiff;
	IMGpixel *pointLocImgL,*pointLocImgR;
	
	if((yFull>=n)&&(yFull<height-n))
	{
	if(yFull==n)//primeira linha valida
		{
		//Para um pixels na imagem da esquerda eu procuro um conjunto de pixels(disparidade) na imagem direita
		pointLocImgR=(IMGpixel *)voidImgR;//aponta ponteiro local para a imagem da Direita, dentro do for este ponteiro não é incrementado, só por isso ele é definido fora do for
		//for(searchRegionY=-yVar;searchRegionY<=yVar;searchRegionY++)//caso particular de seach 1D maxLineVar=1 yVar será 0
		for(searchRegionY=0;searchRegionY<maxLineVar;searchRegionY++)
			for(stepStrip=0,searchRegionX=0;searchRegionX<maxDisp;searchRegionX++)
				{
				//stepStrip=(searchRegionY+yVar)*searchRegionX*stripSize;//Calcula uma única vez para cada strip, como searchRegionY vai de -2 até 2 (quando  maxLineVar é 5 e yVar será 2) e aqui é necessário que oscile de 0 até 4 (inclusive) yVar é somado à searchRegionY que fica de -2+2=0 até 2+2=4
				//stepStrip=(searchRegionY+1)*searchRegionX*stripSize;
				stepStrip=(searchRegionY*maxDisp*stripSize)+searchRegionX*stripSize;//(searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
//propague para os outros stepStrip e stepLine, veja se precisa do (int) no for do searchRegionY
				//stepStrip=searchRegionX*stripSize;//Calcula uma única vez para cada strip
				pointLocImgL=(IMGpixel *)voidImgL;
		
				for(y=0;y<windowSize;y++)//Carga da primeira Aggregation Strip, a altura da strip é igual a windowSize
					{
					stepLineStrip=y*width;
					//para y>=yVar se maxLineVar for 5 o yVar será 2 logo o searchRegionY vai oscilar de -2 até 2(inclusive), por consequência o y será somado a -2 o que resulta que o menor y permitido seja 2, logo y tem que ser >= a yVar 
					//para y<=(height-(1+yVar)) se height é 768 que é de 0 até 767 só pode ir até 765 que é height-(1+var)
					if(	(y>=yVar)&&(y<=((int)height-(1+yVar)))	)// na primeira linha válida nem precisava a segunda parte y<=(height-(1+yVar)) porém optei por manter para evitar futuros erros em manutenções do código
						{
						//stepImageR=(y+searchRegionY)*width;//Calcula uma única vez para cada y. o searchRegionY irá fazer o y oscilar para cima e para baixo do valor que referência da imagem da esqueda, pois altera stepImage que é referência para increment que é usado para deslocar a imagem da direita.
						stepImageR=(y+searchRegionY-yVar)*width;//tem que ter um if antes protejendo, pois quando y=0 e searchRegionY=0 pode haver acesso a uma região de memória não alocada

						for(x=0;x<width;x++,pointLocImgL++)//,pointLocImgR++)//,pointLocSquaredDiff++ )
							{
							if(x>=(maxDisp-1))//se o maxDisp é 10 a searchRegionX vai de 0 a 9, logo o minimo que eu posso aceitar de searchRegionX é 9 que é maxDisp menos um, para que a subtração de x não suba uma linha (para y=0 existe o risco de acessar memória não alocada)
								{
								increment=stepImageR+(x-searchRegionX);//Calcula uma única vez para cada x 
							
								tempL=pointLocImgL->r;//eu uso ponteiro puro aqui, por ser mais rápido (não tem que fazer conta)
								tempR=pointLocImgR[increment].r;
								tempDiff=tempL-tempR;
								Sdiff=tempDiff*tempDiff;
								//W if((searchRegionX==2)&&(y==3)&&(x<50))
								//W 	{
								//W 	LPCTSTR pszFormat = TEXT("R;x;%d;y;%d;increment;%d;tempR;%d;%d;tempL;%d;%d;tempDiff;%d\n");
								//W 	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,increment,tempR,pointLocImgR[increment].r,tempL,pointLocImgL->r,Sdiff);
								//W 	log(text);
								//W 	}
			
								tempL=pointLocImgL->g;
								tempR=pointLocImgR[increment].g;
								tempDiff=tempL-tempR;
								Sdiff=(tempDiff*tempDiff)+Sdiff;
								//W if((searchRegionX==2)&&(y==3)&&(x<50))
								//W 	{
								//W 	LPCTSTR pszFormat = TEXT("G;x;%d;y;%d;increment;%d;tempR;%d;%d;tempL;%d;%d;tempDiff;%d\n");
								//W 	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,increment,tempR,pointLocImgR[increment].g,tempL,pointLocImgL->g,Sdiff);
								//W 	log(text);
								//W 	}
			
								tempL=pointLocImgL->b;
								tempR=pointLocImgR[increment].b;
								tempDiff=tempL-tempR;
								Sdiff=(tempDiff*tempDiff)+Sdiff;
								//W if((searchRegionX==2)&&(y==3)&&(x<50))
								//W 	{
								//W 	LPCTSTR pszFormat = TEXT("B;x;%d;y;%d;increment;%d;tempR;%d;%d;tempL;%d;%d;tempDiff;%d\n");
								//W 	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,increment,tempR,pointLocImgR[increment].b,tempL,pointLocImgL->b,Sdiff);
								//W 	log(text);
								//W 	}

								pointLocAggregationStrip[stepStrip+stepLineStrip+x]=Sdiff;
								//W if((y==2)&&(x<50))
								//W  	{
								//W 	LPCTSTR pszFormat = TEXT("searchRegionY;t;searchRegionX;%d;x;%d;y;%d;AggregationStrip;%d;%d\n");
								//W 	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,searchRegionX,x,y,pointLocAggregationStrip[stepStrip+stepImage+x],Sdiff);
								//W 	log(text);
								//W 	}
								}//if(x>=(maxDisp-1))
							else//if(x>=(maxDisp-1))
								pointLocAggregationStrip[stepStrip+stepLineStrip+x]=195075;//(255*255)+(255*255)+(255*255)=195075 tem que ser alto para evitar falsa correspondência, lembre-se que na fase de Evidence Aggregation  somasse os pontos da janela e que na fase de Disparity Seletion é escolhido o menor valor (WTA), se fosse um valor baixo como 0 pode fazer parecer um winner 
							}//for(x=0
						}//if(y>=yVar)
					else//if(y>=yVar)
						{
						for(x=0;x<width;x++)//havia um bug aqui, eu havia esquecido de colocar o for, e por isso era preenchido somente um pixel
							pointLocAggregationStrip[stepStrip+stepLineStrip+x]=195075;//(255*255)+(255*255)+(255*255)=195075 tem que ser alto para evitar falsa correspondência, lembre-se que na fase de Evidence Aggregation  somasse os pontos da janela e que na fase de Disparity Seletion é escolhido o menor valor (WTA), se fosse um valor baixo como 0 pode fazer parecer um winner 
						}
					}//for(y=0
				}//for(searchRegionX=0
		jumpLine=0;//Na próxima rodada a primeira linha será removida
		}//if(yFull==n)
	if((n<yFull)&&(yFull<height-n)) //Demais linhas,exceto a primeira e as últimas(height-n)
		{
		pointLocImgR=(IMGpixel *)voidImgR;//aponta ponteiro local para a imagem da Direita, dentro do for este ponteiro não é incrementado, só por isso ele é definido fora do for
		//for(searchRegionY=-yVar;searchRegionY<=yVar;searchRegionY++)//caso particular de seach 1D maxLineVar=1 yVar será 0
		for(searchRegionY=0;searchRegionY<maxLineVar;searchRegionY++)
			for(stepStrip=0,searchRegionX=0;searchRegionX<maxDisp;searchRegionX++)
				{
				//stepStrip=(searchRegionY+yVar)*searchRegionX*stripSize;//Calcula uma única vez para cada strip, como searchRegionY vai de -2 até 2 (quando  maxLineVar é 5 e yVar será 2) e aqui é necessário que oscile de 0 até 4 (inclusive) yVar é somado à searchRegionY que fica de -2+2=0 até 2+2=4
				stepStrip=(searchRegionY*maxDisp*stripSize)+(searchRegionX*stripSize);//(searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 

								
				//stepStrip=searchRegionX*stripSize;//Calcula uma única vez para cada strip
				
		
				y=yFull+n;//y será a próxima linha abaixo, pois a demais já estão dentro da Aggregation Strip do loop anterior
				//e for(y=0;y<windowSize;y++)//Carga da primeira Aggregation Strip, a altura da strip é igual a windowSize
				//e	{
				stepImageUnderStrip=jumpLine*width;//Indica a linha que será descartada da rodada anterior


				
				if(	(y>=yVar)&&(y<=((int)height-(1+yVar)))	)// na primeira linha válida(if(yFull==n)) nem precisava a segunda parte y<=(height-(1+yVar)) porém optei por manter para evitar futuros erros em manutenções do código
					{
					//stepImageR=(y+searchRegionY)*width;//Calcula uma única vez para cada y. o searchRegionY irá fazer o y oscilar para cima e para baixo do valor que referência da imagem da esqueda, pois altera stepImage que é referência para increment que é usado para deslocar a imagem da direita.
					stepImageR=(y+searchRegionY-yVar)*width;
					stepImageL=y*width;//Calcula uma única vez para cada y 
					
					pointLocImgL=(IMGpixel *)voidImgL;
					pointLocImgL+=stepImageL;//avança para a linha certa
					for(x=0;x<width;x++,pointLocImgL++)//,pointLocImgR++)//,pointLocSquaredDiff++ )
						{
						if(x>=(maxDisp-1))//se o maxDisp é 10 a searchRegionX vai de 0 a 9, logo o minimo que eu posso aceitar de searchRegionX é 9 que é maxDisp menos um, para que a subtração de x não suba uma linha (para y=0 existe o risco de acessar memória não alocada)
							{
							increment=stepImageR+(x-searchRegionX);//Calcula uma única vez para cada x 

							tempL=pointLocImgL->r;//eu uso ponteiro puro aqui, por ser mais rápido (não tem que fazer conta)
							tempR=pointLocImgR[increment].r;
							tempDiff=tempL-tempR;
							Sdiff=tempDiff*tempDiff;
							//w if((searchRegionX==2)&&(y==3)&&(x<50))
							//w 	{
							//w 	LPCTSTR pszFormat = TEXT("R;x;%d;y;%d;increment;%d;tempR;%d;%d;tempL;%d;%d;tempDiff;%d\n");
							//w 	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,increment,tempR,pointLocImgR[increment].r,tempL,pointLocImgL->r,Sdiff);
							//w 	log(text);
							//w 	}
			
							tempL=pointLocImgL->g;
							tempR=pointLocImgR[increment].g;
							tempDiff=tempL-tempR;
							Sdiff=(tempDiff*tempDiff)+Sdiff;
							//w if((searchRegionX==2)&&(y==3)&&(x<50))
							//w 	{
							//w 	LPCTSTR pszFormat = TEXT("G;x;%d;y;%d;increment;%d;tempR;%d;%d;tempL;%d;%d;tempDiff;%d\n");
							//w 	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,increment,tempR,pointLocImgR[increment].g,tempL,pointLocImgL->g,Sdiff);
							//w 	log(text);
							//w 	}
			
							tempL=pointLocImgL->b;
							tempR=pointLocImgR[increment].b;
							tempDiff=tempL-tempR;
							Sdiff=(tempDiff*tempDiff)+Sdiff;
							//w if((searchRegionX==2)&&(y==3)&&(x<50))
							//w 	{
							//w 	LPCTSTR pszFormat = TEXT("B;x;%d;y;%d;increment;%d;tempR;%d;%d;tempL;%d;%d;tempDiff;%d\n");
							//w 	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,increment,tempR,pointLocImgR[increment].b,tempL,pointLocImgL->b,Sdiff);
							//w 	log(text);
							//w 	}

							pointLocAggregationStrip[stepStrip+stepImageUnderStrip+x]=Sdiff;
						//e	if((y==10)&&(x>230)&&(x<250))
						//e	  	{
						//e	 	LPCTSTR pszFormat = TEXT("searchRegionY;t;searchRegionX;%d;x;%d;y;%d;AggregationStrip;%d;%d\n");
						//e	 	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,searchRegionX,x,y,pointLocAggregationStrip[stepStrip+stepImageUnderStrip+x],Sdiff);
						//e	 	log(text);
						//e	 	}
							}//if(x>=(maxDisp-1))
						else
							pointLocAggregationStrip[stepStrip+stepImageUnderStrip+x]=195075;//(255*255)+(255*255)+(255*255)=195075 tem que ser alto para evitar falsa correspondência, lembre-se que na fase de Evidence Aggregation  somasse os pontos da janela e que na fase de Disparity Seletion é escolhido o menor valor (WTA), se fosse um valor baixo como 0 pode fazer parecer um winner 
						}//for(x=0
					}//if(y>=yVar)
				else//if(y>=yVar)
					{
					for(x=0;x<width;x++)//havia um bug aqui, eu havia esquecido de colocar o for, e por isso era preenchido somente um pixel
						pointLocAggregationStrip[stepStrip+stepImageUnderStrip+x]=195075;//(255*255)+(255*255)+(255*255)=195075 tem que ser alto para evitar falsa correspondência, lembre-se que na fase de Evidence Aggregation  somasse os pontos da janela e que na fase de Disparity Seletion é escolhido o menor valor (WTA), se fosse um valor baixo como 0 pode fazer parecer um winner 
					}
				//e	}//for(y=0
				}//for(searchRegionX=0
		jumpLine+=1;
		if(jumpLine==windowSize) jumpLine=0;
		}//else do if(yFull==n)
		}//if((yFull>=n)&&(yFull<height-n))
	
	//Evidence Aggregation
	for(searchRegionY=0;searchRegionY<maxLineVar;searchRegionY++)//aqui o for é diferente do for usado para esta mesma variável no Matching Cost, pois lá ela é usada principalmente para deslocar a linha de procura da imagem da direita o que exige que ela oscile de -yVar até yVar, como para stepStrip searchRegionY precisa oscilar de 0 até maxLineVar-1 é usada uma operação searchRegionY+yVar para conseguir esta oscilação. Como aqui o objetivo é varrer as Aggregation Strips e as Disparity Selection Lines faz mais sentido que o for oscile de 0 até maxLineVar-1
		for(stepLine=0,searchRegionX=0;searchRegionX<maxDisp;searchRegionX++)
		{
		stepLine=(searchRegionY*maxDisp*width)+(searchRegionX*width);//Calcula uma única vez para cada line, variável para salto entre as Disparity Selection Lines, (searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
		stepStrip=(searchRegionY*maxDisp*stripSize)+(searchRegionX*stripSize);//Calcula uma única vez para cada strip, variável de salto entre da Aggregation Strip, (searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
		//stepStrip=searchRegionX*stripSize;//Calcula uma única vez para cada strip
		//stepLine=searchRegionX*width;//Calcula uma única vez para cada linha
	//e	for(y=0;y<windowSize;y++)//Carga da primeira Aggregation Strip, a altura da strip é igual a windowSize
	//e		{
			//stepImage=y*width;//Calcula uma única vez para cada y 
			if((yFull>=n)&&(yFull<height-n))
				{
				for(x=0;x<width;x++)//,pointLocImgR++)//,pointLocSquaredDiff++ )
					{
					//borda de proteção:para width= 500 e n=1 o máximo x somente pode ser 499, isto é, width-1-n
					//borda de proteção:para n=1 o mínimo y somente pode ser 1
					//borda de proteção:range=10 representa um deslocamento de disparidade(x-layer) na imagem direita de 10 pixels, logo o minimo x aceitavel é 9 (range-1) 
					//considerando que o primeiro x válido(em relação ao range) fica sendo 9 logo o pixel x=9 seria o primeiro válido, agora considerando um n=2 o menor x aceitavel passa a ser 11
					//então para range=10 e n=2 o primeiro x válido é 11 temos (range-1)+n
					if((x>=(maxDisp-1)+n)&&(x<=(width-1)-n))//borda não tratada
						{
						if(x==((maxDisp-1)+n))
							{//usa-se n no lugar de y pois n é a metade da altura da strip ex. para windowSize=3 n=(3-1)/2=2/2=1 logo temo y=0,y=1 e y=2 sendo que 1 é o do meio, que é igual a n
							sumFirstWindow(width,x,n,n,&pointLocAggregationStrip[stepStrip],pointLocDispSelectLine,stepLine);
							}
						else
							{
							sumNextWindow(width,x,n,n,&pointLocAggregationStrip[stepStrip],pointLocDispSelectLine,stepLine);
							}
						}//if((x>=(range-1)+n)
					else//dentro do X precisa ter um tratamento para zerar a parte não tratada, o que não é necessário para o y, pois para y não tratado, não tem strip e nem selectionLine.
						{
						//i BitmapL.SetPixel(x, y, Color(255,0,125,255)); 
						pointLocDispSelectLine[stepLine+x]=0;
						}

				//u	if((yFull==11)&&(x>120)&&(x<250))
				//u		{
				//u		LPCTSTR pszFormat = TEXT("x;%d;yFull;%d;stepLine;%d;pointLocDispSelectLine;%d\n");
				//u		StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,yFull,stepLine,pointLocDispSelectLine[stepLine+x]);
				//u		log(text);
				//u		}
					}//for(x=0
				}//if((yFull>=n)&&(yFull<height-n))
			//kelse
			//k	{
			//k	//i BitmapL.SetPixel(x, y, Color(255,0,125,255)); 
			//k	pointLocDispSelectLine[stepLine+x]=0;
			//k	}
		//e	}//for(y=0
		}//for(searchRegionX=0


	//Disparity Selection WTA (Winneer Take-All)
	unsigned long bestResult;
	unsigned long disp;
	unsigned int part,color;
	part=255/maxDisp;//250 ao invés de 255 

	for(x=0;x<width;x++)//,pointLocImgR++)//,pointLocSquaredDiff++ )
		{
		//Pressupoe que a primeira linha (primeira strp) já é p melhor resultado
		disp=0;
		bestResult=pointLocDispSelectLine[x];//pressupoe que é a primeira linha stepLine=0
		//e if((yFull==10)&&(x>40)&&(x<60))
		//e				{
		//e				LPCTSTR pszFormat = TEXT("x;%d;yFull;%d;bestResult;%d\n");
		//e				StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,yFull,bestResult);
		//e				log(text);
		//e				}
		for(searchRegionY=0;searchRegionY<maxLineVar;searchRegionY++)
			for(stepLine=0,searchRegionX=0;searchRegionX<maxDisp;searchRegionX++)
				{
				stepLine=(searchRegionY*maxDisp*width)+searchRegionX*width;//Calcula uma única vez para cada line, variáve para salto entre as Disparity Selection Lines, (searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
				//stepLine=searchRegionX*width;//Calcula uma única vez para cada linha
		//e		if((yFull==10)&&(x>40)&&(x<60))
		//e		{
		//e		LPCTSTR pszFormat = TEXT("x;%d;yFull;%d;stepLine;%d;disp;%d;pointLocDispSelectLine;%d\n");
		//e		StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,yFull,stepLine,disp,pointLocDispSelectLine[stepLine+x]);
		//e		log(text);
		//e		}
				if((yFull>=n)&&(yFull<height-n))
					if(bestResult>pointLocDispSelectLine[stepLine+x])
						{
						disp=searchRegionX;
						bestResult=pointLocDispSelectLine[stepLine+x];
						}
				//else não é necessário, pois disp e bestResult já tem um valor pressuposto.
				}//for(searchRegionX

		//c if((yFull==10)&&(x>40)&&(x<60))
		//c		{
		//c		LPCTSTR pszFormat = TEXT("x;%d;yFull;%d;stepLine;%d;disp;%d;bestResult;%d\n");
		//c		StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,yFull,stepLine,disp,bestResult);
		//c		log(text);
		//c		}
		color=part*disp;
		pointLocDispMap[(yFull*width)+x]=color;//disp;
		
		//é usado color e não o ponteiro pointLocDispMap[] pois para acessar os seus conteúdos é necessária multiplicação, logo é mais eficiente usar uma variável já como o valor da cor
		DispMap.SetPixel(x, yFull, Color(255,color,color,color)); //lembre-se o alfa TEM que ser 255
		}//for(x=0
	


	
	}//for(yFull=0



//m	//este código de comparação exige que o processo de stereo matching tradicional (chamar a função matchingCost(HDC hdc,unsigned int width,unsigned int height,unsigned int range)) primeiro
//m	dispMap *pointLocDispMapLegacy;
//m	pointLocDispMapLegacy=(dispMap *)voidDispMapLegacy;
//m	unsigned long yProdWidth;

//m	for(y=0;y<height;y++)
//m		{
//m		yProdWidth=y*width;//Calcula uma única vez para cada y
//m		for(x=0;x<width;x++)
//m			{
//m			if(pointLocDispMapLegacy[yProdWidth+x].disp!=pointLocDispMap[yProdWidth+x])
//m				{
//m				LPCTSTR pszFormat = TEXT("x;%d;y;%d;pointLocDispMapLegacy;%d;pointLocDispMap;%d\n");
//m				StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,pointLocDispMapLegacy[yProdWidth+x].disp,pointLocDispMap[yProdWidth+x]);
//m				log(text);
//m				}
//m			}
//m		}


	

	CLSID  clsid;
    //GetEncoderClsid(L"image/jpeg", &clsid);//veja os encoders disponiveis no Windows 2000
	GetEncoderClsid(L"image/bmp", &clsid);
	Graphics graphics(hdc);//i
	
	//No futuro fazer um tratamento para o usuário selecionar o formato do arquivo de saída do diparity map
	if(compare) DispMap.Save(L"DispTsukubaNew.bmp", &clsid, NULL);//teste para comparação como os nomes de saída tem que ser diferentes, por isso forço um nome especifico
	else DispMap.Save(GdisparityMap, &clsid, NULL);
	
	//graphics.DrawImage(&DispMap, DispMap.GetWidth()+10, 0,DispMap.GetWidth(),DispMap.GetHeight());

	dispGenerated=true;

	

	


	//w graphics.DrawImage(&DispMap, 384+10, 0,384,288);

	delete(voidAggregationStrip);
	delete(voidDispSelectLine);
	}









//cuidados: N tem que ser impar e maior que 3
//			x tem que ser maior que 0+n
//			y tem que ser maior que 0+n
void sumFirstWindow(unsigned long width,unsigned long x,unsigned long y,unsigned long n,unsigned long *AggregationStrip,unsigned long *DispSelectLine,unsigned long stepLine)
	{
	//h WCHAR text[40];
	//h LPCTSTR pszFormat = TEXT("%d;%d;%0.0f;%0.0f\n");
	unsigned long xi,yi;
	unsigned long yiProdWidth;


	unsigned long tempSum;

	
	tempSum=0;
	for(yi=y-n;yi<=y+n;yi++)//no fundo equivale a um for de yi=0 até yi<windowSize, pois y é igual a n então o for começa de yi=n-n=0 até yi<=n+n=2n repare que é <=, logo temos para windowSize=3, n=1 então temos o for de yi=0 até yi<=2 que é a mesma coisa que yi<3
		{
		yiProdWidth=yi*width;
		for(xi=x-n;xi<=x+n;xi++)
			{
			tempSum=tempSum+AggregationStrip[yiProdWidth+xi];
			}
		}//for(yi=y-n

	DispSelectLine[stepLine+x]=tempSum;
	//if(tempSum<0)
	//	{
	//	WCHAR text[40];
	//	LPCTSTR pszFormat = TEXT("First;%0.0f\n");
	//	StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,tempSum);
	//	log(text);
	//	}
	}

//cuidados: o kernel (x-1,y) tem que estar calculado
//			N tem que ser impar e maior que 3
//			x tem que ser maior que 0+n
//			y tem que ser maior que 0+n
///	tá dando resultado negativo, como é possivel: descontinuidade.... deixe rodar na imagem inteira....
void sumNextWindow(unsigned long width,unsigned long x,unsigned long y,unsigned long n,unsigned long *AggregationStrip,unsigned long *DispSelectLine,unsigned long stepLine)
	{
	//WCHAR text[40];
	
	unsigned long xi,yi;
	unsigned long tempSum;

	tempSum=DispSelectLine[stepLine+(x-1)];//carrega tempSum com o valor (x-1,y)
	
	
	//remove a coluna anterior
	xi=x-n-1;
	for(yi=y-n;yi<=y+n;yi++)
		{
		tempSum=tempSum-AggregationStrip[(yi*width)+xi];//a subtração não tem perigo de gerar resultado negativo, pois está retirando o que havia sido adicionado na rodada anterior
		tempSum=tempSum+AggregationStrip[(yi*width)+x+n];//adiciona a próxima coluna
		}//for(yi=y-n

	//adiciona a próxima coluna
/*	xi=x+n;
	for(yi=y-n;yi<=y+n;yi++)
		{
		tempSum=tempSum+AggregationStrip[(yi*width)+xi];
		}//for(yi=y-n*/

	DispSelectLine[stepLine+x]=tempSum;
	//if(tempSum<0)
	//	{
	//	WCHAR text[40];
	//	LPCTSTR pszFormat = TEXT("Next;%0.0f\n");
	//	StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,tempSum);
	//	log(text);
	//	}

	}
//Método Tradicional
//Matching Cost
void matchingCost(HDC hdc,unsigned int width,unsigned int height,unsigned int range)
	{
	void *voidSquaredDiff;
	//e WCHAR text[200];
	//h WCHAR text[40];
	//h LPCTSTR pszFormat = TEXT("%d;%d;%d;%d;%d;%d;%d;%0.0f\n");
   	unsigned long layerProdImgSize,yProdWidth,increment;
	unsigned int x,y,layer;
	unsigned long ImgSize;
	float tempL,tempR,tempDiff,Sdiff;
	IMGpixel *pointLocImgL,*pointLocImgR;
	float *pointLocSquaredDiff;

	if(range==0) range=1;//range=0 é invalido
	ImgSize=width*height;
	voidSquaredDiff= new float[ImgSize*range];

	pointLocSquaredDiff=(float *)voidSquaredDiff;
	
	pointLocImgR=(IMGpixel *)voidImgR;

	startT=clock();//Contabiliza somente a execução do código de procura de correspondências
	for(layer=0;layer<range;layer++)
		{
		layerProdImgSize=layer*ImgSize;//Calcula uma única vez para cada layer 
		pointLocImgL=(IMGpixel *)voidImgL;
		
		for(y=0;y<height;y++)
			{
			yProdWidth=y*width;//Calcula uma única vez para cada y 

			for(x=0;x<width;x++,pointLocImgL++)//,pointLocImgR++)//,pointLocSquaredDiff++ )
				{
				if(x>=(range-1))//se o range é 10 a layer vai de 0 a 9, logo o minimo que eu posso aceitar de layer é 9 que é range menos um, para que a subtração de x não suba uma linha (para y=0 existe o risco de acessar memória não alocada)
					{
					increment=yProdWidth+(x-layer);//Calcula uma única vez para cada x 

					tempL=(float)pointLocImgL->r;//eu uso ponteiro puro aqui, por ser mais rápido (não tem que fazer conta)
					//tempR=(float)pointLocImgR->r;
					tempR=(float)pointLocImgR[increment].r;
					tempDiff=tempL-tempR;
					Sdiff=tempDiff*tempDiff;
		//i			if((layer==2)&&(y==3)&&(x<50))
		//i			 		{
		//i			 		LPCTSTR pszFormat = TEXT("OR;x;%d;y;%d;increment;%d;tempR;%0.0f;%0.0f;tempL;%0.0f;%0.0f;tempDiff;%0.0f\n");
		//i			 		StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,increment,tempR,(float)pointLocImgR[increment].r,tempL,(float)pointLocImgL->r,Sdiff);
		//i			 		log(text);
		//i			 		}
			
					tempL=(float)pointLocImgL->g;
					//tempR=(float)pointLocImgR->g;
					tempR=(float)pointLocImgR[increment].g;
					tempDiff=tempL-tempR;
					Sdiff=(tempDiff*tempDiff)+Sdiff;
		//i			if((layer==2)&&(y==3)&&(x<50))
		//i			 		{
		//i			 		LPCTSTR pszFormat = TEXT("OG;x;%d;y;%d;increment;%d;tempR;%0.0f;%0.0f;tempL;%0.0f;%0.0f;tempDiff;%0.0f\n");
		//i			 		StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,increment,tempR,(float)pointLocImgR[increment].g,tempL,(float)pointLocImgL->g,Sdiff);
		//i			 		log(text);
		//i			 		}
			
					tempL=(float)pointLocImgL->b;
					//tempR=(float)pointLocImgR->b;
					tempR=(float)pointLocImgR[increment].b;
					tempDiff=tempL-tempR;
					Sdiff=(tempDiff*tempDiff)+Sdiff;
		//i			 if((layer==2)&&(y==3)&&(x<50))
		//i			 		{
		//i			 		LPCTSTR pszFormat = TEXT("OB;x;%d;y;%d;increment;%d;tempR;%0.0f;%0.0f;tempL;%0.0f;%0.0f;tempDiff;%0.0f\n");
		//i			 		StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,increment,tempR,(float)pointLocImgR[increment].b,tempL,(float)pointLocImgL->b,Sdiff);
		//i			 		log(text);
		//i			 		}

					pointLocSquaredDiff[layerProdImgSize+yProdWidth+x]=Sdiff;
					//e if((y==10)&&(x>230)&&(x<250))
					//e 		{
					//e 		LPCTSTR pszFormat = TEXT("searchRegionY;n;searchRegionX;%d;x;%d;y;%d;AggregationStrip;%0.0f;%0.0f\n");
					//e 		StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,layer,x,y,pointLocSquaredDiff[layerProdImgSize+yProdWidth+x],Sdiff);
					//e		log(text);
					//e 		}
					}//if(x>=(range-1))
				else
					pointLocSquaredDiff[layerProdImgSize+yProdWidth+x]=0;
				}//for(x=0
			}//for(y=0
		}//for(layer=0

	//d WCHAR text[40];
	//d LPCTSTR pszFormat = TEXT("%0.0f\n");
	//d StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,pointLocSquaredDiff[1560]);
	//d log(text);
	
	// Agregação
	aggregation(width,height,range,windowSizeForCompare,&pointLocSquaredDiff[0]);
	
	// Seleção
	selection(hdc,width,height,range,&pointLocSquaredDiff[0]);
	
	delete(voidSquaredDiff);
	}




//WTA (Winneer Take-All)
void selection(HDC hdc,unsigned long width,unsigned long height,unsigned int range,float *squaredDiff)
	{
	//WCHAR text[250];
	unsigned long yProdWidth;
	unsigned int x,y,layer;
	unsigned int disp;
	float SSDtemp;
	//diferenças entre tradicional e stripping technique (comentarios com //e)
	//e dispMap *pointLocDispMap;
	unsigned long *pointLocDispMap;
	unsigned int part,color;
	unsigned long ImgSize;

	ImgSize=width*height;

	part=255/range;
	
	

	//diferenças entre tradicional e stripping technique (comentarios com //e)
	//e  voidDispMapLegacy= new dispMap[width*height];// é deletado somente no fim do programa
	//e  pointLocDispMap=(dispMap *)voidDispMapLegacy;
	voidDispMapLegacy= new unsigned long[width*height];// é deletado somente no fim do programa
	pointLocDispMap=(unsigned long *)voidDispMapLegacy;
	

	for(y=0;y<height;y++)
		{
		yProdWidth=y*width;//Calcula uma única vez para cada y
		for(x=0;x<width;x++)
			{
			disp=0;//pressupoe que a layer 0 tem o menor valor de Soma dos Quadrados das Diferençaas
			SSDtemp=squaredDiff[yProdWidth+x];
			for(layer=1;layer<range;layer++)//como o range tem que ser no mínimo 1, logo layer pode começar a partir de 1, uma vez que eu já pressupus que 0 é a menor disparidade
				{
				//if((y==1)&&(x>40)&&(x<200))
				//	{
				//	LPCTSTR pszFormat = TEXT("x;%d;y;%d;stepLine;N;disp;%d;squaredDiff;%0.0f\n");
				//	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,disp,squaredDiff[(layer*ImgSize)+yProdWidth+x]);
				//	log(text);
				//	}
				if(SSDtemp>squaredDiff[(layer*ImgSize)+yProdWidth+x])
					{
					disp=layer;
                    SSDtemp=squaredDiff[(layer*ImgSize)+yProdWidth+x];
					}
				}//for(layer=1
			//c if((y==1)&&(x>40)&&(x<200))
			//c	{
			//c	LPCTSTR pszFormat = TEXT("x;%d;y;%d;stepLine;N;disp;%d;SSDtemp;%0.0f\n");
			//c	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,disp,SSDtemp);
			//c	log(text);
			//c	}

		//diferenças entre tradicional e stripping technique (comentarios com //e)
			//e pointLocDispMap[yProdWidth+x].disp=disp;
			//e pointLocDispMap[yProdWidth+x].type=0;

			color=part*disp;
			pointLocDispMap[yProdWidth+x]=color;//disp;
			
			//é usado color e não o ponteiro pointLocDispMap[] pois para acessar os seus conteúdos é necessária multiplicação, logo é mais eficiente usar uma variável já como o valor da cor
//e			DispMap.SetPixel(x, y, Color(255,color,color,color)); 
			}//	for(x=0	
		}//for(y=0
	finishT=clock(); //Contabiliza somente a execução do código, retirando a etapa para salvar o arquivo
	durationT=finishT-startT;
	

	Bitmap DispMap(GleftFile);//left);//i
	unsigned int xs,ys;
	unsigned long ysProdWidth;
	pointLocDispMap=(unsigned long *)voidDispMapLegacy;

	for (ys=0; ys<height;ys++)
		{
		ysProdWidth=ys*width;
		for (xs=0; xs<width;xs++)
			{
			color=pointLocDispMap[(ys*width)+xs];
			DispMap.SetPixel(xs, ys, Color(255,color,color,color)); //lembre-se o alfa TEM que ser 255
			}
		}
	
//diferenças entre tradicional e stripping technique (comentarios com //e)
	//sameLayer(pointLocDispMap,width,height,0,0,pointLocDispMap[0].disp);
	//e sameLayer(pointLocDispMap,width,height);
	//sameLayer(pointLocDispMap,width,height,110,110,pointLocDispMap[(110*width)+110].disp);
	//Quatro cantos, são pixels relevantes que se tornarão vertices.CUIDADO (0,0) NÃO PODE SER CONFIGURADO COMO type 2 ANTES DE sameLayer()
	//e pointLocDispMap[0].type=3;//x=0 e y=0// Caso especial como a procura começa pelo (0,0) ele não pode ser type 2 antes de chamar sameLayer() mas depois tem que ser configurado como 2, pois é um canto.
	//e pointLocDispMap[width-1].type=3;//x=0 e y=height-1
	//e pointLocDispMap[width*(height-1)].type=3;//x=width e y=0
	//e pointLocDispMap[(width*height)-1].type=3;//x=width-1 e y=height-1

	//v WCHAR text[40];
	//v LPCTSTR pszFormat = TEXT("%d\t%d\t%d\t%d\n");

//t	//marca pixels com cores para identificar tipos
//t	for(y=0;y<height;y++)
//t		{
//t		yProdWidth=y*width;//Calcula uma única vez para cada y
//t		for(x=0;x<width;x++)
//t			{
//t			//v StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,x,y,yProdWidth+x,pointLocDispMap[yProdWidth+x].disp);
//t			//v log(text);
//t			if(pointLocDispMap[yProdWidth+x].type==2)//Pinta de vermelho pixels relevantes
//t    				DispMap.SetPixel(x, y, Color(255,255,0,0));
//t			if(pointLocDispMap[yProdWidth+x].type==3)//Pinta de vermelho pixels relevantes
//t   				DispMap.SetPixel(x, y, Color(255,255,0,0));
//t			//if(pointLocDispMap[yProdWidth+x].type==1)//Pinta de azul pixels irrelevantes
//t    		//		DispMap.SetPixel(x, y, Color(255,0,0,255)); 
//t			}
//t		}



	//d pszFormat = TEXT("%0.0f\n");
	//d StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,pointLocSquaredDiff[1560]);
	//d log(text);

	
	CLSID  clsid;
    //GetEncoderClsid(L"image/jpeg", &clsid);//veja os encoders disponiveis no Windows 2000
	GetEncoderClsid(L"image/bmp", &clsid);
	Graphics graphics(hdc);//i
	
	
	DispMap.Save(disparityMap, &clsid, NULL);

	//u graphics.DrawImage(&DispMap, DispMap.GetWidth()+10, 0,DispMap.GetWidth(),DispMap.GetHeight());
	
	//diferenças entre tradicional e stripping technique
	if(compare==false) graphics.DrawImage(&DispMap, 384+10, 0,384,288); //se estiver comparando, não apresenta na tela, pois a CorrespSearch() não apresenta. Para Tsukuba com janela 9 e Max Disp 16 a retirada desta linha economizou 20ms
	//e graphics.DrawImage(&DispMap, 384+10, 0,384,288);

	}

//usar o voidDispMap na função recursiva

// Objetivo da sameLayer:
// Varrer pixels com conectividade 4 e checar se estão na mesma camada de disparidade, se sim ignora o pixel não 
// tranformando-o em vertice. Caso o pixel esteja em outra camada o mesmo é duplicado e representado nas duas camadas
// tipos de pixel
// 0 - Não analisado
// 1 - Pixel não relevante, não vira Vertice
// 2 - Pixel relevante, candidato a virar Vertice de transição entre duas layers
// 3 - Pixel relevante, vertice dos cantos

// antes de chama-la 
//	pointLocDispMap[0].type=2;//x=0 e y=0
//	pointLocDispMap[width-1].type=2;//x=0 e y=height-1
//	pointLocDispMap[width*(height-1)].type=2;//x=width e y=0
//	pointLocDispMap[(width*height)-1].type=2;//x=width-1 e y=height-1


void sameLayer(dispMap *pointDispMap,unsigned long width,unsigned long height)
	{
	unsigned long x,y;
	unsigned long yProdWidth;
	int disp;

	disp=pointDispMap[0].disp;//pega a primeira disparidade como referência
	for(y=0;y<height;y++)
		{
		yProdWidth=y*width;//Calcula uma única vez para cada y
		for(x=0;x<width;x++)
			{
			if(pointDispMap[x+yProdWidth].disp==disp) 
				pointDispMap[x+yProdWidth].type=1;//Pixel não relevante, não se tornará vertice
			else
				{
				pointDispMap[x+yProdWidth].type=2;
				disp=pointDispMap[x+yProdWidth].disp;
				}//else
			}//for(x=0;x<width;x++)
		}//for(y=0;y<height;y++)

	disp=pointDispMap[0].disp;//pega a primeira disparidade como referência
	for(x=0;x<width;x++)
		{
		for(y=0;y<height;y++)
			{
			yProdWidth=y*width;//Calcula uma única vez para cada y
			if(pointDispMap[x+yProdWidth].disp!=disp) 
				{
				pointDispMap[x+yProdWidth].type=2;
				disp=pointDispMap[x+yProdWidth].disp;
				}//if(pointDispMap[x+yProdWidth].disp!=disp) 
			}//for(x=0;x<width;x++)
		}//for(y=0;y<height;y++)
	}
//esta função recursiva ficou muito pesada e "derrubava" o programa (por teste a metade está comentada..) fiz uma versão não recursiva que ficou boa.
void sameLayer(dispMap *pointDispMap,unsigned long width,unsigned long height,unsigned long x,unsigned long y,int disp)
	{
	unsigned long yProdWidth;
	yProdWidth=y*width;//Calcula uma única vez para cada y

	if(pointDispMap[x+yProdWidth].type==0)//ainda não foi analisado
		{
		if(pointDispMap[x+yProdWidth].disp==disp)
			{
			pointDispMap[x+yProdWidth].type=1;//Pixel não relevante, não se tornará vertice

			if(x<width)
				sameLayer(pointDispMap,width,height,x+1,y,disp);
			if(x>0)
				sameLayer(pointDispMap,width,height,x-1,y,disp);
			if(y>0)
				sameLayer(pointDispMap,width,height,x,y-1,disp);
			if(y<height)
				sameLayer(pointDispMap,width,height,x,y+1,disp);
			}//fecha if do disp
		
		else//se trata de uma borda
			{

			pointDispMap[x+yProdWidth].type=2;//Pixel candidato a virar vertice, o tipo somente pode ser modificado depois de fazer a procura.
			/*
			//procura com nova disparidade
			if(x<width)
				sameLayer(pointDispMap,width,height,x+1,y,pointDispMap[x+yProdWidth].disp);
			if(x>0)
				sameLayer(pointDispMap,width,height,x-1,y,pointDispMap[x+yProdWidth].disp);
			if(y>0)
				sameLayer(pointDispMap,width,height,x,y-1,pointDispMap[x+yProdWidth].disp);
			if(y<height)
				sameLayer(pointDispMap,width,height,x,y+1,pointDispMap[x+yProdWidth].disp);
			*/

			//MessageBox(NULL,L"Antes",L"stereo",MB_OK);
			//sameLayer(pointDispMap,width,height,x,y,pointDispMap[x+yProdWidth].disp);//procura com nova disparidade
			//pointDispMap[x+yProdWidth].type=2;//Pixel candidato a virar vertice, o tipo somente pode ser modificado depois de fazer a procura.
			//MessageBox(NULL,L"Depois",L"stereo",MB_OK);
			}//fecha else
		}//fecha if do type
	}



//Aggregation
void aggregation(unsigned long width,unsigned long height,unsigned int range,unsigned long Kernel,float *squaredDiff)
	{
	unsigned int layer;
	unsigned long layerProdImgSize;
	unsigned long ImgSize;

	ImgSize=width*height;

	for(layer=0;layer<range;layer++)
		{
		layerProdImgSize=layer*ImgSize;//Calcula uma única vez para cada layer 
		aggregationOneLayer(width,height,range,Kernel,&squaredDiff[layerProdImgSize]);
		}
	}


//faz a convolução usando um kernel NxN 
void aggregationOneLayer(unsigned long width,unsigned long height,unsigned int range,unsigned long Kernel,float *squaredDiff)
	{
	// WCHAR text[200];
	//h WCHAR text[40];
	//h LPCTSTR pszFormat = TEXT("%s,%d;%d;%0.0f\n");

	//float temp;
	unsigned long x,y,n;
	unsigned long yProdWidth;
	void *voidLocTempSquaredDiff;
	float *pointLocSquaredDiff,*pointLocTempSquaredDiff;

	if(Kernel%2==0)	{log(L"Kernel invalido, número par.\n");return;}
	if(Kernel<3)	{log(L"Kernel invalido, menor que 3.\n");return;}
	n=(Kernel-1)/2;//para N=3 n=1 para N=5 n=2 N é o tamanho do kernel e n é o incremento/decremento a ser feito em x e y

	//iGraphics graphics(hdc);
	//iBitmap BitmapL(L"tsukubaL.bmp");
	
	pointLocSquaredDiff=squaredDiff;

	voidLocTempSquaredDiff= new float[width*height];//Temporario
	pointLocTempSquaredDiff=(float *)voidLocTempSquaredDiff;

	for(y=0;y<height;y++)
		{
		yProdWidth=y*width;//Calcula uma única vez para cada y

		//depois coloque os if do y aqui fora
		if(y==n)//primeira linha válida para a agregação
			for(x=0;x<width;x++)
				{
				//borda de proteção:para width= 500 e n=1 o máximo x somente pode ser 499, isto é, width-1-n
				//borda de proteção:para n=1 o mínimo y somente pode ser 1
				//borda de proteção:range=10 representa um deslocamento de disparidade(x-layer) na imagem direita de 10 pixels, logo o minimo x aceitavel é 9 (range-1) 
				//considerando que o primeiro x válido(em relação ao range) fica sendo 9 logo o pixel x=9 seria o primeiro válido, agora considerando um n=2 o menor x aceitavel passa a ser 11
				//então para range=10 e n=2 o primeiro x válido é 11 temos (range-1)+n
				if((x>=(range-1)+n)&&(x<=(width-1)-n))//borda não tratada
					{
					if(x==((range-1)+n))
						{
						sumFirstKernel(width,x,y,n,squaredDiff,pointLocTempSquaredDiff);
						}
					else
						{
						sumNextKernel(width,x,y,n,squaredDiff,pointLocTempSquaredDiff);
						}
					}//if((x>=(range-1)+n)
				else//do if((x>=(range-1)+n)
					{
					//i BitmapL.SetPixel(x, y, Color(255,0,125,255)); 
					pointLocTempSquaredDiff[yProdWidth+x]=0;
					}

				//u if((y==11)&&(x>120)&&(x<250))
				//u	{
				//u	LPCTSTR pszFormat = TEXT("OriX;%d;y;%d;stepLine;%d;pointLocTempSquaredDiff;%0.0f\n");
				//u	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,yProdWidth,pointLocTempSquaredDiff[yProdWidth+x]);
				//u	log(text);
				//u	}
				}//for(x=0
		if((y>n)&&(y<=(height-1)-n))//demais linhas válidas para a agregação, otimização da técnica tradicional
			for(x=0;x<width;x++)
				{
				//borda de proteção:para width= 500 e n=1 o máximo x somente pode ser 499, isto é, width-1-n
				//borda de proteção:para n=1 o mínimo y somente pode ser 1
				//borda de proteção:range=10 representa um deslocamento de disparidade(x-layer) na imagem direita de 10 pixels, logo o minimo x aceitavel é 9 (range-1) 
				//considerando que o primeiro x válido(em relação ao range) fica sendo 9 logo o pixel x=9 seria o primeiro válido, agora considerando um n=2 o menor x aceitavel passa a ser 11
				//então para range=10 e n=2 o primeiro x válido é 11 temos (range-1)+n
				if((x>=(range-1)+n)&&(x<=(width-1)-n))//borda não tratada
					{
					sumKernel(width,x,y,n,squaredDiff,pointLocTempSquaredDiff);
					}//if((x>=(range-1)+n)
				else//do if((x>=(range-1)+n)
					{
					//i BitmapL.SetPixel(x, y, Color(255,0,125,255)); 
					pointLocTempSquaredDiff[yProdWidth+x]=0;
					}

				//u if((y==11)&&(x>120)&&(x<250))
				//u	{
				//u	LPCTSTR pszFormat = TEXT("OriX;%d;y;%d;stepLine;%d;pointLocTempSquaredDiff;%0.0f\n");
				//u	StringCbPrintfW(text, (200*sizeof(WCHAR)),pszFormat,x,y,yProdWidth,pointLocTempSquaredDiff[yProdWidth+x]);
				//u	log(text);
				//u	}
				}//for(x=0
		}//for(y=0

	//transfere o buffer temporário: pointLocTempSquaredDiff
	//para o buffer geral: pointLocSquaredDiff
	//carrega a layer com a soma dos quadrados das diferenças

//diferenças entre tradicional e stripping technique
	//buffer temporário
	 for(y=0;y<height;y++)
		{
		yProdWidth=y*width;//Calcula uma única vez para cada y
		for(x=0;x<width;x++)
			pointLocSquaredDiff[yProdWidth+x]=pointLocTempSquaredDiff[yProdWidth+x];//carrega a layer com a soma dos quadrados das diferenças
		}

	delete(voidLocTempSquaredDiff);
	//i graphics.DrawImage(&BitmapL, 0, 300,BitmapL.GetWidth(),BitmapL.GetHeight());
	}

//cuidados: N tem que ser impar e maior que 3
//			x tem que ser maior que 0+n
//			y tem que ser maior que 0+n
void sumFirstKernel(unsigned long width,unsigned long x,unsigned long y,unsigned long n,float *squaredDiff,float *sumSquaredDiff)
	{
	//h WCHAR text[40];
	//h LPCTSTR pszFormat = TEXT("%d;%d;%0.0f;%0.0f\n");
	unsigned long xi,yi;
	unsigned long yiProdWidth;


	float tempSum;

	
	tempSum=0;
	for(yi=y-n;yi<=y+n;yi++)
		{
		yiProdWidth=yi*width;
		for(xi=x-n;xi<=x+n;xi++)
			{
			tempSum=tempSum+squaredDiff[yiProdWidth+xi];
			}
		}//for(yi=y-n

	sumSquaredDiff[(y*width)+x]=tempSum;
	//if(tempSum<0)
	//	{
	//	WCHAR text[40];
	//	LPCTSTR pszFormat = TEXT("First;%0.0f\n");
	//	StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,tempSum);
	//	log(text);
	//	}
	}

//cuidados: o kernel (x-1,y) tem que estar calculado
//			N tem que ser impar e maior que 3
//			x tem que ser maior que 0+n
//			y tem que ser maior que 0+n
///	tá dando resultado negativo, como é possivel: descontinuidade.... deixe rodar na imagem inteira....
void sumNextKernel(unsigned long width,unsigned long x,unsigned long y,unsigned long n,float *squaredDiff,float *sumSquaredDiff)
	{
	//WCHAR text[40];
	
	unsigned long xi,yi;
	float tempSum;

	tempSum=sumSquaredDiff[(y*width)+(x-1)];//carrega tempSum com o valor (x-1,y)
	
	
	//remove a coluna anterior
	xi=x-n-1;
	for(yi=y-n;yi<=y+n;yi++)
		{
		tempSum=tempSum-squaredDiff[(yi*width)+xi];//a subtração não tem perigo de gerar resultado negativo, pois está retirando o que havia sido adicionado na rodada anterior
		tempSum=tempSum+squaredDiff[(yi*width)+x+n];//adiciona a próxima coluna
		}//for(yi=y-n

/*	//adiciona a próxima coluna
	xi=x+n;
	for(yi=y-n;yi<=y+n;yi++)
		{
		tempSum=tempSum+squaredDiff[(yi*width)+xi];
		}//for(yi=y-n*/

	sumSquaredDiff[(y*width)+x]=tempSum;
	//if(tempSum<0)
	//	{
	//	WCHAR text[40];
	//	LPCTSTR pszFormat = TEXT("Next;%0.0f\n");
	//	StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,tempSum);
	//	log(text);
	//	}

	}

void sumKernel(unsigned long width,unsigned long x,unsigned long y,unsigned long n,float *squaredDiff,float *sumSquaredDiff)
	{
	//WCHAR text[40];
	
	unsigned long xi,yProdWidthIn,yProdWidthOut;
	float tempSum;

	//note que aqui é y-1 pois o ponteiro é sumSquaredDiff
	tempSum=sumSquaredDiff[(((y-1)*width)+x)];//carrega tempSum com o valor (x,y-1)resultado da linha acima
	

	//note que aqui é (y-1-n) pois o ponteiro é squaredDiff
	yProdWidthOut=(y-1-n)*width;//uma linha da rodada anterior
	yProdWidthIn=(y+n)*width;//proxima linha
// as duas linhas acima poderiam ser otimizadas, sendo calculadas antes do for(x... que chamou sumKernel, de forma que não seria necessário recalcular a cada chamada.
	
	for(xi=x-n;xi<=x+n;xi++)
			{
			tempSum=tempSum-squaredDiff[yProdWidthOut+xi];//retira custos da linha descartada. Não tem perigo de dar negativo, (que seria um erro dificil de detectar) porque o valor que está sendo subtraido, havia sido adicionado na rodada anterior. 
			tempSum=tempSum+squaredDiff[yProdWidthIn+xi];//adiciona custos da linha adicionada.
			}//for(xi=x-n;xi<=x+n;xi++)

	sumSquaredDiff[(y*width)+x]=tempSum;
	//if(tempSum<0)
	//	{
	//	WCHAR text[40];
	//	LPCTSTR pszFormat = TEXT("Next;%0.0f\n");
	//	StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,tempSum);
	//	log(text);
	//	}

	}


//expandir para um range de disparidades....ok 
//tratar a layer subtraindo o x da imagem direita
//cuidado o OnPaint é chamado toda hora que mexe na janela e recarrega a imagem, pelo menos não calcula tudo de novo
//tratar aggregation 
//tá indo bem... acredito que pintar uma imagem dê um resultado visual das bordas ....
//testar a soma dos kernels.... os ifs estão validados

void carregaImgBuffer(IMGpixel *pointLocImg,Bitmap *bitmap)
	{
	Color pixelColor;

	for (UINT y = 0; y < bitmap->GetHeight(); y ++)
		for (UINT x = 0; x < bitmap->GetWidth(); x ++,pointLocImg++ )
			{
			bitmap->GetPixel(x, y, &pixelColor);

			pointLocImg->a=pixelColor.GetA();
			pointLocImg->r=pixelColor.GetR();
			pointLocImg->g=pixelColor.GetG();
			pointLocImg->b=pixelColor.GetB();
			}
	}

void carregaDispMapBuffer(unsigned long *pointLocImg,Bitmap *bitmap)
	{
	Color pixelColor;

	for (UINT y = 0; y < bitmap->GetHeight(); y ++)
		for (UINT x = 0; x < bitmap->GetWidth(); x ++,pointLocImg++ )
			{
			bitmap->GetPixel(x, y, &pixelColor);

			
			//*pointLocImg==pixelColor.GetR();
			*pointLocImg=pixelColor.GetG();//como são tons de cinza vou carregar pelo verde, mais poderia ser qualquer cor uma vez que sendo tons de cinza todas são iguais.
			//*pointLocImg==pixelColor.GetB();
			}
	}



void deleteBuffer(void)
	{
	if(voidImgL!=0) delete(voidImgL);
	if(voidImgR!=0) delete(voidImgR);
	if(voidDispMap!=0) delete(voidDispMap);
	if(voidDispMapLegacy!=0) delete(voidDispMapLegacy);//delete(pointLocDispMap);

	if(Pad!=0)		delete (Pad);
	if(BitmapL!=0)	delete (BitmapL);
	if(BitmapR!=0)	delete (BitmapR);
	if(DispMap!=0)	delete (DispMap);

	}

void paint(HDC hdc)
	{
	Graphics graphics(hdc);
		
	if(imagesLoaded)
		{
		if(BitmapL==0)	BitmapL = new Bitmap(GleftFile);//left);
		
		if(BitmapR==0) BitmapR = new Bitmap(GrightFile);//right);
		

		if(ImgBufferLoaded==false)//carrega buffers básicos, para permitir que no caso de portar para outro sistema operacional seja usado o buffer neste formato, ao invés do formato do GDI+
			{
			IMGpixel *pointLocImgL,*pointLocImgR;

			globalWidth=BitmapL->GetWidth();
			globalHeight=BitmapL->GetHeight();
	
			voidImgL= new IMGpixel[BitmapL->GetWidth()*BitmapL->GetHeight()];// é deletado somente no fim do programa
			voidImgR= new IMGpixel[BitmapR->GetWidth()*BitmapR->GetHeight()];// é deletado somente no fim do programa
	
			pointLocImgL=(IMGpixel *)voidImgL;
			pointLocImgR=(IMGpixel *)voidImgR;
	
			if(gradient)
				{	
				carregaImgBufferWithSobel(pointLocImgL,BitmapL);
				carregaImgBufferWithSobel(pointLocImgR,BitmapR);
				}
			else
				{
				carregaImgBuffer(pointLocImgL,BitmapL);
				carregaImgBuffer(pointLocImgR,BitmapR);
				}

			

            ImgBufferLoaded=true;
			}

		graphics.DrawImage(BitmapL, 0, 300,384,288);
		graphics.DrawImage(BitmapR, 384+10,300,384,288);

		WCHAR text[40];
		LPCTSTR pszFormat = TEXT("Left %d x %d\nRight %d x %d");
		StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat, BitmapL->GetWidth(),BitmapL->GetHeight(),BitmapR->GetWidth(),BitmapR->GetHeight());
		Text(hdc,text);
		}
	else
		{
		if(Pad==0) Pad = new Bitmap(hInstanceSave, MAKEINTRESOURCE(IDB_BITMAP1));//pad enquanto não gera o mapa de disparidade
		graphics.DrawImage(Pad, 0, 300,384,288);
		graphics.DrawImage(Pad, 384+10,300,384,288);
		}


	if(dispGenerated)//quando o mapa de disparidade é gerado pela CorrespSearch() o buffer interno já está carregado, porém quando o mapa de disparidade é lido é precisso carregar o buffer
		{
		if(DispMap==0) DispMap = new Bitmap(GdisparityMap);
		graphics.DrawImage(DispMap, 384+10, 0,384,288);

		if(LoadOnlyDispMap==true)
			{	
			//Mapa de disparidade lido sem ter sido carregado pela CorrespSearch()
			//Carrega os tamanhos da imagem caso seja lido somente o mapa de disparidade
			if(globalWidth==0)	globalWidth=DispMap->GetWidth();
			if(globalHeight==0)	globalHeight=DispMap->GetHeight();
			//void *voidDispMap; mudou para variavel global que será usada em D3D9.cpp para montar a malha 3D
			unsigned long *pointLocDispMap;

			voidDispMap= new unsigned long[globalWidth*globalHeight];
			pointLocDispMap=(unsigned long *)voidDispMap;
			//*****************************************************************
			carregaDispMapBuffer(pointLocDispMap,DispMap);//Carrega voidDispMap, pois o mapa de disparidades é carregado diretamente do arquivo sem chamar CorrespSearch()
			LoadOnlyDispMap=false; // parra não carregar repetidas vezes
			}
		}
	else
		{
		if(Pad==0) Pad = new Bitmap(hInstanceSave, MAKEINTRESOURCE(IDB_BITMAP1));//pad enquanto não gera o mapa de disparidade
		graphics.DrawImage(Pad, 384+10, 0,384,288);
		}
   
   
	
 	}



VOID OnPaint(HDC hdc)
{
	Graphics graphics(hdc);

	if(Pad==0) Pad = new Bitmap(hInstanceSave, MAKEINTRESOURCE(IDB_BITMAP1));//pad enquanto não gera o mapa de disparidade

	//carrega a imagem
	if(BitmapL==0)	BitmapL = new Bitmap(left);
	//Bitmap BitmapL(left);//(L"Sample.jpg");//(L"a.tif");//(L"a.jpg");//
	graphics.DrawImage(BitmapL, 0, 300,384,288);
	
	if(BitmapR==0) BitmapR = new Bitmap(right);
	//Bitmap BitmapR(right);//(L"a.jpg");//(L"Sample.jpg");//(L"a.tif");//
	graphics.DrawImage(BitmapR, 384+10,300,384,288);

	//Bitmap DispMap(L"null.bmp");//(disparityMap);//i
	
	if(dispGenerated)
		{
		if(DispMap==0) DispMap = new Bitmap(disparityMap);
		//Bitmap DispMap(disparityMap);
		graphics.DrawImage(DispMap, 384+10, 0,384,288);
		}
	else
		{
		
		//Bitmap DispMap(hInstanceSave, MAKEINTRESOURCE(IDB_BITMAP1));//pad enquanto não gera o mapa de disparidade
		graphics.DrawImage(Pad, 384+10, 0,384,288);
		}
   
	   
	//u Color pixelColor;

	//mostra a imagem
	//u graphics.DrawImage(&BitmapL, 0, 300,BitmapL.GetWidth(),BitmapL.GetHeight());
	//u graphics.DrawImage(&BitmapR, BitmapL.GetWidth()+10,300,BitmapR.GetWidth(),BitmapR.GetHeight());
	//u graphics.DrawImage(&DispMap, DispMap.GetWidth()+10, 0,DispMap.GetWidth(),DispMap.GetHeight());
  
   
	WCHAR text[40];
	LPCTSTR pszFormat = TEXT("Left %d x %d\nRight %d x %d");
	StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat, BitmapL->GetWidth(),BitmapL->GetHeight(),BitmapR->GetWidth(),BitmapR->GetHeight());
	Text(hdc,text);
   
}	


VOID OnPaint2(HDC hdc)
{
   WCHAR text[120];
   Graphics graphics(hdc);
   //carrega a imagem
	
   if(Pad==0) Pad = new Bitmap(hInstanceSave, MAKEINTRESOURCE(IDB_BITMAP1));//pad enquanto não gera o mapa de disparidade
   if(BitmapL==0) BitmapL = new Bitmap(left);
   if(BitmapR==0) BitmapR = new Bitmap(right);



   ///Bitmap BitmapL(left);//(L"Sample.jpg");//(L"a.tif");//(L"a.jpg");//
   //Bitmap BitmapR(right);//(L"a.jpg");//(L"Sample.jpg");//(L"a.tif");//
   globalWidth=BitmapL->GetWidth();
   globalHeight=BitmapL->GetHeight();
   
	Color pixelColor;

	// -----------------------------------------------------------------------------------
	
	
	IMGpixel *pointLocImgL,*pointLocImgR;
	
	voidImgL= new IMGpixel[BitmapL->GetWidth()*  BitmapL->GetHeight()];// é deletado somente no fim do programa
	voidImgR= new IMGpixel[BitmapR->GetWidth()*  BitmapR->GetHeight()];// é deletado somente no fim do programa
	
	pointLocImgL=(IMGpixel *)voidImgL;
	pointLocImgR=(IMGpixel *)voidImgR;

	
	carregaImgBuffer(pointLocImgL,BitmapL);
	carregaImgBuffer(pointLocImgR,BitmapR);


	//CorrespSearch(hdc,2048,1280,200,100,15);
	

	clock_t startLegacy, finishLegacy,startNew, finishNew;
	double  durationLegacy,durationNew;
	
	
	
	startNew=clock();
	//CorrespSearch(hdc,BitmapL->GetWidth(),BitmapL->GetHeight(),MaxdispForCompare,25,windowSizeForCompare);//Andre
	FastCorrespSearch(hdc,BitmapL->GetWidth(),BitmapL->GetHeight(),MaxdispForCompare,1,windowSizeForCompare);//tsukuba low
	
	//brincando com a rotação 21
	//e  CorrespSearch(hdc,BitmapL.GetWidth(),BitmapL.GetHeight(),16,1,7);
	finishNew=clock();
	durationNew=finishNew-startNew;

	startLegacy=clock();
	matchingCost(hdc,BitmapL->GetWidth(),BitmapL->GetHeight(),MaxdispForCompare);
	//FastCorrespSearch(hdc,BitmapL->GetWidth(),BitmapL->GetHeight(),MaxdispForCompare,25,windowSizeForCompare);//Andre

	
	finishLegacy=clock();
	durationLegacy=finishLegacy-startLegacy;


	//r LPCTSTR pszFormat1 = TEXT("New;%2.3f\n");
	//r StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat1,durationNew/CLOCKS_PER_SEC);
	//r log(text);

	LPCTSTR pszFormat1 = TEXT("Legacy;%2.3f;New;%2.3f;Dif;%2.3f;%s;%s;%d;%d\n");
	StringCbPrintfW(text, (120*sizeof(WCHAR)),pszFormat1,durationLegacy/CLOCKS_PER_SEC,durationNew/CLOCKS_PER_SEC,(durationLegacy/CLOCKS_PER_SEC)-(durationNew/CLOCKS_PER_SEC),left,right,MaxdispForCompare,windowSizeForCompare);
	log(text);
	
	//y pointLocImgL=(IMGpixel *)voidImgL;
	//y for (UINT y = 0; y < BitmapL.GetHeight(); y ++)
	//y		for (UINT x = 0; x < BitmapL.GetWidth(); x ++,pointLocImgL++ )
	//y			{
	//y			BitmapL.SetPixel(x, y, Color(pointLocImgL->a,pointLocImgL->g  ,pointLocImgL->r, pointLocImgL->b)); //troca red por green e vice-versa
	//y			}
		
	
//u	delete(voidImgL);
//u	delete(voidImgR);
	// -----------------------------------------------------------------------------------

    //o // Create a checkered pattern with red pixels.
    //ofor (UINT y = 0; y < BitmapL.GetHeight(); y += 4)
    //o	for (UINT x = 0; x < BitmapL.GetWidth(); x += 4)
    //o		{
    //o		BitmapR.GetPixel(x, y, &pixelColor);
		 
    //o		//Inverte o vermelho com o verde 
    //o		BitmapR.SetPixel(x, y, Color(pixelColor.GetA(),  pixelColor.GetG(),pixelColor.GetR(), pixelColor.GetB())); 
    //o	    }
   

   //mostra a imagem
   //ori graphics.DrawImage(&BitmapL, 0, 300,BitmapL.GetWidth(),BitmapL.GetHeight());
   //ori graphics.DrawImage(&BitmapR, BitmapL.GetWidth()+10,300,BitmapR.GetWidth(),BitmapR.GetHeight());
   graphics.DrawImage(BitmapL, 0, 300,384,288);
   graphics.DrawImage(BitmapR, 384+10,300,384,288);
   
   
   LPCTSTR pszFormat = TEXT("Left %d x %d\nRight %d x %d");
   StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat, BitmapL->GetWidth(),BitmapL->GetHeight(),BitmapR->GetWidth(),BitmapR->GetHeight());
   Text(hdc,text);
   
}	

void Text(HDC hdc,WCHAR *text)
{
   Graphics    graphics(hdc);
   SolidBrush  brush(Color(255, 0, 120, 255));
   FontFamily  fontFamily(L"Times New Roman");
   Font        font(&fontFamily, 16, FontStyleRegular, UnitPixel);
   PointF      pointF(10.0f, 590.0f);
   
   graphics.DrawString(text, -1, &font, pointF, &brush);
}


void carregaImgBufferWithSobel(IMGpixel *pointImg,Bitmap *bitmap)
	{
	//lembre-se linha x coluna
	int knlV[][3]=	{
					1, 0, -1,
					2, 0, -2,
					1, 0, -1
					};
	int knlH[][3]=	{
					-1, -2, -1,
					 0,  0,  0,
					 1,  2,  1
					};

	IMGpixel *pointLocImg;
	pointLocImg=pointImg;
	
	Color z1,z2,z3;
	Color z4,z5,z6;
	Color z7,z8,z9;

	ARGB black = 0x00000000;
	
	BYTE alfa=255;
	BYTE red,green,blue;
	int resRed,resGreen,resBlue;
	int resRedH,resGreenH,resBlueH;
	int resRedV,resGreenV,resBlueV;
	//Color result;

	int x,y;
	
	for(y=0;y<(int)bitmap->GetHeight();y++)
		for(x=0;x<(int)bitmap->GetWidth();x++,pointLocImg++)
			{
			if(((x-1)<0)||((y-1)<0)) z1.SetValue(black);
			else bitmap->GetPixel(x-1,y-1,&z1);
						
			if((y-1)<0) z2.SetValue(black);
			else bitmap->GetPixel(x,y-1,&z2);

			if(((x+1)>(int)bitmap->GetWidth())||((y-1)<0)) z3.SetValue(black);
			else bitmap->GetPixel(x+1,y-1,&z3);

			if((x-1)<0) z4.SetValue(black);
			else bitmap->GetPixel(x-1,y,&z4);

			bitmap->GetPixel(x,y,&z5);

			if((x+1)>(int)bitmap->GetWidth()) z6.SetValue(black);
			else bitmap->GetPixel(x+1,y,&z6);

			if(((x-1)<0)||((y+1)>(int)bitmap->GetHeight())) z7.SetValue(black);
			else bitmap->GetPixel(x-1,y+1,&z7);


			if((y+1)>(int)bitmap->GetHeight()) z8.SetValue(black);
			else bitmap->GetPixel(x,y+1,&z8);
			
			if(((x+1)>(int)bitmap->GetWidth())||((y+1)>(int)bitmap->GetHeight())) z9.SetValue(black);
			else bitmap->GetPixel(x+1,y+1,&z9);

			
			
			// Tratamento para o Red
			resRedH=(z1.GetR()*knlH[0][0])+(z2.GetR()*knlH[0][1])+(z3.GetR()*knlH[0][2])+
					(z4.GetR()*knlH[1][0])+(z5.GetR()*knlH[1][1])+(z6.GetR()*knlH[1][2])+
					(z7.GetR()*knlH[2][0])+(z8.GetR()*knlH[2][1])+(z9.GetR()*knlH[2][2]);
			
			
			
			if(resRedH<0)		resRedH=-resRedH;
			if(resRedH>255)		resRedH=255;

			resRedV=(z1.GetR()*knlV[0][0])+(z2.GetR()*knlV[0][1])+(z3.GetR()*knlV[0][2])+
					(z4.GetR()*knlV[1][0])+(z5.GetR()*knlV[1][1])+(z6.GetR()*knlV[1][2])+
					(z7.GetR()*knlV[2][0])+(z8.GetR()*knlV[2][1])+(z9.GetR()*knlV[2][2]);

			
			if(resRedV<0)		resRedV=-resRedV;
			if(resRedV>255)		resRedV=255;


			resRed=resRedH+resRedV;
			if(resRed>255)		resRed=255; //Não precisa da proteção para o negativo, pois sempre virá numeros positivos

			red=(BYTE) resRed;
			
			// Tratamento para o Green
			resGreenH=(z1.GetG()*knlH[0][0])+(z2.GetG()*knlH[0][1])+(z3.GetG()*knlH[0][2])+
					  (z4.GetG()*knlH[1][0])+(z5.GetG()*knlH[1][1])+(z6.GetG()*knlH[1][2])+
					  (z7.GetG()*knlH[2][0])+(z8.GetG()*knlH[2][1])+(z9.GetG()*knlH[2][2]);

			
			if(resGreenH<0)		resGreenH=-resGreenH;
			if(resGreenH>255)	resGreenH=255;

			resGreenV=(z1.GetG()*knlV[0][0])+(z2.GetG()*knlV[0][1])+(z3.GetG()*knlV[0][2])+
					  (z4.GetG()*knlV[1][0])+(z5.GetG()*knlV[1][1])+(z6.GetG()*knlV[1][2])+
					  (z7.GetG()*knlV[2][0])+(z8.GetG()*knlV[2][1])+(z9.GetG()*knlV[2][2]);

			
			if(resGreenV<0)		resGreenV=-resGreenV;
			if(resGreenV>255)	resGreenV=255;

			resGreen=resGreenH+resGreenV;
			if(resGreen>255)	resGreen=255; //Não precisa da proteção para o negativo, pois sempre virá numeros positivos

			green=(BYTE) resGreen;

			// Tratamento para o Blue
			resBlueH=(z1.GetB()*knlH[0][0])+(z2.GetB()*knlH[0][1])+(z3.GetB()*knlH[0][2])+
					 (z4.GetB()*knlH[1][0])+(z5.GetB()*knlH[1][1])+(z6.GetB()*knlH[1][2])+
					 (z7.GetB()*knlH[2][0])+(z8.GetB()*knlH[2][1])+(z9.GetB()*knlH[2][2]);

			
			if(resBlueH<0)		resBlueH=-resBlueH;
			if(resBlueH>255)	resBlueH=255;

			resBlueV=(z1.GetB()*knlV[0][0])+(z2.GetB()*knlV[0][1])+(z3.GetB()*knlV[0][2])+
					 (z4.GetB()*knlV[1][0])+(z5.GetB()*knlV[1][1])+(z6.GetB()*knlV[1][2])+
					 (z7.GetB()*knlV[2][0])+(z8.GetB()*knlV[2][1])+(z9.GetB()*knlV[2][2]);

			
			if(resBlueV<0)		resBlueV=-resBlueV;
			if(resBlueV>255)	resBlueV=255;

			resBlue=resBlueH+resBlueV;
			if(resBlue>255)	resBlue=255; //Não precisa da proteção para o negativo, pois sempre virá numeros positivos

			blue=(BYTE) resBlue;

			// Set da combinação após Sobel
			pointLocImg->a=alfa;
			pointLocImg->r=red;
			pointLocImg->g=green;
			pointLocImg->b=blue;
			}

	// Aplica na imagem Origem
	pointLocImg=pointImg;
	
	for(y=0;y<(int)bitmap->GetHeight();y++)
		for(x=0;x<(int)bitmap->GetWidth();x++,pointLocImg++)
			{
			bitmap->SetPixel(x,y,Color(255,pointLocImg->r,pointLocImg->g,pointLocImg->b));//lembre-se o alfa tem que ser 255
			//bitmap->SetPixel(x,y,Color(255,255,0,255));
			}

	
			
//tes	// Aplica textura de apoio em regiões de continuidade após sobel		
//tes	int i=0;
//tes	bool afterFirstBorder;
//tes	Color a,b,c;
//tes	BYTE treshold=50;
//tes	IMGpixel *pointLocImg1,*pointLocImg2;
		
//tes	pointLocImg=pointImg;
	
//tes	int width=bitmap->GetWidth();
//tes	//GmaxDisp tem a finalidade de ignorar a borda esquerda pois podem haver pontos sem correspondência que não podem ter a texturização. Outro fator de proteção é que a texturização só pode acontecer depois da primeira borda após GmaxDisp.
//tes	for(y=0;y<(int)bitmap->GetHeight();y++)
//tes		{
//tes		afterFirstBorder=false;
//tes		pointLocImg=pointImg+(y*width)+GmaxDisp;
//tes		pointLocImg1=pointLocImg+1;
//tes		pointLocImg2=pointLocImg+2;

//tes		for(x=GmaxDisp;x<(width-2);x++,pointLocImg++,pointLocImg1++,pointLocImg2++)//x<((int)bitmap->GetWidth()-1) são proteções, pois dentro do for eu acesso pointLocImg+1 +2
//tes			{
			
//tes			//bitmap->GetPixel(x-1,y,&a);
//tes			//bitmap->GetPixel(x,y,&b);
//tes			//bitmap->GetPixel(x+1,y,&c);

//tes			if(	((pointLocImg->r<treshold) &&(pointLocImg->g<treshold) &&(pointLocImg->b<treshold))&&
//tes				((pointLocImg1->r<treshold)&&(pointLocImg1->g<treshold)&&(pointLocImg1->b<treshold))&&
//tes				((pointLocImg2->r<treshold)&&(pointLocImg2->g<treshold)&&(pointLocImg2->b<treshold))&&(afterFirstBorder==true)	) 
//tes				{
//tes				setPixelCount(x+1,y,i,bitmap);
//tes				//bitmap->SetPixel(x+1,y,Color(255,255,0,0));//x+1 é para pintar o pixel do meio dos três em questão,lembre-se o alfa tem que ser 255
//tes				i++;
//tes				}
//tes			else
//tes				{ 
//tes				i=0;
//tes				afterFirstBorder=true;
//tes				}
//tes			}//for(x=0;
//tes		}//for(y=0;
//tes	carregaImgBuffer(pointImg,bitmap);
	}

void setPixelCount(int x, int y ,int i,Bitmap *bitmap)
	{
	int evenOdd,step10,step50;
	BYTE red,green,blue;

	evenOdd=i%2;//0 1
	step10=i%10;//0 até 9
	step50=i%50;//0 até 49


	red=step10*25;//de 0 até 225 para 225 pixels ocorrerá 25 ciclos
	green=120+(120*evenOdd);//120 ou 240 para cada 2 pixels ocorrerá um ciclo
	blue=255-(step50*5);//de 255 até 10 para 245 pixels ocorrerá 5 ciclos
	
		
	bitmap->SetPixel(x,y,Color(255,red,green,blue));//lembre-se o alfa tem que ser 255
	}
//Estudo SSD x SAD
//Resolução 648x486
//VASOL.BMP VASOR.BMP 40 3 21 I 64 64


//SSD vs8 2,172 2,188
//SSD VS5 2,703 2,750 2,750 2,703
//SSD VS3 2,797 2,781

//SAD VS5 2,953 3,000 primeira versão (if)
//SAD VS5 2,719 2,813 2,765 2,843  2,703 2,750 segunda versão (labs())

//de volta ao ssd
//SSD VS5 2,703 2,672 2,688 2,734 2,719 2,766


//veja para mudar  a Aggregation Strip para float, pois quando a janela(soma do conteúdo) for muito grande maior que 148x148 no pior caso (região preta versus branca) pode extrapolar a capacidade do unsigened long... já com float tudo bem... , outra cois mude também o código para a o Tradicional Approach (usar float).Em 17/09 : acho que já está com float
//Novo algoritmo: Stripping Technique - Otimizado para aumentar a performance
void FastCorrespSearch(HDC hdc,unsigned int width,unsigned int height,unsigned int maxDisp,unsigned int maxLineVar,int windowSize)
	{
	maxDisparity=maxDisp;//variavel global usada em D3D9 para construção da interface 3D
	unsigned long n;
	int yVar;//long pois no for que será usado será colocado na variáve searchRegionY que é long
	//WCHAR text[250];
	
	//proteções
	if(windowSize%2==0)	{log(L"windowSize invalido, número par.\n");return;}
	if(windowSize<3)	{log(L"windowSize invalido, menor que 3.\n");return;}
	if(maxDisp<1)		{log(L"maxDisp invalido, menor que 1.\n");return;}
	if(maxLineVar<1)	{log(L"maxLineVar invalido, menor que 1.\n");return;}
	if(maxLineVar%2==0)	{log(L"maxLineVar invalido, número par.\n");return;}//funciona como a window, porém aceita tamanho 1, que na prática significa um busca 1D

	n=(windowSize-1)/2;//para N=3 n=1 para N=5 n=2 N é o tamanho do kernel e n é o incremento/decremento a ser feito em x e y
	yVar=(maxLineVar-1)/2;//para maxLineVar=7 yVar=3


	//Alocação de memória para a Faixa de Aggregação (Aggregation Strip)
//Mude para Float, pois em janela muito grande o resultado pode não caber
	unsigned long stripSize,numberOfStrips,aggregationStripSize;
	void *voidAggregationStrip;
	unsigned long *pointLocAggregationStrip;

	if(maxDisp<=0) maxDisp=1;//maxDisp=0 é invalido
	if(maxLineVar<=0) maxLineVar=1;//maxLineVar=0 é invalido

	stripSize=width*(windowSize+1);//a altura da strip é o windowSize, o +1 se refere a linha de para armazenamento temporário no processo de Fast Aggregation
	numberOfStrips=maxDisp*maxLineVar;
	aggregationStripSize=stripSize*numberOfStrips;

	voidAggregationStrip= new unsigned long[aggregationStripSize];
	pointLocAggregationStrip=(unsigned long *)voidAggregationStrip;
	//*****************************************************************

	//Alocação de memória para as linhas de seleção de disparidade
	unsigned long dispSelectLineSize;
	void *voidDispSelectLine;
	unsigned long *pointLocDispSelectLine;

	dispSelectLineSize=width*numberOfStrips;//deve ter uma linha para cada faixa

	voidDispSelectLine= new unsigned long[dispSelectLineSize];
	pointLocDispSelectLine=(unsigned long *)voidDispSelectLine;
	//*****************************************************************
	
	//Alocação de memória para o Mapa de Disparidades
	
	//void *voidDispMap; mudou para variavel global que será usada em D3D9.cpp para montar a malha 3D
	unsigned long *pointLocDispMap;

	voidDispMap= new unsigned long[width*height];
	pointLocDispMap=(unsigned long *)voidDispMap;
	//*****************************************************************
	
	WCHAR text[10];
	LPCTSTR pszFormat = TEXT("%0.0f %%");
  
	
	Bitmap DispMap(GleftFile);//i usei o left só para gerar um bitmap de mesmas proporções, poderia ser o right


	unsigned int x,yFull,searchRegionX,searchRegionY;
	int y;//tem que aceitar negativo pois vai ser um fator de variação de uma linha, isto é, irá tanto para cima (-) quanto para baixo (+)
	unsigned long stepStrip,stepLineStrip,stepImageL,stepImageR,stepImageUnderStripIn,stepImageUnderStripOut,stepLine,increment;
	unsigned long yFullProdWidth;
	long jumpLineIn,jumpLineOut;

	startST=clock();//Contabiliza somente a execução do código de procura de correspondências
	

	for(yFull=0;yFull<height;yFull++)//yFull sempre esta no centro da Strip
	{
	yFullProdWidth=yFull*width;
	StringCbPrintfW(text, (10*sizeof(WCHAR)),pszFormat,((float)yFull/(height))*100 );
	SetDlgItemText(hDialog, IDC_STATIC_PROGRESS,text);
	
	long tempL,tempR,tempDiff,Sdiff;
	IMGpixel *pointLocImgL,*pointLocImgR;
	
	if((yFull>=n)&&(yFull<height-n))
	{
	if(yFull==n)//primeira linha valida
		{
		//Matching Cost - Strips da primeira linha válida
		//Para um pixels na imagem da esquerda eu procuro um conjunto de pixels(disparidade) na imagem direita
		pointLocImgR=(IMGpixel *)voidImgR;//aponta ponteiro local para a imagem da Direita, dentro do for este ponteiro não é incrementado, só por isso ele é definido fora do for
		for(searchRegionY=0;searchRegionY<maxLineVar;searchRegionY++)
			for(stepStrip=0,searchRegionX=0;searchRegionX<maxDisp;searchRegionX++)
				{
				stepStrip=(searchRegionY*maxDisp*stripSize)+searchRegionX*stripSize;//(searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
				pointLocImgL=(IMGpixel *)voidImgL;
		
				for(y=0;y<windowSize;y++)//Carga da primeira Aggregation Strip, a altura da strip é igual a windowSize
					{
					stepLineStrip=y*width;
					//para y>=yVar se maxLineVar for 5 o yVar será 2 logo o searchRegionY vai oscilar de -2 até 2(inclusive), por consequência o y será somado a -2 o que resulta que o menor y permitido seja 2, logo y tem que ser >= a yVar 
					//para y<=(height-(1+yVar)) se height é 768 que é de 0 até 767 só pode ir até 765 que é height-(1+var)
					if(	(y>=yVar)&&(y<=((int)height-(1+yVar)))	)// na primeira linha válida nem precisava a segunda parte y<=(height-(1+yVar)) porém optei por manter para evitar futuros erros em manutenções do código
						{
						//stepImageR=(y+searchRegionY)*width;//Calcula uma única vez para cada y. o searchRegionY irá fazer o y oscilar para cima e para baixo do valor que referência da imagem da esqueda, pois altera stepImage que é referência para increment que é usado para deslocar a imagem da direita.
						stepImageR=(y+searchRegionY-yVar)*width;//tem que ter um if antes protejendo, pois quando y=0 e searchRegionY=0 pode haver acesso a uma região de memória não alocada

						for(x=0;x<width;x++,pointLocImgL++)//,pointLocImgR++)//,pointLocSquaredDiff++ )
							{
							if(x>=(maxDisp-1))//se o maxDisp é 10 a searchRegionX vai de 0 a 9, logo o minimo que eu posso aceitar de searchRegionX é 9 que é maxDisp menos um, para que a subtração de x não suba uma linha (para y=0 existe o risco de acessar memória não alocada)
								{
								increment=stepImageR+(x-searchRegionX);//Calcula uma única vez para cada x 
							
								tempL=pointLocImgL->r;//eu uso ponteiro puro aqui, por ser mais rápido (não tem que fazer conta)
								tempR=pointLocImgR[increment].r;
								tempDiff=tempL-tempR;
								// Sdiff = labs( tempDiff ); //SAD 
								Sdiff=tempDiff*tempDiff; //SSD
					
			
								tempL=pointLocImgL->g;
								tempR=pointLocImgR[increment].g;
								tempDiff=tempL-tempR;
								//Sdiff = labs( tempDiff )+ Sdiff; //SAD
								Sdiff=(tempDiff*tempDiff)+Sdiff; //SSD
						
			
								tempL=pointLocImgL->b;
								tempR=pointLocImgR[increment].b;
								tempDiff=tempL-tempR;
								//Sdiff = labs( tempDiff )+ Sdiff; //SAD
								Sdiff=(tempDiff*tempDiff)+Sdiff; //SSD
					

								pointLocAggregationStrip[stepStrip+stepLineStrip+x]=Sdiff;
								}//if(x>=(maxDisp-1))
							else//if(x>=(maxDisp-1))
								pointLocAggregationStrip[stepStrip+stepLineStrip+x]=195075;//(255*255)+(255*255)+(255*255)=195075 tem que ser alto para evitar falsa correspondência, lembre-se que na fase de Evidence Aggregation  somasse os pontos da janela e que na fase de Disparity Seletion é escolhido o menor valor (WTA), se fosse um valor baixo como 0 pode fazer parecer um winner 
							}//for(x=0
						}//if(y>=yVar)
					else//if(y>=yVar)
						{
						for(x=0;x<width;x++)//havia um bug aqui, eu havia esquecido de colocar o for, e por isso era preenchido somente um pixel
							pointLocAggregationStrip[stepStrip+stepLineStrip+x]=195075;//(255*255)+(255*255)+(255*255)=195075 tem que ser alto para evitar falsa correspondência, lembre-se que na fase de Evidence Aggregation  somasse os pontos da janela e que na fase de Disparity Seletion é escolhido o menor valor (WTA), se fosse um valor baixo como 0 pode fazer parecer um winner 
						}
					}//for(y=0
				}//for(searchRegionX=0
		

	//Evidence Aggregation - Strips da primeira linha válida
	for(searchRegionY=0;searchRegionY<maxLineVar;searchRegionY++)//aqui o for é diferente do for usado para esta mesma variável no Matching Cost, pois lá ela é usada principalmente para deslocar a linha de procura da imagem da direita o que exige que ela oscile de -yVar até yVar, como para stepStrip searchRegionY precisa oscilar de 0 até maxLineVar-1 é usada uma operação searchRegionY+yVar para conseguir esta oscilação. Como aqui o objetivo é varrer as Aggregation Strips e as Disparity Selection Lines faz mais sentido que o for oscile de 0 até maxLineVar-1
		for(stepLine=0,searchRegionX=0;searchRegionX<maxDisp;searchRegionX++)
		{
		stepLine=(searchRegionY*maxDisp*width)+(searchRegionX*width);//Calcula uma única vez para cada line, variável para salto entre as Disparity Selection Lines, (searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
		stepStrip=(searchRegionY*maxDisp*stripSize)+(searchRegionX*stripSize);//Calcula uma única vez para cada strip, variável de salto entre da Aggregation Strip, (searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
		if((yFull>=n)&&(yFull<height-n))
//acho que está incoerente, se a borda não é tratada para a disparidade (lado esquerdo do mapa de disparidades) porque aqui não tem uma proteção igual (	(y>=yVar)&&(y<=((int)height-(1+yVar))), ????
				{
				for(x=0;x<width;x++)//,pointLocImgR++)//,pointLocSquaredDiff++ )
					{
					//borda de proteção:para width= 500 e n=1 o máximo x somente pode ser 499, isto é, width-1-n
					//borda de proteção:para n=1 o mínimo y somente pode ser 1
					//borda de proteção:range=10 representa um deslocamento de disparidade(x-layer) na imagem direita de 10 pixels, logo o minimo x aceitavel é 9 (range-1) 
					//considerando que o primeiro x válido(em relação ao range) fica sendo 9 logo o pixel x=9 seria o primeiro válido, agora considerando um n=2 o menor x aceitavel passa a ser 11
					//então para range=10 e n=2 o primeiro x válido é 11 temos (range-1)+n
					if((x>=(maxDisp-1)+n)&&(x<=(width-1)-n))//borda não tratada
						{
						if(x==((maxDisp-1)+n))
							{//usa-se n no lugar de y pois n é a metade da altura da strip ex. para windowSize=3 n=(3-1)/2=2/2=1 logo temo y=0,y=1 e y=2 sendo que 1 é o do meio, que é igual a n
							sumFirstWindow(width,x,n,n,&pointLocAggregationStrip[stepStrip],pointLocDispSelectLine,stepLine);
							}
						else
							{
							sumNextWindow(width,x,n,n,&pointLocAggregationStrip[stepStrip],pointLocDispSelectLine,stepLine);
							}
						}//if((x>=(range-1)+n)
					else//dentro do X precisa ter um tratamento para zerar a parte não tratada, o que não é necessário para o y, pois para y não tratado, não tem strip e nem selectionLine.
						{
						//i BitmapL.SetPixel(x, y, Color(255,0,125,255)); 
						pointLocDispSelectLine[stepLine+x]=0;
						}
					}//for(x=0
				}//if((yFull>=n)&&(yFull<height-n))
		}//for(searchRegionX=0

	jumpLineOut=0;//Na agregação esta linha sai
	jumpLineIn=windowSize;//Na agregação esta linha entra
	}//if(yFull==n)
	
	
	if((n<yFull)&&(yFull<height-n)) //Demais linhas até a útima válida, exceto a primeira válida
		{
		//Matching Cost - Strips das demais linhas até a útima válida, exceto a primeira válida
		pointLocImgR=(IMGpixel *)voidImgR;//aponta ponteiro local para a imagem da Direita, dentro do for este ponteiro não é incrementado, só por isso ele é definido fora do for
		stepImageUnderStripIn=jumpLineIn*width;//Indica a linha que será descartada da rodada anterior recebendo o novo dado, que será adicionado na agregação (na primeira vez é a linha de posição igual WindowSize que é a ultima linha, dai roda para a primeira (0) e assim por diante, sempre depois da jumpLineOut
		stepImageUnderStripOut=jumpLineOut*width;//Indica a linha que será removida na agregação, na proxima rodada esta será a In, isto é, um novo dado irá substitui-la (na primeira vez é a linha de posição igual 0 que é a primeira linha, dai roda para a segunda (1) e assim por diante, sempre a frete da jumpLineIn

		
		for(searchRegionY=0;searchRegionY<maxLineVar;searchRegionY++)
			for(stepStrip=0,searchRegionX=0;searchRegionX<maxDisp;searchRegionX++)
				{
				stepStrip=(searchRegionY*maxDisp*stripSize)+(searchRegionX*stripSize);//(searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
				y=yFull+n;//y será a próxima linha abaixo, pois a demais já estão dentro da Aggregation Strip do loop anterior
				
				if(	(y>=yVar)&&(y<=((int)height-(1+yVar)))	)// na primeira linha válida(if(yFull==n)) nem precisava a segunda parte y<=(height-(1+yVar)) porém optei por manter para evitar futuros erros em manutenções do código
					{
					//stepImageR=(y+searchRegionY)*width;//Calcula uma única vez para cada y. o searchRegionY irá fazer o y oscilar para cima e para baixo do valor que referência da imagem da esqueda, pois altera stepImage que é referência para increment que é usado para deslocar a imagem da direita.
					stepImageR=(y+searchRegionY-yVar)*width;
					stepImageL=y*width;//Calcula uma única vez para cada y 
					
					pointLocImgL=(IMGpixel *)voidImgL;
					pointLocImgL+=stepImageL;//avança para a linha certa
					for(x=0;x<width;x++,pointLocImgL++)//,pointLocImgR++)//,pointLocSquaredDiff++ )
						{
						if(x>=(maxDisp-1))//se o maxDisp é 10 a searchRegionX vai de 0 a 9, logo o minimo que eu posso aceitar de searchRegionX é 9 que é maxDisp menos um, para que a subtração de x não suba uma linha (para y=0 existe o risco de acessar memória não alocada)
							{
							increment=stepImageR+(x-searchRegionX);//Calcula uma única vez para cada x 

							tempL=pointLocImgL->r;//eu uso ponteiro puro aqui, por ser mais rápido (não tem que fazer conta)
							tempR=pointLocImgR[increment].r;
							tempDiff=tempL-tempR;
							//Sdiff = labs( tempDiff );//SAD
							Sdiff=tempDiff*tempDiff; //SSD
							
									
							tempL=pointLocImgL->g;
							tempR=pointLocImgR[increment].g;
							tempDiff=tempL-tempR;
							//Sdiff = labs( tempDiff )+ Sdiff; //SAD
							Sdiff=(tempDiff*tempDiff)+Sdiff;//SSD
														
										
							tempL=pointLocImgL->b;
							tempR=pointLocImgR[increment].b;
							tempDiff=tempL-tempR;
							//Sdiff = labs( tempDiff )+ Sdiff;//SAD
							Sdiff=(tempDiff*tempDiff)+Sdiff;//SSD
							
							pointLocAggregationStrip[stepStrip+stepImageUnderStripIn+x]=Sdiff;
							}//if(x>=(maxDisp-1))
						else
							pointLocAggregationStrip[stepStrip+stepImageUnderStripIn+x]=195075;//(255*255)+(255*255)+(255*255)=195075 tem que ser alto para evitar falsa correspondência, lembre-se que na fase de Evidence Aggregation  somasse os pontos da janela e que na fase de Disparity Seletion é escolhido o menor valor (WTA), se fosse um valor baixo como 0 pode fazer parecer um winner 
						}//for(x=0
					}//if(y>=yVar)
				else//if(y>=yVar)
					{
					for(x=0;x<width;x++)//havia um bug aqui, eu havia esquecido de colocar o for, e por isso era preenchido somente um pixel
						pointLocAggregationStrip[stepStrip+stepImageUnderStripIn+x]=195075;//(255*255)+(255*255)+(255*255)=195075 tem que ser alto para evitar falsa correspondência, lembre-se que na fase de Evidence Aggregation  somasse os pontos da janela e que na fase de Disparity Seletion é escolhido o menor valor (WTA), se fosse um valor baixo como 0 pode fazer parecer um winner 
					}
				}//for(searchRegionX=0
	
	
		//Evidence Aggregation - Strips das demais linhas até a útima válida, exceto a primeira válida
		for(searchRegionY=0;searchRegionY<maxLineVar;searchRegionY++)//aqui o for é diferente do for usado para esta mesma variável no Matching Cost, pois lá ela é usada principalmente para deslocar a linha de procura da imagem da direita o que exige que ela oscile de -yVar até yVar, como para stepStrip searchRegionY precisa oscilar de 0 até maxLineVar-1 é usada uma operação searchRegionY+yVar para conseguir esta oscilação. Como aqui o objetivo é varrer as Aggregation Strips e as Disparity Selection Lines faz mais sentido que o for oscile de 0 até maxLineVar-1
			for(stepLine=0,searchRegionX=0;searchRegionX<maxDisp;searchRegionX++)
			{
			stepLine=(searchRegionY*maxDisp*width)+(searchRegionX*width);//Calcula uma única vez para cada line, variável para salto entre as Disparity Selection Lines, (searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
			stepStrip=(searchRegionY*maxDisp*stripSize)+(searchRegionX*stripSize);//Calcula uma única vez para cada strip, variável de salto entre da Aggregation Strip, (searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
			if((yFull>=n)&&(yFull<height-n))
//acho que está incoerente, se a borda não é tratada para a disparidade (lado esquerdo do mapa de disparidades) porque aqui não tem uma proteção igual (	(y>=yVar)&&(y<=((int)height-(1+yVar))), ????
				{
				for(x=0;x<width;x++)//,pointLocImgR++)//,pointLocSquaredDiff++ )
					{
					//borda de proteção:para width= 500 e n=1 o máximo x somente pode ser 499, isto é, width-1-n
					//borda de proteção:para n=1 o mínimo y somente pode ser 1
					//borda de proteção:range=10 representa um deslocamento de disparidade(x-layer) na imagem direita de 10 pixels, logo o minimo x aceitavel é 9 (range-1) 
					//considerando que o primeiro x válido(em relação ao range) fica sendo 9 logo o pixel x=9 seria o primeiro válido, agora considerando um n=2 o menor x aceitavel passa a ser 11
					//então para range=10 e n=2 o primeiro x válido é 11 temos (range-1)+n
					if((x>=(maxDisp-1)+n)&&(x<=(width-1)-n))//borda não tratada
						{
//testar com o código aqui para melhorar o desempenho sem chamar função... o mesmo vale para o tradicional
//(no matching cost use SAD)... 
						sumPair(x,stepImageUnderStripOut,stepImageUnderStripIn,n,&pointLocAggregationStrip[stepStrip],pointLocDispSelectLine,stepLine);
						}//if((x>=(maxDisp-1)+n)&&(x<=(width-1)-n))//borda não tratada
					else//dentro do X precisa ter um tratamento para zerar a parte não tratada, o que não é necessário para o y, pois para y não tratado, não tem strip e nem selectionLine.
						{
						//i BitmapL.SetPixel(x, y, Color(255,0,125,255)); 
						pointLocDispSelectLine[stepLine+x]=0;
						}
					}//for(x=0
				}//if((yFull>=n)&&(yFull<height-n))
			}//for(searchRegionX=0

				
		jumpLineOut+=1;
		jumpLineIn+=1;
		if(jumpLineOut==(windowSize+1)) jumpLineOut=0;
		if(jumpLineIn==(windowSize+1))	jumpLineIn=0;
		}//if((n<yFull)&&(yFull<height-n))


	}//if((yFull>=n)&&(yFull<height-n))
	
	

	//Disparity Selection WTA (Winneer Take-All)
	unsigned long bestResult;
	unsigned long disp;
	unsigned int part,color;
	part=255/maxDisp;

	for(x=0;x<width;x++)//,pointLocImgR++)//,pointLocSquaredDiff++ )
		{
		//Pressupoe que a primeira linha (primeira strp) já é p melhor resultado
		disp=0;
		bestResult=pointLocDispSelectLine[x];//pressupoe que é a primeira linha stepLine=0
		for(searchRegionY=0;searchRegionY<maxLineVar;searchRegionY++)
			for(stepLine=0,searchRegionX=0;searchRegionX<maxDisp;searchRegionX++)
				{
				stepLine=(searchRegionY*maxDisp*width)+searchRegionX*width;//Calcula uma única vez para cada line, variáve para salto entre as Disparity Selection Lines, (searchRegionY*maxDisp*stripSize) equivale a um pulo de linha de strips 
				//stepLine=searchRegionX*width;//Calcula uma única vez para cada linha
				if((yFull>=n)&&(yFull<height-n))
					if(bestResult>pointLocDispSelectLine[stepLine+x])
						{
						disp=searchRegionX;
						bestResult=pointLocDispSelectLine[stepLine+x];
						}
				//else não é necessário, pois disp e bestResult já tem um valor pressuposto.
				}//for(searchRegionX
		color=part*disp;
		pointLocDispMap[(yFullProdWidth)+x]=color;//disp;
		
		//é usado color e não o ponteiro pointLocDispMap[] pois para acessar os seus conteúdos é necessária multiplicação, logo é mais eficiente usar uma variável já como o valor da cor
	//e	DispMap.SetPixel(x, yFull, Color(255,color,color,color)); //lembre-se o alfa TEM que ser 255
		}//for(x=0
	}//for(yFull=0
	finishST=clock(); //Contabiliza somente a execução do código, retirando a etapa para salvar o arquivo

	durationST=finishST-startST;

	unsigned int xs,ys;
	unsigned long ysProdWidth;
	unsigned int color;
	pointLocDispMap=(unsigned long *)voidDispMap;

	for (ys=0; ys<height;ys++)
		{
		ysProdWidth=ys*width;
		for (xs=0; xs<width;xs++)
			{
			color=pointLocDispMap[(ysProdWidth)+xs];
			DispMap.SetPixel(xs, ys, Color(255,color,color,color)); //lembre-se o alfa TEM que ser 255
			}
		}

	CLSID  clsid;
    //GetEncoderClsid(L"image/jpeg", &clsid);//veja os encoders disponiveis no Windows 2000
	GetEncoderClsid(L"image/bmp", &clsid);
	Graphics graphics(hdc);//i
	
	//No futuro fazer um tratamento para o usuário selecionar o formato do arquivo de saída do diparity map
	DispMap.Save(GdisparityMap, &clsid, NULL);
	
	//graphics.DrawImage(&DispMap, DispMap.GetWidth()+10, 0,DispMap.GetWidth(),DispMap.GetHeight());

	dispGenerated=true;

	delete(voidAggregationStrip);
	delete(voidDispSelectLine);
	}


//cuidados: N tem que ser impar e maior que 3
//			x tem que ser maior que 0+n
//			y tem que ser maior que 0+n
void sumPair(unsigned long x,unsigned long stepImageUnderStripOut,unsigned long stepImageUnderStripIn,unsigned long n,unsigned long *AggregationStrip,unsigned long *DispSelectLine,unsigned long stepLine)
	{
	//h WCHAR text[40];
	//h LPCTSTR pszFormat = TEXT("%d;%d;%0.0f;%0.0f\n");
	unsigned long xi;


	unsigned long tempSum;

	tempSum=DispSelectLine[stepLine+x];//guarda o valor anterior
	
	for(xi=x-n;xi<=x+n;xi++)
			{
			tempSum=tempSum-AggregationStrip[stepImageUnderStripOut+xi];//retira custos da linha descartada da strip. Não tem perigo de dar negativo, (que seria um erro dificil de detectar) porque o valor que está sendo subtraido, havia sido adicionado na rodada anterior. 
			tempSum=tempSum+AggregationStrip[stepImageUnderStripIn+xi];//adiciona custos da linha adicionada na strip
			}//for(xi=x-n;xi<=x+n;xi++)

	DispSelectLine[stepLine+x]=tempSum;//salva valor atual
	//if(tempSum<0)
	//	{
	//	WCHAR text[40];
	//	LPCTSTR pszFormat = TEXT("First;%0.0f\n");
	//	StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,tempSum);
	//	log(text);
	//	}
	}

