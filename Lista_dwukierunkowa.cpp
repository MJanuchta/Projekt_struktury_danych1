#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// ==========================================
// 1. STRUKTURA: LISTA DWUKIERUNKOWA
// ==========================================
struct Node {
    int data;
    Node* prev;
    Node* next;
    Node(int val) : data(val), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~DoublyLinkedList() {
        while (head != nullptr) {
            removeFront();
        }
    }

    void addFront(int value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    void addBack(int value) {
        Node* newNode = new Node(value);
        if (!tail) {
            head = tail = newNode;
        }
        else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void addAtIndex(int index, int value) {
        if (index < 0 || index > size) return;
        if (index == 0) { addFront(value); return; }
        if (index == size) { addBack(value); return; }

        Node* newNode = new Node(value);
        Node* current = head;
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }

        newNode->next = current->next;
        newNode->prev = current;
        current->next->prev = newNode;
        current->next = newNode;
        size++;
    }

    void removeFront() {
        if (!head) return;
        Node* temp = head;
        if (head == tail) {
            head = tail = nullptr;
        }
        else {
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
        size--;
    }

    void removeBack() {
        if (!tail) return;
        Node* temp = tail;
        if (head == tail) {
            head = tail = nullptr;
        }
        else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete temp;
        size--;
    }

    void removeAtIndex(int index) {
        if (index < 0 || index >= size) return;
        if (index == 0) { removeFront(); return; }
        if (index == size - 1) { removeBack(); return; }

        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }

        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
        size--;
    }

    bool search(int value) {
        Node* current = head;
        while (current) {
            if (current->data == value) return true;
            current = current->next;
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
        DoublyLinkedList list;
        for (int j = 0; j < n; ++j) {
            list.addBack(j);
        }

        auto start = high_resolution_clock::now();
        auto end = start;

        switch (operationChoice) {
        case 1:
            start = high_resolution_clock::now();
            list.addFront(-1);
            end = high_resolution_clock::now();
            break;
        case 2:
            start = high_resolution_clock::now();
            list.addBack(-1);
            end = high_resolution_clock::now();
            break;
        case 3:
            start = high_resolution_clock::now();
            list.addAtIndex(n / 2, -1); 
            end = high_resolution_clock::now();
            break;
        case 4:
            start = high_resolution_clock::now();
            list.removeFront();
            end = high_resolution_clock::now();
            break;
        case 5:
            start = high_resolution_clock::now();
            list.removeBack();
            end = high_resolution_clock::now();
            break;
        case 6:
            start = high_resolution_clock::now();
            list.removeAtIndex(n / 2);
            end = high_resolution_clock::now();
            break;
        case 7:
            start = high_resolution_clock::now();
            list.search(-1);
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
        cout << "=== MENU TESTOWANIA LISTY DWUKIERUNKOWEJ ===" << endl;
        cout << "1. Dodawanie na poczatek" << endl;
        cout << "2. Dodawanie na koniec" << endl;
        cout << "3. Dodawanie pod wybrany indeks" << endl;
        cout << "4. Usuwanie z poczatku" << endl;
        cout << "5. Usuwanie z konca" << endl;
        cout << "6. Usuwanie z wybranego indeksu" << endl;
        cout << "7. Wyszukiwanie elementu" << endl;
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

        cout << "Podaj wielkosc poczatkowa struktury: ";
        cin >> n;

        cout << "Podaj liczbe powtorzen do wyliczenia sredniej: ";
        cin >> iterations;

        cout << "\nTrwa testowanie, prosze czekac..." << endl;

        runBenchmark(choice, n, iterations);
    }

    return 0;
}
