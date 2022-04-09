// IgesDoc.h : interface of the CIgesDoc class
//

#pragma once
#include "IgesRead.h"

////////////////////////////////////
// Structure to hold data retrieved
// from nurb callback functions
struct CBDATA {
	BOOL beg;
	unsigned int surface;
	unsigned int type;
	Vector3<float> v;
};
////////////////////////////////////

class CIgesDoc : public CDocument
{
protected: // create from serialization only
	CIgesDoc();
	DECLARE_DYNCREATE(CIgesDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CIgesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	IgesRead mIgesReadObj;	//IGES object
	GLUnurbs *nurb;			//NURBS object
	
	float sampleTol;	//sampling tolerance
	float step;			//domain step
	float trimTol;		//trimming tolerance
	BOOL viewPoly;
	BOOL viewFill;
	BOOL viewPoints;
	int method;		//sampling method
	
	vector<CBDATA> cb_data;				//callback data
	vector<Vector3<float>>* stl_data;	//triangulation for stl data. array of surfaces
	Vector3<float> vMax;				//Maximum value in mesh
		
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void Process(BOOL reProcess=TRUE);	//Process the file
	void FindMax(Vector3<float>& v);	//Helper function to find maximum value in mesh

	GLfloat m_xAngle;     //Mouse rotation/zoom/translation variables
	GLfloat m_yAngle;
	GLfloat m_Zoom;
	GLfloat m_xPos;
	GLfloat m_yPos;

	GLuint* DLnumber;	//Array holding index of surfaces
	GLuint* winding;	//Winding direction (0 - CCW, 1 - CW) of surface
						//Matches with DLnumber
	GLuint DLbase;		//Index of first list
	GLuint DLsize;		//Number of display lists
	

};


