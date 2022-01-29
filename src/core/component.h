#ifndef ENGINEPROTOTYPE_COMPONENT
#define ENGINEPROTOTYPE_COMPONENT

#include <string>
#include <functional>

class Component {
    public:
        static int NEXT_COMPONENT; 

        int id; 
        std::string name;
        std::function<void()> debug; 

        Component(); 
};

#endif