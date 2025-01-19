// //
// // Created by nitaymayo on 1/10/25.
// //
//
// #ifndef DYNAMICARRAY_H
// #define DYNAMICARRAY_H
// #include <memory>
// #include <stdexcept>
//
// #define DEFAULT_CAPACITY 16
//
// template<class T>
// class DynamicArray {
//   private:
//     std::unique_ptr<T[]> data;
//     int size;
//     int capacity;
//
//     void multiplyArray(){
//       std::unique_ptr<T[]> newData = std::unique_ptr<T[]>(new T[capacity * 2]);
//       // copy data from old array to newData
//       for(int i = 0; i < size; i++){
//         newData[i] = data[i];
//       }
//       // overide the old array
//       data = std::move(newData);
//       capacity *= 2;
//     }
//     public:
//
//       DynamicArray<T>(): data(std::make_unique<T>(DEFAULT_CAPACITY)), size(0), capacity(DEFAULT_CAPACITY) {
//            for (int i = 0; i < DEFAULT_CAPACITY; i++) {
//              data[i] = nullptr;
//            }
//       };
//
//       DynamicArray<T>(int capacity): data(std::make_unique<T>(capacity)), size(0), capacity(capacity) {
//            for (int i = 0; i < DEFAULT_CAPACITY; i++) {
//              data[i] = nullptr;
//            }
//       };
//
//       ~DynamicArray(){
//         data.reset();
//       }
//
//       int insert(const T& item){
//         if(size >= capacity){
//           multiplyArray();
//         }
//         data[size++] = item;
//         return size;
//       }
//
//       T& operator[](int index) {
//         if (index < 0 || index >= size) {
//           throw std::logic_error("index out of range");
//         }
//         return data[index];
//       }
//
//       const T& operator[](int index) const {
//         return data[index];
//       }
//
//
//
// };
//
//
//
// #endif //DYNAMICARRAY_H
