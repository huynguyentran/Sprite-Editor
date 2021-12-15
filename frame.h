#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QPainter>
#include "spriteframe.h"
#include <iostream>

using namespace std;


/*
 * This frame header file responsibilities:
 * Holds the information of every spriteframes
 * Sends the information to the model to view in UI
*/
class frame : public QObject
{
    Q_OBJECT

private:
    QColor curColor;
    //Number of frames we currently have.
    int numOfCurrentFrames;
    //Update the preview on the top right.
    void updatePreview(int i);
    //The frame that is being played.
    int currentFrameInAnimation =0;

public:
    explicit frame(QObject *parent = nullptr);
    //The list of frames.
    std::vector<SpriteFrame> listOfFrame;
    //Rules-of-three.
    frame(const frame& other);
    ~frame();
    frame& operator=(const frame other);
    //Enum for tool.
    enum Tool{eraser, pen, mirror};
    Tool curTool;
    //The current frame on the editor.
    SpriteFrame currentFrame;
    //Return the current number of current frames.
    int getNumOfFrames();
    //The first frame to create when start the editor.
    void initialFrame();
    //Sets the current number of frame.
    void setCurrentNumberOfFrame(int i);

public slots:
    void resize(int width, int height);
    void frameClearFrame();
    void frameInverseFrame();
    void frameFlipHorizontal();
    void frameFlipVertical();
    void userSelectTool(frame::Tool);
    void addAFrame();
    void loadFrame(int i);
    void duplicateFrame();
    void deleteCurrentFrame(int i);
    void framechangeColor(QColor color);
    void updateTheFrameList();
    void framechangeBrushSize(int);
    void runAnimation();

signals:
    void updateFrame();
    void frameNum(int);
    void userErased();
    void changeImageLabel(QPixmap);
};

#endif // FRAME_H
