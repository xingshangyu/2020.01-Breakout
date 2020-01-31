#include "droppings.h"
#include "play.h"

void stretch::paint(QPainter& painter)
{
	if (posit)		painter.drawPixmap(mid_x - r, mid_y - r, 2 * r, 2 * r, QPixmap(":/Resources/stretch_1.png"));
	else    painter.drawPixmap(mid_x - r, mid_y - r, 2 * r, 2 * r, QPixmap(":/Resources/stretch_0.png"));
}

void stretch::apply(rod& plank, balls&, play* p)
{
	p->addscore(10 * !posit);
	plank.add_length(posit);
}

void big::paint(QPainter& painter)
{
	if (posit)		painter.drawPixmap(mid_x - r, mid_y - r, 2 * r, 2 * r, QPixmap(":/Resources/big_1.png"));
	else    painter.drawPixmap(mid_x - r, mid_y - r, 2 * r, 2 * r, QPixmap(":/Resources/big_0.png"));
}

void big::apply(rod&, balls& bals, play* p)
{
	p->addscore(10 * !posit);
	bals.change_size(posit);
}

void speed::paint(QPainter& painter)
{
	if (posit)		painter.drawPixmap(mid_x - r, mid_y - r, 2 * r, 2 * r, QPixmap(":/Resources/speed_1.png"));
	else    painter.drawPixmap(mid_x - r, mid_y - r, 2 * r, 2 * r, QPixmap(":/Resources/speed_0.png"));
}

void speed::apply(rod&, balls& bals, play* p)
{
	bals.change_velocity(posit);
}

bool droppings::update_pos(rod& plank, balls& bals, play* p)
{
	if (mid_y + v > win_height - rod::margin - rod::height)
	{
		if (mid_x >= plank.mid - plank.length / 2 && mid_x <= plank.mid + plank.length / 2)
			apply(plank, bals, p);
		return true;
	}
	mid_y += v;
	return false;
}

void multiple::paint(QPainter& painter)
{
	painter.drawPixmap(mid_x - r, mid_y - r, 2 * r, 2 * r, QPixmap(":/Resources/multiple.png"));
}

void multiple::apply(rod&, balls& bals, play* p)
{
	bals.add();
}

void penetrate::paint(QPainter& painter)
{
	painter.drawPixmap(mid_x - r, mid_y - r, 2 * r, 2 * r, QPixmap(":/Resources/penetrate.png"));
}

void penetrate::apply(rod&, balls& bals, play* p)
{
	bals.set_penetrate(true);
}
