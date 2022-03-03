#include <iostream>

///I couldn't use my linked list class in the main because i could figure out how to sort!

template<class T>
class MyList {

public:
    struct node {
        T data; //holds object
        node* next;//pointer to next object
    };

    MyList(){
        head = nullptr;
        current = nullptr;
        temp = nullptr;
        tail = nullptr;
    }

    //deletes all variables to free up space and avoid memory leaks
    ~MyList(){
        clear();
    }

    bool isEmpty(){
        return head == nullptr;
    }

    T back(){
        return tail->data;
    }

    T front(){
        return head->data;
    }

    unsigned int size(){
        if(m_size<0)
            return 0;
        return m_size;
    }

    //called when first element is inserted
    void insertFirst(T object){
        temp = new node;
        temp->data = object;
        temp->next = nullptr;
        head = temp;
        tail = temp;
        tail->next = nullptr;
        ++m_size;
    }

    void push_back(T object){
        if (isEmpty()){
            insertFirst(object);
            return;
        }

        temp = new node;
        temp->data = object;
        temp->next = nullptr;
        tail->next = temp;
        tail = temp;
        tail->next = nullptr;
        ++m_size;
    }

    //finds object and deletes it from list
    void remove(T object){
        if(isEmpty())
            return;
        //temp is object before current
        current = head;
        while (current != nullptr){
            if(object == current->data){
                if(current == head){
                    head = head->next;
                    delete current;
                    current = nullptr;
                    --m_size;
                    return;
                } else if(current == tail){
                    tail = temp;
                    tail->next = nullptr;
                    delete current;
                    current = nullptr;
                    --m_size;
                    return;
                } else {
                    temp->next = current->next;
                    delete current;
                    current = nullptr;
                    --m_size;
                    return;
                }
            }
            temp = current;
            current = current->next;
        }
        std::cout<<"Object "<<object<<" could not be found!"<<std::endl;
    }

    //deletes everything in the list
    void clear(){
        current = head;
        while(current != nullptr){
            temp = current;
            current = current->next;
            delete temp;
            temp = nullptr;
        }
        delete head;
        delete tail;
        delete temp;
        delete current;
        head = nullptr;
        m_size = 0;
    }

    //prints list
    void print(){
        if(isEmpty())
            return;
        current = head;
        while (current != nullptr){
            std::cout << current->data << std::endl;
            current = current->next;
        }
    }

    //swap a and b objects
    void swap (node* a, node* b){
        T t = a->data;
        a->data = b->data;
        b->data = t;
        delete t;
    }

private:
    node* head = nullptr;
    node* current = nullptr;
    node* temp = nullptr;
    node* tail = nullptr;
    size_t m_size = 0;
};