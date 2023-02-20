//
// Created by LENOVO on 03.10.2022.
//

#ifndef TOF_ISAVER_H
#define TOF_ISAVER_H

class ISaver{
public:
    virtual int save() = 0;

    virtual ~ISaver() = default;
};

#endif //TOF_ISAVER_H
