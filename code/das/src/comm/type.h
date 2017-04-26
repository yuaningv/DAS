#ifndef TYPE_H
#define TYPE_H

enum ITEMTYPE
{
    Item_None = 0,
    Item_TimeAxis = 1,
    Item_Video = 2,
    Item_Chart = 3,
    Item_Table = 4
};

// ÍÏ×§·½Ïò
enum DragDirection{
    NONE = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4,
    LEFTTOP = 5,
    LEFTBOTTOM = 6,
    RIGHTBOTTOM = 7,
    RIGHTTOP = 8
};

#define PADDING 5 // margin


#endif // TYPE_H
