#include "QSortTheStringList.h"
#include<QStringList>
int StringComPare(QString s1,QString s2)
{
    QStringList temp1 = s1.split("\\");
    QStringList temp2 = s2.split("\\");

    if(temp1.size()!=temp2.size())
    {
        return temp1.size()<temp2.size();
    }
    else
    {
        return s1.compare(s2);
    }
}
