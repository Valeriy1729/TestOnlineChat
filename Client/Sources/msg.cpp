#include "msg.h"

QString Message::formated()
{
	QString sp {QString(" ")};
	return time + sp + userName + sp + sp + sp + text;
}
