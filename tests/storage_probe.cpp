#include "ops.h"
#include <iostream>
int main(int argc,char**argv){
    QCoreApplication app(argc,argv);
    auto bank=std::make_shared<spng::Bank>();
    bank->createStage("test",5,QSize(64,48),CV_8U,"L");
    bank->frame("test",0).setTo(12);
    std::cout<<"ready"<<std::endl;
    std::string line;std::getline(std::cin,line);
    return 0;
}
