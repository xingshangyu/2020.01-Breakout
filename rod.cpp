#include "rod.h"
#include <qtimer.h>
void rod::piant(QPainter& painter)
{
	int left = mid - length / 2, right = mid + length / 2;
	painter.save();
	painter.setBrush(QBrush(QColor(255, 0, 0)));
	painter.drawEllipse(left, win_height - height - margin, height, height);
	painter.drawEllipse(right - height, win_height - height - margin, height, height);
	painter.setBrush(QBrush(QColor(0, 0, 130)));
	painter.drawRect(left + height / 2, win_height - height - margin, length - height , height);
	painter.restore();
}

void rod::setpos(int m)
{
	if (m - length / 2 < 0)    mid = length / 2;
	else if (m + length / 2 > win_width)    mid = win_width - length / 2;
	else    mid = m;
}

void rod::add_length(bool positive)
{
	if (length > 120 && length < 600)
	{
		if (positive) {
			length += 60;
			QTimer::singleShot(60000, [=]() { length -= 60; });
		}
		else {
			length -= 60;
			QTimer::singleShot(60000, [=]() { length += 60; });
		}
	}
}
