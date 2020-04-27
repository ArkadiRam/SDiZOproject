/** autor: Arkadzi Zaleuski 250929 czw.TN */
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
#include <chrono>
#include <string>
#include <vector>

using namespace std;

static void getChoice(int *choice) {
    int data;
    while (true) {
        cin >> *choice;
        if (cin.fail()) {
            cout << "Wrong input try again\n";
            cin.clear();
            cin.ignore();
        } else {
            return;
        }
    }
}

class Reader {
public:
    ifstream myfile;
    int size = 0;

    void setPath() {
        string path;
        cout << "Podaj nazwe pliku\n";
        cin >> path;
        myfile.open(path, ios::in);
        myfile >> size;
    }

    int next() {
        int num;
        myfile >> num;
        return num;
    }
};

class Printer {

public:
    void setPath() {
        string path;
        cout << "Podaj nazwe pliku\n";
        cin >> path;
        myfile.open(path);
    }

    ofstream myfile;
};

class Tablica {
public:
    int *array = new int[0];
    int size = 0;
    string menu = "1.Wygeneruj tablice\n"
                  "2.Usun element na pozycji\n"
                  "3.Dodaj element\n"
                  "4.Wyswietl tablice\n"
                  "5.Wczytaj z pliku\n"
                  "6.Odszukaj wartosc\n"
                  "7.Pomiar czasu\n"
                  "0. Powrot do menu\n"
                  "Twoj wybor: ";

    void showTableMenu() {
        int k;
        while (true) {
            cout << menu;
            getChoice(&k);
            switch (k) {
                case 1: {
                    cout << "Wprowadz rozmiar\n";
                    getChoice(&size);
                    this->generate(size);
                    this->print();
                    break;
                }
                case 2: {
                    cout << "Wprowadz index pozycji: 0 -" << size - 1 << endl;
                    int pos;
                    getChoice(&pos);
                    if (pos >= 0 && pos < size)
                        remove(pos);
                    this->print();
                    break;
                }
                case 3: {
                    string way = "Podaj sposub w ktory chcesz dodac:\n"
                                 "1.Na pocz?tku\n"
                                 "2.Na koncu\n"
                                 "3.W losowym miejscu\n"
                                 "4.We wskazanym miejscu\n"
                                 "Twoj wybor: ";
                    cout << way;
                    int choice;
                    getChoice(&choice);
                    if (choice < 1 || choice > 4)
                        cout << "Niema takiej opcji\n";
                    else {
                        cout << "Wprowadz wartosc\n";
                        int val;
                        getChoice(&val);
                        if (choice == 1)
                            this->addBegining(val);
                        else if (choice == 2)
                            this->addEnding(val);
                        else if (choice == 3)
                            this->addRandom(val);
                        else if (choice == 4) {
                            cout << "Wprowadz index\n";
                            int pos;
                            getChoice(&pos);
                            if (pos > 0 && pos < this->size)
                                addValue(pos, val);
                        }
                    }
                    this->print();
                    break;
                }

                case 4: {
                    this->print();
                    break;
                }
                case 5: {
                    array = new int[1];
                    Reader reader;
                    reader.setPath();
                    for (int i = 0; i < reader.size; i++) {
                        addEnding(reader.next());
                    }
                    this->print();
                    reader.myfile.close();
                    break;
                }
                case 6: {
                    cout << "Wprowadz wartosc\n";
                    int val;
                    getChoice(&val);
                    int i = findVal(val);
                    if (findVal(val) != -1)
                        cout << "Pierwszy spotkany element o podanym kluczu znajduje sie na pozycji " << i << endl;
                    else cout << "Niema takiej wartosci\n";
                    this->print();

                    break;
                }
                case 7: {
                    measureTime();
                    break;
                }
                case 0: {
                    free(array);   //clearing the array
                    return;
                }
                default: {
                    cout << "No such option. Try again\n";
                    break;
                }
            }
        }
    }

    void measureTime() {
        Tablica tab;
        chrono::high_resolution_clock::time_point end;
        chrono::high_resolution_clock::time_point start;
        Printer printer;
        printer.setPath();
        printer.myfile << "Rozmiar struktury(Tablica): 1000000, 2000000,3000000,4000000,5000000" << endl;
        //mierzymy czas dodawania;
        printer.myfile << "Czas wstawienia: ";
        for (int i = 1; i < 6; i++) {
            tab.generate(1000000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                tab.addValue(rand() % (size - 1), rand() % INT32_MAX);
            }
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 100 << " ";
            array = (int *) realloc(array, sizeof(int));   //clearing the array
        }

        printer.myfile << endl;
        printer.myfile << "Czas wstawienia poczaktu: ";
        //mierzymy czas usuwania
        for (int i = 1; i < 6; i++) {
            tab.generate(1000000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                tab.addBegining(rand() % INT32_MAX);
            }
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 100 << " ";
            array = (int *) realloc(array, sizeof(int));   //clearing the array
        }

        printer.myfile << endl;
        printer.myfile << "Czas wstawienia konca: ";
        //mierzymy czas usuwania
        for (int i = 1; i < 6; i++) {
            tab.generate(1000000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                tab.addEnding(rand() % INT32_MAX);
            }
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 100 << " ";
            array = (int *) realloc(array, sizeof(int));   //clearing the array
        }

        printer.myfile << endl;
        printer.myfile << "Czas usuwania: ";
        //mierzymy czas usuwania
        for (int i = 1; i < 6; i++) {
            tab.generate(1000000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                tab.remove(rand() % INT32_MAX);
            }
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 100 << " ";
            array = (int *) realloc(array, sizeof(int));   //clearing the array
        }

        //mierzymy czas wyszukiwania
        printer.myfile << endl;
        printer.myfile << "Czas wyszukiwania: ";
        for (int i = 1; i < 6; i++) {
            tab.generate(1000000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100000; i++)
                tab.findVal(rand() % INT32_MAX);
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 100000 << " ";
            array = (int *) realloc(array, sizeof(int));   //clearing the array
        }

        printer.myfile.close();
    }

    void addEnding(int k) {
        //add time spent func
        array[size] = k;
        size++;
        array = (int *) realloc(array, sizeof(int) * (size + 1));
    }

    void addBegining(int k) {
        //add time spent function
        size++;
        array = (int *) realloc(array, sizeof(int) * (size));
        for (int i = size - 1; i > 0; i--) {
            array[i] = array[i - 1];
        }
        array[0] = k;
    }

    // Complexity O(n)
    void addRandom(int k) {
        size++;
        int pos = rand() % size;
        array = (int *) realloc(array, sizeof(int) * (size));
        for (int i = size - 1; i > pos; i--) {
            array[i] = array[i - 1];
        }
        array[pos] = k;
    }

    void addValue(int k, int val) {
        size++;
        array = (int *) realloc(array, sizeof(int) * (size));
        for (int i = size - 1; i > k; i--) {
            array[i] = array[i - 1];
        }
        array[k] = val;
    }

    void print() {
        for (int i = 0; i < size; i++) {
            cout << array[i] << " ";
        }
        cout << endl << endl;
    }

//removes value at specified position.Worst case complexity Complexity O(n)
    void remove(int pos) {
        for (int t = pos; t < size; t++) {
            array[t] = array[t + 1];
        }
        size--;
        array = (int *) realloc(array, sizeof(int) * (size));
    }

//returns first occurrence of specified value. Complexity O(n)
    int findVal(int value) {
        for (int i = 0; i < size; i++) {
            if (array[i] == value) {
                return i;
            }
        }
        return -1;
    }

    void generate(int s) {
        size = s;
        array = (int *) realloc(array, sizeof(int) * (size));
        for (int i = 0; i < size; i++) {
            array[i] = rand() % INT32_MAX;
        }
    }
};

class Lista {

    struct node {

        int val;
        node *next;
        node *prev;
    };
private:
    node *tail = nullptr;
    node *head = nullptr;
public:
    int size = 0;
    string menu = "\n\nWybierz instrukcje:\n"
                  "1.Wygeneruj liste\n"
                  "2.Dodaj element\n"
                  "3.Usun element\n"
                  "4.Wyswietl liste\n"
                  "5.Wyszukaj wartosc\n"
                  "6.Usun losowa wartosc\n"
                  "7.Usun ogon\n"
                  "8.Usun glowe\n"
                  "9.Wczytaj z pliku\n"
                  "10.Wykonaj pomiar czasow\n"
                  "0.Wroc\n"
                  "Twoj wybor: ";

    void showListMenu() {
        int k;
        while (true) {
            cout << menu;
            getChoice(&k);
            switch (k) {
                case 1: {
                    int size;
                    cout << "Wprowadz rozmiar do wygenerowania\n";
                    getChoice(&size);
                    this->generate(size);
                    print();
                    break;
                }
                case 2: {
                    string way = "Podaj sposub w ktory chcesz dodac:\n"
                                 "1.Na poczatku\n"
                                 "2.Na koncu\n"
                                 "3.W losowym miejscu\n"
                                 "4. W podanym miejscu\n"
                                 "Twoj wybor: ";
                    cout << way;
                    int choice;
                    getChoice(&choice);
                    if (choice < 1 || choice > 4)
                        cout << "Niema takiej opcji\n";
                    else {
                        cout << "Wprowadz wartosc\n";
                        int val;
                        getChoice(&val);
                        if (choice == 1)
                            this->addValueB(val);
                        else if (choice == 2)
                            this->addValueE(val);
                        else if (choice == 3)
                            this->addRand(val);
                        else if (choice == 4) {
                            int pos;
                            cout << "Podaj pozycj?\n";
                            getChoice(&pos);
                            addValue(pos, val);
                        }
                    }
                    print();
                    break;
                }
                case 3: {
                    cout << "Wprowadz wartosc do usuniecia" << endl;
                    int pos;
                    getChoice(&pos);
                    if (findVal(pos))
                        deleteVal(pos);
                    else
                        cout << "Lista nie zawiera takiej wartosci\n";
                    print();
                    break;
                }
                case 4: {
                    print();
                    break;
                }
                case 5: {
                    cout << "Wprowadz wartosc\n";
                    int val;
                    getChoice(&val);
                    if (this->findVal(val))
                        cout << "Lista zawiera tak? wartosc\n";
                    else cout << "Lista nie zawiera podanej wartosci\n\n";
                    print();
                    break;
                }
                case 6: {
                    this->deleteRand();
                    print();
                    break;
                }
                case 7: {
                    this->deleteTail();
                    cout << "Wartosc ogonowa zostala usunieta\n\n";
                    print();
                    break;
                }
                case 8: {
                    this->deleteHead();
                    cout << "Wartosc na glowie zostala usunieta\n\n";
                    print();
                    break;
                }
                case 9: {
                    removeList(head);
                    Reader reader;
                    reader.setPath();
                    for (int i = 0; i < reader.size; i++) {
                        addValueE(reader.next());
                    }
                    print();
                    reader.myfile.close();
                    break;
                }
                case 10: {
                    measureTime();
                    removeList(head);
                    break;
                }

                case 0: {
                    removeList(head);
                    return;
                }
                default: {
                    cout << "No such option. Try again\n";
                    break;
                }
            }
        }

    }

    void addValueB(int k) {
        size++;
        node *temp = new node();
        temp->next = head;
        temp->prev = nullptr;
        temp->val = k;

        if (head != nullptr) {
            head->prev = temp;
        }
        head = temp;
    }

    //Pass head to delete a List
    void removeList(node *n) {
        size = 0;
        node *curr = n;
        node *next;

        while (curr != NULL) {
            next = curr->next;
            free(curr);
            curr = next;
        }
        head = NULL;
        tail = NULL;
    }

    //Adding value to the end of the list
    void addValueE(int k) {
        size++;
        struct node *temp = new node();
        temp->next = nullptr;
        temp->prev = tail;
        temp->val = k;

        if (tail != nullptr) {
            tail->next = temp;
        }
        if (head == nullptr) {
            head = temp;
            tail = temp;
        }
        tail = temp;
    }

    void measureTime() {
        Lista lista;
        chrono::high_resolution_clock::time_point end;
        chrono::high_resolution_clock::time_point start;
        Printer printer;
        printer.setPath();
        printer.myfile << "Rozmiar struktury(Lista): 100000,    200000,   300000,    400000,   500000" << endl;
        //mierzymy czas dodawania;
        printer.myfile << "Czas wstawienia losowego: ";
        for (int i = 1; i < 6; i++) {
            lista.generate(100000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                lista.addRand(rand() % INT32_MAX);
            }
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 100 << " ";
        }

        printer.myfile << endl;
        printer.myfile << "Dodanie ko?ca: ";
        for (int i = 1; i < 6; i++) {
            lista.generate(100000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000; i++)
                lista.addValueE(rand() % INT32_MAX);
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 1000 << " ";
        }

        printer.myfile << endl;
        printer.myfile << "Dodanie poczatku: ";
        for (int i = 1; i < 6; i++) {
            lista.generate(100000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000; i++)
                lista.addValueB(rand() % INT32_MAX);
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 1000 << " ";
        }
        printer.myfile << endl;
        printer.myfile << "Czas usuwania: ";
        //mierzymy czas usuwania
        for (int i = 1; i < 6; i++) {
            lista.generate(100000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                lista.deleteVal(rand() % INT32_MAX);

            }
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 100 << " ";
        }
        //mierzymy czas wyszukiwania
        printer.myfile << endl;
        printer.myfile << "Czas wyszukiwania: ";
        for (int i = 1; i < 6; i++) {
            lista.generate(10000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000; i++)
                lista.findVal(rand() % INT32_MAX);
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 1000 << " ";
        }
        printer.myfile.close();
    }

    //Prining list in reverse order. Pass tail calling it. For printing list use print() instead;
    void printRev(node *n) {
        if (tail == NULL)
            return;
        cout << n->val << " ";
        if (n->prev != NULL)
            printRev(n->prev);
    }

    void addValue(int i, int val) {
        int length = 0;
        node *temp;
        temp = head;
        while (temp != nullptr) {
            length++;
            temp = temp->next;
        }

        if (i == 0) {
            addValueB(val);
            return;
        }
        if (i == length - 1) {
            addValueE(val);
            return;
        }
        temp = head;
        while (temp != nullptr && i != 0) {
            i--;
            temp = temp->next;
        }
        node *newNode = new node();
        newNode->val = val;
        newNode->prev = temp->prev;
        newNode->next = temp;

        temp->prev->next = newNode;
        temp->prev = newNode;
    }

    //add a value 'v'at a random position
    void addRand(int v) {
        int length = 0;
        node *temp;
        temp = head;
        while (temp != nullptr) {
            length++;
            temp = temp->next;
        }
        int k = rand() % length;
        if (k == 0) {
            addValueB(v);
            return;
        }
        if (k == length - 1) {
            addValueE(v);
            return;
        }
        temp = head;
        while (temp != nullptr && k != 0) {
            k--;
            temp = temp->next;
        }
        node *newNode = new node();
        newNode->val = v;
        newNode->prev = temp->prev;
        newNode->next = temp;

        temp->prev->next = newNode;
        temp->prev = newNode;


    }

    //Display the values of List in normal and reversed order
    void print() {
        if (head == nullptr) {
            cout << "List is empty\n";
            return;
        } else {
            node *temp = head;
            while (temp != nullptr) {
                cout << temp->val << " ";
                temp = temp->next;
            }
        }
        cout << endl << endl;
        cout << "Reversed: ";
        printRev(tail);
    }

//Complexity O(n), n - list length
    bool findVal(int v) {
        node *temp = head;
        while (temp != nullptr) {
            if (temp->val == v)
                return true;
            temp = temp->next;
        }
        return false;
    }

    //Deleting a first occurrence of a particular value
    void deleteVal(int v) {
        node *temp = head;
        while (temp != nullptr) {
            if (temp->val == v) {
                if(temp==tail)
                {tail=tail->prev;
                tail->next=NULL;
                }
                else {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                }
                size--;
                return;
            }
            temp = temp->next;
        }
        size--;
    }

    //deleting rand value from list
    void deleteRand() {

        int length = 0;
        node *temp;
        temp = head;
        while (temp != nullptr) {
            length++;
            temp = temp->next;
        }
        int pos = rand() % length;
        temp = head;
        while (pos != 0) {
            pos--;
            temp = temp->next;
        }
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        cout << "Wartosc  na pozycji " << pos + 1 << " zostala usunieta\n\n";
    }

    //removing tail from the list
    void deleteTail() {
        size--;
        tail = tail->prev;
        tail->next = nullptr;
    }

    //removing head
    void deleteHead() {
        size--;
        head = head->next;
    }

    //Generating a list via sequential adding of rand values at its end;
    void generate(int size) {
        this->size = size;
        this->removeList(head);

        for (int i = 0; i < size; i++) {
            addValueE(rand() % INT32_MAX);
        }
    }
};


class BinaryHeap {
    const int maxSize = 1000000;
    int *heap = new int[maxSize];
    int size = 0;

private:
    string menu = "\n\nWybierz instrukcje:\n"
                  "1.Wygeneruj kopiec\n"
                  "2.Dodaj element\n"
                  "3.Usun element\n"
                  "4.Wyswietl kopiec\n"
                  "5.Wyszukaj wartosc\n"
                  "6.Wczytaj z pliku\n"
                  "7.Pomiar czasu\n"
                  "0.Wroc\n"
                  "Twoj wybor: ";
public:
    void showHeapMenu() {
        int k;
        while (true) {
            cout << menu;
            getChoice(&k);
            switch (k) {
                case 1: {
                    cout << "Wprowadz rozmiar kopcu (max " << maxSize << ")\n";
                    getChoice(&size);
                    this->generate(size);
                    print();
                    break;
                }
                case 2: {
                    int val;
                    getChoice(&val);
                    this->heapInsert(val);
                    print();
                    break;
                }
                case 3: {
                    cout << "Wprowadz wartosc do usuniecia" << endl;
                    int pos;
                    getChoice(&pos);
                    if (this->deleteKey(pos)) {
                        cout << "Wartosc zostala usunieta\n";
                    } else cout << "Kopiec nie zawiera takiej wartosci\n";
                    print();
                    break;
                }
                case 4: {
                    this->print();
                    break;
                }
                case 5: {
                    cout << "Wprowadz wartosc\n";
                    int val;
                    getChoice(&val);
                    if (this->find(val)) {
                        cout << "Jest taka wartosc jak" << val << endl;
                    } else
                        cout << "Niema takiej wartosci jak " << val << endl;
                    print();
                    break;
                }
                case 6: {
                    delete (heap);
                    heap = new int[maxSize];
                    Reader reader;
                    reader.setPath();
                    for (int i = 0; i < reader.size; i++) {
                        heapInsert(reader.next());
                    }
                    reader.myfile.close();
                    print();
                    break;
                }
                case 7: {
                    measureTime();
                    break;
                }
                case 0: {
                    free(heap);
                    return;
                }
                default: {
                    cout << "No such option. Try again\n";
                    break;
                }
            }
        }
    }

    void measureTime() {
        BinaryHeap heap;
        chrono::high_resolution_clock::time_point end;
        chrono::high_resolution_clock::time_point start;
        Printer printer;
        printer.setPath();
        printer.myfile << "Rozmiar struktury(Kopiec): 100000,    200000,   300000,    400000,   500000" << endl;
        //mierzymy czas dodawania;
        printer.myfile << "Czas wstawienia: ";
        for (int i = 1; i < 6; i++) {
            heap.generate(100000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 10000; i++) {
                heap.heapInsert(rand() % INT32_MAX);
            }
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 10000 << " ";
        }
        printer.myfile << endl;
        printer.myfile << "Czas usuwania: ";
        //mierzymy czas usuwania
        for (int i = 1; i < 6; i++) {
            heap.generate(100000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                heap.deleteKey(rand() % INT32_MAX);

            }
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 100 << " ";
        }
        //mierzymy czas wyszukiwania
        printer.myfile << endl;
        printer.myfile << "Czas wyszukiwania: ";
        for (int i = 1; i < 6; i++) {
            heap.generate(100000 * i);
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 10000; i++)
                heap.find(rand() % INT32_MAX);
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 10000 << " ";
        }

        printer.myfile.close();
    }

//Inserting new value into the heap, preserving the heap's characteristics
    void heapInsert(int val) {
        if (size == maxSize) {
            cout << "Error: No room for insertion. Process aborted\n\n";
            return;
        }
        size++;
        int i = size;
        while (i > 1 && heap[parent(i)] < val) {
            heap[i] = heap[parent(i)];
            i = parent(i);
        }
        heap[i] = val;
    }

//Sorting method for tables, which restores heap characteristics in it
    void heapify(int i) {
        int largest;
        int l = left(i);
        int r = right(i);
        if (l < size && heap[l] > heap[i])
            largest = l;
        else largest = i;
        if (r < size && heap[r] > heap[largest])
            largest = r;
        if (largest != i) {
            int temp = heap[i];
            heap[i] = heap[largest];
            heap[largest] = temp;
            heapify(largest);
        }
    }

//deleting a specified key from heap. Restoring heap's properties using heapify
    bool deleteKey(int key) {
        for (int i = 1; i <= size; i++) {
            if (heap[i] == key) {
                heap[i] = heap[size];
                size--;
                heapify(i);
                return true;
            }
        }
        return false;
    }

    int parent(int i) {
        return (int) i / 2;
    }

    int left(int i) {
        return (int) 2 * i;
    }

    int right(int i) {
        return (int) 2 * i + 1;
    }

    bool isEmpty() {
        return size == 0;
    }

    //Printing heap like an array
    void print() {
        if (isEmpty()) {
            cout << "Heap is empty.";
            return;
        }
        PrintByLevel();
        cout << endl << endl;
    }

    //todo Print heap like a tree
    void PrintByLevel() {
        int k = 1;
        for (int i = 1; i <= size; i++) {
            if (pow(2, k) > i) {
                cout << heap[i] << " ";
            } else {
                i--;
                k++;
                cout << endl;
            }
        }

    }

    void generate(int n) {
        if (n > maxSize) {
            cout << "Error. Not enought room.\n";
            return;
        } else
            size = 0;
        for (int i = 0; i < n; i++) {
            heapInsert(rand() % INT32_MAX);
        }
    }

    //Searching method for heap using its property
    bool find(int key) {
        for (int i = 1; i <= size; i++) {
            if (heap[i] == key) {
                return true;
            }
        }
        return false;
    }

};

class BinaryTree {

    struct Node {
        int key;
        Node *left;
        Node *right;

    };
    Node *root = new Node();
    Node *searchNode = new Node();
    Node *searchParent = new Node();
public:
    string menu = "\n\nWybierz instrukcje:\n"
                  "1.Wygeneruj drzewo\n"
                  "2.Dodaj element\n"
                  "3.Usun element\n"
                  "4.Wyswietl drzewo In-Order\n"
                  "5.Wyszukaj wartosc\n"
                  "6.Wykonaj pomiary czasu\n"
                  "7.Odczytaj z pliku\n"
                  "8.Zastosuj algorytm DSW\n"
                  "9.Zrob backbone\n"
                  "0.Wroc\n"
                  "Twoj wybor: ";

    void showTreeMenu() {
        int k;
        while (true) {
            cout << menu;
            getChoice(&k);
            switch (k) {
                case 1: {
                    deleteTree(root);
                    root = nullptr;
                    int size;
                    cout << "Wprowadz rozmiar do wygenerowania\n";
                    getChoice(&size);
                    this->generate(size);
                    this->printBT(root);
                    break;
                }
                case 2: {
                    cout << "Wprowadz wartosc\n";
                    int val;
                    getChoice(&val);
                    this->insertVal(val);
                    this->printBT(root);
                    break;
                }
                case 3: {
                    cout << "Wprowadz wartosc do usuniecia" << endl;
                    int pos;
                    getChoice(&pos);
                    if (deleteNode(root,pos)) {
                        cout << "Wartosc zostala usunieta\n\n";
                    }
                    this->printBT(root);
                    break;
                }
                case 4: {
                    print();
                    break;
                }
                case 5: {
                    cout << "Wprowadz wartosc\n";
                    int val;
                    getChoice(&val);
                    if (this->find(val)) {
                        cout << "Drzewo  zawiera podana wartosc\n";
                    } else cout << "Drzewo nie zawiera takiej wartosci\n";
                    this->printBT(root);
                    break;
                }
                case 6: {
                    measureTime();
                    this->printBT(root);
                    break;
                }
                case 0: {
                    deleteTree(root);
                    return;
                }
                case 7: {
                    root = nullptr;
                    Reader reader;
                    reader.setPath();
                    int size = reader.size;
                    for (int i = 0; i < size; i++) {
                        insertVal(reader.next());
                    }
                    reader.myfile.close();
                    this->printBT(root);
                    break;
                }
                case 8: {
                    DSW();
                    this->print();
                    break;
                }
                case 9: {
                    createBackbone();
                    break;
                }
                default: {
                    cout << "No such option. Try again\n";
                    break;
                }
            }
        }
    }

    BinaryTree() {
        root = nullptr;
    }

    void measureTime() {
        BinaryTree tree1;
        chrono::high_resolution_clock::time_point end;
        chrono::high_resolution_clock::time_point start;
        Printer printer;
        printer.setPath();
        printer.myfile << "Rozmiar struktury(Drzewa): 100000, 200000,30000,400000,500000" << endl;

        //mierzymy czas dodawania;
        printer.myfile << "Czas wstawienia: ";
        for (int i = 1; i < 6; i++) {
            tree1.generate(100000 * i);
            tree1.DSW();
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                tree1.insertVal(rand() % INT32_MAX);
                tree1.DSW();
            }
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 100 << " ";
            tree1.deleteTree(tree1.root);
            tree1.root = nullptr;
        }

        printer.myfile << endl;
        printer.myfile << "Czas usuwania: ";
        //mierzymy czas usuwania
        for (int i = 1; i < 6; i++) {
            tree1.generate(100000 * i);
            tree1.DSW();
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; i++) {
                tree1.deleteNode( root, rand() % INT32_MAX);
                tree1.DSW();
            }
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 10 << " ";
            tree1.deleteTree(tree1.root);
            tree1.root = nullptr;
        }
        //mierzymy czas wyszukiwania
        printer.myfile << endl;
        printer.myfile << "Czas wyszukiwania: ";
        for (int i = 1; i < 6; i++) {
            tree1.generate(100000 * i);
            tree1.createBackbone();
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000; i++)
                tree1.find(rand() % INT32_MAX);
            end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = chrono::duration_cast<chrono::duration<double>>(end - start);
            printer.myfile << diff.count() / 1000 << " ";
            tree1.deleteTree(tree1.root);
            tree1.root = nullptr;
        }


        printer.myfile.close();
    }

    void insertVal(int val) {
        if (root == nullptr) {
            root = new Node;
            root->left = nullptr;
            root->right = nullptr;
            root->key = val;
        } else {
            insertVal(val, root);
        }

    }


    void deleteTree(Node *root) {
        if (root == nullptr)
            return;
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }

    //DSW algorithm implementation. Key methods are left and right roatitions;
    //one's used for creating a backbone. Second one's for making a tree balanced
    void DSW() {
        if (NULL != root) {
            createBackbone();// effectively: createBackbone( root)
            createPerfectBST();//effectively: createPerfectBST( root)
        }
    }

    void createBackbone() {
        Node *grandParent = NULL;
        Node *parent = root;
        Node *leftChild;

        while (NULL != parent) {
            leftChild = parent->left;
            if (NULL != leftChild) {
                grandParent = rotateRight(grandParent, parent, leftChild);
                parent = leftChild;
            } else {
                grandParent = parent;
                parent = parent->right;
            }
        }
    }

    Node *rotateRight(Node *grandParent, Node *parent, Node *leftChild) {
        if (NULL != grandParent) {
            grandParent->right = leftChild;
        } else {
            root = leftChild;
        }
        parent->left = leftChild->right;
        leftChild->right = parent;
        return grandParent;
    }

    void createPerfectBST() {
        int n = 0;
        for (Node *tmp = root; NULL != tmp; tmp = tmp->right) {
            n++;
        }
        //m = 2^floor[lg(n+1)]-1, ie the greatest power of 2 less than n: minus 1
        int m = greatestPowerOf2LessThanN(n + 1) - 1;
        makeRotations(n - m);

        while (m > 1) {
            makeRotations(m /= 2);
        }
    }

    int greatestPowerOf2LessThanN(int n) {
        int x = MSB(n);//MSB
        return (1 << x);//2^x
    }


    int MSB(int n) {
        int ndx = 0;
        while (1 < n) {
            n = (n >> 1);
            ndx++;
        }
        return ndx;
    }

    void makeRotations(int bound) {
        Node *grandParent = NULL;
        Node *parent = root;
        Node *child = root->right;
        for (; bound > 0; bound--) {
            try {
                if (NULL != child) {
                    rotateLeft(grandParent, parent, child);
                    grandParent = child;
                    parent = grandParent->right;
                    child = parent->right;
                } else {
                    break;
                }
            } catch (exception &e) {
                break;
            }
        }
    }

    void rotateLeft(Node *grandParent, Node *parent, Node *rightChild) {
        if (NULL != grandParent) {
            grandParent->right = rightChild;
        } else {
            root = rightChild;
        }
        parent->right = rightChild->left;
        rightChild->left = parent;
    }

    void insertVal(int val, Node *node) {
        if (node->key > val) {
            if (node->left != nullptr)
                insertVal(val, node->left);
            else {
                node->left = new Node;
                node->left->left = nullptr;
                node->left->right = nullptr;
                node->left->key = val;
            }
        } else if (node->key < val) {
            if (node->right != nullptr)
                insertVal(val, node->right);
            else {
                node->right = new Node;
                node->right->left = nullptr;
                node->right->right = nullptr;
                node->right->key = val;
            }
        }
    }

//In-Order
    void print(Node *n) {
        if (root == NULL) {
            cout << "Drzewo jest puste\n\n";
            return;
        }
        if (n->left != nullptr)
            print(n->left);
        cout << n->key << " ";
        if (n->right != nullptr)
            print(n->right);

    }

    void print() {
        print(root);
    }

    void generate(int size) {
        searchParent = NULL;
        searchNode = NULL;
        for (int i = 0; i < size; i++) {
            insertVal(rand() % INT32_MAX);
        }
    }


    bool find(int val) {
        searchNode = NULL;
        searchParent = NULL;
        if (isEmpty()) {
            cout << "Tree is empty\n";
            return false;
        } else {
            return find(val, root);
        }
    }

    bool find(int val, Node *node) {
        bool b;
        if (node->key == val) {
            searchNode = node;
            return true;
        } else if (node->key < val) {
            searchParent = node;
            if (node->right == NULL)
                return false;
            return find(val, node->right);
        } else if (node->key > val) {
            searchParent = node;
            if (node->left == NULL)
                return false;
            return find(val, node->left);
        }
        return false;
    }

    bool isEmpty() {
        if (!root)
            return true;
        else return false;
    }

    Node *getMin(Node *n) {
        while (n->left != NULL) {
            n = n->left;
        }
        return n;
    }

    //Method that prints BST for visually representative; Makes it look more like a tree.
    void printBT(const std::string &prefix, const Node *node, bool isLeft) {
        if (node != nullptr) {
            std::cout << prefix;

            std::cout << (isLeft ? (char) 195
                                 : (char) 192);     //those symbols aren't displayed right, probably because of file encoding
            //todo: might fix it later might not

            // print the value of the node
            std::cout << node->key << std::endl;

            // enter the next tree level - left and right branch
            printBT(prefix + (isLeft ? "|   " : "   "), node->left, true);
            printBT(prefix + (isLeft ? "|   " : "   "), node->right, false);
        }
    }

    void printBT(const Node *node) {
        printBT("", node, false);
    }


    //While removing a value from a tree we consider 3 situations: when it has no children, when it has a single child and
    // when it has both children. It case it has both children we look for the next smallest value of the subtree and replace
    //the key of the node to be removed with this smallest value;
    struct Node* deleteNode(Node* root, int key)
    {
        // base case
        if (root == NULL) return root;

        // If the key to be deleted is smaller than the root's key,
        // then it lies in left subtree
        if (key < root->key)
            root->left = deleteNode(root->left, key);

            // If the key to be deleted is greater than the root's key,
            // then it lies in right subtree
        else if (key > root->key)
            root->right = deleteNode(root->right, key);

            // if key is same as root's key, then This is the node
            // to be deleted
        else
        {
            // node with only one child or no child
            if (root->left == NULL)
            {
                Node *temp = root->right;
                free(root);
                return temp;
            }
            else if (root->right == NULL)
            {
                Node *temp = root->left;
                free(root);
                return temp;
            }

            // node with two children: Get the inorder successor (smallest
            // in the right subtree)
           Node* temp = getMin(root->right);

            // Copy the inorder successor's content to this node
            root->key = temp->key;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
        return root;
    }
};

class Menu {
public:
    string mainMenu = "Wybierz strukture:\n"
                      "1.Tablica\n"
                      "2.Lista\n"
                      "3.Kopiec\n"
                      "4.Drzewo Binarne\n"
                      "0.Koniec\n"
                      "Twoj wybor: ";
    int choice = -1;

    void structMenuSwitch() {

        do {
            cout << mainMenu;
            getChoice(&choice);
            switch (choice) {
                case 1: {
                    Tablica tablica;
                    tablica.showTableMenu();
                    break;
                }
                case 2: {
                    Lista lista;
                    lista.showListMenu();
                    break;
                }
                case 3: {
                    BinaryHeap heap;
                    heap.showHeapMenu();

                    break;
                }
                case 4: {
                    BinaryTree tree;
                    tree.showTreeMenu();
                    break;
                }
                default: {
                    cout << "No such option. Try again\n";
                    break;
                }
                case 0: {
                    return;
                }
            }
        } while (true);
    }


};


int main() {
    srand((unsigned) time(nullptr));
    Menu menu;
    menu.structMenuSwitch();
    return 0;
}