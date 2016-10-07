#ifndef UIGRIDLAYOUT_H
#define UIGRIDLAYOUT_H


#include "../../../common.h"
#include "../../../Containers/containers.h"
#include "uiwidget.h"
#include "uiboxlayout.h"


class UIGridLayout: public UIBoxLayout
{
private:
    int cols;
    int rows;
protected:
    void rearrange(int height, int width);
public:
    UIGridLayout(int cols, int rows): UIBoxLayout(UI_BOX_HORIZONTAL) { this->cols = cols; this->rows = rows; }

    int getCols() { return cols; }
    void setCols(int cols) { this->cols = cols; }
    int getRows() { return rows; }
    void setRows(int rows) { this->rows = rows; }
};


#endif // UIGRIDLAYOUT_H
