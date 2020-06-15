#ifndef MYLIST_H
#define MYLIST_H

#include <iostream>

template <class T>
struct Node {
        T data;
        Node<T> *next;
        ~Node() {
            // if(sizeof(data) == sizeof(void*))
            //     delete data;
        }
};


template <class T>
class myList {
    private:
        Node<T> *head;
    
    public:
        myList() {
            head = NULL;
        }

        void insert(T data) {
            if(head == NULL) {
                head = new Node<T>;
                head->data = data;
                head->next = NULL;
            }
            else {
                Node<T> *temp = head;
                while(temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = new Node<T>;
                temp->next->data = data;
                temp->next->next = NULL;
            }
        }

        Node<T>* initList() {
            return head;
        }

        ~myList() {
            while(head != NULL) {
                Node<T> *tmp=head;
                head = head->next;
                delete tmp;
            }
            delete head;
        }
};

#endif