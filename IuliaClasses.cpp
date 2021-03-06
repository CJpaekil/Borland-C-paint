//---------------------------------------------------------------------------

#include "IuliaClasses.h"
#include <algorithm>
#include <iostream>
#include <vcl.h>

namespace Iulia {

Vec2i::Vec2i(int x, int y) {
        this->x = x;
        this->y = y;
}

Vec2i Shape::getTopLeft() const {
        return Vec2i(position.x, position.y);
}

Vec2i Shape::getTopRight() const {
        return Vec2i(position.x + size.x, position.y);
}

Vec2i Shape::getBottomLeft() const {
        return Vec2i(position.x, position.y + size.y);
}

Vec2i Shape::getBottomRight() const {
        return Vec2i(position.x + size.x, position.y + size.y);
}

void Shape::setCorners(Vec2i corner1, Vec2i corner2) {
        int left = std::min(corner1.x, corner2.x);
        int right = std::max(corner1.x, corner2.x);
        int top = std::min(corner1.y, corner2.y);
        int bottom = std::max(corner1.y, corner2.y);
        position = Vec2i(left, top);
        size = Vec2i(right - left, bottom - top);
}

void Ellipse::drawTo(TCanvas* canvas) {
      canvas->Ellipse(position.x, position.y,
        position.x + size.x, position.y + size.y);
}

void Ellipse::writeToStream(std::ostream& stream) {
        stream << "ellipse "
                << position.x << " "
                << position.y << " "
                << size.x << " "
                << size.y << "\n";
}

void Rectangle::drawTo(TCanvas* canvas) {
        canvas->Rectangle(position.x, position.y,
        position.x + size.x, position.y + size.y);
}

void Rectangle::writeToStream(std::ostream& stream) {
        stream << "rectangle "
                << position.x << " "
                << position.y << " "
                << size.x << " "
                << size.y << "\n";
}

void Document::addShape(Shape* shape) {
        shapes.push_back(shape);
        manipulationTool->onShapeAdded(shape);
}

void Document::deleteShape(Shape* shape) {
        shapes.erase(std::remove(shapes.begin(), shapes.end(), shape), shapes.end());
        manipulationTool->onShapeDeleted(shape);
        delete shape;
}


void Document::loadFromStream(std::istream& stream) {
        for(int i = 0; i < shapes.size(); i++) {
                manipulationTool->onShapeDeleted(shapes[i]);
                delete shapes[i];
        }
        shapes.clear();
        std::string shapeType;
        Vec2i position, size;
        while(stream >> shapeType >> position.x >> position.y >> size.x >> size.y) {
                Shape* newShape;
                if(shapeType == "ellipse")
                        newShape = new Ellipse();
                else if(shapeType == "rectangle")
                        newShape = new Rectangle();
                newShape->position = position;
                newShape->size = size;
                addShape(newShape);
        }
}

void Document::writeToStream(std::ostream& stream) {
        for(int i = 0; i < shapes.size(); i++) {
                shapes[i]->writeToStream(stream);
        }
}

void Document::setManipulationTool(ManipulationTool* manipulationTool) {
        this->manipulationTool = manipulationTool;
}

std::vector<Shape*>& Document::getShapes() {
        return shapes;
}

Tool::Tool(Document& document) {
        this->document = &document;
}

void Tool::drawDocumentToCanvas(Iulia::Document const& document, TCanvas* canvas) {
        for(int i = 0; i < document.getShapes().size(); i++) {
                canvas->Pen->Color = clBlack;
                canvas->Brush->Style = bsClear;
                document.getShapes()[i]->drawTo(canvas);
        }
}


ShapeTool::ShapeTool(Document& document) : Tool(document) {
        currentlyCreatedShape = NULL;
}
RectangleTool::RectangleTool(Document& document) : ShapeTool(document) {}
EllipseTool::EllipseTool(Document& document) : ShapeTool(document) {}
ManipulationTool::ManipulationTool(Document& document) : Tool(document) {
        manipulatedHandle = NULL;
        selectedShape = NULL;
}
Handle::Handle(Shape* controlledShape) {
        this->controlledShape = controlledShape;
}

ResizeHandle::ResizeHandle(Shape* controlledShape) : Handle(controlledShape) {
}

TRect ResizeHandle::getRect() {
        const int radius = 5;
        return TRect(getPosition().x - radius-15, getPosition().y - radius-15,
        getPosition().x + radius, getPosition().y + radius);
}

void ResizeHandle::drawTo(TCanvas* canvas) {
        canvas->Rectangle(getRect());


}

void ResizeHandle::beginMove(Vec2i) {
        oppositeCornerAtStartOfResize = getOppositeCorner();
}
void ResizeHandle::move(Vec2i newPos) {
        controlledShape->setCorners(newPos, oppositeCornerAtStartOfResize);
}

MoveHandle::MoveHandle(Shape* controlledShape) : Handle(controlledShape) {
}

TRect MoveHandle::getRect() {
	const int radius = 5;
	return TRect(getPosition().x - radius, getPosition().y - radius,
			getPosition().x + radius+ controlledShape->size.x-13, getPosition().y + radius+ controlledShape->size.x-13);
}

void MoveHandle::drawTo(TCanvas* canvas) {

	//canvas->Ellipse(getRect());
}

void MoveHandle::beginMove(Vec2i initialPos) {
	lastPosition = initialPos;
}

void MoveHandle::move(Vec2i newPos) {
	controlledShape->position.x += newPos.x - lastPosition.x;
	controlledShape->position.y += newPos.y - lastPosition.y;
	lastPosition = newPos;
}

Vec2i MoveHandle::getPosition() {
	return Vec2i(controlledShape->position.x,
	  		controlledShape->position.y);
}

Shape* RectangleTool::createNewShape() {
        return new Rectangle;
}

Shape* EllipseTool::createNewShape() {
        return new Ellipse;
}

TopLeftResizeHandle::TopLeftResizeHandle(Shape* controlledShape) : ResizeHandle(controlledShape) {
}

Vec2i TopLeftResizeHandle::getPosition() {
	return controlledShape->getTopLeft();
}

Vec2i TopLeftResizeHandle::getOppositeCorner() {
	return controlledShape->getBottomRight();
}

TopRightResizeHandle::TopRightResizeHandle(Shape* controlledShape) : ResizeHandle(controlledShape) {
}

Vec2i TopRightResizeHandle::getPosition() {
	return controlledShape->getTopRight();
}

Vec2i TopRightResizeHandle::getOppositeCorner() {
	return controlledShape->getBottomLeft();
}

BottomLeftResizeHandle::BottomLeftResizeHandle(Shape* controlledShape) : ResizeHandle(controlledShape) {
}

Vec2i BottomLeftResizeHandle::getPosition() {
	return controlledShape->getBottomLeft();
}

Vec2i BottomLeftResizeHandle::getOppositeCorner() {
	return controlledShape->getTopRight();
}

BottomRightResizeHandle::BottomRightResizeHandle(Shape* controlledShape) : ResizeHandle(controlledShape) {
}

Vec2i BottomRightResizeHandle::getPosition() {
	return controlledShape->getBottomRight();
}

Vec2i BottomRightResizeHandle::getOppositeCorner() {
	return controlledShape->getTopLeft();
}

void ShapeTool::onMousePress(Vec2i position) {
        currentCorner1 = position;
        currentlyCreatedShape = createNewShape();
        document->addShape(currentlyCreatedShape);
}

void ShapeTool::onMouseRelease(Vec2i position) {
        currentlyCreatedShape = NULL;
}

Shape* Handle::getControlledShape() {
        return controlledShape;
}

void ShapeTool::onMouseMove(Vec2i position) {
        if(currentlyCreatedShape != NULL) {
                currentCorner2 = position;
                currentlyCreatedShape->setCorners(currentCorner1, currentCorner2);
        }
}


void ManipulationTool::drawDocumentToCanvas(Iulia::Document const& document, TCanvas* canvas) {
        canvas->Brush->Style = bsClear;
        for(int i = 0; i < document.getShapes().size(); i++) {
                Shape* shape = document.getShapes()[i];
                canvas->Pen->Color = clBlack;
                shape->drawTo(canvas);
        }

        for(int i = 0; i < handles.size(); i+=4) {
                Handle* handle = handles[i];
                canvas->Pen->Color = clGray;
                handle->drawTo(canvas);
        }

        if(selectedShape != NULL) {
                canvas->Brush->Style = bsBDiagonal;
                canvas->Brush->Color = clGray;
                selectedShape->drawTo(canvas);
        }
}

void ManipulationTool::onShapeAdded(Shape* shape) {
        handles.push_back(new TopLeftResizeHandle(shape));
        handles.push_back(new TopRightResizeHandle(shape));
        handles.push_back(new BottomLeftResizeHandle(shape));
        handles.push_back(new BottomRightResizeHandle(shape));
        handles.push_back(new MoveHandle(shape));
}

void ManipulationTool::onShapeDeleted(Shape* shape) {
        std::vector<Handle*> newHandleList;
        for(int i = 0; i < handles.size(); i++) {
                Handle* handle = handles[i];
                if(handle->getControlledShape() == shape)
                        delete handle;
                else
                        newHandleList.push_back(handle);
        }
        handles = newHandleList;
}

void ManipulationTool::onMousePress(Vec2i mousePos) {
        // if selectedShape doesn't get a value in the in the next loop, it will remain NULL.
        selectedShape = NULL;
	for(int i = 0; i < handles.size(); i++) {
		Handle* handle = handles[i];
		TRect rect = handle->getRect();

                //ShowMessage(mousePos.x);
		if(rect.left < mousePos.x && rect.right > mousePos.x && rect.top < mousePos.y && rect.Bottom > mousePos.y) {
                        //ShowMessage("cv");
			handle->beginMove(mousePos);
			manipulatedHandle = handle;
                        selectedShape = handle->getControlledShape();
			break;
		}
	}
}
void ManipulationTool::onMouseRelease(Vec2i mousePos) {
	manipulatedHandle = NULL;
}
void ManipulationTool::onMouseMove(Vec2i mousePos) {
	if(manipulatedHandle != NULL) {
	       	manipulatedHandle->move(mousePos);
	}
}
void ManipulationTool::onKeyDown(WORD& key) {
        if(key == VK_DELETE) {
                if(selectedShape != NULL) {
                        document->deleteShape(selectedShape);
                        selectedShape = NULL;
                }
        }
}

} // namespace Iulia

//---------------------------------------------------------------------------

//#pragma package(smart_init)



