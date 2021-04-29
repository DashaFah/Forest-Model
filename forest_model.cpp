#include "iostream"
#include <random>
#include <iomanip>
#include <ctime>
using namespace std;

const int SZ=10; // SZ*2 - кол-во объектов в экосистеме == кол-во ячеек в матрице
const int _pl=7;//начальное кол-во растений/травоядных/хищников в системе
const int _he=4;
const int _pr=3;
const long N=3; //кол-во итераций
int Time = 0; //кол-во итераций на данный момент

class Base{ //Базовый класс для объектов экосистемы
public:
    char Name=' '; // имя объекта, отображаемое в таблице
    int Ind=0; //идентификатор для определения типа объекта
    int Energy=0; // энергия объекта(голод)
    int DiplEnergy=0; // кол-во энергии, необходимое для размножения
    
};

Base MAP[SZ][SZ]; //таблица для визуального представления экосистемы

class Plant: public Base{ //Класс растения наследован от базового класса
public:
    int Growth; //Промежуток времени через который вырастает новое растение в экосистеме.
    Plant(){
        Growth = 3;
        Name = '*';//обозначение растения в таблице
    }
};

class Herbivore : public Base{ //Класс травоядного животного наследован от базового класса
public:
    Herbivore(){
        DiplEnergy = 30; // кол-во энергии, необходимое для размножения
        Name = 'o';
    }
};

class Predator : public Base{ //Класс хищника наследован от базового класса
public:
    Predator(){
        DiplEnergy = 40; // кол-во энергии, необходимое для размножения
        Name = '#';
    }
};

void clear(int i, int j){
    cout << "clear "; //очищение ячейки
    MAP[i][j].Ind = 0;
    MAP[i][j].Energy = 0;
    MAP[i][j].Name = ' ';
}


void Step(int s, int& i, int& j){
    cout << "step ";
    switch (s)
    {
        case 1:
            MAP[i - 1][j].Ind = MAP[i][j].Ind;  //Клонирование объекта в выбранную ячейку
            MAP[i - 1][j].Energy = MAP[i][j].Energy;
            MAP[i - 1][j].Name = MAP[i][j].Name;
            i=i-1;
            break;
        case 2:
            MAP[i][j + 1].Ind = MAP[i][j].Ind;  //Клонирование объекта в выбранную ячейку
            MAP[i][j + 1].Energy = MAP[i][j].Energy;
            MAP[i][j + 1].Name = MAP[i][j].Name;
            j=j+1;
            break;
        case 3:
            MAP[i + 1][j].Ind = MAP[i][j].Ind;  //Клонирование объекта в выбранную ячейку
            MAP[i + 1][j].Energy = MAP[i][j].Energy;
            MAP[i + 1][j].Name = MAP[i][j].Name;
            i=i+1;
            break;
        case 4:
            MAP[i][j - 1].Ind = MAP[i][j].Ind; //Клонирование объекта в выбранную ячейку
            MAP[i][j - 1].Energy = MAP[i][j].Energy;
            MAP[i][j - 1].Name = MAP[i][j].Name;
            j=j-1;
            break;
        default:
            cout << "ERROR";
            break;
    }
}

void Random(int& m, int& n){
    cout << "Random ";
    int side;
    bool k = 0;
    do {
        k = 0;
        side = rand() % 4  + 1; //рандомно выбираем сторону
        switch (side)
        {
            case 1: if (MAP[m-1][n].Ind == 0 && m-1<SZ && n <SZ){ //если выпало вверх и ячейка пуста
                Step(side, m, n); //вызов функции перемещения
                k = 1;
            }
                break;
            case 2: if (MAP[m][n+1].Ind == 0 && m<SZ && n+1 <SZ){ //если выпало направо и ячейка пуста
                Step(side, m, n); //вызов функции перемещения
                k = 1;
            }
                break;
            case 3: if (MAP[m+1][n].Ind == 0 && m+1<SZ && n <SZ){ //если выпало вниз и ячейка пуста
                Step(side, m, n); //вызов функции перемещения
                k = 1;
            }
                break;
            case 4: if (MAP[m][n-1].Ind == 0 && m<SZ && n-1 <SZ ){ //если выпало влево и ячейка пуста
                Step(side, m, n); //вызов функции перемещения
                k = 1;
            }
                break;
            default:
                cout << "ERROR";
                break;
        }
    } while (k != 1);
}



void LivePl(int m, int n){ //функция жизненного цикла растения
    cout << "Plant ";
    int DiplTime = 3; //промежуток времени через который происходит размножение в экосистеме.
    MAP[m][n].Energy = MAP[m][n].Energy - 1; //отнимает 1 энергию,
    if (MAP[m][n].Energy == 0){ //если энергии не осталось, убирает растение с поля
        MAP[m][n].Ind = 0;
        MAP[m][n].Name = ' ';
    }
    else {
        Plant Simple; //опытный образец растения для сравнения
        if (Time >= DiplTime){ //условие размножения
            Random(m, n);
            MAP[m][n].Energy = 10;
        }
    }
}

void LiveHe(int m, int n){ //функция жизненного цикла травоядного
    cout << "Herbivore ";
    int DiplTime = 5; //промежуток времени через который происходит размножение в экосистеме.
    MAP[m][n].Energy = MAP[m][n].Energy - 1; //отнимает 1 энергию,
    if (MAP[m][n].Energy == 0){ //если энергии не осталось, убирает животное с поля
        clear(m, n);
    }
    else{
        int side = 0;
        if ((n != 1) && (MAP[m-1][n].Ind == 1)){ //если верхняя клетка над объектом растение, то съесть
            MAP[m][n].Energy = MAP[m-1][n].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
            clear(m-1,n); //обнуление растения
            side = 1; //передвижение вверх
            int tempm=m; int tempn=n;//сохранение старых координат
            Step(side, m, n); //вызов функции перемещения
            clear(tempm,tempn);//удаление бывшего места обитания
            
        }
        else{
            if ((m != SZ) && (MAP[m][n+1].Ind == 1)){ //если правая клетка около объекта - растение, то съесть
                MAP[m + 1][n].Energy = MAP[m][n+1].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                clear(m, n+1); //обнуление растения
                side = 2; //передвижение вправо
                int tempm=m; int tempn=n;
                Step(side, m, n);
                clear(tempm,tempn);
            }
            else{
                if ((n != SZ) && (MAP[m+1][n].Ind == 1)){ //если нижняя клетка под объектом - растение, то съесть
                    MAP[m][n].Energy = MAP[m+1][n].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                    clear(m+1, n); //обнуление растения
                    side = 3; //передвижение вниз
                    int tempm=m; int tempn=n;
                    Step(side, m, n);
                    clear(tempm,tempn);
                    
                }
                else{
                    if ((m != 1) && (MAP[m][n-1].Ind == 1)){ //если левая клетка около объекта - растение, то съесть
                        MAP[m][n].Energy = MAP[m][n-1].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                        clear(m, n-1); //обнуление растения
                        side = 4; //передвижение влево
                        int tempm=m; int tempn=n;
                        Step(side, m, n);
                        clear(tempm,tempn);
                    }
                    else{ //если растений вокруг травоядного нет
                        int tempm=m; int tempn=n;
                        Random(m, n);
                        clear(tempm,tempn);
                    }
                }
            }
        }
        Herbivore Simple; //опытный образец травоядного для сравнения
        cout << Simple.DiplEnergy << endl;
        cout << MAP[m][n].Energy << endl;
        cout << DiplTime << endl;
        if ((MAP[m][n].Energy >= Simple.DiplEnergy) && (Time >= DiplTime)){ //условие размножения
            Random(m, n);
            MAP[m][n].Energy = 20;
        }
    }
}

void LivePr(int m, int n){ //функция жизненного цикла хищника
    cout << "Predator ";
    int DiplTime = 7;//промежуток времени через который происходит размножение в экосистеме.
    MAP[m][n].Energy = MAP[m][n].Energy - 1; //отнимает 1 энергию,
    if (MAP[m][n].Energy == 0){ //если энергии не осталось, убирает животное с поля
        clear(m, n);
    }
    else{
        int side = 0;
        if ((n != 1) && (MAP[m-1][n].Ind == 3)){ //если верхняя клетка над объектом добыча, то съесть
            MAP[m][n].Energy = MAP[m-1][n].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
            clear(m-1, n); //обнуление добычи
            side = 1; //передвижение вверх
            Step(side, m, n); //вызов функции перемещения
            int tempm=m; int tempn=n;//сохранение старых координат
            clear(tempm,tempn); //удаление бывшего места обитания
        }
        else{
            if ((m != SZ) && (MAP[m][n+1].Ind == 3)){ //если правая клетка около объекта - добыча, то съесть
                MAP[m][n].Energy = MAP[m][n+1].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                clear(m, n+1);//обнуление добычи
                side = 2; //передвижение вправо
                int tempm = m; int tempn = n;
                Step(side, m, n);
                clear(tempm,tempn);
            }
            else{
                if ((n != SZ) && (MAP[m+1][n].Ind == 3)){ //если нижняя клетка под объектом - добыча, то съесть
                    MAP[m][n].Energy = MAP[m+1][n].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                    clear(m+1, n); //обнуление добычи
                    side = 3; //передвижение вниз
                    int tempm=m; int tempn=n;
                    Step(side, m, n);
                    clear(tempm,tempn);//удаление его бывшего места обитания
                }
                else{
                    if ((m != 1) && (MAP[m][n-1].Ind == 3)){ //если левая клетка около объекта - добыча, то съесть
                        MAP[m][n].Energy = MAP[m][n-1].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                        clear(m, n-1); //обнуление добычи
                        side = 4; //передвижение влево
                        int tempm=m; int tempn=n;
                        Step(side, m, n);
                        clear(tempm,tempn);
                    }
                    else{ //если добычи вокруг хищника нет
                       int tempm=m; int tempn=n;
                        Random(m, n);
                        clear(tempm,tempn);
                    }
                }
            }
        }
        Predator Simple; //опытный образец хищника для сравнения
        cout << Simple.DiplEnergy << endl;
        cout << MAP[m][n].Energy << endl;
        cout << DiplTime << endl;
        if ((MAP[m][n].Energy >= Simple.DiplEnergy) && (Time >= DiplTime)){ //условие размножения
            Random(m, n);
            MAP[m][n].Energy = 25;
        }
    }
}

//Начальные условия
void NachUsl(){ //функция задающая начальные условия экосистеме, помещает объекты на поле
    cout << "Nachalo\n";
      for (int i = 0; i < SZ; i++){
        for (int j = 0; j < SZ; j++){
            clear(i, j);
        }
    }
    int m, n; //для задания начального кол-ва растений в экосистеме;
    int i = 0;
    do {
        m = rand() % SZ; //находим рандомные координаты для растения на карте
        n = rand() % SZ;
        
        if (MAP[m][n].Ind == 0){
            i++;
            MAP[m][n].Ind = 1; // идентификатор
            MAP[m][n].Energy = 10;  //даем энергию объекту
            MAP[m][n].Name = '*'; //отображение на экране
        }
    } while (i < _pl);
    
    i = 0;//для задания начального кол-ва травоядных в экосистеме;
    do {
        m = rand() % SZ; //находим рандомные координаты для травоядного на карте
        n = rand() % SZ;
        if (MAP[m][n].Ind == 0){
            i++;
            MAP[m][n].Ind = 2; //индефикатор
            MAP[m][n].Energy = 20;  //даем энергию объекту
            MAP[m][n].Name = 'o'; //отоброжение на экране
        }
    } while (i < _he);
    
    i = 0;//для задания начального кол-ва хищников в экосистеме;
    do {
        m = rand() % SZ; //находим рандомные координаты для хищника на карте
        n = rand() % SZ;
        if (MAP[m][n].Ind == 0){
            i++;
            MAP[m][n].Ind = 3; //индефикатор
            MAP[m][n].Energy = 25;  //даем энергию объекту
            MAP[m][n].Name = '#'; //отоброжение на экране
        }
    } while (i < _pr);
    
}

//Вывод леса
void show(){
    cout << "\nshow ";
    int c;
    cout << "FOREST" << endl; //рисуем таблицу и выводим данные
    for (int i = 0; i < SZ*2+3; i++){
        cout << char(126);
    }
    cout << endl;
    for (int i = 0; i < SZ; i++){ //заполнение таблицы - левый бок
        cout << char(91);
        for (int j = 0; j < SZ; j++){
            c = MAP[i][j].Ind;
            cout << setw(2);
            switch (c)
            {
                case 0:
                    cout << MAP[i][j].Name;
                    break;
                case 1:
                    cout << MAP[i][j].Name;
                    break;
                case 2:
                    cout << MAP[i][j].Name;
                    break;
                case 3:
                    cout << MAP[i][j].Name;
                    break;
                default:
                    cout << "ERROR";
                    break;
            }
            cout << setw(2);
        }
        cout << char(93) << endl; // - правый бок
    }
    for (int i = 0; i < SZ * 2 + 3; i++){
        cout << char(126);
    }
    cout << endl;
}

int Life(){ // Функция прохождения 1 жизненного цикла для карты
    cout << "life" << endl;
    Time = Time + 1;
    
    for (int i = 0; i < SZ; i++){  //рассматриваем все ячейки, ищем заполненные
        for (int j = 0; j < SZ; j++){
            if (MAP[i][j].Ind != 0){
                int type;
                type = MAP[i][j].Ind; //считываем идентификатор объекта
                switch (type)
                {
                    case 1: //если растение - запуск функции жизненного цикла для растения
                        LivePl(i,j);
                        break;
                    case 2: //если травоядное - запуск функции жизненного цикла для травоядного
                        LiveHe(i,j);
                        break;
                    case 3: //если хищник - запуск функции жизненного цикла хищника
                        LivePr(i,j);
                        break;
                    default: //идентификатор не опознан - ошибка
                        cout << "ERROR";
                        break;
                }
                show();
            }
        }
    }
    return Time;
}

int main()
{
    
    srand(((int)time(NULL)));
    NachUsl();
    do{
        show();
        Life();
    } while (Time<N);
    show();
    system("pause");
    return 0;
}
