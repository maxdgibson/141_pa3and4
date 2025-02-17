//
//  BufferManager.hpp
//  Assignment2
//
//

#ifndef BufferManager_h
#define BufferManager_h
#include <iostream>

namespace ECE141   {

    template <typename T>
    class BufferManager {
    public:
        T* buffer_pntr;
        size_t buf_capacity;
        size_t cur_length;

        //finished default
        BufferManager() : buffer_pntr(nullptr), buf_capacity(0), cur_length(0) {}

        BufferManager(const BufferManager &original_buff) {
            if(original_buff.buffer_pntr) {
                buffer_pntr = new T[original_buff.buf_capacity]; //copyign capacity (size) over to new
                std::copy(original_buff.buffer_pntr, original_buff.buffer_pntr + original_buff.cur_length, buffer_pntr);
                //takes from buf_pntr to the end of length and puts into the new pntr

                buf_capacity = original_buff.buf_capacity;
                cur_length = original_buff.cur_length;
            } else{ //null buffer just need to construct normally
                buffer_pntr = nullptr;
                buf_capacity = 0;
                cur_length = 0;
            }
        }

        BufferManager(size_t aSize){ //is prefefined size like mf cur_length or capacity?
        //size is going to be capacity bc this is just creating an empty buffer that could hold aSize
            buf_capacity = aSize;
            cur_length = 0;
            if (aSize > 0) {
                buffer_pntr = new T[aSize]; //allocating space for type T of aSize
            } else {
                buffer_pntr = nullptr;  // incase aSize is zero
            }
        } //finish me - constructor with predefined size

        // Destructor
        ~BufferManager() {
            delete[] buffer_pntr;
        }

        //---- Add other ocf methods --- not sure what else i need i have constrc, copy constrc, destroyer,

        //---- Basic methods you must implement. Add more if you need to...
        size_t getCapacity() const{
            return buf_capacity;
        }
        T*     getBuffer() const{
            return buffer_pntr;
        }

        size_t willExpand(size_t aNewSize, size_t anOffset=0) {
            if (aNewSize > buf_capacity) { //as long as they are asking for more than we have
                T* new_buffer = new T[aNewSize]; // allocating new buffer
                if (buffer_pntr) {
                    std::copy(buffer_pntr, buffer_pntr + cur_length, new_buffer); // take old data to new buffer
                    delete[] buffer_pntr; // delete the old buffer
                }
                buffer_pntr = new_buffer;
                buf_capacity = aNewSize; // capacity to new size
            }
            return buf_capacity;
        }
        size_t willCompact(size_t aNewSize, size_t anOffset=0){
            if (aNewSize < buf_capacity) {
                T* new_buffer = new T[aNewSize]; // allocating new buffer
                size_t new_length = std::min(cur_length, aNewSize); // taking which ever is smaller
                if (buffer_pntr) {
                    std::copy(buffer_pntr, buffer_pntr + new_length, new_buffer); // take as much as new_length and thats it
                    delete[] buffer_pntr; // delete the old buffer
                }
                buffer_pntr = new_buffer;
                buf_capacity = aNewSize; // Update capacity
                cur_length = new_length; // new current length
            }
            return buf_capacity;
        }

        size_t getLength() const {
            return cur_length;  // Method to get the current length
        }

    };

} // namespace ECE141

#endif /* BufferManager_h */