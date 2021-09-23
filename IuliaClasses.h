//---------------------------------------------------------------------------

#ifndef IuliaClassesH
#define IuliaClassesH

#include <vcl.h>
#include <vector>


#pragma hdrstop

namespace Iulia {

class Vec2i {
public:
        int x;
        int y;

        Vec2i() { }
        Vec2i(int x, int y);
};

class Shape {
public:
        Shape() { }

        Vec2i position;
        Vec2i size;

        virtual ~Shape() { }

        virtual void drawTo(TCanvas* canvas) = 0;

        virtual void writeToStream(std::ostream& stream) = 0;

        Vec2i getTopLeft() const;

        Vec2i getTopRight() const;

        Vec2i getBottomLeft() const;

        Vec2i getBottomRight() const;

        void setCorners(Vec2i corner1, Vec2i corner2);
};

class Ellipse : public Shape {
public:
        virtual void drawTo(TCanvas* canvas);
        virtual void writeToStream(std::ostream& stream);
};

class Rectangle : public Shape {
public:
        virtual void drawTo(TCanvas* canvas);
        virtual void writeToStream(std::ostream& stream);
};

class ManipulationTool;

class Document {
public:
        void addShape(Shape* shape);
        void deleteShape(Shape* shape);
        void loadFromStream(std::istream& stream);
        void writeToStream(std::ostream& stream);
        void setManipulationTool(ManipulationTool*);
        std::vector<Shape*>& getShapes();

private:
        std::vector<Shape*> shapes;
        ManipulationTool* manipulationTool;
};

class Tool {
public:
        Tool(Document& document);
        virtual ~Tool() { }
        virtual void drawDocumentToCanvas(Iulia::Document const& document, TCanvas* canvas);
        virtual void onMousePress(Vec2i position) { }
        virtual void onMouseRelease(Vec2i position) { }
        virtual void onMouseMove(Vec2i position) { }
        virtual void onKeyDown(WORD& key) { }

protected:
        Document* document;
};

class ShapeTool : public Tool {
public:
        ShapeTool(Document& document);
        virtual void onMousePress(Vec2i position);
        virtual void onMouseRelease(Vec2i position);
        virtual void onMouseMove(Vec2i position);

protected:
        virtual Shape* createNewShape() = 0;

private:
        Vec2i currentCorner1;
        Vec2i currentCorner2;
        Shape* currentlyCreatedShape;
};

class RectangleTool : public ShapeTool {
public:
        RectangleTool(Document& document);

protected:
        virtual Shape* createNewShape();
};

class EllipseTool : public ShapeTool {
public:
        EllipseTool(Document& document);

protected:
        virtual Shape* createNewShape();
};

class Handle {
public:
        Handle(Shape* controlledShape);
        virtual Vec2i getPosition() = 0;
        virtual TRect getRect() = 0;
        virtual void move(Vec2i) = 0;
        virtual void beginMove(Vec2i) { }
        virtual void drawTo(TCanvas* canvas) = 0;
        Shape* getControlledShape();

protected:
        Shape* controlledShape;
};

class ResizeHandle : public Handle {
public:
        ResizeHandle(Shape* controlledShape);
        TRect getRect();
        void drawTo(TCanvas* canvas);
        virtual Vec2i getOppositeCorner() = 0;
        void beginMove(Vec2i);
        void move(Vec2i newPos);
        
private:
        Vec2i oppositeCornerAtStartOfResize;        
};

class MoveHandle : public Handle {
public:
        MoveHandle(Shape* controlledShape);
        virtual TRect getRect();
        virtual void drawTo(TCanvas* canvas);
        virtual void beginMove(Vec2i initialPos);
        virtual void move(Vec2i newPos);
        virtual Vec2i getPosition();

private:
        Vec2i lastPosition;
};

class TopLeftResizeHandle : public ResizeHandle {
public:
        TopLeftResizeHandle(Shape* controlledShape);
private:
        virtual Vec2i getPosition();
        virtual Vec2i getOppositeCorner();
};

class TopRightResizeHandle : public ResizeHandle {
public:
        TopRightResizeHandle(Shape* controlledShape);
private:
        virtual Vec2i getPosition();
        virtual Vec2i getOppositeCorner();
};

class BottomLeftResizeHandle : public ResizeHandle {
public:
        BottomLeftResizeHandle(Shape* controlledShape);
private:
        virtual Vec2i getPosition();
        virtual Vec2i getOppositeCorner();
};

class BottomRightResizeHandle : public ResizeHandle {
public:
        BottomRightResizeHandle(Shape* controlledShape);
private:
        virtual Vec2i getPosition();
        virtual Vec2i getOppositeCorner();
};

class ManipulationTool : public Tool {
public:
        virtual void drawDocumentToCanvas(Iulia::Document const& document, TCanvas* canvas);
        ManipulationTool(Document& document);

        void onShapeAdded(Shape* shape);
        void onShapeDeleted(Shape* shape);

        virtual void onMousePress(Vec2i mousePos);
        virtual void onMouseRelease(Vec2i mousePos);
        virtual void onMouseMove(Vec2i mousePos);
        virtual void onKeyDown(WORD& key);

private:
        std::vector<Handle*> handles;
        Handle* manipulatedHandle;
        Shape* selectedShape;
};

}
//---------------------------------------------------------------------------
#endif
