#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// ==========================================
// 1. STRUKTURA: TABLICA DYNAMICZNA
// ==========================================
class DynamicArray {
private:
    int* data;
    int size;     
    int capacity; 

    void resize() {
        capacity *= 2;
        int* newData = new int[capacity];
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    DynamicArray() {
        capacity = 1;
        size = 0;
        data = new int[capacity];
    }

    ~DynamicArray() {
        delete[] data;
    }

    void addFront(int value) {
        if (size == capacity) resize();
        for (int i = size; i > 0; --i) {
            data[i] = data[i - 1];
        }
        data[0] = value;
        size++;
    }

    void addBack(int value) {
        if (size == capacity) resize();
        data[size] = value;
        size++;
    }

    void addAtIndex(int index, int value) {
        if (index < 0 || index > size) return;
        if (size == capacity) resize();
        for (int i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        size++;
    }

    void removeFront() {
        if (size == 0) return;
        for (int i = 0; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        size--;
    }

    void removeBack() {
        if (size == 0) return;
        size--;
    }

    void removeAtIndex(int index) {
        if (index < 0 || index >= size) return;
        for (int i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        size--;
    }

    bool search(int value) {
        for (int i = 0; i < size; ++i) {
            if (data[i] == value) return true;
        }
        return false;
    }
};

// ==========================================
// 2. SYSTEM POMIAROWY (BENCHMARK)
// ==========================================
void runBenchmark(int operationChoice, int n, int iterations) {
    double totalTimeNs = 0;

    for (int i = 0; i < iterations; ++i) {
        DynamicArray arr;
        for (int j = 0; j < n; ++j) {
            arr.addBack(j);
        }

        auto start = high_resolution_clock::now();
        auto end = start;

        switch (operationChoice) {
        case 1:
            start = high_resolution_clock::now();
            arr.addFront(-1);
            end = high_resolution_clock::now();
            break;
        case 2:
            start = high_resolution_clock::now();
            arr.addBack(-1);
            end = high_resolution_clock::now();
            break;
        case 3:
            start = high_resolution_clock::now();
            arr.addAtIndex(n / 2, -1); 
            end = high_resolution_clock::now();
            break;
        case 4:
            start = high_resolution_clock::now();
            arr.removeFront();
            end = high_resolution_clock::now();
            break;
        case 5:
            start = high_resolution_clock::now();
            arr.removeBack();
            end = high_resolution_clock::now();
            break;
        case 6:
            start = high_resolution_clock::now();
            arr.removeAtIndex(n / 2); 
            end = high_resolution_clock::now();
            break;
        case 7:
            start = high_resolution_clock::now();
            arr.search(-1);
            end = high_resolution_clock::now();
            break;
        }

        totalTimeNs += duration<double, std::nano>(end - start).count();
    }
    double averageTimeNs = totalTimeNs / iterations;
    cout << "\n--------------------------------------------------\n";
    cout << "Wynik testu:" << endl;
    cout << "Poczatkowy rozmiar struktury (N): " << n << endl;
    cout << "Liczba powtorzen testu: " << iterations << endl;
    cout << "SREDNI CZAS OPERACJI: " << averageTimeNs << " ns" << endl;
    cout << "--------------------------------------------------\n\n";
}

// ==========================================
// 3. INTERFEJS UZYTKOWNIKA
// ==========================================
int main() {
    int choice = 0;
    int n = 0;
    int iterations = 0;

    while (true) {
        cout << "=== MENU TESTOWANIA TABLICY DYNAMICZNEJ ===" << endl;
        cout << "1. Dodawanie na poczatek" << endl;
        cout << "2. Dodawanie na koniec" << endl;
        cout << "3. Dodawanie pod wybrany indeks (srodek)" << endl;
        cout << "4. Usuwanie z poczatku" << endl;
        cout << "5. Usuwanie z konca" << endl;
        cout << "6. Usuwanie z wybranego indeksu (srodek)" << endl;
        cout << "7. Wyszukiwanie elementu (najgorszy przypadek)" << endl;
        cout << "8. Wyjscie z programu" << endl;
        cout << "Wybierz operacje (1-8): ";
        cin >> choice;

        if (choice == 8) {
            cout << "Zamykanie programu..." << endl;
            break;
        }

        if (choice < 1 || choice > 8) {
            cout << "Niepoprawny wybor! Sprobuj ponownie.\n\n";
            continue;
        }

        cout << "Podaj wielkosc poczatkowa struktury (np. 10000): ";
        cin >> n;

        cout << "Podaj liczbe powtorzen do wyliczenia sredniej (np. 1000): ";
        cin >> iterations;

        cout << "\nTrwa testowanie, prosze czekac..." << endl;

        runBenchmark(choice, n, iterations);
    }

    return 0;
}