#include "scene.h"
#include<qpainter.h>
#include<qpixmap.h>
#include<qdialog.h>
#include<qmessagebox.h>
#include<qfile.h>
QVector<int> rankings;
scene::scene(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon(":/Resources/icon.jpg"));
	setWindowTitle("Breakout");
	setFixedSize(win_width, win_height);
	QFile rank("rankings.txt");
	rank.open(QIODevice::ReadOnly);
	while (rank.isOpen() && !rank.atEnd()) {
		auto ranking = rank.readLine();
		rankings.push_back(ranking.toInt());
	}
	if (rankings.isEmpty())    rankings = QVector<int>{ 0,0,0 };
	connect(ui.btn_about, &QPushButton::clicked, [=]() {
		QMessageBox::information(this, "About", "Developed by xsy.\nwww.github.com/xingshangyu\n2020.1.26");
		});
	connect(ui.btn_help, &QPushButton::clicked, [=]() {
		QMessageBox::information(this, "Help", 
"The goal is to break bricks as many as possible. Move your mouse to manipulate.\
 Don't let the ball fall, for you are only allowed three balls.The less time you use,\
 the higher score you will get.\nThere are some special\
 droppings in the game, each of them have a different effect, such as stretching the\
 board, altering the size and speed of the balls, producing multiple balls at a time, etc.\
 Note that the effects can be either positive or negative, but the negative ones will\
 bring with them additional points. Do achieve a balance. Press the screen to begin.\
 Make use of them and achieve a high score!");
		});
	connect(ui.btn_rankings, &QPushButton::clicked, [=]() {
		QMessageBox::information(this, "Rankings",
			QString("1:    %1    \n\n2:    %2    \n\n3:    %3"    ).arg(rankings[0]).arg(rankings[1]).arg(rankings[2]));
	});
	connect(ui.btn_start, &QPushButton::clicked, [=]() {
		hide();
		next = new play(this);
		next->setAttribute(Qt::WA_DeleteOnClose, true);
		next->show();
	});
}

void scene::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), QString(":/Resources/scene.png"));
}
