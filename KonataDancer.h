#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KonataDancer.h"

class KonataDancer : public QMainWindow
{
    Q_OBJECT

public:
    KonataDancer(QWidget *parent = nullptr);
    ~KonataDancer();

private:
    Ui::KonataDancerClass ui;
};
