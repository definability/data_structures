#ifndef LIST_HPP
#define LIST_HPP

template<typename T> class List {
    public:
        List() {
        }
        List(const List<T>& list) {
            *(this) = list;
        }
        ~List() {
        }
};

#endif
