#include "timestamp_readerCSV.h"

ts_readerCSV::ts_readerCSV(const QString &fn, QObject *parent)
    : ts_reader(parent)
{
    QFile file(fn);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Читаем весь контент из ресурса в QByteArray, затем в std::string
        std::string csvData = file.readAll().toStdString();

        // Создаем stringstream для rapidcsv
        std::stringstream sstream(csvData);

        qDebug() << "ts_readerCSV::ts_readerCSV fn: " << fn;
    // //    std::string filename = "../../additional/Test_1.csv";
    //     std::string filename = fn.toStdString();

        rapidcsv::SeparatorParams sepParams(';');
        rapidcsv::LabelParams labelParams(-1, -1);

        try{
            doc = new rapidcsv::Document{sstream, labelParams, sepParams};
            maxRow = doc->GetRowCount();
            std::vector<std::string> vstr = doc->GetRow<std::string>(currRow);
            for(int i=0; i<8; i++){
                curValue[i] = QString::fromStdString(vstr[i+1]);
            }
        }
        catch (const std::out_of_range& e) {
            emit msgStatusbar("File " + fn + " is in the wrong format or empty.");
        }
        catch (const std::invalid_argument& e) {
            emit msgStatusbar("Can't open file " + fn);
        }
        catch (const std::exception& e) {
            emit msgStatusbar("Can't open file " + fn);
        }
    }
}

ts_readerCSV::~ts_readerCSV(){}

int ts_readerCSV:: readString() {
    if(maxRow > currRow) currRow++;
    std::vector<std::string> vstr = doc->GetRow<std::string>(currRow);
//    curValue.clear();
    for(int i=0; i<8; i++){
        curValue[i] = (QString::fromStdString(vstr[i+1]));
    }
    emit wasChanged(curValue);
    return 0;
}

int ts_readerCSV::getMaxIndex(){
    if(doc) return doc->GetRowCount();
    return -1;
}
