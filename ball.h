#pragma once
#include <QtWidgets/QMainWindow>
#include "rod.h"
#include "brick.h"

extern class play;

class ball :
	public QObject
{
	Q_OBJECT

public:
	ball(const rod& plank);
	ball(const ball& b) :x(b.x), y(b.y), vx(b.vx), vy(b.vy) {};
	void paint(QPainter& painter) const;
	bool update_pos(QVector<brick>& bricks, const rod& plank, play* p);
	void restate(const rod& plank);
	void change_cur_velocity(bool positive);
	ball min_adjust();
	static bool can_penetrate() { return penetrate; }
	static void add_size(bool positive);
	static void change_velocity(bool positive);
	static void set_penetrate(bool yes);
	static int sign(double x);
private:
	static int r, v;
	int x, y, vx, vy;//v: the number of pixel moving once update
	static bool penetrate;
	void set_random_velocity();
	void set_pos(const rod& plank);
	ball(int i, int j, int vi, int vj) :x(i), y(j), vx(vi), vy(vj) {};
};

class balls:public QObject
{
	Q_OBJECT

public:
	balls(const rod& plank);
	void paint(QPainter& painter) const;
	void update_pos(QVector<brick>& bricks, const rod& plank, play* p);
	void restate(const rod& plank);
	void change_size(bool positive);
	void change_velocity(bool positive);
	void add();
	void set_penetrate(bool yes) { ball::set_penetrate(yes); }
	void clear() { bals.clear(); }
private:
	QVector<ball> bals;
};