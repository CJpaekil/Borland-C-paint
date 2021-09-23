//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "IuliaClasses.h"
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TTimer *Timer1;
        TPaintBox *PaintBox1;
        TToolBar *ToolBar1;
        TToolButton *AddRectangle;
        TImageList *ToolbarImageList;
        TToolButton *AddEllipse;
        TToolButton *Arrow;
        TToolButton *LoadFile;
        TToolButton *SaveFile;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall PaintboxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall RectangleToolClick(TObject *Sender);
        void __fastcall EllipseToolClick(TObject *Sender);
        void __fastcall ManipulationToolClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall PaintboxMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall PaintboxMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall LoadFileClick(TObject *Sender);
        void __fastcall SaveFileClick(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
        Iulia::Document document;
        Iulia::RectangleTool rectangleTool;
        Iulia::EllipseTool ellipseTool;
        Iulia::ManipulationTool manipulationTool;
        Iulia::Tool* currentTool;
        Graphics::TBitmap* offscreenBuffer;
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 