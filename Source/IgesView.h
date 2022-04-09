// IgesView.h : interface of the CIgesView class
//


#pragma once


class CIgesView : public CView
{
protected: // create from serialization only
	CIgesView();
	DECLARE_DYNCREATE(CIgesView)

// Attributes
public:
	CIgesDoc* GetDocument() const;

	HGLRC m_hRC; //Rendering Context
	CDC* m_pDC; //Device Context
	
	CPoint m_MouseDownPoint;  //Temp variable for rotation and zoom
	Vector4<float> color;
	BOOL normalCorrectionMode;

	//Tessellation callback functions
	static void CALLBACK beginCallback(GLenum which, void *userData);
	static void CALLBACK endCallback(void *userData);
	static void CALLBACK errorCallback(GLenum errorCode);
	static void CALLBACK vertexCallback(GLfloat *vertex, void *userData);
	static void CALLBACK normalCallback(GLfloat *normal, void *userData);

	//static int numpoly;
// Operations
public:
	BOOL InitializeOpenGL();	//Initialize the opengl properties
	BOOL SetupPixelFormat();	//Setups the rendering context
	void RenderScene (GLenum mode);		//Draws to screen
	void processHits (GLint hits, GLuint buffer[]);


// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CIgesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnMeshSampling();
	afx_msg void OnViewPoly();
	afx_msg void OnViewFill();
	afx_msg void OnUpdateViewFill(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewPoly(CCmdUI *pCmdUI);
	afx_msg void OnMeshUnsupported();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateMeshUnsupported(CCmdUI *pCmdUI);
	afx_msg void OnViewOutline();
	afx_msg void OnUpdateViewOutline(CCmdUI *pCmdUI);
	afx_msg void OnMeshColor();
	afx_msg void OnMeshEntitiesinthismodel();
	afx_msg void OnViewUseantialiasing();
	afx_msg void OnUpdateViewUseantialiasing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnViewUsetransparency();
	afx_msg void OnUpdateViewUsetransparency(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMeshNormalcorrection();
	afx_msg void OnUpdateMeshNormalcorrection(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in IgesView.cpp
inline CIgesDoc* CIgesView::GetDocument() const
   { return reinterpret_cast<CIgesDoc*>(m_pDocument); }
#endif

