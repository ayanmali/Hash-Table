#include <iostream>
#include <functional>
#include <vector>

/*
Hash table based on hybrid chaining + open addressing
Each key is associated with a bucket index.
Each key's value is a pointer to its data
*/
template <typename K, typename V>
class HashTable {
    private:
        size_t size;
        std::function<size_t(K)> hashFunction;
        std::vector<std::pair<size_t, std::unique_ptr<std::pair<K, V>> > > table; // each entry is a pair of the hash and the pointer to the KV pair
        //std::vector<std::pair<K, V>> buckets; // each entry is a pair of the key and the value

    public:
        HashTable(size_t size, std::function<size_t(K)> hashFunction) {
            this->size = size;
            this->hashFunction = hashFunction;
            this->table.resize(size);
            //this->buckets.resize(size);
        };

        HashTable(size_t size) 
        : HashTable(size, 
            [size](K key) -> size_t { return std::hash<K>()(key); } ) {};
        
        V get(K key);
        void remove(K key);
        void set(K key, V value);
        
};

template <typename K, typename V>
V HashTable<K, V>::get(K key) {
    // get the index of the given key
    const size_t index = this->hashFunction(key) % this->size;
    // check if that index's key matches the provided key
    auto& [foundKey, value] = this->table[index]->second;
    if (key == foundKey) { // match
        return value;
    } else { // does not match
        for (size_t i = index + 1; i < this->size; ++i) {
            auto& [foundKey, value] = this->table[i]->second;
            if (key == foundKey) {
                return value;
            }
        }
    }
    
};

template<typename K, typename V>
void HashTable<K, V>::remove(K key) {
    // get the index of the given key
    const size_t index = this->hashFunction(key) % this->size;
    // check if that index's key matches the provided key
    auto& [foundKey, value] = this->table[index]->second;
    if (key == foundKey) { // match
        // delete the KV pair
        this->table[index]->second->first = nullptr;
        this->table[index]->second->second = nullptr;
        this->table[index] = nullptr;
        return;
    } else { // does not match
        for (size_t i = index + 1; i < this->size; ++i) {
            auto& [foundKey, value] = this->table[i]->second;
            if (key == foundKey) {
                this->table[index]->second->first = nullptr;
                this->table[index]->second->second = nullptr;
                this->table[index] = nullptr;
                return;
            }
        }
    }
    std:: cout << "Key " << key << " not found" << std::endl;
};

template <typename K, typename V>
void HashTable<K, V>::set(K key, V value) {
// compute the index to insert
size_t index = this->hashFunction(key) % this->size;
size_t indexCopy = size_t(index);
while (index < this->size + indexCopy && this->table[index % this->size] != nullptr) {
    ++index;
}
// if the key does not already exist: add it to the end of the vector
if (index == this->size + indexCopy) {
    this->size++;
    this->table.push_back(std::make_unique(std::pair<K, V>(key, value)));
    return;
}
// if the key does already exist, update its value
this->table[index % this->size]->second->second = value;
};

int main() {
    std::cout << "Hash Table based on hybrid chaining + open addressing in C++" << std::endl;
    HashTable<int, std::string> map = HashTable<int, std::string>(5);
    return 0;
};