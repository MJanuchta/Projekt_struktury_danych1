#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// ==========================================
// 1. STRUKTURA: LISTA JEDNOKIERUNKOWA
// ==========================================
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class SinglyLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~SinglyLinkedList() {
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
        current->next = newNode;
        size++;
    }

    void removeFront() {
        if (!head) return;
        Node* temp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        size--;
    }

    void removeBack() {
        if (!head) return;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        }
        else {
            // W liście jednokierunkowej musimy przejść przez całą listę,
            // aby znaleźć element przedostatni (poprzedzający tail)
            Node* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        size--;
    }

    void removeAtIndex(int index) {
        if (index < 0 || index >= size) return;
        if (index == 0) { removeFront(); return; }
        if (index == size - 1) { removeBack(); return; }

        Node* current = head;
        // Wyszukujemy element poprzedzający ten, który chcemy usunąć
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }

        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
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
        SinglyLinkedList list;
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
        cout << "=== MENU TESTOWANIA LISTY JEDNOKIERUNKOWEJ ===" << endl;
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