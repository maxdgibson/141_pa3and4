//
//  String.hpp
//

#ifndef String_hpp
#define String_hpp

#include <iostream>
#include "BufferManager.hpp"
#include "Comparable.cpp"


namespace ECE141 {
    template<typename T=char, size_t aPresize=32>
    class String : public compare<String>;
            //need to create an instance where the string becomes the owner
    public:

        //add rest of the OCF methods...
        String(const char* aBuffer = nullptr) {
            if (aBuffer) {
                size_t len = std::strlen(aBuffer);
                bufferManager.willExpand(len + 1); // Allocate space for string + null terminator
                std::copy(aBuffer, aBuffer + len, bufferManager.getBuffer());
                bufferManager.getBuffer()[len] = '\0'; // Null-terminate
            }
        }
        String(const String& aString) : bufferManager(aString.bufferManager) {}
        String& operator=(const String& aCopy) {
            if (this != &aCopy) {
                bufferManager = aCopy.bufferManager;
            }
            return *this;
        }
        //add version to assign from const char*
        String& operator=(const char* aCStr) {
            if (aCStr) {
                size_t len = std::strlen(aCStr);
                bufferManager.willExpand(len + 1);
                std::copy(aCStr, aCStr + len, bufferManager.getBuffer()); //copyign chars
                bufferManager.cur_length = len; //update
                bufferManager.getBuffer()[len] = '\0'; //manual null
            } else {
                //aCStr is nullptr, assign an empty string
                bufferManager.willExpand(1);
                bufferManager.getBuffer()[0] = '\0';
                bufferManager.cur_length = 0;
            }
            return *this;
        }


        size_t size() const{
            return buffermanager.curLength;  // Return the current length of the buffer
        }

        //mutation methods...
        T& operator[](size_t pos){
            return bufferManager.getBuffer()[pos];
        }

        String  operator+(const String &aString){
            String result(*this);  //copy of the current string
            size_t newLength = result.size() + aString.size();
            if (result.bufferManager.getCapacity() < newLength + 1) { // Check capacity
                result.bufferManager.willExpand(newLength + 1);
            }
            std::copy(aString.getBuffer(), aString.getBuffer() + aString.size(), result.bufferManager.getBuffer() + result.size()); //fancy operation to do the addition usign std::copy
            result.bufferManager.cur_length = newLength;
            result.bufferManager.getBuffer()[newLength] = '\0'; // adding null terminator at end
            return result;
        }

        //add method to add const char*
        String operator+(const char* aCStr) const {
            String result(*this);  //copy of the current string
            size_t cStrLen = std::strlen(aCStr);
            size_t newLength = result.size() + cStrLen;
            if (result.bufferManager.getCapacity() < newLength + 1) { // Check capacity
                result.bufferManager.willExpand(newLength + 1); // Expand if needed
            }
            std::copy(aCStr, aCStr + cStrLen, result.bufferManager.getBuffer() + result.size()); //actual addition
            result.bufferManager.cur_length = newLength; //updating values
            result.bufferManager.getBuffer()[newLength] = '\0';
            return result;
        }
        //add method to support "hello"+theStringObject
        String operator+(const char* lhs, const String& rhs) {
            String result(lhs); // making a string from c style string
            return result + rhs; // Use existing code
        }

        String& operator+=(const String &aString){
            size_t oldLen = size();
            size_t newLen = oldLen + aString.size();
            bufferManager.willExpand(newLen + 1); //null terminator
            std::copy(aString.getBuffer(), aString.getBuffer() + aString.size(), bufferManager.getBuffer() + oldLen);
            bufferManager.cur_length = newLen; // Update val
            bufferManager.getBuffer()[newLen] = '\0'; //manual null terminator

            return *this;
        }
        //add method to append const char*
        //same exact code basically
        String& operator+=(const char* aCStr) {
            size_t oldLen = size();
            size_t addLen = std::strlen(aCStr);
            size_t newLen = oldLen + addLen;
            bufferManager.willExpand(newLen + 1);
            std::copy(aCStr, aCStr + addLen, bufferManager.getBuffer() + oldLen);
            bufferManager.cur_length = newLen;
            bufferManager.getBuffer()[newLen] = '\0';

            return *this;
        }

        String& insert(size_t anIndex, const String &aStr, size_t aStrIndex, size_t aStrCount){
            if (anIndex > size()) {
                throw std::out_of_range("Index out of bounds");
            }
            if (aStrIndex + aStrCount > aStr.size()) {
                aStrCount = aStr.size() - aStrIndex; // Adjust count to prevent overflow
            }
            size_t oldLen = size();
            size_t newLen = oldLen + aStrCount;
            bufferManager.willExpand(newLen + 1);
            std::copy_backward(bufferManager.getBuffer() + anIndex, bufferManager.getBuffer() + oldLen, bufferManager.getBuffer() + newLen); //shift to right
            std::copy(aStr.getBuffer() + aStrIndex, aStr.getBuffer() + aStrIndex + aStrCount, bufferManager.getBuffer() + anIndex); //copy insert into position
            bufferManager.cur_length = newLen; //update
            bufferManager.getBuffer()[newLen] = '\0';//manual null term

            return *this;
        }
        //add method to insert const char*
        String& insert(size_t anIndex, T aChar) {
            if (anIndex > size()) {
                throw std::out_of_range("Index out of bounds");
            }

            size_t oldLen = size();
            size_t newLen = oldLen + 1;
            bufferManager.willExpand(newLen + 1);
            /same thing here
            std::copy_backward(bufferManager.getBuffer() + anIndex, bufferManager.getBuffer() + oldLen, bufferManager.getBuffer() + newLen);
            //insertion
            bufferManager.getBuffer()[anIndex] = aChar;
            bufferManager.cur_length = newLen; //same
            bufferManager.getBuffer()[newLen] = '\0'; //same

            return *this;
        }
        //part of me thinks this replace method should just be a delete and a new constructor
        //looks like the maxcopylen is the most we are going to replace with
        String& replace(size_t anIndex, size_t aMaxCopyLen, const String &aString) {
            if (anIndex > size()) {
                throw std::out_of_range("Index out of bounds");
            }
            size_t replaceLen = std::min(aMaxCopyLen, aString.size()); //usign min to figure out which one effects
            size_t endReplaceIndex = anIndex + replaceLen;
            //adjust buffer size if necessary:
            size_t newLen = std::max(size(), endReplaceIndex);
            bufferManager.willExpand(newLen + 1); // +1 for null terminator
            std::copy(aString.getBuffer(), aString.getBuffer() + replaceLen, bufferManager.getBuffer() + anIndex);
            if (newLen > size()) {
                bufferManager.cur_length = newLen;
            }
            bufferManager.getBuffer()[bufferManager.cur_length] = '\0';
            return *this;
        }
        //add method to replace const char*
        String& replace(size_t anIndex, size_t aMaxCopyLen, const char* aCStr) {
            if (!aCStr) return *this;  // nullptr check
            if (anIndex > size()) {
                throw std::out_of_range("Index out of bounds");
            }

            size_t inputLen = std::strlen(aCStr);
            size_t replaceLen = std::min(aMaxCopyLen, inputLen); //find min effect
            size_t endReplaceIndex = anIndex + replaceLen;
            //adjust buff if needed
            size_t newLen = std::max(size(), endReplaceIndex);
            bufferManager.willExpand(newLen + 1);
            // repacement
            std::copy(aCStr, aCStr + replaceLen, bufferManager.getBuffer() + anIndex);

            if (newLen > size()) {
                bufferManager.cur_length = newLen;
            }
            bufferManager.getBuffer()[bufferManager.cur_length] = '\0';

            return *this;
        }


    String& erase(size_t anIndex, size_t aCount){
        size_t currentLength = size();
        if (anIndex >= currentLength) {
            throw std::out_of_range("Index out of bounds"); //shuld i just cout here?
        }

        if (aCount == 0) {
            return *this; //nothign to erase
        }

        size_t numToErase = std::min(aCount, currentLength - anIndex); // numbers of chars to erase
        size_t newLength = currentLength - numToErase; //quick math

        // Move characters down to overwrite the erased section
        std::copy(bufferManager.getBuffer() + anIndex + numToErase, bufferManager.getBuffer() + currentLength,
                  bufferManager.getBuffer() + anIndex);

        bufferManager.cur_length = newLength; // Update the length of the string
        bufferManager.getBuffer()[newLength] = '\0'; // Null terminate the string

        return *this;
        }



        //Comparision methods...
        int compare( const String& aString ) const{}
        //add method to add compare const char*

        T* getBuffer() const{}

    //String.comparable.idk()
    using template typname<J>
    int compare( J b){
            //returning 0 if equal, 1 if less than, -1 if cant be evaluate6d?
            //check if its string vs string
            //check if its string vs char *
            //check if its string vs some arithmetic?

        }

    //general string comparisons
    bool operator==(const String &aString) const {return compare(aString) == 0;}
    bool operator<(const String &aString) const {return compare(aString) < 0;



        //Search...
        int find( const String &aString, size_t anIndex = 0 ){}

        friend std::ostream& operator << (std::ostream &anOut, const String &aStr){}
        friend std::istream& operator >> (std::istream &anOut,  String &aString){}

    protected:
        // Add your member variables here

    };

} // namespace ECE141

#endif /* String_hpp */