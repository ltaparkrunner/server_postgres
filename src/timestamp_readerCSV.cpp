#include "timestamp_readerCSV.h"

ts_readerCSV::ts_readerCSV(const QString &fn, QObject *parent)
    : ts_reader(parent)
{  // why doesn't work this pointer in new QTimer(/*this*/)?
//    connect(tm1, &QTimer::timeout, this, &ts_readerCSV::readString);
//    tm1->setInterval(1000);
    qDebug() << "ts_readerCSV::ts_readerCSV file:" << fn;
//    QFile file(fn);
    QFile file("../../additional/Test_1.csv");
    if (!file.exists()) {
        qDebug() << "File not found: " << fn;
        // message window here
        return;
    }
    // QIODevice::Text преобразует переводы строк (\r\n -> \n)
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл:" << file.errorString();
        return;
    }

    // 3. Используем QTextStream для удобного чтения текста
    QTextStream in(&file);

    // 4. Считываем только одну (первую) строку
    QString firstLine = in.readLine();

    if (!firstLine.isNull()) {
        qDebug() << "Первая строка:" << firstLine;
    } else {
        qDebug() << "Файл пуст.";
    }

    // Файл закроется автоматически при выходе из области видимости объекта QFile,
    // но можно закрыть и вручную:
    file.close();
    std::string filename = "../../additional/Test_1.csv";
    std::ifstream f(filename);
    if (!f.good()) {
        std::cerr << "Файл не найден или недоступен по пути: " << filename << std::endl;
        return;
    }
    f.close();
    qDebug() << "std::ifstream f(..//..//additional/Test_1.csv); successfully";
    rapidcsv::SeparatorParams sepParams(';');
    rapidcsv::LabelParams labelParams(-1, -1);
    qDebug() << "ts_readerCSV::ts_readerCSV 2";
    try{
        doc = new rapidcsv::Document{fn.toStdString(), labelParams, sepParams};
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Error: Index or column name not found: " << e.what() << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Data error: Wrong digit format: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unpredictable error: " << e.what() << std::endl;
    }
    qDebug() << "ts_readerCSV::ts_readerCSV 3, doc: " << &doc;
    maxRow = doc->GetRowCount();
    qDebug() << "ts_readerCSV::ts_readerCSV 4, maxRow: " << &maxRow << " maxRow: " << maxRow;

    std::vector<std::string> vstr = doc->GetRow<std::string>(currRow);
//    curValue.clear();
    qDebug() << "ts_readerCSV::ts_readerCSV 5";
    for(int i=0; i<8; i++){
        curValue[i] = QString::fromStdString(vstr[i+1]);
    }
}

ts_readerCSV::~ts_readerCSV(){}

int ts_readerCSV:: readString() {
    if(maxRow > currRow) currRow++;
    std::vector<std::string> vstr = doc->GetRow<std::string>(currRow);
    curValue.clear();
    for(int i=0; i<8; i++){
        curValue.append(QString::fromStdString(vstr[i+1]));
    }
    return 0;
}

int ts_readerCSV::getMaxIndex(){
    if(doc) return doc->GetRowCount();
    return -1;
}
