#include "Bulk.h"

#include "version.h"

int version() {
    return PROJECT_VERSION_PATCH;
}

// время
string Bulk::getTime() {
    using namespace std::chrono;
    auto cur_time = system_clock::now();
    auto cur_epoch = duration_cast<seconds>(cur_time.time_since_epoch());
    return to_string(cur_epoch.count());
}


// цикл обработки команд
void Bulk::processLoop()
{
    string cmd;
    State state = State::Unset;

    while( cin >> cmd ) {

        // определяем очередной тип блока
        if (state == State::Unset) {

            if (cmd != "{") {
                state = staticBlock(cmd);
            }
            else{
                d_counter = 1;
                state = dynamicBlock(cmd);
            }
        }
        // продолжаем обрабатывать динамический блок
        else if (state == State::Dynamic) {
            state = dynamicBlock(cmd);
        }
        // продолжаем обрабатывать статический блок
        else if (state == State::Static) {
            state = staticBlock(cmd);
        }
    }

    // для статистического блока при конце ввода завершаем принудительно
    if (state == State::Static) {
        writeFile();
    }

}

// очередной символ обрабатывается в рамках статического блока
State Bulk::staticBlock( string cmd )
{
    // завершаем принудительно по нахождению { - следующий блок : динамический 
    if (cmd == "{") {
        writeFile();
        d_counter = 1;
        return State::Dynamic;
    }
    // } игнорируем
    else if (cmd == "}") {
        return State::Static;
    }

    // очередная команда
    commands.push_back(cmd);
    // первая команда
    if (commands.size()  == 1) {
        setFileName();
    }
    // завершаем планово по нахождению всех N команд - какой будет следующий блок : неизвестно
    else if( commands.size() == sz) {
        writeFile();
        return State::Unset;
    }
    return State::Static;
    
}

// очередной символ обрабатывается в рамках динамического блока
State Bulk::dynamicBlock( string cmd )
{
    // {
    if (cmd == "{") {
        d_counter++;
        return State::Dynamic;
    }
    // }
    else if (cmd == "}") {
        d_counter--;
        // завершеаем планово по нахождению нужного количества }
        if (d_counter == 0) {
            writeFile();
            return State::Unset;
        }
        // продолжаем
        else {
            return State::Dynamic;
        }
    }

    // очередная команда
    commands.push_back(cmd);
    // первая команда
    if (commands.size() == 1) {
        setFileName();
    }
    // продолжаем
    return State::Dynamic;
}

// имя файла
void Bulk::setFileName()
{
    if (fileName.empty()) {
        fileName = "bulk" + getTime() + ".log";
    }
}

// создание и запись в файл
void Bulk::writeFile()
{
    if (!commands.size()) return;

    string res;
    for (int i = 0; i < commands.size(); i++ ) {
        res += commands[i];
        if (i < commands.size() - 1)
            res += ", ";
    }

    // запись в консоль
    cout << "bulk: " << res << endl;

    // запись в файл
    ofstream file = ofstream(fileName, ios::app );
    if (!file.is_open()) {
        throw std::runtime_error("Error open file\n");
    }
    file << res << endl;
    file.close();

    commands.clear();
    fileName.clear();
}
