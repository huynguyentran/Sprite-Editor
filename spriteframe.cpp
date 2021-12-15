#include "spriteframe.h"
#include <QMouseEvent>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <iostream>

/*
 * Construct the sprite frame class, set the basic
 * data of the sprite frame.
 */
SpriteFrame::SpriteFrame(QWidget *parent) : QWidget(parent)
{
    //Sets height and width.
    height = 50;
    width = 50;
    //Sets the 2d array QColor to hold all the
    //colors
    two_d = new QColor*[width];
    for (int i = 0;i<width;i++)
    {
        two_d[i] = new QColor[height];
    }
    //Sets the image object to modify on.
    image = {height*pixelSize, width*pixelSize, QImage::Format_RGB32};
    //Draws the background and set
    //the image to the scroll area.
    background();
    setMinimumSize(image.size());
    undoList.append(image);
}

/*
 * Construct the frame class with input width and height.
 */
SpriteFrame::SpriteFrame(int inputWidth, int inputHeight){
    height = inputHeight;
    width = inputWidth;
    two_d = new QColor*[width];
    for (int i = 0;i<width;i++){
        two_d[i] = new QColor[height];
    }
    image = {inputWidth*pixelSize, inputWidth*pixelSize, QImage::Format_RGB32};
    background();
    setMinimumSize(image.size());
    undoList.append(image);
}

/*
 * Rule-of-three of the Sprite Frame class.
 */
SpriteFrame::~SpriteFrame(){
    for (int i = 0;i<width;i++)
        delete[] two_d[i];
    delete[] two_d;
}

SpriteFrame::SpriteFrame(const SpriteFrame& other){
    height = other.height;
    width = other.width;
    two_d = new QColor*[width];
    for (int i = 0;i<width;i++)
    {
        two_d[i] = new QColor[height];
    }

    for (int x = 0; x < width;x++)
    {
        for(int y =0; y < height;y++)
        {
            if(other.two_d[x][y].isValid())
            {
                two_d[x][y]= other.two_d[x][y];
            }
        }
    }
    image = other.image;
    drawing = other.drawing;
    erasing = other.erasing;
    usingEraser = other.usingEraser;
    usingPen = other.usingPen;
    pixelSize  = other.pixelSize;
    outOfPixel = other.outOfPixel;
    myPenWidth = other.myPenWidth;
    myPenColor = other.myPenWidth;
    frameIndex =other.frameIndex;
    setMinimumSize(other.image.size());
}

SpriteFrame& SpriteFrame::operator=(SpriteFrame other){
    std::swap(image,other.image);
    std::swap(height,other.height);
    std::swap(width,other.width);
    std::swap(two_d,other.two_d);
    std::swap(drawing, other.drawing);
    std::swap(erasing, other.erasing);
    std::swap(usingEraser,other.usingEraser);
    std::swap(usingPen,other.usingPen);
    std::swap(pixelSize,other.pixelSize);
    std::swap(outOfPixel,other.outOfPixel);
    std::swap(myPenWidth,other.myPenWidth);
    std::swap(myPenColor,other.myPenColor);
    std::swap(frameIndex,other.frameIndex);
    setMinimumSize(image.size());
    return *this;
}

/*
 * Returns the size of the image object.
 */
QSize SpriteFrame::sizeHint() const
{
    return image.size();
}

/*
 * Clears the current sprite frame.
 */
void SpriteFrame::clearFrame()
{
    //Redraws the background and
    //clear the inputs list.
    background();
    redoList.clear();
    undoList.clear();
    update();
    //Sets the changes to the image.
    detectChange(image);
}

/*
 * Detects any change to the image.
 */
void SpriteFrame::detectChange(QImage oldImage)
{
    //Append into the list to
    //remember the user's action.
    QImage newImage = oldImage;
    undoList.append(newImage);
    redoList.clear();
}

/*
 * Draws the background.
 */
void SpriteFrame::background(){
    //Fills the image with white color.
    image.fill(qRgb(255, 255, 255));
    //Sets grey value for grey box.
    QRgb value = qRgb(128, 128, 128); // 0xffbd9527
    QPainter painter(&image);
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            //Fills the grey square to create
            //correct pattern fo the background.
            if ((col+row) % 2 == 0)
            {
                painter.fillRect(pixelSize * col, pixelSize * row, pixelSize, pixelSize, value);
            }
        }
    }
}


/*
 * Switchs to the tool that the user is using.
 */
void SpriteFrame::toolUsing(Tool input){
    if (input == eraser)
    {
        usingEraser = true;
        usingPen = false;
        usingMirror = false;
    }
    else if (input == mirror)
    {
        usingMirror = true;
        usingEraser = false;
        usingPen = false;
    }
    else{
        usingMirror = false;
        usingEraser = false;
        usingPen = true;
    }
}

/*
 * Remember the mouse movement and inputs of the user to
 * modify the sprite frame with the correct tool.
 */
void SpriteFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(usingEraser)
        {
            erasing= true;
        }
        if(usingPen)
        {
            drawing = true;
        }
        if(usingMirror)
        {
            mirrorBrush= true;
        }

    }
}

void SpriteFrame::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() && Qt::LeftButton==true){
        if(drawing)
        {
            drawPixel(event->pos());
        }
        if(erasing)
        {
            erase(event->pos());
        }
        if(mirrorBrush)
        {
            drawMirrorPixel(event->pos());
        }
    }
}

void SpriteFrame::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(drawing)
        {
            drawPixel(event->pos());
            drawing = false;
        }
        if (erasing)
        {
            erase(event->pos());
            erasing = false;
        }
        if(mirrorBrush)
        {
            drawMirrorPixel(event->pos());
            mirrorBrush = false;
        }
    }
     detectChange(image);
}

/*
 * Sets up a paint event to modify the sprite frame.
 */
void SpriteFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = event->rect();
    painter.drawImage(rect, image, rect);
}

/*
 * Draws the pixel with the input of the user.
 */
void SpriteFrame::drawPixel(const QPoint &point)
{
    QPainter painter(&image);
    int x = point.x()/brushSize;
    int y = point.y()/brushSize;
    //Fills the pixel at the correct location with the corrent color.
    painter.fillRect(x*brushSize,y*brushSize,brushSize,brushSize,myPenColor);
    //Send a signal to the frame class to update the list of sprite frame
    //saving the user input.
    updateInFrames();
    //Updates to show up in the UI.
    update();
}

/*
 * Draws the pixel with the input of the user. Also mirror the user on the Y-axis.
 */
void SpriteFrame::drawMirrorPixel(const QPoint &point)
{
    QPainter painter(&image);
    int x = point.x()/brushSize;
    int y = point.y()/brushSize;
    painter.fillRect(x*brushSize,y*brushSize,brushSize,brushSize,myPenColor);
    painter.fillRect(width*pixelSize-x*brushSize,y*brushSize,brushSize,brushSize,myPenColor);
    updateInFrames();
    update();
}


/*
 * Erases the user input and redraw the background.
 */
void SpriteFrame::erase(const QPoint &point)
{
    QPainter painter(&image);
    int x = point.x()/pixelSize;
    int y = point.y()/pixelSize;
    QRgb valueGrey = qRgb(128, 128, 128);
    //Fills in the correct background squre.
    if ((x%2==y%2))
    {
        painter.fillRect(x*pixelSize,y*pixelSize,pixelSize,pixelSize,valueGrey);
    }
    else
    {
        painter.fillRect(x*pixelSize,y*pixelSize,pixelSize,pixelSize,Qt::white);
    }
    updateInFrames();
    update();
}

/*
 * Inverts the color of everything in the sprite frame, included the background.
 */
void SpriteFrame::inverseFrame()
{
    image.invertPixels();
    updateInFrames();
    update();
}

/*
 * Flips the sprite frame horizontally.
 */
void SpriteFrame::flipHorizontal()
{
    image = image.mirrored(true,false);
    updateInFrames();
    update();
}

/*
 * Flips the sprite frame vertically.
 */
void SpriteFrame::flipVertical()
{
    image = image.mirrored(false,true);
        updateInFrames();
    update();
}

/*
 * Updates the current frame to reflect on the UI.
 */
void SpriteFrame::updateFrame()
{
    update();
}

/*
 * Changes the color based on the input of the UI.
 */
void SpriteFrame::changeColor(QColor color)
{
    myPenColor = color;
}

/*
 * Returns the current sprite frame index.
 */
int SpriteFrame::getFrameIndex()
{
    return frameIndex;
}

/*
 * Sets the current sprite frame index.
 */
void SpriteFrame::setFrameIndex(int i)
{
    this->frameIndex=i;
}

/*
 * Changes the brush size.
 */
void SpriteFrame::changeBrushSize(int penSize)
{
    brushSize = penSize;

}


/*
 * Sends the signal to the frame class to remember
 * the modification of the current sprite frame.
 */
void SpriteFrame::updateInFrames()
{
    emit updateAfterModifying();
}


/*
 * Repaints the sprite frame based on the frame that the
 * user is choosing.
 */
void SpriteFrame::repaint()
{
    for (int x = 0; x < width;x++)
    {
        for(int y =0; y < height;y++)
        {
            if(two_d[x][y].isValid())
            {
                QPoint point(x,y);
                drawPixel(point);
            }
        }
    }
    update();
}

/*
 * Returns the image object.
 */
QImage SpriteFrame::returnImage(){
    return image;
}

/*
 * Undo the latest action of the user.
 */
void SpriteFrame::undo()
{
    if(undoList.size() > 1)
    {
        //Puts the action into redo.
        redoList.append(undoList.last());
        undoList.removeLast();
        //Update the images based on the undo list.
        QImage updatedImage = undoList.last();
        image = updatedImage;
        update();
        updateInFrames();
    }
}

/*
 * Redo the latest action of the user.
 */
void SpriteFrame::redo()
{
    if(redoList.size() > 0)
    {
        //Take the last action of the user in redo list.
        //Updates the image.
        QImage updatedImage = redoList.last();
        image = updatedImage;
        update();
        undoList.append(redoList.last());
        redoList.removeLast();
        updateInFrames();
    }
}
