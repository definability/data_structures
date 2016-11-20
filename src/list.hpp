#ifndef LIST_HPP
#define LIST_HPP

template<typename T> class List {
    private:
        T value;
        const List<T>* next;
    public:
        List(const T& value, const List<T>* next = nullptr)
                : value(value), next(next) {
        }
        List(const List<T>&) = delete;

        bool operator==(const List<T>& list) const {
            return !(!this->next ^ !list.next)
                && (this->value == list.value)
                && (this->next == list.next || *(this->next) == *(list.next));
        }

        ~List() {
            delete this->next;
        }
};

#endif
