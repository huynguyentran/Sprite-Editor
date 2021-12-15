#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QColor>
#include "frame.h"
#include <QColorDialog>
#include <QListWidgetItem>
#include <QPushButton>
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*
 * This mainwindow header file responsibilities:
 * Display the UI and information of the model to the user.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

/*
 * List of signals to communitcate with the frame model.
 */

signals:
    void changeText(QString);
    void clearImage();
    void clickedInverseImage();
    void clickedflipHorizontal();
    void clickedflipVertical();
    void clickedSetSize(int width, int height);
    void clickUndo();
    void clickRedo();
    void selectedColor(QColor);
    void toolUsing(frame::Tool);
    void clickedAddFrame();
    void chooseFrame(int i);
    void duplicateFrame();
    void removeFrame(int);
    void openSprite();
    void saveSprite();
    void loadNewSprite();
    void brushSizeChange(int);
    void openPreview();
    void clickOnionSkin();
    void updatePreviewPanel();
    void sendFPS(int);
    void updateFrameView();

private slots:
    void on_inverseButton_clicked();
    void on_flipHorizontal_clicked();
    void on_flipVertical_clicked();
    void on_mirrorBrush_clicked();
    void on_setPaintArea_clicked();
    void on_actionInverse_triggered();
    void on_actionFlip_Horizontal_triggered();
    void on_actionFlip_Vertical_triggered();
    void on_actionColor_triggered();
    void on_actionSize_triggered();
    void on_addFrameButton_clicked();
    void on_frameList_itemDoubleClicked(QListWidgetItem *item);
    void on_deleteFrameButton_clicked();
    void on_duplicateButton_clicked();
    void on_currentColor_clicked();
    void on_actionMirror_triggered();
    void on_actionAbout_triggered();
    void on_brushSizeButton_clicked();
    void on_toolColorButton_clicked();
    void on_brushButton_clicked();
    void on_clearFrameButton_clicked();
    void on_eraserButton_clicked();
    void sendFramePerSecond();
    void on_redoButton_clicked();
    void on_undoButton_clicked();
    void on_fpsSlider_valueChanged(int value);

    void on_actionSave_Sprite_triggered();

    void on_actionOpen_Sprite_triggered();

private:
    Ui::MainWindow *ui;
    frame frames;
    QColor color;
    frame loadedFrame;
    int currentBrushSize=1;
    int fps =1;
    void save();
    void load();
};
#endif // MAINWINDOW_H
