#ifndef CONSTANT_H
#define CONSTANT_H

#include "QtWidgets/QApplication"

// ¶à¹úÓï
#define MENU_CONTEXT "MENU"
#define FORM_CONTEXT "FORM"
#define MESSAGE_CONTEXT "MESSAGE"
#define trMenuString(s)         QApplication::translate(MENU_CONTEXT, (s))
#define trFormString(s)         QApplication::translate(FORM_CONTEXT, (s))
#define trMessageString(s)      QApplication::translate(MESSAGE_CONTEXT, (s))



#endif // CONSTANT_H
