#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>


struct Message {
	QString text;
	QString userName;
	QString time;
	QString formated();
};


#endif
