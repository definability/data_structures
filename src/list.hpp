#ifndef LIST_HPP
#define LIST_HPP

template<typename T> class List {
    private:
        T value;
        const List<T>* next;
    public:
        List() {
            this->value = 0;
            this->next = 0;
        }
        List(const List<T>& list) {
            *(this) = list;
        }
        List(const T& value, const List<T>* next = 0) {
            this->value = value;
            this->next = next;
        }

        bool operator==(const List<T>& list) const {
            return !(!this->next ^ !list.next)
                && (this->value == list.value)
                && (this->next == list.next || *(this->next) == *(list.next));
        }

        ~List() {
        }
};

#endif
