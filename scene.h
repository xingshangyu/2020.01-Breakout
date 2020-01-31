#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_scene.h"
#include "play.h"
extern const int win_width, win_height;

class scene : public QMainWindow
{
	Q_OBJECT

public:
	scene(QWidget *parent = Q_NULLPTR);
	void paintEvent(QPaintEvent*);
private:
	Ui::sceneClass ui;
	play* next = 0;
};
