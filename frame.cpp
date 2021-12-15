#include "frame.h"
#include <QtDebug>
#include <QFileDialog>

/*
 * Construct the frame class, set the number of frame to 0
 * and create an initial sprite frame.
 */
frame::frame(QObject *parent) : QObject(parent)
{
    numOfCurrentFrames=0;
    initialFrame();
}

/*
 * Sets the initial sprite frame when starting up the program.
 */
void frame::initialFrame()
{
    //Create a new spriteframe and set it to the current frame.
    SpriteFrame newFrame;
    currentFrame = newFrame;
    //Set the index of the current frame, push to the list
    //and increase the index.
    currentFrame.setFrameIndex(numOfCurrentFrames);
    listOfFrame.push_back(newFrame);
    numOfCurrentFrames++;
}

/*
 * Adds a sprite frame to the editor
 */
void frame::addAFrame()
{
    //Update the changes in the current sprite frame.
    listOfFrame[currentFrame.getFrameIndex()] = currentFrame;
    //Add a new sprite frame to the list.
    SpriteFrame newFrame;
    //Switch to the new sprite frame in the editor
    currentFrame = newFrame;
    //Set the index of the newly
    currentFrame.setFrameIndex(numOfCurrentFrames);
    listOfFrame.push_back(newFrame);
    numOfCurrentFrames++;
    currentFrame.updateFrame();
    //Remains the tool that was using in the previous sprite frame.
    framechangeColor(curColor);
    userSelectTool(curTool);
}

/*
 * Rule-of-three for class frame.
 */
frame::frame(const frame& other)
{
    numOfCurrentFrames = other.numOfCurrentFrames;
    listOfFrame = other.listOfFrame;
    currentFrameInAnimation = other.currentFrameInAnimation;
    curColor = other.curColor;
}

frame& frame::operator=(frame other)
{
    std::swap(numOfCurrentFrames, other.numOfCurrentFrames);
    std::swap(listOfFrame, other.listOfFrame);
    std::swap(currentFrameInAnimation, other.currentFrameInAnimation);
    std::swap(curColor, other.curColor);
    return *this;
}

frame::~frame() {
    listOfFrame.clear();
    numOfCurrentFrames=0;
    currentFrameInAnimation=0;
}

/*
 * Resizes the current sprite frame in the sprite editor.
 */
void frame::resize(int width, int height)
{
    //Creates a new sprite frame and set the current frame
    //to the new frame.
    SpriteFrame newSprite(width, height);
    currentFrame = newSprite;
    currentFrame.updateFrame();
}


/*
 * Duplicates the current choosing sprite frame.
 */
void frame::duplicateFrame()
{
    //Assign the new frame to the current frame.
    SpriteFrame newFrame = currentFrame;
    listOfFrame.push_back(newFrame);
    newFrame.setFrameIndex(numOfCurrentFrames);
    listOfFrame[currentFrame.getFrameIndex()] = currentFrame;
    numOfCurrentFrames++;
    currentFrame=newFrame;
    framechangeColor(curColor);
    userSelectTool(curTool);
    currentFrame.updateFrame();
}

/*
 * Deletes the current choosing sprite frame.
 */
void frame::deleteCurrentFrame(int i){
    //If we only has one sprite frame
    //Clears that sprite frame
    if (listOfFrame.size()==1)
    {
        currentFrame.clearFrame();
        currentFrame.updateFrame();
        return;
    }

    //Incase we are deleting the 0 frame spirte
    //- the first sprite frame on the list.
    if (i == 0)
    {
        //We erase from the list of sprite frame.
        listOfFrame.erase(listOfFrame.begin());
        //We update the remain sprite frames with new indexes.
        for(int index = 0; index < listOfFrame.size(); index++)
        {
            listOfFrame[index].setFrameIndex(index);
        }
        //Set the current frame to the next
        //frame that we deleted.
        currentFrame = listOfFrame[0];
        currentFrame.updateFrame();
        numOfCurrentFrames--;
        return;
    }

    //If we deleted the final frame
    //We don't need to update all the indexes
    if (i == listOfFrame.size() - 1)
    {
        currentFrame = listOfFrame[i-1];
    }
    //Updates all the index of the remains sprite frames
    //and changes the sprite frames.
    else
    {
        for(int index = i; index < listOfFrame.size()-1; index++)
        {
            listOfFrame[index] = listOfFrame[index+1];
            listOfFrame[index].setFrameIndex(index);
        }
        currentFrame = listOfFrame[i];
    }

    //Remove the last frame because
    //we shift sprite frame down by 1.
    currentFrame.updateFrame();
    numOfCurrentFrames--;
    listOfFrame.erase(listOfFrame.end()-1);
}

/*
 * Clear the current sprite frame.
 */
void frame::frameClearFrame()
{
    currentFrame.clearFrame();

}

/*
 * Inverse the current sprite frame.
 */
void frame::frameInverseFrame()
{
    currentFrame.inverseFrame();
}

/*
 * Flips horizontal the current sprite frame.
 */
void frame::frameFlipHorizontal()
{
    currentFrame.flipHorizontal();
}

/*
 * Flips vertical the current sprite frame.
 */
void frame::frameFlipVertical()
{
    currentFrame.flipVertical();
}



/*
 * Choose the tool that we are using.
 */
void frame::userSelectTool(Tool input)
{
    //Let frame remembers the current tool
    //so when we switch sprite frames, we still
    //remembers that tool.
    curTool = input;
    if (input == pen){
        currentFrame.toolUsing(SpriteFrame::pen);
    }
    if(input == eraser){
        currentFrame.toolUsing(SpriteFrame::eraser);
    }
    if(input == mirror){
        currentFrame.toolUsing(SpriteFrame::mirror);
    }
}

/*
 * Loads the frame that we chose.
 */
void frame::loadFrame(int i)
{
    framechangeColor(curColor);
    userSelectTool(curTool);
    //If we are not clicking on the current sprite frame.
    if (currentFrame.getFrameIndex() != i)
    {
        //Saves the current sprite frames to the list
        listOfFrame[currentFrame.getFrameIndex()] = currentFrame;
        //Switch to the other sprite frame.
        currentFrame = listOfFrame[i];
        //Repaint the information.
        currentFrame.repaint();
    }
}

/*
 * Changes the color of the brush tool.
 */
void frame::framechangeColor(QColor color)
{
    curColor = color;
    currentFrame.changeColor(curColor);
}

/*
 * Saves the information of the current sprite frame.
 */
void frame::updateTheFrameList()
{
    listOfFrame[currentFrame.getFrameIndex()] = currentFrame;

}

/*
 * Changes the brush size.
 */
void frame::framechangeBrushSize(int i)
{
    currentFrame.changeBrushSize(i);
}


/*
 * Updates the preview animation playing.
 */
void frame::updatePreview(int i)
{
    //Creates a pixmap of the image
    //and send it to the UI.
    QPixmap pix;
    pix.convertFromImage(listOfFrame[i].returnImage());
    emit changeImageLabel(pix.scaled(191,191));
}

/*
 * Runs the animation.
 */
void frame::runAnimation()
{
    //If the index is valid, we continue the animation.
    if (currentFrameInAnimation < numOfCurrentFrames)
    {
        updatePreview(currentFrameInAnimation);
        currentFrameInAnimation++;
    }
    //Otherwise, go to the front of the animation.
    else
    {
        currentFrameInAnimation = 0;
        updatePreview(0);
    }
}

/*
 * Returns the number of current sprite frames.
 */
int frame::getNumOfFrames()
{
    return numOfCurrentFrames;
}

/*
 * Sets the number of current sprite frames.
 */
void frame::setCurrentNumberOfFrame(int i)
{
    numOfCurrentFrames = i;
}




