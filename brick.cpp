#include "brick.h"
#include "play.h"
#include "droppings.h"
#include <qtimeline.h>
QVector<droppings*> drops;
void brick::paint(QPainter& painter)
{
	QRect rec(mid_x - animate_width / 2, mid_y - animate_height / 2, animate_width, animate_height);
	painter.drawRect(rec);
	QColor col;
	switch (color)
	{
	case blue: col = QColor(128, 255, 255); break;
	case green: col = QColor(128, 255, 128); break;
	case yellow: col = QColor(255, 255, 128); break;
	case orange: col = QColor(255, 192, 128); break;
	default: col = QColor(255, 128, 128);//red
	}
	painter.fillRect(rec, col);
}

brick& brick::operator=(const brick& b)
{
	color = b.color; mid_x = b.mid_x; mid_y = b.mid_y; score = b.score; container = b.container;
	return *this;
}

void brick::hit(play* p, int power)
{
	if (color <= power)    broken(p);
	else    color = brick_color(color - power);
}

void brick::broken(play* p)
{
	broke = true;
	p->addscore(score);
	QTimeLine* timeline = new QTimeLine(300, this);
	timeline->setEasingCurve(QEasingCurve::OutQuad);
	timeline->setFrameRange(height, 0);
	connect(timeline, &QTimeLine::frameChanged, this , &brick::animate_size);
	timeline->start();
	connect(timeline, &QTimeLine::finished, this, [=]() {
		exist = false;
		if ([=]() {
			for(auto i = container->begin(); i != container->end(); ++i)
				if(i->in_exist())    return false;
			return true;
		}())		p->cleared();
	});
	/*container->erase(this);
	if (container->isEmpty())		p->cleared();*/
	if(!ball::can_penetrate())
		switch (rand() % (40 - 5 * p->get_level()))
		{
		case 0: drops.push_back(new stretch(mid_x, mid_y, true)); break;
		case 1: drops.push_back(new stretch(mid_x, mid_y, false)); break;
		case 2: drops.push_back(new big(mid_x, mid_y, true)); break;
		case 3: drops.push_back(new big(mid_x, mid_y, false)); break;
		case 4: drops.push_back(new speed(mid_x, mid_y, true)); break;
		case 5: drops.push_back(new speed(mid_x, mid_y, false)); break;
		case 6: drops.push_back(new multiple(mid_x, mid_y)); break;
		case 7: drops.push_back(new penetrate(mid_x, mid_y));
		}
}

void brick::animate_size(int frame_size)
{
	animate_height = frame_size;
	animate_width = frame_size / 2;
}
