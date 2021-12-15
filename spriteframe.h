#ifndef SPRITEFRAME_H
#define SPRITEFRAME_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <iostream>
#include "QPixmap"
#include <QList>


using namespace std;

/*
 * This mainwindow header file responsibilities:
 * Holds and updates every pixel on the sprite frame.
*/

class SpriteFrame : public QWidget
{
    Q_OBJECT

private:
    //The 2D array holds the color of each sprite.
    QColor **two_d;
    void drawMirrorPixel(const QPoint &Point);
    void erase(const QPoint &point);
    bool drawing = false;
    bool mirrorBrush = false;
    bool erasing = false;
    bool usingEraser = false;
    bool usingPen= true;
    bool usingMirror = false;
    bool outOfPixel = false;
    int brushSize = 10;
    int myPenWidth = 1;
    QColor myPenColor = Qt::blue;
    int pixelSize  = 10;
    int height;
    int width;
    int frameIndex;
    QImage image;
    //The list of actions.
    QList<QImage> undoList;
    QList<QImage> redoList;

public:
    explicit SpriteFrame(QWidget *parent = nullptr);
    //Rule-of-three.
    ~SpriteFrame();
    SpriteFrame(const SpriteFrame& other);
    SpriteFrame& operator=(const SpriteFrame other);

    //A constructor for spirte frame that has width and height.
    SpriteFrame(int width, int height);
    enum Tool{eraser, pen, mirror};
    QSize sizeHint() const override;
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void paintEvent(QPaintEvent *event)override;
    void background();
    void toolUsing(Tool input);
    void clearFrame();
    void inverseFrame();
    void flipHorizontal();
    void flipVertical();
    //Update the current image in spriteframe
    void updateFrame();
    int getFrameIndex();
    void setFrameIndex(int i);
    //Help with emiting signal back to the frames to stash the changes into the list of frames.
    void updateInFrames();
    void changeColor(QColor);
    void changeBrushSize(int);
    void repaint();
    //If there is changes in the picture.
    void detectChange(QImage);
    //Returns the current image object.
    QImage returnImage();
    void drawPixel(const QPoint &Point);

signals:
    //Signal to the frame class to update the frame in the list of vector.
    void updateAfterModifying();

public slots:
    void undo();
    void redo();

};

#endif // SPRITEFRAME_H
