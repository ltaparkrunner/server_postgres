#include "params_watch.h"

watch_t::watch_t():offset(0)
{
//    qDebug() << "watch_t::watch_t Constructor";
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

params::params(QString f_name):
    offsets({0, 0, 0, 0, 0, 0, 0, 0})
    {
//    qDebug() << "params::params Constructor start offsets.size()" << offsets.size();
    QFile file(f_name);
    if (!file.exists()) {
        qDebug() << "Файл не найден:" << f_name;
        // message window here
        return;
    }
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qDebug() << "params::params Constructor start 2";
        QTextStream in(&file);
        bool ok;
        QString line = in.readLine();
        // Проверяем, есть ли символ ':' в строке
        if (line.contains(':')) {
            // Берем всё, что ИДЕТ ПОСЛЕ первого двоеточия (индекс 1)
            // trimmed() уберет лишние пробелы по краям
            QString str = line.section(':', 1).trimmed();
            getIP(str, ip);
        }
//        qDebug() << "params::params Constructor start 3";
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            getIP(str, mask);
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            port = str.toInt(&ok);
            if(!ok) goto err1;
        }
//        qDebug() << "params::params Constructor start 4";
        for(int i = 0; i < num_ch; i++ ){
            line = in.readLine();
            if (line.contains(':')) {
                QString str = line.section(':', 1).trimmed();
                offsets[i] = str.toInt(&ok);
                if(!ok) goto err1;
            }
        }
//        qDebug() << "params::params Constructor start 5";
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.hi_alm = str.toInt(&ok);
            if(!ok) goto err1;
        }
//        qDebug() << "params::params Constructor start 6";
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.lo_alm = str.toInt(&ok);
            if(!ok) goto err1;
        }
//        qDebug() << "params::params Constructor start 7";
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.hi_wrn = str.toInt(&ok);
            if(!ok) goto err1;
        }
        line = in.readLine();
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            th.lo_wrn = str.toInt(&ok);
            if(!ok) goto err1;
        }
        line = in.readLine();
//        qDebug() << "params::params Constructor start 8";
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            if(str.contains("True")) invert = true;
            else invert = false;
        }
        line = in.readLine();
//        qDebug() << "params::params Constructor start 9";
        if (line.contains(':')) {
            QString str = line.section(':', 1).trimmed();
            if(str.contains("True")) relative = true;
            else relative = false;
        }
//        qDebug() << "params::params Constructor start 10";
        file.close();
    }
    else {
//        qDebug() << "Can't find file " << f_name << " so use default settings.\n";
        ip = QHostAddress("192.168.1.170");
        mask = QHostAddress("255.255.0.0");
        port = 503;
        offsets = {0, 0, 0, 0, 0, 0, 0, 0};
        th = {1220, 620, 1520, 500};
        invert = false;
        relative = false;
        //        std::cout << "The point get_params 08" << std::endl;
        set_params(f_name);
        //        std::cout << "The point get_params 09" << std::endl;
        set_params(f_name);
    }

    err1: qDebug() << "Convertion problem, check file: " << f_name;
    qDebug() << "params::params Constructor finish: offsets.size(): " << offsets.size();
}
// params* params::get_params(){
//     return {};
// }
void params::get_params(QString f_name){
    qDebug() << "params::get_params(QString f_name)";
    qDebug() << "get_params(QString f_name): offsets.size(): " << offsets.size();
}
void params::set_params(QString f_name){
    qDebug() << "params::set_params(QString f_name)";
    QFile file(f_name);
    if (!file.exists()) {
        qDebug() << "Файл не найден:" << f_name;
        // message window here
        return;
    }
    qDebug() << "params::set_params(QString f_name) 2";
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
//    QTextStream  file(f_name, std::ios_base::out);
//    if(file.is_open()){
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
    qDebug() << "params::set_params(QString f_name) 3";
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
//    req.resize(60);
//    qDebug() << "bool params::set_params req.size(): " << req.size();
    invert = (req[11] & 0x01) ? true : false;
    relative = (req[11] & 0x02) ? true : false;

    ip = QHostAddress((req[14]<<24)|(req[15]<<16)|(req[16]<<8)|req[17]);
//    qDebug() << "params::set_params(QByteArray & req) IP:" << ip.toString();
    mask = QHostAddress((req[18]<<24)|(req[19]<<16)|(req[20]<<8)|req[21]);
//    qDebug() << "params::set_params(QByteArray & req) MASK: " << mask.toString();

    for (int i = 0; i < 8; i++) {
        offsets[i] = (req[30 + i*2] << 8) | req[31 + i*2];
    }
//    qDebug() << "req[28] " << static_cast<uint>(req[28]) << " req[29] "<< static_cast<uint>(req[29]&0xff);
    port = (req[28] << 8) | (req[29]&0xff);
//    qDebug() << "params::set_params(QByteArray & req) PORT: " << port;

    th.hi_alm = (req[52] << 8) | req[53];
    th.lo_alm = (req[54] << 8) | req[55];
    th.hi_wrn = (req[56] << 8) | req[57];
    th.lo_wrn = (req[58] << 8) | req[59];
//    qDebug() << "bool params::set_params 4";
    return true;
}

bool params::get_params(QByteArray & resp){
//    qDebug() << "get_params(QByteArray & resp): offsets.size(): " << offsets.size();
//    qDebug() << "params::get_params(QByteArray & resp)";
    resp.resize(63);
    resp[0] = 'Q';
    resp[10] = 0xA9;

//    qDebug() << "params::get_params(QByteArray & resp)2";
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
//    qDebug() << "params::get_params(QByteArray & resp) MASK: " << mask.toString();
    resp[18] = port >> 8;
    resp[19] = port & 0xff;
//    qDebug() << "params::get_params(QByteArray & resp) PORT: " << port;
//    qDebug() << "offsets.size(): " << offsets.size() << "  resp.size(): " << resp.size();
    for (int i = 0; i < 8; i++) {
        resp[20 + i * 2] = offsets[i] >> 8;
        resp[21 + i * 2] = offsets[i] & 0xff;
    }
//    qDebug() << "params::get_params(QByteArray & resp)5";
    resp[42] = th.hi_alm >> 8;
    resp[43] = th.hi_alm & 0xff;
    resp[44] = th.lo_alm >> 8;
    resp[45] = th.lo_alm & 0xff;
    resp[46] = th.hi_wrn >> 8;
    resp[47] = th.hi_wrn & 0xff;
    resp[48] = th.lo_wrn >> 8;
    resp[49] = th.lo_wrn & 0xff;
//    qDebug() << "params::get_params(QByteArray & resp)6";
    for(int i = 50; i < 58; i++)
        resp[i] = 0;
//    qDebug() << "params::get_params(QByteArray & resp)7";
    resp[58] = 0xac;
    resp[59] = 0x59;
    resp[60] = 0;
//    qDebug() << "params::get_params(QByteArray & resp)8";
    resp[61] = 'G';
    resp[62] = 'B';
//    qDebug() << "params::get_params(QByteArray & resp)9";
//    wdata_len = 63;
//    sock_.async_write_some(buffer(resp, wdata_len),
//                           MEM_FN2(on_write, _1, _2));
    return true;
}
