#include "params_watch.h"

watch_t::watch_t():offset(0)
{

}
watch_t::~watch_t(){}
bool watch_t::set_watch(QDateTime &dt){
    QDateTime now = QDateTime::currentDateTime();
    offset = now.secsTo(dt);
    return true;
}
bool watch_t::get_watch(QDateTime &dt){
    QDateTime now = QDateTime::currentDateTime();
    dt = now.addSecs(offset);
    return true;
}

QDateTime watch_t::get_watch(){
    return QDateTime::currentDateTime().addSecs(offset);
}

params::params(const QString& f_name):
    fn(f_name)
,    offsets({0, 0, 0, 0, 0, 0, 0, 0})
{
    QFile file(f_name);
    if (!file.exists()) {
        qDebug() << "File not found:" << f_name;
        // message window here
        return;
    }
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        bool ok;
        QString line = in.readLine();
        // Проверяем, есть ли символ ':' в строке
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            getIP(str, ip);
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            getIP(str, mask);
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            port = str.toInt(&ok);
            if(!ok) {
                goto err1;
            }
        }
        for(int i = 0; i < num_ch; i++ ){
            line = in.readLine();
            if (line.contains(':')) {
                QString str = line.section(':', 1).trimmed();
                {
                    QStringList str2 = str.split(',');
                    offsets[i] = str2[0].toInt(&ok);
                    if (str2.size() > 1 && !str2.at(1).isEmpty()) {
                        offsets[i] += 1;
                    }
                }
                if(!ok) {
                    goto err1;
                }
            }
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.hi_alm = str.toInt(&ok);
            if(!ok) {
                goto err1;
            }
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.lo_alm = str.toInt(&ok);
            if(!ok) {
                goto err1;
            }
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.hi_wrn = str.toInt(&ok);
            if(!ok) {
                goto err1;
            }
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.lo_wrn = str.toInt(&ok);
            if(!ok) {
                goto err1;
            }
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            if(str.contains("True")) invert = true;
            else invert = false;
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            if(str.contains("True")) relative = true;
            else relative = false;
        }
        file.close();
    }
    else {
        ip = QHostAddress("192.168.1.170");
        mask = QHostAddress("255.255.0.0");
        port = 503;
        offsets = {0, 0, 0, 0, 0, 0, 0, 0};
        th = {1220, 620, 1520, 500};
        invert = false;
        relative = false;

        set_params(f_name);
    }
    return;
    err1: qDebug() << "Convertion problem, check file: " << f_name;
}

void params::get_params(const QString& f_name){
    QFile file(f_name);
    if (!file.exists()) {
        qDebug() << "File not found:" << f_name;
        // message window here
        return;
    }
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        bool ok;
        QString line = in.readLine();
        // Проверяем, есть ли символ ':' в строке
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            getIP(str, ip);
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            getIP(str, mask);
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            port = str.toInt(&ok);
            if(!ok) {
                goto err2;
            }
        }
        for(int i = 0; i < num_ch; i++ ){
            line = in.readLine();
            if (line.contains(':')) {
                QString str = line.section(':', 1).trimmed();
                {
                    QStringList str2 = str.split(',');
                    offsets[i] = str2[0].toInt(&ok);
                    if (str2.size() > 1 && !str2.at(1).isEmpty()) {
                        offsets[i] += 1;
                    }
                }
                if(!ok) {
                    goto err2;
                }
            }
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.hi_alm = str.toInt(&ok);
            if(!ok) {
                goto err2;
            }
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.lo_alm = str.toInt(&ok);
            if(!ok) {
                goto err2;
            }
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.hi_wrn = str.toInt(&ok);
            if(!ok) {
                goto err2;
            }
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.lo_wrn = str.toInt(&ok);
            if(!ok) {
                goto err2;
            }
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            if(str.contains("True")) invert = true;
            else invert = false;
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            if(str.contains("True")) relative = true;
            else relative = false;
        }
        file.close();
    }
    return;
    err2: qDebug() << "Convertion problem, check file: " << f_name;
}

void params::set_params(const QString& f_name){
    QFile file(f_name);
    if (!file.exists()) {
        qDebug() << "File not found:" << f_name;
        // message window here
        return;
    }
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "IP-адрес ПТК КМ-Дельта:" << ip.toString()<< Qt::endl;
        out << "Маска подсети:" << mask.toString()<< Qt::endl;
        out << "Порт ПТК КМ-Дельта:" << port<< Qt::endl;
        for(int i = 0; i < num_ch; i++ ){
            out << "Смещение канала " << i <<", мкм:"<< offsets[i] << Qt::endl;
        }
        out << "Реле 1 верхний порог:" << th.hi_wrn << Qt::endl;
        out << "Реле 1 нижний порог:" << th.lo_wrn << Qt::endl;
        out << "Реле 2 верхний порог:" << th.hi_alm << Qt::endl;
        out << "Реле 2 нижний порог:" << th.hi_wrn << Qt::endl;
        out << "Инверсия данных:" << (invert ? "True" : "False") << Qt::endl;
        out << "Относительные данные:" << (relative ? "True" : "False") << Qt::endl;
        file.close();
    }
    return;
}
void params::getIP(QString str, QHostAddress& addr){
    addr = QHostAddress(str);
    if(addr.protocol() == QAbstractSocket::UnknownNetworkLayerProtocol){
        addr = QHostAddress("127.0.0.1");
    }
    return;
}
bool params::set_params(QByteArray & req) {

    invert = (req[11] & 0x01) ? true : false;
    relative = (req[11] & 0x02) ? true : false;

    ip = QHostAddress((req[14]<<24)|(req[15]<<16)|(req[16]<<8)|req[17]);
    mask = QHostAddress((req[18]<<24)|(req[19]<<16)|(req[20]<<8)|req[21]);

    for (int i = 0; i < 8; i++) {
        offsets[i] = (req[30 + i*2] << 8) | req[31 + i*2];
    }
    port = (req[28] << 8) | (req[29]&0xff);

    th.hi_alm = (req[52] << 8) | req[53];
    th.lo_alm = (req[54] << 8) | req[55];
    th.hi_wrn = (req[56] << 8) | req[57];
    th.lo_wrn = (req[58] << 8) | req[59];
    set_params(fn);
    return true;
}

bool params::get_params(QByteArray & resp){
    resp.resize(63);
    resp[0] = 'Q';
    resp[10] = 0xA9;

    if (invert) resp[1] = 0x01;
    else resp[1] = 0x00;
    if (relative) resp[1] |= 0x02;
    resp[3] = 100;
    if(ip.protocol() == QAbstractSocket::IPv4Protocol){
        quint32 ip4raw = ip.toIPv4Address();
        resp[4] = (ip4raw>>24) & 0xff;
        resp[5] = (ip4raw>>16) & 0xff;
        resp[6] = (ip4raw>>8) & 0xff;
        resp[7] = ip4raw & 0xff;
    }
//    qDebug() << "params::get_params(QByteArray & resp) IP: " << ip.toString();
    if(mask.protocol() == QAbstractSocket::IPv4Protocol){
        quint32 ip4raw = mask.toIPv4Address();
        resp[8] = (ip4raw>>24) & 0xff;
        resp[9] = (ip4raw>>16) & 0xff;
        resp[10] = (ip4raw>>8) & 0xff;
        resp[11] = ip4raw & 0xff;
    }

    resp[18] = port >> 8;
    resp[19] = port & 0xff;

    for (int i = 0; i < 8; i++) {
        resp[20 + i * 2] = offsets[i] >> 8;
        resp[21 + i * 2] = offsets[i] & 0xff;
    }

    resp[42] = th.hi_alm >> 8;
    resp[43] = th.hi_alm & 0xff;
    resp[44] = th.lo_alm >> 8;
    resp[45] = th.lo_alm & 0xff;
    resp[46] = th.hi_wrn >> 8;
    resp[47] = th.hi_wrn & 0xff;
    resp[48] = th.lo_wrn >> 8;
    resp[49] = th.lo_wrn & 0xff;

    for(int i = 50; i < 58; i++)
        resp[i] = 0;
    resp[58] = 0xac;
    resp[59] = 0x59;
    resp[60] = 0;

    resp[61] = 'G';
    resp[62] = 'B';

    return true;
}
