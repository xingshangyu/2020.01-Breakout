#pragma once
#include <QtWidgets/QMainWindow>
#include <qpainter.h>

extern class play;
enum brick_color { blue = 1, green, yellow, orange, red };
class brick :
	public QObject
{
	Q_OBJECT
public:
	friend class ball;
	brick(int x, int y, QVector<brick>* con, brick_color col = blue)
		:mid_x(x), mid_y(y), container(con),  color(col) {};
	brick(const brick& b)
		:color(b.color), mid_x(b.mid_x), mid_y(b.mid_y), score(b.color), container(b.container) {};
	void paint(QPainter& painter);
	brick& operator=(const brick& b);
	bool is_broken() const { return broke; }
	bool in_exist() const { return exist; }
	static const int width = 60, height = 30;
private:
	brick_color color = blue;
	int mid_x, mid_y, animate_width = width, animate_height = height, score = 1;
	QVector<brick>* container;
	bool broke = false, exist = true;
public slots:
	void hit(play* p, int power = 1);
	void broken(play* p);
	void animate_size(int frame_size);
};

