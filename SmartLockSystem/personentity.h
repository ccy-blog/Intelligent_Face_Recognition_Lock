#ifndef PERSONENTITY_H
#define PERSONENTITY_H
#include <QString>

struct PersonEntity
{
    int id;
    QString name;
public:
    PersonEntity(int i, QString n);
    PersonEntity();
    ~PersonEntity();
};

#endif // PERSONENTITY_H
