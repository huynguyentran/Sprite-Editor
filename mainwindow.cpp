#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QColor>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    setWindowTitle("Sprite Editor v 1.0");
    /*
     * Set up the communications between the frame class and SpriteFrame class with
     * The UI.
     */
    connect(this,&MainWindow::changeText,ui->previewPage, &QLabel::setText);
    connect(this,&MainWindow::clearImage,&frames,&frame::frameClearFrame);
    connect(this,&MainWindow::clickedInverseImage,&frames,&frame::frameInverseFrame);
    connect(this,&MainWindow::clickedflipHorizontal,&frames,&frame::frameFlipHorizontal);
    connect(this,&MainWindow::clickedflipVertical,&frames,&frame::frameFlipVertical);
    connect(this,&MainWindow::clickedSetSize,&frames,&frame::resize);
    connect(this,&MainWindow::toolUsing,&frames,&frame::userSelectTool);
    connect(this,&MainWindow::selectedColor,&frames,  &frame::framechangeColor);
    connect(this,&MainWindow::clickedAddFrame,&frames,&frame::addAFrame);
    connect(this,&MainWindow::chooseFrame,&frames,&frame::loadFrame);
    connect(this,&MainWindow::brushSizeChange,&frames, &frame::framechangeBrushSize);
    connect(this,&MainWindow::duplicateFrame,&frames,&frame::duplicateFrame);
    connect(this,&MainWindow::removeFrame,&frames,&frame::deleteCurrentFrame);
    connect(&(frames.currentFrame),&SpriteFrame::updateAfterModifying,&frames,&frame::updateTheFrameList);
    connect(this,&MainWindow::updateFrameView,&frames,&frame::updateTheFrameList);
    connect(this,&MainWindow::clickUndo, &(frames.currentFrame), &SpriteFrame::undo);
    connect(this,&MainWindow::clickRedo, &(frames.currentFrame), &SpriteFrame::redo);
    connect(this,&MainWindow::openSprite,this,&MainWindow::load);
    connect(this,&MainWindow::saveSprite,this,&MainWindow::save);
    connect(&frames, &frame::changeImageLabel, ui->previewPanel,&QLabel::setPixmap);

    //Sets the Scroll Area to be the sprite frame that we want to modify
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(&(frames.currentFrame));
    //Updates the frame list to hold the initial frame.
    ui->frameList->addItem("Frame " + QString::number(frames.currentFrame.getFrameIndex()));
    ui->fpsSlider->setRange(1,60);
    sendFramePerSecond();
    show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Sets the size of the current frames.
 */
void MainWindow::on_setPaintArea_clicked()
{
    //Get the information in the set size boxes and pass to
    //class frame to resize current frame.
    int width = ui->setXPaintArea->toPlainText().toInt();
    int height = ui->setYPaintArea->toPlainText().toInt();
    emit clickedSetSize(width,height);
    show();
    QColorDialog();
}

/*
 * Sends the signal to the frame class that
 * the user want to inverse the color.
 */
void MainWindow::on_inverseButton_clicked()
{
    emit clickedInverseImage();
}


void MainWindow::on_actionInverse_triggered()
{
    emit clickedInverseImage();
}

/*
 * Sends the signal to the frame class that
 * the user want to flip the sprite frame
 * horizontally.
 */
void MainWindow::on_flipHorizontal_clicked()
{
    emit clickedflipHorizontal();
}

void MainWindow::on_actionFlip_Horizontal_triggered()
{
    emit clickedflipHorizontal();
}


/*
 * Sends the signal to the frame class that
 * the user want to flip the sprite frame
 * vertically.
 */
void MainWindow::on_flipVertical_clicked()
{
    emit clickedflipVertical();
}


void MainWindow::on_actionFlip_Vertical_triggered()
{
    emit clickedflipVertical();
}


/*
 * Sends the signal to the frame class that
 * the user want to use mirror brush.
 */
void MainWindow::on_mirrorBrush_clicked(){
    emit toolUsing(frame::mirror);
}

void MainWindow::on_actionMirror_triggered()
{
    emit toolUsing(frame::mirror);
}

/*
 * Sends the signal to the frame class that
 * the user want to change the color. Either
 * on the color tool button or directly on the
 * color preview
 */
void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor();
    ui->currentColor->setStyleSheet("border-radius: 7px; background-color: " + color.name() + ";");
    emit selectedColor(color);
}

void MainWindow::on_currentColor_clicked()
{
    QColor color = QColorDialog::getColor();
    ui->currentColor->setStyleSheet("border-radius: 7px; background-color: " + color.name() + ";");
    emit selectedColor(color);
}


void MainWindow::on_toolColorButton_clicked()
{
    QColor color = QColorDialog::getColor();
    ui->currentColor->setStyleSheet("border-radius: 7px; background-color: " + color.name() + ";");
    emit selectedColor(color);
}



/*
 * Sends the signal to the frame class that
 * the user want to add another frame.
 */
void MainWindow::on_addFrameButton_clicked()
{
    emit clickedAddFrame();
    ui->frameList->addItem("Frame " + QString::number(frames.currentFrame.getFrameIndex()));

}

/*
 * Sends the signal to the frame class that
 * the user want to change to other sprite frame.
 */
void MainWindow::on_frameList_itemDoubleClicked(QListWidgetItem *item)
{
    int i = ui->frameList->row(item);
    emit chooseFrame(i);
}

/*
 * Sends the signal to the frame class that
 * the user want to change to change the brush size.
 */
void MainWindow::on_actionSize_triggered()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("Brush Size"),
                                 tr("Brush Size:"), 10, 0, 100, 1, &ok);
    emit brushSizeChange(i*10);
    currentBrushSize=i;
}

void MainWindow::on_brushSizeButton_clicked()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("Brush Size"),
                                 tr("Brush Size:"), currentBrushSize, 0, 10, 1, &ok);
    emit brushSizeChange(i*10);
    currentBrushSize=i;
}


/*
 * Sends the signal to the frame class that
 * the user want to change to read about the sprite editor.
 */
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox about;
    about.setText("SE v 1.0 - Made by Huy Nguyen, Steven Tasmin, Elizabeth Thomas, Loc Vinh Phan");
    about.exec();
}

/*
 * Sends the signal to the frame class that
 * the user want to change to change the preview
 * animation
 */
void MainWindow::on_fpsSlider_valueChanged(int value)
{
    if (value == 0)
    {
        ui->framePerSecLabel->setText(QString("Frames/Second: 1 fps"));
        fps = 1;
    }
    else
    {
        fps = value;
        ui->framePerSecLabel->setText(QString("Frames/Second: ").append(QString::number(fps).append(" fps")));
    }
}

/*
 * Sends the signal to the frame class that
 * the user want to change to delete current frame.
 */
void MainWindow::on_deleteFrameButton_clicked()
{
    int i = ui->frameList->currentRow();
    //Removes the frame itme in the List
    emit removeFrame(i);
    //If there is only one frame, we don't remove it.
    if (ui->frameList->count() != 1)
    {
        ui->frameList->takeItem(i);

    }
    //Updates the name of the items in the list widget.
    for (int index = i; index <  ui->frameList->count();index++)
    {
        ui->frameList->item(index)->setText("Frame " + QString::number(frames.listOfFrame[index].getFrameIndex()));
    }
}

/*
 * Sends the signal to the frame class that
 * the user want to change to duplicate current frame.
 */
void MainWindow::on_duplicateButton_clicked()
{
    emit duplicateFrame();
    ui->frameList->addItem("Frame " + QString::number(frames.currentFrame.getFrameIndex()));

}


/*
 * Sends the signal to the frame class that
 * the user want to change to use brush
 */
void MainWindow::on_brushButton_clicked()
{
    emit toolUsing(frame::pen);
}

/*
 * Sends the signal to the frame class that
 * the user want to change to clear current
 * sprite frame.
 */
void MainWindow::on_clearFrameButton_clicked()
{
    emit clearImage();
}

/*
 * Sends the signal to the frame class that
 * the user want to change to use eraser.
 */
void MainWindow::on_eraserButton_clicked()
{
    emit toolUsing(frame::eraser);
}

/*
 * Sets the playback speed of the preview anmation.
 */
void MainWindow::sendFramePerSecond(){

    int numOfFrames = frames.getNumOfFrames();
    for (int index = 0; index < numOfFrames; index++)
    {
        QTimer::singleShot(1000/fps * index, &frames, &frame::runAnimation);
    }

    QTimer::singleShot(1000/fps * (numOfFrames-1),this,&MainWindow::sendFramePerSecond);
}

/*
 * Sends the signal to the sprite frame class that
 * the user want to change to redo
 */
void MainWindow::on_redoButton_clicked()
{
    emit clickRedo();
}


/*
 * Sends the signal to the sprite frame class that
 * the user want to change to undo
 */
void MainWindow::on_undoButton_clicked()
{
    emit clickUndo();
}


/*
 * Save current frames
 */
void MainWindow::on_actionSave_Sprite_triggered()
{
    save();
}

void MainWindow::save(){
    QString data = "";
    //Iterates through every sprite frame in frame list.
    for(unsigned int i = 0; i < frames.listOfFrame.size(); i++)
    {
        int pixelSize = 10;
        //Gets the height and the width of the sprite frames.
        int width = frames.listOfFrame[i].returnImage().width()/pixelSize;
        int height = frames.listOfFrame[i].returnImage().height()/pixelSize;
        data.append(QString::number(width)).append("\n");
        data.append(QString::number(height)).append("\n");
        data.append(QString::number(i)).append("\n");
        for(int y = 0 ; y < height; y++)
        {
            for(int x = 0 ; x < width; x++)
            {
                //Saves the red green blue and alpha of each pixel
                QRgb rgbColor = QRgb(frames.listOfFrame[i].returnImage().pixel(x * pixelSize, y * pixelSize));
                QColor color = QColor(rgbColor);
                data += QString::number(color.red()) + " " + QString::number(color.green()) + " " + QString::number(color.blue()) + " " + QString::number(color.alpha()) + " ";
            }
            data += "\n";
        }
        //Seperate the sprite frames information.
        data += "end\n";
    }
    //Saves files as .ssp.
    QFileDialog *dialog = new QFileDialog;
    dialog->setDefaultSuffix("ssp");
    QString filename = dialog->getSaveFileName(this, tr("Save as"), ".ssp");
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream( &file );
        stream << data;
        file.close();;
    }
}

/*
 * Load from the save file.
 */
void MainWindow::on_actionOpen_Sprite_triggered()
{
    load();
}

void MainWindow::load()
{
    //Selects the ssp file to open.
    QString filename = QFileDialog::getOpenFileName(this, "Select a file to open", QDir::homePath());
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        //Clears the current frame class that holds sprite frames.
        for(SpriteFrame sprtf : frames.listOfFrame)
        {
            sprtf.clearFrame();
            while(ui->frameList->count() > 0)
            {
                QListWidgetItem* x = ui->frameList->takeItem(0);
                delete x;
            }
        }
        //Resets the sprite frames list.
        for (int i = 0; i < ui->frameList->count();i++)
        {
            on_deleteFrameButton_clicked();
        }
        QTextStream stream(&file);
        //Sets booleans to seperate type file.
        bool getWidth = false;
        bool getHeight = false;
        bool getCurrentFrame =false;
        int frameWidth;
        int frameHeight;
        int currentFrameIndex;
        SpriteFrame curSpriteFrame;
        int y =0;
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            //Gets width of the sprite frame.
            if (getWidth == false)
            {
                frameWidth= line.toInt();
                getWidth = true;
                continue;
            }
            //Gets height of the sprite frame.
            if (getHeight == false)
            {
                frameHeight= line.toInt();
                getHeight = true;
                SpriteFrame newFrame(frameWidth,frameHeight);
                curSpriteFrame = newFrame;
                continue;
            }
            //Gets the current frame index.
            if (getCurrentFrame == false)
            {
                currentFrameIndex= line.toInt();
                getCurrentFrame = true;
                continue;
            }
            //Gets the data.
            if(line != "end" && line != "")
            {
                //Splits the information into an array
                QStringList list = line.split(" ");
                for (unsigned int i = 0; i < list.size()-1;i+=4)
                {
                    int red = list[i].toInt();
                    int green = list[i+1].toInt();
                    int blue = list[i+2].toInt();
                    int alpha = list[i+3].toInt();
                    //Paints the sprite frame with the infomation.
                    QColor color(red, green, blue, alpha);
                    QPoint point(i*10/4,y*10);
                    curSpriteFrame.changeColor(color);
                    curSpriteFrame.drawPixel(point);
                }
                y++;
                continue;
            }
            //If meets "end", add the sprite frame in to the frame list
            //and continues.
            if (line == "end")
            {
                curSpriteFrame.setFrameIndex(currentFrameIndex);
                if (currentFrameIndex > 0)
                {
                    frames.listOfFrame.push_back(curSpriteFrame);
                }

                if (currentFrameIndex == 0)
                {
                    frames.currentFrame = curSpriteFrame;
                }
                //Resets the bool conditions.
                getWidth = false;
                getHeight = false;
                getCurrentFrame = false;
                y=0;
                frames.setCurrentNumberOfFrame(currentFrameIndex+1);
                ui->frameList->addItem("Frame " + QString::number(currentFrameIndex));
            }
            //If it is an empty line at the end, just break the loop.
            if(line =="")
            {
                break;
            }
        }
    }
}


