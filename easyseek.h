#ifndef EASYSEEK_H
#define EASYSEEK_H
#include <VLCQtWidgets/WidgetSeek.h>

class VlcWidgetSeek;

class EasySeek : public VlcWidgetSeek
{
public:
    EasySeek();
    virtual ~EasySeek() {}
    QAbstractSlider *slider;
};

#endif // EASYSEEK_H
