//Para mudar o tipo de vertice:
//Em OnCreateDevice ajuste: D3DVERTEXELEMENT9 decl[]
//Em OnResetDevice  ajuste:	CreateVertexBuffer
//Ajustar struct dos vertice D3DNMVERTEX
//Carregar os vertices com as informações adequadas
//Em OnFrameRender ajuste SetStreamSource com  sizeof(D3DNMVERTEX)


#include "D3D9.h"


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int InitD3D9(HINSTANCE hInstance)
{
	posicaoInicial();
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Set the callback functions. These functions allow DXUT to notify
    // the application about device changes, user input, and windows messages.  The 
    // callbacks are optional so you need only set callbacks for events you're interested 
    // in. However, if you don't handle the device reset/lost callbacks then the sample 
    // framework won't be able to reset your device since the application must first 
    // release all device resources before resetting.  Likewise, if you don't handle the 
    // device created/destroyed callbacks then DXUT won't be able to 
    // recreate your device resources.
//i    DXUTSetCallbackDeviceCreated( OnCreateDevice );
//i    DXUTSetCallbackDeviceReset( OnResetDevice );
//i    DXUTSetCallbackDeviceLost( OnLostDevice );
//i    DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
//i    DXUTSetCallbackMsgProc( MsgProc );
//i    DXUTSetCallbackKeyboard( KeyboardProc );
//i    DXUTSetCallbackMouse( MouseProc );
//i    DXUTSetCallbackFrameRender( OnFrameRender );
//i    DXUTSetCallbackFrameMove( OnFrameMove );


	DXUTSetCallbackD3D9DeviceAcceptable( IsDeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnCreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnResetDevice );
    DXUTSetCallbackD3D9DeviceLost( OnLostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( KeyboardProc );
	DXUTSetCallbackMouse( MouseProc );
	DXUTSetCallbackD3D9FrameRender( OnFrameRender );
    //DXUTSetCallbackFrameRender( OnFrameRender );
    DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings( true, true );

    InitApp();
	
    // Initialize DXUT and create the desired Win32 window and Direct3D 
    // device for the application. Calling each of these functions is optional, but they
    // allow you to set several options which control the behavior of the framework.
    //i    DXUTInit( true, true, true ); // Parse the command line, handle the default hotkeys, and show msgboxes
	DXUTInit( true, true ); // Parse the command line and show msgboxes
   // DXUTCreateWindow( L"Stereo Vison", hInstance, NULL, NULL, 0,0);
	// ****************************************
	//DXUTCreateWindow( L"Stereo Vision", NULL, NULL, NULL, 0,0);
	DXUTCreateWindow( L"Stereo Vision");
    HWND d3dHWND = DXUTGetHWNDDeviceWindowed();
	d3dHWNDsave=d3dHWND;

    //PREPARE STYLE
    LONG style = GetWindowLong( d3dHWND, GWL_STYLE );
    style &= ~(WS_OVERLAPPEDWINDOW);
    //style |= WS_CHILD|WS_TABSTOP;// com estes prarametros o Zoom ligado ao Scrow do mouse não funciona
    SetWindowLong( d3dHWND, GWL_STYLE, style );

    //FIX EXSTYLE
    LONG styleEx = GetWindowLong( d3dHWND, GWL_EXSTYLE );
    styleEx |= WS_EX_ACCEPTFILES;
    SetWindowLong( d3dHWND, GWL_EXSTYLE, styleEx );


	//SetParent(d3dHWND, hWindow);
	//DxDialog::OnSize();
    SetFocus( d3dHWND );


	// ****************************************

	
	//i	DXUTCreateDevice( D3DADAPTER_DEFAULT, true, width, height, IsDeviceAcceptable, ModifyDeviceSettings );
	DXUTCreateDevice( true, width, height );
	//DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 640, 480, IsDeviceAcceptable, ModifyDeviceSettings );
	  
    // Pass control to DXUT for handling the message pump and 
    // dispatching render calls. DXUT will call your FrameMove 
    // and FrameRender callback when there is idle time between handling window messages.
	
    DXUTMainLoop();
	
	

    // Perform any application-level cleanup here. Direct3D device resources are released within the
    // appropriate callback functions and therefore don't require any cleanup code here.

    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    // Initialize dialogs
    g_SettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_SampleUI.Init( &g_DialogResourceManager );

    g_HUD.SetCallback( OnGUIEvent ); int iY = 10; 
	//Botões default
    //h g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
    //h g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
    //h g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );

    //g_SampleUI.SetCallback( OnGUIEvent ); iY = 10; 

/*
    TODO: add UI controls as needed
    g_SampleUI.AddComboBox( 19, 35, iY += 24, 125, 22 );
    g_SampleUI.GetComboBox( 19 )->AddItem( L"Text1", NULL );
    g_SampleUI.GetComboBox( 19 )->AddItem( L"Text2", NULL );
    g_SampleUI.GetComboBox( 19 )->AddItem( L"Text3", NULL );
    g_SampleUI.GetComboBox( 19 )->AddItem( L"Text4", NULL );
    g_SampleUI.AddCheckBox( 21, L"Checkbox1", 35, iY += 24, 125, 22 );
    g_SampleUI.AddCheckBox( 11, L"Checkbox2", 35, iY += 24, 125, 22 );
    g_SampleUI.AddRadioButton( 12, 1, L"Radio1G1", 35, iY += 24, 125, 22 );
    g_SampleUI.AddRadioButton( 13, 1, L"Radio2G1", 35, iY += 24, 125, 22 );
    g_SampleUI.AddRadioButton( 14, 1, L"Radio3G1", 35, iY += 24, 125, 22 );
    g_SampleUI.GetRadioButton( 14 )->SetChecked( true ); 
    g_SampleUI.AddButton( 17, L"Button1", 35, iY += 24, 125, 22 );
    g_SampleUI.AddButton( 18, L"Button2", 35, iY += 24, 125, 22 );
    g_SampleUI.AddRadioButton( 15, 2, L"Radio1G2", 35, iY += 24, 125, 22 );
    g_SampleUI.AddRadioButton( 16, 2, L"Radio2G3", 35, iY += 24, 125, 22 );
    g_SampleUI.GetRadioButton( 16 )->SetChecked( true );
    g_SampleUI.AddSlider( 20, 50, iY += 24, 100, 22 );
    g_SampleUI.GetSlider( 20 )->SetRange( 0, 100 );
    g_SampleUI.GetSlider( 20 )->SetValue( 50 );
    g_SampleUI.AddEditBox( 20, L"Test", 35, iY += 24, 125, 32 );
*/
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
/*i bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3DObject(); 
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}*/

//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    // Need to support ps 2.0
    if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
        return false;

    // Need to support A8R8G8B8 textures
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, 0,
                                         D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8 ) ) )
        return false;

    return true;
}



//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// DXUT will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
/*i bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
	bool g_DebugPS=false,g_DebugVS=false;

	#ifdef DEBUG_VS
	g_DebugVS=true;
	#endif

	#ifdef DEBUG_PS
	g_DebugPS=true;
	#endif

    HWND hChild = DXUTGetHWNDDeviceWindowed();
    if(pDeviceSettings->pp.Windowed != FALSE)
	{
        SetParent(hChild, hDialog);//u hWindow);//Vincula uma janela à outra
		OnSize();
	}
    else
        SetParent(hChild, NULL);

    if (g_DebugPS)
    {
        pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
    }
	else
	{
        pDeviceSettings->DeviceType = D3DDEVTYPE_HAL;
	}

    if (g_DebugVS)
    {
        // Debugging vertex shaders requires either REF or software vertex processing 
        // and debugging pixel shaders requires REF.  
        if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
        {
           //e pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
            pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
           //e pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }
    }
	else
	{
        if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
        {
            pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
        }

		// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
		// then switch to SWVP.
		if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
			pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
		{
			//e pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
			//e pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
		else
		{
            //e pDeviceSettings->BehaviorFlags &= ~D3DCREATE_SOFTWARE_VERTEXPROCESSING;
            //e pDeviceSettings->BehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
	}

    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning();
    }

    pDeviceSettings->pp.SwapEffect = D3DSWAPEFFECT_COPY;//required for single manual refresh
    pDeviceSettings->pp.BackBufferCount = 1; //required for D3DSWAPEFFECT_COPY


    return true;
}*/


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the
// application to modify the device settings. The supplied pDeviceSettings parameter
// contains the settings that the framework has selected for the new device, and the
// application can make any desired changes directly to this structure.  Note however that
// DXUT will not correct invalid device settings so care must be taken
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	HWND hChild = DXUTGetHWNDDeviceWindowed();
	if(pDeviceSettings->d3d9.pp.Windowed != FALSE)
	{
        SetParent(hChild, hDialog);//u hWindow);//Vincula uma janela à outra
		OnSize();
	}
    else
        SetParent(hChild, NULL);

    assert( DXUT_D3D9_DEVICE == pDeviceSettings->ver );

    HRESULT hr;
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    D3DCAPS9 caps;

    V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal,
                            pDeviceSettings->d3d9.DeviceType,
                            &caps ) );

    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW
    // then switch to SWVP.
    if( ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
         caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
    {
        pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

     // If the hardware cannot do vertex blending, use software vertex processing.
    if( caps.MaxVertexBlendMatrices < 2 )
        pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

    // If using hardware vertex processing, change to mixed vertex processing
    // so there is a fallback.
    if( pDeviceSettings->d3d9.BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING )
        pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;

    // Debugging vertex shaders requires either REF or software vertex processing
    // and debugging pixel shaders requires REF.
#ifdef DEBUG_VS
    if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
    {
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
        pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif
    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
    }

    return true;
}

//--------------------------------------------------------------------------------------
void OnSize()
{
/*
    RECT rcClient;
    GetClientRect( hDialog, &rcClient );
    int clientWidth = rcClient.right - rcClient.left;
    int clientHeight = rcClient.bottom - rcClient.top;
    int renderWidth = clientWidth;
    int renderHeight = clientHeight - STATUS_HEIGHT - STATUS_BORDER;
    //MoveWindow( GetDlgItem(g_hDlg, IDC_RENDER), 0, 0, renderWidth, renderHeight, true );//We dont really need this window...its a place holder
    MoveWindow( DXUTGetHWNDDeviceWindowed(), 0, 0, renderWidth, renderHeight, true );
    MoveWindow( GetDlgItem(g_hDlg, IDC_STATUS), 0, rcClient.bottom - STATUS_HEIGHT, clientWidth, STATUS_HEIGHT, true );

    DxRender::ResizeCamera();

	DxContent::g_DoRefresh = true;
*/
}

//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
/*
//trabalhar a iluminação e o plano de trabalho
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;


    V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );
    // Initialize the font
    V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"Arial", &g_pFont ) );
tá tudo cagado,
não sei o que fazer,
a declaração do vertex shader tá muito complicada
tente olhar em outro exemplo que não faça tanta merda 
    // Create vertex shader
//e    WCHAR        strPath[512];
//e   LPD3DXBUFFER pCode;

//e original
//e    D3DVERTEXELEMENT9 decl[] =
//e    {
//e        { 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
//e        D3DDECL_END()
//e    };

	D3DVERTEXELEMENT9 decl[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        D3DDECL_END()
    };

//tD3DVERTEXELEMENT9 VertexElements[] =
//t{
//t    { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
//t    { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
//t    { 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
//t    D3DDECL_END()
//t};


    V_RETURN( pd3dDevice->CreateVertexDeclaration( decl, &g_pVertexDeclaration ) );
    
    // Find the vertex shader file
 //e   V_RETURN( DXUTFindDXSDKMediaFileCch( strPath, 512, L"StereoVision.vsh" ) );
    

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger. Debugging vertex shaders requires either REF or software vertex 
    // processing, and debugging pixel shaders requires REF.  The 
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
    // shader debugger.  It enables source level debugging, prevents instruction 
    // reordering, prevents dead code elimination, and forces the compiler to compile 
    // against the next higher available software target, which ensures that the 
    // unoptimized shaders do not exceed the shader model limitations.  Setting these 
    // flags will cause slower rendering since the shaders will be unoptimized and 
    // forced into software.  See the DirectX documentation for more information about 
    // using the shader debugger.
    DWORD dwShaderFlags = 0;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION|D3DXSHADER_DEBUG;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION|D3DXSHADER_DEBUG;
    #endif

 //r   // Assemble the vertex shader from the file
 //r   V_RETURN( D3DXCompileShaderFromFile( strPath, NULL, NULL, "Ripple",
//r                                         "vs_1_1", dwShaderFlags, &pCode,
//r                                         NULL, &g_pConstantTable ) );

    // Create the vertex shader
//r    hr = pd3dDevice->CreateVertexShader( (DWORD*)pCode->GetBufferPointer(),
//r                                            &g_pVertexShader );
//r    pCode->Release();
//r    if( FAILED(hr) )
//r        return DXTRACE_ERR( TEXT("CreateVertexShader"), hr );


    // Setup the camera's view parameters
    g_Camera.SetViewQuat( D3DXQUATERNION(-0.275f, 0.3f, 0.0f, 0.7f) );


    return S_OK;
}
*/
//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
/*
//trabalhar a iluminação e o plano de trabalho
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;


    V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );
    // Initialize the font
    V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"Arial", &g_pFont ) );
tá tudo cagado,
não sei o que fazer,
a declaração do vertex shader tá muito complicada
tente olhar em outro exemplo que não faça tanta merda 
    // Create vertex shader
//e    WCHAR        strPath[512];
//e   LPD3DXBUFFER pCode;

//e original
//e    D3DVERTEXELEMENT9 decl[] =
//e    {
//e        { 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
//e        D3DDECL_END()
//e    };

	D3DVERTEXELEMENT9 decl[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        D3DDECL_END()
    };

//tD3DVERTEXELEMENT9 VertexElements[] =
//t{
//t    { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
//t    { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
//t    { 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
//t    D3DDECL_END()
//t};


    V_RETURN( pd3dDevice->CreateVertexDeclaration( decl, &g_pVertexDeclaration ) );
    
    // Find the vertex shader file
 //e   V_RETURN( DXUTFindDXSDKMediaFileCch( strPath, 512, L"StereoVision.vsh" ) );
    

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger. Debugging vertex shaders requires either REF or software vertex 
    // processing, and debugging pixel shaders requires REF.  The 
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
    // shader debugger.  It enables source level debugging, prevents instruction 
    // reordering, prevents dead code elimination, and forces the compiler to compile 
    // against the next higher available software target, which ensures that the 
    // unoptimized shaders do not exceed the shader model limitations.  Setting these 
    // flags will cause slower rendering since the shaders will be unoptimized and 
    // forced into software.  See the DirectX documentation for more information about 
    // using the shader debugger.
    DWORD dwShaderFlags = 0;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION|D3DXSHADER_DEBUG;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION|D3DXSHADER_DEBUG;
    #endif

 //r   // Assemble the vertex shader from the file
 //r   V_RETURN( D3DXCompileShaderFromFile( strPath, NULL, NULL, "Ripple",
//r                                         "vs_1_1", dwShaderFlags, &pCode,
//r                                         NULL, &g_pConstantTable ) );

    // Create the vertex shader
//r    hr = pd3dDevice->CreateVertexShader( (DWORD*)pCode->GetBufferPointer(),
//r                                            &g_pVertexShader );
//r    pCode->Release();
//r    if( FAILED(hr) )
//r        return DXTRACE_ERR( TEXT("CreateVertexShader"), hr );


    // Setup the camera's view parameters
    g_Camera.SetViewQuat( D3DXQUATERNION(-0.275f, 0.3f, 0.0f, 0.7f) );


    return S_OK;
}
*/
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

   	//i V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
    V_RETURN( g_DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
	//i V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnD3D9CreateDevice( pd3dDevice ) );
    
    // Initialize the font
    V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"Arial", &g_pFont ) );


	/*D3DVERTEXELEMENT9 decl[] =
	{
    { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
    { 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
	};*/

	//jD3DVERTEXELEMENT9 decl[] =
    //j{
    //j    { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	//j	{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
    //j    D3DDECL_END()
    //j};

	D3DVERTEXELEMENT9 decl[] =
	{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
	};

    V_RETURN( pd3dDevice->CreateVertexDeclaration( decl, &g_pVertexDeclaration ) );

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger. Debugging vertex shaders requires either REF or software vertex 
    // processing, and debugging pixel shaders requires REF.  The 
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
    // shader debugger.  It enables source level debugging, prevents instruction 
    // reordering, prevents dead code elimination, and forces the compiler to compile 
    // against the next higher available software target, which ensures that the 
    // unoptimized shaders do not exceed the shader model limitations.  Setting these 
    // flags will cause slower rendering since the shaders will be unoptimized and 
    // forced into software.  See the DirectX documentation for more information about 
    // using the shader debugger.
    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    // Read the D3DX effect file
 //   WCHAR str[MAX_PATH];
 //   V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"SHFuncView.fx" ) );//"StereoVision.fx" ) );

    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
 //   V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags, 
  //                                      NULL, &g_pEffect, NULL ) );


	
/*

    // Setup the camera's view parameters
	// World transform to identity
    D3DXMATRIXA16 mIdent;
    D3DXMatrixIdentity( &mIdent );
    V_RETURN( pd3dDevice->SetTransform( D3DTS_WORLD, &mIdent ) );

    // Setup the camera's view parameters
    D3DXVECTOR3 vFromPt( 0.0f, 0.0f, -2.5f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    g_Camera.SetViewParams( &vFromPt, &vLookatPt );*/


    //D3DXVECTOR3 vecEye(p.x,p.y,p.z);// Posição inicial da camera
   // D3DXVECTOR3 vecAt (0.0f, 0.0f, -0.0f);
   // g_Camera.SetViewParams( &vecEye, &vecAt );

	// Setup the camera's view parameters
//t    g_Camera.SetViewQuat( D3DXQUATERNION(-0.275f, 0.3f, 0.0f, 0.7f) );

    return S_OK;
}

//vertices iguais a resolução da imagem 800x600 = 480.000 vertices
//entender
//vertices 64 x 64
//indice 6 * 63 * 63

//vertices e indices de teste
//0		1
//(-10,10,0)	(10,10,0)
//
//3		2
//(-10,0,0)	(10,0,0)

//0
//1
//2

//0
//2
//3

//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

	
    //i V_RETURN( g_DialogResourceManager.OnResetDevice() );
	V_RETURN( g_DialogResourceManager.OnD3D9ResetDevice() );
    //i V_RETURN( g_SettingsDlg.OnResetDevice() );
	V_RETURN( g_SettingsDlg.OnD3D9ResetDevice() );

    if( g_pFont )
        V_RETURN( g_pFont->OnResetDevice() );
    
    // Create a sprite to help batch calls when drawing many lines of text
    V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pTextSprite ) );

    // Setup render states
    pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pd3dDevice->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_GOURAUD);

	//pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	//pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

    pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);//D3DCULL_NONE );

    // Create and initialize index buffer
    WORD* pIndices;
	WORD* pIndicesSave;

	V_RETURN( pd3dDevice->CreateIndexBuffer( g_dwNumIndices * sizeof(WORD),
                                              0, D3DFMT_INDEX16,
                                               D3DPOOL_DEFAULT, &g_pIB, NULL ) );

//f	V_RETURN( pd3dDevice->CreateIndexBuffer( (g_dwNumIndices+12) * sizeof(WORD),
//f                                              0, D3DFMT_INDEX16,
//f                                               D3DPOOL_DEFAULT, &g_pIB, NULL ) );

//r	V_RETURN( pd3dDevice->CreateIndexBuffer( 12 * sizeof(WORD),
//r                                               0, D3DFMT_INDEX16,
//r                                               D3DPOOL_DEFAULT, &g_pIB, NULL ) );
       
    V_RETURN( g_pIB->Lock( 0, 0, (void**)&pIndices, 0 ) );

	pIndicesSave=pIndices;
	

//	acredito que as faces estão visiveis dos dois lados....
//		não estou entendedo o Z (testar com Zs menores)
   
//Primeiro Triangulo
//y-1 ____ y-1
//x-1|   / x-0
//   |  /  
//   | /   
//y-0|/
//x-1			

//Segundo Triangulo
//		     y-1
//		   /|x-0
//        / |
//       /  |
//y-0   /___|y-0
//x-1		 x-0


   DWORD  x,y;
    for(y = 1; y < VERTS_PER_EDGE_HEIGHT; y++ )
		{
        for(x = 1; x < VERTS_PER_EDGE_WIDTH; x++ )
			{
			//Primeiro Triangulo
            *pIndices++ = (WORD)( (y-1)*VERTS_PER_EDGE_HEIGHT + (x-1) );
            *pIndices++ = (WORD)( (y-0)*VERTS_PER_EDGE_HEIGHT + (x-1) );
            *pIndices++ = (WORD)( (y-1)*VERTS_PER_EDGE_HEIGHT + (x-0) );

			//Segundo Triangulo
            *pIndices++ = (WORD)( (y-1)*VERTS_PER_EDGE_HEIGHT + (x-0) );
            *pIndices++ = (WORD)( (y-0)*VERTS_PER_EDGE_HEIGHT + (x-1) );
            *pIndices++ = (WORD)( (y-0)*VERTS_PER_EDGE_HEIGHT + (x-0) );
			}
		}
//f	//indices para dois quadrados se ativar este código nao esqueça do CreateIndexBuffer,CreateVertexBuffer,DrawIndexedPrimitive
//f	*pIndices=(WORD)g_dwNumVertices;pIndices++;
//f	*pIndices=(WORD)g_dwNumVertices+1;pIndices++;
//f	*pIndices=(WORD)g_dwNumVertices+2;pIndices++;

//f	*pIndices=(WORD)g_dwNumVertices;pIndices++;
//f	*pIndices=(WORD)g_dwNumVertices+2;pIndices++;
//f	*pIndices=(WORD)g_dwNumVertices+3;pIndices++;


//f	*pIndices=(WORD)g_dwNumVertices+4;pIndices++;
//f	*pIndices=(WORD)g_dwNumVertices+5;pIndices++;
//f	*pIndices=(WORD)g_dwNumVertices+6;pIndices++;

//f	*pIndices=(WORD)g_dwNumVertices+4;pIndices++;
//f	*pIndices=(WORD)g_dwNumVertices+6;pIndices++;
//f	*pIndices=(WORD)g_dwNumVertices+7;

  //teste   V_RETURN( g_pIB->Unlock() );

	
    // Create and initialize vertex buffer
//r  	V_RETURN( pd3dDevice->CreateVertexBuffer(	8 * sizeof(D3DNMVERTEX),//D3DUSAGE_WRITEONLY, 
//r												0, 
//r												D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1,
//r                                              D3DPOOL_DEFAULT, &g_pVB, NULL ) );

	V_RETURN( pd3dDevice->CreateVertexBuffer( g_dwNumVertices * sizeof(D3DNMVERTEX),
                                              0, 
											  D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1,
                                              D3DPOOL_DEFAULT, &g_pVB, NULL ) );
	
//f	V_RETURN( pd3dDevice->CreateVertexBuffer( (g_dwNumVertices+8) * sizeof(D3DNMVERTEX),
//f                                              0, 
//f											  D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1,
//f                                              D3DPOOL_DEFAULT, &g_pVB, NULL ) );
	

//t	V_RETURN( pd3dDevice->CreateVertexBuffer( g_dwNumVertices * sizeof(Vertex),//D3DUSAGE_WRITEONLY, 
//t                                                0, 
//t												D3DFVF_XYZ |  D3DFVF_DIFFUSE,
//t                                                 D3DPOOL_DEFAULT, &g_pVB, NULL ) );
//original
//e V_RETURN( pd3dDevice->CreateVertexBuffer( g_dwNumVertices * sizeof(D3DXVECTOR3),
//e                                              0, 0,
//e                                                 D3DPOOL_DEFAULT, &g_pVB, NULL ) );
    //D3DXVECTOR3 *pVertices;
	
	D3DNMVERTEX* pNMVertex = NULL;
	D3DNMVERTEX* pNMVertexSave;
    V_RETURN( g_pVB->Lock( 0, 0, (void**)&pNMVertex, 0 ) );

	pNMVertexSave=pNMVertex;
	//j Vertex* pVertex = NULL;
    //j V_RETURN( g_pVB->Lock( 0, 0, (void**)&pVertex, 0 ) );

	//t Color pixelColor;
	//t Bitmap DispMap(L"tsukubaL.bmp");//(L"DisparityMap.jpg");//i

	//pVertex->Position = D3DXVECTOR3( (float)-5.0 , (float)5.0, (float)10.0 );

	//Cuidado no directX9 é : X Y Z

	
	//WCHAR text[40];
	//LPCTSTR pszFormat = TEXT("%d\t%d\t%d\t%d\t%d\n");
	
	
	
	

	float stepH,stepV;//,stepVprodY;
	stepH=globalWidth/(float)VERTS_PER_EDGE_WIDTH;
	stepV=globalHeight/(float)VERTS_PER_EDGE_HEIGHT;
	float globalWidthDiv2=(float)globalWidth/2;//deslocamento à direita do 0
	float globalHeightDiv2=(float)globalHeight/2;//deslocamento a baixo do 0

	float stepUmap,stepVmap;
	stepUmap=1/(float)VERTS_PER_EDGE_WIDTH;
	stepVmap=1/(float)VERTS_PER_EDGE_HEIGHT;

	//x StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,stepH,stepV);
	//x log(text);


	unsigned long *pointLocDispMap;
	pointLocDispMap=(unsigned long *)voidDispMap;
	//test IMGpixel *pointLocDispMap;
	//test pointLocDispMap=(IMGpixel *)voidDispMap;
	//test float cor;
	

	//este x e y vão de 0 até 64 sendo que multiplicados pelo stepH o x vai de 0 até width e por stepV o y vai de 0 até height
	float xv,yv,u,v;
	unsigned long positionXY;
	//w for(y=0,v=0; y < VERTS_PER_EDGE_HEIGHT; y++,v+=stepVmap )
	for(yv=0,v=0; yv < globalHeight; yv+=stepV,v+=stepVmap )
		{
		
		//w stepVprodY=stepV*y;
		//w for( x=0,u=0; x < VERTS_PER_EDGE_WIDTH; x++,u+=stepUmap )
		for( xv=0,u=0; xv < globalWidth; xv+=stepH,u+=stepUmap )
			{
				x=(unsigned long)xv;//trunca
				y=(unsigned long)yv;//trunca pois não existe 2,5 * width (ou é 2 ou é 3)
				positionXY=x+(((globalHeight-1)-y)*globalWidth);
				//StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,x,y,positionXY,x+(y*globalWidth),pointLocDispMap[positionXY].disp);
				//log(text);

				//w pNMVertex->v = D3DXVECTOR3( (stepH*x)-globalWidthDiv2, stepVprodY-globalHeightDiv2, (float)pointLocDispMap[positionXY].disp*1);//10 foi mágico //1.0f);
				//test pointLocDispMap=(IMGpixel *)voidDispMap;
				//test pointLocDispMap+=positionXY;
				//test cor=((float)-pointLocDispMap->r-(float)pointLocDispMap->g-(float)pointLocDispMap->b);//junta todos canais e divide por 3
				//test cor=cor*10;//escala
				// em 3D o y cresce para cima e na imagem ele cresce para baixo como o disparity map veio de uma imagem tem que inverter como o tv
				//test pNMVertex->v = D3DXVECTOR3( xv-globalWidthDiv2, yv-globalHeightDiv2, cor);//Mudei para ser a própria cor//(float)(maxDisparity-pointLocDispMap[positionXY])*10);//10 foi mágico //1.0f);
				pNMVertex->v = D3DXVECTOR3( xv-globalWidthDiv2, yv-globalHeightDiv2, (float)(255-pointLocDispMap[positionXY]));//Mudei para ser a própria cor//(float)(maxDisparity-pointLocDispMap[positionXY])*10);//10 foi mágico //1.0f);
				//transforme 10 em uma função relacionada com a disparidade. exemplo se é desejado que o valor max seja 100 faça que onde esta 10 seja um valor variavel que ajusta o resultado final, ..... sei lá pensa ai
				
				//x StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,(stepH*x)-(globalWidth/2),stepVprodY-(globalHeight/2));
				//x log(text);
				pNMVertex->n  = D3DXVECTOR3(  0.0f, 1.0f, 0.0f);
				pNMVertex->tu = u;
				pNMVertex->tv = 1-v;//em 3D o y cresce para cima e na imagem ele cresce para baixo
				pNMVertex++;
			}
		}

	exportVRML(GVRMLfile,pNMVertexSave,pIndicesSave);
//f	//Vertices para dois quadrados se ativar este código não esqueça do CreateIndexBuffer,CreateVertexBuffer,DrawIndexedPrimitive
//f	pNMVertex->v  = D3DXVECTOR3( -0.3f, 0.3f, 1.0f);
//f	pNMVertex->n  = D3DXVECTOR3(  0.0f, 0.0f,-1.0f);
//f	pNMVertex->tu = 0.0f;
//f	pNMVertex->tv = 0.0f;
//f	pNMVertex++;

//f	pNMVertex->v  = D3DXVECTOR3( 0.3f , 0.3f, 1.0f);
//f	pNMVertex->n  = D3DXVECTOR3(  0.0f, 0.0f,-1.0f);
//f	pNMVertex->tu = 1.0f;
//f	pNMVertex->tv = 0.0f;
//f	pNMVertex++;

//f	pNMVertex->v  = D3DXVECTOR3( 0.3f, -0.3f, 1.0f);
//f	pNMVertex->n  = D3DXVECTOR3( 0.0f,  0.0f,-1.0f);
//f	pNMVertex->tu = 1.0f;
//f	pNMVertex->tv = 1.0f;
//f	pNMVertex++;

//f	pNMVertex->v  = D3DXVECTOR3( -0.3f , -0.3f, 1.0f);
//f	pNMVertex->n  = D3DXVECTOR3(  0.0f, 0.0f,-1.0f);
//f	pNMVertex->tu = 0.0f;
//f	pNMVertex->tv = 1.0f;
//f	pNMVertex++;


//f	pNMVertex->v = D3DXVECTOR3( -0.7f, -0.7f, 1.0f);
//f	pNMVertex->n  = D3DXVECTOR3(  0.0f, 1.0f, 0.0f);
//f	pNMVertex->tu = 0.0f;
//f	pNMVertex->tv = 0.0f;
//f	pNMVertex++;

//f	pNMVertex->v = D3DXVECTOR3( 0.7f , -0.7f, 1.0f);
//f	pNMVertex->n  = D3DXVECTOR3(  0.0f, 1.0f, 0.0f);
//f	pNMVertex->tu = 1.0f;
//f	pNMVertex->tv = 0.0f;
//f	pNMVertex++;

//f	pNMVertex->v = D3DXVECTOR3( 0.7f , -0.7f, -1.0f);
//f	pNMVertex->n  = D3DXVECTOR3(  0.0f, 1.0f, 0.0f);
//f	pNMVertex->tu = 1.0f;
//f	pNMVertex->tv = 1.0f;
//f	pNMVertex++;

//f	pNMVertex->v = D3DXVECTOR3( -0.7f , -0.7f, -1.0f);
//f	pNMVertex->n  = D3DXVECTOR3(  0.0f, 1.0f, 0.0f);
//f	pNMVertex->tu = 0.0f;
//f	pNMVertex->tv = 1.0f;
	
/*t
    for( y = 0; y < VERTS_PER_EDGE_HEIGHT; y++ )
    {
        for( DWORD x = 0; x < VERTS_PER_EDGE_WIDTH; x++ )
        {
			DispMap.GetPixel(x, y, &pixelColor);

			//pVertex->Position = D3DXVECTOR3( ((float)x / (float)(VERTS_PER_EDGE_WIDTH-1) - 0.5f) * D3DX_PI, ((float)y / (float)(VERTS_PER_EDGE_HEIGHT-1) - 0.5f) * D3DX_PI, 0 );
			pVertex->Position = D3DXVECTOR3( (float)x , (float)y,0 );
			pVertex->Diffuse = D3DCOLOR_ARGB( 255, 128, 128, 128 );
			pVertex++;
			//*pVertices++ = D3DXVECTOR2( ((float)x / (float)(VERTS_PER_EDGE_WIDTH-1) - 0.5f) * D3DX_PI,
            //                            ((float)y / (float)(VERTS_PER_EDGE_HEIGHT-1) - 0.5f) * D3DX_PI );
   //h        *pVertices++ = D3DXVECTOR3( ((float)x / (float)(VERTS_PER_EDGE_WIDTH-1) - 0.5f) * D3DX_PI,
   //h                                     ((float)y / (float)(VERTS_PER_EDGE_HEIGHT-1) - 0.5f) * D3DX_PI,
//h										(pixelColor.GetR() / (float)(VERTS_PER_EDGE_WIDTH-1) - 0.5f) * D3DX_PI);//0);
		//*pVertices++ = D3DXVECTOR3( (float)x*0.5f ,
          //                              (float)y*0.5f ,
			//							pixelColor.GetR());
//r
//r			if(x<20)
//r				*pVertices++ = D3DXVECTOR3( ((float)x / (float)(VERTS_PER_EDGE_WIDTH-1) - 0.5f) * D3DX_PI,
//r                                        ((float)y / (float)(VERTS_PER_EDGE_HEIGHT-1) - 0.5f) * D3DX_PI,
//r										-200);
//r			if((x>=20)&&(x<40))
//r				*pVertices++ = D3DXVECTOR3( ((float)x / (float)(VERTS_PER_EDGE_WIDTH-1) - 0.5f) * D3DX_PI,
//r                                        ((float)y / (float)(VERTS_PER_EDGE_HEIGHT-1) - 0.5f) * D3DX_PI,
//r										0);
//r			if(x>=40)
//r				*pVertices++ = D3DXVECTOR3( ((float)x / (float)(VERTS_PER_EDGE_WIDTH-1) - 0.5f) * D3DX_PI,
//r                                        ((float)y / (float)(VERTS_PER_EDGE_HEIGHT-1) - 0.5f) * D3DX_PI,
//r										200);
			
        }
    }*/
	V_RETURN( g_pIB->Unlock() );// o unlock dos indices veio para cá para ocorrer depois do exportVRML()
    V_RETURN( hr = g_pVB->Unlock() );


	//leiaTextur(pd3dDevice,L"tsukubaL.bmp");
	D3DXCreateTextureFromFile(pd3dDevice,GleftFile, &pTexture);
	//D3DXCreateTextureFromFile(pd3dDevice,L"DisparityMap.jpg", &pTexture);
	SetupMatrices(pd3dDevice);
/*
    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI/4, fAspectRatio, 0.1f, 1000.0f );//1.0f, 1000.0f );//
	//SetProjectionMatrix( matProj, (float)0.78539, 1.0f, 1.0f, 6000.0f );//o ultimo parametro define a profundidade do campo visual, (float)0.78539=pi/4 , linha vinda do NovoMundo 
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

    g_HUD.SetLocation( pBackBufferSurfaceDesc->Width-170, 0 );
    g_HUD.SetSize( 170, 170 );*/
    
    return S_OK;
}




void exportVRML(WCHAR *filename,D3DNMVERTEX* pNMVertex,WORD* pIndices)
	{
	unsigned int i;
	FILE *fileptr;
	WCHAR header1[768];
	WCHAR header2[768];
	SYSTEMTIME atual;

	//vertices, normais e wrap
	D3DNMVERTEX* pNMVertexLocal;
	

	//Indices
	WORD* pIndicesLocal;
	WORD a,b,c;
	

	GetLocalTime(&atual);
//	sprintf(date,"%d/%d/%d",atual.wDay,atual.wMonth,atual.wYear);

	if(voidImgR!=0)
		{
			LPCTSTR pszFormat1 = TEXT("#VRML V2.0 utf8\n\n# Produced by Stereo Vision - Marcelo Archanjo José - 2006\n# VRML File: %s, Date: %d/%d/%d Time: %d:%d:%d\n# Left Image: %s, Right Image: %s, Max Disparity: %u, Max Line Variation: %u, Window Size: %u,Number of X vertex: %u,Number of Y vertex: %u\n\n");
		StringCbPrintfW(header1, (768*sizeof(WCHAR)),pszFormat1,filename,atual.wDay,atual.wMonth,atual.wYear,atual.wHour,atual.wMinute,atual.wSecond,GleftFile,GrightFile,GmaxDisp,GmaxLineVar,GwindowSize,VERTS_PER_EDGE_WIDTH,VERTS_PER_EDGE_HEIGHT);
		}
	else
		{
		LPCTSTR pszFormat1 = TEXT("#VRML V2.0 utf8\n\n# Produced by Stereo Vision - Marcelo Archanjo José - 2006\n# VRML File: %s, Date: %d/%d/%d Time: %d:%d:%d\n# Disparity Map: %s, Left Image: %s, Number of X vertex: %u,Number of Y vertex: %u\n\n\n");
		StringCbPrintfW(header1, (768*sizeof(WCHAR)),pszFormat1,filename,atual.wDay,atual.wMonth,atual.wYear,atual.wHour,atual.wMinute,atual.wSecond,GdisparityMap,GleftFile,VERTS_PER_EDGE_WIDTH,VERTS_PER_EDGE_HEIGHT);
		}



	LPCTSTR pszFormat2 = TEXT("DEF Plane01 Transform {\n  translation 1.554 0 3.765\n  children [\n    Shape {\n      appearance Appearance {\n        material Material {\n          diffuseColor 0.7569 0.7569 0.7569\n          ambientIntensity 0.7772\n          specularColor 0 0 0\n          shininess 0.145\n          transparency 0\n        }\n        texture ImageTexture {\n          url \"%s\"\n        }\n      }\n      geometry DEF Plane01-FACES IndexedFaceSet {\n        ccw TRUE\n        solid TRUE\n        coord DEF Plane01-COORD Coordinate { point [\n");
	StringCbPrintfW(header2, (768*sizeof(WCHAR)),pszFormat2,GleftFile);

	
	errno_t err;
	if( (err  =_wfopen_s( &fileptr,filename,L"w" )) ==0 )
	//i if((fileptr=_wfopen(filename,L"w")) != NULL)
		{
		//"Header"
		fwprintf(fileptr,L"%s",header1);
		fwprintf(fileptr,L"%s",header2);

		//vertices são divididos por 10 para ficar menor no VRML Player
		pNMVertexLocal=pNMVertex;
		fwprintf(fileptr,L"          %0.3f %0.3f %0.3f",pNMVertexLocal->v.x/10,pNMVertexLocal->v.y/10,-pNMVertexLocal->v.z/10);//antes do for por causa da virgula
		pNMVertexLocal++;
		for(i=1;i<g_dwNumVertices;i++)
			{
			fwprintf(fileptr,L",\n          %0.3f %0.3f %0.3f",pNMVertexLocal->v.x/10,pNMVertexLocal->v.y/10,-pNMVertexLocal->v.z/10);
			pNMVertexLocal++;	
			}

		//Wrap - Texture Coordinates
		pNMVertexLocal=pNMVertex;
		fwprintf(fileptr,L"]\n        }\n        texCoord DEF Plane01-TEXCOORD TextureCoordinate { point [\n");
		
		fwprintf(fileptr,L"          %0.3f %0.3f",pNMVertexLocal->tu,1-pNMVertexLocal->tv);//antes do for por causa da virgula
		pNMVertexLocal++;
		for(i=1;i<g_dwNumVertices;i++)
			{
			fwprintf(fileptr,L",\n          %0.3f %0.3f",pNMVertexLocal->tu,1-pNMVertexLocal->tv);
			pNMVertexLocal++;	
			}
		//Index - para vertices
		pIndicesLocal=pIndices;
		fwprintf(fileptr,L"]\n        }\n        coordIndex [\n");

		a=*pIndicesLocal;pIndicesLocal++;
		b=*pIndicesLocal;pIndicesLocal++;
		c=*pIndicesLocal;pIndicesLocal++;
		fwprintf(fileptr,L"          %d, %d, %d, -1",a,c,b);
		
		for(i=3;i<g_dwNumIndices;i+=3)
			{
			a=*pIndicesLocal;pIndicesLocal++;
			b=*pIndicesLocal;pIndicesLocal++;
			c=*pIndicesLocal;pIndicesLocal++;
			fwprintf(fileptr,L",\n          %d, %d, %d, -1",a,c,b);
			}

		//Index - para texturas (igual ao dos vertices)
		pIndicesLocal=pIndices;
		fwprintf(fileptr,L"]\n        texCoordIndex [\n");

		a=*pIndicesLocal;pIndicesLocal++;
		b=*pIndicesLocal;pIndicesLocal++;
		c=*pIndicesLocal;pIndicesLocal++;
		fwprintf(fileptr,L"          %d, %d, %d, -1",a,c,b);

		for(i=3;i<g_dwNumIndices;i+=3)
			{
			a=*pIndicesLocal;pIndicesLocal++;
			b=*pIndicesLocal;pIndicesLocal++;
			c=*pIndicesLocal;pIndicesLocal++;
			fwprintf(fileptr,L",\n          %d, %d, %d, -1",a,c,b);
			}

		//"Footer"
		fwprintf(fileptr,L"]\n        }\n    }\n  ]\n}\n");

		fclose(fileptr);
		}
	}




//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
//ivoid CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
//i{/*
//i    // Update the camera's position based on user input 
//i    g_Camera.FrameMove( fElapsedTime );

//i   // Set up the vertex shader constants
//i   D3DXMATRIXA16 mWorldViewProj;
//i    D3DXMATRIXA16 mWorld;
//i    D3DXMATRIXA16 mView;
//i    D3DXMATRIXA16 mProj;

//i    mWorld = *g_Camera.GetWorldMatrix();
//i    mView  = *g_Camera.GetViewMatrix();
//i    mProj  = *g_Camera.GetProjMatrix();

	
//i	//WCHAR text[512];
//i	//LPCTSTR pszFormat = TEXT("%0.0f\t%0.0f\t%0.0f\t%0.0f\n%0.0f\t%0.0f\t%0.0f\t%0.0f\n%0.0f\t%0.0f\t%0.0f\t%0.0f\n%0.0f\t%0.0f\t%0.0f\t%0.0f\n\n");
//i	//StringCbPrintfW(text, (40*sizeof(WCHAR)),pszFormat,mView._11,mView._12,mView._13,mView._14,mView._21,mView._22,mView._23,mView._24,mView._31,mView._32,mView._33,mView._34,mView._41,mView._42,mView._43,mView._44);
//i	//log(text);

//i	//Defaut View Matriz 
//i	//	1	0	0	0
//i	//	0	1	0	0
//i	//	0	0	1	0
//i	//	0	0	2	1

//i    mWorldViewProj = mWorld * mView * mProj;

//i//   g_pEffect->SetMatrix(  "mWorldViewProj", &mWorldViewProj );
//i//   g_pEffect->SetFloat(  "fTime", (float) fTime );
	
//i*/
//i}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
}
//OBRIGADO MEU DEUS.... DEU CERTO....

void SetupMatrices(IDirect3DDevice9* pd3dDevice)
{
	//World Matrix e uma matriz Identidentidade
	montaMatrizIdentidade(matWorld);
	//View Matrix
	montaViewMatrizTurbo();
		 	
   //Projection Matrix
	float fAspectRatio = (float)height/(float)width ;
	SetProjectionMatrix( matProj, (float)0.78539, fAspectRatio, 0.1f, 6000.0f );//o ultimo parametro define a profundidade do campo visual
	//0.78539 pi/4
    pd3dDevice->SetTransform( D3DTS_WORLD,      &matWorld );
    pd3dDevice->SetTransform( D3DTS_VIEW,       &matView );
    pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

 }


//parabens, a navegação esta master (incluindo o direct Input) agora o proximo passo é ativar materiais e luzes,
//porem entendo que isto deve estar totalmente vinculado ao tratamento do 3ds.lib
// *****************************************************************
// ****************** montaViewMatrizTurbo *************************
// ** eh a funcao que esta otimizada, para aumentar a performance **
// **** pois trata os graus como indices, sem calcular sen e coss **
// *****************************************************************


void montaViewMatrizTurbo(void)
	{
	D3DMATRIX matTemp,matRot;
	// ******* Translacao *********
	montaMatrizIdentidade(matView);
	matView._41=-p.x;//mudei em relação ao NovoMundo DX8 aqui era usado cam ao invés de p
	matView._42=-p.y;
	matView._43=-p.z;

//o	matView._41=-cam.x;
//o	matView._42=-cam.y;
//o	matView._43=-cam.z;
	// ******** "Calculos" *********
	calcSenCoss(&senX,&cosX,&p.indiceSpinX);
	calcSenCoss(&senY,&cosY,&p.indiceSpinY);
	calcSenCoss(&senZ,&cosZ,&p.indiceSpinZ);


	// ****************************
	montaMatrizIdentidade(matTemp);
	// ****** Rotacao em X ********
	montaMatrizIdentidade(matRot);
	matRot._22 =  cosX;
    matRot._23 =  senX;
    matRot._32 = -senX;
    matRot._33 =  cosX;
	MatrixMultiply(matTemp,matTemp,matRot);
	//*****************************

	// ****** Rotacao em Y ********
	montaMatrizIdentidade(matRot);
	matRot._11 =  cosY;
    matRot._13 =-senY;
    matRot._31 =senY;
    matRot._33 =  cosY;
	MatrixMultiply(matTemp,matTemp,matRot);
	//*****************************

	// ****** Rotacao em Z ********
	montaMatrizIdentidade(matRot);
	matRot._11  =  cosZ;
    matRot._12  =senZ;//  senZ;mudei o sentido para ficar como se fosse regra da mao direita
    matRot._21  =-senZ;// -senZ;mudei o sentido para ficar como se fosse regra da mao direita
    matRot._22  =  cosZ;
	MatrixMultiply(matTemp,matTemp,matRot);
	//*****************************
	//MatrixMultiply(matView,matView,matTemp);//gira os objetos em relacao ao centro global
	MatrixMultiply(matView,matTemp,matView);//gira os objetos em relacao ao observador
	}

//porque nao vale o que fiz com as matrizes? , tenho a sensação de que é uma questão de ativar uma flag...
//-----------------------------------------------------------------------------
// Name: preparaLookup()
// Desc: Monta Lookup table para os angulos de movimento com passo minimo de 
//       0.25 graus
// indiceSpin :
//0		=	0		graus
//1		=	0.25	graus
//4		=	1		grau
//360	=	90		graus
//720	=	180		graus
//1080	=	270		graus
//1440	=	360		graus

//-----------------------------------------------------------------------------
void preparaLookup(void)
	{
	short i;
	double alfa;
	double pi=3.141592654;
	for(alfa=0,i=0;(alfa<=90);i++,alfa+=passoMinimo)//0.25 passo minimo mudar tambem angulos1Q
		lookup[i]=sin((alfa*pi)/180);

	// quadranteX eh o numero do ultimo passo que temos dentro de um quadrate
	// supondo que no primeiro quadrante de 0 a 90 graus tivermos passos de 0.25 
	// graus teremos 361 passos dentro do quadrante, logo o ultimo passo do quadrante
	// eh 360 ( 361= de 0 ate 360) quadrante1=360. O segundo quadrante para o mesmo 
	// exemplo temos 721 passos de 0 a 180 graus logo 720 sera o valor de quadrante2
	quadrante1=angulos1Q-1;
	quadrante2=quadrante1*2;
	quadrante3=quadrante1*3;
	quadrante4=quadrante1*4;
	}

void MatrixMultiply( D3DMATRIX& q, D3DMATRIX& a, D3DMATRIX& b )
{// ************************* Depois Otimizar retirando "for" aninhado ****************
    FLOAT* pA = (FLOAT*)&a;
    FLOAT* pB = (FLOAT*)&b;
    FLOAT  pM[16];

    ZeroMemory( pM, sizeof(D3DMATRIX) );
	
    for( WORD i=0; i<4; i++ ) 
        for( WORD j=0; j<4; j++ ) 
            for( WORD k=0; k<4; k++ ) 
                pM[4*i+j] += pA[4*k+j] * pB[4*i+k];

    memcpy( &q, pM, sizeof(D3DMATRIX) );
}

void calcSenCoss(float* seno,float* coss,signed short* spin)
	{
	if(*spin>=(quadrante4+quadrante4)) *spin=*spin%quadrante4;
	if(*spin<=-quadrante4) *spin=*spin%(-quadrante4);
	if(*spin<0) *spin=(short)(quadrante4+*spin);
	if(*spin>=quadrante4) *spin=(short)(*spin-quadrante4);
	

	if((*spin>=0)&&(*spin<=quadrante1))//primeiro quadrante
			{*seno=(float)*(lookup+*spin);*coss=(float)*(lookup+quadrante1-*spin);}
	if((*spin>quadrante1)&&(*spin<=quadrante2))//segundo quadrante 
			{*seno=(float)*(lookup+quadrante2-*spin);*coss=-(float)(*(lookup+*spin-quadrante1));}
	if((*spin>quadrante2)&&(*spin<=quadrante3))//terceiro quadrante 
			{*seno=-(float)(*(lookup+*spin-quadrante2));*coss=-(float)(*(lookup+quadrante3-*spin));}
	if((*spin>quadrante3)&&(*spin<quadrante4))//quarto quadrante
			{*seno=-(float)(*(lookup+quadrante4-*spin));*coss=(float)*(lookup+*spin-quadrante3);}
	}

//-----------------------------------------------------------------------------
// Name: D3DUtil_SetProjectionMatrix()
// Desc: Sets the passed in 4x4 matrix to a perpsective projection matrix built
//       from the field-of-view (fov, in y), aspect ratio, near plane (D),
//       and far plane (F). Note that the projection matrix is normalized for
//       element [3][4] to be 1.0. This is performed so that W-based range fog
//       will work correctly.
//-----------------------------------------------------------------------------
HRESULT SetProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,
                                     FLOAT fNearPlane, FLOAT fFarPlane )
{
    if( fabs(fFarPlane-fNearPlane) < 0.01f )
        return E_INVALIDARG;
    if( fabs(sin(fFOV/2)) < 0.01f )
        return E_INVALIDARG;

	FLOAT w = fAspect * (FLOAT)( cos(fFOV/2)/sin(fFOV/2) );
	FLOAT h =   1.0f  * (FLOAT)( cos(fFOV/2)/sin(fFOV/2) );
    FLOAT Q = fFarPlane / ( fFarPlane - fNearPlane );

    ZeroMemory( &mat, sizeof(D3DMATRIX) );
    mat._11 = w;
    mat._22 = h;
    mat._33 = Q;
    mat._34 = 1.0f;
    mat._43 = -Q*fNearPlane;

    return S_OK;
}
/*
void cameraNavigation(IDirect3DDevice9* pd3dDevice)
	{
	if(firstT) 
	{
	ViewMatrix._11=1;ViewMatrix._12=0;ViewMatrix._13=0;ViewMatrix._14=0;
    ViewMatrix._21=0;ViewMatrix._22=1;ViewMatrix._23=0;ViewMatrix._24=0;
    ViewMatrix._31=0;ViewMatrix._32=0;ViewMatrix._33=1;ViewMatrix._34=0;
    ViewMatrix._41=p.x;ViewMatrix._42=p.y;ViewMatrix._43=p.z;ViewMatrix._44=1;
	firstT=false;
	}
   
	//ViewMatrix._41=p.x;ViewMatrix._42=p.y;ViewMatrix._43=p.z;
	//pd3dDevice->SetTransform( D3DTS_VIEW, &ViewMatrix );
	montaViewMatrizTurbo();
	pd3dDevice->SetTransform( D3DTS_VIEW,&matView );	
	
	}*/

void posicaoInicial(void)
	{
	if(firstT)//Como o DirectX é re-inicializado no WM_PAINT a inicialização de p,old e cam somente ocorre na primeira chamada
		{
		preparaLookup();
		p.x=0.0f;
		p.y=0.0f;
		p.z=-400.0f;
		p.indiceSpinX=0;//1360;
		p.indiceSpinY=0;
		p.indiceSpinZ=0;

		old.x=0;//-35;//0;
		old.y=0;//2;
		old.z=-400.0f;//-150;
		old.indiceSpinX=0;
		old.indiceSpinY=0;
		old.indiceSpinZ=0;

		cam.x=0;//-23;//0;
		cam.y=0;//0;//+15;
		cam.z=-400.0f;//0;//-30;
		cam.indiceSpinX=0;// so inicializo nao uso
		cam.indiceSpinY=0;// so inicializo nao uso
		cam.indiceSpinZ=0;// so inicializo nao uso
		//firstT vira false na primeira chamada da função cameraNavigation()
		}
	}


//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, DXUT will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	//D3DMATERIAL9 mtrl;
    // If the settings dialog is being shown, then
    // render it instead of rendering the app's scene
    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.OnRender( fElapsedTime );
        return;
    }

    HRESULT hr;

	//navegação da camera
	//cameraNavigation(pd3dDevice);

	
	montaViewMatrizTurbo();
	pd3dDevice->SetTransform( D3DTS_VIEW,&matView );	
	

	if(wire) pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	else 	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	
	//VEJA COMO FICA no DirectX9
	//D3DFILL_WIREFRAME 
	//Fill wireframes. This fill mode currently does not work for clipped primitives when you use the DrawPrimitive methods.

	//pd3dDevice->SetRenderState( D3DRS_WRAP0 , D3DWRAP_U);//D3DCULL_NONE );
     
    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 107,124,178), 1.0f, 0) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
/*
		ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
        mtrl.Diffuse.r = mtrl.Diffuse.r = 0.0f;
        mtrl.Diffuse.g = mtrl.Diffuse.g = 0.0f;
        mtrl.Diffuse.b = mtrl.Diffuse.b = 1.0f;
        mtrl.Diffuse.a = mtrl.Diffuse.a = 1.0f;

		mtrl.Diffuse.r = mtrl.Ambient.r = 0.0f;
        mtrl.Diffuse.g = mtrl.Ambient.g = 0.0f;
        mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
        mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;

		mtrl.Diffuse.r = mtrl.Specular.r = 0.0f;
        mtrl.Diffuse.g = mtrl.Specular.g = 0.0f;
        mtrl.Diffuse.b = mtrl.Specular.b = 1.0f;
        mtrl.Diffuse.a = mtrl.Specular.a = 1.0f;
        pd3dDevice->SetMaterial( &mtrl );
		o material não está funcionando*/
        //pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial]  );
		
		if(changeImageDisp) //se mudou de imagem, somente carrega imagem se mudou.
			{
			changeImageDisp=false;//evita a chamada sucessiva
			if(image)D3DXCreateTextureFromFile(pd3dDevice,GleftFile, &pTexture);
			else D3DXCreateTextureFromFile(pd3dDevice,GdisparityMap, &pTexture);
			}
			
		
		pd3dDevice->SetTexture( 0, pTexture );
		
		//pd3dDevice->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);//D3DTSS_ADDRESSU não deu certo directx8
		//pd3dDevice->SetTextureStageState(0,14,D3DTADDRESS_WRAP);//D3DTSS_ADDRESSV não deu certo directx8
		
			//veja (se inverter o bitmap) na internet se é bug conhecido
//ok agora a brincadeira começa....

        pd3dDevice->SetVertexDeclaration( g_pVertexDeclaration );
   //e     pd3dDevice->SetVertexShader( g_pVertexShader );
		
		// **** Objeto 3D 
  //      pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(D3DXVECTOR3) );
		pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(D3DNMVERTEX) );
        pd3dDevice->SetIndices( g_pIB );

 //r       V( pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 8,
 //r                                            0, 4 ) );//4 número de faces
//f		V( pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, g_dwNumVertices+8,
//f                                             0, (g_dwNumIndices/3)+4 ) );

		V( pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, g_dwNumVertices,
                                             0, (g_dwNumIndices/3) ) );
		// ****
        
        RenderText();
        V( g_HUD.OnRender( fElapsedTime ) );
        
        V( pd3dDevice->EndScene() );
    }
}



//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
    // The helper object simply helps keep track of text position, and color
    // and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
    // If NULL is passed in as the sprite object, then it will work however the 
    // pFont->DrawText() will not be batched together.  Batching calls will improves performance.
    CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetInsertionPos( 5, 5 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    txtHelper.DrawTextLine( DXUTGetFrameStats() );
    txtHelper.DrawTextLine( DXUTGetDeviceStats() );

/*
    TODO: add UI text as needed
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    txtHelper.DrawTextLine( L"Put whatever misc status here" );
    
    // Draw help
    const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetBackBufferSurfaceDesc();
    if( g_bShowHelp )
    {
        txtHelper.SetInsertionPos( 10, pd3dsdBackBuffer->Height-15*6 );
        txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.75f, 0.0f, 1.0f ) );
        txtHelper.DrawTextLine( L"Controls (F1 to hide):" );

        txtHelper.SetInsertionPos( 40, pd3dsdBackBuffer->Height-15*5 );
        txtHelper.DrawTextLine( L"Quit: ESC" );
    }
    else
    {
        txtHelper.SetInsertionPos( 10, pd3dsdBackBuffer->Height-15*2 );
        txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
        txtHelper.DrawTextLine( L"Press F1 for help" );
    }
*/
    txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Before handling window messages, DXUT passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass all remaining windows messages to camera so it can respond to user input
    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}




//if( GetKeyState( 'W' ) < 0 )
//if( GetAsyncKeyState( VK_SHIFT ) < 0 )// VK_LSHIFT, VK_RSHIFT, VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU
//a função DXUTIsKeyDown não funciona bem, por isso eu criei esta função equivalente
bool TestKey( BYTE vKey )
	{
	if( vKey >= 0xA0 && vKey <= 0xA5 )  // VK_LSHIFT, VK_RSHIFT, VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU
		{
		if( GetAsyncKeyState( vKey ) < 0 ) return true;
		else return false;
		}
	else
		{
		if( GetKeyState( vKey ) < 0 ) return true;
		else return false;
		}
	}
//--------------------------------------------------------------------------------------
// As a convenience, DXUT inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	short flag;

    if( bKeyDown )
    {
	flag=0;
	//flag 0  nao usou
	//flag 1  generico
	//flag 2  andou para frente
	//flag 3  andou para frente com shift
	//flag 4  andou para traz 
	//flag 5  andou para traz com shift
	//flag 6  girou para a esquerda
	//flag 7  girou para a esquerda com shift
	//flag 8  girou para a direita
	//flag 9  girou para a direita com shift
	//flag 10 Sobe
	//flag 11 Desce
	

	//giro à esquerda
	if ( (TestKey(VK_LEFT)||TestKey(VK_NUMPAD4))&&
		 !(TestKey(VK_MENU)) )
		{p.indiceSpinY+=12;
		if(p.indiceSpinY>=quadrante4) p.indiceSpinY=(short)(p.indiceSpinY-quadrante4);
		flag=6;}

	//giro à esquerda com shift
	if ( (flag==6)&&(TestKey(VK_RSHIFT)||TestKey(VK_LSHIFT)) )
		{p.indiceSpinY+=12;
		if(p.indiceSpinY>=quadrante4) p.indiceSpinY=(short)(p.indiceSpinY-quadrante4);
		flag=7;}

	//giro à direita
	if ( (TestKey(VK_RIGHT)||TestKey(VK_NUMPAD6))&&
		 !(TestKey(VK_MENU)) )
		{p.indiceSpinY-=12;
		if(p.indiceSpinY<0) p.indiceSpinY=(short)(quadrante4+p.indiceSpinY);
		flag=8;}

	//giro à direita com shift
	if ( (flag==8)&&(TestKey(VK_RSHIFT)||TestKey(VK_LSHIFT)) )
		{p.indiceSpinY-=12;
		if(p.indiceSpinY<0) p.indiceSpinY=(short)(quadrante4+p.indiceSpinY);
		flag=9;}

	
	//olha para cima
	if (TestKey(VK_HOME)||TestKey(VK_NUMPAD7))
		{p.indiceSpinX+=12;
		if(p.indiceSpinX>=quadrante4)p.indiceSpinX=(short)(p.indiceSpinX-quadrante4);
		if((p.indiceSpinX>quadrante1)&&(p.indiceSpinX<quadrante3)) p.indiceSpinX=quadrante1;
		flag=1;}

	//olha para baixo
	if (TestKey(VK_END)||TestKey(VK_NUMPAD1))
		{p.indiceSpinX-=12;
		if(p.indiceSpinX<0)p.indiceSpinX=(short)(quadrante4+p.indiceSpinX);
		if((p.indiceSpinX>quadrante1)&&(p.indiceSpinX<quadrante3)) p.indiceSpinX=quadrante3;
		flag=1;}

	//centraliza a visão
	if (TestKey(VK_NEXT)||TestKey(VK_NUMPAD3))
		{p.indiceSpinX=0;
		flag=1;}

	//Sobe
	if (TestKey('A'))
		{p.y+=7;
		flag=10;}

	//Desce
	if (TestKey('Z'))
		{p.y-=7;
		flag=11;}

	// a linha abaixo deixa o movimento suave, pois usa sen e cos baseados
	// no indiceSpiny atualizado acima pelas teclas de giro
	calcSenCoss(&senY,&cosY,&p.indiceSpinY);

	//Anda para frente
	if ((TestKey(VK_UP)||TestKey(VK_NUMPAD8)) )
		{p.x=p.x-senY;p.x=p.x-senY;p.x=p.x-senY;p.x=p.x-senY;//isso mesmo 4 vezes a mesma funcao
		 p.z=p.z+cosY;p.z=p.z+cosY;p.z=p.z+cosY;p.z=p.z+cosY;
		 flag=2;}

	//Anda para frente com shift
	if ( (flag==2)&&(TestKey(VK_LSHIFT)||TestKey(VK_LSHIFT)) )
		{p.x=p.x-senY;p.x=p.x-senY;p.x=p.x-senY;p.x=p.x-senY;
		 p.z=p.z+cosY;p.z=p.z+cosY;p.z=p.z+cosY;p.z=p.z+cosY;
		 flag=3;}

	//Anda para traz	
	if  (TestKey(VK_DOWN)||TestKey(VK_NUMPAD2))
		{p.x=p.x+senY;p.x=p.x+senY;p.x=p.x+senY;p.x=p.x+senY;
		 p.z=p.z-cosY;p.z=p.z-cosY;p.z=p.z-cosY;p.z=p.z-cosY;
		 flag=4;}

	//Anda para traz com shift
	if ( (flag==4)&&(TestKey(VK_RSHIFT)||TestKey(VK_LSHIFT)) )
		{p.x=p.x+senY;p.x=p.x+senY;p.x=p.x+senY;p.x=p.x+senY;
		 p.z=p.z-cosY;p.z=p.z-cosY;p.z=p.z-cosY;p.z=p.z-cosY;
		 flag=5;}

	//anda lateralmente à esquerda
	if ( (TestKey(VK_LEFT)||TestKey(VK_NUMPAD4))&&
		 (TestKey(VK_MENU)) )
		{p.x=p.x-cosY;p.x=p.x-cosY;
		 p.z=p.z-senY;p.z=p.z-senY;
		 flag=1;}
				
	//anda lateralmente à direita
	if ( (TestKey(VK_RIGHT)||TestKey(VK_NUMPAD6))&&
		 (TestKey(VK_MENU)) )
		 {p.x=p.x+cosY;p.x=p.x+cosY;
		  p.z=p.z+senY;p.z=p.z+senY;
		  flag=1;}
	
	if(flag!=0)
		{
		

		calcSenCoss(&senY,&cosY,&p.indiceSpinY);

		/*
		usado para colision detection
		old.x=p.x;
		old.y=p.y;
		old.z=p.z;
		
		old.indiceSpinX=p.indiceSpinX;
		old.indiceSpinY=p.indiceSpinY;
		old.indiceSpinZ=p.indiceSpinZ;
		cam.x=p.x;//+(30*senY);
		cam.y=p.y;//+15;
		cam.z=p.z;//-(30*cosY);*/
		}


	/*if (TestKey(VK_HOME)||TestKey(VK_NUMPAD1))
				{p.indiceSpinX+=12;
				if(p.indiceSpinX>=quadrante4)p.indiceSpinX=(short)(p.indiceSpinX-quadrante4);
				if((p.indiceSpinX>quadrante1)&&(p.indiceSpinX<quadrante3)) p.indiceSpinX=quadrante1;}

	if(TestKey(VK_END)||TestKey(VK_NUMPAD1))
				{p.indiceSpinX-=12;
				if(p.indiceSpinX<0)p.indiceSpinX=(short)(quadrante4+p.indiceSpinX);
				if((p.indiceSpinX>quadrante1)&&(p.indiceSpinX<quadrante3)) p.indiceSpinX=quadrante3;}*/
	

       /* switch( nChar )
        {
            case VK_F1: g_bShowHelp = !g_bShowHelp; break;

			//case VK_UP:		p.z-=0.05f; break;
			//case VK_DOWN:	p.z+=0.05f;	break;
			//case VK_LEFT:	p.x+=0.05f;	break;
			//case VK_RIGHT:	p.x-=0.05f;	break;

			calcSenCoss(&senY,&cosY,&p.indiceSpinY);

			case 'A'://0x41://A
				{p.y+=7;
				 break;
				 }

			
			case 'Z'://0x5A://Z
				{p.y-=7;
				 break;
				 }


			//if ((press(DIK_UP)||press(DIK_NUMPAD8))||
			//	(estrutDIMouse.rgbButtons[0] & 0x80))
			case VK_UP:
				{p.x=p.x-senY;p.x=p.x-senY;p.x=p.x-senY;p.x=p.x-senY;//isso mesmo 4 vezes a mesma funcao
				 p.z=p.z+cosY;p.z=p.z+cosY;p.z=p.z+cosY;p.z=p.z+cosY;
				 break;
				 }

			

			
			//if  (press(DIK_DOWN)||press(DIK_NUMPAD2))
			case VK_DOWN:
				{p.x=p.x+senY;p.x=p.x+senY;p.x=p.x+senY;p.x=p.x+senY;
				 p.z=p.z-cosY;p.z=p.z-cosY;p.z=p.z-cosY;p.z=p.z-cosY;
				 break;
				 }

            //if ( (press(DIK_LEFT)||press(DIK_NUMPAD4))&&
			//	 !(press(DIK_LALT)||press(DIK_RALT)) )
			 case VK_LEFT:
				{p.indiceSpinY+=12;
				if(p.indiceSpinY>=quadrante4) p.indiceSpinY=(short)(p.indiceSpinY-quadrante4);
				break;
				}

			
			//if ( (press(DIK_RIGHT)||press(DIK_NUMPAD6))&&
			//	 !(press(DIK_LALT)||press(DIK_RALT)) )
			 case VK_RIGHT:
				{p.indiceSpinY-=12;
				if(p.indiceSpinY<0) p.indiceSpinY=(short)(quadrante4+p.indiceSpinY);
				break;
				}

			
			


        }*/
    }
}


void CALLBACK MouseProc( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext )
{
//    g_bRightMouseDown = bRightButtonDown;
//veja:	BOOL DXUTIsMouseButtonDown(BYTE vButton);

}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:        DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:     g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
    }
}

/*
bool leiaTextur(IDirect3DDevice9* pd3dDevice,WCHAR *texturePath)
	{
	if( FAILED(D3DXCreateTextureFromFileEx(pd3dDevice,
							texturePath,
							D3DX_DEFAULT,
                            D3DX_DEFAULT,
                            1,
                            0,
                            D3DFMT_UNKNOWN,
                            D3DPOOL_DEFAULT,
                            D3DX_FILTER_LINEAR,
                            D3DX_FILTER_LINEAR,
							0xffff0000,//ColorKey// vermelho é a colorkey 0xffff0000
                            NULL,
                            NULL,
                            g_pTexture ) ) )

                            

        return false;
	else return true;

	}*/



//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
	//i g_DialogResourceManager.OnLostDevice();
	g_DialogResourceManager.OnD3D9LostDevice();
    //i g_SettingsDlg.OnLostDevice();
	g_SettingsDlg.OnD3D9LostDevice();

    if( g_pFont )
        g_pFont->OnLostDevice();
   // if( g_pEffect )
    //    g_pEffect->OnLostDevice();
    
	SAFE_RELEASE( g_pIB );
    SAFE_RELEASE( g_pVB );
	SAFE_RELEASE( g_pTextSprite );
	
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
	//g_DialogResourceManager.OnDestroyDevice();
    g_DialogResourceManager.OnD3D9DestroyDevice();
	//g_SettingsDlg.OnDestroyDevice();
	g_SettingsDlg.OnD3D9DestroyDevice();

    //SAFE_RELEASE( g_pEffect );
    SAFE_RELEASE( g_pFont );
	SAFE_RELEASE(pTexture);
//e	SAFE_RELEASE( g_pVertexShader );
//u    SAFE_RELEASE( g_pConstantTable );
    SAFE_RELEASE( g_pVertexDeclaration );
}



