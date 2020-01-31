#pragma once
#include <QtWidgets/QMainWindow>
#include <qpainter.h>
extern const int win_width, win_height;

class rod: public QObject
{
	Q_OBJECT
public:
	friend class ball;
	friend class droppings;
	void piant(QPainter& painter);
	void setpos(int m);
	void add_length(bool positive);
private:
	int mid = win_width / 2, length = 240;
	const static int height = 25, margin = 60;

};

