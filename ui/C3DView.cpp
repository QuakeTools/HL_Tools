#include "studiomodel/StudioModel.h"

#include "C3DView.h"

wxBEGIN_EVENT_TABLE( C3DView, wxGLCanvas )
	EVT_PAINT( C3DView::Paint )
	EVT_MOUSE_EVENTS( C3DView::MouseEvents )
wxEND_EVENT_TABLE()

C3DView::C3DView( wxWindow* pParent, I3DViewListener* pListener )
	: wxGLCanvas( pParent, wxID_ANY, nullptr, wxDefaultPosition, wxSize( 600, 400 ) )
	, m_pListener( pListener )
{
	m_pContext = new wxGLContext( this );
}

C3DView::~C3DView()
{
	g_studioModel.FreeModel();
	delete m_pContext;
}

void C3DView::Paint( wxPaintEvent& event )
{
	SetCurrent( *m_pContext );

	//Can't use the DC to draw anything since OpenGL draws over it.
	wxPaintDC( this );

	glClearColor( 0.5, 0.5, 0.5, 1.0 );

	const wxSize size = GetClientSize();
	
	if( Options.useStencil )
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	else
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glViewport( 0, 0, size.GetX(), size.GetY() );

	//
	// show textures
	//

	if( Options.showTexture )
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		glOrtho( 0.0f, ( float ) size.GetX(), ( float ) size.GetY(), 0.0f, 1.0f, -1.0f );

		studiohdr_t *hdr = g_studioModel.getTextureHeader();
		if( hdr )
		{
			mstudiotexture_t *ptextures = ( mstudiotexture_t * ) ( ( byte * ) hdr + hdr->textureindex );
			float w = ( float ) ptextures[ Options.texture ].width * Options.textureScale;
			float h = ( float ) ptextures[ Options.texture ].height * Options.textureScale;

			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();

			glDisable( GL_CULL_FACE );

			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			float x = ( ( float ) size.GetX() - w ) / 2;
			float y = ( ( float ) size.GetY() - h ) / 2;

			glDisable( GL_TEXTURE_2D );
			glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
			glRectf( x - 2, y - 2, x + w + 2, y + h + 2 );

			glEnable( GL_TEXTURE_2D );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glBindTexture( GL_TEXTURE_2D, Options.texture ); //d_textureNames[0]);	//TODO: fix this ugly crap

			glBegin( GL_TRIANGLE_STRIP );

			glTexCoord2f( 0, 0 );
			glVertex2f( x, y );

			glTexCoord2f( 1, 0 );
			glVertex2f( x + w, y );

			glTexCoord2f( 0, 1 );
			glVertex2f( x, y + h );

			glTexCoord2f( 1, 1 );
			glVertex2f( x + w, y + h );

			glEnd();

			glPopMatrix();

			glClear( GL_DEPTH_BUFFER_BIT );
			glBindTexture( GL_TEXTURE_2D, 0 );
		}
		return;
	}

	//
	// draw background
	//

	if( Options.showBackground && m_BackgroundTexture != GL_INVALID_TEXTURE_ID && !Options.showTexture )
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		glOrtho( 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f );

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();

		glDisable( GL_CULL_FACE );
		glEnable( GL_TEXTURE_2D );

		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		glBindTexture( GL_TEXTURE_2D, m_BackgroundTexture );

		glBegin( GL_TRIANGLE_STRIP );

		glTexCoord2f( 0, 0 );
		glVertex2f( 0, 0 );

		glTexCoord2f( 1, 0 );
		glVertex2f( 1, 0 );

		glTexCoord2f( 0, 1 );
		glVertex2f( 0, 1 );

		glTexCoord2f( 1, 1 );
		glVertex2f( 1, 1 );

		glEnd();

		glPopMatrix();

		glClear( GL_DEPTH_BUFFER_BIT );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 65.0f, ( GLfloat ) size.GetX() / ( GLfloat ) size.GetY(), 1.0f, 4096.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	if( Options.useWeaponOrigin )
	{
		glTranslatef( -Options.weaponOrigin[ 0 ], -Options.weaponOrigin[ 1 ], -Options.weaponOrigin[ 2 ] );

		glRotatef( -90, 1.0f, 0.0f, 0.0f );
		glRotatef( 90, 0.0f, 0.0f, 1.0f );
	}
	else
	{
		glTranslatef( -Options.trans[ 0 ], -Options.trans[ 1 ], -Options.trans[ 2 ] );

		glRotatef( Options.rot[ 0 ], 1.0f, 0.0f, 0.0f );
		glRotatef( Options.rot[ 1 ], 0.0f, 0.0f, 1.0f );
	}

	// setup stencil buffer
	if( Options.useStencil )
	{
		/* Don't update color or depth. */
		glDisable( GL_DEPTH_TEST );
		glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

		/* Draw 1 into the stencil buffer. */
		glEnable( GL_STENCIL_TEST );
		glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
		glStencilFunc( GL_ALWAYS, 1, 0xffffffff );

		/* Now render floor; floor pixels just get their stencil set to 1. */
		DrawFloor();

		/* Re-enable update of color and depth. */
		glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
		glEnable( GL_DEPTH_TEST );

		/* Now, only render where stencil is set to 1. */
		glStencilFunc( GL_EQUAL, 1, 0xffffffff );  /* draw if ==1 */
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
	}

	g_vright[ 0 ] = g_vright[ 1 ] = Options.trans[ 2 ];

	Options.drawnPolys = 0;

	if( Options.mirror )
	{
		glPushMatrix();
		glScalef( 1, 1, -1 );
		glCullFace( GL_BACK );
		SetupRenderMode();
		Options.drawnPolys += g_studioModel.DrawModel();

		//Draw wireframe overlay
		//TODO: integrate this into DrawModel somehow.
		if( Options.wireframeOverlay )
		{
			SetupRenderMode( RenderMode::WIREFRAME );

			Options.drawnPolys += g_studioModel.DrawModel( true );
		}

		glPopMatrix();
	}

	if( Options.useStencil )
		glDisable( GL_STENCIL_TEST );

	SetupRenderMode();

	glCullFace( GL_FRONT );
	Options.drawnPolys += g_studioModel.DrawModel();

	//Draw wireframe overlay
	if( Options.wireframeOverlay )
	{
		SetupRenderMode( RenderMode::WIREFRAME );

		Options.drawnPolys += g_studioModel.DrawModel( true );
	}

	//
	// draw ground
	//

	if( Options.showGround )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_CULL_FACE );

		if( Options.useStencil )
			glFrontFace( GL_CW );
		else
			glDisable( GL_CULL_FACE );

		glEnable( GL_BLEND );
		if( m_GroundTexture == GL_INVALID_TEXTURE_ID )
		{
			glDisable( GL_TEXTURE_2D );
			glColor4f( Options.groundColor[ 0 ], Options.groundColor[ 1 ], Options.groundColor[ 2 ], 0.7f );
			glBindTexture( GL_TEXTURE_2D, 0 );
		}
		else
		{
			glEnable( GL_TEXTURE_2D );
			glColor4f( 1.0f, 1.0f, 1.0f, 0.6f );
			glBindTexture( GL_TEXTURE_2D, m_GroundTexture );
		}

		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		DrawFloor();

		glDisable( GL_BLEND );

		if( Options.useStencil )
		{
			glCullFace( GL_BACK );
			glColor4f( 0.1f, 0.1f, 0.1f, 1.0f );
			glBindTexture( GL_TEXTURE_2D, 0 );
			DrawFloor();

			glFrontFace( GL_CCW );
		}
		else
			glEnable( GL_CULL_FACE );
	}

	glPopMatrix();

	if( m_pListener )
		m_pListener->Draw3D( size );

	glFlush();
	SwapBuffers();
}

void C3DView::MouseEvents( wxMouseEvent& event )
{
	//Ignore input in weapon origin mode.
	//TODO: refactor
	if( Options.useWeaponOrigin || Options.showTexture )
	{
		event.Skip();
		return;
	}

	if( event.ButtonDown() )
	{
		m_flOldRotX = Options.rot[ 0 ];
		m_flOldRotY = Options.rot[ 1 ];
		VectorCopy( Options.trans, m_vecOldTrans );
		m_flOldX = event.GetX();
		m_flOldY = event.GetY();
		Options.pause = false;

		m_iButtonsDown |= event.GetButton();
	}
	else if( event.ButtonUp() )
	{
		m_iButtonsDown &= ~event.GetButton();
	}
	else if( event.Dragging() )
	{
		if( event.LeftIsDown() && m_iButtonsDown & wxMOUSE_BTN_LEFT )
		{
			if( event.GetModifiers() & wxMOD_SHIFT )
			{
				Options.trans[ 0 ] = m_vecOldTrans[ 0 ] - ( float ) ( event.GetX() - m_flOldX );
				Options.trans[ 1 ] = m_vecOldTrans[ 1 ] + ( float ) ( event.GetY() - m_flOldY );
			}
			else
			{
				Options.rot[ 0 ] = m_flOldRotX + ( float ) ( event.GetY() - m_flOldY );
				Options.rot[ 1 ] = m_flOldRotY + ( float ) ( event.GetX() - m_flOldX );
			}
		}
		else if( event.RightIsDown() && m_iButtonsDown & wxMOUSE_BTN_RIGHT )
		{
			Options.trans[ 2 ] = m_vecOldTrans[ 2 ] + ( float ) ( event.GetY() - m_flOldY );
		}

		Refresh();
	}
	else
	{
		event.Skip();
	}
}

void C3DView::PrepareForLoad()
{
	SetCurrent( *m_pContext );
}

void C3DView::UpdateView()
{
	const wxLongLong curr = wxGetUTCTimeMillis();

	if( Options.playSequence )
		g_studioModel.AdvanceFrame( ( ( curr - m_iPrevTime ).GetValue() / 1000.0 ) * Options.speedScale );

	m_iPrevTime = curr;

	if( !Options.pause )
		Refresh();
}

void C3DView::DrawFloor()
{
	glBegin( GL_TRIANGLE_STRIP );
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3f( -100.0f, 100.0f, 0.0f );

	glTexCoord2f( 0.0f, 1.0f );
	glVertex3f( -100.0f, -100.0f, 0.0f );

	glTexCoord2f( 1.0f, 0.0f );
	glVertex3f( 100.0f, 100.0f, 0.0f );

	glTexCoord2f( 1.0f, 1.0f );
	glVertex3f( 100.0f, -100.0f, 0.0f );

	glEnd();
}

void C3DView::SetupRenderMode( RenderMode renderMode )
{
	if( renderMode == RenderMode::INVALID )
		renderMode = Options.renderMode;

	if( renderMode == RenderMode::WIREFRAME )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_CULL_FACE );
		glEnable( GL_DEPTH_TEST );
	}
	else if( renderMode == RenderMode::FLAT_SHADED ||
			 renderMode == RenderMode::SMOOTH_SHADED )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDisable( GL_TEXTURE_2D );
		glEnable( GL_CULL_FACE );
		glEnable( GL_DEPTH_TEST );

		if( renderMode == RenderMode::FLAT_SHADED )
			glShadeModel( GL_FLAT );
		else
			glShadeModel( GL_SMOOTH );
	}
	else if( renderMode == RenderMode::TEXTURE_SHADED )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glEnable( GL_TEXTURE_2D );
		glEnable( GL_CULL_FACE );
		glEnable( GL_DEPTH_TEST );
		glShadeModel( GL_SMOOTH );
	}
}