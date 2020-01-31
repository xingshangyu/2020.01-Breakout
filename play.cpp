#include "play.h"
#include <qevent.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qfile.h>
#include <qmessagebox.h>
#include "droppings.h"
extern QVector<droppings*> drops;
static QVector<QVector<brick>> bricks;
extern QVector<int> rankings;

play::play(QWidget* parent)
	: QMainWindow(parent), bals(plank)
{
	setFixedSize(win_width, win_height);
	setWindowIcon(QIcon(":/Resources/icon.jpg"));
	setWindowTitle("Breakout");
	setMouseTracking(true);
	for (int i = 0; i < 5; ++i)		bricks.push_back(QVector<brick>());
	//level 1
	for (int i = 0; i < 20; ++i) {
		bricks[0].push_back(brick((i + 0.5) * brick::width, 250, bricks.begin()));
		bricks[0].push_back(brick((i + 0.5) * brick::width, 250 + 2 * brick::width, bricks.begin()));
	}
	//level 2
	for (int i = 5; i < 15; ++i) {
		bricks[1].push_back(brick((i + 0.5) * brick::width, 250, bricks.begin() + 1, green));
		bricks[1].push_back(brick((i + 0.5) * brick::width,
			250 + std::min(i - 4, 15 - i) * brick::height, bricks.begin() + 1));
	}
	//level 3
	for (int i = 1; i < 8; ++i) {
		if (i == 4)    continue;
		bricks[2].push_back(brick((i + 0.5) * brick::width, 250 + std::min(i, 8 - i) * brick::height, bricks.begin() + 2, green));
		bricks[2].push_back(brick((i + 0.5) * brick::width, 250 - std::min(i, 8 - i) * brick::height, bricks.begin() + 2, green));
	}
	for (int i = 12; i < 19; ++i) {
		if (i == 15)    continue;
		bricks[2].push_back(brick((i + 0.5) * brick::width, 250 + std::min(i - 11, 19 - i) * brick::height, bricks.begin() + 2, green));
		bricks[2].push_back(brick((i + 0.5) * brick::width, 250 - std::min(i - 11, 19 - i) * brick::height, bricks.begin() + 2, green));
	}
	bricks[2].push_back(brick((0 + 0.5) * brick::width, 250, bricks.begin() + 2));
	bricks[2].push_back(brick((19 + 0.5) * brick::width, 250, bricks.begin() + 2));
	bricks[2].push_back(brick((8 + 0.5) * brick::width, 250, bricks.begin() + 2));
	bricks[2].push_back(brick((11 + 0.5) * brick::width, 250, bricks.begin() + 2));
	bricks[2].push_back(brick((4 + 0.5) * brick::width, 250, bricks.begin() + 2, yellow));
	bricks[2].push_back(brick((15 + 0.5) * brick::width, 250, bricks.begin() + 2, yellow));
	bricks[2].push_back(brick((4 + 0.5) * brick::width, 250 + 4 * brick::height, bricks.begin() + 2));
	bricks[2].push_back(brick((4 + 0.5) * brick::width, 250 - 4 * brick::height, bricks.begin() + 2));
	bricks[2].push_back(brick((15 + 0.5) * brick::width, 250 + 4 * brick::height, bricks.begin() + 2));
	bricks[2].push_back(brick((15 + 0.5) * brick::width, 250 - 4 * brick::height, bricks.begin() + 2));
	//level 4
	for (int i = 3; i < 17; ++i) {
		bricks[3].push_back(brick((i + 0.5) * brick::width, 200, bricks.begin() + 3, orange));
		bricks[3].push_back(brick((i + 0.5) * brick::width, 200 + brick::height, bricks.begin() + 3, yellow));
		bricks[3].push_back(brick((i + 0.5) * brick::width, 200 + 2 * brick::height, bricks.begin() + 3, yellow));
		bricks[3].push_back(brick((i + 0.5) * brick::width, 200 + 3 * brick::height, bricks.begin() + 3, green));
		bricks[3].push_back(brick((i + 0.5) * brick::width, 200 + 4 * brick::height, bricks.begin() + 3, green));
		bricks[3].push_back(brick((i + 0.5) * brick::width, 200 + 5 * brick::height, bricks.begin() + 3));
	}
	//level 5
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j <= i; ++j)
			bricks[4].push_back(brick((9.5 - 0.5 * i + j) * brick::width,
				200 + i * brick::height, bricks.begin() + 4, brick_color(5 - i / 2)));
	//refresh
	refresh = new QTimer(this);
	connect(refresh, &QTimer::timeout, [=]() {
		bals.update_pos(bricks[level - 1], plank, this);
		for (auto i = drops.begin(); i < drops.end(); ++i)
			if ((*i)->update_pos(plank, bals, this)) {
				delete* i; i = drops.erase(i);
			}
		update();
		});
	count_time = new QTimer(this);
	connect(count_time, &QTimer::timeout, [=]() { score -= 1; });
}

void play::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, win_width, win_height, QString(":/Resources/background_%1.png").arg(level - 1));
	painter.drawLine(0, 100, win_width, 100);
	QFont font;
	font.setPixelSize(40);
	painter.setFont(font);
	for (int i = 1; i < balls_remain; ++i)
		painter.drawPixmap(20 + 30 * i, 60, QPixmap(":/Resources/ball.png"));
	painter.drawText(515, 90, QString("LEVEL %1").arg(level));
	painter.drawText(990, 90, QString("score: %1").arg(score));
	painter.setRenderHint(QPainter::Antialiasing);
	plank.piant(painter);
	for (auto i = bricks[level - 1].begin(); i != bricks[level - 1].end(); ++i)
		if(i->in_exist())    i->paint(painter);
	bals.paint(painter);
	for (auto i = drops.begin(); i != drops.end(); ++i)		(*i)->paint(painter);
}

void play::mouseMoveEvent(QMouseEvent* e)
{
	plank.setpos(e->x());
}

void play::mousePressEvent(QMouseEvent* e)
{
	if(!refresh->isActive())
	{
		refresh->start(10); count_time->start(10000);
	}
}

void play::cleared()
{
	refresh->stop(); count_time->stop();
	drops.clear();
	score += 20;
	if (level >= bricks.size())
	{
		QMessageBox::information(this, "Success", "All levels cleared! Additional 50 points added.\nCongratulations!");
		score += 30;
		end();
	}
	else
	{
		bals.clear();
		QMessageBox::information(this, "Success", QString("Level %1 cleared! Additional 20 points added.").arg(level++));
		bals.restate(plank);
		bals.set_penetrate(false);
	}
}
void play::missed()
{
	refresh->stop(); count_time->stop();
	drops.clear();
	--balls_remain;
	if (balls_remain) {
		QMessageBox::information(this, "Missed", QString("You missed the ball.\nBalls remaining: %1").arg(balls_remain));
		bals.restate(plank);
	}
	else    end();
}

void play::end()
{
	refresh->stop(); count_time->stop();
	bals.restate(plank);
	QString new_record = "";
	if (score > rankings[0])
	{
		rankings[2] = rankings[1];
		rankings[1] = rankings[0];
		rankings[0] = score;
		new_record = "New record! Congratulations!\n";
	}
	else if (score > rankings[1])
	{
		rankings[2] = rankings[1];
		rankings[1] = score;
	}
	else if (score > rankings[2])    rankings[2] = score;
	QFile ranking("rankings.txt");
	ranking.open(QIODevice::WriteOnly);
	ranking.write(QString("%1\n%2\n%3").arg(rankings[0]).arg(rankings[1]).arg(rankings[2]).toStdString().c_str());
	QMessageBox::information(this, "Game Over", QString("Game Over!\n\%1Your score is %2.").arg(new_record).arg(score));
	close(); parentWidget()->show();
	bricks.clear();
}