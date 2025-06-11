#include <iostream>
#include <string>
#include <vector>

const int TABLE_SIZE = 17; // Простое число для уменьшения коллизий

class Student {
public:
    int id;
    std::string name;

    Student(int i = 0, std::string n = "") : id(i), name(n) {}
};

class HashTable {
private:
    struct HashNode {
        Student data;
        bool isOccupied = false;
        bool isDeleted = false;
    };

    std::vector<HashNode> table;

    // Основная хэш-функция
    int primaryHash(const Student& student) const {
        unsigned int hash = 0;
        for (char ch : student.name)
            hash = (hash * 31 + ch) % TABLE_SIZE;

        hash ^= (student.id << 3);
        return hash % TABLE_SIZE;
    }

    // Вторая хэш-функция для двойного хеширования
    int secondaryHash(const Student& student) const {
        return 7 - (student.id % 7); // 7 должно быть меньше TABLE_SIZE
    }

public:
    HashTable() {
        table.resize(TABLE_SIZE);
    }

    void insert(const Student& student) {
        int hash1 = primaryHash(student);
        int hash2 = secondaryHash(student);
        int i = 0;

        while (table[(hash1 + i * hash2) % TABLE_SIZE].isOccupied &&
               !table[(hash1 + i * hash2) % TABLE_SIZE].isDeleted) {
            i++;
        }

        int index = (hash1 + i * hash2) % TABLE_SIZE;
        table[index].data = student;
        table[index].isOccupied = true;
        table[index].isDeleted = false;
        std::cout << "Inserted at index " << index << "\n";
    }

    bool remove(int id, const std::string& name) {
        Student temp(id, name);
        int hash1 = primaryHash(temp);
        int hash2 = secondaryHash(temp);
        int i = 0;

        while (table[(hash1 + i * hash2) % TABLE_SIZE].isOccupied) {
            int index = (hash1 + i * hash2) % TABLE_SIZE;
            if (!table[index].isDeleted &&
                table[index].data.id == id &&
                table[index].data.name == name) {
                table[index].isDeleted = true;
                std::cout << "Deleted from index " << index << "\n";
                return true;
            }
            i++;
        }

        return false;
    }

    bool find(int id, const std::string& name) const {
        Student temp(id, name);
        int hash1 = primaryHash(temp);
        int hash2 = secondaryHash(temp);
        int i = 0;

        while (table[(hash1 + i * hash2) % TABLE_SIZE].isOccupied) {
            int index = (hash1 + i * hash2) % TABLE_SIZE;
            if (!table[index].isDeleted &&
                table[index].data.id == id &&
                table[index].data.name == name) {
                std::cout << "Found at index " << index << "\n";
                return true;
            }
            i++;
        }

        return false;
    }

    void print() const {
        std::cout << "\nHash Table:\n";
        for (int i = 0; i < TABLE_SIZE; ++i) {
            std::cout << i << ": ";
            if (table[i].isOccupied && !table[i].isDeleted) {
                std::cout << table[i].data.id << " - " << table[i].data.name;
            } else if (table[i].isDeleted) {
                std::cout << "[deleted]";
            } else {
                std::cout << "[empty]";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    HashTable ht;
    ht.insert(Student(1, "Alice"));
    ht.insert(Student(2, "Bob"));
    ht.insert(Student(3, "Charlie"));

    ht.print();

    ht.find(2, "Bob");
    ht.remove(2, "Bob");

    ht.print();

    return 0;
}
