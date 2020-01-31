#pragma once
#include <QtWidgets/QMainWindow>
#include "rod.h"
#include "brick.h"
#include "ball.h"
extern const int win_width, win_height;
class play :
	public QMainWindow
{
	Q_OBJECT
public:
	play(QWidget* parent = Q_NULLPTR);
	void paintEvent(QPaintEvent*);
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	inline void addscore(int amount) { score += amount; }
	inline int get_level() const { return level; }
private:
	rod plank;
	balls bals;
	int level = 1, score = 0, balls_remain = 3;
	QTimer* refresh, * count_time;
public slots:
	void missed();
	void end();
	void cleared();
};

