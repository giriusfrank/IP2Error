#include <cstdlib>
#include <sstream>
#include "skiplist.h"

namespace CustomADT
{
    DuplicateValueException::DuplicateValueException(const std::string& message)
        : std::logic_error(message) {}

    class SkipList::SkipListImpl
    {
        private:
            struct Node
            {
                int value;
                int level;
                Node** forward; 

                Node(int val, int lvl) : value(val), level(lvl)
                {
                    forward = new Node*[lvl + 1];
                    for (int i = 0; i <= lvl; ++i)
                    {
                        forward[i] = nullptr;
                    }
                }
                
                ~Node()
                {
                    delete[] forward;
                }
            };

            static const int MAX_LEVEL = 16;
            float probability;
            int currentLevel;
            Node* head;
            int size;

            int randomLevel()
            {
                int lvl = 0;
                while (((float)std::rand() / RAND_MAX) < probability && lvl < MAX_LEVEL)
                {
                    lvl++;
                }
                return lvl;
            }

        public:
            SkipListImpl() : probability(0.5f), currentLevel(0), size(0)
            {
                head = new Node(-1, MAX_LEVEL);
            }

            ~SkipListImpl()
            {
                clear(); delete head;
            }

            void clear()
            {
                Node* curr = head->forward[0];
                while (curr)
                {
                    Node* next = curr->forward[0];
                    delete curr;
                    curr = next;
                }
                for (int i = 0; i <= MAX_LEVEL; ++i) 
                {
                    head->forward[i] = nullptr;
                }
                currentLevel = 0;
                size = 0;
            }

            void deepCopy(const SkipListImpl& other)
            {
                clear();
                Node* curr = other.head->forward[0];
                while (curr)
                {
                    insert(curr->value); 
                    curr = curr->forward[0];
                }
            }

            void insert(int val)
            {
                Node* update[MAX_LEVEL + 1];
                Node* curr = head;
                for (int i = currentLevel; i >= 0; i--)
                {
                    while (curr->forward[i] && curr->forward[i]->value < val)
                    {
                        curr = curr->forward[i];
                    }
                    update[i] = curr;
                }
                curr = curr->forward[0];

                if (curr && curr->value == val)
                {
                    throw DuplicateValueException("Value already exists in SkipList.");
                }

                int newLvl = randomLevel();
                if (newLvl > currentLevel)
                {
                    for (int i = currentLevel + 1; i <= newLvl; i++) update[i] = head;
                    currentLevel = newLvl;
                }

                Node* newNode = new Node(val, newLvl);
                for (int i = 0; i <= newLvl; i++)
                {
                    newNode->forward[i] = update[i]->forward[i];
                    update[i]->forward[i] = newNode;
                }
                size++;
            }

            void remove(int val)
            {
                Node* update[MAX_LEVEL + 1];
                Node* curr = head;
                for (int i = currentLevel; i >= 0; i--)
                {
                    while (curr->forward[i] && curr->forward[i]->value < val) curr = curr->forward[i];
                    update[i] = curr;
                }
                curr = curr->forward[0];

                if (curr && curr->value == val)
                {
                    for (int i = 0; i <= currentLevel; i++)
                    {
                        if (update[i]->forward[i] != curr)
                        {
                            break;
                        }
                        update[i]->forward[i] = curr->forward[i];
                    }
                    delete curr;
                    while (currentLevel > 0 && head->forward[currentLevel] == nullptr)
                    {
                        currentLevel--;
                    }
                    size--;
                } 
                else
                {
                    throw std::invalid_argument("Value not found for deletion.");
                }
            }

            bool search(int val) const
            {
                Node* curr = head;
                for (int i = currentLevel; i >= 0; i--)
                {
                    while (curr->forward[i] && curr->forward[i]->value < val) curr = curr->forward[i];
                }
                curr = curr->forward[0];
                return curr && curr->value == val;
            }

            bool isEqual(const SkipListImpl& other) const
            {
                Node* a = head->forward[0];
                Node* b = other.head->forward[0];
                while (a && b)
                {
                    if (a->value != b->value)
                    {
                        return false;
                    }
                    a = a->forward[0];
                    b = b->forward[0];
                }
                return (a == nullptr && b == nullptr);
            }

            bool isLess(const SkipListImpl& other) const
            {
                Node* a = head->forward[0];
                Node* b = other.head->forward[0];
                while (a && b)
                {
                    if (a->value < b->value)
                    {
                        return true;
                    }
                    if (a->value > b->value)
                    {
                        return false;
                    }
                    a = a->forward[0];
                    b = b->forward[0];
                }
                return (a == nullptr && b != nullptr);
            }

            std::string toString() const 
            {
                std::ostringstream oss;
                oss << "SkipList [Size: " << size << ", Max Actual Lvl: " << currentLevel << "] Elements: ";
                Node* curr = head->forward[0];

                if (size <= 10) 
                {
                    while (curr) 
                    {
                        oss << curr->value << " ";
                        curr = curr->forward[0];
                    }
                } 
                else 
                {
                    for (int i = 0; i < 5; ++i) 
                    {
                        oss << curr->value << " ";
                        curr = curr->forward[0];
                    }
                    
                    oss << "... ";
                    
                    for (int i = 0; i < (size - 10); ++i) 
                    {
                        curr = curr->forward[0];
                    }
                    
                    while (curr) 
                    {
                        oss << curr->value << " ";
                        curr = curr->forward[0];
                    }
                }
                return oss.str();
            }
    };

    SkipList::SkipList() : pImpl(new SkipListImpl()) {}
    SkipList::~SkipList() { delete pImpl; }

    SkipList::SkipList(const SkipList& other) : pImpl(new SkipListImpl())
    {
        pImpl->deepCopy(*(other.pImpl));
    }

    SkipList& SkipList::operator=(const SkipList& other)
    {
        if (this != &other)
        {
            pImpl->deepCopy(*(other.pImpl));
        }
        return *this;
    }

    SkipList& SkipList::operator+=(int value) { pImpl->insert(value); return *this; }
    SkipList& SkipList::operator-=(int value) { pImpl->remove(value); return *this; }

    SkipList& SkipList::operator%=(const std::pair<int, int>& edit)
    {
        pImpl->insert(edit.second);
        return *this;
    }

    void SkipList::operator!() { pImpl->clear(); }
    bool SkipList::operator[](int value) const { return pImpl->search(value); }

    bool SkipList::operator==(const SkipList& other) const { return pImpl->isEqual(*(other.pImpl)); }
    bool SkipList::operator<(const SkipList& other) const { return pImpl->isLess(*(other.pImpl)); }

    bool SkipList::operator!=(const SkipList& other) const { return !(*this == other); }
    bool SkipList::operator>(const SkipList& other) const { return other < *this; }
    bool SkipList::operator<=(const SkipList& other) const { return !(*this > other); }
    bool SkipList::operator>=(const SkipList& other) const { return !(*this < other); }

    std::string SkipList::toString() const { return pImpl->toString(); }

}