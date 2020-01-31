#include "scene.h"
#include <QtWidgets/QApplication>
const int win_width = 1200, win_height = 900;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	scene w;
	w.show();
	return a.exec();
}
