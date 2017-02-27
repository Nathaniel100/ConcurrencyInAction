//
// Created by 吴凡 on 2017/2/24.
//

#ifndef CONCURRENCYINACTION_NOCOPYABLE_H
#define CONCURRENCYINACTION_NOCOPYABLE_H

class nocopyable {
protected:
    nocopyable(){};
    ~nocopyable(){};

private:
    nocopyable(const nocopyable&) = delete;
    nocopyable&operator=(const nocopyable) = delete;
};

#endif //CONCURRENCYINACTION_NOCOPYABLE_H
