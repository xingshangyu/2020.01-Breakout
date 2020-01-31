#include "ball.h"
#include "play.h"
#include <qpixmap.h>
#include <qtimer.h>
#include <cassert>
#define ymin (win_height - rod::margin - rod::height - r)
int ball::r = 15, ball::v = 8; bool ball::penetrate = false;

ball::ball(const rod& plank) :x(plank.mid), y(ymin)
{
	srand(time(0)); rand();
	set_random_velocity();
}

void ball::paint(QPainter& painter) const
{
	painter.drawPixmap(x - r, y - r, 2 * r, 2 * r, QPixmap(":/Resources/ball.png"));
}

bool ball::update_pos(QVector<brick>& bricks, const rod& plank, play* p)
{
	//forced rebounce
	if (x + r > win_width || x - r < 0)    vx = -vx;
	if (y - r < 100)    vy = -vy;
	if (penetrate) {
	//penetrate and destroy all bricks along the way
		for (auto i = bricks.begin(); i != bricks.end(); ++i)
			if (!i->is_broken()) {
				if (abs(y - i->mid_y) > r + brick::height / 2 || abs(x - i->mid_x) > r + brick::width / 2)    continue;
				if (x >= i->mid_x - brick::width / 2 && x <= i->mid_x + brick::width / 2
					|| y >= i->mid_y - brick::height / 2 && y <= i->mid_y + brick::height / 2 || [=]() {
						int dx1 = x - i->mid_x + brick::width / 2, dy1 = y - i->mid_y + brick::height / 2;
						int dx2 = x - i->mid_x + brick::width / 2, dy2 = y - i->mid_y - brick::height / 2;
						int dx3 = x - i->mid_x - brick::width / 2, dy3 = y - i->mid_y + brick::height / 2;
						int dx4 = x - i->mid_x - brick::width / 2, dy4 = y - i->mid_y - brick::height / 2;
						return std::min({ sqrt(dx1 * dx1), sqrt(dx2 * dx2) ,sqrt(dx3 * dx3) ,sqrt(dx4 * dx4) }) < r;
					}()) {
					i->broken(p); break;
				}
			}
	}
	else  //rebound when collide with a brick
		for (auto i = bricks.begin(); i != bricks.end(); ++i)
			if (!i->is_broken()) {
				if (abs(y - i->mid_y) > r + brick::height / 2 || abs(x - i->mid_x) > r + brick::width / 2)    continue;
				if (x >= i->mid_x - brick::width / 2 && x <= i->mid_x + brick::width / 2)    vy = -vy;
				else if (y >= i->mid_y - brick::height / 2 && y <= i->mid_y + brick::height / 2)    vx = -vx;
				else if ([=]() {
					int dx1 = x - i->mid_x + brick::width / 2, dy1 = y - i->mid_y + brick::height / 2;
					int dx2 = x - i->mid_x + brick::width / 2, dy2 = y - i->mid_y - brick::height / 2;
					int dx3 = x - i->mid_x - brick::width / 2, dy3 = y - i->mid_y + brick::height / 2;
					int dx4 = x - i->mid_x - brick::width / 2, dy4 = y - i->mid_y - brick::height / 2;
					return std::min({ sqrt(dx1 * dx1), sqrt(dx2 * dx2) ,sqrt(dx3 * dx3) ,sqrt(dx4 * dx4) }) < r;
				}())    {
					double t = double(y - i->mid_y) / (x - i->mid_x);
					double cos = (1 - t * t) / (1 + t * t), sin = 2 * t / (1 + t * t);
					int tmp = vx;
					vx = std::round(-vx * cos - vy * sin);
					vy = -tmp * sin + vy * cos;
					vy = std::round(sign(vy) * sqrt(v * v - vx * vx));
				}
				else    continue;
				i->hit(p, r / 15); break;
			}
	//interact with the board
	if (y + r > win_height - rod::margin - rod::height)
	{
		if (x >= plank.mid - plank.length / 2 - r / 1.42 && x <= plank.mid + plank.length / 2 + r / 1.42) {
			set_random_velocity();
			if (x - r < 0 && vx < 0 || x + r > win_width && vx > 0)    vx = -vx;
		}
		else    return true;
	}
	//adjust and proceed
	if (!vy)    vy = -1;
	x += vx; y += vy;	return false;
}

void ball::restate(const rod& plank)
{
	v = 8; r = 15;
	set_pos(plank); set_random_velocity();
}

void ball::change_cur_velocity(bool positive)
{
	if (positive) { vx = vx * 3 / 2; vy = vy * 3 / 2; }
	else { vx = vx * 2 / 3; vy = vy * 2 / 3; }
}

ball ball::min_adjust()
{
	return ball(x, y, vx - sign(vx), vy + sign(vy));
}

void ball::add_size(bool positive)
{
	if (positive)     r += 15;
	else    r = 8;
	QTimer::singleShot(15000, [=]() { r = 15; });
}

void ball::change_velocity(bool positive)
{
	if (positive)    v = v * 3 / 2;
	else    v = v * 2 / 3;
	QTimer::singleShot(30000, [=]() { v = 8; });
}

void ball::set_penetrate(bool yes)
{
	penetrate = yes;
	if(yes) 	QTimer::singleShot(6000, [=]() { penetrate = false; });
}

int ball::sign(double x)
{
	if (x > 0)    return 1;
	else if (x == 0)    return 0;
	else    return -1;
}

void ball::set_random_velocity()
{
	double r = ((double)rand() / RAND_MAX / 1.5 + 1 / 6) * 3.14159265358979323846;//pi/6 ~ 5pi/6
	vx = std::round(v * cos(r)); vy = -sqrt(v * v - vx * vx);
	if (!vy) { vy = -1; vx = v; }
}

void ball::set_pos(const rod& plank)
{
	x = plank.mid; y = ymin;
}
#undef ymin

balls::balls(const rod& plank)
{
	restate(plank);
}

void balls::paint(QPainter& painter) const
{
	for (auto i = bals.begin(); i != bals.end(); ++i)    i->paint(painter);
}

void balls::update_pos(QVector<brick>& bricks, const rod& plank, play* p)
{
	for (auto i = bals.begin(); i < bals.end(); ++i)
		if (i->update_pos(bricks, plank, p)) {
			i = bals.erase(i);
			if (bals.isEmpty())    p->missed();
		}
}

void balls::restate(const rod& plank)
{
	bals.clear();
	bals.push_back(ball(plank));
	bals.begin()->restate(plank);
}

void balls::change_size(bool positive)
{
	ball::add_size(positive);
}

void balls::change_velocity(bool positive)
{
	ball::change_velocity(positive);
	for (auto i = bals.begin(); i != bals.end(); ++i)    i->change_cur_velocity(positive);
}

void balls::add()
{
	bals.push_back(bals.begin()->min_adjust());
	bals.push_back(bals.rbegin()->min_adjust());
}