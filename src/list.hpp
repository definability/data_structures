#ifndef LIST_HPP
#define LIST_HPP

template<typename T> class List {
    private:
        T value;
        List<T>* next;
    public:
        List() {
            this->value = 0;
            this->next = 0;
        }
        List(const List<T>& list) {
            *(this) = list;
        }
        ~List() {
        }
};

#endif
