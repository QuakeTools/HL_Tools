#include <experimental/filesystem>

#include "core/shared/Logging.h"
#include "core/shared/Utility.h"

#include "utility/PlatUtils.h"

#include "cvar/CVar.h"

#include "filesystem/IFileSystem.h"
#include "soundsystem/shared/ISoundSystem.h"

#include "engine/shared/renderer/IRendererLibrary.h"
#include "engine/shared/renderer/IRenderContext.h"
#include "engine/shared/renderer/studiomodel/IStudioModelRenderer.h"

#include "CBaseToolApp.h"

extern studiomdl::IStudioModelRenderer* g_pStudioMdlRenderer;
extern soundsystem::ISoundSystem* g_pSoundSystem;

//TODO: remove
extern renderer::IRenderContext* g_pRenderContext;

namespace tools
{
bool CBaseToolApp::StartupApp()
{
	//No log file provided, initialize to executable filename.
	if( m_szLogFilename.empty() )
	{
		bool bSuccess;
		auto szExePath = plat::GetExeFileName( &bSuccess );

		if( bSuccess )
		{
			std::experimental::filesystem::path path( szExePath );

			m_szLogFilename = path.stem().string();
		}

		if( m_szLogFilename.empty() )
		{
			Warning( "CBaseToolApp::StartupApp: Couldn't get executable name, defaulting log filename to \"tool\"\n" );
			m_szLogFilename = "tool";
		}
	}

	const std::string szLogFilename = m_szLogFilename + ".log";

	//Overwrite previous session log.
	logging().OpenLogFile( szLogFilename.c_str(), false );

	UTIL_InitRandom();

	return true;
}

bool CBaseToolApp::LoadAppLibraries()
{
	//TODO: needs to use platform agnostic library names - Solokiller
	if( !LoadLibraries( "LibCVar.dll", "LibFileSystem.dll", "LibSoundSystem.dll", "LibRenderer.dll" ) )
		return false;

	return true;
}

bool CBaseToolApp::Connect( const CreateInterfaceFn* pFactories, const size_t uiNumFactories )
{
	if( !LoadAndCheckInterfaces( pFactories, uiNumFactories, 
							IFace( ICVARSYSTEM_NAME, g_pCVar, "CVar System" ),
							IFace( IFILESYSTEM_NAME, m_pFileSystem, "File System" ),
							IFace( ISOUNDSYSTEM_NAME, m_pSoundSystem, "Sound System" ),
							IFace( IRENDERERLIBRARY_NAME, m_pRendererLib, "Render Library" ),
							IFace( IRENDERCONTEXT_NAME, g_pRenderContext, "Render Context" ),
							IFace( ISTUDIOMODELRENDERER_NAME, g_pStudioMdlRenderer, "StudioModel Renderer" ) ) )
	{
		return false;
	}

	g_pSoundSystem = m_pSoundSystem;

	if( !g_pCVar->Initialize() )
	{
		//TODO: use fatal error.
		Error( "Failed to initialize CVar system!\n" );
		return false;
	}

	cvar::ConnectCVars();

	if( !m_pFileSystem->Initialize() )
	{
		//TODO: use fatal error.
		Error( "Failed to initialize file system!\n" );
		return false;
	}

	if( !m_pRendererLib->Connect( pFactories, uiNumFactories ) )
	{
		//TODO: use fatal error.
		Error( "Failed to connect renderer!\n" );
		return false;
	}

	if( !g_pStudioMdlRenderer->Initialize() )
	{
		//TODO: use fatal error.
		Error( "Failed to initialize StudioModel renderer!\n" );
		return false;
	}

	if( !m_pSoundSystem->Connect( pFactories, uiNumFactories ) )
	{
		//TODO: use fatal error.
		Error( "Failed to connect sound system!\n" );
		return false;
	}

	if( !m_pSoundSystem->Initialize() )
	{
		//TODO: use fatal error.
		Error( "Failed to initialize sound system!\n" );
		return false;
	}

	return true;
}

void CBaseToolApp::ShutdownApp()
{
	if( m_pSoundSystem )
	{
		m_pSoundSystem->Shutdown();
		m_pSoundSystem = nullptr;
	}

	if( g_pStudioMdlRenderer )
	{
		g_pStudioMdlRenderer->Shutdown();
		g_pStudioMdlRenderer = nullptr;
	}

	if( g_pRenderContext )
	{
		g_pRenderContext = nullptr;
	}

	if( m_pRendererLib )
	{
		m_pRendererLib->Disconnect();
		m_pRendererLib = nullptr;
	}

	if( m_pFileSystem )
	{
		m_pFileSystem->Shutdown();
		m_pFileSystem = nullptr;
	}

	if( g_pCVar )
	{
		g_pCVar->Shutdown();
		g_pCVar = nullptr;
	}
}
}