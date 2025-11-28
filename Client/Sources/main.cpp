#include "Client.h"
#include <QApplication>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	Client* client {new Client};
	client->show();

	app.exec();
	delete client;
	return 0;
}
