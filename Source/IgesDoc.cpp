// IgesDoc.cpp : implementation of the CIgesDoc class
//

#include "stdafx.h"
#include "Iges.h"

#include "IgesDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIgesDoc

IMPLEMENT_DYNCREATE(CIgesDoc, CDocument)

BEGIN_MESSAGE_MAP(CIgesDoc, CDocument)
END_MESSAGE_MAP()


// CIgesDoc construction/destruction

CIgesDoc::CIgesDoc() : m_xAngle(0), m_yAngle(0), m_Zoom(0.0), m_xPos(0.0),m_yPos(0.0)
{
	nurb=gluNewNurbsRenderer();		//Initialize NURB object
	//gluNurbsProperty(nurb, GLU_NURBS_MODE, GLU_NURBS_RENDERER);
	gluNurbsProperty(nurb, GLU_NURBS_MODE, GLU_NURBS_TESSELLATOR);	//Set mode to tessellation
	//to retrieve vertex info
	mIgesReadObj.pNurb=nurb;		//Pass pointer to NURB to the IGES object

	gluNurbsProperty( nurb, GLU_DISPLAY_MODE, GLU_FILL );
	//set cb_data as the data structure for the callback functions
	gluNurbsCallbackData( nurb, &cb_data);		
	
	//Initial values
	sampleTol=10.0;
	step=10.0;
	viewFill=TRUE;
	viewPoly=FALSE;
	viewPoints=FALSE;
	method=0;
	trimTol=0.001f;
	DLbase=0;
	DLsize=0;
	gluNurbsProperty( nurb, GLU_PARAMETRIC_TOLERANCE, sampleTol );
	gluNurbsProperty( nurb, GLU_SAMPLING_TOLERANCE, sampleTol );
	gluNurbsProperty( nurb, GLU_U_STEP, step );
	gluNurbsProperty( nurb, GLU_V_STEP, step );
}

CIgesDoc::~CIgesDoc()
{
	gluDeleteNurbsRenderer(nurb);
	if(DLsize)	//If a display list exists
	{
		delete[] DLnumber;
		delete[] winding;
		delete[] stl_data;
	}
}

BOOL CIgesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//Reinitialize values
	mIgesReadObj.reInit(method,trimTol);
	cb_data.clear();
	for(UINT i=0;i<DLsize;i++)
		stl_data[i].clear();
	if(DLsize)
	{
		glDeleteLists(DLbase,DLsize);
		delete[] DLnumber;
		delete[] winding;
		delete[] stl_data;
	}
	DLbase=0;
	DLsize=0;
	return TRUE;
}




// CIgesDoc serialization

void CIgesDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CIgesDoc diagnostics

#ifdef _DEBUG
void CIgesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIgesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CIgesDoc commands

//Loads file and reads data into the IgesRead structure
BOOL CIgesDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	CFile flIges;
	CString readLine;

	CFileException ex;
	CWaitCursor wait;
		
	mIgesReadObj.reInit(method,trimTol);
	cb_data.clear();
	for(UINT i=0;i<DLsize;i++)
		stl_data[i].clear();

	if( flIges.Open(lpszPathName, CFile::modeRead) == FALSE )
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		AfxMessageBox(szError);
		return FALSE;
	}
	
	CArchive ar(&flIges, CArchive::load);
	SetTitle(flIges.GetFileTitle());
	
	
	CProgressBar Bar("Loading",40,flIges.GetLength(),TRUE);
	//Read from file, one line at a time
	while(ar.ReadString(readLine))
	{
		Bar.SetStep(readLine.GetLength());
		Bar.StepIt();
		string strReadLine(readLine);
		if(!mIgesReadObj.loadLine(strReadLine))
		{
			AfxMessageBox("Not a valid IGES file!");
			return FALSE;
		}
	}		
	ar.Close();
	
	m_Zoom=0.0;
	m_xPos=0.0;
	m_yPos=0.0;
	m_xAngle=0.0;
	m_yAngle=0.0;
	Bar.DestroyWindow();

	if(!mIgesReadObj.unsupported.empty())
		AfxMessageBox("There are unsupported types. Drawing may be incomplete.\nYou can find a list of these types in the Mesh menu");

	Process(FALSE);
	flIges.Close();

	return TRUE;
}

//Process the file after it was stored in the IgesRead structure
//Input: BOOL reProcess
//	FALSE when opening the file
//	TRUE when making changes (e.g. changing tesselation or tolerance)
void CIgesDoc::Process(BOOL reProcess)
{
	CWaitCursor wait;
	vector<entity*>::size_type igesObjSz=mIgesReadObj.trimmedSurf144.size();
	CProgressBar Bar("Processing",40,(int)igesObjSz,TRUE);
	Bar.SetStep(1);

	cb_data.clear();
	for(UINT i=0;i<DLsize;i++)
		stl_data[i].clear();
	vMax=Vector3<float>(0.0,0.0,0.0);
		
	//Go through all the trimmed surfaces and do its draw operation
	vector<entity*>::iterator iter=mIgesReadObj.trimmedSurf144.begin();
	
	//If the display list is not empty, erase the previous list
	if(DLsize)
	{
		glDeleteLists(DLbase,DLsize);
		delete[] DLnumber;
		if(!reProcess)
		{
			delete[] winding;
			delete[] stl_data;
		}
	}

	DLsize=(int)mIgesReadObj.trimmedSurf144.size();
	//Assign memory for all display lists
	DLnumber = new GLuint[DLsize];
	//Generate numOfSurfaces contiguous lists. glGenLists returns
	//the index of the first list
	DLbase = glGenLists(DLsize);
	if(!reProcess) winding = new GLuint[DLsize];

	int index=0;
	while(iter!=mIgesReadObj.trimmedSurf144.end())
	{
		Bar.StepIt();
		DLnumber[index]=DLbase+index;
		if(!reProcess) winding[index]=0;
		glNewList(DLnumber[index],GL_COMPILE);
			(*iter)->draw();
		glEndList();
		iter++;
		index++;

		if(iter!=mIgesReadObj.trimmedSurf144.end())
		{
			CBDATA tmp;
			tmp.surface=1;
			tmp.beg=TRUE;
			tmp.type=999;
			cb_data.push_back(tmp);
		}
		
	}

	
	////////////////////////////////////////
	//Separate callback data into triangles
	if(!reProcess) stl_data = new vector<Vector3<float>>[DLsize];
	int surfIndex=0;
	CBDATA tmp;		//add to end as a flag to know we finished processing
	tmp.beg=TRUE;
	tmp.type=0;
	tmp.surface=1;
	cb_data.push_back(tmp);

	vector<CBDATA>::iterator it=cb_data.begin();
	vector<CBDATA>::iterator iV0;
	
	Bar.SetText("Triangulating");
	Bar.SetPos(0);
	Bar.SetRange(0,(int)cb_data.size(),1);
	do
	{
		switch((*it).type)
		{
		case 6:				// GL_TRIANGLE_FAN CONVERTED TO TRIANGLES
		    Bar.StepIt();
			it++;			//One past the beginning
			iV0=it;			//save pivot vertex (see opengl superbible)
			FindMax(iV0->v);
			while((it+2)->beg!=TRUE)  //check that next type hasn't started
			{
				stl_data[surfIndex].push_back(iV0->v);
				stl_data[surfIndex].push_back((it+1)->v);
				stl_data[surfIndex].push_back((it+2)->v);
				FindMax((it+1)->v);
				FindMax((it+2)->v);
				Bar.StepIt();
				it++;
			}
			Bar.StepIt();
			Bar.StepIt();
			it+=2;
			break; 
		case 8:				//GL_QUAD_STRIP
			Bar.StepIt();
		    it++;
			while((it+2)->beg!=TRUE)
			{
				stl_data[surfIndex].push_back((it)->v);
				stl_data[surfIndex].push_back((it+1)->v);
				stl_data[surfIndex].push_back((it+2)->v);
				
				stl_data[surfIndex].push_back((it+3)->v);
				stl_data[surfIndex].push_back((it+2)->v);
				stl_data[surfIndex].push_back((it+1)->v);
				FindMax((it)->v);
				FindMax((it+1)->v);
				FindMax((it+2)->v);
				FindMax((it+3)->v);
				Bar.StepIt();
				Bar.StepIt();		
				it+=2;
			}
			Bar.StepIt();
			Bar.StepIt();
			it+=2;
			break;
		case 999:
			TRACE("Surf %d ended\n",surfIndex);
			surfIndex++;
			it++;
			break;
		case 0:
			Bar.StepIt();
			it++;		//end of data
			break;
		default:
			Bar.StepIt();
			it++;		//For test. could stay if it doesn't bother
			break;
		}
	}
	while(it!=cb_data.end());
	///////////////////////////////////////////////////
	Bar.DestroyWindow();

	UpdateAllViews (NULL);


	///////////////////////////////////////
	// TEST FOR TRIANGLE DIRECTION PROBLEM
	/*for(vector<Vector3<GLfloat>>::iterator its=stl_data.begin();its!=stl_data.end()-3;its++)
	{
	  Vector3<float> p0=*its;
	  its++;
	  Vector3<float> p1=*its;
	  its++;
	  Vector3<float> p2=*its;
	
	  Matrix3<float> m;
	  m.at(0,0)=p0.x;
	  m.at(0,1)=p1.x;
	  m.at(0,2)=p2.x;
	  m.at(1,0)=p0.y;
	  m.at(1,1)=p1.y;
	  m.at(1,2)=p2.y;
	  m.at(2,0)=p0.z;
	  m.at(2,1)=p1.z;
	  m.at(2,2)=p2.z;
	  float det=m.determinant();
	  if(det<0)
	  {
		  *its=p1;
		  its--;
		  *its=p2;
		  its++;
	  }

	  Vector3<float> v1=p1-p0;
	  Vector3<float> v2=p2-p0;
	  Vector3<float> v3=v1.crossProduct(v2);
	  TRACE("d:%f  v3: %f %f %f\n",det,v3.x,v3.y,v3.z);*/
	
	  /*
	  float dx1=(p1.x-p0.x), dy1=(p1.y-p0.y);
	  float dx2=(p2.x-p0.x), dy2=(p2.y-p0.y);
	  if(dy1*dx2 < dy2*dx1) TRACE("1\n");
	  else if(dy1*dx2 > dy2*dx1)
	  {
		  TRACE("-1\n");
		  *its=p1;
		  its--;
		  *its=p2;
		  its++;
	  }
	  else 
	  {
		  TRACE("none\n");
		
	  }
	  */
	 // if(dx1*dx2 < 0 || dy1*dy2 < 0) TRACE("-1\n");
	  //if((sqrt(dx1)+sqrt(dy1)) >= (sqrt(dx2)+sqrt(dy2)))
	//	TRACE("0\n");
	//  else
		//TRACE("1\n");
	//}
	/////////////////////////////////
	
	
}

//Helper function to find maximum value in mesh
void CIgesDoc::FindMax(Vector3<float>& v)
{
	if(fabs(v.x)>fabs(vMax.x)) vMax.x=v.x;
	if(fabs(v.y)>fabs(vMax.y)) vMax.y=v.y;
	if(fabs(v.z)>fabs(vMax.z)) vMax.z=v.z;
}