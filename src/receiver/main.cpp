/* my own header */
#include "resender.h"

static void thread1(resender *a){
    if(a->receiver()){
        delete a;
        exit(1);
    }
}

int main(){
    resender *ctrl = new resender();
    if(ctrl->read_config()){
        delete ctrl;
        return 1;
    }
    std::thread thr1(thread1, ctrl);
    ctrl->controller();
    thr1.join();

    delete ctrl;
    
    return 0;
}
