#pragma once
#include <QtWidgets/QMainWindow>
#include <qpainter.h>
#include "rod.h"
#include "brick.h"
#include "ball.h"

class droppings :public QObject
{
	Q_OBJECT
public:
	virtual void paint(QPainter& painter) = 0;
	virtual void apply(rod& plank, balls& bals, play* p) = 0;
	droppings(int middle_x, int middle_y, bool positive)
		: mid_x(middle_x), mid_y(middle_y), posit(positive) {};
	bool update_pos(rod& plank, balls& bals, play* p);
protected:
	int mid_x, mid_y;
	const static int r = 30, v = 5;
	bool posit = true;
};

class stretch :public droppings
{
	Q_OBJECT
public:
	stretch(int middle_x, int middle_y, bool positive = true)
		:droppings(middle_x, middle_y, positive) {};
	void paint(QPainter& painter);
	void apply(rod& plank, balls&, play* p);
};

class big :public droppings
{
	Q_OBJECT
public:
	big(int middle_x, int middle_y, bool positive = true)
		:droppings(middle_x, middle_y, positive) {};
	void paint(QPainter& painter);
	void apply(rod&, balls& bals, play* p);
};

class speed :public droppings
{
	Q_OBJECT
public:
	speed(int middle_x, int middle_y, bool positive = true)
		:droppings(middle_x, middle_y, positive) {};
	void paint(QPainter& painter);
	void apply(rod&, balls& bals, play* p);
};

class multiple :public droppings
{
	Q_OBJECT
public:
	multiple(int middle_x, int middle_y)
		:droppings(middle_x, middle_y, true) {};
	void paint(QPainter& painter);
	void apply(rod&, balls& bals, play* p);
};

class penetrate : public droppings
{
	Q_OBJECT
public:
	penetrate(int middle_x, int middle_y)
		:droppings(middle_x, middle_y, true) {};
	void paint(QPainter& painter);
	void apply(rod&, balls& bals, play* p);
};