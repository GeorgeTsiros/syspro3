#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <iostream>
#include <cstdlib>
#include <assert.h>


//Here we have a partial custom template of vector
template <class T>
class MyVector {
    private:
        T* myarr;
        int vec_size;
    public:
        MyVector() {
            myarr = NULL;
            vec_size = 0;
        }

        void add(T data) {
            myarr = (T*)realloc(myarr,(vec_size+1) * sizeof(T));
            myarr[vec_size] = data;
            vec_size++;
        }

        int size() const{
            return this->vec_size;
        }
        

        T& operator[] (const int i) {
            assert(i<vec_size && i>=0);
            return this->myarr[i];
        }

        void clear() {
            for(int i=0; i<this->vec_size; i++) 
                if(sizeof(this->myarr[i]) == sizeof(void*))
                    delete this->myarr[i];
            this->vec_size = 0;
        }

        void remove(const int i) {
           assert(i < this->vec_size && i>=0);
           if(this->vec_size != 1 && i != this->vec_size - 1) {
               for(int j=i; j < this->vec_size - 1; j++) {
                   this->myarr[j] = this->myarr[j+1];
               }
           }
           this->vec_size--;        
        }

        void erase(const int i) {
            assert(i < this->vec_size && i>=0);
            if(this->vec_size != 1 && i != this->vec_size - 1) {
                for(int j=i; j < this->vec_size - 1; j++) {
                    if(sizeof(this->myarr[j]) == sizeof(void*)) 
                            delete this->myarr[j];   
                    this->myarr[j] = this->myarr[j+1];
                }
            }
            else if(this->vec_size == 1) {
                if(sizeof(this->myarr[0]) == sizeof(void*))
                        delete this->myarr[0];
            }  
            else {
                    if(sizeof(this->myarr[this->vec_size - 1]) == sizeof(void*))
                        delete this->myarr[this->vec_size - 1];
            }
            this->vec_size--;        
        }

        ~MyVector() {
            free(this->myarr);
        }


};

#endif
