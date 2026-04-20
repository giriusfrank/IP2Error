#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <string>
#include <stdexcept>
#include <utility>

namespace CustomADT
{

    class DuplicateValueException : public std::logic_error
    {
        public:
            explicit DuplicateValueException(const std::string& message);
    };

    class SkipList
    {
        private:
            class SkipListImpl;
            SkipListImpl* pImpl;

        public:
            SkipList();
            ~SkipList();
            
            SkipList(const SkipList& other);
            SkipList& operator=(const SkipList& other);

            SkipList& operator+=(int value);
            SkipList& operator-=(int value);
            SkipList& operator%=(const std::pair<int, int>& edit);

            void operator!();
            bool operator[](int value) const;

            bool operator==(const SkipList& other) const;
            bool operator!=(const SkipList& other) const;
            bool operator<(const SkipList& other) const;
            bool operator<=(const SkipList& other) const;
            bool operator>(const SkipList& other) const;
            bool operator>=(const SkipList& other) const;

            std::string toString() const;
    };
}

#endif