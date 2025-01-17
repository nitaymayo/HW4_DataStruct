//
// Created by nitaymayo on 1/12/25.
//

#ifndef HERD_H
#define HERD_H



class Herd {
private:
    int id;
    bool deleted;

    public:
    explicit Herd(int id): Herd(id, 0){};
    Herd(int id, int record): id(id), deleted(false) {};
    ~Herd() = default;
    int getId() const{
        return id;
    };
    // int getRecord() const {
    //     return record;
    // };
    // void incrementRecord() {
    //     record++;
    // }
    // void decrementRecord() {
    //     record--;
    // }
    void markDelete() {
        deleted = true;
    }
    bool Deleted() const {
        return deleted;
    }

    int key() const {
        return getId();
    };
};



#endif //HERD_H
