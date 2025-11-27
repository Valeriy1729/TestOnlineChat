#include "Client.h"

Client::Client(QWidget* parent) : QWidget(parent)
{
	vlayout = new QVBoxLayout(this);
	textBrowser = new QTextBrowser(this);
	connectBtn = new QPushButton("Connect", this);
	inputLine = new QLineEdit(this);
	sendBtn = new QPushButton("Send", this);

	vlayout->addWidget(textBrowser);
	vlayout->addWidget(connectBtn);
	vlayout->addWidget(inputLine);
	vlayout->addWidget(sendBtn);

	currSocket = new QTcpSocket(this);

	connect(connectBtn, SIGNAL(clicked()), this, SLOT(slot_connect_btn_clicked()));
	connect(sendBtn, SIGNAL(clicked()), this, SLOT(slot_send_btn_clicked()));
	connect(currSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
	connect(currSocket, SIGNAL(disconnected()), currSocket, SLOT(deleteLater()));
}

Client::~Client()
{ }

void Client::slot_connect_btn_clicked()
{
	currSocket->connectToHost("127.0.0.1", 55555);
}

void Client::slot_send_btn_clicked()
{
	SendToServer(inputLine->text());		
	inputLine->setText("");
}

void Client::slotReadyRead()
{
	QDataStream in(currSocket);
	if(in.status() == QDataStream::Ok) {
		cout << "reading data..." << endl;		
		QString msg;
		in >> msg;
		textBrowser->append(msg);
	}
	else cout << "data reading error..." << endl;
}

void Client::SendToServer(QString msg)
{
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out << msg;
	currSocket->write(Data);
}


