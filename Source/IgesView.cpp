// IgesView.cpp : implementation of the CIgesView class
//

#include "stdafx.h"
#include "Iges.h"

#include "IgesDoc.h"
#include "IgesView.h"
#include "SamplingDlg.h"
#include "UnsupDlg.h"
#include "XFormatNumber.h"		//To display comma separated numbers
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CIgesView

IMPLEMENT_DYNCREATE(CIgesView, CView)

BEGIN_MESSAGE_MAP(CIgesView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_OPEN, &CIgesView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, &CIgesView::OnFileSaveAs)
	ON_COMMAND(ID_MESH_SAMPLING, &CIgesView::OnMeshSampling)
	ON_COMMAND(ID_VIEW_POLY, &CIgesView::OnViewPoly)
	ON_COMMAND(ID_VIEW_FILL, &CIgesView::OnViewFill)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILL, &CIgesView::OnUpdateViewFill)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POLY, &CIgesView::OnUpdateViewPoly)
	ON_COMMAND(ID_MESH_UNSUPPORTED, &CIgesView::OnMeshUnsupported)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_MESH_UNSUPPORTED, &CIgesView::OnUpdateMeshUnsupported)
	ON_COMMAND(ID_VIEW_OUTLINE, &CIgesView::OnViewOutline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTLINE, &CIgesView::OnUpdateViewOutline)
	ON_COMMAND(ID_MESH_COLOR, &CIgesView::OnMeshColor)
	ON_COMMAND(ID_MESH_ENTITIESINTHISMODEL, &CIgesView::OnMeshEntitiesinthismodel)
	ON_COMMAND(ID_VIEW_USEANTIALIASING, &CIgesView::OnViewUseantialiasing)
	ON_UPDATE_COMMAND_UI(ID_VIEW_USEANTIALIASING, &CIgesView::OnUpdateViewUseantialiasing)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CIgesView::OnUpdateFileSaveAs)
	ON_COMMAND(ID_VIEW_USETRANSPARENCY, &CIgesView::OnViewUsetransparency)
	ON_UPDATE_COMMAND_UI(ID_VIEW_USETRANSPARENCY, &CIgesView::OnUpdateViewUsetransparency)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_MESH_NORMALCORRECTION, &CIgesView::OnMeshNormalcorrection)
	ON_UPDATE_COMMAND_UI(ID_MESH_NORMALCORRECTION, &CIgesView::OnUpdateMeshNormalcorrection)
END_MESSAGE_MAP()

// CIgesView construction/destruction

CIgesView::CIgesView()
{
	m_MouseDownPoint=CPoint(0,0);
	color= Vector4<float>(0.75f, 0.75f, 0.75f, 1.0f);	//Initial color
	normalCorrectionMode=FALSE;
}


CIgesView::~CIgesView()
{
}

BOOL CIgesView::PreCreateWindow(CREATESTRUCT& cs)
{
	//An OpenGL Window must be created with the following flags
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

// CIgesView drawing

void CIgesView::OnDraw(CDC* /*pDC*/)
{
	CIgesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Clear out the color & depth buffers
    ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	RenderScene(GL_RENDER);
	
	// Tell OpenGL to flush its pipeline
    ::glFinish();
	 // Now Swap the buffers
    ::SwapBuffers( m_pDC->GetSafeHdc() );

}


// CIgesView diagnostics

#ifdef _DEBUG
void CIgesView::AssertValid() const
{
	CView::AssertValid();
}

void CIgesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIgesDoc* CIgesView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIgesDoc)));
	return (CIgesDoc*)m_pDocument;
}
#endif //_DEBUG


// CIgesView message handlers

int CIgesView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Initialize OpenGL Here
    InitializeOpenGL();

	return 0;
}

void CIgesView::OnDestroy()
{
	CView::OnDestroy();

	//Make the RC non-current
    if(::wglMakeCurrent (0,0) == FALSE)
    {
        MessageBox("Could not make RC non-current");
    }
	//Delete the rendering context
    if(::wglDeleteContext (m_hRC)==FALSE)
    {
        MessageBox("Could not delete RC");
    }
	//Delete the DC
    if(m_pDC)
    {
        delete m_pDC;
    }
    //Set it to NULL
    m_pDC = NULL;
}

void CIgesView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	GLdouble aspect_ratio; // width/height ratio

	if ( 0 >= cx || 0 >= cy )
    {
        return;
    }

	// select the full client area
    ::glViewport(0, 0, cx, cy);

    // compute the aspect ratio
    // this will keep all dimension scales equal
    aspect_ratio = (GLdouble)cx/(GLdouble)cy;

    // select the projection matrix and clear it
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();

    // select the viewing volume
    ::gluPerspective(45.0f, aspect_ratio, 3.0f, 2000.0f);
	
	// switch back to the modelview matrix and clear it
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();
}

BOOL CIgesView::OnEraseBkgnd(CDC* pDC)
{
	//comment out the original call
	//return CView::OnEraseBkgnd(pDC);
	//Tell Windows not to erase the background
    return TRUE;

}

void CIgesView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MouseDownPoint=point;
    SetCapture();

	CView::OnLButtonDown(nFlags, point);
}

void CIgesView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseDownPoint=CPoint(0,0);
    ReleaseCapture();

	CView::OnLButtonUp(nFlags, point);
}

void CIgesView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_MouseDownPoint=point;
    SetCapture();

	CView::OnRButtonDown(nFlags, point);
}

void CIgesView::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_MouseDownPoint=CPoint(0,0);
    ReleaseCapture();

	CView::OnRButtonUp(nFlags, point);
}

/////////////////////////////////////////////////////
// MOUSE MOVE FUNCTIONS
// To Rotate:	Drag mouse with Left Button pressed
// To Zoom:		Drag mouse with Right Button pressed
// To Move:		Drag mouse with Left+Right Buttons pressed
void CIgesView::OnMouseMove(UINT nFlags, CPoint point)
{
	CIgesDoc *pDoc = GetDocument();
	if (GetCapture()==this && (nFlags & MK_LBUTTON) && (nFlags & MK_RBUTTON))
	{
		//Change translation values
		pDoc->m_xPos += (point.x - m_MouseDownPoint.x) * 0.15f;
		pDoc->m_yPos -= (point.y - m_MouseDownPoint.y) * 0.15f;
		//Redraw the view
		InvalidateRect(NULL,FALSE);
		//Set the mouse point
		m_MouseDownPoint=point;
	}

	else if (GetCapture()==this && (nFlags & MK_LBUTTON) && !(nFlags & MK_RBUTTON))
	{
		//Increment the object rotation angles
		pDoc->m_xAngle+=(point.y-m_MouseDownPoint.y)/3.6;
		pDoc->m_yAngle+=(point.x-m_MouseDownPoint.x)/3.6;
		//Redraw the view
		InvalidateRect(NULL,FALSE);
		//Set the mouse point
		m_MouseDownPoint=point;
	}
	else if(GetCapture()==this && (nFlags & MK_RBUTTON) && !(nFlags & MK_LBUTTON))
	{
		//Change zoom value
		pDoc->m_Zoom += point.y - m_MouseDownPoint.y;
		if(pDoc->m_Zoom>=pDoc->vMax.length()*2.5) pDoc->m_Zoom=pDoc->vMax.length()*2.5;
		//Redraw the view
		InvalidateRect(NULL,FALSE);
		//Set the mouse point
		m_MouseDownPoint=point;
	}
	
	CView::OnMouseMove(nFlags, point);
}

// Mouse function for double clicking
// Used to mark surfaces as inverted. Uses OpenGL's picking functions
void CIgesView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(normalCorrectionMode)
	{
		CIgesDoc *pDoc = GetDocument();
		GLuint nameBuffer[64];
		GLint hits;
		
		//Buffer to store surfaces IDs
		glSelectBuffer(64,nameBuffer);
		
		//Get modelview, projection matrices and viewport vector
		GLdouble modelMatrix[16];
		glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
		GLdouble projMatrix[16];
		glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT,viewport);
				
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glRenderMode(GL_SELECT);	//Go to selection mode
		
		gluPickMatrix( (GLdouble)point.x, (GLdouble)(viewport[3] - point.y), 2, 2, viewport);
		glMultMatrixd(projMatrix);
		RenderScene(GL_SELECT);
		
		//Return number of surfaces hit
		hits=glRenderMode(GL_RENDER);
		processHits(hits, nameBuffer);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		RenderScene(GL_RENDER);

		CView::OnLButtonDblClk(nFlags, point);

		InvalidateRect(NULL);
	}
}

//Process hits when double clicking
void CIgesView::processHits (GLint hits, GLuint* buffer)
{
	CIgesDoc *pDoc = GetDocument();
  	int i;
	int choose, depth, index;
	
	
	if (hits > 0)								
	{
		choose = buffer[3];					// Make selection the first object
		depth = buffer[1];					// Store how far away it is


		for (i = 1; i < hits; i++)		//Compare to other objects. If it's closer, replace
		{
			
			if (buffer[i*4+1] < GLuint(depth))
			{
				choose = buffer[i*4+3];			
				depth = buffer[i*4+1];			
			}       
		}
		index=choose-pDoc->DLnumber[0];
		if(pDoc->winding[index]==0) pDoc->winding[index]=1;
		else pDoc->winding[index]=0;
		TRACE("hits:%d chosen name:%d index:%d\n", hits, choose,index);													
				
	}
  
}

//Show file open dialog
void CIgesView::OnFileOpen()
{
	CIgesDoc *pDoc = GetDocument();

	char strFilter[] = { "IGS Files (*.igs)|*.igs||" };

	CFileDialog FileDlg(TRUE, ".igs", NULL, 0, strFilter);
		
	if( FileDlg.DoModal() == IDOK )
	{
		//Call doc opening operations
		pDoc->OnOpenDocument(FileDlg.GetPathName());
	}
	else
		return;
}
//Show file save dialog and do saving operations
void CIgesView::OnFileSaveAs()
{
	CIgesDoc *pDoc = GetDocument();

	CFile flStl;

	char strFilter[] = { "STL Files (*.stl)|*.stl||" };

	const CString& pt=pDoc->GetTitle();
	int ptLoc=pt.ReverseFind('.');
	CString filename=pt.Left(ptLoc);	//use same filename as the Iges as default
	
	CFileDialog FileDlg(FALSE, ".stl", filename, 0, strFilter);
	CFileException ex;

	if( FileDlg.DoModal() == IDOK )
	{
		CWaitCursor wait;
		if (!flStl.Open(FileDlg.GetFileName(), CFile::modeCreate | CFile::modeWrite, &ex))
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			AfxMessageBox(szError);
			return;
		}

		CArchive ar(&flStl, CArchive::store);
		
		CString tmp=FileDlg.GetFileTitle();		//Temp variable for writing each line
		tmp.MakeUpper();
		ar.WriteString(("solid " + tmp + "\n"));
		
		int num=0;		//count number of triangles
		UINT ind=0;		//surface index
		int barSz=0;	//Progress bar size
		
		//Calculate progress bar size=sum of the size of each surface
		for(;ind<pDoc->DLsize;ind++)
			barSz+=(int)pDoc->stl_data[ind].size();
		ind=0;			//Reset surface index to 0

		CProgressBar Bar("Saving",40,barSz,TRUE);
		Bar.SetStep(1);

		while(ind<pDoc->DLsize)		//go through each surface separately
		{
			if(pDoc->winding[ind]==0)
			{
				vector<Vector3<float>>::iterator it=pDoc->stl_data[ind].begin();
				Vector3<float> n;
				float length;
				
				while(it!=pDoc->stl_data[ind].end())
				{			
					n=(*(it+2)-*it).crossProduct((*(it+1)-*it));
					length=n.length();
					//if(length==0.) length=1.;
					n/=length;	//calculate normal
					tmp.Format("  facet normal %f %f %f\n", n.x, n.y, n.z);
					ar.WriteString(tmp);
					ar.WriteString("    outer loop\n");
					
					tmp.Format("      vertex %f %f %f\n", it->x, it->y, it->z);
					ar.WriteString(tmp);
					Bar.StepIt();
					it++;
					tmp.Format("      vertex %f %f %f\n", it->x, it->y, it->z);
					ar.WriteString(tmp);
					Bar.StepIt();
					it++;
					tmp.Format("      vertex %f %f %f\n", it->x, it->y, it->z);
					ar.WriteString(tmp);
					Bar.StepIt();
					it++;

					ar.WriteString("    endloop\n");
					ar.WriteString("  endfacet\n");

					num++;
				}
			}
			if(pDoc->winding[ind]==1)
			{
				vector<Vector3<float>>::reverse_iterator it=pDoc->stl_data[ind].rbegin();
				Vector3<float> n;
				float length;
				
				while(it!=pDoc->stl_data[ind].rend())
				{			
					n=(*(it+2)-*it).crossProduct((*(it+1)-*it));
					length=n.length();
					//if(length==0.) length=1.;
					n/=length;	//calculate normal
					tmp.Format("  facet normal %f %f %f\n", n.x, n.y, n.z);
					ar.WriteString(tmp);
					ar.WriteString("    outer loop\n");
					
					tmp.Format("      vertex %f %f %f\n", it->x, it->y, it->z);
					ar.WriteString(tmp);
					Bar.StepIt();
					it++;
					tmp.Format("      vertex %f %f %f\n", it->x, it->y, it->z);
					ar.WriteString(tmp);
					Bar.StepIt();
					it++;
					tmp.Format("      vertex %f %f %f\n", it->x, it->y, it->z);
					ar.WriteString(tmp);
					Bar.StepIt();
					it++;

					ar.WriteString("    endloop\n");
					ar.WriteString("  endfacet\n");

					num++;
				}
			}

			ind++;
		}
		tmp=FileDlg.GetFileTitle();
		tmp.MakeUpper();
		ar.WriteString(("endsolid " + tmp));

		Bar.DestroyWindow();
		ar.Close();

		CString msg;
		CString number;
		number.Format("%d",num);
		CString conv=XFormatNumber(number,0);

		msg.Format("STL saved. Number of triangles: " + conv);
		AfxMessageBox(msg,MB_OK | MB_ICONINFORMATION);
		
	}
	else
		return;

	flStl.Close();
}

BOOL CIgesView::InitializeOpenGL()
{
	CIgesDoc *pDoc = GetDocument();
    //Get a DC for the Client Area
    m_pDC = new CClientDC(this);

    //Failure to Get DC
    if(m_pDC == NULL)
    {
        MessageBox("Error Obtaining DC");
        return FALSE;
    }

	//Failure to set the pixel format
    if(!SetupPixelFormat())
    {
        return FALSE;
    }

	//Create Rendering Context
    m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc());

    //Failure to Create Rendering Context
    if(m_hRC == 0)
    {
        MessageBox("Error Creating RC");
        return FALSE;
    }

	//Make the RC Current
    if(::wglMakeCurrent (m_pDC->GetSafeHdc(), m_hRC)==FALSE)
    {
        MessageBox("Error making RC Current");
        return FALSE;
    }

	//Specify dark blue as the clear color
	::glClearColor(0.4f,0.8f,1.0f,0.0f);

    //Specify the back of the buffer as clear depth
    ::glClearDepth(1.0f);

    //Enable Depth Testing
    ::glEnable(GL_DEPTH_TEST);
	
	::glEnable(GL_BLEND);
	::glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	::glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

	::glShadeModel(GL_SMOOTH);
	::glEnable(GL_NORMALIZE);
	::glEnable(GL_AUTO_NORMAL);

	GLfloat ambientLight0[] = { 0.32f, 0.32f, 0.32f, 1.0f };
	GLfloat diffuseLight0[] = { 0.65f, 0.65f, 0.65f, 1.0f };
	GLfloat specular0[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPos0[] = { 0.0f, 200.0f, 0.0f, 0.0f };

	::glEnable(GL_LIGHTING);

	// Set light model to use ambient light specified by ambientLight[]
	::glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight0);

	// Set up and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight0);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight0);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular0);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos0);
	glEnable(GL_LIGHT0);

	// Set up spotlight
	GLfloat ambientLight1[] = { 0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat diffuseLight1[] = { 0.30f, 0.32f, 0.31f, 1.0f };
	GLfloat specular1[] = { 0.1f, 0.1f, 0.1f, 0.1f};
	GLfloat lightPos1[] = { 0.0f, 0.0f, 200.0f, 0.0f };
	GLfloat spotDir1[] = { 0.0f, 0.0f, 0.0f };

	glLightfv(GL_LIGHT1,GL_AMBIENT,ambientLight1);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuseLight1);
	glLightfv(GL_LIGHT1,GL_SPECULAR,specular1);
	glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);
	//glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spotDir1);
	//glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,90.0f);
	glEnable(GL_LIGHT1);
	

	//Define callback functions
	gluNurbsCallback(pDoc->nurb, GLU_NURBS_BEGIN_DATA, (void (CALLBACK *) ())&beginCallback);
	gluNurbsCallback(pDoc->nurb, GLU_NURBS_VERTEX_DATA, (void (CALLBACK *) ())&vertexCallback);
	gluNurbsCallback(pDoc->nurb, GLU_NURBS_END_DATA, (void (CALLBACK *) ())&endCallback);
	gluNurbsCallback(pDoc->nurb, GLU_NURBS_ERROR, (void (CALLBACK *) ())&errorCallback);
	gluNurbsCallback(pDoc->nurb, GLU_NURBS_NORMAL_DATA, (void (CALLBACK *) ())&normalCallback);
	
	//Enable line and point antialiasing
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_POINT_SMOOTH);
	glPointSize(2.0);
	
	return TRUE;
}

BOOL CIgesView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd
		1,                                // version number
		PFD_DRAW_TO_WINDOW |			  // support window 
	    PFD_SUPPORT_OPENGL |			  // support OpenGL 
		PFD_DOUBLEBUFFER,                 // double buffered
		PFD_TYPE_RGBA,                    // RGBA type
		24,                               // 24-bit color depth
		0, 0, 0, 0, 0, 0,                 // color bits ignored
        0,                                // 8-bit alpha buffer
        0,                                // shift bit ignored
        0,                                // no accumulation buffer
        0, 0, 0, 0,                       // accumulation bits ignored
        24,                               // 24-bit z-buffer
        0,                                // no stencil buffer
        0,                                // no auxiliary buffer
        PFD_MAIN_PLANE,                   // main layer
        0,                                // reserved
        0, 0, 0                           // layer masks ignored
	};
	
	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

    if ( m_nPixelFormat == 0 )
    {
        return FALSE;
    }

	if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
    {
        return FALSE;
    }

	return TRUE;
}

void CIgesView::RenderScene(GLenum mode)
{
	CIgesDoc *pDoc = GetDocument();
	
	GLfloat specref[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	Vector4<GLfloat> invColor(1.-color.x,1.-color.y,1.-color.z,1.);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,specref);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,70);
	Vector4<GLfloat> black(0.,0.,0.,1.);
	
	//GLfloat lightPos0[] = { 0.0f, 0.0f, -100.0f, 1.0f };
		
	glMatrixMode(GL_MODELVIEW);
	//Light Position
	//glLoadIdentity();
	//glTranslatef(0.,0.,pDoc->vMax.length()*2.5+pDoc->m_Zoom);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos0);

	//Mouse rotation and translation
	glLoadIdentity();
	glTranslatef(pDoc->m_xPos,pDoc->m_yPos,-pDoc->vMax.length()*2.5+pDoc->m_Zoom);
	glRotatef(pDoc->m_xAngle+30.0, 1.0f,0.0f,0.0f);
	glRotatef(pDoc->m_yAngle-45.0, 0.0f,1.0f,0.0f);
	
	if(mode==GL_SELECT)
	{
		glInitNames();   //Name stack for picking
		glPushName(0);
	}
	
	//Call display list
	unsigned int index=0;
	//vector<entity*>::iterator iter=pDoc->mIgesReadObj.trimmedSurf144.begin();
	//while(iter!=pDoc->mIgesReadObj.trimmedSurf144.end())
	
	//Normal display mode
	if(!normalCorrectionMode)
	{
		if(pDoc->viewPoly==TRUE) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if(pDoc->viewFill==TRUE) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(pDoc->viewPoints==TRUE) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		while(index<pDoc->DLsize)
		{
			if(pDoc->winding[index]==0)	
				::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);										
			
			if(pDoc->winding[index]==1)	//If it was corrected, illuminate back
										//(only do a real change when saving)
				::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);	
							
			glCallList(pDoc->DLnumber[index]);
			index++;
		}
	}
	
	//Correction mode
	else if(normalCorrectionMode)
	while(index<pDoc->DLsize)
	{
		//(*iter)->endDraw();
		
		if(mode==GL_SELECT)
		{
			glLoadName(pDoc->DLnumber[index]);
			TRACE("surf name:%d winding:%d\n",pDoc->DLnumber[index],pDoc->winding[index]);
		}
		
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(pDoc->winding[index]==0)	//Back is the real back
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
			glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, invColor);

			//Draw filled polygons
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			glCallList(pDoc->DLnumber[index]);
			glDisable(GL_POLYGON_OFFSET_FILL);
			
			//Draw back polygons with wireframe on top to distinguish
			glDisable(GL_LIGHTING);
			//glDisable(GL_BLEND);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glColor4fv(black);
			
			glEnable(GL_POLYGON_OFFSET_LINE);
			glPolygonOffset(-1.0f,-1.0f);
			glPolygonMode(GL_BACK,GL_LINE);
			glCallList(pDoc->DLnumber[index]);
			glDisable(GL_POLYGON_OFFSET_LINE);
			
			glPolygonMode(GL_BACK,GL_FILL);
			glColor4fv(color);
			glDisable(GL_CULL_FACE);
		}
		if(pDoc->winding[index]==1)	//Back turns to front
		{
			glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, color);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, invColor);

			//Draw filled polygons
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			glCallList(pDoc->DLnumber[index]);
			glDisable(GL_POLYGON_OFFSET_FILL);
			
			//Draw back polygons with wireframe on top to distinguish
			glDisable(GL_LIGHTING);
			//glDisable(GL_BLEND);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glColor4fv(black);
			
			glEnable(GL_POLYGON_OFFSET_LINE);
			glPolygonOffset(-1.0f,-1.0f);
			glPolygonMode(GL_FRONT,GL_LINE);
			glCallList(pDoc->DLnumber[index]);
			glDisable(GL_POLYGON_OFFSET_LINE);
			
			glPolygonMode(GL_FRONT,GL_FILL);
			glColor4fv(color);
			glDisable(GL_CULL_FACE);
		}
	
		//glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		index++;
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		//iter++;
	}
	

	/////// DRAW FROM STL TRIANGULATION DATA
	/////// USED FOR TESTING
	/*if(!(pDoc->stl_data.empty()))
	{
		GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
		for(vector<Vector3<float>>::iterator it=pDoc->stl_data.begin();it<pDoc->stl_data.end()-2;)
		{
			Vector3<float> n;
			n=(*(it+1)-*it).crossProduct((*(it+2)-*it));
			glBegin(GL_TRIANGLES);
			glNormal3fv(n);
			glVertex3fv(*it++);
			glVertex3fv(*it++);
			glVertex3fv(*it++);
			glEnd();
		}
		
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2.0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
		for(vector<Vector3<float>>::iterator it=pDoc->stl_data.begin();it<pDoc->stl_data.end()-2;)
		{
			Vector3<float> n;
			n=(*(it+1)-*it).crossProduct((*(it+2)-*it));
			glBegin(GL_TRIANGLES);
			glNormal3fv(n);
			glVertex3fv(*it++);
			glVertex3fv(*it++);
			glVertex3fv(*it++);
			glEnd();
		}
	}*/
}

//Call mesh sampling menu
void CIgesView::OnMeshSampling()
{
	CIgesDoc *pDoc = GetDocument();
	CSamplingDlg Dlg;
	entArray_it entIt=pDoc->mIgesReadObj.ents.find(0);

	Dlg.mSampling = pDoc->sampleTol;
	Dlg.mMethod = pDoc->method;
	Dlg.mStep = pDoc->step;
	Dlg.mTrimTol= pDoc->trimTol;
	if(Dlg.DoModal()==IDOK)	//update data
	{
		pDoc->sampleTol=Dlg.mSampling;
		pDoc->method=Dlg.mMethod;
		pDoc->step=Dlg.mStep;
		pDoc->trimTol=Dlg.mTrimTol;
		entIt->second->setData(&pDoc->trimTol,1);
		gluNurbsProperty( pDoc->nurb, GLU_PARAMETRIC_TOLERANCE, pDoc->sampleTol );
		gluNurbsProperty( pDoc->nurb, GLU_SAMPLING_TOLERANCE, pDoc->sampleTol );
		gluNurbsProperty( pDoc->nurb, GLU_U_STEP, pDoc->step );
		gluNurbsProperty( pDoc->nurb, GLU_V_STEP, pDoc->step );
		if(pDoc->method==0)
		{
			BOOL m=TRUE;
			entIt->second->setData(&m,0);
		}
		else if(pDoc->method!=0)
		{
			BOOL m=FALSE;
			entIt->second->setData(&m,0);
		}
		if(pDoc->method==1) gluNurbsProperty( pDoc->nurb, GLU_SAMPLING_METHOD, GLU_OBJECT_PARAMETRIC_ERROR );
		if(pDoc->method==2) gluNurbsProperty( pDoc->nurb, GLU_SAMPLING_METHOD, GLU_OBJECT_PATH_LENGTH );
		if(pDoc->method==3)	gluNurbsProperty( pDoc->nurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE );
		
		pDoc->Process();
	}
	
}

void CIgesView::OnViewPoly()
{
	CIgesDoc *pDoc = GetDocument();
	if(pDoc->viewPoly == FALSE)
	{
		pDoc->viewPoly=TRUE;
		pDoc->viewFill=FALSE;
		pDoc->viewPoints=FALSE;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		InvalidateRect(NULL);
	}
}

void CIgesView::OnViewFill()
{
	CIgesDoc *pDoc = GetDocument();
	if(pDoc->viewFill == FALSE)
	{
		pDoc->viewPoly=FALSE;
		pDoc->viewFill=TRUE;
		pDoc->viewPoints=FALSE;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		InvalidateRect(NULL);
	}
}

void CIgesView::OnViewOutline()
{
	CIgesDoc *pDoc = GetDocument();
	if(pDoc->viewPoints == FALSE)
	{
		pDoc->viewPoly=FALSE;
		pDoc->viewFill=FALSE;
		pDoc->viewPoints=TRUE;
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		InvalidateRect(NULL);
	}
}

void CIgesView::OnUpdateViewOutline(CCmdUI *pCmdUI)
{
	CIgesDoc *pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->viewPoints==TRUE);
}

void CIgesView::OnUpdateViewFill(CCmdUI *pCmdUI)
{
	CIgesDoc *pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->viewFill==TRUE);
}

void CIgesView::OnUpdateViewPoly(CCmdUI *pCmdUI)
{
	CIgesDoc *pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->viewPoly==TRUE);
}

//Open unsupported list dialog
void CIgesView::OnMeshUnsupported()
{
	CIgesDoc *pDoc = GetDocument();
	CUnsupDlg Dlg;
	CString tmp;
	
	Dlg.unsup=TRUE;	//Selects the unsupported dialog
	map<int,int>::iterator it =	pDoc->mIgesReadObj.unsupported.begin();
	while(it!=pDoc->mIgesReadObj.unsupported.end())
	{
		tmp.Format("%d", (*it).first);
		Dlg.mCstrings.Add(tmp);
		it++;
	}
	Dlg.DoModal();
}



void CIgesView::OnUpdateMeshUnsupported(CCmdUI *pCmdUI)
{
	CIgesDoc *pDoc = GetDocument();
	pCmdUI->Enable(!(pDoc->mIgesReadObj.unsupported.empty()));
}

void CIgesView::OnMeshNormalcorrection()
{
	normalCorrectionMode=!normalCorrectionMode;
	InvalidateRect(NULL);
}

void CIgesView::OnUpdateMeshNormalcorrection(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(normalCorrectionMode==TRUE);
}

//Open list of entities in this model dialog
void CIgesView::OnMeshEntitiesinthismodel()
{
	CIgesDoc *pDoc = GetDocument();
	CUnsupDlg Dlg;
	CString tmp;
	
	Dlg.unsup=FALSE;	//Selects the supported dialog
	map<int,int>::iterator it =	pDoc->mIgesReadObj.count.begin();
	while(it!=pDoc->mIgesReadObj.count.end())
	{
		tmp.Format("%d", (*it).first);
		Dlg.mCstrings.Add(tmp);
		it++;
	}
	Dlg.DoModal();
	
}

//Open mesh color selection dialog
void CIgesView::OnMeshColor()
{
	CColorDialog Dlg;
	Dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	Dlg.m_cc.rgbResult = RGB((int)(color.r*255.0),(int)(color.g*255.0),(int)(color.b*255.0));
	
	if(Dlg.DoModal()==IDOK)
	{
		COLORREF dlgColor = Dlg.GetColor();
		color.r = (float)(GetRValue(dlgColor)/255.0);
		color.g = (float)(GetGValue(dlgColor)/255.0);
		color.b = (float)(GetBValue(dlgColor)/255.0);
		InvalidateRect(NULL);
	}

}
//////////////////////////////////////////////////////////////
//Removed from menu. Conflicts with drawing lines on solid
void CIgesView::OnViewUseantialiasing()
{
	if(glIsEnabled(GL_LINE_SMOOTH))
	{
		::glDisable(GL_POINT_SMOOTH);
		::glDisable (GL_LINE_SMOOTH);
	}
	else
	{
		::glEnable(GL_LINE_SMOOTH);
		::glEnable(GL_POINT_SMOOTH);
	}
	InvalidateRect(NULL);
}

void CIgesView::OnUpdateViewUseantialiasing(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(glIsEnabled(GL_LINE_SMOOTH));
}

void CIgesView::OnViewUsetransparency()
{
	if(color.a == 1.0f) color.a=0.4f;  //transparency
	else color.a=1.0f;
	
	InvalidateRect(NULL);
}

void CIgesView::OnUpdateViewUsetransparency(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck((color.a==0.4f));
}
//////////////////////////////////////////////////////////////////

void CIgesView::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	CIgesDoc *pDoc = GetDocument();
	pCmdUI->Enable(!(pDoc->cb_data.empty()));
}

///////// NURBS CALLBACK FUNCTIONS ///////////

//Begin callback function
// which: type of polygon (6 triangle fan, 8 quad strip)
// userData: container to store the data from tesselation
void CALLBACK CIgesView::beginCallback(GLenum which, void *userData)
{
	CBDATA tmp;
	tmp.beg=TRUE;
	tmp.surface=0;
	tmp.type=which;
	tmp.v=Vector3<float>(0,0,0);
	((vector<CBDATA>*)userData)->push_back(tmp);
	glBegin(which);
}

void CALLBACK CIgesView::errorCallback(GLenum errorCode)
{
	const GLubyte *estring;
	estring = gluErrorString(errorCode);

	CString err;
	err.Format("GL NURBS Error: %s\n", estring);

	AfxMessageBox(err);
	exit(0);
}

void CALLBACK CIgesView::normalCallback( GLfloat *normal, void *userData )
{
	glNormal3fv(normal);
}

void CALLBACK CIgesView::endCallback(void *userData)
{
	glEnd();
}

void CALLBACK CIgesView::vertexCallback(GLfloat *vertex, void *userData)
{
	glVertex3fv(vertex);

	CBDATA tmp;
	tmp.surface=0;
	tmp.beg=FALSE;
	tmp.type=(*(((vector<CBDATA>*)userData)->end()-1)).type;
	tmp.v=Vector3<float>(vertex[0],vertex[1],vertex[2]);
	((vector<CBDATA>*)userData)->push_back(tmp);
}
//////////////////////////////////////
