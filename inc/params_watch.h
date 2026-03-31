#ifndef PARAMS_WATCH_H
#define PARAMS_WATCH_H

#include <QString>
#include <QHostAddress>
#include <QDateTime>
#include <QFile>
/*
struct date_time{
    int year;
    int mon;
    int mday;
    int hour;
    int min;
    int sec;
};
*/

class watch_t{
public:
    watch_t();
    ~watch_t();

    bool set_watch(QDateTime &dt);
    bool get_watch(QDateTime &dt);
    QDateTime get_watch();
private:
//    date_time tf;
    qint64 offset;
};

const int num_ch = 8;
const QString f_name{"config.ini"};
class params {
public:
    explicit params(const QString& f_name);

    void get_params(const QString& f_name);
    void set_params(const QString& f_name);
    bool get_params(QByteArray & resp);
    bool set_params(QByteArray & req);

    void getIP(QString  str, QHostAddress& addr);

private:
    QString fn;
    QHostAddress ip;
    QHostAddress mask;
    uint port;
    std::vector<int> offsets;
    struct threshold {
        int hi_wrn;
        int lo_wrn;
        int hi_alm;
        int lo_alm;
    } th;
    bool invert;
    bool relative;
};

#endif // PARAMS_WATCH_H
