//---------------------------------------------------------------------------

#include <vcl.h>
#include <fstream>
#include <jpeg.hpp>
#pragma hdrstop


#include "Unit1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
        , rectangleTool(document)
        , ellipseTool(document)
        , manipulationTool(document)
{
        document.setManipulationTool(&manipulationTool);

        OpenDialog1->Filter = "*.pic";
        OpenDialog1->DefaultExt = "pic";
        SaveDialog1->Filter = "*.jpg";
        SaveDialog1->DefaultExt = "jpg";

        offscreenBuffer = new Graphics::TBitmap();
        offscreenBuffer->Width  = PaintBox1->Width;
        offscreenBuffer->Height = PaintBox1->Height;

        PaintBox1->ControlStyle = PaintBox1->ControlStyle << csOpaque;
}

void __fastcall TForm1::FormResize(TObject *Sender)
{
        PaintBox1->Width=this->Width-40;
        PaintBox1->Height=this->Height-80;
}

void __fastcall TForm1::FormPaint(TObject *Sender)
{
        //offscreenBuffer->Width = this->Width;
        //offscreenBuffer->Height = this->Height;
        offscreenBuffer->Canvas->Brush->Color = clWhite;
        offscreenBuffer->Canvas->Brush->Style = bsSolid;


        offscreenBuffer->Canvas->FillRect(PaintBox1->ClientRect);
        currentTool->drawDocumentToCanvas(document, offscreenBuffer->Canvas);

        PaintBox1->Canvas->Draw(0, 0, offscreenBuffer);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
        FormPaint(this);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::PaintboxMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        currentTool->onMousePress(Iulia::Vec2i(X, Y));
}
//---------------------------------------------------------------------------



void __fastcall TForm1::RectangleToolClick(TObject *Sender)
{
        currentTool = &rectangleTool;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EllipseToolClick(TObject *Sender)
{
        currentTool = &ellipseTool;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ManipulationToolClick(TObject *Sender)
{
        currentTool = &manipulationTool;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
        currentTool = &rectangleTool;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PaintboxMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        currentTool->onMouseRelease(Iulia::Vec2i(X, Y));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PaintboxMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
        currentTool->onMouseMove(Iulia::Vec2i(X, Y));        
}
//---------------------------------------------------------------------------





void __fastcall TForm1::LoadFileClick(TObject *Sender)
{
        if (OpenDialog1->Execute()) {
                std::ifstream s(OpenDialog1->FileName.c_str());
                document.loadFromStream(s);
        }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveFileClick(TObject *Sender)
{

                if (SaveDialog1->Execute()) {
                offscreenBuffer->Width = PaintBox1->Width;
                 offscreenBuffer->Height= PaintBox1->Height;
                 offscreenBuffer->SaveToFile(SaveDialog1->FileName.c_str());

                }


}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        currentTool->onKeyDown(Key);
}
//---------------------------------------------------------------------------





