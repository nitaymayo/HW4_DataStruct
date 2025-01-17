//
// Created by nitaymayo on 1/12/25.
//

#ifndef HERD_H
#define HERD_H



class Herd {
private:
    int id;
    int record;


    public:
    Herd(int id): Herd(id, 0){};
    Herd(int id, int record): id(id), record(record) {};
    ~Herd() = default;
    int getId() const{
        return id;
    };
    int getRecord() const {
        return record;
    };
    void incrementRecord() {
        record++;
    }

    int key() const {
        return getId();
    };
};



#endif //HERD_H
