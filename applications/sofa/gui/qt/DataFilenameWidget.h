#ifndef SOFA_GUI_QT_DATAFILENAMEWIDGET_H
#define SOFA_GUI_QT_DATAFILENAMEWIDGET_H

#ifdef SOFA_QT_4
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#else
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#endif

#include "DataWidget.h"


namespace sofa
{
namespace gui
{
namespace qt
{

class DataFileNameWidget : public TDataWidget<std::string>
{
    Q_OBJECT
public:

    DataFileNameWidget(
        QWidget* parent,
        const char* name,
        core::objectmodel::Data<std::string>* data):
        TDataWidget<std::string>(parent,name,data) {};

    virtual bool createWidgets();
protected:
    ///Implements how update the widgets knowing the data value.
    virtual void readFromData();
    ///Implements how to update the data, knowing the widget value.
    virtual void writeToData();

    QLineEdit* openFilePath;
    QPushButton* openFileButton;

protected slots :
    virtual void raiseDialog();
};

//      class DataDirectoryWidget : public DataFileNameWidget
//      {
//        Q_OBJECT
//      public:
//        DataDirectoryWidget(QWidget* parent,
//                            const char* name,
//                            core::objectmodel::TData<std::string>* data)
//        :DataFileNameWidget(parent,name,data)
//        {}

//      protected:
//        virtual void readFromData();
//        virtual void writeToData();

//     protected slots:
//        virtual void raiseDialog();

//      };

}
}
}

#endif //SOFA_GUI_QT_DATAFILENAMEWIDGET_H



