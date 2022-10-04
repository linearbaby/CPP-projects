#include "cars_functions.h"
#include "main.h"


int main(){
    car_db db;

    //initializing db
    init(&db);
    //считывание данных из файла
    read_data(&db, DATA_FILE);

    //запись в car последнего элемента нашей базы данных
    car c = db.car_items[db.size - 1];

    printf("\n\n\n");
    //вывод всех элементов массива
    print_all(db);
    //вывод наиболее близкого элемента к последнему (это будет последний элемент)
    print_car(db.car_items[find_nearest(db, c)]);
    //удаление элемента с неправильным индексом
    if (delete_car_from_db(&db, 12)){
        printf("element was not deleted\n");
    }
    else{
        printf("element was deleted\n");
    }
    //удаление элемента с правильным индексом (только если файл данных
    // не был скомпроментирован)
    if (delete_car_from_db(&db, db.size - 1)){
        printf("element was not deleted\n");
    }
    else{
        printf("element was deleted\n");
    }

    //обновление данных
    read_data(&db, DATA_FILE);
    c = db.car_items[db.size - 1];
    //добавим элемент в конец
    append(&db, c);
    print_all(db);
    //удалим 2 последних элемента
    delete_car_from_db(&db, db.size - 1);
    delete_car_from_db(&db, db.size - 1);
    //покажем, что последний элемент изменился
    printf("last element have been changed: \n");
    print_car(db.car_items[db.size - 1]);

    //найдем наиболее похожий элемент на случайный
    c = db.car_items[db.size - 1];
    c.speed = 10;
    c.fuel_consum = 124;
    c.enginev = 89;

    printf("\n\n Mostly same elements:\n");
    print_car(c);
    print_car(db.car_items[find_nearest(db, c)]);

    //запишем данные обратно в файл данных
    read_data(&db, DATA_FILE);
    write_data(&db, DATA_FILE);
    //завершим программу
    _clear(&db);
    return 0;
}
